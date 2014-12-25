#ifndef PARAM_LOADER_H_
#define PARAM_LOADER_H_

#include <fstream>
#include <map>
#include <list>
#include <string>

// Loads parameters from a specific configuration file for program uses.
// After an object is constructed, the caller uses Load function to assign
// five variables using corresponding values in configuration file.
class ParamLoader
{
public:
	ParamLoader(std::string file_name);
	// Returns true if five parameters are loaded successfully.
	// param1: start index of frame
	// param2: end index of frame
	// param3: height of laser height
	// param4: least detection time of a height map
	// param5: most missing time of a height map
	bool Load(int &param1, int &param2, int &param3, int &param4, int &param5);

private:
	// Returns true if five parameters are loaded successfully
	// name_value: results stored in STL map
	bool LoadParam(std::map<std::string, int> &name_value);

private:
	std::string file_name_;
};

#endif
