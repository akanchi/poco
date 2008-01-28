//
// WindowsConsoleChannel.cpp
//
// $Id: //poco/svn/Foundation/src/WindowsConsoleChannel.cpp#2 $
//
// Library: Foundation
// Package: Logging
// Module:  WindowsConsoleChannel
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


#include "Poco/WindowsConsoleChannel.h"
#include "Poco/Message.h"
#if defined(POCO_WIN32_UTF8)
#include "Poco/UnicodeConverter.h"
#endif


namespace Poco {


WindowsConsoleChannel::WindowsConsoleChannel():
	_isFile(false),
	_hConsole(INVALID_HANDLE_VALUE)
{
	_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// check whether the console has been redirected
	DWORD mode;	
	_isFile = (GetConsoleMode(_hConsole, &mode) == 0);
}


WindowsConsoleChannel::~WindowsConsoleChannel()
{
}


void WindowsConsoleChannel::log(const Message& msg)
{
	std::string text = msg.getText();
	text += "\r\n";
	
#if defined(POCO_WIN32_UTF8)
	if (_isFile)
	{
		DWORD written;
		WriteFile(_hConsole, text.data(), text.size(), &written, NULL);	
	}
	else
	{
		std::wstring utext;
		UnicodeConverter::toUTF16(text, utext);
		DWORD written;
		WriteConsoleW(_hConsole, utext.data(), utext.size(), &written, NULL);
	}
#else
	DWORD written;
	WriteFile(_hConsole, text.data(), text.size(), &written, NULL);	
#endif
}


} // namespace Poco
