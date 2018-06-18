/*
Copyright (c) 2018, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of the Oxford RSE C++ Template project.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BACKEND_H_
#define BACKEND_H_

#include <cmath>
#include <sstream>

namespace trase {

/// a transform matrix in the form
///
/// [a c e]
/// [b d f]
/// [0 0 1]
///
struct Transform {
  float a, b, c, d, e, f;

  Transform() : a(1), b(0), c(0), d(1), e(0), f(0) {}

  bool is_identity() {
    return a == 1.0f && b == 0.0f && c == 0.0f && d == 1.0f && e == 0.0f &&
           f == 0.0f;
  }
  void clear() {
    a = d = 1.0;
    c = e = b = f = 0.0;
  }
  void translate(const vfloat2_t &t) {
    /// [1 0 tx]   [a c e]    [a' c' e']
    /// [0 1 ty] * [b d f]  = [b' d' f']
    /// [0 0  1]   [0 0 1]    [0  0  1]

    // pre-mult
    e += t[0];
    f += t[1];
    /*
      // post-mult
     /// [a c e]   [1 0 tx]   [a' c' e']
     /// [b d f] * [0 1 ty] = [b' d' f']
     /// [0 0 1]   [0 0  1]   [0  0  1]


      e += a * t[0] + c * t[1];
      f += b * t[0] + d * t[1];
      */
  }
  void rotate(float angle) {
    const float cos_t = std::cos(angle);
    const float sin_t = std::sin(angle);

    // pre-mult
    ///  [cost -sint 0]   [a c e]   [a' c' e']
    ///  [sint cost  0] * [b d f] = [b' d' f']
    ///  [0     0    1]   [0 0 1]   [0  0   1]

    const float a0 = a * cos_t - b * sin_t;
    b = a * sin_t + b * cos_t;
    const float c0 = c * cos_t - d * sin_t;
    d = c * sin_t + d * cos_t;
    const float e0 = e * cos_t - f * sin_t;
    f = e * sin_t + f * cos_t;
    a = a0;
    c = c0;
    e = e0;
    /*
    // post-mult
    /// [a c e]   [cost -sint 0]   [a' c' e']
    /// [b d f] * [sint cost  0] = [b' d' f']
    /// [0 0 1]   [0     0    1]   [0  0   1]

    a = a * cos_t + c * sin_t;
    b = b * cos_t + d * sin_t;
    c = -a * sin_t + c * cos_t;
    d = -b * sin_t + d * cos_t;
    */
  }

  std::string to_string() {
    std::stringstream stream;
    stream << "transform=\"matrix(" << a << ' ' << b << ' ' << c << ' ' << d
           << ' ' << e << ' ' << f << ")\"";
    return stream.str();
  }
};

enum Align {
  // Horizontal align
  ALIGN_LEFT = 1 << 0,   // Default, align text horizontally to left.
  ALIGN_CENTER = 1 << 1, // Align text horizontally to center.
  ALIGN_RIGHT = 1 << 2,  // Align text horizontally to right.
  // Vertical align
  ALIGN_TOP = 1 << 3,      // Align text vertically to top.
  ALIGN_MIDDLE = 1 << 4,   // Align text vertically to middle.
  ALIGN_BOTTOM = 1 << 5,   // Align text vertically to bottom.
  ALIGN_BASELINE = 1 << 6, // Default, align text vertically to baseline.
};

enum ArcDirection {
  CLOCKWISE = 1 << 0,
  COUNTER_CLOCKWISE = 1 << 1,
};

} // namespace trase

#endif // BACKEND_H_
