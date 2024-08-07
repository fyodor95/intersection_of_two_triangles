#pragma once

#include <array>
#include <cstddef>

#include "intersection_of_two_triangles/primitives/point.hpp"

namespace intersection_of_two_triangles {

class Segment;

class Triangle {
public:
    Triangle(const Point&, const Point&, const Point&);
    [[nodiscard]] const Point& vertex(size_t which) const;

    // Returns the triangle side opposite to `vertex(which)`.
    [[nodiscard]] Segment edge(size_t which) const;

private:
    std::array<Point, 3> vertices;
};

}
