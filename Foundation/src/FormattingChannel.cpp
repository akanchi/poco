//
// FormattingChannel.cpp
//
// $Id: //poco/svn/Foundation/src/FormattingChannel.cpp#2 $
//
// Library: Foundation
// Package: Logging
// Module:  Formatter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/FormattingChannel.h"
#include "Poco/Formatter.h"
#include "Poco/Message.h"
#include "Poco/LoggingRegistry.h"


namespace Poco {


FormattingChannel::FormattingChannel(): 
	_pFormatter(0), 
	_pChannel(0)
{
}


FormattingChannel::FormattingChannel(Formatter* pFormatter): 
	_pFormatter(pFormatter), 
	_pChannel(0)
{
	if (_pFormatter) _pFormatter->duplicate();
}


FormattingChannel::FormattingChannel(Formatter* pFormatter, Channel* pChannel): 
	_pFormatter(pFormatter), 
	_pChannel(pChannel)
{
	if (_pFormatter) _pFormatter->duplicate();
	if (_pChannel)   _pChannel->duplicate();
}


FormattingChannel::~FormattingChannel()
{
	if (_pChannel)   _pChannel->release();
	if (_pFormatter) _pFormatter->release();
}


void FormattingChannel::setFormatter(Formatter* pFormatter)
{
	if (_pFormatter) _pFormatter->release();
	_pFormatter = pFormatter;
	if (_pFormatter) _pFormatter->duplicate();
}


Formatter* FormattingChannel::getFormatter() const
{
	return _pFormatter;
}


void FormattingChannel::setChannel(Channel* pChannel)
{
	if (_pChannel) _pChannel->release();
	_pChannel = pChannel;
	if (_pChannel) _pChannel->duplicate();
}


Channel* FormattingChannel::getChannel() const
{
	return _pChannel;
}


void FormattingChannel::log(const Message& msg)
{
	if (_pChannel)
	{
		if (_pFormatter)
		{
			std::string text;
			_pFormatter->format(msg, text);
			_pChannel->log(Message(msg, text));
		}
		else
		{
			_pChannel->log(msg);
		}
	}
};


void FormattingChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == "channel")
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	else if (name == "formatter")
		setFormatter(LoggingRegistry::defaultRegistry().formatterForName(value));
	else if (_pChannel)
		_pChannel->setProperty(name, value);
}


void FormattingChannel::open()
{
	if (_pChannel)
		_pChannel->open();
}

	
void FormattingChannel::close()
{
	if (_pChannel)
		_pChannel->close();
}


} // namespace Poco
