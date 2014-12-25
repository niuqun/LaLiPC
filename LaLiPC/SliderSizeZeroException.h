#ifndef SLIDER_SIZE_ZERO_EXCEPTION_
#define SLIDER_SIZE_ZERO_EXCEPTION_

// This exception is thrown by HeightSlider.SlideThrough method when the size of
// slide window is equal to 0. In this case, it is easily assumed that the number
// of people regressed by linear regression is too large.

class SliderSizeZeroException
{
public:
	SliderSizeZeroException() {};
	~SliderSizeZeroException() {};

	const char* show_reason() const { return "slider_size == 0"; }
};

#endif
