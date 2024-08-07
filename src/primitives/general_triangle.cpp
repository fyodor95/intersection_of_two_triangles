#include <algorithm>
#include <cassert>
#include <cstddef>

#include "intersection_of_two_triangles/exception.hpp"
#include "intersection_of_two_triangles/primitives/general_triangle.hpp"
#include "intersection_of_two_triangles/primitives/plane.hpp"
#include "intersection_of_two_triangles/primitives/segment.hpp"
#include "intersection_of_two_triangles/primitives/triangle.hpp"

namespace intersection_of_two_triangles {

namespace {

enum class Equal {
    kNo,
    kAlmost,
    kExactly,
};

Equal compare(const Point& a, const Point& b) {
    if (are_exactly_equal(a, b)) {
        return Equal::kExactly;
    }

    if (are_nearly_equal(a, b)) {
        return Equal::kAlmost;
    }

    return Equal::kNo;
}

}

GeneralTriangle::Decomposed GeneralTriangle::as_non_degenerate() const {
    std::array<Equal, 3> equal{};
    size_t equalities_number = 0;
    std::vector<Point> unique_vertices;

    for (size_t i1 = 2, i2 = 0; i2 < 3; i1 = i2++) {
        equal[i1] = compare(vertices[i1], vertices[i2]);
        equalities_number += equal[i1] != Equal::kNo;
        if (equal[i1] != Equal::kExactly) {
            unique_vertices.push_back(vertices[i2]);
        }
    }

    if (equalities_number >= 2) {
        if (unique_vertices.empty()) {
            return {vertices[0]};
        }
        return {unique_vertices.begin(), unique_vertices.end()};
    }

    if (equalities_number == 0) {
        try {
            Plane(vertices[0], vertices[1], vertices[2]);
        } catch (const Exception&) {
            std::array<double, 3> side_lengths;
            for (size_t i = 0; i < 3; ++i) {
                side_lengths[i] = distance(vertices[(i + 1) % 3], vertices[(i + 2) % 3]);
            }
            const double* const max = std::max_element(side_lengths.begin(), side_lengths.end());
            const size_t middle_index = max - side_lengths.begin();
            assert(0 <= middle_index && middle_index <= 2);
            return {edge(middle_index), vertices[middle_index]};
        }
        return {Triangle(vertices[0], vertices[1], vertices[2])};
    }

    for (size_t i = 0; i < 3; ++i) {
        if (equal[i] == Equal::kExactly) {
            return {edge(i)};
        }
        if (equal[i] == Equal::kAlmost) {
            return {edge(i), edge((i + 1) % 3)};
        }
    }

    assert(false);
    return {};
}

Segment GeneralTriangle::edge(const size_t which) const {
    return {vertices[(which + 1) % 3], vertices[(which + 2) % 3]};
}

}
