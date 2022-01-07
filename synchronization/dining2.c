#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN 5
#define NLOOPS 5

#define LEFT(i) ((i+NUM_MEN-1) % NUM_MEN)
#define RIGHT(i) ((i+1) % NUM_MEN)

enum { THINKING, EATING, HUNGRY };

sem_t Philosopher[NUM_MEN];
sem_t Mutex;
int State[NUM_MEN];

void ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

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

void Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

// 본인이 식사를 할 수 있는지 테스트하는 함수 
void Test(int id)
{
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING && State[RIGHT(id)] != EATING) { // State가 HUNGRY이고 양쪽사람이 먹고있지 않으면 
		State[id] = EATING; // State를 EATING으로 

		if (sem_post(&Philosopher[id]) < 0) { // 세마포어 +1
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

// 젓가락 드는 함수 
void PickUp(int id)
{
	if (sem_wait(&Mutex) < 0) { // mutex 0이면 대기, 아니면 -1
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY;

	Test(id);

	if (sem_post(&Mutex) < 0) { // Mutex +1
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_wait(&Philosopher[id]) < 0) { // 세마포어 -1 
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

// 젓가락 내려놓는 함수 
void PutDown(int id)
{
	if (sem_wait(&Mutex) < 0) { // mutex 0이면 대기, 아니면 -1 
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = THINKING;

	Test(LEFT(id)); 
	Test(RIGHT(id));

	if (sem_post(&Mutex) < 0) { // mutex +1
		perror("sem_post");
		pthread_exit(NULL);
	}
}

// NLOOPS만큼 식사하고 종료하는 함수 
void DiningPhilosopher(int *pId)
{
	int i;
	int id = *pId;

	for (i = 0; i < NLOOPS; i++) {
		Thinking(id);
		PickUp(id);
		Eating(id);
		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[NUM_MEN];
	int i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0; i < NUM_MEN; i++) {
		if (sem_init(&Philosopher[i], 0, 0) < 0) { // 사람수만큼 세마포어 생성 
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	if (sem_init(&Mutex, 0, 1) < 0) { // Mutex 세마포어 생성 
		perror("sem_init");
		exit(1);
	}

	for (i = 0; i < NUM_MEN; i++) {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, (void *)&id[i]) < 0) { // 사람수만큼 쓰레드 생성 
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0; i < NUM_MEN; i++) {
		if (pthread_join(tid[i], NULL) < 0) { // 쓰레드 종료까지 대기 
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&Philosopher[i]) < 0) { // 세마포어 삭제 
			perror("sem_destroy");
		}
	}

	return 0;
}

