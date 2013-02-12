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

#ifndef po6_threads_mutex_h 
#define po6_threads_mutex_h

// POSIX
#ifndef _MSC_VER
#include <errno.h>
#include <pthread.h>
#else
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#endif

// po6
#include <po6/error.h>
#include <po6/noncopyable.h>

namespace po6
{
namespace threads
{

class mutex
{
    public:
        class hold;

    public:
        mutex();
        ~mutex() throw ();

    public:
        void lock();
        bool trylock();
        void unlock();

    private:
        friend class cond;

    private:
        PO6_NONCOPYABLE(mutex);

    private:
#ifdef _MSC_VER
        HANDLE m_mutex;
#else 
        pthread_mutex_t m_mutex;
#endif 
        
};

class mutex::hold
{
    public:
        hold(mutex* mtx);
        ~hold() throw ();

    private:
        PO6_NONCOPYABLE(hold);

    private:
        mutex* m_mtx;
};

inline
mutex :: mutex()
    : m_mutex()
{
#ifdef _MSC_VER
    m_mutex = CreateMutex(NULL, false, NULL);
#else
    int ret = pthread_mutex_init(&m_mutex, NULL);
#endif

#ifdef _MSC_VER
    if (m_mutex == NULL)
    {
        throw po6::error(GetLastError());
    }
#else
    if (ret != 0)
    {
        throw po6::error(ret);
    }
#endif
}

inline
mutex :: ~mutex() throw ()
{
#ifdef _MSC_VER
    int ret = CloseHandle(m_mutex);
#else
    int ret = pthread_mutex_destroy(&m_mutex);
#endif

    if (ret != 0)
    {
#ifndef PO6_NDEBUG_LEAKS
        abort();
#endif
    }
}

inline void
mutex :: lock()
{
#ifdef _MSC_VER
    DWORD ret = WaitForSingleObject(m_mutex, INFINITE);

    if (ret != WAIT_OBJECT_0)
#else
    int ret = pthread_mutex_lock(&m_mutex);

    if (ret != 0)
#endif
    {
        throw po6::error(ret);
    }
}

inline bool
mutex :: trylock()
{
#ifdef _MSC_VER
    DWORD ret = WaitForSingleObject(m_mutex, 0);

    if (ret == WAIT_OBJECT_0)
    {
        return true;
    }
    else if (ret == WAIT_TIMEOUT)
    {
        return false;
    }
    else
    {
        throw po6::error(ret);
    }
#else
    int ret = pthread_mutex_trylock(&m_mutex);

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
#endif
}

inline void
mutex :: unlock()
{
#ifdef _MSC_VER
    DWORD ret = ReleaseMutex(m_mutex);
#else
    int ret = pthread_mutex_unlock(&m_mutex);
#endif

    if (ret != 0)
    {
        throw po6::error(ret);
    }
}

inline
mutex :: hold :: hold(mutex* mtx)
    : m_mtx(mtx)
{
    m_mtx->lock();
}

inline
mutex :: hold :: ~hold() throw ()
{
    try
    {
        m_mtx->unlock();
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

#endif /* po6_threads_mutex_h */ 
