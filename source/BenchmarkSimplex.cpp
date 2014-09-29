/*
 * OpenSimplex (Simplectic) Noise Test in C++
 * Original Java version 
 * Converted to C by Arthur Tombs * Modified 2014-09-22
 * Cleaned up version by Michaelangel007 * Removed PNG & C++ bloat, added glsl and reference simplex
 *
 * This file is intended to test the function of OpenSimplexNoise.hh.
 *
 * Compile with:
 *   g++ -o BenchmarkSimplex -O2 OpenSimplexNoiseTest.cc
 *
 * Additional optimization can be obtained with:
 * 
 *    GCC: -Ofast (at the cost of accuracy)
 *         -msse4 (or the highest level of SSE your CPU supports).
 *
 *    MSVC: /fp:fast /arch:SSE2
 */
#define SIMPLEX_SHADER 1

#ifdef _MSC_VER // Shutup Microsoft Visual C++ warnings about fopen()
    #define _CRT_SECURE_NO_WARNINGS
#else
    #include <sys/time.h> // gettimeofday() // Linux, OSX but NOT on Windows
#endif

#include <stdio.h>    // fopen()
//#include <stdlib.h> // rand(), srand() // Optimization: Removed useless randomization
#include <stdint.h>   // uint8_t
#include <string.h>   // memset()
#include <stdint.h>   // int8_t
#include <time.h>     // time()
#include <math.h>     // floor()

#include "OpenSimplexNoise.hh"
#include "SimplexNoise1234.h"
#if SIMPLEX_SHADER
    #include "glsl_to_cpp.h"
    #include "SimplexPerlin3D.glsl" // See note about <math.h> and Windows.h -> WinDef.h min() max()
#endif
#include "PerlinNoise1234.h"

#include "util_targa.h"
#include "util_timer.h"

const int   WIDTH        = 512;
const int   HEIGHT       = 512;
const float FEATURE_SIZE = 24.0f;

// calls Noise() WIDTH*HEIGHT number of times
void make_open( OpenSimplexNoise & simplex, float *values )
{
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float y = (-0.5f + yi / (float)(HEIGHT-1)) * (HEIGHT / FEATURE_SIZE);
        for (int xi = 0; xi < WIDTH; xi++)
        {
            float x = (-0.5f + xi / (float)(WIDTH-1)) * (WIDTH / FEATURE_SIZE);
            values[ xi+yi*WIDTH ] = simplex.eval( x, y, 0.0f );
        }
    }
}

void make_ref( SimplexNoise1234 & simplex, float *values )
{
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float y = (-0.5f + yi / (float)(HEIGHT-1)) * (HEIGHT / FEATURE_SIZE);
        for (int xi = 0; xi < WIDTH; xi++)
        {
            float x = (-0.5f + xi / (float)(WIDTH-1)) * (WIDTH / FEATURE_SIZE);
            values[ xi+yi*WIDTH ] = simplex.noise( x, y, 0.0f );
        }
    }
}

#if SIMPLEX_SHADER
void make_shader( float *values )
{
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float y = (-0.5f + yi / (float)(HEIGHT-1)) * (HEIGHT / FEATURE_SIZE);
        for (int xi = 0; xi < WIDTH; xi++)
        {
            float x = (-0.5f + xi / (float)(WIDTH-1)) * (WIDTH / FEATURE_SIZE);
            values[ xi+yi*WIDTH ] = SimplexPerlin3D( x, y, 0.0f );
        }
    }
}
#endif

void quantize( float *values, uint8_t *texels )
{
    float   *pSrc = values;
    uint8_t *pDst = texels;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int8_t i =  (uint8_t) floor( ((*pSrc++ * 0.5f) + 0.5f) * 255.0f ); // BUGFIX: remove +0.5 bias so final 0 .. 255
            *pDst++ = i; // r
            *pDst++ = i; // g
            *pDst++ = i; // b
        }
    }
}


void DefaultOpenSimplex( float *values, uint8_t *texels )
{
    // Default Seed
    OpenSimplexNoise noise;
    make_open( noise , values ); // generate float array
    quantize ( values, texels ); // convert to 24-bit grayscale
    Targa_Save( "opensimplex_noise_default.tga", WIDTH, HEIGHT, texels, 24 );
}

void BenchmarkOpenSimplex( bool bSaveNoise, float *values, uint8_t *texels )
{
    Timer timer;
    timer.Start();
    uint64_t samples = 0;

        for( uint32_t seed = 0; seed < 256; seed++ )
        {
            if( bSaveNoise )
                printf( "Seed: %d\n", seed );

            OpenSimplexNoise noise( seed );

            make_open( noise , values ); // generate float array
            quantize ( values, texels ); // convert to 24-bit grayscale

            if( bSaveNoise )
            {
                char filename[ 64 ];
                sprintf( filename, "opensimplex_noise_seed_%d.tga", seed );
                Targa_Save( filename, WIDTH, HEIGHT, texels, 24 );
            }

            samples += (WIDTH * HEIGHT);
        }

        timer.Stop();
        timer.Throughput( samples );

    // Throughput = Total samples / Time
    printf( "Open Simplex: %.f seconds = %d:%d, %2d %cnoise/s\n"
//       , (uint32_t)samples
        , timer.elapsed
        , timer.mins
        , timer.secs
        , (uint32_t)timer.throughput.per_sec
        , timer.throughput.prefix
    );
}


void DefaultReferenceSimplex( float *values, uint8_t *texels )
{
    SimplexNoise1234 noise;
    make_ref( noise , values ); // generate float array
    quantize ( values, texels ); // convert to 24-bit grayscale
    Targa_Save( "refsimplex_noise_default.tga", WIDTH, HEIGHT, texels, 24 );
}

void BenchmarkReferenceSimplex( bool bSaveNoise, float *values, uint8_t *texels )
{
    Timer timer;
    timer.Start();
    uint64_t samples = 0;

        for( uint32_t seed = 0; seed < 256; seed++ )
        {
            if( bSaveNoise )
                printf( "Seed: %d\n", seed );

            SimplexNoise1234 simplex; // doesn't support a permutation seed

            make_ref( simplex , values ); // generate float array
            quantize( values, texels ); // convert to 24-bit grayscale

            if( bSaveNoise )
            {
                char filename[ 64 ];
                sprintf( filename, "refsimplex_noise_seed_%d.tga", seed );
                Targa_Save( filename, WIDTH, HEIGHT, texels, 24 );
            }

            samples += (WIDTH * HEIGHT);
        }

    timer.Stop();
    timer.Throughput( samples );

    // Throughput = Total samples / Time
    printf( "Ref. Simplex: %.f seconds = %d:%d, %2d %cnoise/s\n"
//        , (uint32_t)samples
        , timer.elapsed
        , timer.mins
        , timer.secs
        , (uint32_t)timer.throughput.per_sec
        , timer.throughput.prefix
    );
}

#if SIMPLEX_SHADER
void DefaultShaderSimplex( float *values, uint8_t *texels )
{
    make_shader( values ); // generate float array
    quantize   ( values, texels ); // convert to 24-bit grayscale
    Targa_Save( "glslsimplex_noise_default.tga", WIDTH, HEIGHT, texels, 24 );
}

void BenchmarkShaderSimplex(  bool bSaveNoise, float *values, uint8_t *texels )
{
    Timer timer;
    timer.Start();
    uint64_t samples = 0;

        for( uint32_t seed = 0; seed < 256; seed++ )
        {
            if( bSaveNoise )
                printf( "Seed: %d\n", seed );

            make_shader( values ); // generate float array
            quantize   ( values , texels ); // convert to 24-bit grayscale

            samples += (WIDTH * HEIGHT);
        }

    timer.Stop();
    timer.Throughput( samples );

    // Throughput = Total samples / Time
    printf( "GLSL Simplex: %.f seconds = %d:%d, %2d %cnoise/s\n"
//        , (uint32_t)samples
        , timer.elapsed
        , timer.mins
        , timer.secs
        , (uint32_t)timer.throughput.per_sec
        , timer.throughput.prefix
    );
}
#endif

/*
Microsoft VS 2010
Properties, Code Generation, 
    Floating Point Model: /fp:fast
    Enable Enhanced Instruction Set: /arch:sse2

Samples: 67108864

Mnoise/s      w/ sse2   fp:fast    w/o sse2   w/o fp:fast
Open Simplex: 12         9                     6
Ref. Simplex: 21        16                    10
GLSL Simplex:  8         5                     4
*/
int main( int nArg, char *aArg[] )
{

    float   *values = new float   [WIDTH * HEIGHT      ]; // intensity
    uint8_t *texels = new uint8_t [ WIDTH * HEIGHT * 3 ]; // rgb

    // Open Simplex Seed specified
    bool bBenchmark = (nArg > 1); // TODO: -bench
    bool bSaveNoise = (nArg > 2); // TODO: -save

bBenchmark = 1;

    DefaultOpenSimplex     ( values, texels );
    DefaultReferenceSimplex( values, texels );
#if SIMPLEX_SHADER
    DefaultShaderSimplex   ( values, texels );
#endif

    if( bBenchmark )
    {
        BenchmarkOpenSimplex     ( bSaveNoise, values, texels );
        BenchmarkReferenceSimplex( bSaveNoise, values, texels );
#if SIMPLEX_SHADER
        BenchmarkShaderSimplex   ( bSaveNoise, values, texels );
#endif

        // Repeat the benchmark but in reverse
#if SIMPLEX_SHADER
        BenchmarkShaderSimplex   ( bSaveNoise, values, texels );
#endif
        BenchmarkReferenceSimplex( bSaveNoise, values, texels );
        BenchmarkOpenSimplex     ( bSaveNoise, values, texels );
    }

#if _WIN32
    getchar();
#endif

    delete [] texels;
    delete [] values;

    return 0;
}
