#include "stdafx.h"
#include "Input.hpp"

Legacy::CInput::CInput(::IInput* pInput)
	: m_pInput(pInput)
{
}

void Legacy::CInput::AddEventListener(IInputEventListener* pListener)
{
	LegacyListener listener(pListener);
	auto           result = m_Listeners.insert({ pListener, listener });
	if (result.second)
	{
		m_pInput->AddEventListener(&result.first->second);
	}
}

void Legacy::CInput::RemoveEventListener(IInputEventListener* pListener)
{
	if (auto it = m_Listeners.find(pListener); it != m_Listeners.end())
	{
		m_pInput->RemoveEventListener(&it->second);
		m_Listeners.erase(it);
	}
}

void Legacy::CInput::EnableEventPosting(bool bEnable)
{
}

void Legacy::CInput::AddConsoleEventListener(IInputEventListener* pListener)
{
}

void Legacy::CInput::RemoveConsoleEventListener(IInputEventListener* pListener)
{
}

void Legacy::CInput::SetExclusiveListener(IInputEventListener* pListener)
{
}

Legacy::IInputEventListener* Legacy::CInput::GetExclusiveListener()
{
	return nullptr;
}

void Legacy::CInput::Update(bool bFocus)
{
	m_Keyboard.Update();
}

void Legacy::CInput::ShutDown()
{
}

void Legacy::CInput::SetMouseExclusive(bool exclusive, void* hwnd)
{
}

void Legacy::CInput::SetKeyboardExclusive(bool exclusive, void* hwnd)
{
}

bool Legacy::CInput::KeyDown(int p_key)
{
	return m_Keyboard.KeyDown(p_key);
}

bool Legacy::CInput::KeyPressed(int p_key)
{
	return m_Keyboard.KeyPressed(p_key);
}

bool Legacy::CInput::KeyReleased(int p_key)
{
	return m_Keyboard.KeyReleased(p_key);
}

bool Legacy::CInput::MouseDown(int p_numButton)
{
	return m_Mouse.MouseDown(p_numButton);
}

bool Legacy::CInput::MousePressed(int p_numButton)
{
	return m_Mouse.MousePressed(p_numButton);
}

bool Legacy::CInput::MouseDblClick(int p_numButton)
{
	return false;
}

bool Legacy::CInput::MouseReleased(int p_numButton)
{
	return m_Mouse.MouseReleased(p_numButton);
}

float Legacy::CInput::MouseGetDeltaX()
{
	return 0.0f;
}

float Legacy::CInput::MouseGetDeltaY()
{
	return 0.0f;
}

float Legacy::CInput::MouseGetDeltaZ()
{
	return 0.0f;
}

float Legacy::CInput::MouseGetVScreenX()
{
	return m_Mouse.GetVScreenX();
}

float Legacy::CInput::MouseGetVScreenY()
{
	float x, y;
	Env::HardwareMouse()->GetHardwareMouseClientPosition(&x, &y);
	return y;
}

int Legacy::CInput::GetKeyID(const char* sName)
{
	return 0;
}

void Legacy::CInput::EnableBufferedInput(bool bEnable)
{
}

void Legacy::CInput::FeedVirtualKey(int nVirtualKey, long lParam, bool bDown)
{
}

int Legacy::CInput::GetBufferedKey()
{
	return 0;
}

const char* Legacy::CInput::GetBufferedKeyName()
{
	return nullptr;
}

void Legacy::CInput::PopBufferedKey()
{
}

void Legacy::CInput::SetMouseInertia(float)
{
}

int Legacy::CInput::JoyGetDefaultControllerId() const
{
	return 0;
}

bool Legacy::CInput::JoyIsRawBtnDown(int idCtrl, int p_numButton)
{
	return false;
}

bool Legacy::CInput::JoyIsRawBtnPressed(int idCtrl, int p_numButton)
{
	return false;
}

bool Legacy::CInput::JoyIsRawBtnReleased(int idCtrl, int p_numButton)
{
	return false;
}

int Legacy::CInput::JoyGetDir(int idCtrl)
{
	return 0;
}

int Legacy::CInput::JoyGetDirPressed(int idCtrl)
{
	return 0;
}

int Legacy::CInput::JoyGetDirReleased(int idCtrl)
{
	return 0;
}

int Legacy::CInput::JoyGetHatDir(int idCtrl)
{
	return 0;
}

int Legacy::CInput::JoyGetHatDirPressed(int idCtrl)
{
	return 0;
}

int Legacy::CInput::JoyGetHatDirReleased(int idCtrl)
{
	return 0;
}

Legacy::Vec3 Legacy::CInput::JoyGetAnalog1Dir(int idCtrl) const
{
	return Legacy::Vec3();
}

Legacy::Vec3 Legacy::CInput::JoyGetAnalog2Dir(int idCtrl) const
{
	return Legacy::Vec3();
}

bool Legacy::CInput::JoyIsXKeyPressed(int idCtrl, int idXKey)
{
	return false;
}

bool Legacy::CInput::JoyIsXKeyDown(int idCtrl, int idXKey)
{
	return false;
}

bool Legacy::CInput::JoyIsXKeyReleased(int idCtrl, int idXKey)
{
	return false;
}

float Legacy::CInput::GetJoySensitivityHGain(int idCtrl)
{
	return 0.0f;
}

float Legacy::CInput::GetJoySensitivityHScale(int idCtrl)
{
	return 0.0f;
}

float Legacy::CInput::GetJoySensitivityVGain(int idCtrl)
{
	return 0.0f;
}

float Legacy::CInput::GetJoySensitivityVScale(int idCtrl)
{
	return 0.0f;
}

void Legacy::CInput::SetJoySensitivityHGain(int idCtrl, float fHGain)
{
}

void Legacy::CInput::SetJoySensitivityHScale(int idCtrl, float fHScale)
{
}

void Legacy::CInput::SetJoySensitivityVGain(int idCtrl, float fVGain)
{
}

void Legacy::CInput::SetJoySensitivityVScale(int idCtrl, float fVScale)
{
}

Legacy::IKeyboard* Legacy::CInput::GetIKeyboard()
{
	return &m_Keyboard;
}

Legacy::IMouse* Legacy::CInput::GetIMouse()
{
	return &m_Mouse;
}

const char* Legacy::CInput::GetKeyName(int nKey, int modifiers, bool bGUI)
{
	return nullptr;
}

bool Legacy::CInput::GetOSKeyName(int nKey, wchar_t* szwKeyName, int iBufSize)
{
	return false;
}

int Legacy::CInput::GetKeyPressedCode()
{
	return 0;
}

const char* Legacy::CInput::GetKeyPressedName()
{
	return nullptr;
}

int Legacy::CInput::GetKeyDownCode()
{
	return 0;
}

const char* Legacy::CInput::GetKeyDownName()
{
	return nullptr;
}

void Legacy::CInput::WaitForKey()
{
}

Legacy::IActionMapManager* Legacy::CInput::CreateActionMapManager()
{
	return nullptr;
}

const char* Legacy::CInput::GetXKeyPressedName()
{
	return m_Keyboard.GetKeyPressedName();
}

void Legacy::CInput::ClearKeyState()
{
	m_Keyboard.ClearKeyState();
	m_Mouse.ClearKeyState();
}

unsigned char Legacy::CInput::GetKeyState(int nKey)
{
	return 0;
}

inline Legacy::CKeyboard::CKeyboard()
{
	m_pKeyboard = Env::Input()->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
}

inline void Legacy::CKeyboard::ShutDown()
{
}

//! check for key pressed and held

inline bool Legacy::CKeyboard::KeyDown(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	assert(key != nullptr);
	return m_pKeyboard->InputState(key, EInputState::eIS_Down);
}

//! check for key pressed only once

inline bool Legacy::CKeyboard::KeyPressed(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	return m_pKeyboard->InputState(key, EInputState::eIS_Pressed);
}

//! check if the key has been released

inline bool Legacy::CKeyboard::KeyReleased(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	return m_pKeyboard->InputState(key, EInputState::eIS_Released);
}

//! clear the key status

inline void Legacy::CKeyboard::ClearKey(int p_key)
{
}

//! return the code of the key pressed

inline int Legacy::CKeyboard::GetKeyPressedCode()
{
	return XKEY_NULL;
}

//! return the name of the key pressed

inline const char* Legacy::CKeyboard::GetKeyPressedName()
{
	auto name = m_pKeyboard->GetKeyName(m_LastKey);
	if (m_LastKey == eKI_Space)
	{
		name = "spacebar";
	}

	return name;
}

//! return the code of the key down

inline int Legacy::CKeyboard::GetKeyDownCode()
{
	return XKEY_NULL;
}

//! return the name of the key down

inline const char* Legacy::CKeyboard::GetKeyDownName()
{
	return "XKEY_NULL";
}

//! set/unset directinput to exclusive mode

inline void Legacy::CKeyboard::SetExclusive(bool value, void* hwnd)
{
}

//! wait for a key pressed

inline void Legacy::CKeyboard::WaitForKey()
{
}

//! clear the key (pressed) state

inline void Legacy::CKeyboard::ClearKeyState()
{
	m_pKeyboard->ClearKeyState();
}

inline void Legacy::CKeyboard::Update()
{
	m_LastKey = eKI_Unknown;
	for (auto key = KI_KEYBOARD_BASE; key < KI_MOUSE_BASE; key++)
	{
		const auto& symbol = m_pKeyboard->LookupSymbol(EKeyId(key));
		if (symbol && symbol->state == eIS_Pressed)
		{
			m_LastKey = EKeyId(key);
		}
	}
}
