// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

/*************************************************************************
   -------------------------------------------------------------------------
   $Id$
   $DateTime$
   Description:

   System "Hardware mouse" cursor with reference counter.
   This is needed because Menus / HUD / Profiler / or whatever
   can use the cursor not at the same time be successively
   => We need to know when to enable/disable the cursor.

   -------------------------------------------------------------------------
   History:
   - 18:12:2006   Created by Julien Darré

*************************************************************************/

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/HardwareMouse.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ITimer.hpp>
//#include <BlackBox/Core/Platform/WindowsUtils.hpp>

//#include <BlackBox/Core/Platform/CryLibrary.hpp>

#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	#include <CryRenderer/IImage.h>
#endif

namespace
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
void ReleaseCursor()
{
	::ClipCursor(nullptr);
}
#endif
}

//-----------------------------------------------------------------------------------------------------

CHardwareMouse::CHardwareMouse(bool bVisibleByDefault)
	: m_debugHardwareMouse(0)
	, m_pExclusiveEventListener(nullptr)
	, m_pCursorTexture(nullptr)
	, m_bPrevShowState(true)
#if !defined(_RELEASE)
	, m_allowConfine(GetISystem()->GetICmdLine()->FindArg(eCLAT_Pre, "nomouse") == nullptr)
#else
	, m_allowConfine(true)
#endif // !defined(_RELEASE)
	, m_shouldUseSystemCursor(gEnv->IsEditor())
	, m_usingSystemCursor(true)
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	, m_hCursor(nullptr)
	, m_nCurIDCCursorId(~0)
#endif
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	atexit(ReleaseCursor);
#endif

	if (gEnv->pConsole)
	{
		ICVar* pCursorTexturePath = gEnv->pConsole->GetCVar("r_MouseCursorTexture");
		if (pCursorTexturePath)
		{
			SetCursor(pCursorTexturePath->GetString());
		}
	}

#if !BB_PLATFORM_WINDOWS
	if (gEnv->pRenderer)
		SetHardwareMousePosition(gEnv->pRenderer->GetWidth() * 0.5f, gEnv->pRenderer->GetHeight() * 0.5f);
	else
		SetHardwareMousePosition(0.0f, 0.0f);
#endif

	Reset(bVisibleByDefault);

	if (gEnv->pSystem)
		gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this,"CHardwareMouse");

#if !defined(_RELEASE)
	if (gEnv->pConsole)
		REGISTER_CVAR2("g_debugHardwareMouse", &m_debugHardwareMouse, 0, VF_CHEAT, "Enables debug mode for the hardware mouse.");
#endif //!defined(_RELEASE)

	m_hide = false;
	m_calledShowHWMouse = false;

	if (IsFullscreen())
		ConfineCursor(true);

#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	CryLog("Initialized hardware mouse (game is %s to confine mouse to window)", m_allowConfine ? "allowed" : "not allowed");
#endif
}

//-----------------------------------------------------------------------------------------------------

CHardwareMouse::~CHardwareMouse()
{
	if (gEnv)
	{
		if (gEnv->pRenderer)
		{
			//gEnv->pRenderer->RemoveListener(this);
#if !defined(DEDICATED_SERVER)
			SAFE_RELEASE(m_pCursorTexture);     // On dedicated server this texture is actually a static returned by NULL renderer.. can't release that.
#endif
		}
		if (gEnv->pInput)
			gEnv->pInput->RemoveEventListener(this);

		if (gEnv->pSystem)
			gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

#if !defined(_RELEASE)
		if (gEnv->pConsole)
			gEnv->pConsole->UnregisterVariable("g_debugHardwareMouse", true);
#endif //!defined(_RELEASE)
	}

	DestroyCursor();
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::ShowHardwareMouse(bool bShow)
{
	// Only manage mouse visibility if the application actually has focus.
	// We don't want to mess around with the cursor otherwise
	if (!m_bFocus)
	{
		return;
	}

	// Apply overrides here. Needless to say if any override changes value, then ShowHardwareMouse
	// should be called with the reference counted value
	bShow = m_shouldUseSystemCursor && ((bShow && !m_hide) || (m_allowConfine == false));

	// This is very important! Since ShowCursor acts as a stack on OS level,
	// and the state here is quite complex, with variables suspending reference
	// counting and enforcable state, we really should only call pInput->ShowCursor if the value
	// was different than before. This ensures that at OS level we never have
	// a stack value more than 0/1 (at least we don't have a bigger/lower value that is
	// caused by CHardwareMouse).
	if (bShow == m_bPrevShowState)
	{
		return;
	}

	m_bPrevShowState = bShow;

	if (m_debugHardwareMouse)
	{
		LogAlways("HM: ShowHardwareMouse = %d", bShow);
	}

	if (bShow)
	{
		SetHardwareMousePosition(m_fCursorX, m_fCursorY);
	}
	else
	{
		GetHardwareMousePosition(&m_fCursorX, &m_fCursorY);
	}

	IInput* const pInput = gEnv->pInput;
	if (pInput)
	{
		pInput->ShowCursor(bShow);
		pInput->SetExclusiveMode(eIDT_Mouse, false);
	}

	m_calledShowHWMouse = true;
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::ConfineCursor(bool confine)
{
	if (m_debugHardwareMouse)
		LogAlways("HM: ConfineCursor = %d", confine);

	if (!gEnv || gEnv->pRenderer == NULL || m_allowConfine == false || !m_bFocus)
		return;

#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	HWND hWnd = 0;

	if (gEnv->IsEditor())
		hWnd = (HWND) gEnv->pRenderer->GetCurrentContextHWND();
	else
		hWnd = (HWND) gEnv->pRenderer->GetHWND();

	if (hWnd)
	{
		// It's necessary to call ClipCursor AFTER the calls to
		// CreateDevice/ResetDevice otherwise the clip area is reseted.
		if (confine && !gEnv->IsEditing())
		{
			if (m_debugHardwareMouse)
				gEnv->pLog->Log("HM:   Confining cursor");
			RECT rcClient;
			::GetClientRect(hWnd, &rcClient);
			::ClientToScreen(hWnd, (LPPOINT)&rcClient.left);
			::ClientToScreen(hWnd, (LPPOINT)&rcClient.right);
			::ClipCursor(&rcClient);
		}
		else
		{
			if (m_debugHardwareMouse)
				gEnv->pLog->Log("HM:   Releasing cursor");
			::ClipCursor(NULL);
		}
	}
#elif BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_MAC
	if (gEnv->pInput)
	{
		if (confine && !gEnv->IsEditing())
			gEnv->pInput->GrabInput(true);
		else
			gEnv->pInput->GrabInput(false);
	}
#endif
}

#if 0
//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::OnPostCreateDevice()
{
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::OnPostResetDevice()
{
}

//-----------------------------------------------------------------------------------------------------
#endif

bool CHardwareMouse::OnInputEvent(const SInputEvent& rInputEvent)
{
	static float s_fAcceleration = 1.0f;

#if CRY_PLATFORM_ORBIS
	const EKeyId thumbX = eKI_Orbis_StickLX;
	const EKeyId thumbY = eKI_Orbis_StickLY;
	const EKeyId XButton = eKI_Orbis_Cross;
#else
	const EKeyId thumbX = eKI_XI_ThumbLX;
	const EKeyId thumbY = eKI_XI_ThumbLY;
	const EKeyId XButton = eKI_XI_A; //A on 360
#endif

	if (0 == m_iReferenceCounter)
	{
		// Do not emulate if mouse is not present on screen

		// But need to reset movement values otherwise if using controller and mouse at same time, it can get into the situation where
		// the mouse activates before analog stick gets back to deadzone which will cause the cursor to be stuck moving from in below eKI_SYS_Commit event
		m_fIncX = 0.0f;
		m_fIncY = 0.0f;
	}
	else if (eIDT_Gamepad == rInputEvent.deviceType)
	{
		if (rInputEvent.keyId == thumbX)
		{
			m_fIncX = rInputEvent.value;
		}
		else if (rInputEvent.keyId == thumbY)
		{
			m_fIncY = -rInputEvent.value;
		}
		else if (rInputEvent.keyId == XButton)
		{
			// This emulation was just not right, A-s meaning is context sensitive
			/*if(eIS_Pressed == rInputEvent.state)
			   {
			   Event((int)m_fCursorX,(int)m_fCursorY,HARDWAREMOUSEEVENT_LBUTTONDOWN);
			   }
			   else if(eIS_Released == rInputEvent.state)
			   {
			   Event((int)m_fCursorX,(int)m_fCursorY,HARDWAREMOUSEEVENT_LBUTTONUP);
			   }*/
			// TODO: do we simulate double-click?
		}
	}
#if !BB_PLATFORM_WINDOWS
	else if (rInputEvent.deviceType == eIDT_Mouse)
	{
		if (rInputEvent.keyId == eKI_MouseXAbsolute)
		{
			m_fCursorX = rInputEvent.value;
			SetHardwareMousePosition(m_fCursorX, m_fCursorY);
		}
		else if (rInputEvent.keyId == eKI_MouseYAbsolute)
		{
			m_fCursorY = rInputEvent.value;
			SetHardwareMousePosition(m_fCursorX, m_fCursorY);
		}
		else if (rInputEvent.keyId == eKI_Mouse1)
		{
			if (rInputEvent.state == eIS_Pressed)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_LBUTTONDOWN);
			else if (rInputEvent.state == eIS_Released)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_LBUTTONUP);
		}
		else if (rInputEvent.keyId == eKI_Mouse2)
		{
			if (rInputEvent.state == eIS_Pressed)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_RBUTTONDOWN);
			else if (rInputEvent.state == eIS_Released)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_RBUTTONUP);
		}
		else if (rInputEvent.keyId == eKI_Mouse3)
		{
			if (rInputEvent.state == eIS_Pressed)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_MBUTTONDOWN);
			else if (rInputEvent.state == eIS_Released)
				Event((int)m_fCursorX, (int)m_fCursorY, HARDWAREMOUSEEVENT_MBUTTONUP);
		}
	}
#endif
	else if (rInputEvent.keyId == eKI_SYS_Commit)
	{
		const float fSensitivity = 100.0f;
		const float fDeadZone = 0.3f;

		if (m_fIncX < -fDeadZone || m_fIncX > +fDeadZone ||
		    m_fIncY < -fDeadZone || m_fIncY > +fDeadZone)
		{
			float fFrameTime = gEnv->pTimer->GetFrameTime(ITimer::ETIMER_UI);
			if (s_fAcceleration < 10.0f)
			{
				s_fAcceleration += fFrameTime * 5.0f;
			}
			m_fCursorX += m_fIncX * fSensitivity * s_fAcceleration * fFrameTime;
			m_fCursorY += m_fIncY * fSensitivity * s_fAcceleration * fFrameTime;
			SetHardwareMousePosition(m_fCursorX, m_fCursorY);
		}
		else
		{
			GetHardwareMousePosition(&m_fCursorX, &m_fCursorY);
			s_fAcceleration = 1.0f;
		}
	}

	return false;
}

void CHardwareMouse::HandleFocusEvent(bool bFocus)
{
	//gEnv->pLog->Log("Change focus - %d", bFocus);

	if (bFocus)
	{
		// set focus first so the calls below work
		m_bFocus = true;

		ShowHardwareMouse(m_iReferenceCounter >= 1);
		EvaluateCursorConfinement();
	}
	else
	{
		// always, always show and release the mouse confinement when we lose focus
		ShowHardwareMouse(true);
		ConfineCursor(false);

		// first release, then lose focus, so calls above work
		m_bFocus = false;
	}
}

//-----------------------------------------------------------------------------------------------------
void CHardwareMouse::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	// if we are in editor then we wait for ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE instead because we care for more
	// fine grained focus notifications to determine mouse visibility and capture
	if (event == ESYSTEM_EVENT_ACTIVATE || event == ESYSTEM_EVENT_CHANGE_FOCUS)
	{
#if 0
		if (!gEnv->IsEditor())
		{
			bool bFocus = (wparam != 0);
			HandleFocusEvent(bFocus);
		}
#else
			bool bFocus = (wparam != 0);
			HandleFocusEvent(bFocus);
#endif
	}
#if 0
	// only makes sense for editor (or engine clients that support many windows)
	else if (event == ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE)
	{
		bool bFocus = (wparam != 0);
		HandleFocusEvent(bFocus);
	}
	else if (event == ESYSTEM_EVENT_MOVE)
	{
		EvaluateCursorConfinement();
	}
#endif
	else if (event == ESYSTEM_EVENT_RESIZE)
	{
		EvaluateCursorConfinement();
	}
#if 0
	else if (event == ESYSTEM_EVENT_TOGGLE_FULLSCREEN)
	{
		EvaluateCursorConfinement();
	}
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Release()
{
	delete this;
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::OnPreInitRenderer()
{
#if 0
	ASSERT(gEnv->pRenderer);

	if (gEnv->pRenderer)
		gEnv->pRenderer->AddListener(this);
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::OnPostInitInput()
{
	ASSERT(gEnv->pInput);

	if (gEnv->pInput)
		gEnv->pInput->AddEventListener(this);
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Event(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta)
{
	// TODO: console event filtering should not be hardwired, it should use something like the exclusive event listener mechanism, maybe with some priority ordering capability added
	// Ignore events while console is active
	if (gEnv->pConsole->IsOpened())
	{
		return;
	}

	if (m_pExclusiveEventListener)
		m_pExclusiveEventListener->OnHardwareMouseEvent(iX, iY, eHardwareMouseEvent, wheelDelta);
	else
	{
		for (TListHardwareMouseEventListeners::iterator iter = m_listHardwareMouseEventListeners.begin(); iter != m_listHardwareMouseEventListeners.end(); ++iter)
		{
			(*iter)->OnHardwareMouseEvent(iX, iY, eHardwareMouseEvent, wheelDelta);
		}
	}
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::AddListener(IHardwareMouseEventListener* pHardwareMouseEventListener)
{
#if 0
	stl::push_back_unique(m_listHardwareMouseEventListeners, pHardwareMouseEventListener);
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::RemoveListener(IHardwareMouseEventListener* pHardwareMouseEventListener)
{
#if 0
	stl::find_and_erase(m_listHardwareMouseEventListeners, pHardwareMouseEventListener);
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::SetGameMode(bool bGameMode)
{
	if (bGameMode)
	{
		DecrementCounter();
	}
	else
	{
		IncrementCounter();
	}
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::IncrementCounter()
{
	m_iReferenceCounter++;

	if (m_debugHardwareMouse)
		LogAlways("HM: IncrementCounter = %d", m_iReferenceCounter);
	ASSERT(m_iReferenceCounter >= 0);

	ShowHardwareMouse(m_iReferenceCounter > 0);
	EvaluateCursorConfinement();
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::DecrementCounter()
{
	m_iReferenceCounter--;

	if (m_debugHardwareMouse)
		LogAlways("HM: DecrementCounter = %d", m_iReferenceCounter);
	ASSERT(m_iReferenceCounter >= 0);

	ShowHardwareMouse(m_iReferenceCounter > 0);
	EvaluateCursorConfinement();
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::GetHardwareMousePosition(float* pfX, float* pfY)
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	POINT pointCursor;
	GetCursorPos(&pointCursor);
	*pfX = (float)pointCursor.x;
	*pfY = (float)pointCursor.y;
#else
	*pfX = m_fVirtualX;
	*pfY = m_fVirtualY;
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::SetHardwareMousePosition(float fX, float fY)
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	if (gEnv->pRenderer)
	{
		HWND hWnd = (HWND)gEnv->pRenderer->GetCurrentContextHWND();
		if (hWnd == ::GetFocus() && m_allowConfine)
		{
			// Move cursor position only if our window is focused.
			SetCursorPos((int)fX, (int)fY);
		}
	}
#else
	m_fVirtualX = fX;
	m_fVirtualY = fY;
	if (gEnv && gEnv->pRenderer)
	{
		float fWidth = float(gEnv->pRenderer->GetWidth());
		float fHeight = float(gEnv->pRenderer->GetHeight());

		if (m_fVirtualX < 0.0f)
		{
			m_fVirtualX = 0.0f;
		}
		else if (m_fVirtualX >= fWidth)
		{
			m_fVirtualX = fWidth - 1.0f;
		}

		if (m_fVirtualY < 0.0f)
		{
			m_fVirtualY = 0.0f;
		}
		else if (m_fVirtualY >= fHeight)
		{
			m_fVirtualY = fHeight - 1.0f;
		}
	}
	Event(int(m_fVirtualX), int(m_fVirtualY), HARDWAREMOUSEEVENT_MOVE);
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::GetHardwareMouseClientPosition(float* pfX, float* pfY)
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	if (gEnv == NULL || gEnv->pRenderer == NULL)
		return;

	HWND hWnd = (HWND) gEnv->pRenderer->GetCurrentContextHWND();
	ASSERT(hWnd, "Impossible to get client coordinates from a non existing window!");

	if (hWnd)
	{
		POINT pointCursor;
		GetCursorPos(&pointCursor);
		ScreenToClient(hWnd, &pointCursor);
		*pfX = (float) pointCursor.x;
		*pfY = (float) pointCursor.y;
	}
	else
	{
		*pfX = 0.0f;
		*pfY = 0.0f;
	}
#else
	*pfX = m_fVirtualX;
	*pfY = m_fVirtualY;
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::SetHardwareMouseClientPosition(float fX, float fY)
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	HWND hWnd = (HWND) gEnv->pRenderer->GetCurrentContextHWND();
	ASSERT(hWnd, "Impossible to set position of the mouse relative to client coordinates from a non existing window!");

	if (hWnd)
	{
		POINT pointCursor;
		pointCursor.x = (int)fX;
		pointCursor.y = (int)fY;
		ClientToScreen(hWnd, &pointCursor);
		SetCursorPos(pointCursor.x, pointCursor.y);
	}
#else
	SetHardwareMousePosition(fX, fY);
#endif
}

//-----------------------------------------------------------------------------------------------------
bool CHardwareMouse::SetCursor(int idc_cursor_id)
{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	if (m_nCurIDCCursorId != idc_cursor_id || !m_curCursorPath.empty())
		DestroyCursor();

	if (m_shouldUseSystemCursor)
	{
		if (!m_hCursor)
		{
			m_hCursor = ::LoadCursor(CryGetCurrentModule(), MAKEINTRESOURCE(idc_cursor_id));

			if (!m_hCursor)
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unable to load cursor from windows resource id: %i", idc_cursor_id);
				return false;
			}
		}

		if (m_hCursor)
		{
			if (HCURSOR ret = ::SetCursor(m_hCursor))
			{
				m_nCurIDCCursorId = idc_cursor_id;
				m_curCursorPath.clear();
			}
		}
	}
	else // SW cursor
	{
		return SetCursor(gEnv->pConsole->GetCVar("r_MouseCursorTexture")->GetString());
	}

	return m_hCursor ? true : false;
#else
	CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Setting cursor via windows resource id is not supported on this platform. Using default cursor.");
	return SetCursor(gEnv->pConsole->GetCVar("r_MouseCursorTexture")->GetString());
#endif
}

//-----------------------------------------------------------------------------------------------------
bool CHardwareMouse::SetCursor(const char* path)
{
#if 0
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	if (m_nCurIDCCursorId || m_curCursorPath.compare(path))
		DestroyCursor();
#else
	if (m_curCursorPath.compare(path))
		DestroyCursor();
#endif

	// Update the CVar value to match, in case this function was not called through CVar-change-callback
	gEnv->pConsole->GetCVar("r_MouseCursorTexture")->Set(path);

	// Load cursor
	if (m_shouldUseSystemCursor)
	{
#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
		// HW cursor
		if (!m_hCursor)
		{
			m_hCursor = CreateResourceFromTexture(gEnv->pRenderer, path, eResourceType_Cursor);
			if (!m_hCursor)
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unable to load cursor from texture file. Path %s", path);
				return false;
			}

			//Example to load a cursor from file (.cur or .ani) (when not in pak)
			//m_hCursor = LoadCursorFromFile("<my_path>\\my_cursor.cur");
		}

		// Set cursor
		if (m_hCursor)
		{
			if (HCURSOR ret = ::SetCursor(m_hCursor))
			{
				m_nCurIDCCursorId = 0;
				m_curCursorPath.assign(path);
			}
		}
		return m_hCursor != nullptr;
#else
		return false;
#endif
	}
	else // SW cursor
	{
		if (gEnv->pRenderer && !m_pCursorTexture)
		{
			m_pCursorTexture = gEnv->pRenderer->EF_LoadTexture(path, FT_DONT_RELEASE | FT_DONT_STREAM);

			if (!m_pCursorTexture)
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unable to open image for cursor from path: %s", path);
				return false;
			}

			m_pCursorTexture->SetClamp(true);
			m_curCursorPath.assign(path);
			return true;
		}
		return true;
	}
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Reset(bool bVisibleByDefault)
{
	m_iReferenceCounter = bVisibleByDefault ? 1 : 0;
	//ShowHardwareMouse(bVisibleByDefault);
	GetHardwareMousePosition(&m_fCursorX, &m_fCursorY);

	if (!bVisibleByDefault)
		ConfineCursor(true);

	m_fIncX = 0.0f;
	m_fIncY = 0.0f;
	m_bFocus = true;
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Update()
{
	// Note: Calling ShowCursor from a different thread does not work
	if (m_usingSystemCursor != m_shouldUseSystemCursor)
	{
		m_usingSystemCursor = m_shouldUseSystemCursor;
		ShowHardwareMouse(m_iReferenceCounter > 0);
	}
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Render()
{
#if 0
	if (m_shouldUseSystemCursor)
		return;

	if (gEnv && gEnv->pRenderer && m_iReferenceCounter && m_pCursorTexture && !m_hide)
	{
		float fScalerX = gEnv->pRenderer->ScaleCoordX(1.f);
		float fScalerY = gEnv->pRenderer->ScaleCoordY(1.f);
		const float fSizeX = float(m_pCursorTexture->GetWidth());
		const float fSizeY = float(m_pCursorTexture->GetHeight());
		float fPosX, fPosY;
		GetHardwareMouseClientPosition(&fPosX, &fPosY);
		gEnv->pRenderer->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
		gEnv->pRenderer->Draw2dImage(fPosX / fScalerX, fPosY / fScalerY, fSizeX / fScalerX, fSizeY / fScalerY, m_pCursorTexture->GetTextureID(), 0, 1, 1, 0, 0, 1, 1, 1, 1, 0);
	}
#endif
}

//-----------------------------------------------------------------------------------------------------

bool CHardwareMouse::IsFullscreen()
{
#if 0
	assert(gEnv);
	assert(gEnv->pRenderer || gEnv->IsDedicated());

	bool bFullScreen = false;
	if (gEnv->pRenderer)
	{
		gEnv->pRenderer->EF_Query(EFQ_Fullscreen, bFullScreen);
	}
	return bFullScreen;
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::Hide(bool hide)
{
	m_hide = hide;
	if (m_calledShowHWMouse && gEnv->pInput)
	{
		bool bShow = m_shouldUseSystemCursor && ((m_iReferenceCounter > 0 && !m_hide) || (m_allowConfine == false));

		if (bShow != m_bPrevShowState)
		{
			m_bPrevShowState = bShow;
			gEnv->pInput->ShowCursor(bShow);
		}
	}
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::UseSystemCursor(bool useSystemCursor)
{
	m_shouldUseSystemCursor = useSystemCursor;
}

//-----------------------------------------------------------------------------------------------------

#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
void* CHardwareMouse::GetCurrentCursor()
{
	return m_hCursor;
}
#endif

//-----------------------------------------------------------------------------------------------------

bool CHardwareMouse::SetExclusiveEventListener(IHardwareMouseEventListener* pExclusiveEventListener)
{
	if (!m_pExclusiveEventListener)
	{
		m_pExclusiveEventListener = pExclusiveEventListener;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::RemoveExclusiveEventListener(IHardwareMouseEventListener* pExclusiveEventListener)
{
	if (m_pExclusiveEventListener == pExclusiveEventListener)
		m_pExclusiveEventListener = NULL;
}

//-----------------------------------------------------------------------------------------------------

void CHardwareMouse::DestroyCursor()
{
	if (m_pCursorTexture)
	{
		m_pCursorTexture->Release();
		m_pCursorTexture = 0;
	}
	m_curCursorPath.clear();

#if !defined(USE_LINUXINPUT) && defined(BB_PLATFORM_WINDOWS)
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = 0;
	}
	m_nCurIDCCursorId = 0;
#endif
}

void CHardwareMouse::EvaluateCursorConfinement()
{
	bool bConfine = IsFullscreen() || m_iReferenceCounter == 0;

	ConfineCursor(bConfine);
}
