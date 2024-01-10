#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

// executed by thread 1 
void *sum(void *thread_id) {
    int *arr = (int *)thread_id; // point to array passed as params
    printf("thread_1 (TID %lu) reads X = %d and Y = %d from input_array[]\n", (unsigned long)pthread_self(), arr[0], arr[1]);
    // adding x + y and putting into inp_arr[2]
    arr[2] = arr[0] + arr[1];
    printf("thread_1 (TID %lu) writes X + Y = %d to the input_array[2]\n", (unsigned long)pthread_self(), arr[2]);
    pthread_exit(NULL);
}

// executed by thread 2 (second)
void *multiplication(void *thread_id) {
    int *arr = (int *)thread_id; // point to array passed as params
    arr[3] = arr[0] * arr[1];
    printf("thread_2 (TID %lu) reads X and Y from input_array[], writes X * Y = %d to input_array[3]\n", (unsigned long)pthread_self(), arr[3]);
    pthread_exit(NULL);
}

// executed by thread 2 (first)
void *even_odd(void *thread_id) {
    int *arr = (int *)thread_id; // point to array passed as params
    printf("thread_2 (TID %lu) reads %d from the input_array[2]\n", (unsigned long)pthread_self(), arr[2]);

    // checking if even or odd
    if ((arr[2] % 2) == 0) {
        // number is even
        printf("thread_2 (TID %lu) identifies that %d is an even number\n", (unsigned long)pthread_self(), arr[2]);
    } else {
        // number is odd
        printf("thread_2 (TID %lu) identifies that %d is an odd number\n", (unsigned long)pthread_self(), arr[2]);
    }
    // calling multiplication function
    multiplication(thread_id);
    return NULL;
}

// executed by thread 3
void *reverse_num(void *thread_id) {
    int *arr = (int *)thread_id; // point to array passed as params
    printf("thread_3 (TID %lu) reads %d from input_array[3]\n", (unsigned long)pthread_self(), arr[3]);

    // reverse number
    int reversed= 0;
    int num = arr[3];
    while (num > 0) {
        int digit = num % 10;
        reversed = reversed * 10 + digit;
        num /= 10;
    }
    printf("thread_3 (TID %lu) reverses the number %d -> %d\n\n", (unsigned long)pthread_self(), arr[3], reversed);
    pthread_exit(NULL);
}

// main function
int main(int argc, char *argv[]) {
    // checking for correct number of arguments
    if (argc != 3) {
        printf("need 2 arguments (ints)");
        return 1;
    }

    printf("parent (PID %d) receives X = %s and Y = %s from the user\n", getpid(),argv[1],argv[2]);

    // array to hold arguments
    int input_array[4];
    input_array[0] = atoi(argv[1]);
    input_array[1] = atoi(argv[2]);

    printf("parent (PID %d) writes X = %d and Y = %d to input_array[]\n", getpid(),input_array[0],input_array[1]);

    pthread_t thread_1, thread_2, thread_3;

    // creating thread1 and calling sum w/ array as arguments
    if (pthread_create(&thread_1, NULL, sum, (void *)input_array)) {
        printf("Error creating thread 1");
        exit(1);
    }
    
    // creating thread2 and calling even_odd w/ array as arguments
    if (pthread_create(&thread_2, NULL, even_odd, (void *)input_array)) {
        printf("Error creating thread 2");
        exit(1);
    }

    // creating thread2 and calling even_odd w/ array as arguments
    if (pthread_create(&thread_3, NULL, reverse_num, (void *)input_array)) {
        printf("Error creating thread 2");
        exit(1);
    }

    // wait for threads to finish
    pthread_join(thread_1, NULL); 
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);
    // default return statement
    return 0;
}
