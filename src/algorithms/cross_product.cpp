#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/algorithms/cross_product.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

Vector cross_product(const Vector& v1, const Vector& v2) {
    const double yz = v1.y * v2.z;
    const double zy = v1.z * v2.y;
    const double zx = v1.z * v2.x;
    const double xz = v1.x * v2.z;
    const double xy = v1.x * v2.y;
    const double yx = v1.y * v2.x;

    if (are_nearly_equal(yz, zy) && are_nearly_equal(zx, xz) && are_nearly_equal(xy, yx)) {
        return {0, 0, 0};
    }

    return {
        yz - zy,
        zx - xz,
        xy - yx,
    };
}

}
