Benchmarks for various Simplex noise implementations on the CPU:

1. Stefan Gustavson's Simplex "reference" implementation
2. Arthur Tombs C++ version of Kurt Spencer OpenSimplexNoise
3. BrianSharpe's GLSL SimplexPerlin3D but on the CPU
4. not yet implemented -- Ashima Arts's WebGL snoise


Details:

1) Stefan Gustavson's Simplex "reference" implementation
      http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/
  Specifically,
      http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/simplexnoise1234.cpp
  It has a bug in SimplexNoise1234::noise
      float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
      Note: The scale factor is: 32
  License: Public Domain
  Contact: stegu CIRCLE_A itn.liu.se

  Christian Maher
      https://github.com/cmaher/ashvattha/tree/cpp/src
  Specifically
      https://github.com/cmaher/ashvattha/blob/cpp/src/SimplexNoise.cpp
  has the same broken version
      float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
  License: "Advertising/Attribution License" -> " copyright notice and this permission notice shall be included in all copies"
      https://github.com/cmaher/ashvattha/blob/cpp/src/SimplexNoise.cpp

2) OpenSimplex

  Kurt Spencer (KdotJPG) initial Java version OpenSimplexNoise
  https://gist.github.com/KdotJPG/b1270127455a94ac5d19
  http://uniblock.tumblr.com/post/97868843242/noise

  Arthur Tombs ported KdotJPG's OpenSimplexNoise to C++
  https://gist.github.com/tombsar/716134ec71d1b8c1b530

3) Brian Sharpe's SimplexPerlin3D
  There is a C++ wrapper to run GLSL on the CPU.
     https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin3D.glsl
  License: Public Doman -> "I'm not one for copyrights. Use the code however you wish."
  Contact: brisharpe CIRCLE_A yahoo DOT com
  Brian pointed out:
     f(x) = (0.6 - x*x)^4 // artifacts
     f(x) = (0.5 - x*x)^4 // smooth blending
  http://briansharpe.files.wordpress.com/2012/01/perlin_simplex_artifacts1.jpg

4) Ian McEwan wrote a GLSL version that doesn't use any uniform arrays or texture engines.
  Ashima Arts hosted the WebGL version (MIT License)
  https://github.com/ashima/webgl-noise


References:

"Chapter 2 - Noise Hardware - Ken Perlin"
- http://www.csee.umbc.edu/~olano/s2002c36/ch02.pdf
- http://briansharpe.wordpress.com/2012/01/13/simplex-noise/
- http://cmaher.github.io/posts/working-with-simplex-noise/
- http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
- http://uniblock.tumblr.com/post/97868843242/noise
- http://www.reddit.com/r/proceduralgeneration/comments/2gu3e7/like_perlins_simplex_noise_but_dont_like_the/

Special Thanks:

- Stefan Gustavson for taking the time to explain Simplex Noise.
- KdotJPG and Tombsar for the source for OpenSimplexNoise !


No Thanks:

-  To Ken Perlin for patenting an algorithm for noise. BOO. HISS.

   What's next, patenting Mathematics?!?

