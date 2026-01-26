#ifndef TRI_H
#define TRI_H

#include <cmath>
#include "vec3.h"

class tri {
    public:
    vec3 a[3];

    tri() : a{vec3(), vec3(), vec3()} {}
    //tri(vec3 v0(double a0, double a1, double a2), vec3 v1(double a0, double a1, double a2), vec3 v2(double a0, double a1, double a2)) : a{v0,v1,v2} {}

    vec3 vec1() const {return a[0];}
    vec3 vec2() const {return a[1];}
    vec3 vec3() const {return a[2];}


};



#endif