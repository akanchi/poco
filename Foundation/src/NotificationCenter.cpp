//
// NotificationCenter.cpp
//
// $Id: //poco/svn/Foundation/src/NotificationCenter.cpp#2 $
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
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


#include "Poco/NotificationCenter.h"
#include "Poco/Notification.h"
#include "Poco/Observer.h"
#include "Poco/AutoPtr.h"
#include "Poco/SingletonHolder.h"


namespace Poco {


NotificationCenter::NotificationCenter()
{
}


NotificationCenter::~NotificationCenter()
{
	for (ObserverList::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		delete *it;
	}
}


void NotificationCenter::addObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	_observers.push_front(observer.clone());
}


void NotificationCenter::removeObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	for (ObserverList::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		if (*it && observer.equals(**it))
		{
			delete *it;
			*it = 0;
			return;
		}
	}
}


void NotificationCenter::postNotification(Notification* pNotification)
{
	poco_check_ptr (pNotification);

	Mutex::ScopedLock lock(_mutex);
	AutoPtr<Notification> pNf = pNotification;
	ObserverList::iterator it = _observers.begin();
	while (it != _observers.end())
	{
		ObserverList::iterator cur = it++;
		if (*cur)
		{
			(*cur)->notify(pNotification);
		}
		else
		{
			_observers.erase(cur);
		}
	}
}


bool NotificationCenter::hasObservers() const
{
	Mutex::ScopedLock lock(_mutex);

	ObserverList::const_iterator it = _observers.begin();
	while (it != _observers.end())
	{
		if (*it) return true;
		++it;
	}
	return false;
}


NotificationCenter& NotificationCenter::defaultCenter()
{
	static SingletonHolder<NotificationCenter> sh;
	return *sh.get();
}


} // namespace Poco
