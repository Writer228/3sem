/*При N = 1 программа работает быстро (1 секунда) и правильно.
 * При N = 2 программа в 1 случае из 3х выдает правильный ответ и работает в среднем 8.5 секунд.
 * При N = 3 программа работает не правильно и в течение 20 секунд.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

# define N 2

/*
 * FIXIT:
 * Для 10^8 нужна отдельная константа.
 * И странно, что такой большой массив получилось выделить на стеке...
 * Обычно стараются на куче столько много памяти просить. 
 */
double value[100000000];
double sum[N];
double sum_square[N];

void* my_thread(void *dummy)
{
	int k = *(long int*) dummy;
	unsigned long long int size, i = 0;
	size = 100000000/N;
	sum[k] = 0;
	sum_square[k] = 0;
	
  /*
    sum[k] += value[i + k * size];
   */
	while(i < size)
	{
		sum[k] = sum[k] + value[i + k*size];	
		sum_square[k] = sum_square[k] + (value[i + k*size]) * (value[i + k*size]);		
		i++;
	}
	return NULL;
}

int main()
{
	pthread_t thread_id[N];
	int result;
	double a = 0, b = 0;
	double average = 0, average_square = 0;
	unsigned long long int i = 0;
  
  /*
   * FIXIT:
   * Не надо лепить while, где "направшивается" обычный for.
   * Относится ко всем while ниже.
   */
	while (i < 100000000)
	{
		value[i] = random() %7;
		i++;
	}
	i = 0;
	
	clock_t begin = clock();
	while (i < N)
	{
		result = pthread_create(thread_id + i, (pthread_attr_t *)NULL, my_thread, &i);
		if (result > 0)
		{
			printf("Can't create thread");
		}
		i++;
    /*
     * FIXIT:
     * Если у вас операции i++ выполнится быстрее, чем int k = *(long int*) dummy;
     * то результат будет неверный. В презентации к семинару 5 есть отдельный слайд, посвященный этой проблеме.
     */
	}
	i = 0;
	
	while (i < N)
	{
		pthread_join(thread_id[i], (void**)NULL);
		i++;
	}
	i = 0;
	clock_t end = clock();
	
	while (i < N)
	{
		a = a + sum[i];
		b = b + sum_square[i];
		i++;
	}
	average = a/100000000;
	average_square = b/100000000;
	b = average_square - average * average;
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("average value = %f \ndispersion = %f\ntime spent = %f\n ", average, b, time_spent);
  /*
   * http://stackoverflow.com/questions/19616895/increasing-pthreads-thread-count-has-no-effect-on-speed
   * Я забыл про эту особенность clock'a, поэтому ускорение скорее всего вы не получили.
   */
	return 0;
}
