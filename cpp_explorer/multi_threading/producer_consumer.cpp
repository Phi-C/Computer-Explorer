#include <assert.h>
#include <condition_variable>
#include <iostream>
#include <thread>

#define CAN_PRODUCE (count < 5)
#define CAN_CONSUME (count > 0)

int count = 0;
std::mutex mtx;
std::condition_variable consumer_cv, producer_cv;

void producer() {
    while (1) {
        std::unique_lock<std::mutex> lk(mtx);
        while (!CAN_PRODUCE) {
            // 缓冲区如果满了, 生产者需要等待消费者消费(减少count)
            // std::condition_variable::wait() will release the lock and block
            // the current thread until the condition variable is notified. When
            // the condition variable is notified, it will reacquire the lock
            // before returning from the wait() call. This is why we need to use
            // std::unique_lock<std::mutex> instead of
            // std::lock_guard<std::mutex>. cv.wait() must use unique_lock
            // because it automatically releases the lock while waiting and
            // reacquires the lock upon being woken up.
            consumer_cv.wait(lk);
        }
        assert(CAN_PRODUCE);
        count++;
        printf("(");
        producer_cv.notify_one();
    }
}

void consumer() {
    while (1) {
        std::unique_lock<std::mutex> lk(mtx);
        while (!CAN_CONSUME) {
            producer_cv.wait(lk);
        }
        assert(CAN_CONSUME);
        count--;
        printf(")");
        consumer_cv.notify_one();
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}