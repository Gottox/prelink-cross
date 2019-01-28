#ifndef HAVE_ERROR_H
// src: https://marc.info/?l=musl&m=140406253215517&w=2
extern void (*error_print_progname)(void);
extern unsigned int error_message_count;
extern int error_one_per_line;

void error(int, int, const char *, ...);
void error_at_line(int, int, const char *, unsigned int, const char *, ...);
#else
#	include <error.h>
#endif
