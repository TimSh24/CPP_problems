/*
Часто от хранилища данных требуется быстрый поиск по любому полю. Для этого конструируются вспомогательные структуры данных, 
ускоряющие поиск, они называются вторичными индексами (secondary index). В рамках задачи реализовано такое хранилище для данных типа Record ниже:
struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

class Database {
public:
  bool Put(const Record& record);
  const Record* GetById(const string& id) const;
  bool Erase(const string& id);

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const;

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const;

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const;
};
Операция Put возвращает true, если вставка удалась, и false, если вставка не удалась, потому что в базе данных уже есть запись с таким же id. 
В последнем случае состояние базы данных не должно меняться.
Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id.
Операция Erase должна возвращать true, если удалось удалить элемент с заданным id, и false, если элемент с заданным id не был найден. 
В последнем случае состояние базы данных не должно меняться.
Подразумевается, что callback должен возвращать true, если требуется продолжить обход найденных записей, и false в противном случае. 
Например, это позволит вывести первые 10 записей или найти первую запись, удовлетворяющую дополнительному критерию.
Время выполнения всех операций должно иметь в среднем сублинейную (например, константную или логарифмическую) зависимость от общего числа записей в базе данных.
В задаче предполагается, что bad_alloc кидаться не будет
*/

#include "test_runner.h"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;

    bool operator==(const Record& other) const {
        return tie(id,title,user,timestamp, karma) ==
               tie(other.id, other.title,other.user, other.timestamp, other.karma);
    }
};

struct Iterators {
    multimap<string, const Record*>::iterator user_it;
    multimap<int, const Record*>::iterator time_it;
    multimap<int, const Record*>::iterator karma_it;
};

class Database {
public:
    bool Put(const Record &record) {
        if (id_map.count(record.id)) return false;
        id_map[record.id].first = record;
        auto i = &id_map[record.id].first;
        id_map[record.id].second.user_it = user_map.insert({record.user, i});
        id_map[record.id].second.time_it = time_map.insert({record.timestamp, i});
        id_map[record.id].second.karma_it = karma_map.insert({record.karma, i});
        return true;
    }

    const Record *GetById(const string &id) const {
        if (!id_map.count(id)) return nullptr;
        return &id_map.at(id).first;
    }

    bool Erase(const string &id) {
        if (!id_map.count(id)) return false;
        auto a = id_map[id].second;
        id_map.erase(id);
        user_map.erase(a.user_it);
        time_map.erase(a.time_it);
        karma_map.erase(a.karma_it);
        return true;
    }

    template<typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto it = time_map.lower_bound(low);
        auto it_last = time_map.upper_bound(high);
        while (it != it_last && callback(*(it->second))) it++;
    }

    template<typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto it = karma_map.lower_bound(low);
        auto it_last = karma_map.upper_bound(high);
        while (it != it_last && callback(*(it->second))) it++;
    }

    template<typename Callback>
    void AllByUser(const string &user, Callback callback) const {
        auto range = user_map.equal_range(user);
        auto it = range.first;
        while (it != range.second && callback(*(it->second))) it++;
    }
private:
    unordered_map<string, pair<Record, Iterators>> id_map;
    multimap<string, const Record*> user_map;
    multimap<int, const Record*> time_map;
    multimap<int, const Record*> karma_map;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    Database a;
    a.Put({"id1", "aaaa", "u1", 1, 1000});
    a.Put({"id2", "bbbb", "u2", 2, 500});
    a.Put({"id3", "cccc", "u3", 5, 1200});
    a.Put({"id4", "dddd", "u2", 0, 100});
    a.Erase("id4");
    a.Erase ("id1");
    a.Put({"id5", "eeee", "u1", 10, 2000});
    a.Put({"id6", "ffff", "u3", 25, 0});
    cout << a.Put({"id7", "ffff", "u3", 25, 0}) << endl;
    auto b = a.GetById("id2");
    auto c = a.GetById("id4");
    const int good_karma = 400;
    const int bad_karma = 0;
    int count = 0;
    a.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });
    cout << "count=" << count << endl;
    cout << "b=" << b->id << endl;
    cout << "c=" << c->id << endl;
    return 0;
}
