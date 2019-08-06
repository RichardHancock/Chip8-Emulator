#pragma once

namespace Utility
{
	///A function to normalise a number to between 1.0f and -1.0f, using the range that the number already uses
	float normaliseBetweenMinusOneAndOne(float numberToNormalise, float max, float min);


	///A function to normalise a number to between 0 and 1, using the range that the number already exists in
	float normaliseFloat(float numberToNormalise, float max, float min);
}