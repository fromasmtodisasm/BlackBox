#include <BlackBox/System/SystemEventDispatcher.hpp>
#include <cassert>

CSystemEventDispatcher::CSystemEventDispatcher()

{
}

bool CSystemEventDispatcher::RegisterListener(ISystemEventListener* pListener, const char* szName)
{
  if (!pListener || !szName || !strlen(szName) || szName[0] == '\0')
  {
    CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR_DBGBRK, "CSystemEventDispatcher::RegisterListener(ISystemEventListener* pListener, const char* szName) => Was called with Null listener or invalid szName! The Listner was not registered!");
    return false;
  }

  m_listeners.insert(pListener);
  return true;
}

bool CSystemEventDispatcher::RemoveListener(ISystemEventListener* pListener)
{
  if (!pListener)
  {
    CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR_DBGBRK, "CSystemEventDispatcher::RemoveListener(ISystemEventListener* pListener) => Was called with Null listener!");
  }

  //m_listenerRegistrationLock.Lock();
  m_listeners.erase(pListener);
  //m_listenerRegistrationLock.Unlock();
  return true;
}

//////////////////////////////////////////////////////////////////////////
void CSystemEventDispatcher::OnSystemEventAnyThread(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
#if 0
  //m_listenerRegistrationLock.Lock();
  for (TSystemEventListeners::Notifier notifier(m_listeners); notifier.IsValid(); notifier.Next())
  {
    notifier->OnSystemEventAnyThread(event, wparam, lparam);
  }
  m_listenerRegistrationLock.Unlock();
#endif
}

//////////////////////////////////////////////////////////////////////////
void CSystemEventDispatcher::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam, bool force_queue /*= false*/)
{
  if (!force_queue)
  {
    for (auto notifier : m_listeners)
    {
      notifier->OnSystemEvent(event, wparam, lparam);
    }
  }
  else
  {
    SEventParams params;
    params.event = event;
    params.wparam = wparam;
    params.lparam = lparam;
    m_systemEventQueue.push(params);
  }

  // Also dispatch the event on this thread. This technically means the event
  //	will be sent twice (thru different OnSystemEventXX functions), therefore it is up to listeners which one they react to.
  OnSystemEventAnyThread(event, wparam, lparam);
}

//////////////////////////////////////////////////////////////////////////
void CSystemEventDispatcher::Update()
{
  SEventParams params;
  while (!m_systemEventQueue.empty())
  {
    params = m_systemEventQueue.front();
    m_systemEventQueue.pop();
    OnSystemEvent(params.event, params.wparam, params.lparam);
  }
}