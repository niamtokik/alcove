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
alcove_define_t alcove_mount_constants[] = {
#ifdef MS_RDONLY
    {"rdonly", MS_RDONLY},
    ALCOVE_DEFINE(MS_RDONLY),
#endif
#ifdef MS_NOSUID
    {"nosuid", MS_NOSUID},
    ALCOVE_DEFINE(MS_NOSUID),
#endif
#ifdef MS_NODEV
    ALCOVE_DEFINE(MS_NODEV),
#endif
#ifdef MS_NOEXEC
    {"noexec", MS_NOEXEC},
    ALCOVE_DEFINE(MS_NOEXEC),
#endif
#ifdef MS_SYNCHRONOUS
    ALCOVE_DEFINE(MS_SYNCHRONOUS),
#endif
#ifdef MS_REMOUNT
    ALCOVE_DEFINE(MS_REMOUNT),
#endif
#ifdef MS_MANDLOCK
    ALCOVE_DEFINE(MS_MANDLOCK),
#endif
#ifdef MS_DIRSYNC
    ALCOVE_DEFINE(MS_DIRSYNC),
#endif
#ifdef MS_NOATIME
    {"noatime", MS_NOATIME},
    ALCOVE_DEFINE(MS_NOATIME),
#endif
#ifdef MS_NODIRATIME
    ALCOVE_DEFINE(MS_NODIRATIME),
#endif
#ifdef MS_BIND
    ALCOVE_DEFINE(MS_BIND),
#endif
#ifdef MS_MOVE
    ALCOVE_DEFINE(MS_MOVE),
#endif
#ifdef MS_REC
    ALCOVE_DEFINE(MS_REC),
#endif
#ifdef MS_SILENT
    ALCOVE_DEFINE(MS_SILENT),
#endif
#ifdef MS_POSIXACL
    ALCOVE_DEFINE(MS_POSIXACL),
#endif
#ifdef MS_UNBINDABLE
    ALCOVE_DEFINE(MS_UNBINDABLE),
#endif
#ifdef MS_PRIVATE
    ALCOVE_DEFINE(MS_PRIVATE),
#endif
#ifdef MS_SLAVE
    ALCOVE_DEFINE(MS_SLAVE),
#endif
#ifdef MS_SHARED
    ALCOVE_DEFINE(MS_SHARED),
#endif
#ifdef MS_RELATIME
    ALCOVE_DEFINE(MS_RELATIME),
#endif
#ifdef MS_KERNMOUNT
    ALCOVE_DEFINE(MS_KERNMOUNT),
#endif
#ifdef MS_I_VERSION
    ALCOVE_DEFINE(MS_I_VERSION),
#endif
#ifdef MS_STRICTATIME
    ALCOVE_DEFINE(MS_STRICTATIME),
#endif
#ifdef MS_ACTIVE
    ALCOVE_DEFINE(MS_ACTIVE),
#endif
#ifdef MS_NOUSER
    ALCOVE_DEFINE(MS_NOUSER),
#endif
#ifdef MS_RMT_MASK
    ALCOVE_DEFINE(MS_RMT_MASK),
#endif
#ifdef MS_MGC_VAL
    ALCOVE_DEFINE(MS_MGC_VAL),
#endif
#ifdef MS_MGC_MSK
    ALCOVE_DEFINE(MS_MGC_MSK),
#endif
#ifdef MNT_FORCE
    ALCOVE_DEFINE(MNT_FORCE),
#endif
#ifdef MNT_DETACH
    ALCOVE_DEFINE(MNT_DETACH),
#endif
#ifdef MNT_EXPIRE
    ALCOVE_DEFINE(MNT_EXPIRE),
#endif

#ifdef MNT_NOATIME
    {"noatime", MNT_NOATIME},
    ALCOVE_DEFINE(MNT_NOATIME),
#endif
#ifdef MNT_RDONLY
    {"rdonly", MNT_RDONLY},
    ALCOVE_DEFINE(MNT_RDONLY),
#endif
#ifdef MNT_EXEC
    {"noexec", MNT_NOEXEC},
    ALCOVE_DEFINE(MNT_NOEXEC),
#endif
#ifdef MNT_NOSUID
    {"nosuid", MNT_NOSUID},
    ALCOVE_DEFINE(MNT_NOSUID),
#endif

    {NULL, 0}
};
