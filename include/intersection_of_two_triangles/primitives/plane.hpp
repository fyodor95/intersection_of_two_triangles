#pragma once

#include <array>
#include <optional>

#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

struct Line;
struct Point;

// The equation is dot_product(normal, point) + d = 0.
struct Plane {
    Plane(const Point&, const Point&, const Point&);

    [[nodiscard]] double signed_distance(const Point& to) const;

    Vector normal;
    double d;
};

[[nodiscard]] std::optional<Line> intersection(const Plane&, const Plane&);

}
