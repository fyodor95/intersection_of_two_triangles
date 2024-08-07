#include <cstddef>

#include "intersection_of_two_triangles/algorithms/cross_product.hpp"
#include "intersection_of_two_triangles/algorithms/determinant.hpp"
#include "intersection_of_two_triangles/algorithms/dot_product.hpp"
#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/exception.hpp"
#include "intersection_of_two_triangles/primitives/line.hpp"
#include "intersection_of_two_triangles/primitives/plane.hpp"
#include "intersection_of_two_triangles/primitives/point.hpp"
#include "intersection_of_two_triangles/primitives/vector.hpp"

namespace intersection_of_two_triangles {

Plane::Plane(const Point& a, const Point& b, const Point& c) :
    normal(cross_product(b - a, c - a)), d(-dot_product(normal, a.radius_vector())) {
    if (normal.is_zero()) {
        throw Exception("Plane::Plane: the given points are collinear");
    }
}

double Plane::signed_distance(const Point& to) const {
    return dot_product(normal, to.radius_vector()) + d;
}

std::optional<Line> intersection(const Plane& p1, const Plane& p2) {
    const Vector direction = cross_product(p1.normal, p2.normal);

    if (direction.is_zero()) {
        return std::nullopt;
    }

    for (size_t coord0 = 0; coord0 < 2; ++coord0) {
        for (size_t coord1 = coord0 + 1; coord1 < 3; ++coord1) {
            const size_t not_chosen = 3 - coord0 - coord1;
            const std::array<std::array<double, 2>, 2> chosen{
                {
                    {p1.normal.coord(coord0),
                     p2.normal.coord(coord0)},
                    {p1.normal.coord(coord1),
                     p2.normal.coord(coord1)},
                }
            };
            const std::array<double, 2> ds{p1.d, p2.d};
            const double chosen_det = determinant(chosen[0], chosen[1]);
            if (chosen_det == 0) {
                continue;
            }
            Point point_on_result(0, 0, 0);
            point_on_result.coord(coord0) = determinant(ds, chosen[1]) / -chosen_det;
            point_on_result.coord(coord1) = determinant(ds, chosen[0]) / +chosen_det;
            for (const Plane* const p: {&p1, &p2}) {
                if (!are_nearly_equal(0, p->normal.coord(not_chosen))) {
                    point_on_result.coord(not_chosen) =
                        -(p->d +
                          p->normal.coord(coord0) * point_on_result.coord(coord0) +
                          p->normal.coord(coord1) * point_on_result.coord(coord1)) /
                        p->normal.coord(not_chosen);
                    break;
                }
            }
            return Line(direction, point_on_result);
        }
    }

    return std::nullopt;
}

}
