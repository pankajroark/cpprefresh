#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void threadFunction(int id) {
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * id));

    // Acquire the lock
    std::unique_lock<std::mutex> lock(mtx);

    // Set ready to true
    ready = true;
    std::cout << "Thread " << id << " is ready." << std::endl;

    // Notify waiting thread
    cv.notify_one();
}

int main() {
    // Create threads
    std::cout << "Main is starting threads." << std::endl;
    std::thread setter = std::thread(threadFunction, 2);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return ready; });
    }
    std::cout << "Threads is ready, now main can proceed." << std::endl;

    // Join threads
    setter.join();

    return 0;
}
