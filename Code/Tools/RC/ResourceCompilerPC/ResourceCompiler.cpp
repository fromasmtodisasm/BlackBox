#include "ResourceCompiler.h"

#include <cstdio>

struct IResourceCompiler;

// This is an example of an exported function.
void __stdcall RegisterConverters(IResourceCompiler* RC)
{
	std::printf("Register PC Resource Compiler");
}
