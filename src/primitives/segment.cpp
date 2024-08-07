#include "intersection_of_two_triangles/algorithms/dot_product.hpp"
#include "intersection_of_two_triangles/exception.hpp"
#include "intersection_of_two_triangles/primitives/segment.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

Segment::Segment(const Point& a, const Point& b) : endpoints{a, b} {
    const Vector ab = b.radius_vector() - a.radius_vector();

    if (dot_product(ab, ab) == 0) {
        throw Exception("Segment::Segment: the given points are too close");
    }
}

const Point& Segment::endpoint(const bool which) const {
    return endpoints[which];
}

Vector Segment::as_vector() const {
    return endpoints[1] - endpoints[0];
}

}
