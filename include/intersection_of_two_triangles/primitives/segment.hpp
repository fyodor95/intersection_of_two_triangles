#pragma once

#include <array>

#include "intersection_of_two_triangles/primitives/point.hpp"

namespace intersection_of_two_triangles {

struct Vector;

class Segment {
public:
    Segment(const Point&, const Point&);
    [[nodiscard]] const Point& endpoint(bool which) const;
    [[nodiscard]] Vector as_vector() const;

private:
    std::array<Point, 2> endpoints;
};

}
