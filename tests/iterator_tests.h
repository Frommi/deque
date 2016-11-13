#pragma once

#include "../deque.h"

struct Complex {
    Complex() :
        r(0.0),
        i(0.0)
    {}

    Complex(double r, double i) :
        r(r),
        i(i)
    {}

    double r;
    double i;

    double sqr_abs() const {
        return r * r + i * i;
    }

    bool operator==(const Complex& other) const {
        return r == other.r && i == other.i;
    }
};


template<class Iter>
class DEQUE_ITERATOR : public testing::Test {
protected:
    const int n = 97;
    const int m = 61;

    void SetUp() {
        for (int i = -n * m; i <= n; ++i)
            d.push_back(Complex(i * 1.0, i * 1.0));

        for (int i = -n * m; i < 0; ++i)
            d.pop_front();
    }

    Deque<Complex> d;
};

typedef testing::Types<Deque<Complex>::iterator, Deque<Complex>::const_iterator> iterators;
TYPED_TEST_CASE(DEQUE_ITERATOR, iterators);


TYPED_TEST(DEQUE_ITERATOR, TRIVIAL) {
    int i = 0;
    for (TypeParam it = this->d.begin(); it != this->d.end(); ++it, ++i)
        ASSERT_DOUBLE_EQ(it->sqr_abs(), 2.0 * i * i);

    i = 0;
    for (TypeParam it = this->d.begin(); it < this->d.end(); it += 2, i += 2)
        ASSERT_DOUBLE_EQ(it->sqr_abs(), 2.0 * i * i);
}

TYPED_TEST(DEQUE_ITERATOR, REVERSE_TRIVIAL) {
    int i = this->d.size() - 1;
    for (std::reverse_iterator<TypeParam> it = this->d.rbegin(); it != this->d.rend(); ++it, --i)
        ASSERT_DOUBLE_EQ(it->sqr_abs(), 2.0 * i * i);
}

TYPED_TEST(DEQUE_ITERATOR, COMPARISONS) {
    for (TypeParam it = this->d.begin() + 1; it < this->d.end() - 1; ++it) {
        TypeParam prev = it - 1;
        TypeParam next = it + 1;

        ASSERT_NE(it, prev);
        ASSERT_NE(it, next);
        ASSERT_EQ(it, prev + 1);
        ASSERT_EQ(it, next - 1);

        ASSERT_LT(it, next);

        ASSERT_GT(it, prev);

        ASSERT_GE(it, prev);
        ASSERT_GE(it, prev + 1);

        ASSERT_LE(it, next);
        ASSERT_LE(it, next - 1);
    }
}


TYPED_TEST(DEQUE_ITERATOR, ITERATOR_TO_CONST_ITERATOR) {
    int i = 0;
    for (Deque<Complex>::iterator it = this->d.begin(); it != this->d.end(); ++it, ++i) {
        Deque<Complex>::const_iterator cit(it);

        ASSERT_EQ(*it, *cit);
        if (it + 1 != this->d.end())
            ASSERT_EQ(*(it + 1), *(cit + 1));
    }
}

TYPED_TEST(DEQUE_ITERATOR, ITERATOR_TO_REVERSE_ITERATOR) {
    int i = 0;
    for (TypeParam it = this->d.begin(); it != this->d.end(); ++it, ++i) {
        std::reverse_iterator<TypeParam> rit(it + 1);

        ASSERT_EQ(*it, *rit);
        if (it + 1 != this->d.end())
            ASSERT_EQ(*(it + 1), *(rit - 1));
    }
}

TYPED_TEST(DEQUE_ITERATOR, REVERSE_ITERATOR_TO_ITERATOR) {
    int i = 0;
    for (std::reverse_iterator<TypeParam> rit = this->d.rbegin(); rit != this->d.rend(); ++rit, ++i) {
        TypeParam it(rit + 1);

        ASSERT_EQ(*it, *rit);
        if (rit + 1 != this->d.rend())
            ASSERT_EQ(*(it - 1), *(rit + 1));
    }
}
