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
#include "po6/threads/thread.h"

using po6::threads::thread;

thread :: thread(function func)
    : m_started(false)
    , m_joined(false)
    , m_func(func)
    , m_thread()
{
}

thread :: ~thread() throw ()
{
    if (m_started && !m_joined)
    {
        abort();
    }
}

void
thread :: start()
{
    assert(!m_started);
    int ret = pthread_create(&m_thread, NULL, thread::start_routine, &m_func);

    if (ret != 0)
    {
        abort();
    }

    m_started = true;
}

void
thread :: join()
{
    assert(m_started && !m_joined);
    int ret = pthread_join(m_thread, NULL);

    if (ret != 0)
    {
        abort();
    }

    m_joined = true;
}

void*
thread :: start_routine(void * arg)
{
    function* f;
    f = static_cast<function*>(arg);
    (*f)();
    return NULL;
}
