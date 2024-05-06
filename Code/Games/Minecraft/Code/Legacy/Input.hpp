#pragma once
#include "IInput.h"
#include <BlackBox\Input\IInput.hpp>

namespace Input
{
	EKeyId kconvertKey(Legacy::KeyCodes key);
} // namespace Input

inline Legacy::SInputEvent::EType getType(EInputState is)
{
	switch (is)
	{
	case eIS_Unknown:
		return Legacy::SInputEvent::UNKNOWN;
	case eIS_Pressed:
		return Legacy::SInputEvent::KEY_PRESS;
	case eIS_Released:
		return Legacy::SInputEvent::KEY_RELEASE;
	/*case eIS_Down:
		return Legacy::SInputEvent::KEY_PRESS;*/
	case eIS_Changed:
		return Legacy::SInputEvent::MOUSE_MOVE;
	default:
		return Legacy::SInputEvent::UNKNOWN;
	}
}

Legacy::KeyCodes convertKey(EKeyId k);

class ProxyListener : public IInputEventListener
{
public:
	using LegacyListener = Legacy::IInputEventListener;
	~ProxyListener();
	ProxyListener(LegacyListener* parent);
	virtual bool OnInputEvent(const SInputEvent& event) override;

private:
	LegacyListener* m_Parent;
};
namespace Legacy
{
	struct CKeyboard : public IKeyboard
	{
		EKeyId m_LastKey = eKI_Unknown;
		CKeyboard();
		virtual void ShutDown() override;

		//! check for key pressed and held
		virtual bool KeyDown(int p_key) override;

		//! check for key pressed only once
		virtual bool KeyPressed(int p_key) override;

		//! check if the key has been released
		virtual bool KeyReleased(int p_key) override;

		//! clear the key status
		virtual void ClearKey(int p_key) override;

		//! return the code of the key pressed
		virtual int GetKeyPressedCode() override;

		//! return the name of the key pressed
		virtual const char* GetKeyPressedName() override;

		//! return the code of the key down
		virtual int GetKeyDownCode() override;

		//! return the name of the key down
		virtual const char* GetKeyDownName() override;

		//! set/unset directinput to exclusive mode
		virtual void SetExclusive(bool value, void* hwnd = 0) override;

		//! wait for a key pressed
		virtual void WaitForKey() override;

		//! clear the key (pressed) state
		virtual void ClearKeyState() override;

		void Update();
		IInputDevice* m_pKeyboard{};
	};

	//////////////////////////////////////////////////////////////////////
	/*! Interface to the Mouse system.
*/
	struct CMouse : public IMouse
	{
		CMouse();
		virtual void Shutdown() override;

		//! check for a mouse button pressed and held
		virtual bool MouseDown(int p_numButton) override;

		//! check for a mouse button pressed only once
		virtual bool MousePressed(int p_numButton) override;

		//! check if the mouse button has been released
		virtual bool MouseReleased(int p_numButton) override;

		//! force the mouse wheel rotation to a certain value
		virtual void SetMouseWheelRotation(int value) override;

		//! set/reset Directinput to exclusive mode
		virtual bool SetExclusive(bool value, void* hwnd = 0) override;

		//! get mouse X delta (left-right)
		virtual float GetDeltaX() override;

		//! get mouse Y delta (up-down)
		virtual float GetDeltaY() override;

		//! get mouse Z delta (mouse wheel)
		virtual float GetDeltaZ() override;

		//! set mouse inertia
		virtual void SetInertia(float) override;

		//! set mouse X screen corrdinate
		virtual void SetVScreenX(float fX) override;

		//! set mouse Y screen corrdinate
		virtual void SetVScreenY(float fY) override;

		//! get mouse X screen corrdinate
		virtual float GetVScreenX() override;

		//! get mouse Y screen corrdinate
		virtual float GetVScreenY() override;

		//! set the mouse sensitivity
		virtual void SetSensitvity(float fSensitivity) override;

		//! get the mouse sensitivity
		virtual float GetSensitvity() override;

		//! set the mouse sensitivity scale (from 0 to 1)
		virtual void SetSensitvityScale(float fSensScale) override;

		//! get the mouse sensitivity scale
		virtual float GetSensitvityScale() override;

		//! clear the key states
		virtual void ClearKeyState() override;

		IInputDevice* m_pMouse;
	};

	class CInput : public IInput
	{
		using LegacyListener = ProxyListener;
		std::map<Legacy::IInputEventListener*, LegacyListener> m_Listeners;

	public:
		CInput(::IInput* pInput);
		virtual void AddEventListener(IInputEventListener* pListener) override;
		virtual void RemoveEventListener(IInputEventListener* pListener) override;
		virtual void EnableEventPosting(bool bEnable) override;
		virtual void AddConsoleEventListener(IInputEventListener* pListener) override;
		virtual void RemoveConsoleEventListener(IInputEventListener* pListener) override;
		virtual void SetExclusiveListener(IInputEventListener* pListener) override;
		virtual IInputEventListener* GetExclusiveListener() override;
		virtual void Update(bool bFocus) override;
		virtual void ShutDown() override;
		virtual void SetMouseExclusive(bool exclusive, void* hwnd = 0) override;
		virtual void SetKeyboardExclusive(bool exclusive, void* hwnd = 0) override;
		virtual bool KeyDown(int p_key) override;
		virtual bool KeyPressed(int p_key) override;
		virtual bool KeyReleased(int p_key) override;
		virtual bool MouseDown(int p_numButton) override;
		virtual bool MousePressed(int p_numButton) override;
		virtual bool MouseDblClick(int p_numButton) override;
		virtual bool MouseReleased(int p_numButton) override;
		virtual float MouseGetDeltaX() override;
		virtual float MouseGetDeltaY() override;
		virtual float MouseGetDeltaZ() override;
		virtual float MouseGetVScreenX() override;
		virtual float MouseGetVScreenY() override;
		virtual int GetKeyID(const char* sName) override;
		virtual void EnableBufferedInput(bool bEnable) override;
		virtual void FeedVirtualKey(int nVirtualKey, long lParam, bool bDown) override;
		virtual int GetBufferedKey() override;
		virtual const char* GetBufferedKeyName() override;
		virtual void PopBufferedKey() override;
		virtual void SetMouseInertia(float) override;
		virtual int JoyGetDefaultControllerId() const override;
		virtual bool JoyIsRawBtnDown(int idCtrl, int p_numButton) override;
		virtual bool JoyIsRawBtnPressed(int idCtrl, int p_numButton) override;
		virtual bool JoyIsRawBtnReleased(int idCtrl, int p_numButton) override;
		virtual int JoyGetDir(int idCtrl) override;
		virtual int JoyGetDirPressed(int idCtrl) override;
		virtual int JoyGetDirReleased(int idCtrl) override;
		virtual int JoyGetHatDir(int idCtrl) override;
		virtual int JoyGetHatDirPressed(int idCtrl) override;
		virtual int JoyGetHatDirReleased(int idCtrl) override;
		virtual Legacy::Vec3 JoyGetAnalog1Dir(int idCtrl) const override;
		virtual Legacy::Vec3 JoyGetAnalog2Dir(int idCtrl) const override;
		virtual bool JoyIsXKeyPressed(int idCtrl, int idXKey) override;
		virtual bool JoyIsXKeyDown(int idCtrl, int idXKey) override;
		virtual bool JoyIsXKeyReleased(int idCtrl, int idXKey) override;
		virtual float GetJoySensitivityHGain(int idCtrl) override;
		virtual float GetJoySensitivityHScale(int idCtrl) override;
		virtual float GetJoySensitivityVGain(int idCtrl) override;
		virtual float GetJoySensitivityVScale(int idCtrl) override;
		virtual void SetJoySensitivityHGain(int idCtrl, float fHGain) override;
		virtual void SetJoySensitivityHScale(int idCtrl, float fHScale) override;
		virtual void SetJoySensitivityVGain(int idCtrl, float fVGain) override;
		virtual void SetJoySensitivityVScale(int idCtrl, float fVScale) override;
		virtual IKeyboard* GetIKeyboard() override;
		virtual IMouse* GetIMouse() override;
		virtual const char* GetKeyName(int nKey, int modifiers = 0, bool bGUI = 0) override;
		virtual bool GetOSKeyName(int nKey, wchar_t* szwKeyName, int iBufSize) override;
		virtual int GetKeyPressedCode() override;
		virtual const char* GetKeyPressedName() override;
		virtual int GetKeyDownCode() override;
		virtual const char* GetKeyDownName() override;
		virtual void WaitForKey() override;
		virtual IActionMapManager* CreateActionMapManager() override;
		virtual const char* GetXKeyPressedName() override;
		virtual void ClearKeyState() override;
		virtual unsigned char GetKeyState(int nKey) override;
		::IInput* m_pInput;
		CKeyboard m_Keyboard;
		CMouse    m_Mouse;
	};
} // namespace Legacy
