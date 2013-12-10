// Copyright (c) 2011, Robert Escriva
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

#ifndef po6_threads_spinlock_h_
#define po6_threads_spinlock_h_

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include <po6/error.h>
#include <po6/noncopyable.h>

#ifdef __APPLE__
#define pthread_spinlock_t pthread_mutex_t
#define pthread_spin_init pthread_mutex_init
#define pthread_spin_destroy pthread_mutex_destroy
#define pthread_spin_lock pthread_mutex_lock
#define pthread_spin_unlock pthread_mutex_unlock
#define pthread_spin_trylock pthread_mutex_trylock
#endif

namespace po6
{
namespace threads
{

class spinlock
{
    public:
        class hold;

    public:
        spinlock();
        ~spinlock() throw ();

    public:
        void lock();
        bool trylock();
        void unlock();

    private:
        PO6_NONCOPYABLE(spinlock);

    private:
        pthread_spinlock_t m_spin;
};

class spinlock::hold
{
    public:
        hold(spinlock* sp);
        ~hold() throw ();

    private:
        PO6_NONCOPYABLE(hold);

    private:
        spinlock* m_spin;
};

inline
spinlock :: spinlock()
    : m_spin()
{
    int ret = pthread_spin_init(&m_spin, 0);

    if (ret != 0)
    {
        throw po6::error(ret);
    }
}

inline
spinlock :: ~spinlock() throw ()
{
    int ret = pthread_spin_destroy(&m_spin);

    if (ret != 0)
    {
#ifndef PO6_NDEBUG_LEAKS
        abort();
#endif
    }
}

inline void
spinlock :: lock()
{
    int ret = pthread_spin_lock(&m_spin);

    if (ret != 0)
    {
        throw po6::error(ret);
    }
}

inline bool
spinlock :: trylock()
{
    int ret = pthread_spin_trylock(&m_spin);

    if (ret == 0)
    {
        return true;
    }
    else if (ret == EBUSY)
    {
        return false;
    }
    else
    {
        throw po6::error(ret);
    }
}

inline void
spinlock :: unlock()
{
    int ret = pthread_spin_unlock(&m_spin);

    if (ret != 0)
    {
        throw po6::error(ret);
    }
}

inline
spinlock :: hold :: hold(spinlock* sp)
    : m_spin(sp)
{
    m_spin->lock();
}

inline
spinlock :: hold :: ~hold() throw ()
{
    try
    {
        m_spin->unlock();
    }
    catch (...)
    {
#ifndef PO6_NDEBUG_LEAKS
        abort();
#endif
    }
}

} // namespace threads
} // namespace po6

#endif // po6_threads_spinlock_h_
