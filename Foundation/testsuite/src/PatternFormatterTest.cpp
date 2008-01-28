//
// PatternFormatterTest.cpp
//
// $Id: //poco/svn/Foundation/testsuite/src/PatternFormatterTest.cpp#2 $
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


#include "PatternFormatterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"
#include "Poco/DateTime.h"


using Poco::PatternFormatter;
using Poco::Message;
using Poco::DateTime;


PatternFormatterTest::PatternFormatterTest(const std::string& name): CppUnit::TestCase(name)
{
}


PatternFormatterTest::~PatternFormatterTest()
{
}


void PatternFormatterTest::testPatternFormatter()
{
	Message msg;
	PatternFormatter fmt;
	msg.setSource("TestSource");
	msg.setText("Test message text");
	msg.setPid(1234);
	msg.setTid(1);
	msg.setThread("TestThread");
	msg.setPriority(Message::PRIO_ERROR);
	msg.setTime(DateTime(2005, 1, 1, 14, 30, 15, 500).timestamp());
	msg["testParam"] = "Test Parameter";
	
	std::string result;
	fmt.setProperty("pattern", "%Y-%m-%dT%H:%M:%S [%s] %p: %t");
	fmt.format(msg, result);
	assert (result == "2005-01-01T14:30:15 [TestSource] Error: Test message text");
	
	result.clear();
	fmt.setProperty("pattern", "%w, %e %b %y %H:%M:%S.%i [%s:%I:%T] %q: %t");
	fmt.format(msg, result);
	assert (result == "Sat, 1 Jan 05 14:30:15.500 [TestSource:1:TestThread] E: Test message text");

	result.clear();
	fmt.setProperty("pattern", "%Y-%m-%d %H:%M:%S [%N:%P:%s]%l-%t");
	fmt.format(msg, result);
	assert (result.find("2005-01-01 14:30:15 [") == 0);
	assert (result.find(":TestSource]3-Test message text") != std::string::npos);
	
	result.clear();
	assert (fmt.getProperty("times") == "UTC");
	fmt.setProperty("times", "local");
	fmt.format(msg, result);
	assert (result.find("2005-01-01 ") == 0);
	assert (result.find(":TestSource]3-Test message text") != std::string::npos);
	
	result.clear();
	fmt.setProperty("pattern", "%[testParam]");
	fmt.format(msg, result);
	assert (result == "Test Parameter");

	result.clear();
	fmt.setProperty("pattern", "%[testParam] %p");
	fmt.format(msg, result);
	assert (result == "Test Parameter Error");
}


void PatternFormatterTest::setUp()
{
}


void PatternFormatterTest::tearDown()
{
}


CppUnit::Test* PatternFormatterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PatternFormatterTest");

	CppUnit_addTest(pSuite, PatternFormatterTest, testPatternFormatter);

	return pSuite;
}
