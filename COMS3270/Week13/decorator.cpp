#include <iostream>

using namespace std;

class shape {
    public:
    virtual void draw() = 0;
};

class circle : public shape {
public:
    virtual void draw() {
        cout << "0" << endl;
    }
};

class square : public shape {
public:
    virtual void draw() {
        cout << "[]" << endl;
    }
};

/*Assume we do not have access to the sources of the above, we on,y the interface and a binary library. We would still like to extend them.*/

class colored_circle : public circle {
public:
    // we'd want a constructor that sets the color then:
    virtual void draw() {
        cout << "the_color";
        circle::draw();
    }
};

class resized_circle : public circle {
public:
    //we'd want a constructor that sets the size (maybe with a matrix), then:
    virtual void draw() {
        cout << "the_size";
        circle::draw();
    }
};

// the above functions get very ugly because every new functionality we want to add requires a new class. ALl of these problems go away if we have
// access for the source code for the base class(es), since that would allow us to add our own data in the location where it belongs.

// this is where the decorator pattern comes in handy.
// rather than extend a concrete class (circle or square), we extend the base class.
// the key insight that makes decorators work is that we'll use a has-a, rather than an is-a relationship.

class shape_decorator : public shape {
public:
    shape_decorator(shape *s) : s(s) {}

    // the destructor (not made) needs to free s.
protected:
    // points to a concrete shape like square or circle, and s->draw() calls that concrete shapes' draw method.
    shape *s;
    virtual void draw() {
        s->draw();
    }
};

class resized_shape : public shape_decorator {
public:
    resized_shape(shape *s) : shape_decorator(s) {}
    //write an appropriate constructor
    virtual void draw() {
        cout << "resized-";
        s->draw();
    }
};

class colored_shape : public shape_decorator {
public:
    colored_shape(shape *s) : shape_decorator(s) {}
    //write an appropriate constructor
    virtual void draw() {
        cout << "colored-";
        s->draw();
    }
};

int main(int argc, char*argv[]) {

    circle c;
    
    c.draw();

    square s;

    s.draw();

    resized_circle rc;

    rc.draw();

    // if we want a circle that is both resized and colored, we need to write a class for that. We never even wrote any code to modify squares...


    // using the decorator pattern:

    shape *p = new resized_shape(new colored_shape(new circle));

    p->draw();

    shape *d = new resized_shape(new colored_shape(new square));

    d->draw();

    // you can even add mutiple to the mix!

    shape *q = new resized_shape(new colored_shape(new resized_shape(new colored_shape(new square))));

    q->draw();

    return 0;

}