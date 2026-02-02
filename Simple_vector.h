#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
public:
    SimpleVector() : data(nullptr), end_(nullptr), capacity(0), size_(0) {};

    explicit SimpleVector(const size_t& size) {
        data = new T[size];
        end_ = data + size;
        capacity = size;
        size_ = size;

    };

    explicit SimpleVector(const SimpleVector<T>& other)
        : data(new T[other.capacity]),
        capacity(other.capacity) {
        size_(other.size);
        copy(other.begin(), other.end(),begin());
    }

    ~SimpleVector() {
        if (Size() != 0) {
            delete[] data;
        }
    };

    T& operator[](const size_t& index) {
        if (Size() != 0) {
            return *(data + index);
        }

    };

    T* begin() {
        return data;
    }

    T* end() {
        return end_;
    }

    const T* begin() const {
        return data;
    };

    const T* end() const {
        return end_;
    };

    size_t Size() const {
        return size_;
    };

    size_t Capacity() const {
        return capacity;
    };

    void PushBack(const T& value) {
        auto size = size_;
        if (size == capacity && size != 0) {
            T* new_data = new T[2 * capacity];
            for (auto i = 0; i < size ; i++) {
                new_data[i] = data[i];
            }
            new_data[size] = value;
            delete[] data;
            data = new_data;
            end_ = new_data + size + 1;
            capacity *=2;
        }
        else if (size != 0) {
            *(end_) = value;
            end_ ++;
        }

        else if (size == 0) {
            data = new T[1];
            end_ = data + 1;
            data[0] = value;
            capacity = 1;
        }
        size_++;
    };

    void  operator=(const SimpleVector<T>& other) {
        delete[] data;
        data = new T[other.capacity];
        end_ = data + size_;
        capacity = other.capacity;
        size_= other.size_;
       copy(other.begin(), other.end(),begin());
    };

private:
    T* data;
    T* end_;
    size_t capacity;
    size_t size_;
};

