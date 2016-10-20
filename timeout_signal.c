#include "timeout_signal.h"

void timeout_signal_handler(int sig)
{	
	siglongjmp(invoke_env, 1);
}

sigfunc* timeout_set_signal(int signo, sigfunc *func)
{
	struct sigaction act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	} else {
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;
	return oact.sa_handler;
}


