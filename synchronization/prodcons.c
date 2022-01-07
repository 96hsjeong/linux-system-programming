#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType Buf;
sem_t EmptySem, FullSem, MutexSem;

void ThreadUsleep(int usecs)
{
	pthread_cond_t cond;
	pthread_mutex_t	mutex;
	struct timespec ts;
	struct timeval tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Producer(void *dummy)
{
	int i, data;

	printf("Producer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (sem_wait(&EmptySem) < 0) { // EmptySem Wait -1 (0이면 대기), 버퍼가 꽉차있으면 대기 
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&MutexSem) < 0) { // MutexSem Wait -1 (0이면 대기), 다른 쓰레드에서 실행 중이면 대기  
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000; // 임의의 데이터 생성 
		Buf.buf[Buf.in].data = data; // 버퍼에 데이터 넣기 
		Buf.in = (Buf.in + 1) % MAX_BUF; // 다음에 데이터넣을 인덱스 
		Buf.counter++; // 카운터 증가 

		if (sem_post(&MutexSem) < 0) { // MutexSem Signal +1, 작업 끝나면 1
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&FullSem) < 0) { // FullSem Signal +1
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL); // 쓰레드 종료 
}

void Consumer(void *dummy)
{
	int i, data;

	printf("Consumer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (sem_wait(&FullSem) < 0) { // FullSem Wait -1 (0이면 대기), 버퍼가 비어있으면 대기 
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&MutexSem) < 0) { // MutexSem Wait -1 (0이면 대기), 다른 쓰레드에서 실행 중이면 대기  
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; // 버퍼에서 데이터 꺼내기 
		Buf.out = (Buf.out + 1) % MAX_BUF; // 다음에 데이터 꺼낼 인덱스 
		Buf.counter--; // 카운터 감소 

		if (sem_post(&MutexSem) < 0) { // MutexSem Signal +1, 작업 끝나면 1
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&EmptySem) < 0) { // EmptySem Signal +1
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i); 
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL); // 쓰레드 종료 
}

int main() 
{
	pthread_t tid1, tid2;

	srand(0x9999);

	if (sem_init(&EmptySem, 0, MAX_BUF) < 0) { // 버퍼에 비어있는 수를 카운트해주는 세마포어 생성
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0) { // 버퍼에 데이터가 들어있는 수를 카운트해주는 세마포어 생성 
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0) { // Mutual Exclusion Semaphore 생성 
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0) { // 쓰레드 생성 
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0) { // 쓰레드 생성 
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0) { // 쓰레드 종료까지 대기 
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) { // 쓰레드 종료까지 대기
		perror("pthread_join");
		exit(1);
	}

	printf("Main : %d items in buffer.....\n", Buf.counter);

	if (sem_destroy(&EmptySem) < 0) { // 세마포어 삭제 
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0) { // 세마포어 삭제 
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0) { // 세마포어 삭제 
		perror("sem_destroy");
	}

	return 0;
}

