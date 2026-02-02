#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <set>

using namespace std;

struct Date {
    int year;
    int month;
    int day;
};

void dateParser (const string& date_str, Date& date) {
    stringstream stream(date_str);
        if (!(stream >> date.year) or stream.peek() != '-') {
            stringstream err;
            err << "Wrong date format: " << date_str << endl;
            throw runtime_error(err.str());
        }
        stream.ignore(1);
        if (!(stream >> date.month) or stream.peek() != '-') {
            stringstream err;
            err << "Wrong date format: " << date_str << endl;
            throw runtime_error(err.str());
        }
        stream.ignore(1);
        if (!(stream >> date.day) or !stream.eof()) {
            stringstream err;
            err << "Wrong date format: " << date_str << endl;
            throw runtime_error(err.str());
    }

        if (date.month < 1 or date.month > 12) {
            stringstream err;
            err << "Month value is invalid: " << date.month << endl;
            throw runtime_error(err.str());
        }

        if (date.day < 1 or date.day > 31) {
            stringstream err;
            err << "Day value is invalid: " << date.day << endl;
            throw runtime_error(err.str());
        }
    }

    ostream& operator<<(ostream& stream, const Date& parsed_date) {
        stream << setfill('0');
        stream << setw(4) << parsed_date.year << '-' << setw(2) << parsed_date.month <<
            '-' << setw(2) << parsed_date.day;
        return stream;
    }

    bool operator<(const Date& lhs, const Date& rhs) {
        return tie(lhs.year, lhs.month, lhs.day) <
               tie(rhs.year, rhs.month, rhs.day);
    }

class Database {
public:
    void add (const Date& key, const string& event) {
            db[key].insert(event);
    };

    void del_single(const Date& key, const string& event){
        try
        {
            if (count(db.at(key).begin(), db.at(key).end(), event) == 0){
                cout << "Event not found" << endl;
            }
            else {
                db[key].erase(event);
                cout << "Deleted successfully" << endl;
                if (db[key].empty()) {
                    db.erase(key);
                }
            }
        }
        catch (exception& ex) {
            cout << "Event not found" << endl;}
    };

    void del_multiple(const Date& key){
        try {
        size_t set_size;
        set_size = db.at(key).size();
        db.erase(key);
        cout << "Deleted " << set_size << " events" << endl;}
        catch (exception &ex){
            cout << "Deleted 0 events" << endl;
        }
    };

    void print() const {
        for (const auto& item : db) {
            for (auto& j : item.second) {
                cout << item.first << " " << j << endl;
            }
        }
    };

    void find (const Date& key) const {
        try{
            for (auto i : db.at(key)){
                cout << i << endl;
            }
        }
        catch (exception &ex) {}
    };

private:
    map <Date, set<string>> db;
};

int main(){
    string date_str;
    Date parsed_date{};
    Database base;
    string command;
    string event;
    string request;
    try {
        while (getline(cin, request)) {
            stringstream line_stream(request);
            if (!(line_stream >> command)) {
                continue;
            }
            if (command != "Print" and command != "Add" and command != "Del" and command != "Find") {
                stringstream err;
                err << "Unknown command: " << command << endl;
                throw runtime_error(err.str());
            } else if (command == "Print") {
                base.print();
            } else {
                line_stream >> date_str;
                dateParser(date_str, parsed_date);
                if (command == "Add" and (line_stream >> event)) {
                    base.add(parsed_date, event);
                }
                if (command == "Del") {
                    if (line_stream >> event) {
                        base.del_single(parsed_date, event);
                    } else {
                        base.del_multiple(parsed_date);
                    }
                }
                if (command == "Find") {
                    base.find(parsed_date);
                }
            }
        }
    }
    catch (exception & ex){
        cout << ex.what();
    }
    return 0;
}
