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

#ifndef po6_threads_rwlock_h_
#define po6_threads_rwlock_h_

// POSIX
#include <pthread.h>

namespace po6
{
namespace threads
{

class rwlock
{
    public:
        class rdhold;
        class wrhold;

    public:
        rwlock();
        ~rwlock() throw ();

    public:
        void rdlock();
        void wrlock();
        void unlock();

    private:
        pthread_rwlock_t m_rwlock;

    private:
        rwlock(const rwlock&);
        rwlock& operator = (const rwlock&);
};

class rwlock::rdhold
{
    public:
        rdhold(rwlock* rwl);
        ~rdhold() throw ();

    public:
        void release();

    private:
        bool m_held;
        rwlock* m_rwl;

    private:
        rdhold(const rdhold&);
        rdhold& operator = (const rdhold&);
};

class rwlock::wrhold
{
    public:
        wrhold(rwlock* rwl);
        ~wrhold() throw ();

    public:
        void release();

    private:
        bool m_held;
        rwlock* m_rwl;

    private:
        wrhold(const wrhold&);
        wrhold& operator = (const wrhold&);
};

} // namespace threads
} // namespace po6

#endif // po6_threads_rwlock_h_
