#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>

using namespace std;

class WorkQueue {
  public:
    WorkQueue() {}

    void push(int i) {
      unique_lock<mutex> lock(mtx);
      queue.push_back(i);
      if (queue.size() == 1) {
        cv.notify_one();
      }
    }

    int pop() {
      unique_lock<mutex> lock(mtx);
      cout << "Waiting for work" << endl;
      cv.wait(lock, [this] { return !queue.empty(); });
      int i = queue.front();
      queue.pop_front();
      return i;
    }

  private:
    list<int> queue;
    mutex mtx;
    condition_variable cv;
};

class Writer {
  public:
    Writer(WorkQueue& queue): queue(queue) {}

    void operator()() {
      for (int i = 0; i < 1000; i++) {
        cout << "Write: " << i << endl;
        queue.push(i);
        this_thread::sleep_for(chrono::milliseconds(1000));
      }
    }
  private:
    WorkQueue& queue;
};

class Reader {
  public:
    Reader(WorkQueue& queue): queue(queue) {}

    void operator()() {
      for (int i = 0; i < 10000; i++) {
        cout << "try read" << endl;
        auto j = queue.pop();
        cout << "Read: " << j << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
      }
    }
  private:
    WorkQueue& queue;
};


int main() {
  WorkQueue q;
  list<thread> writers;
  for (int i = 0; i < 10; i++) {
    writers.push_back(thread(Writer(q)));
  }
  auto reader = thread(Reader(q));
  reader.join();
  for (auto& writer: writers) {
    writer.join();
  }
}