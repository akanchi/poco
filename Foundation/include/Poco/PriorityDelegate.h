//
// PriorityDelegate.h
//
// $Id: //poco/svn/Foundation/include/Poco/PriorityDelegate.h#2 $
//
// Library: Foundation
// Package: Events
// Module:  PriorityDelegate
//
// Implementation of the PriorityDelegate template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef  Foundation_PriorityDelegate_INCLUDED
#define  Foundation_PriorityDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractPriorityDelegate.h"
#include "Poco/PriorityExpire.h"
#include "Poco/FunctionPriorityDelegate.h"


namespace Poco {


template <class TObj, class TArgs, bool useSender = true> 
class PriorityDelegate: public AbstractPriorityDelegate<TArgs>
{
public:
	typedef void (TObj::*NotifyMethod)(const void*, TArgs&);

	PriorityDelegate(TObj* obj, NotifyMethod method, int prio):
		AbstractPriorityDelegate<TArgs>(obj, prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}
	
	PriorityDelegate(const PriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate._pTarget, delegate._priority),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}
	
	PriorityDelegate& operator = (const PriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~PriorityDelegate()
	{
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		(_receiverObject->*_receiverMethod)(sender, arguments);
		return true; // per default the delegate never expires
	}

	AbstractPriorityDelegate<TArgs>* clone() const
	{
		return new PriorityDelegate(*this);
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;

private:
	PriorityDelegate();
};



template <class TObj, class TArgs> 
class PriorityDelegate<TObj, TArgs, false>: public AbstractPriorityDelegate<TArgs>
{
public:
	typedef void (TObj::*NotifyMethod)(TArgs&);

	PriorityDelegate(TObj* obj, NotifyMethod method, int prio):
		AbstractPriorityDelegate<TArgs>(obj, prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}
	
	PriorityDelegate(const PriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate._pTarget, delegate._priority),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}
	
	PriorityDelegate& operator = (const PriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~PriorityDelegate()
	{
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		(_receiverObject->*_receiverMethod)(arguments);
		return true; // per default the delegate never expires
	}

	AbstractPriorityDelegate<TArgs>* clone() const
	{
		return new PriorityDelegate(*this);
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;

private:
	PriorityDelegate();
};


template <class TObj, class TArgs>
static PriorityDelegate<TObj, TArgs, true> priorityDelegate(TObj* pObj, void (TObj::*NotifyMethod)(const void*, TArgs&), int prio)
{
	return PriorityDelegate<TObj, TArgs, true>(pObj, NotifyMethod, prio);
}


template <class TObj, class TArgs>
static PriorityDelegate<TObj, TArgs, false> priorityDelegate(TObj* pObj, void (TObj::*NotifyMethod)(TArgs&), int prio)
{
	return PriorityDelegate<TObj, TArgs, false>(pObj, NotifyMethod, prio);
}


template <class TObj, class TArgs>
static PriorityExpire<TArgs> priorityDelegate(TObj* pObj, void (TObj::*NotifyMethod)(const void*, TArgs&), int prio, Timestamp::TimeDiff expireMilliSec)
{
	return PriorityExpire<TArgs>(PriorityDelegate<TObj, TArgs, true>(pObj, NotifyMethod, prio), expireMilliSec);
}


template <class TObj, class TArgs>
static PriorityExpire<TArgs> priorityDelegate(TObj* pObj, void (TObj::*NotifyMethod)(TArgs&), int prio, Timestamp::TimeDiff expireMilliSec)
{
	return PriorityExpire<TArgs>(PriorityDelegate<TObj, TArgs, false>(pObj, NotifyMethod, prio), expireMilliSec);
}


template <class TArgs>
static PriorityExpire<TArgs> priorityDelegate(void (*NotifyMethod)(const void*, TArgs&), int prio, Timestamp::TimeDiff expireMilliSec)
{
	return PriorityExpire<TArgs>(FunctionPriorityDelegate<TArgs, true, true>(NotifyMethod, prio), expireMilliSec);
}


template <class TArgs>
static PriorityExpire<TArgs> priorityDelegate(void (*NotifyMethod)(void*, TArgs&), int prio, Timestamp::TimeDiff expireMilliSec)
{
	return PriorityExpire<TArgs>(FunctionPriorityDelegate<TArgs, true, false>(NotifyMethod, prio), expireMilliSec);
}


template <class TArgs>
static PriorityExpire<TArgs> priorityDelegate(void (*NotifyMethod)(TArgs&), int prio, Timestamp::TimeDiff expireMilliSec)
{
	return PriorityExpire<TArgs>(FunctionPriorityDelegate<TArgs, false>(NotifyMethod, prio), expireMilliSec);
}


template <class TArgs>
static FunctionPriorityDelegate<TArgs, true, true> priorityDelegate(void (*NotifyMethod)(const void*, TArgs&), int prio)
{
	return FunctionPriorityDelegate<TArgs, true, true>(NotifyMethod, prio);
}


template <class TArgs>
static FunctionPriorityDelegate<TArgs, true, false> priorityDelegate(void (*NotifyMethod)(void*, TArgs&), int prio)
{
	return FunctionPriorityDelegate<TArgs, true, false>(NotifyMethod, prio);
}


template <class TArgs>
static FunctionPriorityDelegate<TArgs, false> priorityDelegate(void (*NotifyMethod)(TArgs&), int prio)
{
	return FunctionPriorityDelegate<TArgs, false>(NotifyMethod, prio);
}


} // namespace Poco


#endif
