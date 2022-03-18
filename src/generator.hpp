#pragma once

#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#ifdef __llvm__
#include <experimental/coroutine>
namespace std {
template <class T>
using coroutine_handle = experimental::coroutine_handle<T>;
using suspend_always = experimental::suspend_always;
using suspend_never = experimental::suspend_never;
using noop_coroutine_handle = experimental::noop_coroutine_handle;
}  // namespace std
#else
#include <coroutine>
#endif

namespace Flier::Task {
struct generator {
  struct promise_type;
  using handle = std::coroutine_handle<promise_type>;
  struct promise_type {
    std::unique_ptr<generator> task_stack;
    static auto get_return_object_on_allocation_failure() {
      return generator(nullptr);
    }
    auto get_return_object() { return generator{handle::from_promise(*this)}; }
    auto initial_suspend() noexcept { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
    auto yield_value(bool value) { return std::suspend_always{}; }
    auto yield_value() { return std::suspend_always{}; }
    auto yield_value(generator&& task) {
      task_stack = std::make_unique<generator>(std::move(task));
      task_stack->next();
      return std::suspend_always{};
    }
    ~promise_type() { task_stack = nullptr; }
  };
  // 単純にコルーチンを一個進める
  bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
  // スタックされたコルーチンがあるならそれを進める
  void next() {
    if (coro.promise().task_stack && !coro.promise().task_stack->done()) {
      coro.promise().task_stack->next();
      if (coro.promise().task_stack->done()) {
        coro.resume();
      }
    } else if (coro && !coro.done()) {
      coro.resume();
    }
  }
  bool done() { return coro.done(); }
  generator(generator const&) = delete;
  generator(generator&& rhs) : coro(rhs.coro) { rhs.coro = nullptr; }
  ~generator() {
    if (coro) coro.destroy();
  }

 public:
  std::string task_name();

 private:
  generator(handle h) : coro(h) {}
  handle coro;
};
}  // namespace Flier::Task
