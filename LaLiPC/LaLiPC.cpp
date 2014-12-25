#include <iostream>
#include <fstream>

#include "ParamLoader.h"
#include "HeightMatrix.h"
#include "CountingResult.h"
#include "SliderSizeZeroException.h"

using namespace std;

int main()
{
	string file_name;
	fstream file_reader;

	// counting results
	int in_people_number;
	int out_people_number;
	int undefined_people_number;
	int frame_id;

	// parameters
	int start_index;
	int end_index;
	int laser_height;
	int least_detect_time;
	int most_missing_time;

	cout << "Enter parameter file name: ";
	cin >> file_name;

	ParamLoader loader(file_name);

	if (loader.Load(start_index, end_index, laser_height, least_detect_time,
		most_missing_time))
	{
		in_people_number = 0;
		out_people_number = 0;
		undefined_people_number = 0;
		frame_id = 0;

		cout << "Enter distance data file name: ";
		cin >> file_name;

		file_reader.open(file_name, std::ios_base::in);
		
		string plane;
		int plane_id;
		char colon;
		const int kFrameLength = 274;
		int frame_distance_holder[kFrameLength] = {0};
		HeightMatrix height_matrix( 200,
									kFrameLength,
									least_detect_time,
									most_missing_time,
									start_index,
									end_index,
									laser_height
									);

		CountingResult result;

		while (file_reader.good())
		{
			file_reader >> plane >> plane_id >> colon;

			for (int i = 0; i < kFrameLength; ++i)
			{
				file_reader >> frame_distance_holder[i];
			}

			if (plane_id == 0)
			{
				++frame_id;

				cout << "Processing frame " << frame_id << endl;
				
				try
				{
					height_matrix.AddFrameHeight(frame_distance_holder,
						kFrameLength);
				}
				catch (RegressResultException e)
				{
					cout << e.show_reason() << endl;
				}
				catch (SliderSizeZeroException e)
				{
					cout << e.show_reason() << endl;
					//break;
				}				

				height_matrix.Result(result);
				//cout << result;
				//cout << "people number: " << result.people_number_ << endl;
				cout << "people in: " << result.people_in_number_
					<< " people out: " << result.people_out_number_
					<< " people unknown: " << result.people_unknown_number_
					<< endl;
			}
		}

		file_reader.close();
	}
	else
	{
		cout << "Load parameters failed" << endl;
	}

	return 0;
}
