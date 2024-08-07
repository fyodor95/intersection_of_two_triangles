#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>

#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"
#include "intersection_of_two_triangles/algorithms/are_intersecting.hpp"
#include "intersection_of_two_triangles/algorithms/determinant.hpp"
#include "intersection_of_two_triangles/algorithms/dot_product.hpp"
#include "intersection_of_two_triangles/primitives/general_triangle.hpp"
#include "intersection_of_two_triangles/primitives/line.hpp"
#include "intersection_of_two_triangles/primitives/plane.hpp"
#include "intersection_of_two_triangles/primitives/segment.hpp"
#include "intersection_of_two_triangles/primitives/triangle.hpp"

namespace intersection_of_two_triangles {

namespace {

struct TrianglesTestResult {
    Plane plane;
    std::array<double, 3> signed_distances_to_triangle_plane;
    bool all_vertices_are_on_the_same_side;
    bool same_plane;

    // The vertex which is single in its half-space (the other two triangle vertices are in the other half-space).
    // When one of the triangle vertices belongs to `plane`, this vertex can be any of the two other vertices.
    // When two of the triangle vertices belong to `plane`, this vertex is the vertex which doesn't belong to `plane`.
    // This field is constructed only when `!all_vertices_are_on_the_same_side` and `!same_plane`.
    std::optional<size_t> lonely_vertex;
};

[[nodiscard]] TrianglesTestResult test_triangles(const Triangle& t1, const Triangle& t2) {
    TrianglesTestResult result{Plane(t2.vertex(0), t2.vertex(1), t2.vertex(2))};
    auto& distances_to_p2 = result.signed_distances_to_triangle_plane;
    std::vector<size_t> positive;
    std::vector<size_t> negative;

    for (size_t i = 0; i < 3; ++i) {
        distances_to_p2[i] = result.plane.signed_distance(t1.vertex(i));
        if (!are_nearly_equal(distances_to_p2[i], 0)) {
            if (distances_to_p2[i] > 0) {
                positive.push_back(i);
            } else {
                negative.push_back(i);
            }
        }
    }

    result.all_vertices_are_on_the_same_side = (positive.size() == 3 ||
                                                negative.size() == 3);
    result.same_plane = (positive.empty() &&
                         negative.empty());

    if (!result.all_vertices_are_on_the_same_side && !result.same_plane) {
        assert(positive.size() <= 2);
        assert(negative.size() <= 2);
        assert(positive.size() + negative.size() >= 1);
        if (positive.size() == negative.size()) {
            const size_t belonging_to_plane = 3 - positive[0] - negative[0];
            result.lonely_vertex = (distances_to_p2[belonging_to_plane] > 0 ? negative[0] : positive[0]);
        } else if (positive.size() + negative.size() != 2) {
            result.lonely_vertex = (positive.size() == 1 ? positive[0] : negative[0]);
        } else {
            result.lonely_vertex = 3 - (positive.size() == 2 ? positive[0] + positive[1] : negative[0] + negative[1]);
        }
    }

    return result;
}

// Projects p on the line ab. Returns the number t such that `t * (b - a)` is the projection.
// Returns `std::nullopt` iff `a` and `b` are too close.
[[nodiscard]] double project(const Point& p, const Point& a, const Point& b) {
    assert(!are_nearly_equal(a, b));
    const Vector ab = b.radius_vector() - a.radius_vector();
    const double ab2 = dot_product(ab, ab);
    assert(ab2 != 0);
    const Vector ap = p.radius_vector() - a.radius_vector();
    const double ap_ab = dot_product(ap, ab);
    return ap_ab / ab2;
}

// Returns the distance from p to the line through the points a and b.
[[nodiscard]] double distance_from_point_to_segment(const Point& p, const Point& a, const Point& b) {
    assert(!are_nearly_equal(a, b));
    const double t = project(p, a, b);

    // Check if t is within the segment
    if (t < 0.0) { // Closest point is a
        const Vector ap = p.radius_vector() - a.radius_vector();
        return ap.length();
    } else if (t > 1.0) { // Closest point is b
        const Vector bp = p.radius_vector() - b.radius_vector();
        return bp.length();
    } else { // Closest point is on the segment
        const Vector ap = p.radius_vector() - a.radius_vector();
        const Vector ab = b.radius_vector() - a.radius_vector();
        return (ap - t * ab).length();
    }
}

[[nodiscard]] bool in_range(const std::array<double, 2>& range, const double number) {
    return range[0] <= number && number <= range[1];
}

enum class IntersectionStatus {
    kIntersected,
    kOverlapped,
    kNotIntersected,
};

struct IntersectionResult {
    IntersectionStatus intersection_status;
    std::optional<std::array<double, 2>> st;
};

[[nodiscard]] IntersectionResult test_for_intersections(const Segment& s1, const Segment& s2) {
    const Point& a = s1.endpoint(0);
    const Point& b = s1.endpoint(1);
    const Point& c = s2.endpoint(0);
    const Point& d = s2.endpoint(1);

    assert(!are_nearly_equal(a, b));
    assert(!are_nearly_equal(c, d));

    // s1: tA + (1-t)B
    // s2: sC + (1-s)D
    //
    // The intersection of s1 and s2: tA + (1-t)B = sC + (1-s)D <=>
    // <=> t(A-B) + s(D-C) = D - B

    const Vector u = a - b;
    const Vector v = d - c;
    const Vector w = d - b;

    // tu + sv = w

    for (size_t coord0 = 0; coord0 < 2; ++coord0) {
        for (size_t coord1 = coord0 + 1; coord1 < 3; ++coord1) {
            const double vu_xy_det = determinant({v.coord(coord0), v.coord(coord1)},
                                                 {u.coord(coord0), u.coord(coord1)});
            if (vu_xy_det == 0 && coord0 + coord1 != 3) {
                continue;
            }
            const double wu_xy_det = determinant({w.coord(coord0), w.coord(coord1)},
                                                 {u.coord(coord0), u.coord(coord1)});
            // +vu_xy_det * s = wu_xy_det
            // -vu_xy_det * t = wv_xy_det
            if (are_nearly_equal(0, vu_xy_det)) {
                if (!are_nearly_equal(0, wu_xy_det) ||
                    !are_nearly_equal(0, determinant({w.coord(coord0), w.coord(coord1)},
                                                     {v.coord(coord0), v.coord(coord1)}))) {
                    return {IntersectionStatus::kNotIntersected};
                }
                if (are_intersecting(a, s2) || are_intersecting(b, s2) || are_intersecting(c, s1)) {
                    return {IntersectionStatus::kOverlapped};
                }
                return {IntersectionStatus::kNotIntersected};
            }
            const double s = wu_xy_det / vu_xy_det;
            if (s < 0 || 1 < s) {
                return {IntersectionStatus::kNotIntersected};
            }
            const double t =
                -determinant({w.coord(coord0), w.coord(coord1)}, {v.coord(coord0), v.coord(coord1)}) / vu_xy_det;
            const size_t coord2 = 3 - coord0 - coord1;
            if (0 <= t && t <= 1 && are_nearly_equal(t * u.coord(coord2) + s * v.coord(coord2), w.coord(coord2))) {
                return {IntersectionStatus::kIntersected, {{s, t}}};
            }
            return {IntersectionStatus::kNotIntersected};
        }
    }
}

[[nodiscard]] bool triangle_contains_coplanar_point(const Triangle& t, const Point& p) {
    const std::array<Segment, 2> basis{{{t.vertex(0), t.vertex(1)},
                                        {t.vertex(0), t.vertex(2)}}};
    std::array<IntersectionResult, 2> test_results;

    for (const bool i: {0, 1}) {
        test_results[i] = test_for_intersections(basis[i], {p, p - basis[!i].as_vector()});
        if (test_results[i].intersection_status == IntersectionStatus::kNotIntersected) {
            return false;
        }
        if (test_results[i].intersection_status == IntersectionStatus::kOverlapped) {
            return true;
        }
    }

    return (1 - (*test_results[0].st)[0]) + (1 - (*test_results[1].st)[0]) <= 1;
}

}

bool are_intersecting(const GeneralTriangle& gt1, const GeneralTriangle& gt2) {
    for (const auto& primitive1: gt1.as_non_degenerate()) {
        for (const auto& primitive2: gt2.as_non_degenerate()) {
            if (std::visit([](auto&& arg1, auto&& arg2) { return are_intersecting(arg1, arg2); },
                           primitive1, primitive2)) {
                return true;
            }
        }
    }

    return false;
}

bool are_intersecting(const Point& p1, const Point& p2) {
    return are_nearly_equal(p1, p2);
}

bool are_intersecting(const Point& p, const Segment& s) {
    return are_nearly_equal(0, distance_from_point_to_segment(p, s.endpoint(0), s.endpoint(1)));
}

bool are_intersecting(const Point& p, const Triangle& t) {
    const Plane plane(t.vertex(0), t.vertex(1), t.vertex(2));
    return are_nearly_equal(0, plane.signed_distance(p)) && triangle_contains_coplanar_point(t, p);
}

bool are_intersecting(const Segment& s, const Point& p) {
    return are_intersecting(p, s);
}

bool are_intersecting(const Segment& s1, const Segment& s2) {
    return test_for_intersections(s1, s2).intersection_status != IntersectionStatus::kNotIntersected;
}

bool are_intersecting(const Segment& s, const Triangle& t) {
    const Plane plane(t.vertex(0), t.vertex(1), t.vertex(2));
    const Line line(s.endpoint(1) - s.endpoint(0), s.endpoint(0));

    // u * direction + o = point and dot_product(normal, point) + d = 0 =>
    // dot_product(normal, u * direction + o) + d = 0 =>
    // u * dot_product(normal, direction) = -(d + dot_product(normal, o))

    const double denominator = dot_product(plane.normal, line.direction);
    const double normal_dot_o = dot_product(plane.normal, line.o.radius_vector());

    if (are_nearly_equal(0, denominator)) {
        if (!are_nearly_equal(-plane.d, normal_dot_o)) {
            return false;
        }
        // s and t are coplanar
        for (size_t i = 0; i < 3; ++i) {
            if (are_intersecting(s, t.edge(i))) {
                return true;
            }
        }
        return are_intersecting(s.endpoint(0), t);
    }

    const double u = -(plane.d + normal_dot_o) / denominator;
    return 0 <= u && u <= 1 && triangle_contains_coplanar_point(t, line.o + u * line.direction);
}

bool are_intersecting(const Triangle& t, const Point& p) {
    return are_intersecting(p, t);
}

bool are_intersecting(const Triangle& t, const Segment& s) {
    return are_intersecting(s, t);
}

// The algorithm is described here: https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tritri_tam.pdf
bool are_intersecting(const Triangle& t1, const Triangle& t2) {
    const std::array<const Triangle*, 2> ts{&t1, &t2};
    std::array<std::optional<TrianglesTestResult>, 2> triangles_tests_results;

    for (const bool i: {0, 1}) {
        triangles_tests_results[i].emplace(test_triangles(*ts[i], *ts[!i]));
        if (triangles_tests_results[i]->all_vertices_are_on_the_same_side) {
            return false;
        }
        if (triangles_tests_results[i]->same_plane) {
            // TODO: this part can be optimized.
            for (size_t edge_index_1 = 0; edge_index_1 < 3; ++edge_index_1) {
                for (size_t edge_index_2 = 0; edge_index_2 < 3; ++edge_index_2) {
                    if (are_intersecting(t1.edge(edge_index_1), t2.edge(edge_index_2))) {
                        return true;
                    }
                }
            }
            return (are_intersecting(t1.vertex(0), t2) ||
                    are_intersecting(t2.vertex(0), t1));
        }
    }

    if (const auto planes_intersection = intersection(triangles_tests_results[0]->plane,
                                                      triangles_tests_results[1]->plane)) {
        std::array<std::array<double, 2>, 2> us;
        for (const bool i: {0, 1}) {
            std::array<double, 3> projs;
            const size_t o = *triangles_tests_results[i]->lonely_vertex;
            for (size_t j = 0; j < 3; ++j) {
                projs[j] = dot_product(planes_intersection->direction,
                                       ts[i]->vertex((o + j) % 3) - planes_intersection->o);
            }
            const auto& signed_dists = triangles_tests_results[i]->signed_distances_to_triangle_plane;
            for (const bool j: {0, 1}) {
                us[i][j] = projs[1 + j] + (projs[0] - projs[1 + j]) * signed_dists[(o + 1 + j) % 3] /
                                          (signed_dists[(o + 1 + j) % 3] - signed_dists[o]);
            }
            std::sort(us[i].begin(), us[i].end());
        }
        return in_range(us[0], us[1][0]) || in_range(us[0], us[1][1]) || in_range(us[1], us[0][0]);
    }

    return false;
}

}
