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

#ifndef po6_threads_rwlock_h_
#define po6_threads_rwlock_h_

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include "po6/error.h"

namespace po6
{
namespace threads
{

class rwlock
{
    public:
        class rdhold
        {
            public:
                rdhold(rwlock* rwl)
                    : m_rwl(rwl)
                {
                    m_rwl->rdlock();
                }

                ~rdhold() throw ()
                {
                    try
                    {
                        m_rwl->unlock();
                    }
                    catch (...)
                    {
                        try
                        {
                            PO6_DTOR_ERROR("Unable to release reader-writer lock with RAII.");
                        }
                        catch (...)
                        {
                        }
                    }
                }

            private:
                rdhold(const rdhold&);

            private:
                rdhold& operator = (const rdhold&);

            private:
                rwlock* m_rwl;
        };

        class wrhold
        {
            public:
                wrhold(rwlock* rwl)
                    : m_rwl(rwl)
                {
                    m_rwl->rdlock();
                }

                ~wrhold() throw ()
                {
                    try
                    {
                        m_rwl->unlock();
                    }
                    catch (...)
                    {
                        try
                        {
                            PO6_DTOR_ERROR("Unable to release reader-writer lock with RAII.");
                        }
                        catch (...)
                        {
                        }
                    }
                }

            private:
                wrhold(const wrhold&);

            private:
                wrhold& operator = (const wrhold&);

            private:
                rwlock* m_rwl;
        };

    public:
        rwlock()
            : m_rwlock()
        {
            int ret = pthread_rwlock_init(&m_rwlock, NULL);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        ~rwlock() throw ()
        {
            int ret = pthread_rwlock_destroy(&m_rwlock);

            if (ret != 0)
            {
                try
                {
                    PO6_DTOR_ERROR("Could not destroy reader-writer lock");
                }
                catch (...)
                {
                }
            }
        }

    public:
        void rdlock()
        {
            int ret = pthread_rwlock_rdlock(&m_rwlock);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        void wrlock()
        {
            int ret = pthread_rwlock_wrlock(&m_rwlock);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        void unlock()
        {
            int ret = pthread_rwlock_unlock(&m_rwlock);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

    private:
        rwlock(const rwlock&);

    private:
        rwlock& operator = (const rwlock&);

    private:
        pthread_rwlock_t m_rwlock;
};

} // namespace threads
} // namespace po6

#endif // po6_threads_rwlock_h_
