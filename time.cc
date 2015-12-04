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
//     * Neither the name of this project nor the names of its contributors may
//       be used to endorse or promote products derived from this software
//       without specific prior written permission.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

// po6
#include "po6/time.h"

#if HAVE_CLOCK_GETTIME

#include <time.h>

uint64_t
po6 :: time()
{
    return wallclock_time();
}

uint64_t
po6 :: monotonic_time()
{
    timespec ts;
    clockid_t ci;
#ifdef CLOCK_UPTIME
    ci = CLOCK_UPTIME;
#elif defined CLOCK_MONOTONIC_RAW
    ci = CLOCK_MONOTONIC_RAW;
#elif defined CLOCK_MONOTONIC
    ci = CLOCK_MONOTONIC;
#else
#error no known-working monotonic clock
#endif

    if (clock_gettime(ci, &ts) < 0)
    {
        return 0;
    }

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

uint64_t
po6 :: wallclock_time()
{
    timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
    {
        return 0;
    }

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void
po6 :: sleep(uint64_t x)
{
    struct timespec ts;
    ts.tv_sec = x / PO6_SECONDS;
    ts.tv_nsec = x % PO6_SECONDS;
    nanosleep(&ts, NULL);
}

#elif __MACH__

#include <mach/clock.h>
#include <mach/mach.h>

uint64_t
po6 :: time()
{
    return wallclock_time();
}

uint64_t
po6 :: monotonic_time()
{
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return mts.tv_sec * 1000000000 + mts.tv_nsec;
}

uint64_t
po6 :: wallclock_time()
{
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return mts.tv_sec * 1000000000 + mts.tv_nsec;
}

#endif
