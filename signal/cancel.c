#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

pthread_t ThreadId[2];

// SIGINT 신호 받으면 다음 함수 실행 
void SigIntHandler(int signo)
{
	int i;

	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	for (i = 0; i < 2; i++) {
		if (pthread_cancel(ThreadId[i])) {
			perror("pthread_cancel");
			exit(1);
		}
	}
	for (i = 0; i < 2; i++) {
		if (pthread_join(ThreadId[i], NULL)) {
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Threads terminated: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

// 쓰레드1 에서 다음 함수 실행
void Thread1(void *dummy)
{
	printf("Thread %ld created......\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1);
}

// 쓰레드2 에서 다음 함수 실행 
void Thread2(void *dummy)
{
	printf("Thread %ld created......\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1);
}

int main()
{
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0) { // 쓰레드1 생성
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0) { // 쓰레드2 생성 
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler); // Ctrl+C 입력하면 SigIntHandler 실행 

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause(); // 시그널 받을때까지 대기 

	return 0;
}
