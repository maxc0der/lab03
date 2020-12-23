// Copyright 2020 Maxim Nasachenko <maximka5406@gmail.com>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <utility>

template <class T>
class SharedPtr {
 public:
  SharedPtr() : object(nullptr), counter(nullptr) {}

  explicit SharedPtr(T *ptr) : object(ptr), counter(new std::atomic_uint(1)) {}

  SharedPtr(const SharedPtr &r) : object(r.object), counter(r.counter) {
    ++(*counter);
  }

  SharedPtr(SharedPtr &&rr) noexcept {
    object = rr.object;
    counter = rr.counter;

    rr.object = nullptr;
    rr.counter = nullptr;
  }

  ~SharedPtr() {
    if (!counter) return;
    if (--(*counter) == 0) {
      delete object;
      delete counter;
    }
  }

  SharedPtr &operator=(const SharedPtr &r) {
    //    if (r == *this) return *this;
    reset();
    object = r.object;
    counter = r.counter;
    ++(*counter);
    return *this;
  }

  SharedPtr &operator=(SharedPtr &&rr) noexcept {
    //    if (rr == *this) return *this;
    reset();
    object = rr.object;
    counter = rr.counter;
    rr.counter = nullptr;
    rr.object = nullptr;

    return *this;
  }

  explicit operator bool() const { return object != nullptr; }

  T &operator*() const { return *object; }

  T *operator->() const { return object; }

  T *get() { return object; }

  void reset() {
    if (!counter) return;
    --(*counter);
    if (*counter == 0) {
      delete object;
      delete counter;
    }
    object = nullptr;
    counter = nullptr;
  }

  void reset(T *ptr) {
    reset();
    object = ptr;
    counter = new std::atomic_uint(1);
  }

  void swap(SharedPtr &r) {
    std::swap(object, r.object);
    std::swap(counter, r.counter);
  }

  std::size_t use_count() const {
    if (!counter) return 0;
    return *counter;
  }

 private:
  T *object;
  std::atomic_uint *counter;
};

#endif  // INCLUDE_SHAREDPTR_HPP_
