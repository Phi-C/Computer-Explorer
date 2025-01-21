// g++ -o test_volatile_ex2 test_volatile_ex2.cpp
// g++ -O2 -o test_volatile_ex2 test_volatile_ex2.cpp

#include <pthread.h>
#include <stdio.h>

// volatile int flag = 0;
int flag = 0;

void* worker_thread(void* arg) {
    printf("Worker thread started. Waiting for flag to be set...\n");

    while (flag == 0) {
    }

    printf("Worker thread detected flag change. Exiting...\n");
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker_thread, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    printf("Main thread is running. Press Enter to set the falg...\n");
    int user_input;
    if (scanf("%d", &user_input) != 1) {
        fprintf(stderr, "Invalid input, Please enter an interger.\n");
        return 1;
    }

    flag = user_input;
    printf("Main thread set the flag to %d.\n", flag);

    pthread_join(tid, NULL);
    printf("Main thread exiting.\n");

    return 0;
}
