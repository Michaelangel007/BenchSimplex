Benchmarks for various Simplex implementations on the CPU

* Stefan Gustavson's Simplex "reference" implementation
      http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/
  Specifically,
      http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/simplexnoise1234.cpp
  It has a bug in SimplexNoise1234::noise
      float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
      Note: The scale factor is: 32
  License: Public Domain

  Christian Maher
      https://github.com/cmaher/ashvattha/tree/cpp/src
  Specifically
      https://github.com/cmaher/ashvattha/blob/cpp/src/SimplexNoise.cpp
  has the same broken version
      float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
  License: "Advertising/Attribution License" -> " copyright notice and this permission notice shall be included in all copies"
      https://github.com/cmaher/ashvattha/blob/cpp/src/SimplexNoise.cpp

* OpenSimplex

* BrianSharpe's SimplexPerlin3D but running on the CPU
     https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin3D.glsl
  License: Public Doman -> "I'm not one for copyrights. Use the code however you wish."
  Brian pointed out:
     f(x) = (0.6 - x*x)^4 // artifacts
     f(x) = (0.5 - x*x)^4 // smooth blending
  http://briansharpe.files.wordpress.com/2012/01/perlin_simplex_artifacts1.jpg
  brisharpe CIRCLE_A yahoo DOT com

Ian McEwan wrote a GLSL version that doesn't use any uniform arrays or texture engines.
  Ashima Arts hosted the WebGL version (MIT License)
  https://github.com/ashima/webgl-noise
  There is a C++ wrapper to run GLSL on the CPU.

References:

"Chapter 2 - Noise Hardware - Ken Perlin"
- http://www.csee.umbc.edu/~olano/s2002c36/ch02.pdf
- http://briansharpe.wordpress.com/2012/01/13/simplex-noise/
- http://cmaher.github.io/posts/working-with-simplex-noise/
- http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
