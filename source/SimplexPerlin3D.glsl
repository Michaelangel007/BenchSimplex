//  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin3D.glsl

float SimplexPerlin3D( float x, float y, float z ) // vec3 P )
{
    vec3 P( x, y, z );

    //  simplex math constants
    const float SKEWFACTOR = 1.0f/3.0f;
    const float UNSKEWFACTOR = 1.0f/6.0f;
    const float SIMPLEX_CORNER_POS = 0.5f;
    const float SIMPLEX_TETRAHADRON_HEIGHT = 0.70710678118654752440084436210485f;    // sqrt( 0.5 )

    //  establish our grid cell.
    P *= SIMPLEX_TETRAHADRON_HEIGHT;    // scale space so we can have an approx feature size of 1.0
    vec3 Pi = floor( P + dot( P, vec3( SKEWFACTOR) ) );

    //  Find the vectors to the corners of our simplex tetrahedron
    vec3 x0 = P - Pi + dot(Pi, vec3( UNSKEWFACTOR ) );
    vec3 g = step( x0.yzx, x0 ); // x0.xyz
    vec3 l = 1.0 - g;
    vec3 Pi_1 = min( g, l.zxy ); // g.xyz
    vec3 Pi_2 = max( g, l.zxy ); // g.xyz
    vec3 x1 = x0 - Pi_1 + UNSKEWFACTOR;
    vec3 x2 = x0 - Pi_2 + SKEWFACTOR;
    vec3 x3 = x0 - SIMPLEX_CORNER_POS;

    //  pack them into a parallel-friendly arrangement
    vec4 v1234_x = vec4( x0.x, x1.x, x2.x, x3.x );
    vec4 v1234_y = vec4( x0.y, x1.y, x2.y, x3.y );
    vec4 v1234_z = vec4( x0.z, x1.z, x2.z, x3.z );

    // clamp the domain of our grid cell
    //Pi.xyz = Pi.xyz - floor(Pi.xyz * ( 1.0 / 69.0 )) * 69.0;
    Pi -= floor(Pi * ( 1.0f / 69.0f )) * 69.0f;
    vec3 Pi_inc1 = step( Pi, vec3( 69.0f - 1.5f ) ) * ( Pi + 1.0f );

    //	generate the random vectors
    vec4 Pt = vec4( Pi.xy, Pi_inc1.xy ) + vec4( 50.0f, 161.f, 50.f, 161.f ); // vec2( 50.0, 161.0 ).xyxy;
    Pt *= Pt;
    vec4 V1xy_V2xy = mix( Pt.xyxy, Pt.zwzw, vec4( Pi_1.xy, Pi_2.xy ) );
    Pt = vec4( Pt.x, V1xy_V2xy.xz, Pt.z ) * vec4( Pt.y, V1xy_V2xy.yw, Pt.w );
    const vec3 SOMELARGEFLOATS = vec3( 635.298681f, 682.357502f, 668.926525f );
    const vec3 ZINC = vec3( 48.500388f, 65.294118f, 63.934599f );
    //vec3 lowz_mods = vec3( 1.0f / ( SOMELARGEFLOATS.xyz + Pi.zzz * ZINC.xyz ) );
    //vec3 highz_mods = vec3( 1.0f / ( SOMELARGEFLOATS.xyz + Pi_inc1.zzz * ZINC.xyz ) );
    vec3 lowz_mods = vec3( 1.0f / ( SOMELARGEFLOATS + Pi.zzz * ZINC ) );
    vec3 highz_mods = vec3( 1.0f / ( SOMELARGEFLOATS + Pi_inc1.zzz * ZINC ) );
    Pi_1 = ( Pi_1.z < 0.5f ) ? lowz_mods : highz_mods;
    Pi_2 = ( Pi_2.z < 0.5f ) ? lowz_mods : highz_mods;
    vec4 hash_0 = fract( Pt * vec4( lowz_mods.x, Pi_1.x, Pi_2.x, highz_mods.x ) ) - 0.49999f;
    vec4 hash_1 = fract( Pt * vec4( lowz_mods.y, Pi_1.y, Pi_2.y, highz_mods.y ) ) - 0.49999f;
    vec4 hash_2 = fract( Pt * vec4( lowz_mods.z, Pi_1.z, Pi_2.z, highz_mods.z ) ) - 0.49999f;

    //	evaluate gradients
    vec4 grad_results = inversesqrt( hash_0 * hash_0 + hash_1 * hash_1 + hash_2 * hash_2 ) * ( hash_0 * v1234_x + hash_1 * v1234_y + hash_2 * v1234_z );

    //	Normalization factor to scale the final result to a strict 1.0->-1.0 range
    //	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
    const float FINAL_NORMALIZATION = 37.837227241611314102871574478976f;

    //  evaulate the kernel weights ( use (0.5-x*x)^3 instead of (0.6-x*x)^4 to fix discontinuities )
    vec4 kernel_weights = v1234_x * v1234_x + v1234_y * v1234_y + v1234_z * v1234_z;
    kernel_weights = max(0.5f - kernel_weights, 0.0f);
    kernel_weights = kernel_weights*kernel_weights*kernel_weights;

    //	sum with the kernel and return
    return dot( kernel_weights, grad_results ) * FINAL_NORMALIZATION;
}

#undef xy
#undef xz
#undef yw

#undef xyz
#undef xzy
#undef zyx
#undef yzx
#undef yxz
#undef yxx
#undef xyx
#undef xxy
#undef zzz

#undef xyxy
#undef zwzw

