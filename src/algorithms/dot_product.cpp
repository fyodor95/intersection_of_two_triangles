#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/algorithms/dot_product.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

double dot_product(const Vector& v1, const Vector& v2) {
    const double x = v1.x * v2.x;
    const double y = v1.y * v2.y;
    const double z = v1.z * v2.z;

    if (are_nearly_equal(-x, y + z) ||
        are_nearly_equal(-y, z + x) ||
        are_nearly_equal(-z, x + y)) {
        return 0;
    }

    return x + y + z;
}

}
