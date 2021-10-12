#pragma once
#include "IInput.h"
#include <BlackBox\Input\IInput.hpp>

namespace Legacy
{
	struct CKeyboard : public IKeyboard
	{
		virtual void ShutDown() override
		{
		}

		//! check for key pressed and held
		virtual bool KeyDown(int p_key) override
		{
			return false;
		}

		//! check for key pressed only once
		virtual bool KeyPressed(int p_key) override
		{
			return false;
		}

		//! check if the key has been released
		virtual bool KeyReleased(int p_key) override
		{
			return false;
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
		}
	};

	//////////////////////////////////////////////////////////////////////
	/*! Interface to the Mouse system.
*/
	struct CMouse : public IMouse
	{
		virtual void Shutdown() override
		{
		}

		//! check for a mouse button pressed and held
		virtual bool MouseDown(int p_numButton) override
		{
			return true;
		}

		//! check for a mouse button pressed only once
		virtual bool MousePressed(int p_numButton) override
		{
			return true;
		}

		//! check if the mouse button has been released
		virtual bool MouseReleased(int p_numButton) override
		{
			return true;
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
		}
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
			return false;
		}
		virtual bool KeyPressed(int p_key) override
		{
			return false;
		}
		virtual bool KeyReleased(int p_key) override
		{
			return false;
		}
		virtual bool MouseDown(int p_numButton) override
		{
			return false;
		}
		virtual bool MousePressed(int p_numButton) override
		{
			return false;
		}
		virtual bool MouseDblClick(int p_numButton) override
		{
			return false;
		}
		virtual bool MouseReleased(int p_numButton) override
		{
			return false;
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
			float x, y;
			gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x,&y);
			return x;
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
