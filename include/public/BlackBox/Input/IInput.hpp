#ifndef __IINPUT_H__
#define __IINPUT_H__

#ifdef INPUT_EXPORTS
	#define INPUT_API DLL_EXPORT
#else
	#define INPUT_API DLL_IMPORT
#endif
struct ISystem;

#include <cstdint>
#include <cstring>
#include <vector>

#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Core/Platform/Platform.hpp>

enum EModifierMask
{
  eMM_None = 0,
  eMM_LCtrl = (1 << 0),
  eMM_LShift = (1 << 1),
  eMM_LAlt = (1 << 2),
  eMM_LWin = (1 << 3),
  eMM_RCtrl = (1 << 4),
  eMM_RShift = (1 << 5),
  eMM_RAlt = (1 << 6),
  eMM_RWin = (1 << 7),
  eMM_NumLock = (1 << 8),
  eMM_CapsLock = (1 << 9),
  eMM_ScrollLock = (1 << 10),

  eMM_Ctrl = (eMM_LCtrl | eMM_RCtrl),
  eMM_Shift = (eMM_LShift | eMM_RShift),
  eMM_Alt = (eMM_LAlt | eMM_RAlt),
  eMM_Win = (eMM_LWin | eMM_RWin),
  eMM_Modifiers = (eMM_Ctrl | eMM_Shift | eMM_Alt | eMM_Win),
  eMM_LockKeys = (eMM_CapsLock | eMM_NumLock | eMM_ScrollLock)
};

static const int32_t EFF_INVALID_DEVICE_INDEX = -1;

enum EFFEffectId
{
  eFF_Rumble_Basic = 0,
  eFF_Rumble_Frame
};

//! Input Event types.
enum EInputState
{
  eIS_Unknown = 0,
  eIS_Pressed = (1 << 0),
  eIS_Released = (1 << 1),
  eIS_Down = (1 << 2),
  eIS_Changed = (1 << 3),
};

enum EInputDeviceType
{
  eIDT_Keyboard,
  eIDT_Mouse,
  eIDT_Joystick,
  eIDT_EyeTracker,
  eIDT_Gamepad,
  eIDT_MotionController,
  eIDT_Unknown = 0xff,
};

enum EInputPlatformFlags
{
  eIPF_NONE = 0,
  eIPF_SwapFrontEndForwardAndBack = (1 << 0),
};

struct TKeyName
{
  const char* key;

  TKeyName() { key = ""; }
  TKeyName(const char* _key) { key = _key; };
  operator const char* () const { return key; };
  bool        operator<(const TKeyName& n) const { return stricmp(key, n.key) < 0; }
  bool        operator>(const TKeyName& n) const { return stricmp(key, n.key) > 0; }
  bool        operator==(const TKeyName& n) const { return stricmp(key, n.key) == 0; }
  bool        operator!=(const TKeyName& n) const { return stricmp(key, n.key) != 0; }
  bool        operator<(const char* str) const { return stricmp(key, str) < 0; }
  bool        operator>(const char* str) const { return stricmp(key, str) > 0; }
  bool        operator==(const char* str) const { return stricmp(key, str) == 0; }
  bool        operator!=(const char* str) const { return stricmp(key, str) != 0; }
  const char* c_str() const { return key; }
};
inline bool   operator==(const char* str, const TKeyName& n) { return n == str; }
inline bool   operator!=(const char* str, const TKeyName& n) { return n != str; }
inline bool   operator<(const char* str, const TKeyName& n) { return n < str; }
inline bool   operator>(const char* str, const TKeyName& n) { return n > str; }

#define KI_KEYBOARD_BASE   0
#define KI_MOUSE_BASE      256
#define KI_XINPUT_BASE     512
#define KI_ORBIS_BASE      1024
#define KI_MOTION_BASE     2048
#define KI_EYETRACKER_BASE 4096
#define KI_SYS_BASE        8192

enum EKeyId : uint32_t
{
  eKI_Escape = KI_KEYBOARD_BASE,
  eKI_1,
  eKI_2,
  eKI_3,
  eKI_4,
  eKI_5,
  eKI_6,
  eKI_7,
  eKI_8,
  eKI_9,
  eKI_0,
  eKI_Minus,
  eKI_Equals,
  eKI_Backspace,
  eKI_Tab,
  eKI_Q,
  eKI_W,
  eKI_E,
  eKI_R,
  eKI_T,
  eKI_Y,
  eKI_U,
  eKI_I,
  eKI_O,
  eKI_P,
  eKI_LBracket,
  eKI_RBracket,
  eKI_Enter,
  eKI_LCtrl,
  eKI_A,
  eKI_S,
  eKI_D,
  eKI_F,
  eKI_G,
  eKI_H,
  eKI_J,
  eKI_K,
  eKI_L,
  eKI_Semicolon,
  eKI_Apostrophe,
  eKI_Tilde,
  eKI_LShift,
  eKI_Backslash,
  eKI_Z,
  eKI_X,
  eKI_C,
  eKI_V,
  eKI_B,
  eKI_N,
  eKI_M,
  eKI_Comma,
  eKI_Period,
  eKI_Slash,
  eKI_RShift,
  eKI_NP_Multiply,
  eKI_LAlt,
  eKI_Space,
  eKI_CapsLock,
  eKI_F1,
  eKI_F2,
  eKI_F3,
  eKI_F4,
  eKI_F5,
  eKI_F6,
  eKI_F7,
  eKI_F8,
  eKI_F9,
  eKI_F10,
  eKI_NumLock,
  eKI_ScrollLock,
  eKI_NP_7,
  eKI_NP_8,
  eKI_NP_9,
  eKI_NP_Substract,
  eKI_NP_4,
  eKI_NP_5,
  eKI_NP_6,
  eKI_NP_Add,
  eKI_NP_1,
  eKI_NP_2,
  eKI_NP_3,
  eKI_NP_0,
  eKI_F11,
  eKI_F12,
  eKI_F13,
  eKI_F14,
  eKI_F15,
  eKI_Colon,
  eKI_Underline,
  eKI_NP_Enter,
  eKI_RCtrl,
  eKI_NP_Period,
  eKI_NP_Divide,
  eKI_Print,
  eKI_RAlt,
  eKI_Pause,
  eKI_Home,
  eKI_Up,
  eKI_PgUp,
  eKI_Left,
  eKI_Right,
  eKI_End,
  eKI_Down,
  eKI_PgDn,
  eKI_Insert,
  eKI_Delete,
  eKI_LWin,
  eKI_RWin,
  eKI_Apps,
  eKI_OEM_102,

  // Mouse.
  eKI_Mouse1 = KI_MOUSE_BASE,
  eKI_Mouse2,
  eKI_Mouse3,
  eKI_Mouse4,
  eKI_Mouse5,
  eKI_Mouse6,
  eKI_Mouse7,
  eKI_Mouse8,
  eKI_MouseWheelUp,
  eKI_MouseWheelDown,
  eKI_MouseX,
  eKI_MouseY,
  eKI_MouseZ,
  eKI_MouseXAbsolute,
  eKI_MouseYAbsolute,
  eKI_MouseLast,

  // XBox controller.
  eKI_XI_DPadUp = KI_XINPUT_BASE,
  eKI_XI_DPadDown,
  eKI_XI_DPadLeft,
  eKI_XI_DPadRight,
  eKI_XI_Start,
  eKI_XI_Back,
  eKI_XI_ThumbL,
  eKI_XI_ThumbR,
  eKI_XI_ShoulderL,
  eKI_XI_ShoulderR,
  eKI_XI_A,
  eKI_XI_B,
  eKI_XI_X,
  eKI_XI_Y,
  eKI_XI_TriggerL,
  eKI_XI_TriggerR,
  eKI_XI_ThumbLX,
  eKI_XI_ThumbLY,
  eKI_XI_ThumbLUp,
  eKI_XI_ThumbLDown,
  eKI_XI_ThumbLLeft,
  eKI_XI_ThumbLRight,
  eKI_XI_ThumbRX,
  eKI_XI_ThumbRY,
  eKI_XI_ThumbRUp,
  eKI_XI_ThumbRDown,
  eKI_XI_ThumbRLeft,
  eKI_XI_ThumbRRight,
  eKI_XI_TriggerLBtn,
  eKI_XI_TriggerRBtn,
  eKI_XI_Connect,    //!< Should be deprecated because all devices can be connected, use eKI_SYS_ConnectDevice instead.
  eKI_XI_Disconnect, //!< Should be deprecated because all devices can be disconnected, use eKI_SYS_DisconnectDevice instead.

  // Normal inputs should be added above
  // eKI_SYS_Commit and below will be ignored by input blocking functionality
  eKI_SYS_Commit = KI_SYS_BASE,
  eKI_SYS_ConnectDevice,
  eKI_SYS_DisconnectDevice,

  // Terminator.
  eKI_Unknown = 0xffffffff,
};

struct STouchEvent
{
  EInputDeviceType deviceType;
  uint8_t          deviceIndex;
  Vec2             pos;
  uint8_t					 id;
};

struct ITouchEventListener
{
  virtual ~ITouchEventListener() {}
  virtual void OnTouchEvent(const STouchEvent& event) = 0;
};

//////////////////////////////////////////////////////////////////////

typedef int XACTIONID;

#ifdef _XBOX
#define MAX_BINDS_PER_ACTION 3
#else
#define MAX_BINDS_PER_ACTION 2
#endif

//////////////////////////////////////////////////////////////////////
enum XActionActivationMode
{
  aamOnPress,
  aamOnDoublePress,
  aamOnPressAndRelease,
  aamOnRelease,
  aamOnHold
};

//////////////////////////////////////////////////////////////////////
enum XActivationEvent
{
  etPressing,
  etHolding,
  etReleasing,
  etDoublePressing
};

//////////////////////////////////////////////////////////////////////
struct XBind
{
  XBind()
  {
    nKey = eKI_Unknown;
    nModifier = eMM_None;
  }
  uint32 nKey;
  int nModifier;
};

//////////////////////////////////////////////////////////////////////
struct IActionMapSink
{
  virtual void OnAction(XACTIONID nActionID, float fValue, XActivationEvent ae) = 0;
};

//////////////////////////////////////////////////////////////////////
struct IActionMap
{
  virtual void ResetAllBindings() = 0;
  virtual void ResetBinding(XACTIONID nActionID) = 0;
  virtual void RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam) = 0;
  virtual void BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos = -1) = 0;//int nKey,int nModifier=XKEY_NULL) = 0;
  virtual void BindAction(XACTIONID nActionID, uint32 nKey, EModifierMask nModifier = eMM_None, int iKeyPos = -1) = 0;//, bool bConfigurable=false, bool bReplicate=false) = 0;
  virtual void BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier = NULL, int iKeyPos = -1) = 0;
  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind) = 0;
  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier) = 0;
  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier) = 0;
  // compare this action map with the one passed and store the key differences in keys
  virtual void GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys) = 0;
};

//////////////////////////////////////////////////////////////////////
typedef unsigned char INPUTACTIONID;

//@{ Helper macros to implement the action triggers callback interface
#define BEGIN_INPUTACTIONMAP() void OnAction(XACTIONID nActionID, float fValue,XActivationEvent ae) override { switch(nActionID) {
#define END_INPUTACTIONMAP() default: break; } }
#define REGISTER_INPUTACTIONMAP(actionid, handler) case actionid: handler(fValue,ae); break;
//@}

//////////////////////////////////////////////////////////////////////
struct IActionMapDumpSink
{
  virtual void OnElementFound(const char* pszActionMapName, IActionMap* pActionMap) = 0;
};

//////////////////////////////////////////////////////////////////////
struct IActionMapManager
{
  virtual void SetInvertedMouse(bool bEnable) = 0;
  virtual bool GetInvertedMouse() = 0;;

  virtual void RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam) = 0;

  virtual void SetSink(IActionMapSink* pSink) = 0;
  virtual void CreateAction(XACTIONID nActionID, const char* sActionName, XActionActivationMode aam = aamOnPress) = 0;

  virtual IActionMap* CreateActionMap(const char* s) = 0;
  virtual IActionMap* GetActionMap(const char* s) = 0;

  virtual void ResetAllBindings() = 0;

  virtual void GetActionMaps(IActionMapDumpSink* pCallback) = 0;

  virtual void SetActionMap(const char* s) = 0;

  virtual bool CheckActionMap(XACTIONID nActionID) = 0;
  virtual bool CheckActionMap(const char* sActionName) = 0;
  virtual void Reset() = 0;
  virtual void Update(unsigned int nTimeMSec) = 0;
  virtual void Release() = 0;

  virtual void Enable() = 0;
  virtual void Disable() = 0;
  virtual bool IsEnabled() = 0;
};

//////////////////////////////////////////////////////////////////////
// Action triggers callback interface
struct IInputActionTriggerSink
{
  virtual void OnAction(INPUTACTIONID nActionID, float fValue) = 0;
};

//! Action map interface
struct IInputActionMap
{
  virtual void SetSink(IInputActionTriggerSink* pIActionTrigger) = 0;

  virtual void Release() = 0;

  //! Check all actions
  virtual void Update() = 0;

  // Call the action trigger
  virtual void CallActionTrigger(INPUTACTIONID nActionID, float fValue) = 0;

  //! Return the amount of pressing of the action input if the action is
  //! currently done
  virtual float CheckAction(const INPUTACTIONID nActionID) = 0;

  /*! Set a new action
  @param nActionID id that identity the action[eg. ACTION_JUMP]
  @param bCheckPressed if true the action event is triggered only once when a button is pressed
  else the action is send every frame until the button is released
  @param szCodes key identifiers [eg. "MBT_1" mouse button]
  @param szMods key modifier [eg. "SHIFT"]
  @return true=succeded,false=failed*/

  virtual bool SetAction(const INPUTACTIONID nActionID, bool bCheckPressed, const char* szCodes, const char* szMods = NULL) = 0;

  virtual void ClearAction(const INPUTACTIONID nActionID) = 0;
};

struct SInputSymbol;

//! InputEvents are generated by input system and dispatched to all event listeners.
//! The stream of SInputEvent should not be used for text input, the SUnicodeEvent stream should be used instead
struct SInputEvent
{
  EInputDeviceType deviceType;       //!< Device type from which the event originated.
  EInputState      state;            //!< Type of input event.
  TKeyName         keyName;          //!< Human readable name of the event.
  EKeyId           keyId;            //!< Device-specific id corresponding to the event.
  int              modifiers;        //!< Key modifiers enabled at the time of this event.
  float            value;            //!< Value associated with the event.
  SInputSymbol* pSymbol;          //!< Input symbol the event originated from.
  uint8_t          deviceIndex;      //!< Local index of this particular controller type.
  uint8_t          deviceUniqueID;   //!< Process wide unique controller ID.

  SInputEvent()
    : deviceType(eIDT_Unknown)
    , state(eIS_Unknown)
    , keyName("")
    , keyId(eKI_Unknown)
    , modifiers(eMM_None)
    , value(0.0f)
    , pSymbol(nullptr)
    , deviceIndex(0)
    , deviceUniqueID(0)
  {}
};

//! UnicodeEvents are generated by input system and dispatched to all event listeners.
//! The stream of Unicode events should only be used for text input.
struct SUnicodeEvent
{
  //! The unicode code-point that was entered.
  const uint32_t inputChar;

  SUnicodeEvent(uint32_t inputChar = 0) : inputChar(inputChar) {}
};

//!SFFTriggerOutputData are force feedback signals send to an input controller's triggers.
struct SFFTriggerOutputData
{
  struct Initial
  {
    enum Value
    {
      ZeroIt = 0,
      Default
    };
  };

  struct Flags
  {
    enum Value
    {
      LeftTouchToActivate = 1 << 0,
      RightTouchToActivate = 1 << 1,
    };
  };

  float  leftGain, rightGain;
  float  leftStrength, rightStrength;
  uint16_t leftEnv, rightEnv;
  uint32_t flags;

  SFFTriggerOutputData() { Init(Initial::ZeroIt); }
  SFFTriggerOutputData(Initial::Value v) { Init(v); }
  SFFTriggerOutputData(bool leftTouchToActivate, bool rightTouchToActivate, float lTrigger, float rTrigger, float lStrength, float rStrength, uint16_t lTriggerEnv, uint16_t rTriggerEnv) :
    leftGain(lTrigger), rightGain(rTrigger), leftStrength(lStrength), rightStrength(rStrength), leftEnv(lTriggerEnv), rightEnv(rTriggerEnv)
  {
    SetFlag(Flags::LeftTouchToActivate, leftTouchToActivate);
    SetFlag(Flags::RightTouchToActivate, rightTouchToActivate);
  }

  void Init(Initial::Value v)
  {
    if (v == Initial::ZeroIt)
    {
      flags = 0;
      leftGain = rightGain = 0.0f;
      leftStrength = rightStrength = 0.0f;
      leftEnv = rightEnv = 0;
    }
    else if (v == Initial::Default)
    {
      flags = 0;
      leftGain = rightGain = 1.0f;
      leftStrength = rightStrength = 0.0f;
      leftEnv = rightEnv = 4;
    }
  }

  void operator+=(const SFFTriggerOutputData& operand2)
  {
    leftGain += operand2.leftGain;
    rightGain += operand2.rightGain;
    // DARIO_TODO: check what to do with envelopes in this case
    leftEnv = std::max(leftEnv, operand2.leftEnv);
    rightEnv = std::max(rightEnv, operand2.rightEnv);
    // DARIO_TODO: check what to do with the touch required in this case
    SetFlag(Flags::LeftTouchToActivate, IsFlagEnabled(Flags::LeftTouchToActivate) || operand2.IsFlagEnabled(Flags::LeftTouchToActivate));
    SetFlag(Flags::RightTouchToActivate, IsFlagEnabled(Flags::RightTouchToActivate) || operand2.IsFlagEnabled(Flags::RightTouchToActivate));
  }

#define clamp_tpl(val, l, r) (std::max((l), std::min((val), (r))))

  ILINE float GetClampedLeftGain() const { return clamp_tpl(leftGain, 0.0f, 1.0f); }
  ILINE float GetClampedRightGain() const { return clamp_tpl(rightGain, 0.0f, 1.0f); }

  bool        IsFlagEnabled(Flags::Value f) const { return (flags & f) != 0; }

private:
  void EnableFlag(Flags::Value f) { flags |= f; }
  void DisableFlag(Flags::Value f) { flags &= ~f; }
  void SetFlag(Flags::Value f, bool b) { if (b) EnableFlag(f); else DisableFlag(f); }
};

//! SFFOutputEvents are force feedback signals send to an input controller.
struct SFFOutputEvent
{
  EInputDeviceType     deviceType;           //!< Which device will receive the event.
  EFFEffectId          eventId;
  float                amplifierS, amplifierA;
  float                timeInSeconds;

  SFFTriggerOutputData triggerData;

  SFFOutputEvent() : triggerData(SFFTriggerOutputData::Initial::Default)
  {
    deviceType = eIDT_Unknown;
    eventId = eFF_Rumble_Basic;
    amplifierS = 1.0f;
    amplifierA = 1.0f;
    timeInSeconds = 0.3f;
  }

  SFFOutputEvent(EInputDeviceType id, EFFEffectId event, SFFTriggerOutputData::Initial::Value triggerInitValue, float time = 1.0f, float ampA = 1.0f, float ampB = 1.0f) :
    deviceType(id), eventId(event), timeInSeconds(time), amplifierS(ampA), amplifierA(ampB),
    triggerData(triggerInitValue)
  {}

  SFFOutputEvent(EInputDeviceType id, EFFEffectId event, const SFFTriggerOutputData& triggerData, float time = 1.0f, float ampA = 1.0f, float ampB = 1.0f) :
    deviceType(id), eventId(event), timeInSeconds(time), amplifierS(ampA), amplifierA(ampB),
    triggerData(triggerData)
  {}
};

struct SInputSymbol
{
  //! Input symbol types.
  enum EType
  {
    Button,  //!< State == press/hold/release -- value = 0, 1.
    Toggle,  //!< Transition state with a press.
    RawAxis, //!< State == change -- value = movement of axis.
    Axis,    //!< State == change -- value = -1.0 to 1.0.
    Trigger, //!< State == change -- value = 0.0 to 1.0.
  };

  SInputSymbol(uint32_t devSpecId_, EKeyId keyId_, const TKeyName& name_, EType type_, uint32_t user_ = 0)
    : devSpecId(devSpecId_)
    , keyId(keyId_), name(name_)
    , state(eIS_Unknown)
    , type(type_)
    , value(0.0f)
    , user(user_)
    , deviceIndex(0)
  {
  }

  void PressEvent(bool pressed)
  {
    if (pressed)
    {
      state = eIS_Pressed;
      value = 1.0f;
    }
    else
    {
      state = eIS_Released;
      value = 0.0f;
    }
  }
  void ChangeEvent(float v)
  {
    state = eIS_Changed;
    value = v;
  }
  void AssignTo(SInputEvent& event, int modifiers = 0)
  {
    event.pSymbol = this;
    event.deviceType = deviceType;
    event.modifiers = modifiers;
    event.state = state;
    event.value = value;
    event.keyName = name;
    event.keyId = keyId;
    //event.deviceIndex = deviceIndex; //symbol does not know device index, but needs to cache it for hold events
  }

  const EKeyId     keyId;           //!< External id for fast comparison.
  const TKeyName   name;            //!< Human readable name of the event.
  const uint32_t     devSpecId;       //!< Device internal id of this symbol (we will use it to look it up).
  EInputState      state;           //!< Current state.
  const EType      type;            //!< Type of this symbol.
  float            value;           //!< Current value.
  uint32_t           user;            //!< Type dependent value (toggle-mask for toggles).
  EInputDeviceType deviceType;      //!< Which device does the symbol belong to.
  uint8_t            deviceIndex;     //!< Device index - controller 1/2 etc.
};

typedef std::vector<SInputSymbol*> TInputSymbols;
struct SInputBlockData
{
  //! Data used to block input symbol from firing event if matched.
  SInputBlockData(const EKeyId keyId_,
    const float fBlockDuration_,
    const bool bAllDeviceIndices_ = true,
    const uint8_t deviceIndex_ = 0)
    : keyId(keyId_)
    , fBlockDuration(fBlockDuration_)
    , bAllDeviceIndices(bAllDeviceIndices_)
    , deviceIndex(deviceIndex_)
  {
  }

  float        fBlockDuration;     //!< How long will still be blocked for.
  const EKeyId keyId;              //!< External id for fast comparison.
  uint8_t        deviceIndex;        //!< Device index - controller 1/2 etc.
  bool         bAllDeviceIndices;  //!< True to block all device indices of deviceID type, otherwise uses deviceIndex.
};

//! Input event listeners registered to input system and receive input events when they are generated.
struct IInputEventListener
{
  // <interfuscator:shuffle>
  virtual ~IInputEventListener() {}

  //! Called every time input event is generated.
  //! \return true if the broadcasting of this event should be aborted and the rest of input listeners should not receive this event.
  virtual bool OnInputEvent(const SInputEvent& event) = 0;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) { return false; }

  //! Used to sort the listeners based on priority
  //! \return Priority associated with the listener (Higher the priority, the earlier it will be processed relative to other listeners, default = 0)
  virtual int GetPriority() const { return 0; }
  // </interfuscator:shuffle>
};

struct IFFParams
{
  EInputDeviceType     deviceType;
  EFFEffectId          effectId;
  float                strengthA, strengthB;
  float                timeInSeconds;
  SFFTriggerOutputData triggerData;

  IFFParams() : strengthA(0), strengthB(0), timeInSeconds(0), triggerData(SFFTriggerOutputData::Initial::ZeroIt)
  {
    effectId = eFF_Rumble_Basic;
    deviceType = eIDT_Unknown;
  }
};

//////////////////////////////////////////////////////////////////////////
typedef uint64_t TInputDeviceId;
#define InvalidInputDeviceId 0

struct IInputDevice
{
  // <interfuscator:shuffle>
  //! Implements virtual destructor just for safety.
  virtual ~IInputDevice() {}

  virtual const char* GetDeviceName() const = 0;
  virtual EInputDeviceType GetDeviceType() const = 0;
  virtual TInputDeviceId   GetDeviceId() const = 0;
  virtual int              GetDeviceIndex() const = 0;

  //! Initialization.
  virtual bool Init() = 0;
  virtual void PostInit() = 0;

  //! Update.
  virtual void Update(bool bFocus) = 0;

  //! Sets force feedback.
  //! \return true if successful.
  virtual bool SetForceFeedback(IFFParams params) = 0;

  //! Checks for key pressed and held.
  virtual bool InputState(const TKeyName& key, EInputState state) = 0;

  //! Sets/unsets DirectInput to exclusive mode.
  virtual bool SetExclusiveMode(bool value) = 0;

  //! Clears the key (pressed) state.
  virtual void ClearKeyState() = 0;

  //! Clears analog position state.
  virtual void ClearAnalogKeyState(TInputSymbols& clearedSymbols) = 0;

  //! Called upon creation to assign a process wide unique Id.
  virtual void                SetUniqueId(uint8_t const uniqueId) = 0;
  virtual const char* GetKeyName(const SInputEvent& event) const = 0;
  virtual const char* GetKeyName(const EKeyId keyId) const = 0;
  virtual uint32_t              GetInputCharUnicode(const SInputEvent& event) = 0;
  virtual const char* GetOSKeyName(const SInputEvent& event) = 0;
  virtual SInputSymbol* LookupSymbol(EKeyId id) const = 0;
  virtual const SInputSymbol* GetSymbolByName(const char* name) const = 0;
  virtual bool                IsOfDeviceType(EInputDeviceType type) const = 0;
  virtual void                Enable(bool enable) = 0;
  virtual bool                IsEnabled() const = 0;
  virtual void                OnLanguageChange() = 0;

  //! Dead zone settings for input devices where this is relevant (i.e. controllers with analog sticks).
  //! \param fThreshold Between 0 and 1.
  virtual void SetDeadZone(float fThreshold) = 0;
  virtual void RestoreDefaultDeadZone() = 0;
  // </interfuscator:shuffle>
};

//! Interface to the Input system.
//! The input system give access and initialize Keyboard,Mouse and Joystick SubSystems.
struct IInput
{
  // <interfuscator:shuffle>
  virtual ~IInput() {}
  //! Registers new input events listener.
  virtual void AddEventListener(IInputEventListener* pListener) = 0;
  virtual void RemoveEventListener(IInputEventListener* pListener) = 0;

  //! Registers new console input event listeners. console input listeners receive all events, no matter what.
  virtual void AddConsoleEventListener(IInputEventListener* pListener) = 0;
  virtual void RemoveConsoleEventListener(IInputEventListener* pListener) = 0;

  virtual bool AddTouchEventListener(ITouchEventListener* pListener, const char* name) = 0;
  virtual void RemoveTouchEventListener(ITouchEventListener* pListener) = 0;

  //! Registers an exclusive listener which has the ability to filter out events before they arrive at the normal listeners.
  virtual void                 SetExclusiveListener(IInputEventListener* pListener) = 0;
  virtual IInputEventListener* GetExclusiveListener() = 0;

  virtual bool                 AddInputDevice(IInputDevice* pDevice) = 0;
  virtual void                 EnableEventPosting(bool bEnable) = 0;
  virtual bool                 IsEventPostingEnabled() const = 0;
  virtual void                 PostInputEvent(const SInputEvent& event, bool bForce = false) = 0;
  virtual void                 PostTouchEvent(const STouchEvent& event, bool bForce = false) = 0;
  virtual void                 PostUnicodeEvent(const SUnicodeEvent& event, bool bForce = false) = 0;

  //! For direct key processing (e.g. win proc functions).
  //! Currently only used by durango metro launcher.
  virtual void ProcessKey(uint32_t key, bool pressed, wchar_t unicode, bool repeat) = 0;

  //! Initializes input system.
  //! \note Required params should be passed through constructor.
  virtual bool Init() = 0;

  //! Post Initialization called at end of initialization.
  virtual void PostInit() = 0;

  //! Updates Keyboard, Mouse and Joystick. Sets bFocus to true if window has focus and input is enabled.
  virtual void Update(bool bFocus) = 0;

  //! Clears all subsystems.
  virtual void ShutDown() = 0;

  //! \see IInputDevice::SetExclusive
  virtual void SetExclusiveMode(EInputDeviceType deviceType, bool exclusive, void* hwnd = 0) = 0;

  //! \see IInputDevice::InputState
  virtual bool InputState(const TKeyName& key, EInputState state) = 0;

  //! Converts an input event to the key name.
  //! The function should internally dispatch to all managed input devices and return the first recognized event.
  //! \param event Input event to translate into a name.
  virtual const char* GetKeyName(const SInputEvent& event) const = 0;

  //! Converts an input event to the key name.
  //! The function should internally dispatch to all managed input devices and return the first recognized keyId.
  //! \param keyId Input keyId to translate into a name.
  //! \return Translated key name.
  virtual const char* GetKeyName(EKeyId keyId) const = 0;

  //! Gets an input char translated to unicode from the event.
  //! The function should internally dispatch to all managed input devices and return the first recognized event.
  //! \param event Input event to translate into a name.
  virtual uint32_t GetInputCharUnicode(const SInputEvent& event) = 0;

  //! Lookups a symbol for a given symbol and key ids.
  virtual SInputSymbol* LookupSymbol(EInputDeviceType deviceType, int deviceIndex, EKeyId keyId) = 0;

  //! Looks up a symbol for a key name.
  virtual const SInputSymbol* GetSymbolByName(const char* name) const = 0;

  //! Gets OS Keyname.
  //! \param event Input event to translate into a name.
  virtual const char* GetOSKeyName(const SInputEvent& event) = 0;

  //! Clears key states of all devices.
  virtual void ClearKeyState() = 0;

  //! Clears analog key states of all devices.
  virtual void ClearAnalogKeyState() = 0;

  //! Re-triggers pressed keys.
  //! \note Used for transitioning action maps.
  virtual void RetriggerKeyState() = 0;

  //! Gets if we are currently re-triggering.
  //! \note Needed to filter out actions.
  virtual bool Retriggering() = 0;

  //! Queries to see if this machine has some kind of input device connected.
  virtual bool HasInputDeviceOfType(EInputDeviceType type) = 0;

  //! Gets the currently pressed modifiers.
  virtual int GetModifiers() const = 0;

  //! Tells devices whether to report input or not.
  virtual void EnableDevice(EInputDeviceType deviceType, bool enable) = 0;

  //! Dead zone settings for input devices where this is relevant (i.e. controllers with analog sticks).
  //! \param fThreshold Between 0 and 1.
  virtual void SetDeadZone(float fThreshold) = 0;
  virtual void RestoreDefaultDeadZone() = 0;

  //! Gets the device at an index for a specific Type.
  //! \return Pointer to device, or NULL if not found/valid.
  virtual IInputDevice* GetDevice(uint16_t id, EInputDeviceType deviceType) = 0;

  //! Returns EInputPlatformFlags.
  virtual uint32_t GetPlatformFlags() const = 0;

  //! Adds or updates blocking input if SInputBlockData::fBlockDuration exceeds previously blocking time.
  //! \return true if successfully added inputBlocker or updated existed input blocker's remaining blocking time.
  virtual bool SetBlockingInput(const SInputBlockData& inputBlockData) = 0;

  //! Removes blocking input.
  //! \return True if found and removed, false otherwise.
  virtual bool RemoveBlockingInput(const SInputBlockData& inputBlockData) = 0;

  //! Checks if the specified input is currently being blocked.
  //! \return true if specified input is currently being blocked, false otherwise.
  virtual bool HasBlockingInput(const SInputBlockData& inputBlockData) const = 0;

  //! Gets the number of inputs currently being blocked.
  //! \return Number of inputs currently being blocked.
  virtual int GetNumBlockingInputs() const = 0;

  //! Clears all the inputs being blocked.
  virtual void ClearBlockingInputs() = 0;

  //! Checks if the input specified should be blocked.
  //! \return True if input specified should be blocked, false otherwise.
  virtual bool ShouldBlockInputEventPosting(const EKeyId keyId, const EInputDeviceType deviceType, const uint8_t deviceIndex) const = 0;

  //! Show or hide the mouse cursor
  //! \return Internal display count. Cursor is visible if the display count is greater or equal to zero.
  virtual int ShowCursor(const bool bShow) = 0;
  //////////////////////////////////////////////////////////////////////////
  // SDL
  virtual bool GrabInput(bool bGrab) = 0;

  //! action mapper
  virtual struct IActionMapManager* CreateActionMapManager() = 0;

  //! Allows for Sandbox to intercept and modify events prior to system wide sending.
  //! \return True if the event is supposed to be broadcasted.
  bool (*OnFilterInputEvent)(SInputEvent*);

  // </interfuscator:shuffle>
};

extern "C" {

  INPUT_API IInput* CreateInput(ISystem* pSystem, void* hwnd);
  typedef IInput*(* PTRCREATEINPUTFUNC)(ISystem* pSystem, void* nwnd);

};

#endif //__IINPUT_H__
