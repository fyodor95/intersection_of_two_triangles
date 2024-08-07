#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "intersection_of_two_triangles/algorithms/are_intersecting.hpp"
#include "intersection_of_two_triangles/primitives/general_triangle.hpp"
#include "intersection_of_two_triangles/primitives/point.hpp"

int main(const int argc, const char* const* const argv) {
    const bool test_implementations = argc >= 2 && !std::strcmp(argv[1], "--test-implementations");

    if (argc <= 1 + test_implementations) {
        std::cerr << "A test file must be provided as a command line argument. "
                  << "Example: intersection_of_two_triangles ./tests.txt" << std::endl;
        return 1;
    }

    std::cout << std::boolalpha;

    for (int i = 1 + test_implementations; i < argc; ++i) {
        std::ifstream in(argv[i]);
        std::string line;
        std::vector<double> input;
        input.reserve(18);
        size_t total_number_of_tests = 0;
        size_t number_of_failed_tests = 0;
        for (size_t line_index = 1; std::getline(in, line); ++line_index) {
            if (line[0] == '#') {
                continue;
            }
            std::optional<bool> expected_answer;
            if (line.find("false") != std::string::npos) {
                expected_answer = false;
            } else if (line.find("true") != std::string::npos) {
                expected_answer = true;
            }
            if (expected_answer) {
                ++total_number_of_tests;
                assert(input.size() == 18);
                using namespace intersection_of_two_triangles;
                //std::cout << "Running test " << total_number_of_tests << std::endl;
                const bool result = are_intersecting(
                    GeneralTriangle{
                        Point(input[0], input[1], input[2]),
                        Point(input[3], input[4], input[5]),
                        Point(input[6], input[7], input[8])},
                    GeneralTriangle{
                        Point(input[9], input[10], input[11]),
                        Point(input[12], input[13], input[14]),
                        Point(input[15], input[16], input[17])});
                if (*expected_answer != result) {
                    std::cout << "line " << line_index << ": expected " << *expected_answer << ", got " << result
                              << '\n';
                    ++number_of_failed_tests;
                }
                input.clear();
                continue;
            }
            const char* begin{};
            char* end = line.data();
            for (; ; ) {
                begin = end;
                const double parsed = std::strtod(begin, &end);
                if (begin != end) {
                    input.push_back(parsed);
                } else {
                    break;
                }
                if (errno == ERANGE) {
                    std::cerr << "A range error occurred for " << std::string_view(begin, *end) << '\n';
                    errno = 0;
                    return 1;
                }
            }
            if (input.empty()) {
                continue;
            }
            assert(input.size() % 3 == 0 && input.size() <= 18);
            const size_t start = (input.size() <= 9 ? 0 : 9);
            while (input.size() < start + 9) {
                input.insert(input.end(), input.begin() + start, input.begin() + start + 3);
            }
        }
        std::cout << "Tests done " << total_number_of_tests << '/' << number_of_failed_tests << " failed\n";
    }
}
