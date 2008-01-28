//
// DynamicAny.cpp
//
// $Id: //poco/svn/Foundation/src/DynamicAny.cpp#3 $
//
// Library: Foundation
// Package: Core
// Module:  DynamicAny
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DynamicAny.h"
#include "Poco/DynamicStruct.h"
#include <algorithm>
#include <cctype>


namespace Poco {


DynamicAny::DynamicAny():
	_pHolder(new DynamicAnyHolderImpl<int>(0))
{
}


DynamicAny::DynamicAny(const char* pVal): 
	_pHolder(new DynamicAnyHolderImpl<std::string>(pVal))
{
}


DynamicAny::DynamicAny(const DynamicAny& other):
	_pHolder(0)
{
	if (other._pHolder)
		_pHolder = other._pHolder->clone();
}


DynamicAny::~DynamicAny()
{
	delete _pHolder;
}


DynamicAny& DynamicAny::operator = (const DynamicAny& other)
{
	DynamicAny tmp(other);
	swap(tmp);
	return *this;
}


const DynamicAny DynamicAny::operator + (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return add<Poco::Int64>(other);
		else
			return add<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return add<double>(other);
	else if (isString())
		return add<std::string>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator += (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = add<Poco::Int64>(other);
		else
			return *this = add<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = add<double>(other);
	else if (isString())
		return *this = add<std::string>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator - (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return subtract<Poco::Int64>(other);
		else
			return subtract<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return subtract<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator -= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = subtract<Poco::Int64>(other);
		else
			return *this = subtract<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = subtract<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator * (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return multiply<Poco::Int64>(other);
		else
			return multiply<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return multiply<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator *= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = multiply<Poco::Int64>(other);
		else
			return *this = multiply<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = multiply<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator / (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return divide<Poco::Int64>(other);
		else
			return divide<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return divide<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator /= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = divide<Poco::Int64>(other);
		else
			return *this = divide<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = divide<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator ++ ()
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	return *this = *this + 1;
}

DynamicAny DynamicAny::operator ++ (int)
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	DynamicAny tmp(*this);
	*this += 1;
	return tmp;
}

DynamicAny& DynamicAny::operator -- ()
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	return *this = *this - 1;
}

DynamicAny DynamicAny::operator -- (int)
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	DynamicAny tmp(*this);
	*this -= 1;
	return tmp;
}


DynamicAny& DynamicAny::operator [] (std::vector<DynamicAny>::size_type n)
{
	DynamicAnyHolderImpl<std::vector<DynamicAny> >* pHolder = dynamic_cast<DynamicAnyHolderImpl<std::vector<DynamicAny> > *>(_pHolder);
	if (pHolder)
		return pHolder->operator[](n);
	else
		throw BadCastException();
}


const DynamicAny& DynamicAny::operator [] (std::vector<DynamicAny>::size_type n) const
{
	const DynamicAnyHolderImpl<std::vector<DynamicAny> >* pHolder = dynamic_cast<const DynamicAnyHolderImpl<std::vector<DynamicAny> > *>(_pHolder);
	if (pHolder)
		return pHolder->operator[](n);
	else
		throw BadCastException();
}


DynamicAny& DynamicAny::operator [] (const std::string& name)
{
	DynamicAnyHolderImpl<DynamicStruct>* pHolder = dynamic_cast<DynamicAnyHolderImpl<DynamicStruct> *>(_pHolder);
	if (pHolder)
		return pHolder->operator[](name);
	else
		throw BadCastException();
}


const DynamicAny& DynamicAny::operator [] (const std::string& name) const
{
	const DynamicAnyHolderImpl<DynamicStruct>* pHolder = dynamic_cast<const DynamicAnyHolderImpl<DynamicStruct>* >(_pHolder);
	if (pHolder)
		return pHolder->operator[](name);
	else
		throw BadCastException();
}


DynamicAny DynamicAny::parse(const std::string& val)
{
	std::string::size_type t = 0;
	return parse(val, t);
}


DynamicAny DynamicAny::parse(const std::string& val, std::string::size_type& pos)
{
	// { -> an Object==DynamicStruct
	// [ -> an array
	// '/" -> a string (strip '/")
	// other: also treat as string
	skipWhiteSpace(val, pos);
	if (pos < val.size())
	{
		switch (val[pos])
		{
		case '{':
			return parseObject(val, pos);
		case '[':
			return parseArray(val, pos);
		default:
			return parseString(val, pos);
		}
	}
	std::string empty;
	return empty;
}


DynamicAny DynamicAny::parseObject(const std::string& val, std::string::size_type& pos)
{
	poco_assert_dbg (val[pos] == '{');
	++pos;
	skipWhiteSpace(val, pos);
	DynamicStruct aStruct;
	while (val[pos] != '}' && pos < val.size())
	{
		std::string key = parseString(val, pos);
		skipWhiteSpace(val, pos);
		if (val[pos] != ':')
			throw DataFormatException("Incorrect object, must contain: key : value pairs"); 
		++pos; // skip past :
		DynamicAny value = parse(val, pos);
		aStruct.insert(key, value);
		skipWhiteSpace(val, pos);
		if (val[pos] == ',')
		{
			++pos;
			skipWhiteSpace(val, pos);
		}
	}
	if (val[pos] != '}')
		throw DataFormatException("Unterminated object"); 
	++pos;
	return aStruct;
}


DynamicAny DynamicAny::parseArray(const std::string& val, std::string::size_type& pos)
{
	poco_assert_dbg (val[pos] == '[');
	++pos;
	skipWhiteSpace(val, pos);
	std::vector<DynamicAny> result;
	while (val[pos] != ']' && pos < val.size())
	{
		result.push_back(parse(val, pos));
		skipWhiteSpace(val, pos);
		if (val[pos] == ',')
		{
			++pos;
			skipWhiteSpace(val, pos);
		}
	}
	if (val[pos] != ']')
		throw DataFormatException("Unterminated array"); 
	++pos;
	return result;
}


std::string DynamicAny::parseString(const std::string& val, std::string::size_type& pos)
{
	static const std::string STR_STOP("'\"");
	static const std::string OTHER_STOP(" ,]}"); // we stop at space, ',', ']' or '}'

	bool inString = false;
	//skip optional ' "
	if (val[pos] == '\'' || val[pos] == '"')
	{
		inString = true;
		++pos;
	}
	
	std::string::size_type stop = std::string::npos;
	if (inString)
	{
		stop = val.find_first_of(STR_STOP, pos);
		if (stop == std::string::npos)
			throw DataFormatException("Unterminated string");
	}
	else
	{
		// we stop at space, ',', ']' or '}' or end of string
		stop = val.find_first_of(OTHER_STOP, pos);
		if (stop == std::string::npos)
			stop = val.size();

		std::string::size_type safeCheck = val.find_first_of(STR_STOP, pos);
		if (safeCheck != std::string::npos && safeCheck < stop)
			throw DataFormatException("Misplaced string termination char found");

	}

	// stop now points to the last char to be not included
	std::string result = val.substr(pos, stop - pos);
	++stop; // point past '/"
	pos = stop;
	return result;
}


void DynamicAny::skipWhiteSpace(const std::string& val, std::string::size_type& pos)
{
	while (std::isspace(val[pos]))
		++pos;
}


std::string DynamicAny::toString(const DynamicAny& any)
{
	std::string res;
	appendJSONString(res, any);
	return res;
}


} // namespace Poco::Poco
