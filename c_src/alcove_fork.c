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
#ifdef __linux__
#define _GNU_SOURCE
#include <sched.h>
#endif

#include "alcove.h"
#include "alcove_call.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

typedef struct {
    int ctl[2];
    int in[2];
    int out[2];
    int err[2];
} alcove_fd_t;

typedef struct {
    alcove_state_t *ap;
    alcove_fd_t *fd;
} alcove_arg_t;

static int alcove_stdio(alcove_fd_t *fd);
static int alcove_set_cloexec(int fd);
static void alcove_close_pipe(int fd[2]);
static int alcove_child_fun(void *arg);
static int alcove_parent_fd(alcove_state_t *ap, alcove_fd_t *fd, pid_t pid);
static int avail_pid(alcove_child_t *c, void *arg1, void *arg2);
static int stdio_pid(alcove_child_t *c, void *arg1, void *arg2);

/*
 * fork(2)
 *
 */
    ETERM *
alcove_fork(alcove_state_t *ap, ETERM *arg)
{
    alcove_arg_t child_arg = {0};
    alcove_fd_t fd = {{0}};
    pid_t pid = 0;

    if (pid_foreach(ap, 0, NULL, NULL, pid_equal, avail_pid) != 0)
        return alcove_errno(EAGAIN);

    if (alcove_stdio(&fd) < 0)
        return alcove_errno(errno);

    child_arg.ap = ap;
    child_arg.fd = &fd;

    pid = fork();

    switch (pid) {
        case -1:
            return alcove_errno(errno);
        case 0:
            (void)alcove_child_fun(&child_arg);
            erl_err_sys("fork");
        default:
            if (alcove_parent_fd(ap, &fd, pid) < 0)
                return alcove_errno(errno);

            return alcove_ok(erl_mk_int(pid));
    }
}

/*
 * clone(2)
 *
 */
    ETERM *
alcove_clone(alcove_state_t *ap, ETERM *arg)
{
#ifdef __linux__
    ETERM *hd = NULL;
    alcove_arg_t child_arg = {0};
    alcove_fd_t fd = {{0}};
    const size_t stack_size = 1024 * 1024;
    char *child_stack = NULL;
    int flags = 0;
    pid_t pid = 0;

    if (pid_foreach(ap, 0, NULL, NULL, pid_equal, avail_pid) != 0)
        return alcove_errno(EAGAIN);

    /* flags */
    arg = alcove_list_head(&hd, arg);
    if (!hd || !ERL_IS_INTEGER(hd))
        goto BADARG;

    flags = ERL_INT_UVALUE(hd);

    child_stack = calloc(stack_size, 1);
    if (!child_stack)
        return alcove_errno(errno);

    if (alcove_stdio(&fd) < 0)
        return alcove_errno(errno);

    child_arg.ap = ap;
    child_arg.fd = &fd;

    pid = clone(alcove_child_fun, child_stack + stack_size, flags | SIGCHLD, &child_arg);

    if (pid < 0)
        return alcove_errno(errno);

    free(child_stack);

    if (alcove_parent_fd(ap, &fd, pid) < 0)
        return alcove_errno(errno);

    return alcove_ok(erl_mk_int(pid));

BADARG:
    return erl_mk_atom("badarg");
#else
    return alcove_error("unsupported");
#endif
}

/*
 * setns(2)
 *
 */
    ETERM *
alcove_setns(alcove_state_t *ap, ETERM *arg)
{
#ifdef __linux__
    ETERM *hd = NULL;
    char *path = NULL;
    int fd = -1;
    int rv = 0;

    /* path */
    arg = alcove_list_head(&hd, arg);
    if (!hd || !ALCOVE_IS_IOLIST(hd))
        goto BADARG;

    if (erl_iolist_length(hd) > 0)
        path = erl_iolist_to_string(hd);

    if (!path)
        goto BADARG;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return alcove_errno(errno);

    rv = setns(fd, 0);

    (void)close(fd);

    return ( (rv < 0) ? alcove_errno(errno) : erl_mk_atom("ok"));

BADARG:
    return erl_mk_atom("badarg");
#else
    return alcove_error("unsupported");
#endif
}

/*
 * unshare(2)
 *
 */
    ETERM *
alcove_unshare(alcove_state_t *ap, ETERM *arg)
{
#ifdef __linux__
    ETERM *hd = NULL;
    int flags = 0;
    int rv = 0;

    /* flags */
    arg = alcove_list_head(&hd, arg);
    if (!hd || !ERL_IS_INTEGER(hd))
        goto BADARG;

    flags = ERL_INT_VALUE(hd);

    rv = unshare(flags);

    return ( (rv < 0) ? alcove_errno(errno) : erl_mk_atom("ok"));

BADARG:
    return erl_mk_atom("badarg");
#else
    return alcove_error("unsupported");
#endif
}

/*
 * clone flags
 *
 */
    ETERM *
alcove_clone_define(alcove_state_t *ap, ETERM *arg)
{
#ifdef __linux__
    ETERM *hd = NULL;
    char *flag = NULL;

    /* flag */
    arg = alcove_list_head(&hd, arg);
    if (!hd || !ERL_IS_ATOM(hd))
        goto BADARG;

    flag = ERL_ATOM_PTR(hd);

    if      (!strncmp(flag, "newns", 5))    return erl_mk_int(CLONE_NEWNS);
    else if (!strncmp(flag, "newuts", 6))   return erl_mk_int(CLONE_NEWUTS);
    else if (!strncmp(flag, "newipc", 6))   return erl_mk_int(CLONE_NEWIPC);
    else if (!strncmp(flag, "newuser", 7))  return erl_mk_int(CLONE_NEWUSER);
    else if (!strncmp(flag, "newpid", 6))   return erl_mk_int(CLONE_NEWPID);
    else if (!strncmp(flag, "newnet", 6))   return erl_mk_int(CLONE_NEWNET);
    else return erl_mk_atom("false");

BADARG:
    return erl_mk_atom("badarg");
#else
    return erl_mk_atom("false");
#endif
}

/*
 * Utility functions
 */
    int
alcove_stdio(alcove_fd_t *fd)
{
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd->ctl) < 0)
        return -1;

    if ( (alcove_set_cloexec(fd->ctl[0]) < 0)
         || (alcove_set_cloexec(fd->ctl[0]) < 0)) {
        alcove_close_pipe(fd->ctl);
        return -1;
    }

    if ( (pipe(fd->in) < 0)
            || (pipe(fd->out) < 0)
            || (pipe(fd->err) < 0)) {
        alcove_close_pipe(fd->ctl);
        alcove_close_pipe(fd->in);
        alcove_close_pipe(fd->out);
        alcove_close_pipe(fd->err);
        return -1;
    }

    return 0;
}

    static int
alcove_set_cloexec(int fd)
{
    int flags = 0;

    flags = fcntl(fd, F_GETFD, 0);
    if (flags < 0)
        return -1;

    return fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
}

    static void
alcove_close_pipe(int fd[2])
{
    /* fd should be above 0 since STDIN_FILENO is still open */
    if (fd[0] > 0)
        (void)close(fd[0]);

    if (fd[1] > 0)
        (void)close(fd[1]);
}

    int
alcove_child_fun(void *arg)
{
    alcove_arg_t *child_arg = arg;
    alcove_state_t *ap = child_arg->ap;
    alcove_fd_t *fd = child_arg->fd;
    long maxfd = sysconf(_SC_OPEN_MAX);
    int n = 0;

    if ( (close(fd->ctl[PIPE_WRITE]) < 0)
            || (close(fd->in[PIPE_WRITE]) < 0)
            || (close(fd->out[PIPE_READ]) < 0)
            || (close(fd->err[PIPE_READ]) < 0))
        return -1;

    if ( (dup2(fd->in[PIPE_READ], STDIN_FILENO) < 0)
            || (dup2(fd->out[PIPE_WRITE], STDOUT_FILENO) < 0)
            || (dup2(fd->err[PIPE_WRITE], STDERR_FILENO) < 0))
        return -1;

    /* Close all other fd's inherited from the parent. */
    for (n = 0; n < maxfd; n++) {
        if ( (n != fd->ctl[PIPE_READ])
                && (n != STDIN_FILENO)
                && (n != STDOUT_FILENO)
                && (n != STDERR_FILENO))
            (void)close(n);
    }

    alcove_ctl(ap);

    return 0;
}

    int
alcove_parent_fd(alcove_state_t *ap, alcove_fd_t *fd, pid_t pid)
{
    /* What to do if close(2) fails here?
     *
     * The options are ignore the failure, kill the child process and
     * return errno or exit (the child will be forced to exit as well
     * when stdin is closed).
     */
    if ( (close(fd->ctl[PIPE_READ]) < 0)
            || (close(fd->in[PIPE_READ]) < 0)
            || (close(fd->out[PIPE_WRITE]) < 0)
            || (close(fd->err[PIPE_WRITE]) < 0))
        erl_err_sys("alcove_parent_fd:close");

    return pid_foreach(ap, 0, fd, &pid, pid_equal, stdio_pid);
}

    static int
avail_pid(alcove_child_t *c, void *arg1, void *arg2)
{
    /* slot found */
    if (c->pid == 0)
        return 0;

    return 1;
}

    static int
stdio_pid(alcove_child_t *c, void *arg1, void *arg2)
{
    alcove_fd_t *fd = arg1;
    pid_t *pid = arg2;

    c->pid = *pid;
    c->fdctl = fd->ctl[PIPE_WRITE];
    c->fdin = fd->in[PIPE_WRITE];
    c->fdout = fd->out[PIPE_READ];
    c->fderr = fd->err[PIPE_READ];

    return 0;
}
