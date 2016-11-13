#pragma once

#include <deque>

#include "../deque.h"

TEST(DEQUE_TRIVIAL, SMALL_TEST) {
    Deque<int> d;
    ASSERT_TRUE(d.empty());

    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    d.push_front(0);

    Deque<int> d2(d);
    d2[0] = 100;

    ASSERT_FALSE(d.empty());
    ASSERT_EQ(d.size(), 4);

    for (int i = 0; i < 4; ++i)
        ASSERT_EQ(d[i], i);

    ASSERT_EQ(d.front(), 0);
    ASSERT_EQ(d.back(), 3);

    d.pop_front();
    d.pop_back();

    ASSERT_EQ(d.front(), 1);
    ASSERT_EQ(d.back(), 2);

    d.pop_back();
    d.pop_back();

    ASSERT_TRUE(d.empty());
}

TEST(DEQUE_TRIVIAL, LINEAR_PUSH_BACK) {
    const size_t n = 10000;

    Deque<int> d;
    std::deque<int> v;
    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(d.size(), v.size());
        EXPECT_EQ(d.empty(), v.empty());

        int val = rand();
        d.push_back(val);
        v.push_back(val);
    }

    for (size_t i = 0; i < n; ++i)
        EXPECT_EQ(d[i], v[i]);
}

TEST(DEQUE_TRIVIAL, RANDOM_PUSH_POP) {
    const size_t n = 10000;

    Deque<int> d;
    std::deque<int> v;
    for (size_t i = 0; i < n; ++i) {
        size_t next_move = rand() % 4;

        EXPECT_EQ(d.empty(), v.empty());
        if (v.empty()) {
            next_move %= 2;
        } else {
            EXPECT_EQ(d.size(), v.size());
            size_t index = rand() % v.size();
            EXPECT_EQ(d[index], v[index]);
        }

        int val;
        switch (next_move) {
            case 0:
                val = rand();
                v.push_back(val);
                d.push_back(val);
                break;
            case 1:
                val = rand();
                v.push_front(val);
                d.push_front(val);
                break;
            case 2:
                v.pop_back();
                d.pop_back();
                break;
            case 3:
                v.pop_front();
                d.pop_front();
                break;
        }
    }
}

TEST(DEQUE_TRIVIAL, CONST_DEQUE) {
    const int n = 239;
    const int m = 17;

    Deque<int> d;
    for (int i = -(n * m); i < n; ++i)
        d.push_back(rand());

    for (int i = -(n * m); i < 0; ++i)
        d.pop_front();

    const Deque<int> cd = d;

    ASSERT_EQ(d.size(), cd.size());
    ASSERT_EQ(d.empty(), cd.empty());
    ASSERT_EQ(d.back(), cd.back());
    ASSERT_EQ(d.front(), cd.front());
    for (int i = 0; i < n; ++i)
        ASSERT_EQ(d[i], cd[i]);

    Deque<int>::const_iterator cit = cd.cbegin();
    for (Deque<int>::iterator it = d.begin(); it != d.end(); ++it, ++cit)
        ASSERT_EQ(*it, *cit);
}
