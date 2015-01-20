//=============================================================================
// This file is part of the Long Integer library
// Copyright (C) 2014 Borislav Karaivanov
//
// Long Integer library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Long Integer library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with the Long Integer library. If not, see 
// <http://www.gnu.org/licenses/>.
//=============================================================================

#ifdef LONGINT_EXPORTS
#define LONG_INT_DLL_API __declspec(dllexport) 
#else
#define LONG_INT_DLL_API __declspec(dllimport) 
#endif


#ifndef LONG_INT_NON_MEMBERS_HEADER
#define LONG_INT_NON_MEMBERS_HEADER

#include "longInt.h"

//=====================================================================
// Non-member non-friend functions of class LongInt.
//=====================================================================
namespace LongIntInVector
{
	// Check if all elements of given vector of itegers are digits.
	LONG_INT_DLL_API bool isVectOfDigits(const vector<int> & intVect);

	// Overload the unary operator-.
	LONG_INT_DLL_API LongInt operator-(LongInt longInteger);
	// Overload the absolute value function.
	LONG_INT_DLL_API LongInt abs(LongInt longInteger);

	// Overload comparison operators.
	LONG_INT_DLL_API bool operator==(const LongInt & lhs, const LongInt & rhs);
	inline bool operator!=(const LongInt & lhs, const LongInt & rhs) { return !operator==(lhs, rhs); }
	LONG_INT_DLL_API bool operator< (const LongInt & lhs, const LongInt & rhs);
	inline bool operator> (const LongInt & lhs, const LongInt & rhs) { return  operator< (rhs, lhs); }
	inline bool operator<=(const LongInt & lhs, const LongInt & rhs) { return !operator> (lhs, rhs); }
	inline bool operator>=(const LongInt & lhs, const LongInt & rhs) { return !operator< (lhs, rhs); }

	// Comparison in magnitude: less than (-1), equal to (0), or greater than (1).
	LONG_INT_DLL_API int lessEqualOrGreaterInMagnitude(const LongInt & lhs, const LongInt & rhs);
	// Comparison in magnitude.
	LONG_INT_DLL_API bool equalToInMagnitude(const LongInt & lhs, const LongInt & rhs);
	inline bool notEqualToInMagnitude(const LongInt & lhs, const LongInt & rhs) { return !equalToInMagnitude(lhs, rhs); }
	LONG_INT_DLL_API bool lessInMagnitude(const LongInt & lhs, const LongInt & rhs);
	inline bool greaterInMagnitude(const LongInt & lhs, const LongInt & rhs) { return  lessInMagnitude(rhs, lhs); }
	inline bool lessEqualInMagnitude(const LongInt & lhs, const LongInt & rhs) { return !greaterInMagnitude(lhs, rhs); }
	inline bool greaterEqualInMagnitude(const LongInt & lhs, const LongInt & rhs) { return !lessInMagnitude(lhs, rhs); }
	
	// Overload the binary operator+.
	inline LongInt operator+(LongInt lhs, const LongInt & rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// Overload the binary operator-.
	inline LongInt operator-(LongInt lhs, const LongInt & rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	// Overload the binary operator*.
	inline LongInt operator*(LongInt lhs, const LongInt & rhs)
	{
		lhs *= rhs;
		return lhs;
	}
	
	// Find the remainder (and quotient) of division of two long integers.
	LONG_INT_DLL_API LongInt longIntRemainder(const LongInt & numer, const LongInt & denom, LongInt & quotient);
	LONG_INT_DLL_API LongInt longIntRemainder(const LongInt & numer, const LongInt & denom);
	// Find the remainder of division of two long integers in Number theory sense, 
	// i.e., as an integer in [0, |denominator|) .
	LONG_INT_DLL_API LongInt longIntNonNegativeRemainder(const LongInt & numer, const LongInt & denom);

	// Overload the binary operator/.
	inline LongInt operator/(LongInt lhs, const LongInt & rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	// Overload the binary operator%.
	inline LongInt operator%(const LongInt & lhs, const LongInt & rhs)
	{
		return longIntRemainder(lhs, rhs);
	}

	// Unsigned integer power.
	LONG_INT_DLL_API LongInt pow(LongInt base, unsigned int exponent);

	// Overload the operator <<.
	LONG_INT_DLL_API std::ostream & operator<< (std::ostream & os, const LongInt & longInteger);

	// Overload the operator >>.
	LONG_INT_DLL_API std::istream & operator>> (std::istream & is, LongInt & longInteger);

	// Convert a string to a long integer.
	LONG_INT_DLL_API LongInt stringToLongInt(const string & str);
}

#endif  // LONG_INT_NON_MEMBERS_HEADER