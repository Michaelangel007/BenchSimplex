/*
GLSL -> C++ Wrapper

References:
    http://www.fractalforums.com/programming/compilerun-glsl-shader-as-cplusplus/
    https://code.google.com/p/boxplorer2/source/browse/trunk/glsl.h?spec=svn82&r=82
*/

// This fails to compile "vec3 floor() { ... }" unless <math.h> is included ?!
#include <math.h>     // floor()

struct vec2
{
    float x;
    float y;

    vec2( float _x, float _y ) {
        x = _x;
        y = _y;
    }

//    vec4 xyxy() {
//        return vec4( x, y, x, y );
//    }
};

struct vec3
{
    union {
        float r;
        float x;
    };
    union {
        float g;
        float y;
    };
    union {
        float b;
        float z;
    };

    vec3( float scaler ) {
        x = y = z = scaler;
    }

    vec3( const float _x, const float _y, const float _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }

    vec3( const vec3& copy ) {
        x = copy.x;
        y = copy.y;
        z = copy.z;
    }

    friend vec3 operator + ( const vec3 & a, const vec3 & b ) {
       return vec3( a.x + b.x, a.y + b.y, a.z + b.z );
    }

    friend vec3 operator - ( const vec3 & a, const vec3 & b ) {
       return vec3( a.x - b.x, a.y - b.y, a.z - b.z );
    }

    vec3& operator -= ( const vec3 & v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
       return *this;
    }

    vec3& operator * ( const float scale )
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    vec3& operator *= ( const float scale )
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    friend vec3 operator * ( const vec3 & a, const vec3 & b ) {
       return vec3( a.x * b.x, a.y * b.y, a.z * b.z );
    }

    friend vec3 operator / ( const float a, const vec3& b ) {
        return vec3( a / b.x, a / b.y, a / b.z );
    }

    vec2 xy() const {
        return vec2( x, y );
    }

    vec3& xyz() {
        return *this;
    }

    vec3 yzx() const {
        return vec3( y, z, x );
    }

    vec3 zxy() const {
        return vec3( z, x, y );
    }

    vec3 zzz() const {
        return vec3( z, z, z );
    }
};

struct vec4
{
    union {
        float r;
        float x;
    };
    union {
        float g;
        float y;
    };
    union {
        float b;
        float z;
    };
    union {
        float a;
        float w;
    };

    vec4( float _x, float _y, float _z, float _w ) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    vec4( vec2 a, vec2 b )
    {
        x = a.x;
        y = a.y;

        z = b.x;
        w = b.y;
    }

    vec4( float _x, vec2 yz, float _w ) {
        x = _x;
        y = yz.x;
        z = yz.y;
        w = _w;
    }

    vec2 xy() {
        return vec2( x, y );
    }

    vec2 xz() {
        return vec2( x, z );
    }

    vec2 yw() {
        return vec2( y, w );
    }

    vec4 xyxy() {
        return vec4( x, y, x, y );
    }

    vec4 zwzw() {
        return vec4( z, w, z, w );
    }

    friend vec4 operator + ( const vec4 & a, const vec4 & b ) {
        return vec4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w );
    }

    friend vec4 operator - ( const vec4 & a, const float & s ) {
        return vec4( a.x - s, a.y - s, a.z - s, a.w - s );
    }

    friend vec4 operator - ( const float & s, const vec4 & b ) {
        return vec4( s - b.x, s - b.y, s - b.z, s - b.w );
    }

    friend vec4 operator * ( const vec4 & a, const vec4 & b ) {
        return vec4( a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w );
    }

    vec4& operator *= ( const vec4 & v ) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }
};

// https://code.google.com/p/boxplorer2/source/browse/trunk/glsl.h?spec=svn82&r=82
inline
float clamp( const float x, const float min, const float max )
{ 
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline
vec3 clamp( const vec3& v, float min, float max )
{
    return vec3(
        clamp( v.x, min, max),
        clamp( v.y, min, max),
        clamp( v.z, max, max)
    );
}

//float dot( const vec3 & v )
//{
//    return v.x*v.x + v.y*v.y + v.z*v.z;
//}

inline
float dot( const vec3 &a, const vec3 &b )
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline
float dot( const vec4 &a, const vec4 &b )
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline
vec3 floor( const vec3 & v ) {
    return vec3( floor( v.x ), floor( v.y ), floor( v.z ) );
}

// https://www.opengl.org/sdk/docs/man/html/fract.xhtml
inline
float fract( const float x )
{
    return (x - floor( x ));
}

inline
vec4 fract( const vec4& v )
{
    return vec4(
        fract( v.x ),
        fract( v.y ),
        fract( v.z ),
        fract( v.w )
    );
}

inline
vec3 max( const vec3 & a, const vec3 & b )
{
    return vec3(
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z
    );
}

inline
vec4 max( const vec4 & a, const float s )
{
    return vec4(
        a.x > s ? a.x : s,
        a.y > s ? a.y : s,
        a.z > s ? a.z : s,
        a.w > s ? a.w : s
    );
}

inline
vec3 min( const vec3 & a, const vec3 & b )
{
    return vec3(
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z
    );
}

inline
vec3 mix(const vec3& a, const vec3&b, const float t)
{
    float s = 1.0f - t;
    return vec3( a.x*s + b.x*t, a.y*s + b.y*t, a.z*s + b.z*t );
}

inline
vec4 mix(const vec4& a, const vec4&b, const float t)
{
    float s = 1.0f - t;
    return vec4( a.x*s + b.x*t, a.y*s + b.y*t, a.z*s + b.z*t, a.w*s + b.w*t );
}

inline
vec4 mix(const vec4& start, const vec4& end, const vec4& percent)
{
    vec4 remain( 1.f - percent.x, 1.f - percent.y, 1.f - percent.z, 1.f - percent.w );
    return vec4( start.x*remain.x + end.x*percent.x, start.y*remain.y + end.y*percent.y, start.z*remain.z + end.z*percent.z, start.w*remain.w + end.w*percent.w );
}

inline
float mod(float a, float b) { return a - b*floor(a/b); }

inline
vec3  mod(const vec3& a, float b) { return vec3(mod(a.x, b), mod(a.y, b), mod(a.z, b)); }

inline
float step( const float edge, const float value )
{
    return (value < edge) ? 0.f : 1.f;
}

inline
vec3 step( const vec3 &edge, const vec3& value )
{
    return vec3( 
        step( edge.x, value.x ),
        step( edge.y, value.y ),
        step( edge.z, value.z )
    );
}

// reciprocal square root
// If your CPU doesn't have a broken FPU: return (1.0f / sqrt(x));
inline
float rsqrtf(float f)
{
    float half = 0.5f*f;
    int i = *(int*)&f;
    // 0x5f375a86 Lomont 2003, Robertson 2012, minmax error 0.00175122 http://www.geometrictools.com/Documentation/FastInverseSqrt.pdf
    // 0x5f3759df Quake 3 not quite as accurate
    // 0x5f37642f Lomont 2003 worse after 1 iteration   http://www.slideshare.net/maksym_zavershynskyi/fast-inverse-square-root
    // 0x1fbd1df5 Alternate: i = 0x1fbd1df5 + (i >> 1); http://h14s.p5r.org/2012/09/0x5f3759df.html
    i = 0x5f375a86 - (i >> 1); 
    f = *(float*)&i;
    f = f*(1.5f - half*f*f); // still 3x to 4x faster then x86
//	f = f*(1.5f - half*f*f); // optional further precision: max relative error = 0.000047%
    return f;
}

inline
vec4 inversesqrt( const vec4 & v )
{
    return vec4( rsqrtf( v.x ), rsqrtf( v.y ), rsqrtf( v.z ), rsqrtf( v.w ) );
}

#define xx xx()
#define xy xy()
#define xz xz()
#define xw xw()
#define yx yx()
#define yy yy()
#define yz yz()
#define yw yw()
#define zx zx()
#define zy zy()
#define zz zz()
#define zw zw()
#define wx wx()
#define wy wy()
#define wz wz()
#define ww ww()

#define xyz xyz()
#define xzy xzy()
#define zyx zyx()

#define yzx yzx()
#define yxz yxz()
#define yxx yxx()

#define xyx xyx()
#define xxy xxy()
#define zzz zzz()
#define zxy zxy()

#define xyxy xyxy()
#define zwzw zwzw()

