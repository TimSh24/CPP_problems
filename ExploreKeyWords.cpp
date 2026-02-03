#include "test_runner.h"
#include "profile.h"
#include <string_view>
#include <map>
#include <string>
#include <future>
#include <vector>
#include <sstream>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
            : first(begin)
            , last(end)
            , size_(distance(first, last))
    {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

    size_t size() const {
        return size_;
    }

private:
    Iterator first, last;
    size_t size_;
};

template <typename Iterator>
class Paginator {
private:
    vector<IteratorRange<Iterator>> pages;

public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0; ) {
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

    size_t size() const {
        return pages.size();
    }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

struct Stats {
  map<string, int> word_frequences;
  void operator += (const Stats& other) {
      for (auto &i : other.word_frequences) {
          this->word_frequences[i.first]+=i.second;
      }
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats answer;
    string_view s = line;
    while (true) {
        size_t space = s.find(' ');
        if (key_words.count(string(s.substr(0, space)))) {
            answer.word_frequences[string(s.substr(0, space))] += 1;
        }
        if (space == s.npos) {
            break;
        }
        else {
            s.remove_prefix(space+1);
        }
    }
    return answer;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    Stats result;
    vector<future<Stats>> futures;
    vector<string> strings;
    vector<stringstream> streams;

    for (string line; getline (input, line); ) {
        strings.push_back(line);
    }

    for (auto i : Paginate(strings, 2000)) {
        stringstream single_stream;
        for (auto j = i.begin(); j != i.end(); j++) {
            single_stream << *j << '\n';
        }
        streams.push_back(move(single_stream));
    }

    for (auto &stream : streams) {
        futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(stream)));
    }

    for (auto &item : futures) {
        result += item.get();
    }
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
