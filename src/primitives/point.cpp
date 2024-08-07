#include <cassert>

#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/exception.hpp"
#include "intersection_of_two_triangles/primitives/point.hpp"
#include "intersection_of_two_triangles/primitives/segment.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

Point::Point(const double x, const double y, const double z) : x(x), y(y), z(z) {}

Vector Point::radius_vector() const {
    return {x, y, z};
}

double Point::coord(const size_t which) const {
    return const_cast<Point&>(*this).coord(which);
}

double& Point::coord(const size_t which) {
    assert(which <= 3);
    return (which == 0 ? x : (which == 1 ? y : z));
}

Vector operator-(const Point& a, const Point& b) {
    return a.radius_vector() - b.radius_vector();
}

Point operator+(const Point& p, const Vector& v) {
    return (p.radius_vector() + v).as_point();
}

Point operator-(const Point& p, const Vector& v) {
    return p + -v;
}

bool are_nearly_equal(const Point& p1, const Point& p2) {
    try {
        Segment(p1, p2);
    } catch (const Exception&) {
        return true;
    }

    for (size_t i = 0; i < 3; ++i) {
        if (!are_nearly_equal(p1.coord(i), p2.coord(i))) {
            return false;
        }
    }

    return true;
}

bool are_exactly_equal(const Point& p1, const Point& p2) {
    for (size_t i = 0; i < 3; ++i) {
        if (p1.coord(i) != p2.coord(i)) {
            return false;
        }
    }

    return true;
}

double distance(const Point& p1, const Point& p2) {
    return (p2 - p1).length();
}

}
