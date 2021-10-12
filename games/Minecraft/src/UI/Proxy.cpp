Legacy::SInputEvent::EType getType(EInputState is)
{
	switch (is)
	{
	case eIS_Unknown:
		return Legacy::SInputEvent::UNKNOWN;
	case eIS_Pressed:
		return Legacy::SInputEvent::KEY_PRESS;
	case eIS_Released:
		return Legacy::SInputEvent::KEY_RELEASE;
	case eIS_Down:
		return Legacy::SInputEvent::KEY_PRESS;
	case eIS_Changed:
		return Legacy::SInputEvent::MOUSE_MOVE;
	default:
		return Legacy::SInputEvent::UNKNOWN;
	}
}

Legacy::KeyCodes convertKey(EKeyId k)
{
	using namespace Legacy;
	switch (k)
	{
	  case eKI_Escape: return XKEY_ESCAPE;
	  case eKI_1: return XKEY_1;
	  case eKI_2: return XKEY_2;
	  case eKI_3: return XKEY_3;
	  case eKI_4: return XKEY_4;
	  case eKI_5: return XKEY_5;
	  case eKI_6: return XKEY_6;
	  case eKI_7: return XKEY_7;
	  case eKI_8: return XKEY_8;
	  case eKI_9: return XKEY_9;
	  case eKI_0: return XKEY_0;
	  case eKI_Minus: return XKEY_MINUS;
	  case eKI_Equals: return XKEY_EQUALS;
	  case eKI_Backspace: return XKEY_BACKSPACE;
	  case eKI_Tab: return XKEY_TAB;
	  case eKI_Q: return XKEY_Q;
	  case eKI_W: return XKEY_W;
	  case eKI_E: return XKEY_E;
	  case eKI_R: return XKEY_R;
	  case eKI_T: return XKEY_T;
	  case eKI_Y: return XKEY_Y;
	  case eKI_U: return XKEY_U;
	  case eKI_I: return XKEY_I;
	  case eKI_O: return XKEY_O;
	  case eKI_P: return XKEY_P;
	  case eKI_LBracket: return XKEY_LBRACKET;
	  case eKI_RBracket: return XKEY_RBRACKET;
	  case eKI_Enter: return XKEY_RETURN;
	  case eKI_LCtrl: return XKEY_RCONTROL;
	  case eKI_A: return XKEY_A;
	  case eKI_S: return XKEY_S;
	  case eKI_D: return XKEY_D;
	  case eKI_F: return XKEY_F;
	  case eKI_G: return XKEY_G;
	  case eKI_H: return XKEY_H;
	  case eKI_J: return XKEY_J;
	  case eKI_K: return XKEY_K;
	  case eKI_L: return XKEY_L;
	  case eKI_Semicolon: return XKEY_SEMICOLON;
	  case eKI_Apostrophe: return XKEY_APOSTROPHE;
	  case eKI_Tilde: return XKEY_TILDE;
	  case eKI_LShift: return XKEY_LSHIFT;
	  case eKI_Backslash: return XKEY_BACKSLASH;
	  case eKI_Z: return XKEY_Z;
	  case eKI_X: return XKEY_X;
	  case eKI_C: return XKEY_C;
	  case eKI_V: return XKEY_V;
	  case eKI_B: return XKEY_B;
	  case eKI_N: return XKEY_N;
	  case eKI_M: return XKEY_M;
	  case eKI_Comma: return XKEY_COMMA;
	  case eKI_Period: return XKEY_PERIOD;
	  case eKI_Slash: return XKEY_SLASH;
	  case eKI_RShift: return XKEY_RSHIFT;
	  case eKI_NP_Multiply: return XKEY_MULTIPLY;
	  case eKI_LAlt: return XKEY_LALT;
	  case eKI_Space: return XKEY_SPACE;
	  case eKI_CapsLock: return XKEY_CAPSLOCK;
	  case eKI_F1: return XKEY_F1;
	  case eKI_F2: return XKEY_F2;
	  case eKI_F3: return XKEY_F3;
	  case eKI_F4: return XKEY_F4;
	  case eKI_F5: return XKEY_F5;
	  case eKI_F6: return XKEY_F6;
	  case eKI_F7: return XKEY_F7;
	  case eKI_F8: return XKEY_F8;
	  case eKI_F9: return XKEY_F9;
	  case eKI_F10: return XKEY_F10;
	  case eKI_NumLock: return XKEY_NUMLOCK;
	  case eKI_ScrollLock: return XKEY_SCROLLLOCK;
	  case eKI_NP_7: return XKEY_NUMPAD7;
	  case eKI_NP_8: return XKEY_NUMPAD8;
	  case eKI_NP_9: return XKEY_NUMPAD9;
	  case eKI_NP_Substract: return XKEY_SUBTRACT;
	  case eKI_NP_4: return XKEY_NUMPAD4;
	  case eKI_NP_5: return XKEY_NUMPAD5;
	  case eKI_NP_6: return XKEY_NUMPAD6;
	  case eKI_NP_Add: return XKEY_ADD;
	  case eKI_NP_1: return XKEY_NUMPAD1;
	  case eKI_NP_2: return XKEY_NUMPAD2;
	  case eKI_NP_3: return XKEY_NUMPAD3;
	  case eKI_NP_0: return XKEY_NUMPAD0;
	  case eKI_F11: return XKEY_F11;
	  case eKI_F12: return XKEY_F12;
	  case eKI_F13: return XKEY_F13;
	  case eKI_F14: return XKEY_F14;
	  case eKI_F15: return XKEY_F15;
		  //FIXME:
		  #if 0
	  case eKI_Colon: return XKEY_COLON;
	  case eKI_Underline: return XKEY_UNDERLINE;
		  #endif
	  case eKI_NP_Enter: return XKEY_NUMPADENTER;
	  case eKI_RCtrl: return XKEY_RCONTROL;
	  case eKI_NP_Period: return XKEY_PERIOD;
	  case eKI_NP_Divide: return XKEY_SLASH;
	  case eKI_Print: return XKEY_PRINT;
	  case eKI_RAlt: return XKEY_RALT;
	  case eKI_Pause: return XKEY_PAUSE;
	  case eKI_Home: return XKEY_HOME;
	  case eKI_Up: return XKEY_UP;
	  case eKI_PgUp: return XKEY_PAGE_UP;
	  case eKI_Left: return XKEY_LEFT;
	  case eKI_Right: return XKEY_RIGHT;
	  case eKI_End: return XKEY_END;
	  case eKI_Down: return XKEY_DOWN;
	  case eKI_PgDn: return XKEY_PAGE_DOWN;
	  case eKI_Insert: return XKEY_INSERT;
	  case eKI_Delete: return XKEY_DELETE;
	  case eKI_LWin: return XKEY_WIN_LWINDOW;
	  case eKI_RWin: return XKEY_WIN_RWINDOW;
	  case eKI_Apps: return XKEY_WIN_APPS;
	  case eKI_OEM_102: return XKEY_OEM_102;

	  // Mouse.
	  case eKI_Mouse1: return XKEY_MOUSE1;
	  case eKI_Mouse2: return XKEY_MOUSE2;
	  case eKI_Mouse3: return XKEY_MOUSE3;
	  case eKI_Mouse4: return XKEY_MOUSE4;
	  case eKI_Mouse5: return XKEY_MOUSE5;
	  case eKI_Mouse6: return XKEY_MOUSE6;
	  case eKI_Mouse7: return XKEY_MOUSE7;
	  case eKI_Mouse8: return XKEY_MOUSE8;
	  case eKI_MouseWheelUp: return XKEY_MWHEEL_UP;
	  case eKI_MouseWheelDown: return XKEY_MWHEEL_DOWN;
	  case eKI_MouseX: return XKEY_MAXIS_X;
	  case eKI_MouseY: return XKEY_MAXIS_Y;
		  //FIXME:
		  #if 0
	  case eKI_MouseZ: return XKEY_MOUSEZ;

	  case eKI_MouseXAbsolute: return XKEY_MOUSEXABSOLUTE;
	  case eKI_MouseYAbsolute: return XKEY_MOUSEYABSOLUTE;
		  #endif

		  //FIXME: support gamepad
		  #if 0
	  // XBox controller.
	  case eKI_XI_DPadUp: return XKEY_XI_DPADUP;
	  case eKI_XI_DPadDown: return XKEY_XI_DPADDOWN;
	  case eKI_XI_DPadLeft: return XKEY_XI_DPADLEFT;
	  case eKI_XI_DPadRight: return XKEY_XI_DPADRIGHT;
	  case eKI_XI_Start: return XKEY_XI_START;
	  case eKI_XI_Back: return XKEY_XI_BACK;
	  case eKI_XI_ThumbL: return XKEY_XI_THUMBL;
	  case eKI_XI_ThumbR: return XKEY_XI_THUMBR;
	  case eKI_XI_ShoulderL: return XKEY_XI_SHOULDERL;
	  case eKI_XI_ShoulderR: return XKEY_XI_SHOULDERR;
	  case eKI_XI_A: return XKEY_XI_A;
	  case eKI_XI_B: return XKEY_XI_B;
	  case eKI_XI_X: return XKEY_XI_X;
	  case eKI_XI_Y: return XKEY_XI_Y;
	  case eKI_XI_TriggerL: return XKEY_XI_TRIGGERL;
	  case eKI_XI_TriggerR: return XKEY_XI_TRIGGERR;
	  case eKI_XI_ThumbLX: return XKEY_XI_THUMBLX;
	  case eKI_XI_ThumbLY: return XKEY_XI_THUMBLY;
	  case eKI_XI_ThumbLUp: return XKEY_XI_THUMBLUP;
	  case eKI_XI_ThumbLDown: return XKEY_XI_THUMBLDOWN;
	  case eKI_XI_ThumbLLeft: return XKEY_XI_THUMBLLEFT;
	  case eKI_XI_ThumbLRight: return XKEY_XI_THUMBLRIGHT;
	  case eKI_XI_ThumbRX: return XKEY_XI_THUMBRX;
	  case eKI_XI_ThumbRY: return XKEY_XI_THUMBRY;
	  case eKI_XI_ThumbRUp: return XKEY_XI_THUMBRUP;
	  case eKI_XI_ThumbRDown: return XKEY_XI_THUMBRDOWN;
	  case eKI_XI_ThumbRLeft: return XKEY_XI_THUMBRLEFT;
	  case eKI_XI_ThumbRRight: return XKEY_XI_THUMBRRIGHT;
	  case eKI_XI_TriggerLBtn: return XKEY_XI_TRIGGERLBTN;
	  case eKI_XI_TriggerRBtn: return XKEY_XI_TRIGGERRBTN;
		  #endif

	  case eKI_Unknown:  return XKEY_NULL;
	default:
	  return XKEY_NULL;
		break;
	}
}

//FIXME: wring convertion
bool CUISystem::OnInputEvent(const SInputEvent& event)
{
	if (event.keyId == eKI_SYS_Commit)
		return false;
	Legacy::SInputEvent legacyEvent;
	auto&				l = legacyEvent;

	l.key		 = convertKey(event.keyId);
	l.keyname	 = event.keyName;
	l.moidifiers = event.modifiers;
	l.timestamp	 = 0;
	l.type		 = getType(event.state);
	l.value		 = event.value;

	return OnInputEvent(l);
}
