#pragma once

#include <cstddef>

namespace intersection_of_two_triangles {

struct Vector;

struct Point {
    Point() = default;
    Point(double x, double y, double z);

    [[nodiscard]] Vector radius_vector() const;
    [[nodiscard]] double coord(size_t which) const;
    [[nodiscard]] double& coord(size_t which);

    double x{}, y{}, z{};
};

[[nodiscard]] Vector operator-(const Point&, const Point&);

[[nodiscard]] Point operator+(const Point&, const Vector&);
[[nodiscard]] Point operator-(const Point&, const Vector&);

[[nodiscard]] bool are_nearly_equal(const Point&, const Point&);
[[nodiscard]] bool are_exactly_equal(const Point&, const Point&);

[[nodiscard]] double distance(const Point&, const Point&);

}
