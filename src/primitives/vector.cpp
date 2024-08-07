#include <cassert>
#include <cmath>

#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/primitives/point.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

Vector::Vector(const double x, const double y, const double z) : x(x), y(y), z(z) {}

Point Vector::as_point() const {
    return {x, y, z};
}

bool Vector::is_zero() const {
    return (are_nearly_equal(x, 0) &&
            are_nearly_equal(y, 0) &&
            are_nearly_equal(z, 0));
}

double Vector::coord(const size_t which) const {
    return const_cast<Vector&>(*this).coord(which);
}

double& Vector::coord(const size_t which) {
    assert(which <= 3);
    return (which == 0 ? x : (which == 1 ? y : z));
}

[[nodiscard]] double Vector::length() const {
    return std::hypot(x, y, z);
}

Vector& Vector::operator+=(const Vector& v) {
    for (size_t i = 0; i < 3; ++i) {
        coord(i) += v.coord(i);
    }

    return *this;
}

Vector& Vector::operator-=(const Vector& v) {
    return *this += -v;
}

Vector& Vector::operator*=(const double value) {
    for (size_t i = 0; i < 3; ++i) {
        coord(i) *= value;
    }

    return *this;
}

Vector& Vector::operator/=(const double value) {
    return *this *= (1 / value);
}

Vector operator-(Vector v) {
    return v *= -1;
}

Vector operator-(Vector v1, const Vector v2) {
    return v1 -= v2;
}

Vector operator+(Vector v1, const Vector v2) {
    return v1 += v2;
}

Vector operator*(const double d, Vector v) {
    return v *= d;
}

Vector operator*(const Vector v, const double d) {
    return d * v;
}

Vector operator/(const Vector v, const double d) {
    return v * (1 / d);
}

}
