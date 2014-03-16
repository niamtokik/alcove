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
#include "alcove.h"
#include "alcove_call.h"
#include "alcove_calls.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <err.h>
#include <errno.h>

#include <sys/types.h>

static int cons_pid(alcove_child_t *c, void *arg1, void *arg2);

    ETERM *
alcove_call(alcove_state_t *ap, u_int32_t call, ETERM *arg)
{
    alcove_call_t *fun = NULL;

    if (call >= sizeof(calls)/sizeof(calls[0]))
        return erl_mk_atom("badarg");

    fun = &calls[call];

    if (!ERL_IS_LIST(arg) || erl_length(arg) != fun->narg)
        return erl_mk_atom("badarg");

    return (*fun->fp)(ap, arg);
}

    ETERM *
alcove_version(alcove_state_t *ap, ETERM *arg)
{
    return alcove_bin(ALCOVE_VERSION);
}

    ETERM *
alcove_pid(alcove_state_t *ap, ETERM *arg)
{
    ETERM *t = erl_mk_empty_list();
    (void)pid_foreach(ap, 0, &t, NULL, pid_not_equal, cons_pid);
    return t;
}

/*
 * Utility functions
 */

    char **
alcove_list_to_argv(ETERM *arg)
{
    ETERM *hd = NULL;
    ssize_t len = 0;
    int i = 0;
    char **argv = NULL;
    long maxarg = sysconf(_SC_ARG_MAX);

    len = erl_length(arg);

    if (len < 0 || len >= maxarg)
        return NULL;

    /* NULL terminate */
    argv = calloc(len + 1, sizeof(char **));

    if (!argv)
        return NULL;

    for (i = 0; i < len; i++) {
        arg = alcove_list_head(&hd, arg);
        if (!hd)
            return NULL;

        argv[i] = erl_iolist_to_string(hd);
        if (!argv[i])
            return NULL;
    }

    return argv;
}

    void
alcove_free_argv(char **argv)
{
    int i = 0;

    if (argv == NULL)
        return;

    for (i = 0; argv[i]; i++)
        free(argv[i]);

    free(argv);
}

    static int
cons_pid(alcove_child_t *c, void *arg1, void *arg2)
{
    ETERM **t = arg1;
    *t = erl_cons(erl_mk_int(c->pid), *t);
    return 1;
}
