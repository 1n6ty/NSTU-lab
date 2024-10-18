#include "closest_pair.h"
#include <limits.h>
#include <algorithm>
#include <stdexcept>

using namespace std;

std::pair<Point, Point> closest_pair_brute(const std::vector<Point> &points) {
	// Return the closest pair of points from given points.
	// Order of points in the result doesn't matter.
	// If there are fewer than 2 points, throw an exception.

	if (points.size() < 2) {
		throw invalid_argument("Not enough points");
	}	

	std::pair<Point, Point> result;	
	double result_dist = __DBL_MAX__, tmp_dist;
	for(auto it_point_fst = points.begin(); it_point_fst != points.end() - 1; it_point_fst++){
		for(auto it_point_sec = it_point_fst + 1; it_point_sec != points.end(); it_point_sec++){
			tmp_dist = (*it_point_fst).distance(*it_point_sec);
			if(tmp_dist < result_dist){
				result_dist = tmp_dist;
				result = std::make_pair(Point(*it_point_fst), Point(*it_point_sec));
			}
		}
	}
	
	return result;
}

std::pair<Point, Point> closest_pair_between(const std::vector<Point> &points_cpy) {
	int points_size = points_cpy.size();

	int mid = points_size / 2;

	std::vector<std::pair<Point, Point>> range;

	range.push_back(closest_pair_between(std::vector<Point>(points_cpy.begin(), points_cpy.begin() + mid)));
	range.push_back(closest_pair_between(std::vector<Point>(points_cpy.begin() + mid, points_cpy.end())));

	double d = std::min(range[0].first.distance(range[0].second), range[1].first.distance(range[1].second));
	double xm = (points_cpy[mid - 1].x + points_cpy[mid].x) / 2;

	std::vector<Point> pstripe;
	for(int i = mid - 1; i >= 0; i--){
		if(abs(points_cpy[i].x - xm) < d) pstripe.push_back(points_cpy[i]);
		else break;
	}
	for(int i = mid; i < points_size; i++){
		if(abs(points_cpy[i].x - xm) < d) pstripe.push_back(points_cpy[i]);
		else break;
	}

	std::sort(pstripe.begin(), pstripe.end(), [&](Point a, Point b){return a.y < b.y;});

	int pstripe_size = pstripe.size();
	double tmp_dist = __DBL_MAX__, min_dist = __DBL_MAX__;
	for(int i = 0; i < pstripe_size - 1; i++){
		for(int j = i + 1; j < pstripe_size; j++){
			if(abs(pstripe[i].y - pstripe[j].y) < d){
				tmp_dist = pstripe[i].distance(pstripe[j]);
				if(tmp_dist < min_dist){
					min_dist = tmp_dist;
					if(range.size() < 3) range.push_back(std::make_pair(pstripe[i], pstripe[j]));
					else range[2] = std::make_pair(pstripe[i], pstripe[j]);
				}
			} else break;
		}
	}

	std::sort(range.begin(), range.end(), [&](std::pair<Point, Point> a, std::pair<Point, Point> b){return a.first.distance(a.second) < b.first.distance(b.second);});

	return range[0];
}

std::pair<Point, Point> closest_pair(const std::vector<Point> &points){
	// Return the closest pair of points from given points.
	// Order of points in the result doesn't matter.
	// If there are fewer than 2 points, throw an exception.
	int points_size = points.size();
	if (points_size < 2) {
		throw invalid_argument("Not enough points");
	}

	if(points_size < 4) return closest_pair_brute(points);

	std::vector<Point> points_cpy = points;
	std::sort(points_cpy.begin(), points_cpy.end(), [&](Point a, Point b){return a.x < b.x;});

	return closest_pair_between(points_cpy);
}