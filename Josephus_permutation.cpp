#include "test_runner.h"
#include "profile.h"
#include <list>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <queue>

using namespace std;
/*template <typename RandomIt>
void MakeJosephusPermutation_vector(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> pool;
    pool.reserve(last - first);
    for (RandomIt i = first; i < last; i++) {
        pool.push_back(move(*i));
    }
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = move(pool[cur_pos]);
        pool.erase(pool.begin() + cur_pos);
        if (pool.empty()) {
            break;
        }
        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

template <typename RandomIt>
void MakeJosephusPermutation_list(RandomIt first, RandomIt last, uint32_t step_size) {
    list<typename RandomIt::value_type> pool;
    for (RandomIt i = first; i < last; i++) {
        pool.push_back(move(*i));
    }
    auto pool_it = pool.begin();
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = move(*(pool_it));
        pool.erase(pool_it);
        if (pool.empty()) {
            break;
        }
        cur_pos = (cur_pos + step_size - 1) % pool.size();
        pool_it = pool.begin();
        for (int i = 0; i < cur_pos; i++) pool_it++;
    }
}*/
template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    queue<typename RandomIt::value_type> pool;
    for (RandomIt i = first; i < last; i++) {
        pool.push(move(*i));
    }
    size_t cur_pos = 0;
    while (!pool.empty()) {
        if (cur_pos % step_size == 0) {
            *(first++) = move(pool.front());
        }
        else {
            pool.push(move(pool.front()));
        }
        pool.pop();
        if (pool.empty()) {
            break;
        }
        cur_pos++;
    }
}

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    ASSERT_EQUAL(numbers, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
    /*auto size = 100000;
    vector<int> a(size);
    for (int i = 0 ; i < size; i++ ) {
        a[i] = i;
    }
    LOG_DURATION("vector") {
        MakeJosephusPermutation_vector(a.begin(), a.end(), 5);
        cout << a[size/10] << endl;
    }
    vector<int> b(size);
    for (int i = 0 ; i < size; i++ ) {
        b[i] = i;
    }
    LOG_DURATION("list") {
        MakeJosephusPermutation_list(b.begin(), b.end(), 5);
        cout << b[size/10] << endl;
    }
    vector<int> c(size);
    for (int i = 0 ; i < size; i++ ) {
        c[i] = i;
    }
    LOG_DURATION("queue") {
        MakeJosephusPermutation_queue(c.begin(), c.end(), 5);
        cout << c[size/10] << endl;
    }*/
    return 0;


}
