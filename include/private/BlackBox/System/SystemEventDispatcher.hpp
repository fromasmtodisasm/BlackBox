#ifndef __SYSTEMEVENTDISPATCHER_H__
#define __SYSTEMEVENTDISPATCHER_H__

#include <BlackBox/System/ISystem.hpp>
#include <set>
#include <map>
#include <queue>

class CSystemEventDispatcher : public ISystemEventDispatcher
{
public:
  CSystemEventDispatcher();
  virtual ~CSystemEventDispatcher() {}

  // ISystemEventDispatcher
  virtual bool RegisterListener(ISystemEventListener* pListener, const char* szName);
  virtual bool RemoveListener(ISystemEventListener* pListener);

  virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam, bool force_queue = false);
  virtual void Update();
  uint RegisterEvent(const string& EventName) override;

  // ~ISystemEventDispatcher
private:
  void OnSystemEventAnyThread(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam);

  typedef std::set<ISystemEventListener*> TSystemEventListeners;
  TSystemEventListeners m_listeners;

  // for the events coming from other threads
  struct SEventParams
  {
    ESystemEvent event;
    UINT_PTR     wparam;
    UINT_PTR     lparam;
  };

  typedef std::queue<SEventParams> TSystemEventQueue;
  TSystemEventQueue  m_systemEventQueue;

	;
	std::map<string, uint> m_UserEvents;
};

#endif //__SYSTEMEVENTDISPATCHER_H__