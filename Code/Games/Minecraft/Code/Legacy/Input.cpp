#include "stdafx.h"
#include "Input.hpp"

float gLastMouseClickTime = 0.0f;
bool gDblClick = false;
Legacy::CInput::CInput(::IInput* pInput)
	: m_pInput(pInput)
{
	gLastMouseClickTime = Env::System()->GetITimer()->GetAsyncCurTime();
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
	gDblClick = false;


	float fTime = Env::System()->GetITimer()->GetAsyncCurTime();
	float fDiff = fTime - gLastMouseClickTime;
	if (m_Mouse.MouseDown(Legacy::XKEY_MOUSE1))
	{
		gLastMouseClickTime = fTime;
		CryLog("Pressed diff: %f", fDiff);
		if ((fDiff) < 0.5f)
		{
			gDblClick = true;
		}
	}
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
	return gDblClick;
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
	return m_Keyboard.m_pKeyboard->GetKeyName(Input::kconvertKey(Legacy::KeyCodes(nKey)));
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

Legacy::CKeyboard::CKeyboard()
{
	m_pKeyboard = Env::Input()->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
}

void Legacy::CKeyboard::ShutDown()
{
}

//! check for key pressed and held

bool Legacy::CKeyboard::KeyDown(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	if (key == nullptr)
	{
		CryError("Key not found: %d", p_key);
		return false;
	}
	assert(key != nullptr);
	return m_pKeyboard->InputState(key, EInputState::eIS_Down);
}

//! check for key pressed only once

bool Legacy::CKeyboard::KeyPressed(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	return m_pKeyboard->InputState(key, EInputState::eIS_Pressed);
}

//! check if the key has been released

bool Legacy::CKeyboard::KeyReleased(int p_key)
{
	auto key = m_pKeyboard->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_key));
	return m_pKeyboard->InputState(key, EInputState::eIS_Released);
}

//! clear the key status

void Legacy::CKeyboard::ClearKey(int p_key)
{
}

//! return the code of the key pressed

int Legacy::CKeyboard::GetKeyPressedCode()
{
	return XKEY_NULL;
}

//! return the name of the key pressed

#pragma optimize("", off)
const char* Legacy::CKeyboard::GetKeyPressedName()
{
	auto name = m_pKeyboard->GetKeyName(m_LastKey);
	switch (m_LastKey)
	{
	case eKI_Escape:
		name = "esc";
		break;
	case eKI_1:
		break;
	case eKI_2:
		break;
	case eKI_3:
		break;
	case eKI_4:
		break;
	case eKI_5:
		break;
	case eKI_6:
		break;
	case eKI_7:
		break;
	case eKI_8:
		break;
	case eKI_9:
		break;
	case eKI_0:
		break;
	case eKI_Minus:
		break;
	case eKI_Equals:
		break;
	case eKI_Backspace:
		break;
	case eKI_Tab:
		break;
	case eKI_Q:
		break;
	case eKI_W:
		break;
	case eKI_E:
		break;
	case eKI_R:
		break;
	case eKI_T:
		break;
	case eKI_Y:
		break;
	case eKI_U:
		break;
	case eKI_I:
		break;
	case eKI_O:
		break;
	case eKI_P:
		break;
	case eKI_LBracket:
		break;
	case eKI_RBracket:
		break;
	case eKI_Enter:
		break;
	case eKI_LCtrl:
		break;
	case eKI_A:
		break;
	case eKI_S:
		break;
	case eKI_D:
		break;
	case eKI_F:
		break;
	case eKI_G:
		break;
	case eKI_H:
		break;
	case eKI_J:
		break;
	case eKI_K:
		break;
	case eKI_L:
		break;
	case eKI_Semicolon:
		break;
	case eKI_Apostrophe:
		break;
	case eKI_Tilde:
		break;
	case eKI_LShift:
		break;
	case eKI_Backslash:
		break;
	case eKI_Z:
		break;
	case eKI_X:
		break;
	case eKI_C:
		break;
	case eKI_V:
		break;
	case eKI_B:
		break;
	case eKI_N:
		break;
	case eKI_M:
		break;
	case eKI_Comma:
		break;
	case eKI_Period:
		break;
	case eKI_Slash:
		break;
	case eKI_RShift:
		break;
	case eKI_NP_Multiply:
		break;
	case eKI_LAlt:
		break;
	case eKI_Space:
		name = "spacebar";
		break;
	case eKI_CapsLock:
		break;
	case eKI_F1:
		break;
	case eKI_F2:
		break;
	case eKI_F3:
		break;
	case eKI_F4:
		break;
	case eKI_F5:
		break;
	case eKI_F6:
		break;
	case eKI_F7:
		break;
	case eKI_F8:
		break;
	case eKI_F9:
		break;
	case eKI_F10:
		break;
	case eKI_NumLock:
		break;
	case eKI_ScrollLock:
		break;
	case eKI_NP_7:
		break;
	case eKI_NP_8:
		break;
	case eKI_NP_9:
		break;
	case eKI_NP_Substract:
		break;
	case eKI_NP_4:
		break;
	case eKI_NP_5:
		break;
	case eKI_NP_6:
		break;
	case eKI_NP_Add:
		break;
	case eKI_NP_1:
		break;
	case eKI_NP_2:
		break;
	case eKI_NP_3:
		break;
	case eKI_NP_0:
		break;
	case eKI_F11:
		break;
	case eKI_F12:
		break;
	case eKI_F13:
		break;
	case eKI_F14:
		break;
	case eKI_F15:
		break;
	case eKI_Colon:
		break;
	case eKI_Underline:
		break;
	case eKI_NP_Enter:
		break;
	case eKI_RCtrl:
		break;
	case eKI_NP_Period:
		break;
	case eKI_NP_Divide:
		break;
	case eKI_Print:
		break;
	case eKI_RAlt:
		break;
	case eKI_Pause:
		break;
	case eKI_Home:
		break;
	case eKI_Up:
		break;
	case eKI_PgUp:
		break;
	case eKI_Left:
		break;
	case eKI_Right:
		break;
	case eKI_End:
		break;
	case eKI_Down:
		break;
	case eKI_PgDn:
		break;
	case eKI_Insert:
		break;
	case eKI_Delete:
		break;
	case eKI_LWin:
		break;
	case eKI_RWin:
		break;
	case eKI_Apps:
		break;
	case eKI_OEM_102:
		break;
	case eKI_Mouse1:
		break;
	case eKI_Mouse2:
		break;
	case eKI_Mouse3:
		break;
	case eKI_Mouse4:
		break;
	case eKI_Mouse5:
		break;
	case eKI_Mouse6:
		break;
	case eKI_Mouse7:
		break;
	case eKI_Mouse8:
		break;
	case eKI_MouseWheelUp:
		break;
	case eKI_MouseWheelDown:
		break;
	case eKI_MouseX:
		break;
	case eKI_MouseY:
		break;
	case eKI_MouseZ:
		break;
	case eKI_MouseXAbsolute:
		break;
	case eKI_MouseYAbsolute:
		break;
	case eKI_MouseLast:
		break;
	case eKI_XI_DPadUp:
		break;
	case eKI_XI_DPadDown:
		break;
	case eKI_XI_DPadLeft:
		break;
	case eKI_XI_DPadRight:
		break;
	case eKI_XI_Start:
		break;
	case eKI_XI_Back:
		break;
	case eKI_XI_ThumbL:
		break;
	case eKI_XI_ThumbR:
		break;
	case eKI_XI_ShoulderL:
		break;
	case eKI_XI_ShoulderR:
		break;
	case eKI_XI_A:
		break;
	case eKI_XI_B:
		break;
	case eKI_XI_X:
		break;
	case eKI_XI_Y:
		break;
	case eKI_XI_TriggerL:
		break;
	case eKI_XI_TriggerR:
		break;
	case eKI_XI_ThumbLX:
		break;
	case eKI_XI_ThumbLY:
		break;
	case eKI_XI_ThumbLUp:
		break;
	case eKI_XI_ThumbLDown:
		break;
	case eKI_XI_ThumbLLeft:
		break;
	case eKI_XI_ThumbLRight:
		break;
	case eKI_XI_ThumbRX:
		break;
	case eKI_XI_ThumbRY:
		break;
	case eKI_XI_ThumbRUp:
		break;
	case eKI_XI_ThumbRDown:
		break;
	case eKI_XI_ThumbRLeft:
		break;
	case eKI_XI_ThumbRRight:
		break;
	case eKI_XI_TriggerLBtn:
		break;
	case eKI_XI_TriggerRBtn:
		break;
	case eKI_XI_Connect:
		break;
	case eKI_XI_Disconnect:
		break;
	case eKI_SYS_Commit:
		break;
	case eKI_SYS_ConnectDevice:
		break;
	case eKI_SYS_DisconnectDevice:
		break;
	case eKI_Unknown:
		break;
	default:
		break;
	}

	return name;
}

//! return the code of the key down

int Legacy::CKeyboard::GetKeyDownCode()
{
	return XKEY_NULL;
}

//! return the name of the key down

const char* Legacy::CKeyboard::GetKeyDownName()
{
	return "XKEY_NULL";
}

//! set/unset directinput to exclusive mode

void Legacy::CKeyboard::SetExclusive(bool value, void* hwnd)
{
}

//! wait for a key pressed

void Legacy::CKeyboard::WaitForKey()
{
}

//! clear the key (pressed) state

void Legacy::CKeyboard::ClearKeyState()
{
	m_pKeyboard->ClearKeyState();
}

void Legacy::CKeyboard::Update()
{
	m_LastKey = eKI_Unknown;
	for (auto key = KI_KEYBOARD_BASE; key < KI_MOUSE_BASE; key++)
	{
		const auto& symbol = m_pKeyboard->LookupSymbol(EKeyId(key));
		if (symbol)
		{
			if (symbol->state == eIS_Pressed || symbol->state == eIS_Down)
			{
				m_LastKey = EKeyId(key);
			}
		}
	}
}

Legacy::CMouse::CMouse()
{
	m_pMouse = Env::Input()->GetDevice(0, EInputDeviceType::eIDT_Mouse);
}

void Legacy::CMouse::Shutdown()
{
}

//! check for a mouse button pressed and held

bool Legacy::CMouse::MouseDown(int p_numButton)
{
	auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));
	return m_pMouse->InputState(key, EInputState::eIS_Down);
	return true;
}

//! check for a mouse button pressed only once

bool Legacy::CMouse::MousePressed(int p_numButton)
{
	auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));
	return m_pMouse->InputState(key, EInputState::eIS_Pressed);
}

//! check if the mouse button has been released

bool Legacy::CMouse::MouseReleased(int p_numButton)
{
	auto key = m_pMouse->GetKeyName(Input::kconvertKey((Legacy::KeyCodes)p_numButton));
	return m_pMouse->InputState(key, EInputState::eIS_Released);
}

//! force the mouse wheel rotation to a certain value

void Legacy::CMouse::SetMouseWheelRotation(int value)
{
}

//! set/reset Directinput to exclusive mode

bool Legacy::CMouse::SetExclusive(bool value, void* hwnd)
{
	return false;
}

//! get mouse X delta (left-right)

float Legacy::CMouse::GetDeltaX()
{
	return 0.f;
}

//! get mouse Y delta (up-down)

float Legacy::CMouse::GetDeltaY()
{
	return 0.f;
}

//! get mouse Z delta (mouse wheel)

float Legacy::CMouse::GetDeltaZ()
{
	return 0.f;
}

//! set mouse inertia

void Legacy::CMouse::SetInertia(float)
{
}

//! set mouse X screen corrdinate

void Legacy::CMouse::SetVScreenX(float fX)
{
}

//! set mouse Y screen corrdinate

void Legacy::CMouse::SetVScreenY(float fY)
{
}

//! get mouse X screen corrdinate

float Legacy::CMouse::GetVScreenX()
{
	float x, y;
	Env::HardwareMouse()->GetHardwareMouseClientPosition(&x, &y);
	return x / Env::Renderer()->ScaleCoordX(1.f);
}

//! get mouse Y screen corrdinate

float Legacy::CMouse::GetVScreenY()
{
	float x, y;
	Env::HardwareMouse()->GetHardwareMouseClientPosition(&x, &y);
	return y / Env::Renderer()->ScaleCoordY(1.f);
}

//! set the mouse sensitivity

void Legacy::CMouse::SetSensitvity(float fSensitivity)
{
}

//! get the mouse sensitivity

float Legacy::CMouse::GetSensitvity()
{
	return 0.f;
}

//! set the mouse sensitivity scale (from 0 to 1)

void Legacy::CMouse::SetSensitvityScale(float fSensScale)
{
}

//! get the mouse sensitivity scale

float Legacy::CMouse::GetSensitvityScale()
{
	return 0.f;
}

//! clear the key states

void Legacy::CMouse::ClearKeyState()
{
	m_pMouse->ClearKeyState();
}
