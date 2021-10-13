#pragma once
#include "IInput.h"
#include <BlackBox\Input\IInput.hpp>

namespace	  Input {
inline  EKeyId kconvertKey(Legacy::KeyCodes key)
{
	using namespace Legacy;
	switch (key)
	{
	  case XKEY_ESCAPE: return eKI_Escape;
	  case XKEY_1: return eKI_1;
	  case XKEY_2: return eKI_2;
	  case XKEY_3: return eKI_3;
	  case XKEY_4: return eKI_4;
	  case XKEY_5: return eKI_5;
	  case XKEY_6: return eKI_6;
	  case XKEY_7: return eKI_7;
	  case XKEY_8: return eKI_8;
	  case XKEY_9: return eKI_9;
	  case XKEY_0: return eKI_0;
	  case XKEY_MINUS: return eKI_Minus;
	  case XKEY_EQUALS: return eKI_Equals;
	  case XKEY_BACKSPACE: return eKI_Backspace;
	  case XKEY_TAB: return eKI_Tab;
	  case XKEY_Q: return eKI_Q;
	  case XKEY_W: return eKI_W;
	  case XKEY_E: return eKI_E;
	  case XKEY_R: return eKI_R;
	  case XKEY_T: return eKI_T;
	  case XKEY_Y: return eKI_Y;
	  case XKEY_U: return eKI_U;
	  case XKEY_I: return eKI_I;
	  case XKEY_O: return eKI_O;
	  case XKEY_P: return eKI_P;
	  case XKEY_LBRACKET: return eKI_LBracket;
	  case XKEY_RBRACKET: return eKI_RBracket;
	  case XKEY_RETURN: return eKI_Enter;
	  case XKEY_RCONTROL: return eKI_LCtrl;
	  case XKEY_A: return eKI_A;
	  case XKEY_S: return eKI_S;
	  case XKEY_D: return eKI_D;
	  case XKEY_F: return eKI_F;
	  case XKEY_G: return eKI_G;
	  case XKEY_H: return eKI_H;
	  case XKEY_J: return eKI_J;
	  case XKEY_K: return eKI_K;
	  case XKEY_L: return eKI_L;
	  case XKEY_SEMICOLON: return eKI_Semicolon;
	  case XKEY_APOSTROPHE: return eKI_Apostrophe;
	  case XKEY_TILDE: return eKI_Tilde;
	  case XKEY_LSHIFT: return eKI_LShift;
	  case XKEY_BACKSLASH: return eKI_Backslash;
	  case XKEY_Z: return eKI_Z;
	  case XKEY_X: return eKI_X;
	  case XKEY_C: return eKI_C;
	  case XKEY_V: return eKI_V;
	  case XKEY_B: return eKI_B;
	  case XKEY_N: return eKI_N;
	  case XKEY_M: return eKI_M;
	  case XKEY_COMMA: return eKI_Comma;
	  case XKEY_PERIOD: return eKI_Period;
	  case XKEY_SLASH: return eKI_Slash;
	  case XKEY_RSHIFT: return eKI_RShift;
	  case XKEY_MULTIPLY: return eKI_NP_Multiply;
	  case XKEY_LALT: return eKI_LAlt;
	  case XKEY_SPACE: return eKI_Space;
	  case XKEY_CAPSLOCK: return eKI_CapsLock;
	  case XKEY_F1: return eKI_F1;
	  case XKEY_F2: return eKI_F2;
	  case XKEY_F3: return eKI_F3;
	  case XKEY_F4: return eKI_F4;
	  case XKEY_F5: return eKI_F5;
	  case XKEY_F6: return eKI_F6;
	  case XKEY_F7: return eKI_F7;
	  case XKEY_F8: return eKI_F8;
	  case XKEY_F9: return eKI_F9;
	  case XKEY_F10: return eKI_F10;
	  case XKEY_NUMLOCK: return eKI_NumLock;
	  case XKEY_SCROLLLOCK: return eKI_ScrollLock;
	  case XKEY_NUMPAD7: return eKI_NP_7;
	  case XKEY_NUMPAD8: return eKI_NP_8;
	  case XKEY_NUMPAD9: return eKI_NP_9;
	  case XKEY_SUBTRACT: return eKI_NP_Substract;
	  case XKEY_NUMPAD4: return eKI_NP_4;
	  case XKEY_NUMPAD5: return eKI_NP_5;
	  case XKEY_NUMPAD6: return eKI_NP_6;
	  case XKEY_ADD: return eKI_NP_Add;
	  case XKEY_NUMPAD1: return eKI_NP_1;
	  case XKEY_NUMPAD2: return eKI_NP_2;
	  case XKEY_NUMPAD3: return eKI_NP_3;
	  case XKEY_NUMPAD0: return eKI_NP_0;
	  case XKEY_F11: return eKI_F11;
	  case XKEY_F12: return eKI_F12;
	  case XKEY_F13: return eKI_F13;
	  case XKEY_F14: return eKI_F14;
	  case XKEY_F15: return eKI_F15;
		  //FIXME:
		  #if 0
	  case XKEY_COLON: return eKI_Colon;
	  case XKEY_UNDERLINE: return eKI_Underline;
		  #endif
	  case XKEY_NUMPADENTER: return eKI_NP_Enter;
	  //case XKEY_RCONTROL: return eKI_RCtrl;
	  //case XKEY_PERIOD: return eKI_NP_Period;
	  //case XKEY_SLASH: return eKI_NP_Divide;
	  case XKEY_PRINT: return eKI_Print;
	  case XKEY_RALT: return eKI_RAlt;
	  case XKEY_PAUSE: return eKI_Pause;
	  case XKEY_HOME: return eKI_Home;
	  case XKEY_UP: return eKI_Up;
	  case XKEY_PAGE_UP: return eKI_PgUp;
	  case XKEY_LEFT: return eKI_Left;
	  case XKEY_RIGHT: return eKI_Right;
	  case XKEY_END: return eKI_End;
	  case XKEY_DOWN: return eKI_Down;
	  case XKEY_PAGE_DOWN: return eKI_PgDn;
	  case XKEY_INSERT: return eKI_Insert;
	  case XKEY_DELETE: return eKI_Delete;
	  case XKEY_WIN_LWINDOW: return eKI_LWin;
	  case XKEY_WIN_RWINDOW: return eKI_RWin;
	  case XKEY_WIN_APPS: return eKI_Apps;
	  case XKEY_OEM_102: return eKI_OEM_102;

	  // Mouse.
	  case XKEY_MOUSE1: return eKI_Mouse1;
	  case XKEY_MOUSE2: return eKI_Mouse2;
	  case XKEY_MOUSE3: return eKI_Mouse3;
	  case XKEY_MOUSE4: return eKI_Mouse4;
	  case XKEY_MOUSE5: return eKI_Mouse5;
	  case XKEY_MOUSE6: return eKI_Mouse6;
	  case XKEY_MOUSE7: return eKI_Mouse7;
	  case XKEY_MOUSE8: return eKI_Mouse8;
	  case XKEY_MWHEEL_UP: return eKI_MouseWheelUp;
	  case XKEY_MWHEEL_DOWN: return eKI_MouseWheelDown;
	  case XKEY_MAXIS_X: return eKI_MouseX;
	  case XKEY_MAXIS_Y: return eKI_MouseY;
		  //FIXME:
		  #if 0
	  case XKEY_MOUSEZ: return eKI_MouseZ;

	  case XKEY_MOUSEXABSOLUTE: return eKI_MouseXAbsolute;
	  case XKEY_MOUSEYABSOLUTE: return eKI_MouseYAbsolute;
		  #endif

		  //FIXME: support gamepad
		  #if 0
	  // XBox controller.
	  case XKEY_XI_DPADUP: return eKI_XI_DPadUp;
	  case XKEY_XI_DPADDOWN: return eKI_XI_DPadDown;
	  case XKEY_XI_DPADLEFT: return eKI_XI_DPadLeft;
	  case XKEY_XI_DPADRIGHT: return eKI_XI_DPadRight;
	  case XKEY_XI_START: return eKI_XI_Start;
	  case XKEY_XI_BACK: return eKI_XI_Back;
	  case XKEY_XI_THUMBL: return eKI_XI_ThumbL;
	  case XKEY_XI_THUMBR: return eKI_XI_ThumbR;
	  case XKEY_XI_SHOULDERL: return eKI_XI_ShoulderL;
	  case XKEY_XI_SHOULDERR: return eKI_XI_ShoulderR;
	  case XKEY_XI_A: return eKI_XI_A;
	  case XKEY_XI_B: return eKI_XI_B;
	  case XKEY_XI_X: return eKI_XI_X;
	  case XKEY_XI_Y: return eKI_XI_Y;
	  case XKEY_XI_TRIGGERL: return eKI_XI_TriggerL;
	  case XKEY_XI_TRIGGERR: return eKI_XI_TriggerR;
	  case XKEY_XI_THUMBLX: return eKI_XI_ThumbLX;
	  case XKEY_XI_THUMBLY: return eKI_XI_ThumbLY;
	  case XKEY_XI_THUMBLUP: return eKI_XI_ThumbLUp;
	  case XKEY_XI_THUMBLDOWN: return eKI_XI_ThumbLDown;
	  case XKEY_XI_THUMBLLEFT: return eKI_XI_ThumbLLeft;
	  case XKEY_XI_THUMBLRIGHT: return eKI_XI_ThumbLRight;
	  case XKEY_XI_THUMBRX: return eKI_XI_ThumbRX;
	  case XKEY_XI_THUMBRY: return eKI_XI_ThumbRY;
	  case XKEY_XI_THUMBRUP: return eKI_XI_ThumbRUp;
	  case XKEY_XI_THUMBRDOWN: return eKI_XI_ThumbRDown;
	  case XKEY_XI_THUMBRLEFT: return eKI_XI_ThumbRLeft;
	  case XKEY_XI_THUMBRRIGHT: return eKI_XI_ThumbRRight;
	  case XKEY_XI_TRIGGERLBTN: return eKI_XI_TriggerLBtn;
	  case XKEY_XI_TRIGGERRBTN: return eKI_XI_TriggerRBtn;
		  #endif

	  case XKEY_NULL: return eKI_Unknown;
	default:
	  return eKI_Unknown;
		break;
	}
}
}
namespace Legacy
{
	struct CKeyboard : public IKeyboard
	{
		CKeyboard()
		{
			m_pKeyboard = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
		}
		virtual void ShutDown() override
		{
		}

		//! check for key pressed and held
		virtual bool KeyDown(int p_key) override
		{
			auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));	
			return m_pKeyboard->InputState(key, EInputState::eIS_Down);

		}

		//! check for key pressed only once
		virtual bool KeyPressed(int p_key) override
		{
			auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));	
			return m_pKeyboard->InputState(key, EInputState::eIS_Pressed);
		}

		//! check if the key has been released
		virtual bool KeyReleased(int p_key) override
		{
			auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));	
			return m_pKeyboard->InputState(key, EInputState::eIS_Released);
		}

		//! clear the key status
		virtual void ClearKey(int p_key) override
		{
		}

		//! return the code of the key pressed
		virtual int GetKeyPressedCode() override
		{
			return XKEY_NULL;
		}

		//! return the name of the key pressed
		virtual const char* GetKeyPressedName() override
		{
			return "XKEY_NULL";
		}

		//! return the code of the key down
		virtual int GetKeyDownCode() override
		{
			return XKEY_NULL;
		}

		//! return the name of the key down
		virtual const char* GetKeyDownName() override
		{
			return "XKEY_NULL";
		}

		//! set/unset directinput to exclusive mode
		virtual void SetExclusive(bool value, void* hwnd = 0) override
		{
		}

		//! wait for a key pressed
		virtual void WaitForKey() override
		{
		}

		//! clear the key (pressed) state
		virtual void ClearKeyState() override
		{
			m_pKeyboard->ClearKeyState();
		}
		IInputDevice* m_pKeyboard{};
	};

	//////////////////////////////////////////////////////////////////////
	/*! Interface to the Mouse system.
*/
	struct CMouse : public IMouse
	{
		CMouse()
		{
			m_pMouse = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Mouse);
		}
		virtual void Shutdown() override
		{
		}

		//! check for a mouse button pressed and held
		virtual bool MouseDown(int p_numButton) override
		{
			auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));	
			return m_pMouse->InputState(key, EInputState::eIS_Down);
			return true;
		}

		//! check for a mouse button pressed only once
		virtual bool MousePressed(int p_numButton) override
		{
			auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));	
			return m_pMouse->InputState(key, EInputState::eIS_Pressed);
		}

		//! check if the mouse button has been released
		virtual bool MouseReleased(int p_numButton) override
		{
			auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));	
			return m_pMouse->InputState(key, EInputState::eIS_Released);
		}

		//! force the mouse wheel rotation to a certain value
		virtual void SetMouseWheelRotation(int value) override
		{
		}

		//! set/reset Directinput to exclusive mode
		virtual bool SetExclusive(bool value, void* hwnd = 0) override
		{
			return false;
		}

		//! get mouse X delta (left-right)
		virtual float GetDeltaX() override
		{
			return 0.f;
		}

		//! get mouse Y delta (up-down)
		virtual float GetDeltaY() override
		{
			return 0.f;
		}

		//! get mouse Z delta (mouse wheel)
		virtual float GetDeltaZ() override
		{
			return 0.f;
		}

		//! set mouse inertia
		virtual void SetInertia(float) override
		{
		}

		//! set mouse X screen corrdinate
		virtual void SetVScreenX(float fX) override
		{
		}

		//! set mouse Y screen corrdinate
		virtual void SetVScreenY(float fY) override
		{
		}

		//! get mouse X screen corrdinate
		virtual float GetVScreenX() override
		{
			float x, y;
			gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x,&y);
			return x;
		}

		//! get mouse Y screen corrdinate
		virtual float GetVScreenY() override
		{
			float x, y;
			gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x,&y);
			return y;
		}

		//! set the mouse sensitivity
		virtual void SetSensitvity(float fSensitivity) override
		{
		}

		//! get the mouse sensitivity
		virtual float GetSensitvity() override
		{
			return 0.f;
		}

		//! set the mouse sensitivity scale (from 0 to 1)
		virtual void SetSensitvityScale(float fSensScale) override
		{
		}

		//! get the mouse sensitivity scale
		virtual float GetSensitvityScale() override
		{
			return 0.f;
		}

		//! clear the key states
		virtual void ClearKeyState() override
		{
			m_pMouse->ClearKeyState();
		}

		IInputDevice* m_pMouse;
	};

	class CInput : public IInput
	{
	  public:
		CInput(::IInput* pInput)
			: m_pInput(pInput)
		{
		}
		virtual void AddEventListener(IInputEventListener* pListener) override
		{
		}
		virtual void RemoveEventListener(IInputEventListener* pListener) override
		{
		}
		virtual void EnableEventPosting(bool bEnable) override
		{
		}
		virtual void AddConsoleEventListener(IInputEventListener* pListener) override
		{
		}
		virtual void RemoveConsoleEventListener(IInputEventListener* pListener) override
		{
		}
		virtual void SetExclusiveListener(IInputEventListener* pListener) override
		{
		}
		virtual IInputEventListener* GetExclusiveListener() override
		{
			return nullptr;
		}
		virtual void Update(bool bFocus) override
		{
		}
		virtual void ShutDown() override
		{
		}
		virtual void SetMouseExclusive(bool exclusive, void* hwnd = 0) override
		{
		}
		virtual void SetKeyboardExclusive(bool exclusive, void* hwnd = 0) override
		{
		}
		virtual bool KeyDown(int p_key) override
		{
			return m_Keyboard.KeyDown(p_key);
		}
		virtual bool KeyPressed(int p_key) override
		{
			return m_Keyboard.KeyPressed(p_key);
		}
		virtual bool KeyReleased(int p_key) override
		{
			return m_Keyboard.KeyReleased(p_key);
		}
		virtual bool MouseDown(int p_numButton) override
		{
			return m_Mouse.MouseDown(p_numButton);
		}
		virtual bool MousePressed(int p_numButton) override
		{
			return m_Mouse.MousePressed(p_numButton);
		}
		virtual bool MouseDblClick(int p_numButton) override
		{
			return false;
		}
		virtual bool MouseReleased(int p_numButton) override
		{
			return m_Mouse.MouseReleased(p_numButton);
		}
		virtual float MouseGetDeltaX() override
		{
			return 0.0f;
		}
		virtual float MouseGetDeltaY() override
		{
			return 0.0f;
		}
		virtual float MouseGetDeltaZ() override
		{
			return 0.0f;
		}
		virtual float MouseGetVScreenX() override
		{
			return m_Mouse.GetVScreenX();
		}
		virtual float MouseGetVScreenY() override
		{
			float x, y;
			gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x,&y);
			return y;
		}
		virtual int GetKeyID(const char* sName) override
		{
			return 0;
		}
		virtual void EnableBufferedInput(bool bEnable) override
		{
		}
		virtual void FeedVirtualKey(int nVirtualKey, long lParam, bool bDown) override
		{
		}
		virtual int GetBufferedKey() override
		{
			return 0;
		}
		virtual const char* GetBufferedKeyName() override
		{
			return nullptr;
		}
		virtual void PopBufferedKey() override
		{
		}
		virtual void SetMouseInertia(float) override
		{
		}
		virtual int JoyGetDefaultControllerId() const override
		{
			return 0;
		}
		virtual bool JoyIsRawBtnDown(int idCtrl, int p_numButton) override
		{
			return false;
		}
		virtual bool JoyIsRawBtnPressed(int idCtrl, int p_numButton) override
		{
			return false;
		}
		virtual bool JoyIsRawBtnReleased(int idCtrl, int p_numButton) override
		{
			return false;
		}
		virtual int JoyGetDir(int idCtrl) override
		{
			return 0;
		}
		virtual int JoyGetDirPressed(int idCtrl) override
		{
			return 0;
		}
		virtual int JoyGetDirReleased(int idCtrl) override
		{
			return 0;
		}
		virtual int JoyGetHatDir(int idCtrl) override
		{
			return 0;
		}
		virtual int JoyGetHatDirPressed(int idCtrl) override
		{
			return 0;
		}
		virtual int JoyGetHatDirReleased(int idCtrl) override
		{
			return 0;
		}
		virtual Legacy::Vec3 JoyGetAnalog1Dir(int idCtrl) const override
		{
			return Legacy::Vec3();
		}
		virtual Legacy::Vec3 JoyGetAnalog2Dir(int idCtrl) const override
		{
			return Legacy::Vec3();
		}
		virtual bool JoyIsXKeyPressed(int idCtrl, int idXKey) override
		{
			return false;
		}
		virtual bool JoyIsXKeyDown(int idCtrl, int idXKey) override
		{
			return false;
		}
		virtual bool JoyIsXKeyReleased(int idCtrl, int idXKey) override
		{
			return false;
		}
		virtual float GetJoySensitivityHGain(int idCtrl) override
		{
			return 0.0f;
		}
		virtual float GetJoySensitivityHScale(int idCtrl) override
		{
			return 0.0f;
		}
		virtual float GetJoySensitivityVGain(int idCtrl) override
		{
			return 0.0f;
		}
		virtual float GetJoySensitivityVScale(int idCtrl) override
		{
			return 0.0f;
		}
		virtual void SetJoySensitivityHGain(int idCtrl, float fHGain) override
		{
		}
		virtual void SetJoySensitivityHScale(int idCtrl, float fHScale) override
		{
		}
		virtual void SetJoySensitivityVGain(int idCtrl, float fVGain) override
		{
		}
		virtual void SetJoySensitivityVScale(int idCtrl, float fVScale) override
		{
		}
		virtual IKeyboard* GetIKeyboard() override
		{
			return &m_Keyboard;
		}
		virtual IMouse* GetIMouse() override
		{
			return &m_Mouse;
		}
		virtual const char* GetKeyName(int nKey, int modifiers = 0, bool bGUI = 0) override
		{
			return nullptr;
		}
		virtual bool GetOSKeyName(int nKey, wchar_t* szwKeyName, int iBufSize) override
		{
			return false;
		}
		virtual int GetKeyPressedCode() override
		{
			return 0;
		}
		virtual const char* GetKeyPressedName() override
		{
			return nullptr;
		}
		virtual int GetKeyDownCode() override
		{
			return 0;
		}
		virtual const char* GetKeyDownName() override
		{
			return nullptr;
		}
		virtual void WaitForKey() override
		{
		}
		virtual IActionMapManager* CreateActionMapManager() override
		{
			return nullptr;
		}
		virtual const char* GetXKeyPressedName() override
		{
			return nullptr;
		}
		virtual void ClearKeyState() override
		{
			m_Keyboard.ClearKeyState();
			m_Mouse.ClearKeyState();
		}
		virtual unsigned char GetKeyState(int nKey) override
		{
			return 0;
		}
		::IInput* m_pInput;
		CKeyboard m_Keyboard;
		CMouse	  m_Mouse;
	};
} // namespace Legacy
