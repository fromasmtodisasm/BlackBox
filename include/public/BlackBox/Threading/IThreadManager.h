#pragma once
#include <thread>
#include <mutex>

class IThread
{
public:
	// <interfuscator:shuffle>
	virtual ~IThread()
	{
	}

	//! Entry functions for code executed on thread.
	virtual void ThreadEntry() = 0;
	// </interfuscator:shuffle>
};

