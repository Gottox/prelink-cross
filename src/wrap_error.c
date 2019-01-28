#ifndef HAVE_ERROR_H
// src: https://marc.info/?l=musl&m=140406253215517&w=2

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrap_error.h"

extern char *__progname;

void (*error_print_progname)(void) = 0;
unsigned int error_message_count = 0;
int error_one_per_line = 0;

static void eprint(int status, int e, const char *file, unsigned int line, const char *fmt, va_list ap)
{
	if (file && error_one_per_line) {
		static const char *oldfile;
		static unsigned int oldline;
		if (line == oldline && strcmp(file, oldfile) == 0)
			return;
		oldfile = file;
		oldline = line;
	}
	if (error_print_progname)
		error_print_progname();
	else
		fprintf(stderr, "%s: ", __progname);
	if (file)
		fprintf(stderr, "%s:%u: ", file, line);
	vfprintf(stderr, fmt, ap);
	if (e)
		fprintf(stderr, ": %s\n", strerror(e));
	error_message_count++;
	if (status)
		exit(status);
}

void error(int status, int e, const char *fmt, ...)
{
	va_list ap;
	va_start(ap,fmt);
	eprint(status, e, 0, 0, fmt, ap);
	va_end(ap);
}

void error_at_line(int status, int e, const char *file, unsigned int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap,fmt);
	eprint(status, e, file, line, fmt, ap);
	va_end(ap);
}
#endif
