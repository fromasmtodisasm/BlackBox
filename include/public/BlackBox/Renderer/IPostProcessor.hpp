#pragma once

struct IPostProcessor
{
	virtual void Do(unsigned int texture) = 0;
};