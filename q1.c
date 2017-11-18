/* Assignment 3: Question 1
 * November 18, 2017
 * Cameron Novina, 991400962
 * Program calculates various statistical information (average, minimum, maximum)
 * of a list of numbers passed from the command line. For each statistic the
 * the program will generate a new thread in order to calculate it with the 
 * parent thread output the calculated values.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

//Global variables
int avg = 0;
int min = INT_MAX;
int max = INT_MIN;

void *average(void *numberList);
void *minimum(void *numberList);
void *maximum(void *numberList);

int main(int argc, char *argv[]){
    
    pthread_t avgThread, minThread, maxThread;

    if(argc < 2){
        fprintf(stderr, "%s %s %d\n", "Incorrect number of arguments", "Expected at least 2 arguments; Received ", (argc-1));
        printf("%s\n", "usage: stats 'num1' 'num2' '...' 'numN'");
        return (-1);
    }

    else{
        if(pthread_create(&avgThread, NULL, &average, (void *)argv)){
            perror("pthread avgThread initialization failed.");
            exit(1);
        }
        if(pthread_create(&minThread, NULL, &minimum, (void *)argv)){
            perror("pthread minThread initialization failed.");
            exit(1);
        }
        if(pthread_create(&maxThread, NULL, &maximum, (void *)argv)){
            perror("pthread maxThread initialization failed.");
            exit(1);
        }
    }
    pthread_join(avgThread, NULL);
    pthread_join(minThread, NULL);
    pthread_join(maxThread, NULL);
    return (0);
}

//Stat Functions
void *average(void *vNumberList){
    int *numberList = vNumberList;
    int numberCount = (sizeof(numberList)/sizeof(numberList[0]));
    for(int i; i < numberCount; i++){
        avg += numberList[i];    
    }    
    avg = avg / numberCount;
    pthread_exit(NULL);
}

void *minimum(void *vNumberList){
    int *numberList = vNumberList;
    int numberCount = (sizeof(numberList)/sizeof(numberList[0]));
    for(int i; i < numberCount; i++){
        if(numberList[i] < min){
            min = numberList[i];    
        }
    }
    pthread_exit(NULL);
}

void *maximum(void *vNumberList){
    int *numberList = vNumberList;
    int numberCount = (sizeof(numberList)/sizeof(numberList[0]));
    for(int i; i < numberCount; i++){
        if(numberList[i] > max){
            max = numberList[i];
        }    
    }
    pthread_exit(NULL);
}
