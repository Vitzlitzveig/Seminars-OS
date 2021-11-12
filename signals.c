#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


// ********** DONT STOP ME NOW ***********

static int dontStopMePos = 0;
static const char dontStopMeText[] =
        "\r  \t\t\tDon\'t stop me now                                \0"
        "\r  \t\t\tI\'m having such a good time                      \0"
		"\r  \t\t\tI\'m having a ball                                \0"
		"\r  \t\t\tDon\'t stop me now                                \0"
		"\r  \t\t\tIf you wanna have a good time just give me a call \0"
		"\r  \t\t\tDon\'t stop me now ('Cause I'm having a good time)\0"
		"\r  \t\t\tDon\'t stop me now (Yes I'm havin' a good time)   \0"
		"\r  \t\t\tI don\'t want to stop at all                      \0\0";


void mode0()
{
	pid_t pid = getpid();
	while (1)
	{
		printf("Doing while(1)... [pid=%d]\n", pid);
		sleep(1);
	}
}

void dontStopMeHandler(int sig)
{
	if (dontStopMeText[dontStopMePos] == 0)
		dontStopMePos = 0;
	dontStopMePos += printf("%s\n", dontStopMeText+dontStopMePos);
//	printf("%d\n", sig);
}

void mode1()
{
	/* Нерекомендованный способ: такой вызов не блокирует прерывания во время обработки предыдущего
	 * sighandler_t signal(int signum, sighandler_t handler);
	 * signal(SIGINT, stopHandler);
	 */
	// #include <signal.h>
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = dontStopMeHandler;
	sigaction(SIGINT, &act, NULL);

	pid_t pid = getpid();
	while (1)
	{
		printf("Doing while(1)... [pid=%d]\n", pid);
		sleep(1);
	}
}


// ********** SIGUSR + SIGRTnn ***********

void echoHandler(int sig)
{
	sleep(1);
	printf("Got signal %d!\n", sig);
}

void mode2()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = echoHandler;

	sigemptyset(&(act.sa_mask));
	sigaddset(  &(act.sa_mask), SIGINT);

	/* Пользовательские сигналы
	 * SIGUSR1,2 = 10,12
	 * SIGRTMIN..SIGRTMAX = 34..64
	 */
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	for (int sig = SIGRTMIN; sig <= SIGRTMAX; ++sig)
		sigaction(sig, &act, 0);

	pid_t pid = getpid();
	printf("Waiting for signals...\n");
	printf("[pid=%d]\n", pid);

	raise(40);
	while (1) sleep(1);
}


// ********** Don't give a ***************

void mode3()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;

	for (int sig = 0; sig <= SIGRTMAX; ++sig)
	{
		if (sigaction(sig, &act, 0))
			printf("Can\'t set signal handler for %d\n", sig);
	}

	pid_t pid = getpid();
	printf("My PID is %d and I don\'t care about your signals\n", pid);


	while (1) sleep(1);
}


int main()
{
	mode2();
	return 0;
}
