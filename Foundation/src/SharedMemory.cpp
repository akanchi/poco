//
// SharedMemory.cpp
//
// $Id: //poco/svn/Foundation/src/SharedMemory.cpp#2 $
//
// Library: Foundation
// Package: Processes
// Module:  SharedMemory
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


#if POCO_OS == POCO_OS_SOLARIS
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif


#include "Poco/SharedMemory.h"
#include "Poco/Exception.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "SharedMemory_WIN32.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "SharedMemory_POSIX.cpp"
#else
#include "SharedMemory_DUMMY.cpp"
#endif


namespace Poco {


SharedMemory::SharedMemory():
	_pImpl(0)
{
}


SharedMemory::SharedMemory(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint, bool server):
	_pImpl(new SharedMemoryImpl(name, size, mode, addrHint, server))
{
}


SharedMemory::SharedMemory(const Poco::File& file, AccessMode mode, const void* addrHint):
	_pImpl(new SharedMemoryImpl(file, mode, addrHint))
{
}


SharedMemory::SharedMemory(const SharedMemory& other):
	_pImpl(other._pImpl)
{
	if (_pImpl)
		_pImpl->duplicate();
}


SharedMemory::~SharedMemory()
{
	if (_pImpl)
		_pImpl->release();
}


SharedMemory& SharedMemory::operator = (const SharedMemory& other)
{
	SharedMemory tmp(other);
	swap(tmp);
	return *this;
}


char* SharedMemory::begin() const
{
	if (_pImpl)
		return _pImpl->begin();
	else
		return 0;
}


char* SharedMemory::end() const
{
	if (_pImpl)
		return _pImpl->end();
	else
		return 0;
}


} // namespace Poco
