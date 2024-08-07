#include <cassert>

#include "intersection_of_two_triangles/primitives/segment.hpp"
#include "intersection_of_two_triangles/primitives/triangle.hpp"

namespace intersection_of_two_triangles {

Triangle::Triangle(const Point& a, const Point& b, const Point& c) : vertices{a, b, c} {
    assert(!are_nearly_equal(a, b));
    assert(!are_nearly_equal(b, c));
    assert(!are_nearly_equal(c, a));
}

const Point& Triangle::vertex(const size_t which) const {
    assert(which < 3);
    return vertices[which];
}

Segment Triangle::edge(const size_t which) const {
    return {vertices[(which + 1) % 3], vertices[(which + 2) % 3]};
}

}
