#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <thread>


class ConcMap {
  private:
    std::map<std::string, int> m;
    std::mutex mtx;

  
  public:
    void insert(const std::string& key, int value) {
      std::lock_guard<std::mutex> lock(mtx);
      m[key] = value;
    }

    void erase(const std::string& key) {
      std::lock_guard<std::mutex> lock(mtx);
      m.erase(key);
    }

    int& operator[](const std::string& key) {
      std::lock_guard<std::mutex> lock(mtx);
      return m[key];
    }

    void print() {
      for (const auto& pair : m) {
        std::cout << pair.first << " is " << pair.second << " years old." << std::endl;
      }
    }
};

void writer(ConcMap& cm, int start) {
  for (int i = 0; i < 100; i++) {
    cm.insert(std::to_string(i), i + start);
  }
}

int main() {
  ConcMap ages;
  ages["Alice"] = 30;
  ages["Bob"] = 40;

  std::thread t1(writer, std::ref(ages), 0);
  std::thread t2(writer, std::ref(ages), 100);
  t1.join();
  t2.join();

  ages.print();
  return 0;
}