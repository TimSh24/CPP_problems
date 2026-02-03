#include <iostream>
#include <math.h>
#include <memory>
#include <vector>
#include <iomanip>

using namespace std;

class Figure {
public:
    Figure(const string& name) : Name_(name) {}
    virtual string Name() const = 0;
    virtual float Perimeter() const = 0;
    virtual float Area() const = 0;
protected:
    const string Name_;
};

class Triangle : public Figure {
public:
    Triangle(const int& a, const int& b, const int& c) : Figure("TRIANGLE") {
        a_ = a;
        b_ = b;
        c_ = c;
    }
    string Name() const override {
        return Name_;
    }
    float Perimeter() const override {
        return a_ + b_ + c_;
    }
    float Area() const override {
        float Half_per = 0.5* Perimeter();
        return sqrt((Half_per * (Half_per - a_) * (Half_per - b_) * (Half_per - c_)));
    }
private:
    int a_;
    int b_;
    int c_;
};

class Rect : public Figure {
public:
    Rect(const int& a, const int& b) : Figure("RECT") {
        a_ = a;
        b_ = b;
    }
    string Name() const override {
        return Name_;
    }
    float Perimeter() const override {
        return 2 * (a_ + b_);
    }
    float Area() const override {
        return a_ * b_;
    }
private:
    int a_;
    int b_;
};

class Circle : public Figure {
public:
    Circle(const int& r) : Figure("CIRCLE") {
        r_ = r;
    }
    string Name() const override {
        return Name_;
    }
    float Perimeter() const override {
        return 2 * 3.14 * r_;
    }
    float Area() const override {
        return 3.14 * r_ * r_;
    }
private:
    int r_;
};

shared_ptr<Figure> CreateFigure(istream& is) {
    string figure_type;
    shared_ptr<Figure> t;
    is >> figure_type;
    if (figure_type == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        t = make_shared<Triangle>(a, b, c);
    }
    if (figure_type == "RECT") {
        int a, b;
        is >> a >> b;
        t = make_shared<Rect>(a, b);
    }
    if (figure_type == "CIRCLE") {
        int r;
        is >> r;
        t = make_shared<Circle>(r);
    }
    return t;
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);
        string command;
        is >> command;
        if (command == "ADD") {
            is >> ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}
