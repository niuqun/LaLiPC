#ifndef HEIGHT_MAP_H_
#define HEIGHT_MAP_H_

#include "Point.h"
#include "ErrorCode.h"
#include "CountingResult.h"
#include "WalkingDirection.h"
#include "SliderSizeZeroException.h"
#include <map>
#include <vector>

// HeightMap is a collection of scanning points representing one or several
// walking pedestrians. This class has functions and variables needed to
// construct a HeightMap and also provides interface for other classes
// to retrieve information about this HeightMap if needed.
class HeightMap
{
public:
	// Constructor. Called by methods in HeightMatrix.
	HeightMap();
	
	// Adds new points to height map, this is called by methods in height
	// matrix. Point p is the point to be inserted.
	// Returns ErrorCode depending on the result of this operation.
	// For more information about ErrorCode, see ErrorCode.h
	ErrorCode AddNewPoints(const Point &p);
	
	// Returns if Point with Coordinate c is already in this height map.
	// Usually called by methods in HeightMatrix.
	bool HasPoint(Coordinate c);

	// Returns the direction of each pedestrian calculated by linear
	// regression.
	void DetermineWalkingDirections(int head_number, int step_size,
		CountingResult &result);

	// getters
	int max_height() const { return max_height_; }
	int points_number() const { return points_number_; }
	int max_x() const { return max_x_; }
	int min_x() const { return min_x_; }
	int max_y() const { return max_y_; }
	int min_y() const { return min_y_; }
	int perimeter() const { return perimeter_; }

private:
	// Returns insert result. This is a private function called by AddNewPoints.
	// The job of insertion is actually done by this function.
	ErrorCode InsertPointToMap(const Point &p);

	// This function does the actual work of determining the direction
	// of each pedestrian. It creates a HeaderSlider object and use it
	// to determine the direction of each slider.
	void DetermineDirections(int head_number, int step_size, CountingResult &r);

private:
	// HeadSlider is exclusively created and manipulated by its enclosing
	// class, HeightMap. A sliding window is employed to locate the heads
	// in this height map (if there's any) and determine the direction of
	// each head, or pedestrian
	class HeadSlider
	{
	public:
		// head_number: calculated by regression method
		// step_size: slider move step, 1 by default
		HeadSlider(int head_number, int step_size = 1);

		// Slides through the height map, detects each head and determine
		// its owner's direction
		void SlideThrough(HeightMap &height_map, CountingResult &result);

	private:
		// Prepares the front height of the height map, and prepare the vote
		// of each scanning point
		void Prepare(HeightMap &height_map);

		// Returns the number of extrema in an averaged height plot
		// average: the height array of average height in slider
		// extrema: the extrema index list
		int LocateHeads(int average[], std::vector<int> &extrema);

		// Returns the walking direction of each slider window, the
		// index of which are actually the extrema index calculated by
		// LocateHeads.
		// extrema: the center of each slider
		// directions: direction of each slider
		int DetermineDirectionOfOne(const std::vector<int> &extrema,
			std::vector<WalkingDirection> &directions);

	private:
		// The front project of this height map
		int front_height_[274];
		// Votes result of scanning points
		WalkingDirection votes_[274];
		// Number of height in the height map
		int head_number_;
		// Slider size
		int slider_size_;
		// Step size of the slider
		int step_size_;
	};


private:
	// Stores height map in this structure, using Coordinate as key
	std::map<Coordinate, Point> points;

	// The highest in the height map
	int max_height_;
	// Number of points in this height map
	int points_number_;
	// Maximal x value of points in height map
	int max_x_;
	// Minimal x value of points in height map
	int min_x_;
	// Maximal y value of points in height map
	int max_y_;
	// Minimal y value of points in height map
	int min_y_;
	// Perimeter of height map
	int perimeter_;
};

#endif
