#ifndef REGRESS_RESULT_EXCEPTION_
#define REGRESS_RESULT_EXCEPTION_

// This exception is thrown by Regression class when the regress result
// of linear regression is smaller than 0, which is not realistic because
// where there is a HeightMap, there are a few pedestrians inside.

class RegressResultException
{
public:
	RegressResultException() {}
	~RegressResultException() {}

	const char* show_reason() const { return "regress result smaller than 0"; }
};

#endif
