#ifndef REGRESSION_H_
#define REGRESSION_H_

#include <cmath>

#include "RegressResultException.h"

// This class uses regression result to compute the number of people
// according to the variables.
class Regression
{
public:
	// Returns the number of people.
	// variables: [1, highest, area, height, width, perimeter, h-w ratio,
	// roundness, complexity]
	// length: the number of variables in the array, it has to be 9 because
	// there are only 8 features plus a one in the beginning.
	static int CountPeopleNumber(double variables[], int length = 9);
};

#endif
