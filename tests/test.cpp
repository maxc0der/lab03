// Copyright 2020 Maxim Nasachenko <maximka5406@gmail.com>

#include <gtest/gtest.h>

#include <sharedptr.hpp>
#include <utility>
#include <type_traits>

TEST(SharedPtrTest, DefaultConstructor) {
  SharedPtr<int> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(ptr.use_count(), 0);
}

TEST(SharedPtrTest, PtrConstructor) {
  int *ptr = new int(123);
  SharedPtr<int> sharedPtr(ptr);
  ASSERT_EQ(sharedPtr.get(), ptr);
  ASSERT_EQ(sharedPtr.use_count(), 1);
}

TEST(SharedPtrTest, CopyingConstructor) {
  SharedPtr<int> ptr(new int(123));
  SharedPtr<int> ptr2(ptr);
  ASSERT_EQ(ptr.get(), ptr2.get());
  ASSERT_EQ(ptr.use_count(), ptr2.use_count());
  ASSERT_EQ(ptr.use_count(), 2);
}

TEST(SharedPtrTest, MovingConstructor) {
  SharedPtr<int> ptr(new int(123));

  auto tmp = ptr.get();
  ASSERT_TRUE(std::is_move_constructible<SharedPtr<int>>::value);
  SharedPtr<int> ptr2(std::move(ptr));

  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(ptr.use_count(), 0);
  ASSERT_EQ(ptr2.use_count(), 1);
  ASSERT_EQ(ptr2.get(), tmp);
}

TEST(SharedPtrTest, Destructor) {
  int *ptr = new int(1234);
  SharedPtr<int> sharedPtr(ptr);
  ASSERT_EQ(sharedPtr.use_count(), 1);
  {
    SharedPtr<int> sharedPtr1(sharedPtr);
    ASSERT_EQ(sharedPtr1.use_count(), 2);
    ASSERT_EQ(sharedPtr1.use_count(), sharedPtr.use_count());
  }
  ASSERT_EQ(sharedPtr.use_count(), 1);
}

TEST(SharedPtrTest, CopyingOperator) {
  int *ptr = new int(1234);
  int *ptr2 = new int(5678);

  SharedPtr<int> sharedPtr(ptr);
  SharedPtr<int> sharedPtr1(sharedPtr);

  ASSERT_EQ(sharedPtr.use_count(), sharedPtr1.use_count());
  ASSERT_EQ(sharedPtr.use_count(), 2);
  ASSERT_EQ(sharedPtr.get(), sharedPtr1.get());

  SharedPtr<int> sharedPtr2(ptr2);
  sharedPtr1 = sharedPtr2;

  ASSERT_EQ(sharedPtr.use_count(), 1);
  ASSERT_EQ(sharedPtr1.use_count(), sharedPtr2.use_count());
  ASSERT_EQ(sharedPtr1.use_count(), 2);
  ASSERT_EQ(sharedPtr1.get(), sharedPtr2.get());
  ASSERT_EQ(sharedPtr1.get(), ptr2);
}

TEST(SharedPtrTest, MovingOperator) {
  int *ptr = new int(1234);
  int *ptr2 = new int(5678);

  SharedPtr<int> sharedPtr(ptr);
  SharedPtr<int> sharedPtr1(sharedPtr);

  ASSERT_EQ(sharedPtr.use_count(), sharedPtr1.use_count());
  ASSERT_EQ(sharedPtr.use_count(), 2);
  ASSERT_EQ(sharedPtr.get(), sharedPtr1.get());

  SharedPtr<int> sharedPtr2(ptr2);
  ASSERT_TRUE(std::is_move_assignable<SharedPtr<int>>::value);
  sharedPtr1 = std::move(sharedPtr2);

  ASSERT_EQ(sharedPtr.use_count(), 1);
  ASSERT_EQ(sharedPtr1.use_count(), 1);
  ASSERT_EQ(sharedPtr1.get(), ptr2);

  ASSERT_FALSE(sharedPtr2);
  ASSERT_EQ(sharedPtr2.use_count(), 0);
}

TEST(SharedPtrTest, ResetTest1) {
  SharedPtr<int> sharedPtr(new int(1234));
  ASSERT_EQ(sharedPtr.use_count(), 1);
  ASSERT_TRUE(sharedPtr);

  sharedPtr.reset();
  ASSERT_FALSE(sharedPtr);
  ASSERT_EQ(sharedPtr.use_count(), 0);
}

TEST(SharedPtrTest, ResetTest2) {
  int *ptr = new int(123);
  int *ptr2 = new int(456);

  SharedPtr<int> sharedPtr1(ptr);
  SharedPtr<int> sharedPtr2(sharedPtr1);

  ASSERT_EQ(sharedPtr1.get(), ptr);
  ASSERT_EQ(sharedPtr2.get(), ptr);
  ASSERT_EQ(sharedPtr2.use_count(), 2);
  ASSERT_EQ(sharedPtr1.use_count(), 2);

  sharedPtr2.reset(ptr2);
  ASSERT_EQ(sharedPtr2.get(), ptr2);
  ASSERT_EQ(sharedPtr1.use_count(), 1);
  ASSERT_EQ(sharedPtr2.use_count(), 1);
}
