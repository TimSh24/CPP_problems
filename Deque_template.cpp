#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

template<typename T>
class Deque {
private:
    vector<T> straight_vector;
    vector<T> backward_vector;
public:
    Deque() {
        straight_vector = {};
        backward_vector = {};
    }

    bool Empty() const {
        if(straight_vector.empty() && backward_vector.empty()) {
            return true;
        }
        else {
            return false;
        }
    }

    size_t Size() const {
        return backward_vector.size() + straight_vector.size();
    }

    const T& operator [] (size_t idx) const {
        int back_vec_size = backward_vector.size();
        int straight_vec_size = straight_vector.size();
        if (idx >= back_vec_size) {
            return straight_vector[idx - back_vec_size];
        }
        else {
            return backward_vector[back_vec_size - 1 - idx];
        }
    }

    T& operator [] (size_t idx) {
        int back_vec_size = backward_vector.size();
        int straight_vec_size = straight_vector.size();
        if (idx >= back_vec_size) {
            return straight_vector[idx - back_vec_size];
        }
        else {
            return backward_vector[back_vec_size - 1 - idx];
        }
    }

    T& At(size_t idx) {
        int back_vec_size = backward_vector.size();
        int straight_vec_size = straight_vector.size();
        if (idx + 1 <= back_vec_size + straight_vec_size) {
            if (idx >= back_vec_size) {
                return straight_vector[idx - back_vec_size];
            }
            else {
                return backward_vector[back_vec_size - 1 - idx];
            }
        }
        else {
            throw out_of_range("");
        }
    }

    const T& At(size_t idx) const {
        int back_vec_size = backward_vector.size();
        int straight_vec_size = straight_vector.size();
        if (idx + 1 <= back_vec_size + straight_vec_size) {
            if (idx >= back_vec_size) {
                return straight_vector[idx - back_vec_size];
            }
            else {
                return backward_vector[back_vec_size - 1 - idx];
            }
        }
        else {
            throw out_of_range("Index is out of range");
        }
    }

    T& Front() {
        if (backward_vector.size() != 0) {
            return backward_vector.back();
        }
        else {
            return straight_vector.front();
        }
    }

    const T& Front() const {
        if (backward_vector.size() != 0) {
            return backward_vector.back();
        }
        else {
            return straight_vector.front();
        }
    }

    T& Back() {
        if (straight_vector.size() != 0) {
            return straight_vector.back();
        }
        else {
            return backward_vector.front();
        }
    }

    const T& Back() const {
        if (straight_vector.size() != 0) {
            return straight_vector.back();
        }
        else {
            return backward_vector.front();
        }
    }

void PushFront(const T& item) {
        backward_vector.push_back(item);
    }
    void PushBack(const T& item) {
        straight_vector.push_back(item);
    }
};

int main() {
    Deque<int> a;
    a.PushBack(2);
    a.PushBack(3);
    a.PushBack(4);
    a.PushBack(3);
    cout << a.Front() << endl << a.Back() << endl;
    cout << a.Size() << endl;
    cout << a.At(0) << endl;
    cout << a.At(3) << endl;
    cout << a[3] << endl;
    cout << !a.Empty() << endl;
    return 0;
}
