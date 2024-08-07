#include <algorithm>
#include <cmath>
#include <limits>

#include "intersection_of_two_triangles/algorithms/are_nearly_equal.hpp"

namespace intersection_of_two_triangles {

bool are_nearly_equal(const double a, const double b)
{
    static constexpr double absolute_epsilon = 1e-22;
    static constexpr double relative_epsilon = std::numeric_limits<double>::epsilon() * 128;
    const double diff = std::abs(a - b);
    const double norm = std::min(std::abs(a) + std::abs(b), std::numeric_limits<double>::max());
    return diff < std::max(absolute_epsilon, relative_epsilon * norm);
}

}
