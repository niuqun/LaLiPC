#ifndef COUNTING_RESULT_
#define COUNTING_RESULT_

#include <iostream>
using std::ostream;

// This class contains the result of people counting, assigned by HeightMap
// and displayed by main function.
class CountingResult
{
public:
	CountingResult();
	friend ostream &operator<<(ostream &output, const CountingResult &result);

public:
	int max_height;
	int area;
	int width;
	int height;
	int perimeter_;

	int people_number_;

	int people_in_number_;
	int people_out_number_;
	int people_unknown_number_;
};

#endif
