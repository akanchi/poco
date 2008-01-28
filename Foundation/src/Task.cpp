//
// Task.cpp
//
// $Id: //poco/svn/Foundation/src/Task.cpp#2 $
//
// Library: Foundation
// Package: Tasks
// Module:  Tasks
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


#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/Exception.h"


namespace Poco {


Task::Task(const std::string& name):
	_name(name),
	_pOwner(0),
	_progress(0),
	_state(TASK_IDLE),
	_cancelEvent(false)
{
}


Task::~Task()
{
}


void Task::cancel()
{
	_state = TASK_CANCELLING;
	_cancelEvent.set();
	if (_pOwner)
		_pOwner->taskCancelled(this);
}


void Task::reset()
{
	_progress = 0.0;
	_state    = TASK_IDLE;
	_cancelEvent.reset();
}


void Task::run()
{
	TaskManager* pOwner = getOwner();
	if (pOwner)
		pOwner->taskStarted(this);		
	try
	{
		_state = TASK_RUNNING;
		runTask();
	}
	catch (Exception& exc)
	{
		if (pOwner)
			pOwner->taskFailed(this, exc);
	}
	catch (std::exception& exc)
	{
		if (pOwner)
			pOwner->taskFailed(this, SystemException(exc.what()));
	}
	catch (...)
	{
		if (pOwner)
			pOwner->taskFailed(this, SystemException("unknown exception"));
	}
	_state = TASK_FINISHED;
	if (pOwner)
		pOwner->taskFinished(this);
}


bool Task::sleep(long milliseconds)
{
	return _cancelEvent.tryWait(milliseconds);
}


void Task::setProgress(float progress)
{
	FastMutex::ScopedLock lock(_mutex);

	_progress = progress;
	if (_pOwner)
		_pOwner->taskProgress(this, _progress);
}


void Task::setOwner(TaskManager* pOwner)
{
	FastMutex::ScopedLock lock(_mutex);

	_pOwner = pOwner;
}


void Task::setState(TaskState state)
{
	_state = state;
}


void Task::postNotification(Notification* pNf)
{
	poco_check_ptr (pNf);

	FastMutex::ScopedLock lock(_mutex);
	
	if (_pOwner)
	{
		_pOwner->postNotification(pNf);
	}
}


} // namespace Poco
