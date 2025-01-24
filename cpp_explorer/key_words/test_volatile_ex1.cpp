// g++ -o test_volatile_ex1 test_volatile_ex1.cpp
// g++ -O2 -o test_volatile_ex1 test_volatile_ex1.cpp

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// volatile int g_flag = 0;
int g_flag = 0;

void* worker_thread(void* arg) {
    printf("Worker thread started. Waiting for g_flag to be set...\n");

    pthread_mutex_lock(&lock);
    while (g_flag == 0) {
        pthread_cond_wait(&cond, &lock); // wait for condition
    }
    pthread_mutex_unlock(&lock);

    printf("Worker thread detected g_flag change. Exiting...\n");
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

    pthread_mutex_lock(&lock);
    g_flag = user_input;
    pthread_cond_signal(&cond); // signal woker thread
    pthread_mutex_unlock(&lock);

    printf("Main thread set the g_flag to %d.\n", g_flag);

    pthread_join(tid, NULL);
    printf("Main thread exiting.\n");

    return 0;
}
