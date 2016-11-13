#pragma once

#include "../deque.h"

const int COMPLEXITY_CONSTANT = 2;

class CountingDummy {
public:
    CountingDummy() :
        dummy_data_(0)
    {}

    CountingDummy(const CountingDummy& other) {
        ++couter_;
    }

    CountingDummy& operator=(const CountingDummy& other) {
        ++couter_;
        return *this;
    }

    static void restart() {
        couter_ = 0;
    }

    static size_t get_counter() {
        return couter_;
    }

private:
    static size_t couter_; 
    char dummy_data_;
};

size_t CountingDummy::couter_ = 0;

TEST(DEQUE_COMPLEXITY, LINEAR_TEST) {
    const int n = 10000;
    Deque<CountingDummy> d;
    for (size_t i = 0; i < n; ++i)
        d.push_back(CountingDummy());
    for (size_t i = 0; i < n; ++i)
        d.pop_back();

    ASSERT_LE(CountingDummy::get_counter(), COMPLEXITY_CONSTANT * 2 * n);
    CountingDummy::restart();
}

TEST(DEQUE_COMPLEXITY, RANDOM_TEST) {
    const int n = 10000;
    Deque<CountingDummy> d;

    for (size_t i = 0; i < n; ++i) {
        bool to_push = rand() & d.empty();
        if (to_push) {
            if (rand() % 2)
                d.push_back(CountingDummy());
            else
                d.push_front(CountingDummy());
        } else {
            if (rand() % 2)
                d.pop_back();
            else
                d.pop_front();
        }

        ASSERT_LE(CountingDummy::get_counter(), COMPLEXITY_CONSTANT * (i + 1));
    }

    CountingDummy::restart();
}
