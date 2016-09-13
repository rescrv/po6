// Copyright (c) 2015, Robert Escriva
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
#include <string.h>

// POSIX
#include <errno.h>

// po6
#include <po6/errno.h>

std::string
po6 :: strerror(int err)
{
    int saved = errno;
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    errno = 0;
#if _GNU_SOURCE
    // GNU
    char* ret = strerror_r(err, buf, sizeof(buf));

    if (ret != buf)
    {
        strncpy(buf, ret, sizeof(buf));
    }
#else
    // XSI-compliant
    errno = strerror_r(err, buf, sizeof(buf));
#endif
    int obs = errno;
    errno = saved;

    if (obs != 0)
    {
        return "unknown error";
    }

    buf[sizeof(buf) - 1] = '\0';
    return std::string(buf);
}

#define CSTRINGIFY(x) if ((x) == err) { return #x; }

const char*
po6 :: strerrno(int err)
{
    if (err == 0)
    {
        return "";
    }
#ifdef E2BIG
    CSTRINGIFY(E2BIG)
#endif
#ifdef EACCES
    CSTRINGIFY(EACCES)
#endif
#ifdef EADDRINUSE
    CSTRINGIFY(EADDRINUSE)
#endif
#ifdef EADDRNOTAVAIL
    CSTRINGIFY(EADDRNOTAVAIL)
#endif
#ifdef EAFNOSUPPORT
    CSTRINGIFY(EAFNOSUPPORT)
#endif
#ifdef EAGAIN
    CSTRINGIFY(EAGAIN)
#endif
#ifdef EALREADY
    CSTRINGIFY(EALREADY)
#endif
#ifdef EAUTH
    CSTRINGIFY(EAUTH)
#endif
#ifdef EBADE
    CSTRINGIFY(EBADE)
#endif
#ifdef EBADF
    CSTRINGIFY(EBADF)
#endif
#ifdef EBADFD
    CSTRINGIFY(EBADFD)
#endif
#ifdef EBADMSG
    CSTRINGIFY(EBADMSG)
#endif
#ifdef EBADR
    CSTRINGIFY(EBADR)
#endif
#ifdef EBADRPC
    CSTRINGIFY(EBADRPC)
#endif
#ifdef EBADRQC
    CSTRINGIFY(EBADRQC)
#endif
#ifdef EBADSLT
    CSTRINGIFY(EBADSLT)
#endif
#ifdef EBUSY
    CSTRINGIFY(EBUSY)
#endif
#ifdef ECANCELED
    CSTRINGIFY(ECANCELED)
#endif
#ifdef ECAPMODE
    CSTRINGIFY(ECAPMODE)
#endif
#ifdef ECHILD
    CSTRINGIFY(ECHILD)
#endif
#ifdef ECHRNG
    CSTRINGIFY(ECHRNG)
#endif
#ifdef ECOMM
    CSTRINGIFY(ECOMM)
#endif
#ifdef ECONNABORTED
    CSTRINGIFY(ECONNABORTED)
#endif
#ifdef ECONNREFUSED
    CSTRINGIFY(ECONNREFUSED)
#endif
#ifdef ECONNRESET
    CSTRINGIFY(ECONNRESET)
#endif
#ifdef EDEADLK
    CSTRINGIFY(EDEADLK)
#endif
#ifdef EDEADLOCK
    CSTRINGIFY(EDEADLOCK)
#endif
#ifdef EDESTADDRREQ
    CSTRINGIFY(EDESTADDRREQ)
#endif
#ifdef EDOM
    CSTRINGIFY(EDOM)
#endif
#ifdef EDOOFUS
    CSTRINGIFY(EDOOFUS)
#endif
#ifdef EDQUOT
    CSTRINGIFY(EDQUOT)
#endif
#ifdef EEXIST
    CSTRINGIFY(EEXIST)
#endif
#ifdef EFAULT
    CSTRINGIFY(EFAULT)
#endif
#ifdef EFBIG
    CSTRINGIFY(EFBIG)
#endif
#ifdef EFTYPE
    CSTRINGIFY(EFTYPE)
#endif
#ifdef EHOSTDOWN
    CSTRINGIFY(EHOSTDOWN)
#endif
#ifdef EHOSTUNREACH
    CSTRINGIFY(EHOSTUNREACH)
#endif
#ifdef EIDRM
    CSTRINGIFY(EIDRM)
#endif
#ifdef EILSEQ
    CSTRINGIFY(EILSEQ)
#endif
#ifdef EINPROGRESS
    CSTRINGIFY(EINPROGRESS)
#endif
#ifdef EINTR
    CSTRINGIFY(EINTR)
#endif
#ifdef EINVAL
    CSTRINGIFY(EINVAL)
#endif
#ifdef EIO
    CSTRINGIFY(EIO)
#endif
#ifdef EISCONN
    CSTRINGIFY(EISCONN)
#endif
#ifdef EISDIR
    CSTRINGIFY(EISDIR)
#endif
#ifdef EISNAM
    CSTRINGIFY(EISNAM)
#endif
#ifdef EKEYEXPIRED
    CSTRINGIFY(EKEYEXPIRED)
#endif
#ifdef EKEYREJECTED
    CSTRINGIFY(EKEYREJECTED)
#endif
#ifdef EKEYREVOKED
    CSTRINGIFY(EKEYREVOKED)
#endif
#ifdef EL2HLT
    CSTRINGIFY(EL2HLT)
#endif
#ifdef EL2NSYNC
    CSTRINGIFY(EL2NSYNC)
#endif
#ifdef EL3HLT
    CSTRINGIFY(EL3HLT)
#endif
#ifdef EL3RST
    CSTRINGIFY(EL3RST)
#endif
#ifdef ELIBACC
    CSTRINGIFY(ELIBACC)
#endif
#ifdef ELIBBAD
    CSTRINGIFY(ELIBBAD)
#endif
#ifdef ELIBEXEC
    CSTRINGIFY(ELIBEXEC)
#endif
#ifdef ELIBMAX
    CSTRINGIFY(ELIBMAX)
#endif
#ifdef ELIBSCN
    CSTRINGIFY(ELIBSCN)
#endif
#ifdef ELOOP
    CSTRINGIFY(ELOOP)
#endif
#ifdef EMEDIUMTYPE
    CSTRINGIFY(EMEDIUMTYPE)
#endif
#ifdef EMFILE
    CSTRINGIFY(EMFILE)
#endif
#ifdef EMLINK
    CSTRINGIFY(EMLINK)
#endif
#ifdef EMSGSIZE
    CSTRINGIFY(EMSGSIZE)
#endif
#ifdef EMULTIHOP
    CSTRINGIFY(EMULTIHOP)
#endif
#ifdef ENAMETOOLONG
    CSTRINGIFY(ENAMETOOLONG)
#endif
#ifdef ENEEDAUTH
    CSTRINGIFY(ENEEDAUTH)
#endif
#ifdef ENETDOWN
    CSTRINGIFY(ENETDOWN)
#endif
#ifdef ENETRESET
    CSTRINGIFY(ENETRESET)
#endif
#ifdef ENETUNREACH
    CSTRINGIFY(ENETUNREACH)
#endif
#ifdef ENFILE
    CSTRINGIFY(ENFILE)
#endif
#ifdef ENOATTR
    CSTRINGIFY(ENOATTR)
#endif
#ifdef ENOBUFS
    CSTRINGIFY(ENOBUFS)
#endif
#ifdef ENODATA
    CSTRINGIFY(ENODATA)
#endif
#ifdef ENODEV
    CSTRINGIFY(ENODEV)
#endif
#ifdef ENOENT
    CSTRINGIFY(ENOENT)
#endif
#ifdef ENOEXEC
    CSTRINGIFY(ENOEXEC)
#endif
#ifdef ENOKEY
    CSTRINGIFY(ENOKEY)
#endif
#ifdef ENOLCK
    CSTRINGIFY(ENOLCK)
#endif
#ifdef ENOLINK
    CSTRINGIFY(ENOLINK)
#endif
#ifdef ENOMEDIUM
    CSTRINGIFY(ENOMEDIUM)
#endif
#ifdef ENOMEM
    CSTRINGIFY(ENOMEM)
#endif
#ifdef ENOMSG
    CSTRINGIFY(ENOMSG)
#endif
#ifdef ENONET
    CSTRINGIFY(ENONET)
#endif
#ifdef ENOPKG
    CSTRINGIFY(ENOPKG)
#endif
#ifdef ENOPROTOOPT
    CSTRINGIFY(ENOPROTOOPT)
#endif
#ifdef ENOSPC
    CSTRINGIFY(ENOSPC)
#endif
#ifdef ENOSR
    CSTRINGIFY(ENOSR)
#endif
#ifdef ENOSTR
    CSTRINGIFY(ENOSTR)
#endif
#ifdef ENOSYS
    CSTRINGIFY(ENOSYS)
#endif
#ifdef ENOTBLK
    CSTRINGIFY(ENOTBLK)
#endif
#ifdef ENOTCAPABLE
    CSTRINGIFY(ENOTCAPABLE)
#endif
#ifdef ENOTCONN
    CSTRINGIFY(ENOTCONN)
#endif
#ifdef ENOTDIR
    CSTRINGIFY(ENOTDIR)
#endif
#ifdef ENOTEMPTY
    CSTRINGIFY(ENOTEMPTY)
#endif
#ifdef ENOTSOCK
    CSTRINGIFY(ENOTSOCK)
#endif
#ifdef ENOTSUP
    CSTRINGIFY(ENOTSUP)
#endif
#ifdef ENOTTY
    CSTRINGIFY(ENOTTY)
#endif
#ifdef ENOTUNIQ
    CSTRINGIFY(ENOTUNIQ)
#endif
#ifdef ENXIO
    CSTRINGIFY(ENXIO)
#endif
#ifdef EOPNOTSUPP
    CSTRINGIFY(EOPNOTSUPP)
#endif
#ifdef EOVERFLOW
    CSTRINGIFY(EOVERFLOW)
#endif
#ifdef EPERM
    CSTRINGIFY(EPERM)
#endif
#ifdef EPFNOSUPPORT
    CSTRINGIFY(EPFNOSUPPORT)
#endif
#ifdef EPIPE
    CSTRINGIFY(EPIPE)
#endif
#ifdef EPROCLIM
    CSTRINGIFY(EPROCLIM)
#endif
#ifdef EPROCUNAVAIL
    CSTRINGIFY(EPROCUNAVAIL)
#endif
#ifdef EPROGMISMATCH
    CSTRINGIFY(EPROGMISMATCH)
#endif
#ifdef EPROGUNAVAIL
    CSTRINGIFY(EPROGUNAVAIL)
#endif
#ifdef EPROTO
    CSTRINGIFY(EPROTO)
#endif
#ifdef EPROTONOSUPPORT
    CSTRINGIFY(EPROTONOSUPPORT)
#endif
#ifdef EPROTOTYPE
    CSTRINGIFY(EPROTOTYPE)
#endif
#ifdef ERANGE
    CSTRINGIFY(ERANGE)
#endif
#ifdef EREMCHG
    CSTRINGIFY(EREMCHG)
#endif
#ifdef EREMOTE
    CSTRINGIFY(EREMOTE)
#endif
#ifdef EREMOTEIO
    CSTRINGIFY(EREMOTEIO)
#endif
#ifdef ERESTART
    CSTRINGIFY(ERESTART)
#endif
#ifdef EROFS
    CSTRINGIFY(EROFS)
#endif
#ifdef ERPCMISMATCH
    CSTRINGIFY(ERPCMISMATCH)
#endif
#ifdef ESHUTDOWN
    CSTRINGIFY(ESHUTDOWN)
#endif
#ifdef ESOCKTNOSUPPORT
    CSTRINGIFY(ESOCKTNOSUPPORT)
#endif
#ifdef ESPIPE
    CSTRINGIFY(ESPIPE)
#endif
#ifdef ESRCH
    CSTRINGIFY(ESRCH)
#endif
#ifdef ESTALE
    CSTRINGIFY(ESTALE)
#endif
#ifdef ESTRPIPE
    CSTRINGIFY(ESTRPIPE)
#endif
#ifdef ETIME
    CSTRINGIFY(ETIME)
#endif
#ifdef ETIMEDOUT
    CSTRINGIFY(ETIMEDOUT)
#endif
#ifdef ETXTBSY
    CSTRINGIFY(ETXTBSY)
#endif
#ifdef EUCLEAN
    CSTRINGIFY(EUCLEAN)
#endif
#ifdef EUNATCH
    CSTRINGIFY(EUNATCH)
#endif
#ifdef EUSERS
    CSTRINGIFY(EUSERS)
#endif
#ifdef EWOULDBLOCK
    CSTRINGIFY(EWOULDBLOCK)
#endif
#ifdef EXDEV
    CSTRINGIFY(EXDEV)
#endif
#ifdef EXFULL
    CSTRINGIFY(EXFULL)
#endif
    return "unknown error";
}
