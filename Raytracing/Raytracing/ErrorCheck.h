#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

enum class RESULT
{
	RES_OK		= 0,
	RES_FAIL	= 1
};

// Check fails if e is false;
#define CHECK_ERROR(e, msg) \
if (!(e)) \
{std::cout << "[ERROR] " << msg << " " << __FUNCTION__ << ":" << __LINE__ << std::endl;}

#endif