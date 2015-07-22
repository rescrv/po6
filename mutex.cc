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

// C
#include <assert.h>
#include <stdlib.h>

// po6
#include "po6/threads/mutex.h"

using po6::threads::mutex;

mutex :: mutex()
    : m_mutex()
{
    int ret = pthread_mutex_init(&m_mutex, NULL);

    if (ret != 0)
    {
        abort();
    }
}

mutex :: ~mutex() throw ()
{
    int ret = pthread_mutex_destroy(&m_mutex);

    if (ret != 0)
    {
        abort();
    }
}

void
mutex :: lock()
{
    int ret = pthread_mutex_lock(&m_mutex);

    if (ret != 0)
    {
        abort();
    }
}

void
mutex :: unlock()
{
    int ret = pthread_mutex_unlock(&m_mutex);

    if (ret != 0)
    {
        abort();
    }
}

mutex :: hold :: hold(mutex* mtx)
    : m_held(false)
    , m_mtx(mtx)
{
    m_mtx->lock();
    m_held = true;
}

void
mutex :: hold :: release()
{
    assert(m_held);
    m_held = false;
    m_mtx->unlock();
}

mutex :: hold :: ~hold() throw ()
{
    if (m_held)
    {
        release();
    }
}
