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

//Struct allows us to pass a pointer to the thread creation which only allows us to pass a single argument and
//arrays decay into a pointer to the first element because c. This allows multiple arguments to be passed to
//thread creation.
struct array_info{
    int arraySize;
    int *array;
}array_info;

//Global variables.
float avg = 0;
int min = INT_MAX;
int max = INT_MIN;

//Prototypes for the functions being passed to thread creation.
void *average(void *);
void *minimum(void *);
void *maximum(void *);

int main(int argc, char *argv[]){
    //pthread_t keeps track of thread ids.
    pthread_t avgThread, minThread, maxThread;
    //Array to hold the numbers as they are cast to integers.
    int numberList[argc];
    //Checking to make sure at least 2 numbers have been provided.
    if(argc < 2){
        fprintf(stderr, "%s %s %d\n", "Incorrect number of arguments.", "Expected at least 2 arguments; Received ", (argc-1));
        printf("%s\n", "usage: stats 'num1' 'num2' '...' 'numN'");
        return (-1);
    }
    else{
        //Casting received numbers to integers
        for(int i = 0; i < argc-1; i++){
            numberList[i] = atoi(argv[i+1]);    
        }
        
        //Giving the struct the information about the provided array. The number of elements, and a pointer to the array
        //is given to the struct
        array_info.arraySize = (argc-1);
        array_info.array = numberList;
        
        //Thread creation. First argument is the pthread_t value to keep track of the thread. Second argument is any
        //attributes the thread needs, we set this to NULL as we don't need any. Third argument is the function the
        //thread is going to execute. The Fourth argument is a void pointer typecast of the struct, as this argument must be a
        //void pointer.
        if(pthread_create(&avgThread, NULL, &average, (void *)&array_info)){
            perror("pthread avgThread initialization failed.");
            exit(1);
        }
        
        if(pthread_create(&minThread, NULL, &minimum, (void *)&array_info)){
            perror("pthread minThread initialization failed.");
            exit(1);
        }
        if(pthread_create(&maxThread, NULL, &maximum, (void *)&array_info)){
            perror("pthread maxThread initialization failed.");
            exit(1);
        }    
    }
    
    //Waiting for the threads to complete before continuing.
    pthread_join(avgThread, NULL);
    pthread_join(minThread, NULL);
    pthread_join(maxThread, NULL);
    
    //Printing out the results of each function.
    printf("%s %.2f\n", "The average value is ", avg);
    printf("%s %d\n", "The minimum value is ", min);
    printf("%s %d\n", "The maximum value is ", max);
    return (0);
}

//Stat Functions
//Averaging function. Takes a void pointer of the struct and uses the arraySize to iterate through the
//array while adding each element of array to the global variable average and finally divides the total
//by the number of elements. Last line marks the thread to exit and join at the pthread_join in main.
void *average(void *vNumberList){
    struct array_info *arrayInfo = vNumberList;
    for(int i = 0; i < (arrayInfo -> arraySize); i++){
        avg += arrayInfo -> array[i];    
    }    
    avg = avg / arrayInfo -> arraySize;
    pthread_exit(NULL);
}

//Minimum function. Acquires information the same way as the averaging function. Compares each element
//of array to the current min stored in the global variable min. Min starts as a MAX_INT.
void *minimum(void *vNumberList){
    struct array_info *arrayInfo = vNumberList;
    for(int i = 0; i < (arrayInfo -> arraySize); i++){
        if(arrayInfo -> array[i] < min){
            min = arrayInfo -> array[i];    
        }
    }
    pthread_exit(NULL);
}

//Maximum function. Acquires information the same way as the averaging function. Compares each element
//of array to the current max stored in the global variable max. Max starts as a MIN_INT.
void *maximum(void *vNumberList){
    struct array_info *arrayInfo = vNumberList;
    for(int i = 0; i < (arrayInfo -> arraySize); i++){
        if(arrayInfo -> array[i] > max){
            max = arrayInfo -> array[i];
        }    
    }
    pthread_exit(NULL);
}
