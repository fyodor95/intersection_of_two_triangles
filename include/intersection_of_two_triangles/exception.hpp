#include <stdexcept>

namespace intersection_of_two_triangles {

class Exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}
