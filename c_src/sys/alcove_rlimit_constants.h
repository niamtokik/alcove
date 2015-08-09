/* Copyright (c) 2014, Michael Santos <michael.santos@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
static const alcove_define_t alcove_rlimit_constants[] = {
#ifdef RLIMIT_CPU
    ALCOVE_DEFINE(RLIMIT_CPU),
#endif
#ifdef RLIMIT_FSIZE
    ALCOVE_DEFINE(RLIMIT_FSIZE),
#endif
#ifdef RLIMIT_DATA
    ALCOVE_DEFINE(RLIMIT_DATA),
#endif
#ifdef RLIMIT_STACK
    ALCOVE_DEFINE(RLIMIT_STACK),
#endif
#ifdef RLIMIT_CORE
    ALCOVE_DEFINE(RLIMIT_CORE),
#endif
#ifdef RLIMIT_RSS
    ALCOVE_DEFINE(RLIMIT_RSS),
#endif
#ifdef RLIMIT_NPROC
    ALCOVE_DEFINE(RLIMIT_NPROC),
#endif
#ifdef RLIMIT_NOFILE
    ALCOVE_DEFINE(RLIMIT_NOFILE),
#endif
#ifdef RLIMIT_MEMLOCK
    ALCOVE_DEFINE(RLIMIT_MEMLOCK),
#endif
#ifdef RLIMIT_AS
    ALCOVE_DEFINE(RLIMIT_AS),
#endif
#ifdef RLIMIT_LOCKS
    ALCOVE_DEFINE(RLIMIT_LOCKS),
#endif
#ifdef RLIMIT_SIGPENDING
    ALCOVE_DEFINE(RLIMIT_SIGPENDING),
#endif
#ifdef RLIMIT_MSGQUEUE
    ALCOVE_DEFINE(RLIMIT_MSGQUEUE),
#endif
#ifdef RLIMIT_NICE
    ALCOVE_DEFINE(RLIMIT_NICE),
#endif
#ifdef RLIMIT_RTPRIO
    ALCOVE_DEFINE(RLIMIT_RTPRIO),
#endif
#ifdef RLIMIT_RTTIME
    ALCOVE_DEFINE(RLIMIT_RTTIME),
#endif
#ifdef RLIMIT_NLIMITS
    ALCOVE_DEFINE(RLIMIT_NLIMITS),
#endif
    {NULL, 0}
};