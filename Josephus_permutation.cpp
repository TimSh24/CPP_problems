/*
Дан диапазон объектов некоторого типа. Напишите функцию, переставляющую его элементы в соответствии с порядком, 
определённым так называемой считалкой Иосифа с заданным размером шага:

template <typename RandomIt>
void MakeJosephusPermutation(
    RandomIt range_begin, RandomIt range_end,
    uint32_t step_size
);

Гарантируется, что итераторы range_begin и range_end являются итераторами произвольного доступа, 
то есть допускают вычитание одного из другого и сложение с числом. 
Кроме того, вы можете полагаться на то, что step_size > 0.
Тип переупорядочиваемых объектов можно получить с помощью выражения typename RandomIt::value_type.
Объекты этого типа запрещено копировать.
*/
template <typename RandomIt>
void MakeJosephusPermutation(
    RandomIt range_begin, RandomIt range_end,
    uint32_t step_size
);

#include "test_runner.h"
#include "profile.h"
#include <list>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <queue>

using namespace std;

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
    return 0;


}
