#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/algorithms/determinant.hpp"

namespace intersection_of_two_triangles {

double determinant(const std::array<double, 2>& a,
                   const std::array<double, 2>& b) {
    const double l = a[0] * b[1];
    const double r = a[1] * b[0];
    return (are_nearly_equal(l, r)) ? 0. : l - r;
}

}
