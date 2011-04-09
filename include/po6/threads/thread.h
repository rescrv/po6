// Copyright (c) 2011, Robert Escriva
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
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
#include <tr1/functional>

// po6
#include "po6/error.h"

namespace po6
{
namespace threads
{

class thread
{
    public:
        thread(std::tr1::function<void (void)> func)
            : m_started(false)
            , m_joined(false)
            , m_func(func)
            , m_thread()
        {
        }

        ~thread()
        {
            if (m_started && !m_joined)
            {
                po6::logic_error("Destructing unjoined thread.");
            }
        }

    public:
        void start()
        {
            if (m_started)
            {
                po6::logic_error("Cannot start thread twice.");
            }

            int ret = pthread_create(&m_thread, NULL, thread::start_routine, &m_func);

            if (ret != 0)
            {
                throw po6::error(ret);
            }

            m_started = true;
        }

        void join()
        {
            if (!m_started)
            {
                po6::logic_error("Cannot join unstarted thread.");
            }

            if (m_joined)
            {
                po6::logic_error("Cannot join already-joined thread.");
            }

            int ret = pthread_join(m_thread, NULL);

            if (ret != 0)
            {
                throw po6::error(ret);
            }

            m_joined = true;
        }

    private:
        static void* start_routine(void * arg)
        {
            std::tr1::function<void (void)>* f;
            f = static_cast<std::tr1::function<void (void)>*>(arg);
            (*f)();
            return NULL;
        }

    private:
        thread(const thread&);

    private:
        thread& operator = (const thread&);

    private:
        bool m_started;
        bool m_joined;
        std::tr1::function<void (void)> m_func;
        pthread_t m_thread;
};

} // namespace threads
} // namespace po6

#endif // po6_threads_thread_h_
