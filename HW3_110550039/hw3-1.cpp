#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

void count(int index) {
  static mutex io_mutex;
  static int current_index = 0;
  int num = 1000000;
  // Lock the critical section
  {
    lock_guard<mutex> lock(io_mutex);

    // Check if it's the turn of this thread to print
    while (index != current_index) {
      io_mutex.unlock();
      this_thread::yield(); // Give up the CPU to avoid busy waiting
      io_mutex.lock();
    }


    while (num--) {}
    //  {
          //lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
          
    //  }
    current_index++;
  }
  
}

int main(void) {
  thread t[100];

  for (int i = 0; i < 100; i++)
    t[i] = thread(count, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
