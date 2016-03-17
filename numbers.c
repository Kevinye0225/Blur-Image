#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


double max = 0;
double min = 0;
double sum = 0;
double input_value = 0;
// used to keep track of last 5 values entered
double fifth = 0;
double second = 0;
double third = 0;
double forth = 0;
int end_con = 0;
int count = 0;
char input[100] = {'\0'};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void* scanning(void* p);
void* printing(void* p);

int main(){


	pthread_t t1, t2;


    pthread_create(&t1, NULL, &scanning, NULL);
    pthread_create(&t2, NULL, &printing, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
        
	  

	return 0;


}

void* scanning(void* p){
	while (1){
		
		printf("Please enter a number:\n");
		scanf("%s", input);
		pthread_mutex_lock(&lock);
		
		// check user input
		if (input[0] == 'q'){
			end_con = 1;
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		// check if users enter 0
		if (atof(input) == 0){
			if (input[0] == 48){
				fifth = forth;
				forth = third;
				third = second;
		        second = input_value;

				input_value = 0;
				count++;
			} 
		} else {
			fifth = forth;
		    forth = third;
		    third = second;
            second = input_value;

			input_value = atof(input);
			sum += input_value;
			count++;
		}

		// change max and min value if necessary
		if (input_value >= max){
			max = input_value;
		}
		if (count == 1){
			min = input_value;
		}
		if (input_value <= min){
			min = input_value;
		}
		pthread_mutex_unlock(&lock);

		
	}
}

void* printing(void* p){

	while (end_con == 0){
		sleep(10);
		pthread_mutex_lock(&lock);
		// sleep(10);
		printf("Max value is %f\n", max);
		printf("Min value is %f\n", min);
		if (count != 0){
			printf("Average value is %f\n",  sum/count);
		} else {
			printf("Average value is %d\n", 0);
		}
		// only print the last 5 values if it exists
		if (count == 1){
			printf("Last 5 values: %f\n", input_value);
		}
		if (count == 2){
			printf("Last 5 values: %f, %f\n", input_value, second);

		}
		if (count == 3){
			printf("Last 5 values: %f, %f, %f\n", input_value, second, third);
		}
		if (count == 4){
			printf("Last 5 values: %f, %f, %f, %f\n", input_value, second, third, forth);
		}
		if (count >= 5){
			printf("Last 5 values: %f, %f, %f, %f, %f\n", input_value, second, third, forth, fifth);	
		}
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}