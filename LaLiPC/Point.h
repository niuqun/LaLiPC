// Structure Point to represent each scanning point with coordinate and
// height and boundary information. Point is used by HeightMap to stand
// for a pedestrian or several pedestrians. 


#ifndef POINT_H_
#define POINT_H_

#include "Coordinate.h"

// Coordinate is used to locate this scanning point in height map with
// additional information.
struct Point
{
	Point()
	{
		is_boundary = false;
		height = 0;
	}

	Point(Coordinate co, bool b, int h)
	{
		c = co;
		is_boundary = b;
		height = h;
	}

	// position of this point in height matrix
	Coordinate c;
	// if this point is at the edge of the height map to which it belongs
	bool is_boundary;
	// scanning height of this scanning point in centimeters
	int height;
};

#endif
