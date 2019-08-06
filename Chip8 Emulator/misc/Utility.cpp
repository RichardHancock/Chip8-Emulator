#include "Utility.h"

#include <cstdlib>

float Utility::normaliseBetweenMinusOneAndOne(float numberToNormalise, float max, float min)
{
	//find the new max and min number when shifted to a positive only range and the shifted number to normalise
	max += std::abs(min);
	numberToNormalise += std::abs(min);
	min = 0.0f;

	//normalise the number between zero and one
	float normalisedNumber = normaliseFloat(numberToNormalise, max, min);

	//shift it to between -0.5f and 0.5f
	normalisedNumber -= 0.5f;

	//scale it to between -1.0f and 1.0f
	normalisedNumber *= 2.0f;

	//return the normalised number
	return normalisedNumber;
}


float Utility::normaliseFloat(float numberToNormalise, float max, float min)
{
	//normalise the number between zero and one
	float normalisedNumber = (numberToNormalise - min) / (max - min);

	//return the normalised number
	return normalisedNumber;
}