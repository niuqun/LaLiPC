#ifndef HEIGHT_MATRIX_H_
#define HEIGHT_MATRIX_H_

#include <cmath>
#include <vector>
#include <queue>
#include "HeightMap.h"
#include "CountingResult.h"
#include "Regression.h"

// Direction represents the search direction of breadth first traverse
// algorithm. Four directions are up, down, left and right
struct Direction
{
	int x;
	int y;
};


// This class is primarily used by main to store the height matrix where
// pedestrians have shown up. The breadth first traverse algorithm is applied
// to find height map of one pedestrian or pedestrians
class HeightMatrix
{
public:
	 HeightMatrix(int row = 20, int column = 274,
		 int least_detect_time = 2, int most_missing_time = 1,
		 int start_index = 0, int end_index = 273,
		 int laser_height = 300);
	 ~HeightMatrix();

	 // Adds a newly read frame of distance to height matrix,
	 // before which, a transformation of distance data to height
	 // data is applied.
	 // distance: address of array which stored the distances
	 // length: length of distance array
	 void AddFrameHeight(int *distance, int length);

	 // Returns the number of height maps
	 int SearchHeightMap();

	 // Returns if the coordinate c is in the height matrix
	 bool IsCoordinateInMatrix(Coordinate c);

	 // Returns if the coordinate c is on the edge
	 bool IsCoordinateOnEdge(Coordinate c);

	 // Returns the height map starting from the given Coordinate
	 // (x, y)
	 HeightMap BreadthFirstSearch(Coordinate c);

	 // Gets the counting results from HeightMap
	 void Result(CountingResult &r);

private:
	// A pointer to height matrix
	int **matrix_;
	// A pointer to visited matrix
	bool **visited_;
	// Row of height matrix
	int row_;
	// Column of height matrix
	int column_;

	// Start index of scanning point in a frame
	int start_index_;
	// End index of scanning point in a frame
	int end_index_;
	// Height of laser range scanner in centimeters
	int laser_height_;

	// Actual number of non-zero rows
	int non_zero_row_number_;
	// Actual number of zero rows encountered
	int zero_row_number_;

	// Threshold of least number of row in this matrix
	int least_detect_time_;
	// Threshold of most zero rows encountered before people counting applied
	int most_missing_time_;

	// The collection of height maps in this height matrix
	std::vector<HeightMap> height_maps_;

	// Counting results
	CountingResult result_;
};

#endif
