#include <iostream>

class shape {
//classes are private by default unless specified.
public:
    // virtual: function/method may be overwritten by another derived class, check the hierarchy to call the appropriately defined method (basically it looks for the actual implemented method)
    // = 0 means that it is not going to be implemented here (pure virtual), so it must be implemented later (cannot be instantiated as it is same as abstract.).
    //these are basically the equivalent of making an abstract in java.
    //deconstructers must also be virtual
    virtual ~shape() {};

    virtual double perimeter() = 0;

    virtual double area()= 0;

    virtual std::ostream &print(std::ostream &) const = 0;

};


std::ostream &operator<<(std::ostream &o, const shape &s) {
    return s.print(o);
}


// this is basically extends/implements in java
//public, private, or protected
// we are extending class rectangle from the abstract shape.
class rectangle : public shape {
protected:
    double side1, side2;
public:
    // could also do : side1(s1), side2(s2) to initialize values.
    rectangle(double side1, double side2) {
        this->side1 = side1;
        this->side2 = side2;
    }
    virtual ~rectangle() {};

    virtual double perimeter() {
       return 2 * (side1 + side2);
    }

    virtual double area() {
        return side1 * side2;
    }

    virtual std::ostream &print(std::ostream &o) const {
        return o << "rectangle[" << side1 << "]" << "[" << side2 << "]";
    }


};

class square : public rectangle {
public:
    square(double s) : rectangle(s,s) {};

    virtual ~square() {};

    virtual std::ostream &print(std::ostream &o) const {
     return o << "square[" << side1 << "]";
    }
};

int main() {
    rectangle r(5.0, 10.0);
    square s(4.0);

    std::cout << "Testing Rectangle:\n";
    std::cout << r << "\n";
    std::cout << "Perimeter: " << r.perimeter() << "\n";
    std::cout << "Area: " << r.area() << "\n\n";

    std::cout << "Testing Square:\n";
    std::cout << s << "\n";
    std::cout << "Perimeter: " << s.perimeter() << "\n";
    std::cout << "Area: " << s.area() << "\n\n";

    shape* shape_ptr = &s;
    std::cout << "Testing Polymorphism via shape pointer:\n";
    std::cout << *shape_ptr << " Area: " << shape_ptr->area() << "\n";

    return 0;
}