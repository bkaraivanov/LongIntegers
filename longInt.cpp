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
#include <cassert>    // assert
#include <sstream>    // ostringstream
#include <limits>     // numeric_limits
#include <algorithm>  // max

using namespace LongIntInVector;

// Constructor.
LongIntInVector::LongInt::LongInt(int num)
{
	// Set the sign and absolute value.
	if (num > 0)
		sign = plus;
	else if (num == 0)
		sign = zero;
	else 
	{
		sign = minus;
		num = -num;
	}

	// Fill in the vector with the digits in reverse order.
	do
	{
		int currDigit = num % 10;
		intVect.push_back(currDigit);

		num -= currDigit;
		num /= 10;
	}
	while (num > 0);
}


// Constructor.
LongIntInVector::LongInt::LongInt(const string & str)
{
	*this = LongIntInVector::stringToLongInt(str);
}


// Constructor.
LongIntInVector::LongInt::LongInt(const vector<int> & inIntVect) : intVect(inIntVect)
{
	// Check if all given integers are digits.
	if ( (intVect.size() == 0)||(isVectOfDigits(intVect) == false) )
	{
		(*this).makeNaN();
		return;
	}

	if ( (intVect.size() == 1)&&(intVect[0] == 0) )
		setSign(zero);
	else
		setSign(plus);
}


// Constructor.
LongIntInVector::LongInt::LongInt(const vector<int> & inIntVect, Signs inSign) : intVect(inIntVect), sign(inSign) 
{
	// Check if all given integers are digits.
	if (isVectOfDigits(intVect) == false)
		(*this).makeNaN();
}


LongInt & LongIntInVector::LongInt::swap(LongInt & rhs)
{
	std::swap(this->intVect, rhs.intVect);
	std::swap(this->sign, rhs.sign);
	return *this;
}


LongInt & LongIntInVector::LongInt::operator=(LongInt rhs)
{
	this->swap(rhs);
	return *this;
}


// Overload the unary prefix increment operator++.
LongInt & LongIntInVector::LongInt::operator++()
{
	*this += LongInt(1);
	return *this;
}	


// Overload the unary postfix increment operator++.
LongInt LongIntInVector::LongInt::operator++(int)
{
	LongInt tmp(*this);
	operator++();
	return tmp;
}


// Overload the unary prefix decrement operator--.
LongInt & LongIntInVector::LongInt::operator--()
{
	*this -= LongInt(1);
	return *this;
}	


// Overload the unary postfix decrement operator--.
LongInt LongIntInVector::LongInt::operator--(int)
{
	LongInt tmp(*this);
	operator--();
	return tmp;
}


// Convert a long integer vector to a string of digits.
const string LongIntInVector::LongInt::longIntToString() const
{
	std::ostringstream ostr;
	if (isNaN() == true)
		ostr << "NaN";
	else
	{
		if (sign == minus)
			ostr << '-';
		for (vector<int>::const_reverse_iterator crit = intVect.rbegin(); crit < intVect.rend(); ++crit)
			ostr << *crit;
	}
	return ostr.str();
} 


// Increment the magnitude of a long integer by the magnitude of another
// long integer while preserving the sign of the former. 
LongInt & LongIntInVector::LongInt::incrementMagnitude(const LongInt & rhs)
{
	int carry = 0;
	for (int k = 0; k < max(getNumDigits(), rhs.getNumDigits()); ++k)
	{
		int sum = getNthLeastSignifDigit(k) + rhs.getNthLeastSignifDigit(k) + carry;
		if (sum > 9) 
		{
			setNthLeastSignifDigit(k, sum - 10);
			carry = 1;
		}
		else
		{
			setNthLeastSignifDigit(k, sum);
			carry = 0;
		}
	}
	if (carry == 1)
		setNthLeastSignifDigit(getNumDigits(), carry);

	return *this;
}


// Decrement the magnitude of a long integer by the magnitude of another
// long integer while assuming the former is not smaller than the latter,
// and preserving the sign of the former. 
LongInt & LongIntInVector::LongInt::decrementMagnitude(const LongInt & rhs)
{
	int borrow = 0;
	for (int k = 0; k < max(getNumDigits(), rhs.getNumDigits()); ++k)
	{
		int diff = getNthLeastSignifDigit(k) - rhs.getNthLeastSignifDigit(k) - borrow;
		if (diff < 0) 
		{
			setNthLeastSignifDigit(k, diff + 10);
			borrow = 1;
		}
		else
		{
			setNthLeastSignifDigit(k, diff);
			borrow = 0;
		}
	}
	// Remove any leading zeros.
	eraseLeadingZeros();

	return *this;
}


// Remove any leading zeros from the vector representing the magnitude of
// the long integer.
void LongIntInVector::LongInt::eraseLeadingZeros()
{
	if (isNaN() == true) 
		return;
	std::vector<int>::iterator it = intVect.end();
	do
	--it;
	while ( (*it == 0)&&(it != intVect.begin()) );
	// Erase from the next digit to the end.
	intVect.erase((it + 1), intVect.end());
}


// Overload operator +=.
LongInt & LongIntInVector::LongInt::operator+= (const LongInt & rhs)
{
	// Handle the cases when one of the two summands is zero or NaN.
	if ( (isNaN() == true)||(rhs.getSign() == zero) )
	{}
	else if (rhs.isNaN() == true)
		makeNaN();
	else if (getSign() == zero)
		*this = rhs;
	// Handle the cases when the two summands are non-zeros of the same sign.
	else if (getSign() == rhs.getSign())
		*this = (*this).incrementMagnitude(rhs);
	//=================================================================
	// If this point is reached, then the two summands are non-zeros of
	// different signs.
	//=================================================================
	// Handle the case when the two summands are non-zeros of different
	// sign and the first is greater in magnitude.
	else if (greaterInMagnitude(*this, rhs))
		*this = (*this).decrementMagnitude(rhs);
	// Handle the case when the two summands are non-zeros of the same
	// magnitude but different signs.
	else if (equalToInMagnitude(*this, rhs))
		*this = LongInt(0);
	// Handle the case when the two summands are non-zeros of different
	// sign and the second is greater in magnitude.
	else if (lessInMagnitude(*this, rhs))
	{
		LongInt temp(rhs);
		*this = temp.decrementMagnitude(*this);
		(*this).setSign(rhs.getSign());
		return *this;
	}

	return *this;
}


// Overload operator -=.
LongInt & LongIntInVector::LongInt::operator-= (const LongInt & rhs)
{
	*this += (-rhs);
	return *this;
}


// Overload operator *=.
LongInt & LongIntInVector::LongInt::operator*= (const LongInt & rhs)
{
	// Handle the cases when one of the two factors is zero or NaN.
	if ( (isNaN() == true)||(getSign() == zero) )
	{}
	else if (rhs.isNaN() == true)
		makeNaN();
	else if (rhs.getSign() == zero)
		*this = LongInt(0);
	// Handle the cases when the both factors are non-zeros.
	else 
	{
		// Set the sign of the product. Note that at this point neither
		// factor has sign of zero.
		Signs prodSign = plus;
		if (getSign() != rhs.getSign())
			prodSign = minus;

		// Multiply the magnitudes.
		LongInt total(0);
		int rhsNumDigits = rhs.getNumDigits();
		for (int n = 0; n < getNumDigits(); ++n)
		{
			int nthSmallestDigit = getNthLeastSignifDigit(n);
			if (nthSmallestDigit == 0)
				continue;
			LongInt temp(0);
			//temp.setSign(prodSign);
			for (int m = 1; m < n; ++m)
				temp.setNthLeastSignifDigit(m, 0);
			int carry = 0;
			for (int m = 0; m < rhsNumDigits; ++m)
			{
				int prod = nthSmallestDigit * rhs.getNthLeastSignifDigit(m) + carry;
				temp.setNthLeastSignifDigit(n + m, prod % 10);
				carry = prod / 10;
			}
			if (carry > 0)
				temp.setNthLeastSignifDigit(temp.getNumDigits(), carry);
			temp.finalize();
			total += temp;
		}
		total.setSign(prodSign);
		*this = total;
	}

	return *this;
}


// Overload operator /=.
LongInt & LongIntInVector::LongInt::operator/= (const LongInt & rhs)
{
	longIntRemainder(*this, rhs, *this);
	return *this;
}


// Overload operator %=.
LongInt & LongIntInVector::LongInt::operator%= (const LongInt & rhs)
{
	*this = longIntRemainder(*this, rhs);
	return *this;
}


// Set the sign.
void LongIntInVector::LongInt::setSign(Signs inSign) 
{
	//if (isNaN() == true)
	//	return;
	sign = inSign;
	assert((sign != zero)||( (getNumDigits() == 1)&&(getNthLeastSignifDigit(0) == 0) ));
	assert((sign == zero)||(getNumDigits() > 1)||( (getNumDigits() == 1)&&(getNthLeastSignifDigit(0) != 0) ));
}


// Set the n-th smallest digit, i.e., the one corresponding to 10^n.
void LongIntInVector::LongInt::setNthLeastSignifDigit(int n, int digit) 
{ 
	assert(0 <= digit);
	assert(digit <= 9);

	if ( (n >= 0)&&(n < getNumDigits()) ) 
		intVect[n] = digit;
	else if (n == getNumDigits())
		intVect.push_back(digit);

	return;
} 

// Make sure the long integer has complying representation: no leading zeros,
// zero sign for zero magnitude, and non-zero sign for non-zero magnitude.
void LongIntInVector::LongInt::finalize() 
{ 
	eraseLeadingZeros();
	// Handle the possibility that a non-zero was made zero.
	if ( (intVect.size() == 1)&&(intVect[0] == 0) )
		setSign(zero);
	// Handle the possibility that a zero was made non-zero.
	else if (getSign() == zero)
		setSign(plus);
	return;
} 


// Multiply by 10.
void LongIntInVector::LongInt::multiplyBy10() 
{
	if ( (isNaN() == true)||(*this != LongInt(0)) )
		intVect.insert(intVect.begin(), 0);  
}


// Convert long integer to integer.
int LongIntInVector::LongInt::getInteger() const
{
	assert(isNaN() == false);
	//assert(lessEqualInMagnitude(*this, LongInt(std::numeric_limits<int>::max())));

	int numb(0);
	for (int k = 0; k < getNumDigits(); ++k)
	{
		numb *= 10;
		numb += getNthMostSignifDigit(k);
	}
	return numb * getSign();
}