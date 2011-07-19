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
#include "po6/error.h"

namespace po6
{
namespace threads
{

class spinlock
{
    public:
        class hold
        {
            public:
                hold(spinlock* sp)
                    : m_spin(sp)
                {
                    m_spin->lock();
                }

                ~hold() throw ()
                {
                    try
                    {
                        m_spin->unlock();
                    }
                    catch (...)
                    {
                        try
                        {
                            PO6_DTOR_ERROR("Unable to release spinlock with RAII.");
                        }
                        catch (...)
                        {
                        }
                    }
                }

            private:
                hold(const hold&);

            private:
                hold& operator = (const hold&);

            private:
                spinlock* m_spin;
        };

    public:
        spinlock()
            : m_spin()
        {
            int ret = pthread_spin_init(&m_spin, 0);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        ~spinlock() throw ()
        {
            int ret = pthread_spin_destroy(&m_spin);

            if (ret != 0)
            {
                try
                {
                    PO6_DTOR_ERROR("Could not destroy spinlock.");
                }
                catch (...)
                {
                }
            }
        }

    public:
        void lock()
        {
            int ret = pthread_spin_lock(&m_spin);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        bool trylock()
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

        void unlock()
        {
            int ret = pthread_spin_unlock(&m_spin);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

    private:
        friend class cond;

    private:
        spinlock(const spinlock&);

    private:
        spinlock& operator = (const spinlock&);

    private:
        pthread_spinlock_t m_spin;
};

} // namespace threads
} // namespace po6

#endif // po6_threads_spinlock_h_
