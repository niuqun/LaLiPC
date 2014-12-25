#include "HeightMatrix.h"

const double kPI = 3.1415;

HeightMatrix::HeightMatrix(int row /* = 20 */, int column /* = 274 */,
	int least_detect_time /* = 2 */, int most_missing_time /* = 1 */,
	int start_index /* = 0 */, int end_index /* = 273 */,
	int laser_height /* = 300 */)
{
	row_ = row;
	column_ = column;

	start_index_ = start_index;
	end_index_ = end_index;
	laser_height_ = laser_height;

	non_zero_row_number_ = 0;
	zero_row_number_ = 0;

	least_detect_time_ = least_detect_time;
	most_missing_time_ = most_missing_time;

	matrix_ = new int *[row_];
	for (int i = 0; i < row_; ++i)
	{
		matrix_[i] = new int[column_];
		memset(matrix_[i], 0, column_ * sizeof(int));
	}

	visited_ = new bool *[row_];
	for (int i = 0; i < row_; ++i)
	{
		visited_[i] = new bool[column_];
		memset(visited_[i], 0, column_ * sizeof(bool));
	}
}

HeightMatrix::~HeightMatrix()
{
	for (int i = 0; i < row_; ++i)
	{
		delete[] matrix_[i];
		delete[] visited_[i];
	}
	delete[] matrix_;
	delete[] visited_;
	matrix_ = 0;
	visited_ = 0;
}

// This function is called from main function to append distance frame
// to height matrix. First, distance data is transformed to height data;
// Second, the height data is checked to see if any prominences are
// present, if so, height frame is added to height matrix; if not, the
// number of zero_row_numbers shall be compared with most_missing_time
void HeightMatrix::AddFrameHeight(int *distance, int length)
{
	// The ratio is set according to the deployment of laser scanner.
	double ratio = 1.0;
	int sum = 0;

	for (int i = start_index_; i <= end_index_; ++i)
	{
		distance[i] = laser_height_ - distance[i] / 10 * 19 / 25;

		// Scanning points whose height is less than 1 meter is reset.
		if (distance[i] < 100)
		{
			distance[i] = 0;
		}
		sum = sum + distance[i];
	}

	// No objects detected
	if (sum == 0)
	{
		++zero_row_number_;
		if (zero_row_number_ >= most_missing_time_)
		{
			if (non_zero_row_number_ >= least_detect_time_)
			{
				// locate height map in this height matrix
				SearchHeightMap();

				// TODO: people counting and walking direction determination
				// Remember to check height of each height map before calculating
				// example:
				// if (height_maps[i].max_y() - height_maps[i].min_y()
				// >= least_detect_time)
				// leaving checking least_detect_time here has several reasons.
				// 1) if check in HeightMap construction, it seems useless to destroy
				// itself
				// 2) if check in SearchHeightMap before function returns, we may have
				// to count several times
				// 3) delete only when last calculating reduce repeated searching

				double var[9];

				for (int i = 0; i < height_maps_.size(); ++i)
				{
					var[0] = 1;
					var[1] = height_maps_[i].max_height();
					var[2] = height_maps_[i].points_number();
					var[3] = height_maps_[i].max_x() - height_maps_[i].min_x() + 1;
					var[4] = height_maps_[i].max_y() - height_maps_[i].min_y() + 1;
					var[5] = height_maps_[i].perimeter();
					var[6] = var[3] / var[4];
					var[7] = (4 * kPI * var[2]) / (var[5] * var[5]);
					var[8] = (var[5] * var[5]) / var[2];

					if (var[3] * var[4] >= 150)
					{
						//result_.people_number_ += Regression::CountPeopleNumber(var);
						try
						{
							height_maps_[i].DetermineWalkingDirections(
								Regression::CountPeopleNumber(var),
								1,
								result_
								);
						}
						catch (RegressResultException e)
						{
							throw;
						}
					}
				}
			}

			// cleanup work
			for (int i = 0; i < row_; ++i)
			{
				memset(matrix_[i], 0, column_ * sizeof(int));
			}
			
			zero_row_number_ = 0;
			non_zero_row_number_ = 0;
			height_maps_.clear();
		}
	}
	else
	{
		zero_row_number_ = 0;

		for (int i = 0; i < column_; ++i)
		{
			matrix_[non_zero_row_number_][i] = distance[i];
		}
		++non_zero_row_number_;
	}
}

// Searches through the height matrix to locate height maps. In order
// to speed up the search process, the step sizes in row and column are
// both larger than one. If a height is more than 1 meter, say, 101, first
// it is checked if it belongs to any of existing height map, if so, it is
// eliminated, if not, the loop stops and calls breadth first traverse process
// to find all points belonging to the new height map.
int HeightMatrix::SearchHeightMap()
{
	int row_step_size = 3;
	int column_step_size = 5;

	for (int i = 0;
		i <= non_zero_row_number_ - row_step_size;
		i += row_step_size)
	{
		for (int j = start_index_;
			j <= end_index_ - column_step_size;
			j += column_step_size)
		{
			if (matrix_[i][j] >= 100)
			{
				Coordinate c(i, j);
				bool is_point_already_included = false;

				// check if this point belongs to any other existing height map
				for (int k = 0; k < height_maps_.size(); ++k)
				{
					if (height_maps_[k].HasPoint(c))
					{
						is_point_already_included = true;
						break;
					}
				}

				// only searches from homeless points
				if (!is_point_already_included)
				{
					HeightMap hm = BreadthFirstSearch(c);

					height_maps_.push_back(hm);
				}
			}
		}
	}

	return height_maps_.size();
}

bool HeightMatrix::IsCoordinateInMatrix(Coordinate c)
{
	if (c.x >= 0 && c.x <= non_zero_row_number_)
	{
		if (c.y >= start_index_ && c.y <= end_index_)
		{
			return true;
		}
	}

	return false;
}

// Coordinate c is on edge if any of its four neighborers' height
// is zero, or the one of them is not in the matrix
bool HeightMatrix::IsCoordinateOnEdge(Coordinate c)
{
	if (IsCoordinateInMatrix(c))
	{
		Direction d[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

		for (int i = 0; i < 4; ++i)
		{
			Coordinate tmp;
			tmp.x = c.x + d[i].x;
			tmp.y = c.y + d[i].y;

			if (IsCoordinateInMatrix(tmp))
			{
				if (matrix_[tmp.x][tmp.y] == 0)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

// In this method, breadth first traverse method is employed to construct
// a height map, and in order to increase searching rate, the upper and
// right direction are preferred in the process of traverse.
// This function requires the caller to ensure that maxtrix_[c.x][c.y] > 100.
HeightMap HeightMatrix::BreadthFirstSearch(Coordinate c)
{
	HeightMap hm;
	Direction four_directions[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	for (int i = 0; i < row_; ++i)
	{
		memset(visited_[i], 0, column_ * sizeof(bool));
	}

	std::queue<Coordinate> q;

	if (!visited_[c.x][c.y])
	{
		q.push(c);
		//visited_[c.x][c.y] = true;
		
		Point p(c, IsCoordinateOnEdge(c), matrix_[c.x][c.y]);
		hm.AddNewPoints(p);
	}

	while (!q.empty())
	{
		Coordinate tmp = q.front();

		if (!visited_[tmp.x][tmp.y])
		{
			visited_[tmp.x][tmp.y] = true;

			Point p(tmp, IsCoordinateOnEdge(tmp), matrix_[tmp.x][tmp.y]);
			hm.AddNewPoints(p);

			for (int i = 0; i < 4; ++i)
			{
				Coordinate next;

				next.x = tmp.x + four_directions[i].x;
				next.y = tmp.y + four_directions[i].y;

				if (IsCoordinateInMatrix(next) && matrix_[next.x][next.y] >= 100)
				{
					q.push(next);
				}
			}
		}

		q.pop();
	}

	return hm;
}

void HeightMatrix::Result(CountingResult &r)
{
	r = result_;
}
