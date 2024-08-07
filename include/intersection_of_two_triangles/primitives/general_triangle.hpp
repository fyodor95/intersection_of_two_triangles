#pragma once

#include <array>
#include <cstddef>
#include <variant>
#include <vector>

#include "intersection_of_two_triangles/primitives/point.hpp"

namespace intersection_of_two_triangles {

class Segment;
class Triangle;

struct GeneralTriangle {
    using Decomposed = std::vector<std::variant<Point, Segment, Triangle>>;
    [[nodiscard]] Decomposed as_non_degenerate() const;
    [[nodiscard]] Segment edge(size_t which) const;

    std::array<Point, 3> vertices;
};

}
