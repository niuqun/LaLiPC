#include "CountingResult.h"

CountingResult::CountingResult()
{
	max_height = 0;
	area = 0;
	width = 0;
	height = 0;
	perimeter_ = 0;
	people_number_ = 0;
	people_in_number_ = 0;
	people_out_number_ = 0;
	people_unknown_number_ = 0;
}

ostream &operator<<(ostream &output, const CountingResult &result)
{
	/*output << "max_height = " << result.max_height
		<< "  area = " << result.area
		<< " width = " << result.width
		<< " height = " << result.height
		<< " perimeter = " << result.perimeter_ << std::endl;*/
	output << "people_number = " << result.people_number_ << std::endl;

	return output;
}
