/*
 * this file comes from `Advanced Unix programming`
 */
#include "../include/clib_error.h"

static int dbg_mode;

static void err_common(int has_errno, int error, const char *fmt,
		       va_list ap)
{
	char buf[MAXLINE];
	memset(buf, 0, MAXLINE);
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (has_errno)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
			 strerror(errno));
	size_t len = strlen(buf);
	if (len >= MAXLINE-1) {
		buf[MAXLINE-5] = '.';
		buf[MAXLINE-4] = '.';
		buf[MAXLINE-3] = '.';
		buf[MAXLINE-2] = '\n';
		buf[MAXLINE-1] = '\0';
	} else
		buf[len] = '\n';
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

void err_msg(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_common(0, 0, fmt, ap);
	va_end(ap);
}

void err_sys(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_common(1, errno, fmt, ap);
	va_end(ap);
}

void err_dbg(int has_errno, const char *fmt, ...)
{
	if (dbg_mode) {
		va_list ap;

		va_start(ap, fmt);
		if (has_errno)
			err_common(1, errno, fmt, ap);
		else
			err_common(0, 0, fmt, ap);
	}
}

void err_dbg1(int errval, const char *fmt, ...)
{
	if (dbg_mode) {
		va_list ap;

		va_start(ap, fmt);
		err_common(1, errval, fmt, ap);
		va_end(ap);
	}
}

void err_dump(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_common(1, errno, fmt, ap);
	va_end(ap);

	abort();
	exit(1);
}

void err_exit(int flag_err, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_common(flag_err, flag_err ? errno : 0, fmt, ap);
	va_end(ap);

	exit(0);
}

int err_ret(int has_errno, int retval, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_dbg(has_errno, fmt, ap);
	va_end(ap);

	return retval;
}

void set_dbg_mode(int val)
{
	dbg_mode = val;
}

int get_dbg_mode(void)
{
	return !!dbg_mode;
}