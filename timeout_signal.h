#ifndef PHP_TIMEOUT_SIGNAL_H
#define PHP_TIMEOUT_SIGNAL_H

#include <setjmp.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>


sigjmp_buf invoke_env;

void timeout_signal_handler(int);
typedef void sigfunc(int);
sigfunc *my_signal(int, sigfunc*);


#endif /* PHP_TIMEOUT_SIGNAL_H */
