// Copyright (c) 2011,2015, Robert Escriva
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of po6 nor the names of its contributors may be used
//       to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef po6_pathname_h_
#define po6_pathname_h_

// STL
#include <string>

namespace po6
{
namespace path
{

// On some platforms, basename and dirname may use statically allocated storage,
// or may modify the input string, neither of which are desirable.  These
// basename and dirname calls will return a new string without modifying the
// input.

std::string
basename(const std::string& path);

std::string
dirname(const std::string& path);

// This corresponds to the realpath(3) library call.
bool
realpath(const std::string& path, std::string* real);

// Concatenate pathnames in a manner that is reminiscent of os.path.join in
// Python.

std::string
join(const std::string& p1,
     const std::string& p2);

std::string
join(const std::string& p1,
     const std::string& p2,
     const std::string& p3);

std::string
join(const std::string& p1,
     const std::string& p2,
     const std::string& p3,
     const std::string& p4);

std::string
join(const std::string& p1,
     const std::string& p2,
     const std::string& p3,
     const std::string& p4,
     const std::string& p5);

} // namespace path
} // namespace po6

#endif // po6_pathname_h_
