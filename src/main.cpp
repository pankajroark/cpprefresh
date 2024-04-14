#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

auto gap = chrono::nanoseconds(1);

auto createThreadFunction(int i) {
  return [i]() {
    cout << "Hello from thread " << i << "!" << endl;
    for (int j = 0; j < 10; j++) {
      this_thread::sleep_for(gap);
      cout << "From thread " << i << ": " << j << endl;
    }
  };
}

void threadFunction() {
  cout << "Hello from thread!" << endl;
  for (int i = 0; i < 100; i++) {
    this_thread::sleep_for(gap);
    cout << "From thread: " << i << endl;
  }
}

int main() {
  cout << "Hello from main!" << endl;
  auto threads = vector<thread>();
  for (int i = 0; i < 10; i++) {
    threads.push_back(thread(createThreadFunction(i)));
  }
  for (int i = 0; i < 10; i++) {
    this_thread::sleep_for(gap);
    cout << "From main: " << i << endl;
  }
  for (int i = 0; i < 10; i++) {
    threads[i].join();
  }
  cout << "Back in main" << endl;
  return 0;
}