#include "Regression.h"

int Regression::CountPeopleNumber(double variables[], int length)
{
	/*double theta[] = {0.7234, -0.0044, -0.0001, -0.0244, 0.0012, 0.0126,
	-0.7263, 0.5693, -0.0046};*/
	/*double theta[] = { -1.1395, 	-0.0021, 0.0007, -0.2361, 0.0211, 0.0221,
						5.2605, 0.9037, 0.0188};*/
	double theta[] = { 4.6739, -0.0237, 	0.0011, 0.1517, -0.1312, 0.0649,
		-8.9738, 1.1162, 0.0152};
	double regress_result = 0.0;
	int people_number = 0;

	for (int i = 0; i < length; ++i)
	{
		regress_result += theta[i] * variables[i];
	}

	people_number = static_cast<int>(floor(regress_result + 0.5));

	if (people_number <= 0)
	{
		throw RegressResultException();
	}
	else
	{
		return people_number;
	}
}
