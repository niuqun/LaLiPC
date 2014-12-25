// Structure Coordinate is defined in this file, which is used to represent
// the coordinates of each point in a height matrix. This structure is usually
// used be structure Point. The index starts from zero.

#ifndef COORDINATE_H_
#define COORDINATE_H_

struct Coordinate
{
	Coordinate()
	{
		x = 0;
		y = 0;
	}

	Coordinate(int i, int j)
	{
		x = i;
		y = j;
	}

	bool operator<(const Coordinate &right) const
	{
		return (x * 274 + y) < (right.x * 274 + right.y);
	}

	int x;
	int y;
};

#endif
