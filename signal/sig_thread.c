#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_MAIN
/** #define THREAD_1 */
/** #define THREAD_2 */

// SIGINT 시그널 받으면 실행하는 함수 
void SigIntHandler(int signo)
{
	printf("Received a SIGINT signal by thread %ld\n", pthread_self()); // 현재 실행중인 쓰레드 아이디 출력 
	printf("Terminate this process\n");

	exit(0);
}

void Thread1(void *dummy)
{
#ifdef THREAD_1 // THREAD_1이 선언되어있으면 쓰레드1에서 시그널 함수 실행
	signal(SIGINT, SigIntHandler);
#endif

	while (1);
}

void Thread2(void *dummy)
{
#ifdef THREAD_2 // THREAD_2가 선언되어있으면 쓰레드2에서 시그널 함수 실행 
	signal(SIGINT, SigIntHandler);
#endif

	while (1);
}

int main()
{
	pthread_t tid1, tid2;

	if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0) { // 쓰레드1 생성 
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0) { // 쓰레드2 생성 
		perror("pthread_create");
		exit(1);
	}
	printf("Create two threads: tid1=%ld, tid2=%ld\n", tid1, tid2); // 쓰레드1, 2 아이디 출력 
	printf("Main thread: tid=%ld\n", pthread_self()); // 메인 쓰레드 출력 

#ifdef THREAD_MAIN 
	signal(SIGINT, SigIntHandler); // Ctrl+C 입력들어오면 SigIntHandler 함수 실행 
#endif
	
	printf("Press ^C to quit\n");

	for ( ; ; )
		pause(); // 시그널 받을때까지 대기 

	return 0;
}

