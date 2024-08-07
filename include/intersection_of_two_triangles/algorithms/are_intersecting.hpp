#pragma once

namespace intersection_of_two_triangles {

struct GeneralTriangle;
struct Point;

class Segment;
class Triangle;

[[nodiscard]] bool are_intersecting(const GeneralTriangle&, const GeneralTriangle&);

[[nodiscard]] bool are_intersecting(const Point    &, const Point    &);
[[nodiscard]] bool are_intersecting(const Point    &, const Segment  &);
[[nodiscard]] bool are_intersecting(const Point    &, const Triangle &);
[[nodiscard]] bool are_intersecting(const Segment  &, const Point    &);
[[nodiscard]] bool are_intersecting(const Segment  &, const Segment  &);
[[nodiscard]] bool are_intersecting(const Segment  &, const Triangle &);
[[nodiscard]] bool are_intersecting(const Triangle &, const Point    &);
[[nodiscard]] bool are_intersecting(const Triangle &, const Segment  &);
[[nodiscard]] bool are_intersecting(const Triangle &, const Triangle &);

}
