#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int money, f1_money, f2_money;
int temp=1;
sem_t bin_sem;

void critical_region(char *p)//woori bank
{
	if(p=="f1")
	{
		money=money-10000;
		f1_money=10000;
		printf("In bank: f1 took 10000\n");
		printf("The amount of money(bank) = %d\n", money);
		sleep(1);
	}

	else if(p=="f2")
	{
		money=money+10000;
		f2_money=0;
		printf("In bank: f2 paid back 10000\n");
		printf("The amount of money(bank) = %d\n", money);
		sleep(1);
	}

}

void noncritical_region(char *p)//market
{
	if(p=="f1")
	{
		f1_money=f1_money-10000;
		printf("f1 bought some snacks\n");
		sleep(1);
	}

	else if(p=="f2")
	{
		f2_money=f2_money+10000;
		printf("f2 sold some snacks\n");
		sleep(1);
	}
}

static void* f1(void* p)
{
	for(int i=0;i<10;i++)
	{
		while(temp!=1)
		{
			sem_wait(&bin_sem);
		}
		critical_region(p);
		printf("The amount of money(f1) after bank: %d\n", f1_money);
		temp=2; //wakeup(f2)
		noncritical_region(p);
		printf("The amount of money(f1) after market: %d\n", f1_money);
	}

	return NULL;
}

static void* f2(void* p)
{
	for(int i=0;i<10;i++)
	{
		while(temp!=2){
			sem_post(&bin_sem);
		}
		critical_region(p);
		printf("The amount of money(f2) after bank: %d\n", f2_money);
		temp=1; //wakeup(f1)
		noncritical_region(p);
		printf("The amount of money(f2) after market: %d\n", f2_money);
	}
	return NULL;
}

int main()
{
	int rc;

	money=10000;

	pthread_t t1, t2;

	rc=pthread_create(&t1, NULL, f1, "f1");

	if(rc!=0)
	{
		fprintf(stderr, "pthread f1 failed\n");
		return EXIT_FAILURE;
	}

	rc=pthread_create(&t2, NULL, f2, "f2");

	if(rc!=0)
	{
		fprintf(stderr, "pthread f2, failed\n");
		return EXIT_FAILURE;
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&bin_sem);

	puts("All threads finished.");

	return 0;
}

