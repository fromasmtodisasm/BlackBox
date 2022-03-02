#pragma once

struct IResourceCompiler
{
};

class ResourceCompiler : public IResourceCompiler
{
public:
	ResourceCompiler() = default;
	bool RegisterConverters();
};

// this is the plugin function that's exported by plugins
// Registers all converters residing in this DLL
extern "C"
{
	typedef void(__stdcall* FnRegisterConverters)(IResourceCompiler* pRC);
}
