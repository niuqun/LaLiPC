#ifndef ERROR_CODE_
#define ERROR_CODE_

// This enum structure represents the error code to be returned
// by HeightMap::AddNewPoint(const Point &p), which is the result of
// insert operation
enum ErrorCode
{
	InsertSuccessful,
	AlreadyExist,
	InsertFailed
};

#endif
