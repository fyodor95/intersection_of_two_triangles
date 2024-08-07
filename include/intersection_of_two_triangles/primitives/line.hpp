#pragma once

#include "intersection_of_two_triangles/primitives/point.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

// The equation is t * direction + o = 0
struct Line {
    Line(const Vector& direction, const Point& o);

    Vector direction;
    Point o;
};

}
