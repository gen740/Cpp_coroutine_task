
#include <deque>
#include <iostream>
#include <list>
#include <utility>

#include "generator.hpp"

//  generator を格納してそれを呼ぶ
namespace Flier::TaskQue {
class taskQue {
 public:
  taskQue() {}
  ~taskQue() {}
  void push_front(Flier::Task::generator &&task, std::string task_name) {
    que.push_front({std::move(task), task_name});
  }
  void push_back(Flier::Task::generator &&task, std::string task_name) {
    que.push_back({std::move(task), task_name});
  }
  void clear() { que.clear(); }
  void operator()() {
    for (auto it = que.begin(); !que.empty() && it != que.end();) {
      if (it->first.done()) {
        it = que.erase(it);
      } else {
        it->first.next();
        ++it;
      }
    }
  }
  std::list<std::string> get_task_list() {
    std::list<std::string> task_list;
    for (auto &&i : que) {
      task_list.push_back(i.second);
    }
    return task_list;
  }

 private:
  std::list<std::pair<Flier::Task::generator, std::string>> que;
};
}  // namespace Flier::TaskQue
