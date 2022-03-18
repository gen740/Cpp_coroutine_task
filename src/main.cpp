#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "generator.hpp"
#include "taskQue.hpp"

using Flier::Task::generator;
using Flier::TaskQue::taskQue;

generator test() {
  for (int i = 0; i < 4; ++i) {
    std::cout << "task_one " << i << std::endl;
    co_yield true;
  }
  co_return;
}

generator test2() {
  // co_yield test();
  for (int i = 0; i < 4; ++i) {
    std::cout << "task_two " << i << std::endl;
    co_yield true;
  }
  co_return;
}

generator test3() {
  for (int i = 0; i < 7; ++i) {
    std::cout << "task_three " << i << std::endl;
    co_yield true;
  }
  co_return;
  // co_yield test2();
}

auto main() -> int {
  taskQue maintask;
  maintask.push_back(test3(), "test3");
  maintask.push_back(test2(), "test2");
  std::cout << "=========== start ===========" << std::endl;
  for (int i = 0; i < 20; ++i) {
    maintask();
    for (auto&& i : maintask.get_task_list()) {
      std::cout << "task = " << i << std::endl;
    }
    std::cout << "=========== next ===========" << std::endl;
  }
}
