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

#include "stdafx.h"
#include "longIntNonMembers.h"
#include <cassert>   // assert

using namespace LongIntInVector;


// Check if all elements of given vector of itegers are digits.
bool LongIntInVector::isVectOfDigits(const vector<int> & intVect)
{
	for (vector<int>::const_iterator cit = intVect.begin(); cit != intVect.end(); ++cit)
		if ( (*cit < 0)||(*cit > 9) )
			return false;

	return true;
}


// Overload unary operator-.
LongInt LongIntInVector::operator-(LongInt longInteger)
{
	if (longInteger.isNaN() == true)
	{}
	else if (longInteger.getSign() == plus)
		longInteger.setSign(minus);
	else if (longInteger.getSign() == minus)
		longInteger.setSign(plus);

	return longInteger;
}


// Overload the absolute value function.
LongInt LongIntInVector::abs(LongInt longInteger) 
{ 
	if ( (longInteger.isNaN() == false)&&(longInteger.getSign() == minus) )
		longInteger.setSign(plus); 
	return longInteger; 
}


// Overload comparison operator==.
bool LongIntInVector::operator==(const LongInt & lhs, const LongInt & rhs)
{ 
	assert(lhs.isNaN() == false);
	assert(rhs.isNaN() == false);

	if (lhs.getSign() != rhs.getSign())
		return false;
	return equalToInMagnitude(lhs, rhs);
}


// Overload comparison operator<.
bool LongIntInVector::operator< (const LongInt & lhs, const LongInt & rhs)
{ 
	assert(lhs.isNaN() == false);
	assert(rhs.isNaN() == false);

	// Handle all cases of different or zero signs.
	if (lhs.getSign() < rhs.getSign())
		return true;
	if ( (lhs.getSign() > rhs.getSign())||(lhs.getSign() == zero) )
		return false;

	// Handle the cases of same, non-zero signs. The value of the indicator is:
	// -1 for strictly less, 0 for equal to, and 1 for strictly greater.
	int indicator = lessEqualOrGreaterInMagnitude(lhs, rhs);
	if (indicator == 0)
		return false;
	else
		return ( (lhs.getSign() == plus) == (indicator == -1) );
}


// Comparison in magnitude: less than (-1), equal to (0), or greater than (1).
int LongIntInVector::lessEqualOrGreaterInMagnitude(const LongInt & lhs, const LongInt & rhs)
{ 
	assert(lhs.isNaN() == false);
	assert(rhs.isNaN() == false);

	// Handle the cases of different number of digits.
	if (lhs.getNumDigits() < rhs.getNumDigits())
		return -1;
	if (lhs.getNumDigits() > rhs.getNumDigits())
		return 1;

	// Handle the cases of same number of digits.
	for (int k = (lhs.getNumDigits() - 1) ; k >= 0; --k)
	{
		if (lhs.getNthLeastSignifDigit(k) < rhs.getNthLeastSignifDigit(k))
			return -1;
		if (lhs.getNthLeastSignifDigit(k) > rhs.getNthLeastSignifDigit(k))
			return 1;
	}

	// Handle the case when the two long integers are of the same magnitude.
	return 0;
}


// Comparison in magnitude: equal to.
bool LongIntInVector::equalToInMagnitude(const LongInt & lhs, const LongInt & rhs)
{ 
	if (lhs.getNumDigits() != rhs.getNumDigits())
		return false;
	for (int k = 0; k < lhs.getNumDigits() ; ++k)
		if (lhs.getNthLeastSignifDigit(k) != rhs.getNthLeastSignifDigit(k))
			return false;

	return true;
}


// Comparison in magnitude: less than.
bool LongIntInVector::lessInMagnitude(const LongInt & lhs, const LongInt & rhs)
{
	return (lessEqualOrGreaterInMagnitude(lhs, rhs) == -1);
}


// Overload the operator <<.
std::ostream & LongIntInVector::operator<< (std::ostream & os, const LongInt & longInteger)
{
	return os << longInteger.longIntToString();
}


// Get and remove (if any) the sign from a string representing an integer.
Signs getAndRemoveSignFromString(string & str);


// Overload the operator >>.
std::istream & LongIntInVector::operator>> (std::istream & is, LongInt & longInteger)
{
	// Read a string from the stream.
	string readIntStr;
	is >> readIntStr;

	longInteger = stringToLongInt(readIntStr);
	if (longInteger.isNaN() == true)
		is.setstate(std::ios::failbit);
	//assert(longInteger.isNaN() == false);

	return is;
}


// Get and remove (if any) the sign from a string representing an integer.
Signs getAndRemoveSignFromString(string & str)
{
	Signs stringSign = zero;

	if (str.empty() == true)
		return stringSign;

	switch (str[0])
	{
	case '-':
		{
			// Remove the negative sign.
			str.erase(0, 1);
			if (str.empty() == true)
				break;
			if (str[0] != '0')
				stringSign = minus;
			else
				stringSign = zero;
			break;
		}

	case '+':
		{
			// Remove the positive sign.
			str.erase(0, 1);
			if (str.empty() == true)
				break;
			if (str[0] != '0')
				stringSign = plus;
			else
				stringSign = zero;
			break;
		}

	case '0':
		{
			stringSign = zero;
			break;
		}

	default:
		stringSign = plus;
	}

	return stringSign;
}


// Convert a string to a long integer.
LongInt LongIntInVector::stringToLongInt(const string & inStr)
{
	LongInt longInteger;

	// If the read string is empty, then return immediately.
	if (inStr.empty() == true)
		return longInteger;

	// Pick up a negative sign if present.
	string str(inStr);
	Signs stringSign = getAndRemoveSignFromString(str);

	// Check if the string is made up entirely of digits, is non-empty,
	// and does not start with 0 unless it holds a single digit.
	std::size_t found = str.find_first_not_of("0123456789");
	if ( !( (found != std::string::npos)||(str.empty() == true)||
		((str.size() > 1)&&(str[0] == '0')) ) )
	{
		// Transfer the read string to a long integer.
		int k = 0;
		for (string::const_reverse_iterator crit = str.crbegin(); crit != str.crend() ; ++crit)
			longInteger.setNthLeastSignifDigit(k++, *crit - '0');
		longInteger.setSign(stringSign);
	}

	return longInteger;
}


// Find the quotient and remainder of division of two long integers.
// Denominator is passed by value since we need to use its absolute value and
// thus we engage the compiler to make a copy of it whose sign we can change.
LongInt LongIntInVector::longIntRemainder(const LongInt & numer, const LongInt & denom, LongInt & quotient)
{
	// Handle the cases when one of the two terms is zero or NaN, or the
	// numerator is smaller than the denominator in magnitude.
	if ( (numer.isNaN() == true)||(denom.isNaN() == true)||(denom.getSign() == zero) )
	{
		quotient.makeNaN();
		return LongInt();
	}
	else if ( (numer.getSign() == zero)||(lessInMagnitude(numer, denom)) ) 
	{
		quotient = LongInt(0);
		return numer;
	}
	// Handle the cases when the both terms are non-zeros and the numerator is
	// not smaller than the denominator in magnitude.
	else
	{
		// Set the sign of the fraction. Note that at this point neither
		// numerator nor denominator has sign of zero.
		Signs divSign = plus;
		if (numer.getSign() != denom.getSign())
			divSign = minus;

		// Divide the magnitudes.
		int smallestUsedDigitInd = numer.getNumDigits() - denom.getNumDigits() + 1;
		LongInt temp;  
		for (int k = 0, m = smallestUsedDigitInd; m < numer.getNumDigits(); ++k, ++m)
			temp.setNthLeastSignifDigit(k, numer.getNthLeastSignifDigit(m));
		//temp.finalize();
		int nextDigitInd = smallestUsedDigitInd - 1;
		LongInt absDenom = abs(denom);
		vector<int> quotientVect;
		while (nextDigitInd >= 0)
		{
			// Pull the next digit from the numerator.
			temp.multiplyBy10();
			temp.setNthLeastSignifDigit(0, numer.getNthLeastSignifDigit(nextDigitInd));
			temp.finalize();
			--nextDigitInd;

			int q = 0;
			while (temp >= absDenom)
			{
				temp -= absDenom;
				++q;
			}
			// Store the current digit of the quotient.
			quotientVect.push_back(q);
		}

		// Reverse the digits since more significant digits have been stored
		// first while they need to be last.
		std::reverse(quotientVect.begin(), quotientVect.end());
		// Remove a possible leading zero. It may occur if the "denomNumDigits"
		// most significant digits of the numerator gave a number smaller that
		// the denominator.
		vector<int>::iterator lastDigitIter = quotientVect.end() - 1;
		if (*(lastDigitIter) == 0)
			quotientVect.erase(lastDigitIter);
		// Construct the long integer quotient.
		quotient = LongInt(quotientVect, divSign);
		if (temp != LongInt(0))
			temp.setSign(numer.getSign());
		return temp;
	}
}


// Find the remainder (without the quotient) of division of two long integers.
LongInt LongIntInVector::longIntRemainder(const LongInt & numer, const LongInt & denom)
{
	// Handle the cases when one of the two terms is zero or NaN, or the
	// numerator is smaller than the denominator in magnitude.
	if ( (numer.isNaN() == true)||(denom.isNaN() == true)||(denom.getSign() == zero) )
	{
		return LongInt();
	}
	else if ( (numer.getSign() == zero)||(lessInMagnitude(numer, denom)) ) 
	{
		return numer;
	}
	// Handle the cases when the both terms are non-zeros and the numerator is
	// not smaller than the denominator in magnitude.
	else
	{
		int smallestUsedDigitInd = numer.getNumDigits() - denom.getNumDigits() + 1;
		LongInt temp;
		for (int k = 0, m = smallestUsedDigitInd; m < numer.getNumDigits(); ++k, ++m)
			temp.setNthLeastSignifDigit(k, numer.getNthLeastSignifDigit(m));
		int nextDigitInd = smallestUsedDigitInd - 1;
		LongInt absDenom = abs(denom);
		while (nextDigitInd >= 0)
		{
			// Pull the next digit from the numerator.
			temp.multiplyBy10();
			temp.setNthLeastSignifDigit(0, numer.getNthLeastSignifDigit(nextDigitInd));
			temp.finalize();
			--nextDigitInd;

			int q = 0;
			while (temp >= absDenom)
			{
				temp -= absDenom;
				++q;
			}
		}
		if (temp != LongInt(0))
			temp.setSign(numer.getSign());
		return temp;
	}
}


// Find the remainder of division of two long integers in Number theory sense, 
// i.e., as an integer in [0, |divisor|) .
LongInt LongIntInVector::longIntNonNegativeRemainder(const LongInt & numer, const LongInt & denom)
{
	LongInt remainder = longIntRemainder(numer, denom);
	if ( (remainder.isNaN() == false)&&(remainder < LongInt(0)) )
		remainder += abs(denom);
	return remainder;
}


// Unsigned integer power.
LongInt LongIntInVector::pow(LongInt base, unsigned int exponent)
{
	assert(exponent >= 0);

	LongInt lhs(1);
	LongInt baseToCurrPowerOf2(base);
	if ((exponent % 2) == 1)
		lhs *= baseToCurrPowerOf2;
	exponent /= 2;
	while (exponent != 0)
	{
		baseToCurrPowerOf2 *= baseToCurrPowerOf2;
		if ((exponent % 2) == 1)
			lhs *= baseToCurrPowerOf2;
		exponent /= 2;
	}
	return lhs;
}