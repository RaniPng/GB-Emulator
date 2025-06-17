#pragma once

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long int int64;

union Register
{
	struct {
		int8 low;
		int8 high;
	};
	int16 full;
};