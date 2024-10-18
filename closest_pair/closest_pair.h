#pragma once

#include "point.h"

#include <vector>

// Find the closest pair of points from given points.
std::pair<Point, Point> closest_pair_brute(const std::vector<Point> &points);
std::pair<Point, Point> closest_pair(const std::vector<Point> &points);
