#pragma once

#include <BlackBox/System/FrameProfiler.hpp>

class ProfilingSystem : public IFrameProfileSystem
{
  public:
	ProfilingSystem() {}

	// Inherited via IFrameProfileSystem
	virtual void			Reset() override;
	virtual void			AddFrameProfiler(CFrameProfiler* pProfiler) override;
	virtual void			StartFrame() override;
	virtual void			EndFrame() override;
	virtual int				GetProfilerCount() const override;
	virtual CFrameProfiler* GetProfiler(int index) const override;
	virtual void			Enable(bool bCollect, bool bDisplay) override;
	virtual void			SetSubsystemFilter(bool bFilterSubsystem, EProfiledSubsystem subsystem) override;
	virtual bool			IsEnabled() const override;
	virtual bool			IsProfiling() const override;
	virtual void			SetDisplayQuantity(EDisplayQuantity quantity) override;
	virtual void			StartCustomSection(CCustomProfilerSection* pSection) override;
	virtual void			EndCustomSection(CCustomProfilerSection* pSection) override;
	virtual void			AddPeaksListener(IFrameProfilePeakCallback* pPeakCallback) override;
	virtual void			RemovePeaksListener(IFrameProfilePeakCallback* pPeakCallback) override;
};
