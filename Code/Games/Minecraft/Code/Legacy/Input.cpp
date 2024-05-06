#include "stdafx.h"
#include "Input.hpp"
namespace Unicode
{
	static inline wstring Convert(wstring& widt, const char*& narow)
	{
		::std::wstring wideWhat;
		{
			int convertResult = MultiByteToWideChar(CP_UTF8, 0, narow, (int)strlen(narow), NULL, 0);
			if (convertResult <= 0)
			{
				wideWhat = L"Exception occurred: Failure to convert its message text using MultiByteToWideChar: convertResult=";
				wideWhat += std::to_wstring(convertResult);
				wideWhat += L"  GetLastError()=";
				wideWhat += std::to_wstring(GetLastError());
			}
			else
			{
				wideWhat.resize(convertResult + 10);
				convertResult = MultiByteToWideChar(CP_UTF8, 0, narow, (int)strlen(narow), &wideWhat[0], (int)wideWhat.size());
			}
		}
		widt = wideWhat;
		return widt;
	}
} // namespace Unicode


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
	auto symbol = m_Keyboard.m_pKeyboard->LookupSymbol(EKeyId(nKey));
	if (!symbol)
	{
		return false;
	}
	// Create SInputEvent from symbol
	::SInputEvent event = {};
	event.keyId = symbol->keyId;
	event.state = symbol->state;
	//event.modifiers = symbol->modifiers;
	event.value = symbol->value;
	event.deviceIndex = symbol->deviceIndex;
	event.deviceType = symbol->deviceType;
	event.pSymbol = symbol;
	event.keyName = m_Keyboard.m_pKeyboard->GetKeyName(symbol->keyId);
	// Get OS key name
	
	auto result =  m_Keyboard.m_pKeyboard->GetOSKeyName(event);
	if (result)
	{
		std::wstring wide;
		Unicode::Convert(wide, result);
		wcscpy(szwKeyName, wide.c_str());
		return true;
	}
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
	//m_Keyboard.ClearKeyState();
	//m_Mouse.ClearKeyState();
	m_pInput->ClearKeyState();
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

Legacy::KeyCodes convertKey(EKeyId k)
{
	using namespace Legacy;
	switch (k)
	{
	case eKI_Escape:
		return XKEY_ESCAPE;
	case eKI_1:
		return XKEY_1;
	case eKI_2:
		return XKEY_2;
	case eKI_3:
		return XKEY_3;
	case eKI_4:
		return XKEY_4;
	case eKI_5:
		return XKEY_5;
	case eKI_6:
		return XKEY_6;
	case eKI_7:
		return XKEY_7;
	case eKI_8:
		return XKEY_8;
	case eKI_9:
		return XKEY_9;
	case eKI_0:
		return XKEY_0;
	case eKI_Minus:
		return XKEY_MINUS;
	case eKI_Equals:
		return XKEY_EQUALS;
	case eKI_Backspace:
		return XKEY_BACKSPACE;
	case eKI_Tab:
		return XKEY_TAB;
	case eKI_Q:
		return XKEY_Q;
	case eKI_W:
		return XKEY_W;
	case eKI_E:
		return XKEY_E;
	case eKI_R:
		return XKEY_R;
	case eKI_T:
		return XKEY_T;
	case eKI_Y:
		return XKEY_Y;
	case eKI_U:
		return XKEY_U;
	case eKI_I:
		return XKEY_I;
	case eKI_O:
		return XKEY_O;
	case eKI_P:
		return XKEY_P;
	case eKI_LBracket:
		return XKEY_LBRACKET;
	case eKI_RBracket:
		return XKEY_RBRACKET;
	case eKI_Enter:
		return XKEY_RETURN;
	case eKI_LCtrl:
		return XKEY_RCONTROL;
	case eKI_A:
		return XKEY_A;
	case eKI_S:
		return XKEY_S;
	case eKI_D:
		return XKEY_D;
	case eKI_F:
		return XKEY_F;
	case eKI_G:
		return XKEY_G;
	case eKI_H:
		return XKEY_H;
	case eKI_J:
		return XKEY_J;
	case eKI_K:
		return XKEY_K;
	case eKI_L:
		return XKEY_L;
	case eKI_Semicolon:
		return XKEY_SEMICOLON;
	case eKI_Apostrophe:
		return XKEY_APOSTROPHE;
	case eKI_Tilde:
		return XKEY_TILDE;
	case eKI_LShift:
		return XKEY_LSHIFT;
	case eKI_Backslash:
		return XKEY_BACKSLASH;
	case eKI_Z:
		return XKEY_Z;
	case eKI_X:
		return XKEY_X;
	case eKI_C:
		return XKEY_C;
	case eKI_V:
		return XKEY_V;
	case eKI_B:
		return XKEY_B;
	case eKI_N:
		return XKEY_N;
	case eKI_M:
		return XKEY_M;
	case eKI_Comma:
		return XKEY_COMMA;
	case eKI_Period:
		return XKEY_PERIOD;
	case eKI_Slash:
		return XKEY_SLASH;
	case eKI_RShift:
		return XKEY_RSHIFT;
	case eKI_NP_Multiply:
		return XKEY_MULTIPLY;
	case eKI_LAlt:
		return XKEY_LALT;
	case eKI_Space:
		return XKEY_SPACE;
	case eKI_CapsLock:
		return XKEY_CAPSLOCK;
	case eKI_F1:
		return XKEY_F1;
	case eKI_F2:
		return XKEY_F2;
	case eKI_F3:
		return XKEY_F3;
	case eKI_F4:
		return XKEY_F4;
	case eKI_F5:
		return XKEY_F5;
	case eKI_F6:
		return XKEY_F6;
	case eKI_F7:
		return XKEY_F7;
	case eKI_F8:
		return XKEY_F8;
	case eKI_F9:
		return XKEY_F9;
	case eKI_F10:
		return XKEY_F10;
	case eKI_NumLock:
		return XKEY_NUMLOCK;
	case eKI_ScrollLock:
		return XKEY_SCROLLLOCK;
	case eKI_NP_7:
		return XKEY_NUMPAD7;
	case eKI_NP_8:
		return XKEY_NUMPAD8;
	case eKI_NP_9:
		return XKEY_NUMPAD9;
	case eKI_NP_Substract:
		return XKEY_SUBTRACT;
	case eKI_NP_4:
		return XKEY_NUMPAD4;
	case eKI_NP_5:
		return XKEY_NUMPAD5;
	case eKI_NP_6:
		return XKEY_NUMPAD6;
	case eKI_NP_Add:
		return XKEY_ADD;
	case eKI_NP_1:
		return XKEY_NUMPAD1;
	case eKI_NP_2:
		return XKEY_NUMPAD2;
	case eKI_NP_3:
		return XKEY_NUMPAD3;
	case eKI_NP_0:
		return XKEY_NUMPAD0;
	case eKI_F11:
		return XKEY_F11;
	case eKI_F12:
		return XKEY_F12;
	case eKI_F13:
		return XKEY_F13;
	case eKI_F14:
		return XKEY_F14;
	case eKI_F15:
		return XKEY_F15;
		//FIXME:
#if 0
	case eKI_Colon: return XKEY_COLON;
	case eKI_Underline: return XKEY_UNDERLINE;
#endif
	case eKI_NP_Enter:
		return XKEY_NUMPADENTER;
	case eKI_RCtrl:
		return XKEY_RCONTROL;
	case eKI_NP_Period:
		return XKEY_PERIOD;
	case eKI_NP_Divide:
		return XKEY_SLASH;
	case eKI_Print:
		return XKEY_PRINT;
	case eKI_RAlt:
		return XKEY_RALT;
	case eKI_Pause:
		return XKEY_PAUSE;
	case eKI_Home:
		return XKEY_HOME;
	case eKI_Up:
		return XKEY_UP;
	case eKI_PgUp:
		return XKEY_PAGE_UP;
	case eKI_Left:
		return XKEY_LEFT;
	case eKI_Right:
		return XKEY_RIGHT;
	case eKI_End:
		return XKEY_END;
	case eKI_Down:
		return XKEY_DOWN;
	case eKI_PgDn:
		return XKEY_PAGE_DOWN;
	case eKI_Insert:
		return XKEY_INSERT;
	case eKI_Delete:
		return XKEY_DELETE;
	case eKI_LWin:
		return XKEY_WIN_LWINDOW;
	case eKI_RWin:
		return XKEY_WIN_RWINDOW;
	case eKI_Apps:
		return XKEY_WIN_APPS;
	case eKI_OEM_102:
		return XKEY_OEM_102;

		// Mouse.
	case eKI_Mouse1:
		return XKEY_MOUSE1;
	case eKI_Mouse2:
		return XKEY_MOUSE2;
	case eKI_Mouse3:
		return XKEY_MOUSE3;
	case eKI_Mouse4:
		return XKEY_MOUSE4;
	case eKI_Mouse5:
		return XKEY_MOUSE5;
	case eKI_Mouse6:
		return XKEY_MOUSE6;
	case eKI_Mouse7:
		return XKEY_MOUSE7;
	case eKI_Mouse8:
		return XKEY_MOUSE8;
	case eKI_MouseWheelUp:
		return XKEY_MWHEEL_UP;
	case eKI_MouseWheelDown:
		return XKEY_MWHEEL_DOWN;
	case eKI_MouseX:
		return XKEY_MAXIS_X;
	case eKI_MouseY:
		return XKEY_MAXIS_Y;
		//FIXME:
#if 0
	case eKI_MouseZ: return XKEY_MOUSEZ;

	case eKI_MouseXAbsolute: return XKEY_MOUSEXABSOLUTE;
	case eKI_MouseYAbsolute: return XKEY_MOUSEYABSOLUTE;
#endif

		//FIXME: support gamepad
#if 1
		// XBox controller.
	case eKI_XI_DPadUp: return XKEY_GP_DPAD_UP;
	case eKI_XI_DPadDown: return XKEY_GP_DPAD_DOWN;
	case eKI_XI_DPadLeft: return XKEY_GP_DPAD_LEFT;
	case eKI_XI_DPadRight: return XKEY_GP_DPAD_RIGHT;
	case eKI_XI_Start: return XKEY_GP_START;
	case eKI_XI_Back: return XKEY_GP_BACK;
	case eKI_XI_ThumbL: return XKEY_GP_LEFT_THUMB;
	case eKI_XI_ThumbR: return XKEY_GP_RIGHT_THUMB;
#if 0
	case eKI_XI_ShoulderL: return XKEY_GP_SHOULDERL;
	case eKI_XI_ShoulderR: return XKEY_GP_SHOULDERR;
#endif
	case eKI_XI_A: return XKEY_GP_A;
	case eKI_XI_B: return XKEY_GP_B;
	case eKI_XI_X: return XKEY_GP_X;
	case eKI_XI_Y: return XKEY_GP_Y;
	case eKI_XI_TriggerL: return XKEY_GP_LEFT_TRIGGER;
	case eKI_XI_TriggerR: return XKEY_GP_RIGHT_TRIGGER;
	case eKI_XI_ThumbLX: return XKEY_GP_STHUMBLX;
	case eKI_XI_ThumbLY: return XKEY_GP_STHUMBLY;
	case eKI_XI_ThumbLUp: return XKEY_GP_STHUMBLUP;
	case eKI_XI_ThumbLDown: return XKEY_GP_STHUMBLDOWN;
	case eKI_XI_ThumbLLeft: return XKEY_GP_STHUMBLLEFT;
	case eKI_XI_ThumbLRight: return XKEY_GP_STHUMBLRIGHT;
	case eKI_XI_ThumbRX: return XKEY_GP_STHUMBRX;
	case eKI_XI_ThumbRY: return XKEY_GP_STHUMBRY;
#if 0
	case eKI_XI_ThumbRUp: return XKEY_GP_THUMBRUP;
	case eKI_XI_ThumbRDown: return XKEY_GP_THUMBRDOWN;
	case eKI_XI_ThumbRLeft: return XKEY_GP_THUMBRLEFT;
	case eKI_XI_ThumbRRight: return XKEY_GP_THUMBRRIGHT;
	case eKI_XI_TriggerLBtn: return XKEY_GP_TRIGGERLBTN;
	case eKI_XI_TriggerRBtn: return XKEY_GP_TRIGGERRBTN;
#endif
#endif

	case eKI_Unknown:
		return XKEY_NULL;
	default:
		return XKEY_NULL;
		break;
	}
}

inline ProxyListener::~ProxyListener() {}

inline ProxyListener::ProxyListener(LegacyListener* parent)
	: m_Parent(parent)
{
}

inline bool ProxyListener::OnInputEvent(const SInputEvent& event)
{
	using namespace Legacy;
	if (event.keyId == eKI_SYS_Commit)
		return false;
	Legacy::SInputEvent legacyEvent;
	auto& l = legacyEvent;

	l.key = convertKey(event.keyId);
	l.keyname = event.keyName;
	l.moidifiers = event.modifiers;
	l.timestamp = 0;
	l.type = getType(event.state);
	l.value = event.value;

	if (l.key == XKEY_GP_A)
	{
		l.key = XKEY_RETURN;
	}
	if (event.state != eIS_Unknown)
	{
		if (l.key == XKEY_MWHEEL_DOWN || l.key == XKEY_MWHEEL_UP)
		{
			l.type = Legacy::SInputEvent::KEY_PRESS;
		}
	}

	return m_Parent->OnInputEvent(l);
}

EKeyId Input::kconvertKey(Legacy::KeyCodes key)
{
	using namespace Legacy;
	switch (key)
	{
	case XKEY_ESCAPE:
		return eKI_Escape;
	case XKEY_1:
		return eKI_1;
	case XKEY_2:
		return eKI_2;
	case XKEY_3:
		return eKI_3;
	case XKEY_4:
		return eKI_4;
	case XKEY_5:
		return eKI_5;
	case XKEY_6:
		return eKI_6;
	case XKEY_7:
		return eKI_7;
	case XKEY_8:
		return eKI_8;
	case XKEY_9:
		return eKI_9;
	case XKEY_0:
		return eKI_0;
	case XKEY_MINUS:
		return eKI_Minus;
	case XKEY_EQUALS:
		return eKI_Equals;
	case XKEY_BACKSPACE:
		return eKI_Backspace;
	case XKEY_TAB:
		return eKI_Tab;
	case XKEY_Q:
		return eKI_Q;
	case XKEY_W:
		return eKI_W;
	case XKEY_E:
		return eKI_E;
	case XKEY_R:
		return eKI_R;
	case XKEY_T:
		return eKI_T;
	case XKEY_Y:
		return eKI_Y;
	case XKEY_U:
		return eKI_U;
	case XKEY_I:
		return eKI_I;
	case XKEY_O:
		return eKI_O;
	case XKEY_P:
		return eKI_P;
	case XKEY_LBRACKET:
		return eKI_LBracket;
	case XKEY_RBRACKET:
		return eKI_RBracket;
	case XKEY_RETURN:
		return eKI_Enter;
	case XKEY_RCONTROL:
		return eKI_LCtrl;
	case XKEY_A:
		return eKI_A;
	case XKEY_S:
		return eKI_S;
	case XKEY_D:
		return eKI_D;
	case XKEY_F:
		return eKI_F;
	case XKEY_G:
		return eKI_G;
	case XKEY_H:
		return eKI_H;
	case XKEY_J:
		return eKI_J;
	case XKEY_K:
		return eKI_K;
	case XKEY_L:
		return eKI_L;
	case XKEY_SEMICOLON:
		return eKI_Semicolon;
	case XKEY_APOSTROPHE:
		return eKI_Apostrophe;
	case XKEY_TILDE:
		return eKI_Tilde;
	case XKEY_LSHIFT:
		return eKI_LShift;
	case XKEY_BACKSLASH:
		return eKI_Backslash;
	case XKEY_Z:
		return eKI_Z;
	case XKEY_X:
		return eKI_X;
	case XKEY_C:
		return eKI_C;
	case XKEY_V:
		return eKI_V;
	case XKEY_B:
		return eKI_B;
	case XKEY_N:
		return eKI_N;
	case XKEY_M:
		return eKI_M;
	case XKEY_COMMA:
		return eKI_Comma;
	case XKEY_PERIOD:
		return eKI_Period;
	case XKEY_SLASH:
		return eKI_Slash;
	case XKEY_RSHIFT:
		return eKI_RShift;
	case XKEY_MULTIPLY:
		return eKI_NP_Multiply;
	case XKEY_LALT:
		return eKI_LAlt;
	case XKEY_SPACE:
		return eKI_Space;
	case XKEY_CAPSLOCK:
		return eKI_CapsLock;
	case XKEY_F1:
		return eKI_F1;
	case XKEY_F2:
		return eKI_F2;
	case XKEY_F3:
		return eKI_F3;
	case XKEY_F4:
		return eKI_F4;
	case XKEY_F5:
		return eKI_F5;
	case XKEY_F6:
		return eKI_F6;
	case XKEY_F7:
		return eKI_F7;
	case XKEY_F8:
		return eKI_F8;
	case XKEY_F9:
		return eKI_F9;
	case XKEY_F10:
		return eKI_F10;
	case XKEY_NUMLOCK:
		return eKI_NumLock;
	case XKEY_SCROLLLOCK:
		return eKI_ScrollLock;
	case XKEY_NUMPAD7:
		return eKI_NP_7;
	case XKEY_NUMPAD8:
		return eKI_NP_8;
	case XKEY_NUMPAD9:
		return eKI_NP_9;
	case XKEY_SUBTRACT:
		return eKI_NP_Substract;
	case XKEY_NUMPAD4:
		return eKI_NP_4;
	case XKEY_NUMPAD5:
		return eKI_NP_5;
	case XKEY_NUMPAD6:
		return eKI_NP_6;
	case XKEY_ADD:
		return eKI_NP_Add;
	case XKEY_NUMPAD1:
		return eKI_NP_1;
	case XKEY_NUMPAD2:
		return eKI_NP_2;
	case XKEY_NUMPAD3:
		return eKI_NP_3;
	case XKEY_NUMPAD0:
		return eKI_NP_0;
	case XKEY_F11:
		return eKI_F11;
	case XKEY_F12:
		return eKI_F12;
	case XKEY_F13:
		return eKI_F13;
	case XKEY_F14:
		return eKI_F14;
	case XKEY_F15:
		return eKI_F15;
		//FIXME:
#if 0
	case XKEY_COLON: return eKI_Colon;
	case XKEY_UNDERLINE: return eKI_Underline;
#endif
	case XKEY_NUMPADENTER:
		return eKI_NP_Enter;
		//case XKEY_RCONTROL: return eKI_RCtrl;
		//case XKEY_PERIOD: return eKI_NP_Period;
		//case XKEY_SLASH: return eKI_NP_Divide;
	case XKEY_PRINT:
		return eKI_Print;
	case XKEY_RALT:
		return eKI_RAlt;
	case XKEY_PAUSE:
		return eKI_Pause;
	case XKEY_HOME:
		return eKI_Home;
	case XKEY_UP:
		return eKI_Up;
	case XKEY_PAGE_UP:
		return eKI_PgUp;
	case XKEY_LEFT:
		return eKI_Left;
	case XKEY_RIGHT:
		return eKI_Right;
	case XKEY_END:
		return eKI_End;
	case XKEY_DOWN:
		return eKI_Down;
	case XKEY_PAGE_DOWN:
		return eKI_PgDn;
	case XKEY_INSERT:
		return eKI_Insert;
	case XKEY_DELETE:
		return eKI_Delete;
	case XKEY_WIN_LWINDOW:
		return eKI_LWin;
	case XKEY_WIN_RWINDOW:
		return eKI_RWin;
	case XKEY_WIN_APPS:
		return eKI_Apps;
	case XKEY_OEM_102:
		return eKI_OEM_102;

		// Mouse.
	case XKEY_MOUSE1:
		return eKI_Mouse1;
	case XKEY_MOUSE2:
		return eKI_Mouse2;
	case XKEY_MOUSE3:
		return eKI_Mouse3;
	case XKEY_MOUSE4:
		return eKI_Mouse4;
	case XKEY_MOUSE5:
		return eKI_Mouse5;
	case XKEY_MOUSE6:
		return eKI_Mouse6;
	case XKEY_MOUSE7:
		return eKI_Mouse7;
	case XKEY_MOUSE8:
		return eKI_Mouse8;
	case XKEY_MWHEEL_UP:
		return eKI_MouseWheelUp;
	case XKEY_MWHEEL_DOWN:
		return eKI_MouseWheelDown;
	case XKEY_MAXIS_X:
		return eKI_MouseX;
	case XKEY_MAXIS_Y:
		return eKI_MouseY;
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

	case XKEY_NULL:
		return eKI_Unknown;
	default:
		return eKI_Unknown;
		break;
	}
}
