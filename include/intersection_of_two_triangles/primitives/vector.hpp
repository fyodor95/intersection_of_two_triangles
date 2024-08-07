#pragma once

#include <cstddef>

namespace intersection_of_two_triangles {

struct Point;

struct Vector {
    Vector(double x, double y, double z);

    [[nodiscard]] Point as_point() const;
    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] double coord(size_t which) const;
    [[nodiscard]] double& coord(size_t which);
    [[nodiscard]] double length() const;

    Vector& operator+=(const Vector&);
    Vector& operator-=(const Vector&);
    Vector& operator*=(double);
    Vector& operator/=(double);

    double x, y, z;
};

[[nodiscard]] Vector operator-(Vector);
[[nodiscard]] Vector operator-(Vector, Vector);
[[nodiscard]] Vector operator+(Vector, Vector);
[[nodiscard]] Vector operator*(double, Vector);
[[nodiscard]] Vector operator*(Vector, double);
[[nodiscard]] Vector operator/(Vector, double);

}
