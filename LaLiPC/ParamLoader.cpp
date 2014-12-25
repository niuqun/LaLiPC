#include "ParamLoader.h"

ParamLoader::ParamLoader(std::string file_name)
{
	file_name_ = file_name;
}

bool ParamLoader::Load(int &param1, int &param2, int &param3, int &param4,
						int &param5)
{
	std::map<std::string, int> parameters;
	bool is_load_successful = false;
	std::list<std::string> param_names;

	param_names.push_back("start_index");
	param_names.push_back("end_index");
	param_names.push_back("laser_height");
	param_names.push_back("least_detect_time");
	param_names.push_back("most_missing_time");
	
	if ((is_load_successful = LoadParam(parameters)) == true)
	{
		for (std::list<std::string>::iterator it = param_names.begin();
			it != param_names.end();
			++it)
		{
			if (parameters.find(*it) == parameters.end())
			{
				is_load_successful = false;
				break;
			}
		}

		if (is_load_successful)
		{
			param1 = parameters["start_index"];
			param2 = parameters["end_index"];
			param3 = parameters["laser_height"];
			param4 = parameters["least_detect_time"];
			param5 = parameters["most_missing_time"];
		}
	}

	return is_load_successful;
}

bool ParamLoader::LoadParam(std::map<std::string, int> &name_value)
{
	bool is_load_successful = false;
	std::fstream file_reader(file_name_, std::ios::in);
	std::string param_name;
	int param_value;

	if (file_reader.good())
	{
		while (file_reader.good())
		{
			file_reader >> param_name >> param_value;

			name_value.insert(
				std::make_pair<std::string, int>(param_name, param_value)
				);
		}

		is_load_successful = true;
	}

	file_reader.close();

	return is_load_successful;
}
