#include "HeightMap.h"

HeightMap::HeightMap()
{
	points.clear();
	
	max_height_ = 0;
	points_number_ = 0;
	max_x_ = 0;
	min_x_ = 1000;
	max_y_ = 0;
	min_y_ = 1000;
	perimeter_ = 0;
}

ErrorCode HeightMap::AddNewPoints(const Point &p)
{
	Coordinate c = p.c;
	ErrorCode code = InsertFailed;

	if (HasPoint(c))
	{
		code = AlreadyExist;
	}
	else
	{
		code = InsertPointToMap(p);
	}

	return code;
}

bool HeightMap::HasPoint(Coordinate c)
{
	bool has_point_in_map = false;

	if (points.find(c) != points.end())
	{
		has_point_in_map = true;
	}

	return has_point_in_map;
}

ErrorCode HeightMap::InsertPointToMap(const Point &p)
{
	ErrorCode code = InsertFailed;
	Coordinate c = p.c;
	std::pair<std::map<Coordinate, Point>::iterator, bool> ret;

	ret = points.insert(std::make_pair<Coordinate, Point>(c, p));

	if (ret.second == true)
	{
		code = InsertSuccessful;

		// update height map information
		if (p.height > max_height_)
		{
			max_height_ = p.height;
		}

		++points_number_;

		if (p.c.x > max_x_)
		{
			max_x_ = p.c.x;
		}

		if (p.c.x < min_x_)
		{
			min_x_ = p.c.x;
		}

		if (p.c.y > max_y_)
		{
			max_y_ = p.c.y;
		}

		if (p.c.y < min_y_)
		{
			min_y_ = p.c.y;
		}

		if (p.is_boundary)
		{
			++perimeter_;
		}
	}
	else
	{
		code = AlreadyExist;
	}

	return code;
}

void HeightMap::DetermineWalkingDirections(int head_number, int step_size,
	CountingResult &result)
{
	if (head_number > 0)
	{
		DetermineDirections(head_number, step_size, result);
	}
}

void HeightMap::DetermineDirections(int head_number, int step_size,
	CountingResult &r)
{
	HeadSlider head_slider(head_number, step_size);

	head_slider.SlideThrough(*this, r);
}

HeightMap::HeadSlider::HeadSlider(int head_number, int step_size /* = 1 */)
{
	head_number_ = head_number;
	step_size_ = step_size;

	for (int i = 0; i < 274; ++i)
	{
		front_height_[i] = 0;
		votes_[i] = Unknown;
	}
}

void HeightMap::HeadSlider::SlideThrough(HeightMap &height_map,
	CountingResult &result)
{
	// it's common that the shoulder length is about twice the length of
	// the head's length
	slider_size_ = (height_map.max_y_ - height_map.min_y_) /
		(head_number_ * 2);

	if (slider_size_ == 0)
	{
		throw SliderSizeZeroException();
	}

	Prepare(height_map);

	int sum = 0;
	int average[274] = {0};

	for (int i = 0;
		i <= slider_size_ - 1;
		++i)
	{
		sum += front_height_[i];
	}
	average[height_map.min_y_] = sum / slider_size_;

	for (int i = 1;
		i <= 274 - slider_size_;
		++i)
	{
		sum -= front_height_[i - 1];
		sum += front_height_[i + slider_size_ - 1];
		average[i] = sum / slider_size_;
	}

	// TOTHINK: It suddenly occurred to me that, if the the number of extrema
	// did not equal to the linear regression result, what should we do to
	// deal with this sort of situation.
	// For temporary solution, assigned 'Undefined' to those missing

	std::vector<int> extrema_list;
	std::vector<WalkingDirection> direction_list;
	int slider_number = 0;

	slider_number = LocateHeads(average, extrema_list);
	DetermineDirectionOfOne(extrema_list, direction_list);

	for (int i = 0; i < direction_list.size(); ++i)
	{
		if (direction_list[i] == In)
		{
			++(result.people_in_number_);
		}
		else if (direction_list[i] == Out)
		{
			++(result.people_out_number_);
		}
		else
		{
			++(result.people_unknown_number_);
		}
	}

	if (slider_number < head_number_)
	{
		result.people_unknown_number_ += (head_number_ - slider_number);
	}	
}

void HeightMap::HeadSlider::Prepare(HeightMap &height_map)
{
	Coordinate c;
	std::map<Coordinate, Point>::iterator ret;
	
	for (int i = height_map.min_y_; i <= height_map.max_y_; ++i)
	{
		int max_height_index;
		int max_height_value = 0;
		int first_non_zero_row = height_map.min_x_;
		int last_non_zero_row = height_map.max_x_;
		
		for (int j = height_map.min_x_; j <= height_map.max_x_; ++j)
		{
			c.x = j;
			c.y = i;

			ret = height_map.points.find(c);
			if (ret != height_map.points.end() && ret->second.height >= 100)
			{
				first_non_zero_row = j;
				break;
			}
		}

		for (int j = height_map.max_x_; j >= height_map.min_x_; --j)
		{
			c.x = j;
			c.y = i;

			ret = height_map.points.find(c);
			if (ret != height_map.points.end() && ret->second.height >= 100)
			{
				last_non_zero_row = j;
				break;
			}
		}

		for (int j = first_non_zero_row; j <= last_non_zero_row; ++j)
		{
			c.x = j;
			c.y = i;

			ret = height_map.points.find(c);
			if (ret != height_map.points.end() &&
				ret->second.height > max_height_value)
			{
				max_height_value = ret->second.height;
				max_height_index = j;
			}
		}

		// prepares the front height
		front_height_[i] = max_height_value;
		
		if (front_height_[i] >= 100)
		{
			// prepares the votes_
			if (max_height_index - first_non_zero_row >
				last_non_zero_row - max_height_index)
			{
				votes_[i] = In;
			}
			else if (max_height_index - first_non_zero_row <
				last_non_zero_row - max_height_index)
			{
				votes_[i] = Out;
			}
			else
			{
				votes_[i] = Unknown;
			}
		}
		else
		{
			votes_[i] = Unknown;
		}
	}
}

int HeightMap::HeadSlider::LocateHeads(int average[], std::vector<int> &extrema)
{
	bool is_increasing = false;

	for (int i = 1; i < 274; ++i)
	{
		if (average[i] > average[i - 1])
		{
			is_increasing = true;
		}
		else if (average[i] < average[i - 1])
		{
			if (is_increasing)
			{
				if (extrema.size() > 0)
				{
					int last_head = extrema.back();
					int gap = slider_size_;

					if (i > 200 || i < 100)
					{
						gap = slider_size_ * 2 / 3;
					}

					if (i - last_head <= gap)
					{
						extrema.pop_back();
						extrema.push_back((last_head + i) / 2);
					}
					else
					{
						extrema.push_back(i - 1);
					}
				}
				else
				{
					extrema.push_back(i - 1);
				}
			}
			is_increasing = false;
		}
		else
		{
			// do nothing
		}
	}

	return extrema.size();
}

int HeightMap::HeadSlider::DetermineDirectionOfOne(
	const std::vector<int> &extrema,
	std::vector<WalkingDirection> &directions)
{
	int length = 0;

	for (int i = 0; i < extrema.size(); ++i)
	{
		if (extrema[i] < 100 || extrema[i] > 200)
		{
			length = slider_size_ * 2 / 3;
		}
		else
		{
			length = slider_size_;
		}

		int in_votes = 0;
		int out_votes = 0;
		int unknown_votes = 0;

		int left_bound = extrema[i] - length / 3;
		int right_bound = extrema[i] + 4 * length / 3;

		if (left_bound < 0)
		{
			left_bound = 0;
		}
		if (right_bound > 274)
		{
			left_bound = 274;
		}

		for (int j = left_bound; j < right_bound; ++j)
		{
			if (votes_[j] == In)
			{
				++in_votes;
			}
			else if (votes_[j] == Out)
			{
				++out_votes;
			}
			else
			{
				++unknown_votes;
			}
		}

		if (in_votes > out_votes)
		{
			directions.push_back(In);
		}
		else if (in_votes < out_votes)
		{
			directions.push_back(Out);
		}
		else
		{
			directions.push_back(Unknown);
		}
	}

	return directions.size();
}
