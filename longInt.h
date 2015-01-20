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


#ifndef LONG_INT_HEADER
#define LONG_INT_HEADER

#include <vector>     // vector
#include <string>     // string


namespace LongIntInVector
{
	using std::vector;
	using std::string;

	enum Signs {minus = -1, zero = 0, plus = 1};

	class LongInt
	{
	public:
		// Constructors.
		LongInt() : sign(plus) {}
		LONG_INT_DLL_API explicit LongInt(int num);
		LONG_INT_DLL_API explicit LongInt(const string & str);
		LONG_INT_DLL_API explicit LongInt(const vector<int> & inIntVect);
		LONG_INT_DLL_API LongInt(const vector<int> & inIntVect, Signs inSign);
		~LongInt() {}

		// Swap member function.
		LONG_INT_DLL_API LongInt & swap(LongInt & rhs);

		// Convert a long integer vector to a string of digits.
		LONG_INT_DLL_API const string LongInt::longIntToString() const;

		// Overload operator =.
		LONG_INT_DLL_API LongInt & operator=(LongInt rhs);

		// Overload the unary prefix and postfix increment and decrement
		// operators.
		LONG_INT_DLL_API LongInt & operator++();
		LONG_INT_DLL_API LongInt operator++(int);
		LONG_INT_DLL_API LongInt & operator--();
		LONG_INT_DLL_API LongInt operator--(int);

		// Overload operator +=.
		LONG_INT_DLL_API LongInt & operator+= (const LongInt & rhs);
		// Overload operator -=.
		LONG_INT_DLL_API LongInt & operator-= (const LongInt & rhs);
		// Overload operator *=.
		LONG_INT_DLL_API LongInt & operator*= (const LongInt & rhs);
		// Overload operator /=.
		LONG_INT_DLL_API LongInt & operator/= (const LongInt & rhs);
		// Overload operator %=.
		LONG_INT_DLL_API LongInt & operator%= (const LongInt & rhs);

		// Accessors.
		// Get or set the n-th smallest digit, i.e., the one corresponding to 10^n.
		int getNthLeastSignifDigit(int n) const { return ( (n >= 0)&&(n < getNumDigits()) ) ? intVect[n] : 0; }  
		LONG_INT_DLL_API void setNthLeastSignifDigit(int n, int digit); 
		// Get or set the n-th largest digit, i.e., the one corresponding to 10^(N-n-1) where N is the number of digits.
		int getNthMostSignifDigit(int n) const { return getNthLeastSignifDigit(getNumDigits() - 1 - n); } 
		void setNthMostSignifDigit(int n, int digit) { setNthLeastSignifDigit(getNumDigits() - 1 - n, digit); }
		// Get or set the sign.
		Signs getSign() const { return sign; }
		LONG_INT_DLL_API void setSign(Signs inSign);
		// Make it NaN (not-a-number).
		void makeNaN() { intVect.clear(); }
		bool isNaN() const { return (getNumDigits() == 0); }
		// Make sure the long integer has complying representation: no leading zeros,
		// zero sign for zero magnitude, and non-zero sign for non-zero magnitude.
		LONG_INT_DLL_API void finalize();

		// Get the number of digits.
		int getNumDigits() const { return static_cast<int>(intVect.size()); }
		// Multiply by 10.
		LONG_INT_DLL_API void multiplyBy10();

		// Convert long integer to integer.
		LONG_INT_DLL_API int getInteger() const;

		/* template <class T>
		T getNumberOfType()
		{
			T numb(0);

			for (int k = 0; k < getNumDigits(); ++k)
			{
				numb *= 10;
				numb += getNthMostSignifDigit(k);
			}

			return numb;
		} */

	private:
		// The least significant digits stored first.
		vector<int> intVect;
		Signs sign;
		
		// Increment the object by another long integer object while assuming 
		// they both are non-negative.
		LONG_INT_DLL_API LongInt & incrementMagnitude(const LongInt & rhs);
		// Decrement the object by another long integer object while assuming 
		// they both are non-negative and the former is greater than the later.
		LONG_INT_DLL_API LongInt & decrementMagnitude(const LongInt & rhs);
		// Remove any leading zeros.
		LONG_INT_DLL_API void eraseLeadingZeros();
	};
}

#endif  // LONG_INT_HEADER