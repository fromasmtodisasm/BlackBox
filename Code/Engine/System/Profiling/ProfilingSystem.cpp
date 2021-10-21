#include "ProfilingSystem.hpp"

void ProfilingSystem::Reset()
{
}

void ProfilingSystem::AddFrameProfiler(CFrameProfiler* pProfiler)
{
}

void ProfilingSystem::StartFrame()
{
}

void ProfilingSystem::EndFrame()
{
}

int ProfilingSystem::GetProfilerCount() const
{
	return 0;
}

CFrameProfiler* ProfilingSystem::GetProfiler(int index) const
{
	return nullptr;
}

void ProfilingSystem::Enable(bool bCollect, bool bDisplay)
{
}

void ProfilingSystem::SetSubsystemFilter(bool bFilterSubsystem, EProfiledSubsystem subsystem)
{
}

bool ProfilingSystem::IsEnabled() const
{
	return false;
}

bool ProfilingSystem::IsProfiling() const
{
	return false;
}

void ProfilingSystem::SetDisplayQuantity(EDisplayQuantity quantity)
{
}

void ProfilingSystem::StartCustomSection(CCustomProfilerSection* pSection)
{
}

void ProfilingSystem::EndCustomSection(CCustomProfilerSection* pSection)
{
}

void ProfilingSystem::AddPeaksListener(IFrameProfilePeakCallback* pPeakCallback)
{
}

void ProfilingSystem::RemovePeaksListener(IFrameProfilePeakCallback* pPeakCallback)
{
}
