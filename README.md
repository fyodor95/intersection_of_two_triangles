# Intersection of Two Triangles in 3D

## Description

A pair or triangles is given as a list of their vertices coordinates in three-dimensional space. This repository contains an algorithm that checks whether triangles are intersecting or not.

Notes:
- *Triangle* is a piece of plane, i.e. it contains not only its edges, but also the area enclosed by them.
- Triangles are intersecting iff they have at least one common point.
- Triangles can be degenerate.

## Usage

Prerequisites: you need to have CMake and Make installed. The program doesn't require any libraries apart from the C++ standard library. The program is written in C++17.

In order to build the program, do the following steps:
* go to the repository directory
* create a build directory and go to the created one:
```shell
mkdir build && cd build
```
* run the build:
```shell
cmake .. && make
```

At this point, the executable file `intersection_of_two_triangles` inside the `build` directory must be created. Now you can run tests:
```shell
build/intersection_of_two_triangles tests.txt
```

## Project structure
The input triangles are represented with the structure `GeneralTriangle`, which has the method
```c++
std::vector<std::variant<Point, Segment, Triangle>> as_non_degenerate() const;
```
It returns *non-degenerate* objects corresponding to the given "general" triangle. The goal of this function is to get a set of non-degenerate points/segments/triangles which closely represent the given triangle.
- For regular triangles — which are non-degenerate — it returns a vector of size 1 containing only that regular triangle.
- For a thin triangles the function can return
  1) the two longest edges of the triangle, or
  2) the longest edge and the vertex opposite to it, or
  3) just one edge (only if there are two triangle vertices which are exactly equal).
- In other cases, it returns the set of the unique vertices of the given triangle.

We will refer the returned set of points, segments and triangles as the *sub-objects* of the given general triangle.

The idea of returning multiple sub-objects is to increase the accuracy of the intersection algorithm. Although calculating the center of mass of the triangle vertices is simpler, it may result in missed intersections.

After constructing non-degenerate representations of the input triangles, we intersect each sub-object of the first triangle with each sub-object of the second input triangle. Each intersection of this kind is done by calling one of the overloaded functions `are_intersecting` (see the file `include/algorithms/are_intersecting.hpp`) — these functions do the real job. The program concludes that the initial general triangles intersect iff at least one intersection of the sub-objects is detected.
