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
#include "po6/threads/rwlock.h"

using po6::threads::rwlock;

rwlock :: rwlock()
    : m_rwlock()
{
    int ret = pthread_rwlock_init(&m_rwlock, NULL);

    if (ret != 0)
    {
        abort();
    }
}

rwlock :: ~rwlock() throw ()
{
    int ret = pthread_rwlock_destroy(&m_rwlock);

    if (ret != 0)
    {
        abort();
    }
}

void
rwlock :: rdlock()
{
    int ret = pthread_rwlock_rdlock(&m_rwlock);

    if (ret != 0)
    {
        abort();
    }
}

void
rwlock :: wrlock()
{
    int ret = pthread_rwlock_wrlock(&m_rwlock);

    if (ret != 0)
    {
        abort();
    }
}

void
rwlock :: unlock()
{
    int ret = pthread_rwlock_unlock(&m_rwlock);

    if (ret != 0)
    {
        abort();
    }
}

rwlock :: rdhold :: rdhold(rwlock* rwl)
    : m_held(false)
    , m_rwl(rwl)
{
    m_rwl->rdlock();
    m_held = true;
}

void
rwlock :: rdhold :: release()
{
    assert(m_held);
    m_held = false;
    m_rwl->unlock();
}

rwlock :: rdhold :: ~rdhold() throw ()
{
    if (m_held)
    {
        release();
    }
}

rwlock :: wrhold :: wrhold(rwlock* rwl)
    : m_held(false)
    , m_rwl(rwl)
{
    m_rwl->wrlock();
    m_held = true;
}

void
rwlock :: wrhold :: release()
{
    assert(m_held);
    m_held = false;
    m_rwl->unlock();
}

rwlock :: wrhold :: ~wrhold() throw ()
{
    if (m_held)
    {
        release();
    }
}
