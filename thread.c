#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

void *producer(void *arg);
void *consumer(void *arg);

int gNumber;//global variable
int count;

int main()
{

	int res1, res2;
	pthread_t pro_thread;
	pthread_t con_thread;
	void *pro_result;
	void *con_result;

	res1=pthread_create(&pro_thread, NULL, producer, NULL);
	res2=pthread_create(&con_thread, NULL, consumer, NULL);

	printf("waiting for thread to finish\n");

	res1=pthread_join(pro_thread, &pro_result);
	res2=pthread_join(con_thread, &con_result);

	if((int *)pro_result==(int *)con_result)
		printf("SUCCESS\n");

	else
		printf("Fail\n");

	printf("finished");
	exit(EXIT_SUCCESS);


}

void *producer(void *arg)
{
	int sum1=0;

	srand(time(NULL)); //initialize random variable

	for(int i=1;i<101;i++)
	{
		gNumber=rand()%100;

		printf("%d) gNumber in producer(): %d\n", i, gNumber);
		fflush(stdout);
		count=1;
		sum1=sum1+gNumber;

		sleep(1);
	}
	printf("producer finished\n");
	printf("producer sum is... %d\n", sum1);

	pthread_exit((void *)sum1);

}

void *consumer(void *arg)
{

	int cnt=0;
	int sum2=0;

	while(cnt<=99)
	{
		if(count==1)
		{
			cnt=cnt+1;
			printf("%d) gNumber in consumer(): %d\n", cnt, gNumber);
			fflush(stdout);

			count=0;
			sum2=sum2+gNumber;
		}
	}

	printf("consumer finished\n");
	printf("consumer sum is...%d\n", sum2);
	pthread_exit((void *)sum2);
}
