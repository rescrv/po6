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

#ifndef po6_threads_thread_h_
#define po6_threads_thread_h_

// C++
#ifdef _LIBCPP_VERSION
#include <functional>
#else
#include <tr1/functional>
#endif

// po6
#include <po6/error.h>
#include <po6/noncopyable.h>

namespace po6
{
namespace threads
{
#ifdef _LIBCPP_VERSION
typedef std::function<void (void)> function;
#else
typedef std::tr1::function<void (void)> function;
#endif

template <class T>
struct thread_wrapper0
{
    typedef void (T::*func)();
    thread_wrapper0(func f, T* t)
        : m_func(f), m_ptr(t) {}
    thread_wrapper0(const thread_wrapper0& o)
        : m_func(o.m_func), m_ptr(o.m_ptr) {}
    void operator () () { (m_ptr->*m_func)(); }
    func m_func;
    T* m_ptr;
};

template <class T>
thread_wrapper0<T>
make_thread_wrapper(typename thread_wrapper0<T>::func f, T* t)
{
    return thread_wrapper0<T>(f, t);
}

template <class T, typename A1>
struct thread_wrapper1
{
    typedef void (T::*func)(A1 a1);
    thread_wrapper1(func f, T* t, const A1& a1)
        : m_func(f), m_ptr(t), m_a1(a1) {}
    thread_wrapper1(const thread_wrapper1& o)
        : m_func(o.m_func), m_ptr(o.m_ptr), m_a1(o.m_a1) {}
    void operator () () { (m_ptr->*m_func)(m_a1); }
    func m_func;
    T* m_ptr;
    A1 m_a1;
};

template <class T, typename A1>
thread_wrapper1<T, A1>
make_thread_wrapper(typename thread_wrapper1<T, A1>::func f, T* t, const A1& a1)
{
    return thread_wrapper1<T, A1>(f, t, a1);
}

template <class T, typename A1, typename A2>
struct thread_wrapper2
{
    typedef void (T::*func)(A1 a1, A2 a2);
    thread_wrapper2(func f, T* t, const A1& a1, const A2& a2)
        : m_func(f), m_ptr(t), m_a1(a1), m_a2(a2) {}
    thread_wrapper2(const thread_wrapper2& o)
        : m_func(o.m_func), m_ptr(o.m_ptr), m_a1(o.m_a1), m_a2(o.m_a2) {}
    void operator () () { (m_ptr->*m_func)(m_a1, m_a2); }
    func m_func;
    T* m_ptr;
    A1 m_a1;
    A2 m_a2;
};

template <class T, typename A1, typename A2>
thread_wrapper2<T, A1, A2>
make_thread_wrapper(typename thread_wrapper2<T, A1, A2>::func f, T* t, const A1& a1, const A2& a2)
{
    return thread_wrapper2<T, A1, A2>(f, t, a1, a2);
}

template <class T, typename A1, typename A2, typename A3>
struct thread_wrapper3
{
    typedef void (T::*func)(A1 a1, A2 a2, A3 a3);
    thread_wrapper3(func f, T* t, const A1& a1, const A2& a2, const A3& a3)
        : m_func(f), m_ptr(t), m_a1(a1), m_a2(a2), m_a3(a3) {}
    thread_wrapper3(const thread_wrapper3& o)
        : m_func(o.m_func), m_ptr(o.m_ptr), m_a1(o.m_a1), m_a2(o.m_a2), m_a3(o.m_a3) {}
    void operator () () { (m_ptr->*m_func)(m_a1, m_a2, m_a3); }
    func m_func;
    T* m_ptr;
    A1 m_a1;
    A2 m_a2;
    A3 m_a3;
};

template <class T, typename A1, typename A2, typename A3>
thread_wrapper3<T, A1, A2, A3>
make_thread_wrapper(typename thread_wrapper3<T, A1, A2, A3>::func f, T* t, const A1& a1, const A2& a2, const A3& a3)
{
    return thread_wrapper3<T, A1, A2, A3>(f, t, a1, a2, a3);
}

class thread
{
    public:
        thread(function func);
        ~thread() throw ();

    public:
        void start();
        void join();

    private:
        static void* start_routine(void * arg);

    private:
        PO6_NONCOPYABLE(thread);

    private:
        bool m_started;
        bool m_joined;
        function m_func;
        pthread_t m_thread;
};

inline
thread :: thread(function func)
    : m_started(false)
    , m_joined(false)
    , m_func(func)
    , m_thread()
{
}

inline
thread :: ~thread() throw ()
{
    if (m_started && !m_joined)
    {
        abort();
    }
}

inline void
thread :: start()
{
    if (m_started)
    {
        throw std::logic_error("Cannot start thread twice.");
    }

    int ret = pthread_create(&m_thread, NULL, thread::start_routine, &m_func);

    if (ret != 0)
    {
        throw po6::error(ret);
    }

    m_started = true;
}

inline void
thread :: join()
{
    if (!m_started)
    {
        throw std::logic_error("Cannot join unstarted thread.");
    }

    if (m_joined)
    {
        throw std::logic_error("Cannot join already-joined thread.");
    }

    int ret = pthread_join(m_thread, NULL);

    if (ret != 0)
    {
        throw po6::error(ret);
    }

    m_joined = true;
}

inline void*
thread :: start_routine(void * arg)
{
    function* f;
    f = static_cast<function*>(arg);
    (*f)();
    return NULL;
}

} // namespace threads
} // namespace po6

#endif // po6_threads_thread_h_
