#pragma once

#include <cstring>
#include <cstdlib>
#include <iterator>

template<class T>
class Deque {
private:
    template<class IT, class Container>
    class deque_iterator : public std::iterator<std::random_access_iterator_tag, IT> {
    public:
        deque_iterator() : 
            container_(nullptr),
            index_(0)
        {}

        template<class IT2, class Container2>
        deque_iterator(const deque_iterator<IT2, Container2>& other) : 
            container_(other.container_),
            index_(other.index_)
        {}

        template<class IT2, class Container2>
        deque_iterator(const std::reverse_iterator< deque_iterator<IT2, Container2> >& other) : 
            container_(other.base().container_),
            index_(other.base().index_)
        {}

        template<class IT2, class Container2>
        deque_iterator& operator=(const deque_iterator<IT2, Container2>& other) {
            this->container_ = other.container_;
            this->index_ = other.index_;
            return *this;
        }

        template<class IT2, class Container2>
        deque_iterator& operator=(const std::reverse_iterator< deque_iterator<IT2, Container2> >& other) {
            this->container_ = other.container_;
            this->index_ = other.index_;
            return *this;
        }

        deque_iterator& operator+=(typename deque_iterator::difference_type diff) {
            index_ += diff;
            return *this;
        }

        deque_iterator& operator-=(typename deque_iterator::difference_type diff) {
            return *this += -diff;
        }

        deque_iterator& operator++() {
            return *this += 1;
        }

        deque_iterator operator++(int) {
            deque_iterator tmp(*this);
            *this += 1;
            return tmp;
        }

        deque_iterator& operator--() {
            return *this += -1;
        }

        deque_iterator operator--(int) {
            deque_iterator tmp(*this);
            *this -= 1;
            return tmp;
        }

        deque_iterator operator+(typename deque_iterator::difference_type diff) const {
            return deque_iterator(*this) += diff;
        }

        deque_iterator operator-(typename deque_iterator::difference_type diff) const {
            return deque_iterator(*this) -= diff;
        }

        typename deque_iterator::difference_type operator-(const deque_iterator& other) const {
            return this->index_ - other.index_;
        }

        template<class IT2, class Container2>
        bool operator==(const deque_iterator<IT2, Container2>& other) const {
            return index_ == other.index_;
        }

        template<class IT2, class Container2>
        bool operator!=(const deque_iterator<IT2, Container2>& other) const {
            return index_ != other.index_;
        }

        template<class IT2, class Container2>
        bool operator<(const deque_iterator<IT2, Container2>& other) const {
            return *this - other < 0;
        }

        template<class IT2, class Container2>
        bool operator<=(const deque_iterator<IT2, Container2>& other) const {
            return *this - other <= 0;
        }

        template<class IT2, class Container2>
        bool operator>(const deque_iterator<IT2, Container2>& other) const {
            return *this - other > 0;
        }

        template<class IT2, class Container2>
        bool operator>=(const deque_iterator<IT2, Container2>& other) const {
            return *this - other >= 0;
        }

        typename deque_iterator::reference operator*() const {
            return (*container_)[index_];
        }

        typename deque_iterator::pointer operator->() const {
            return &(*container_)[index_];
        }

        typename deque_iterator::reference operator[](typename deque_iterator::difference_type diff) const {
            return *(*this + diff);
        }
    private:
        friend Container;

        Container* container_;
        int index_;
    };

public:
    typedef deque_iterator<T, Deque> iterator;
    typedef deque_iterator<const T, const Deque> const_iterator;

    typedef std::reverse_iterator< iterator > reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

    Deque() :
        left_(0),
        right_(0),
        capacity_(1),
        data_(new T[capacity_])
    {}

    Deque(const Deque& other) : 
        left_(other.left_),
        right_(other.right_),
        capacity_(other.capacity_),
        data_(new T[capacity_])
    {
        memcpy(data_, other.data_, capacity_ * sizeof(T));
    }

    ~Deque() {
        delete[] data_;
    }

    size_t size() const {
        return right_ >= left_ ? right_ - left_ : capacity_ - left_ + right_;
    }

    bool empty() const {
        return size() == 0;
    }

    void push_back(const T& value) {
        if (size() + 1 >= capacity_)
            grow();

        data_[right_++] = value;
        right_ %= capacity_;
    }
    
    void pop_back() {
        if (size() < capacity_ / 4)
            shrink();

        right_ = (right_ + capacity_ - 1) % capacity_;
    }
    
    void push_front(const T& value) {
        if (size() + 1 >= capacity_)
            grow();

        left_ = (left_ + capacity_ - 1) % capacity_;
        data_[left_] = value;
    }

    
    void pop_front() {
        if (size() < capacity_ / 4)
            shrink();

        left_ = (left_ + 1) % capacity_;
    }
    
    T& operator[](size_t index) {
        return data_[(left_ + index) % capacity_];
    }
    
    const T& operator[](size_t index) const {
        return data_[(left_ + index) % capacity_];
    }

    T& front() {
        return (*this)[0];
    }

    const T& front() const {
        return (*this)[0];
    }

    T& back() {
        return (*this)[size() - 1];
    }

    const T& back() const {
        return (*this)[size() - 1];
    }


    iterator begin() {
        return get_iterator(0);
    }

    iterator end() {
        return get_iterator(size());
    }

    const_iterator cbegin() const {
        return get_citerator(0);
    }

    const_iterator cend() const {
        return get_citerator(size());
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator rbegin() const {
        return crbegin();
    }

    const_reverse_iterator rend() const {
        return crend();
    }

private:
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = left_, j = 0; i != right_; i = (i + 1) % capacity_, ++j)
            new_data[j] = data_[i];

        right_ = size();
        left_ = 0;

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void grow() {
        resize(capacity_ * 2);
    }

    void shrink() {
        resize(capacity_ / 2);
    }

    iterator get_iterator(size_t index) {
        iterator it;
        it.container_ = this;
        it.index_ = index;
        return it; 
    }

    const_iterator get_citerator(size_t index) const {
        const_iterator it;
        it.container_ = this;
        it.index_ = index;
        return it; 
    }

    size_t left_;
    size_t right_;
    size_t capacity_;
    T* data_;
};
