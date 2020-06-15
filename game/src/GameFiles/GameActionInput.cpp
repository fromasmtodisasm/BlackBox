#include <Game.hpp>
#include <GameShared.hpp>

//////////////////////////////////////////
//small utility macro
//////////////////////////////////////////
#define ADD_ACTION(_action,_activationmode,_desc,_type,_configurable) \
{	\
	ActionInfo Info; \
	Info.nId=ACTION_##_action; \
	Info.sDesc=_desc; \
	Info.bConfigurable=_configurable; \
	Info.ActivationMode=_activationmode; \
	Info.nType=_type; \
	m_pIActionMapManager->CreateAction(ACTION_##_action,#_action,_activationmode); \
	m_mapActionsEnum.insert(ActionsEnumMapItor::value_type(#_action,Info)); \
}

#define ACTIONMAPS_ALL "default", "zoom", "binozoom", "vehicle", "player_dead", ""
#define ACTIONMAPS_NODEAD "default", "zoom", "binozoom", "vehicle", ""
#define ACTIONMAPS_WEAPON "default", "binozoom", ""
#define ACTIONMAPS_DEF_ZOOMS "default", "zoom", "binozoom",""

void CGame::SetConfigToActionMap(const char* pszActionName, ...)
{
  ActionsEnumMapItor It = m_mapActionsEnum.find(pszActionName);
  if (It == m_mapActionsEnum.end())
    return;
  ActionInfo& Info = It->second;
  va_list v;
  va_start(v, pszActionName);
#if defined(LINUX64)
  char* sActionMapName = va_arg(v, char*);
  while (*sActionMapName)
  {
    Info.vecSetToActionMap.push_back(string(sActionMapName));
    sActionMapName = va_arg(v, char*);
  }
#else
  char* sActionMapName = *(char**)v;

  while (*sActionMapName)
  {
    Info.vecSetToActionMap.push_back(string(sActionMapName));
    sActionMapName = *(char**)(v += sizeof(char*));
  }
#endif
  va_end(v);
}

void CGame::InitInputMap()
{
  IInput* pInput = m_pSystem->GetIInput();

  m_pIActionMapManager = pInput->CreateActionMapManager();
	if(m_pIActionMapManager)
		m_pIActionMapManager->SetSink(this);

  ResetInputMap();
}

void CGame::InitConsoleCommands()
{
  IConsole* pConsole = m_pSystem->GetIConsole();

  REGISTER_COMMAND("reload_script", "Script:ReloadScript(%1)", VF_CHEAT,
    "\n"
    "Usage: \n"
    "");
  REGISTER_COMMAND("dump_scripts", "Script:DumpLoadedScripts()", VF_CHEAT,
    "Outputs a list of currently loaded scripts.\n"
    "Usage: dump_scripts\n");
  REGISTER_COMMAND("reload_scripts", "Script:ReloadScripts()", VF_CHEAT,
    "Reload a list of currently loaded scripts.\n"
    "Usage: dump_scripts\n");
  REGISTER_COMMAND("quit", "Game:Quit()", 0,
    "Quits the game.\n"
    "Usage: quit\n");
  REGISTER_COMMAND("clear", "System:ClearConsole()", 0,
    "Clears console text.\n"
    "Usage: clear\n");
  REGISTER_COMMAND("fov", "Game:SetCameraFov(%1/180*3.14159)", VF_CHEAT,
    "Sets the player's field of view.\n"
    "Usage: fov 120\n"
    "The field of vision is set in degrees between 1 and 180.");
  REGISTER_COMMAND("dumpcommandsvars", "System:DumpCommandsVars(%%)", 0,
    "Outputs a list of commands and variables.\n"
    "Usage: dumpcommandsvars\n"
    "Saves a list of all registered commands and variables\n"
    "to a file called consolecommandsandvars.txt");

  REGISTER_COMMAND("savepos", "Game:SavePlayerPos(%%)", 0,
    "Saves current player position to the tagpoint file.\n"
    "Usage: savepos pointname\n");
  REGISTER_COMMAND("loadpos", "Game:LoadPlayerPos(%%)", 0,
    "Loads player position from the tagpoint file.\n"
    "Usage: loadpos pointname\n");
}

//extern float CameraRayLength = 40.f;
void CGame::InitConsoleVars()
{
  IConsole* pConsole = m_pSystem->GetIConsole();
  r_displayinfo = CREATE_CVAR("r_displayinfo", 1, 0, "Display info [1/0]");
  r_profile = CREATE_CVAR("r_profile", 1, 0, "Profile [1/0]");
  r_cap_profile = CREATE_CVAR("r_cap_profile", 1, 0, "Capture frame [1/0]");
  m_pCVarCheatMode = CREATE_CVAR("zz0x067MD4", "DEVMODE", VF_NET_SYNCED, "");

  //REGISTER_CVAR(CameraRayLength, CameraRayLength, 0);

	g_playerprofile= pConsole->CreateVariable("g_playerprofile","default",VF_DUMPTODISK,
		"Sets the player profile (to store player settings).\n"
		"Leave this empty to get the defaults from the root directory.\n");
	sv_port= pConsole->CreateVariable("sv_port",DEFAULT_SERVERPORT_STR,0,
		"Sets the server port for a multiplayer game.\n"
		"Usage: sv_port portnumber\n"
		"Default is '49001'.");
	sv_timeout= pConsole->CreateVariable("sv_timeout","60",0, // until entity loading speeds up :)	
		"Sets the server timeout (seconds).\n"
		"Usage: sv_timeout 60\n"
		"The default timeout is 60 seconds. This is the time the\n"
		"server waits while trying to establish a connection with\n"
		"a client."); 


	g_LevelName= pConsole->CreateVariable("g_LevelName","0.4",0);
	pConsole->CreateVariable("g_LevelStated","0",0, "");
	g_MissionName= pConsole->CreateVariable("g_MissionName","TestMission",0);

	{
		cv_game_Difficulty = pConsole->CreateVariable("game_AdaptiveDifficulty", "0", VF_SAVEGAME,
			"0=off, 1=on\n"
			"Usage: \n"
			"");
		cv_game_Aggression = pConsole->CreateVariable("game_Aggression", "1", VF_SAVEGAME,
			"Factor to scale the ai agression, default = 1.0\n"
			"Usage: cv_game_Aggression 1.2\n"
			"");
		cv_game_Accuracy = pConsole->CreateVariable("game_Accuracy", "1", VF_SAVEGAME,
			"Factor to scale the ai accuracy, default = 1.0\n"
			"Usage: game_Accuracy 1.2\n");
		cv_game_GliderGravity = pConsole->CreateVariable("game_GliderGravity", "-0.1f", VF_DUMPTODISK,
			"Sets paraglider's gravity.\n"
			"Usage: game_GliderGravity -.1\n");
		cv_game_GliderBackImpulse = pConsole->CreateVariable("game_GliderBackImpulse", "2.5f", VF_DUMPTODISK,
			"Sets paraglider's back impulse (heading up).\n"
			"Usage: game_GliderBackImpulse 2.5\n");
		cv_game_GliderDamping = pConsole->CreateVariable("game_GliderDamping", "0.15f", VF_DUMPTODISK,
			"Sets paraglider's damping (control/inertia).\n"
			"Usage: game_GliderDamping 0.15\n");
		cv_game_GliderStartGravity = pConsole->CreateVariable("game_GliderStartGravity", "-0.8f", VF_DUMPTODISK,
			"Sets initial paraglider's gravity.\n"
			"Usage: game_GliderStartGravity -0.8");
	}

}

void  CGame::ResetInputMap()
{
  assert(m_pIActionMapManager);

  m_pIActionMapManager->ResetAllBindings();

  //-------------------------------------------------------------------------------------------------
  ADD_ACTION(MOVE_LEFT, aamOnHold, "@MoveLeft", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVE_LEFT", ACTIONMAPS_ALL);
  ADD_ACTION(MOVE_RIGHT, aamOnHold, "@MoveRight", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVE_RIGHT", ACTIONMAPS_ALL);
  ADD_ACTION(MOVE_FORWARD, aamOnHold, "@MoveForward", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVE_FORWARD", ACTIONMAPS_NODEAD);
  ADD_ACTION(MOVELR, aamOnHold, "@MoveLeftRight", 0, false) SetConfigToActionMap("MOVELR", ACTIONMAPS_NODEAD);
  ADD_ACTION(MOVEFB, aamOnHold, "@MoveFwdBack", 0, false) SetConfigToActionMap("MOVEFB", ACTIONMAPS_NODEAD);

  ADD_ACTION(WALK, aamOnHold, "@Walk", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("WALK", ACTIONMAPS_NODEAD);
  ADD_ACTION(RUNSPRINT, aamOnHold, "@SprintRun", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("RUNSPRINT", ACTIONMAPS_NODEAD);

  ADD_ACTION(MESSAGEMODE, aamOnPress, "@Chat", ACTIONTYPE_MULTIPLAYER, true) SetConfigToActionMap("MESSAGEMODE", ACTIONMAPS_ALL);
  ADD_ACTION(MESSAGEMODE2, aamOnPress, "@TeamChat", ACTIONTYPE_MULTIPLAYER, true) SetConfigToActionMap("MESSAGEMODE2", ACTIONMAPS_ALL);

  //ADD_ACTION(RUN,aamOnDoublePress,"Move Forward2",false) SetConfigToActionMap("RUN", ACTIONMAPS_NODEAD);
  ADD_ACTION(MOVE_BACKWARD, aamOnHold, "@MoveBackward", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVE_BACKWARD", ACTIONMAPS_NODEAD);
  //ADD_ACTION(VEHICLE_BOOST,aamOnDoublePress)
  ADD_ACTION(SCORE_BOARD, aamOnHold, "@ViewScoreboard", ACTIONTYPE_MULTIPLAYER, true) SetConfigToActionMap("SCORE_BOARD", ACTIONMAPS_ALL);
  ADD_ACTION(JUMP, aamOnHold, "@Jump", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("JUMP", "default", "binozoom", "vehicle", "");
  ADD_ACTION(MOVEMODE, aamOnHold, "@Crouch", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVEMODE", ACTIONMAPS_NODEAD);
  ADD_ACTION(MOVEMODE2, aamOnPress, "@Prone", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVEMODE2", ACTIONMAPS_NODEAD);
  //	ADD_ACTION(MOVEMODE2,aamOnDoublePress,"Change Movemode",true)
  ADD_ACTION(LEANLEFT, aamOnHold, "@LeanLeft", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("LEANLEFT", "default", "zoom", "binozoom", "");
  ADD_ACTION(LEANRIGHT, aamOnHold, "@LeanRight", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("LEANRIGHT", "default", "zoom", "binozoom", "");
  ADD_ACTION(FIRE0, aamOnHold, "@Fire", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("FIRE0", "default", "zoom", "vehicle", "player_dead", "");
  //ADD_ACTION(FIRECANCEL,aamOnHold,"@CancelCurrentTarget",ACTIONTYPE_COMBAT,true) SetConfigToActionMap("FIRECANCEL", "default", "zoom", "vehicle", "");
  ADD_ACTION(USE, aamOnPress, "@Use", ACTIONTYPE_GAME, true) SetConfigToActionMap("USE", ACTIONMAPS_NODEAD);
  ADD_ACTION(TURNLR, aamOnHold, "@TurnLeftRight", 0, false) SetConfigToActionMap("TURNLR", ACTIONMAPS_NODEAD);
  ADD_ACTION(TURNUD, aamOnHold, "@TurnUpDown", 0, false) SetConfigToActionMap("TURNUD", ACTIONMAPS_NODEAD);
  //	ADD_ACTION(RUN,aamOnHold,"Toggle Run",true)
  ADD_ACTION(NEXT_WEAPON, aamOnPress, "@NextWeapon", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("NEXT_WEAPON", "default", "");
  ADD_ACTION(PREV_WEAPON, aamOnPress, "@PrevWeapon", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("PREV_WEAPON", "default", "");
  ADD_ACTION(RELOAD, aamOnPress, "@Reload", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("RELOAD", ACTIONMAPS_NODEAD);
  //ADD_ACTION(WEAPON_0,aamOnPress,"@Slot0",ACTIONTYPE_COMBAT,true) SetConfigToActionMap("WEAPON_0", ACTIONMAPS_WEAPON);
  ADD_ACTION(DROPWEAPON, aamOnPress, "@DropWeapon", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("DROPWEAPON", "default", "");
  ADD_ACTION(WEAPON_1, aamOnPress, "@Slot1", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("WEAPON_1", "default", "zoom", "binozoom", "");
  ADD_ACTION(WEAPON_2, aamOnPress, "@Slot2", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("WEAPON_2", "default", "zoom", "binozoom", "");
  ADD_ACTION(WEAPON_3, aamOnPress, "@Slot3", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("WEAPON_3", "default", "zoom", "binozoom", "");
  ADD_ACTION(WEAPON_4, aamOnPress, "@Slot4", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("WEAPON_4", "default", "zoom", "binozoom", "");
  /*ADD_ACTION(WEAPON_5,aamOnPress,"@Slot5",true) SetConfigToActionMap("WEAPON_5", ACTIONMAPS_WEAPON);
  ADD_ACTION(WEAPON_6,aamOnPress,"@Slot6",true) SetConfigToActionMap("WEAPON_6", ACTIONMAPS_WEAPON);
  ADD_ACTION(WEAPON_7,aamOnPress,"@Slot7",true) SetConfigToActionMap("WEAPON_7", ACTIONMAPS_WEAPON);
  ADD_ACTION(WEAPON_8,aamOnPress,"@Slot8",true) SetConfigToActionMap("WEAPON_8", ACTIONMAPS_WEAPON);
  */

  ADD_ACTION(CYCLE_GRENADE, aamOnPress, "@CycleGrenade", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("CYCLE_GRENADE", ACTIONMAPS_NODEAD);

  //ADD_ACTION(LOADPOS,aamOnPress,"Load Position",ACTIONTYPE_DEBUG,true) SetConfigToActionMap("LOADPOS", ACTIONMAPS_ALL);
  //ADD_ACTION(SAVEPOS,aamOnPress,"Save Position",ACTIONTYPE_DEBUG,true) SetConfigToActionMap("SAVEPOS", ACTIONMAPS_ALL);
  ADD_ACTION(ITEM_0, aamOnPress, "@Binoculars", ACTIONTYPE_GAME, true) SetConfigToActionMap("ITEM_0", ACTIONMAPS_DEF_ZOOMS);
  ADD_ACTION(ITEM_1, aamOnPress, "@ThermalVision", ACTIONTYPE_GAME, true) SetConfigToActionMap("ITEM_1", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_2,aamOnPress,"@Item2",0,false) SetConfigToActionMap("ITEM_2", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_3,aamOnPress,"@Item3",0,false) SetConfigToActionMap("ITEM_3", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_4,aamOnPress,"@Item4",0,false) SetConfigToActionMap("ITEM_4", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_5,aamOnPress,"@Item5",0,false) SetConfigToActionMap("ITEM_5", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_6,aamOnPress,"@Item6",0,false) SetConfigToActionMap("ITEM_6", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_7,aamOnPress,"@Item7",0,false) SetConfigToActionMap("ITEM_7", ACTIONMAPS_NODEAD);
  //ADD_ACTION(ITEM_8,aamOnPress,"@Item8",0,false) SetConfigToActionMap("ITEM_8", ACTIONMAPS_NODEAD);

  ADD_ACTION(ZOOM_TOGGLE, aamOnPressAndRelease, "@ToggleZoom", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("ZOOM_TOGGLE", "default", "zoom", "");
  ADD_ACTION(ZOOM_IN, aamOnPress, "@ZoomIn", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("ZOOM_IN", "zoom", "binozoom", "");
  ADD_ACTION(ZOOM_OUT, aamOnPress, "@ZoomOut", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("ZOOM_OUT", "zoom", "binozoom", "");
  ADD_ACTION(HOLDBREATH, aamOnHold, "@HoldBreath", ACTIONTYPE_GAME, true) SetConfigToActionMap("HOLDBREATH", "zoom", "");
  ADD_ACTION(FIREMODE, aamOnPress, "@ToggleFiremode", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("FIREMODE", ACTIONMAPS_NODEAD);
  ADD_ACTION(QUICKLOAD,aamOnPress,"@Quickload",ACTIONTYPE_GAME,true) SetConfigToActionMap("QUICKLOAD", ACTIONMAPS_ALL);
  ADD_ACTION(QUICKSAVE,aamOnPress,"@Quicksave",ACTIONTYPE_GAME,true) SetConfigToActionMap("QUICKSAVE", ACTIONMAPS_ALL);
  //ADD_ACTION(FULLSCRN_TOOGLE,aamOnPress,"@ToogleFullScreen",ACTIONTYPE_GAME,true) SetConfigToActionMap("FULLSCRN_TOOGLE", ACTIONMAPS_ALL);
  ADD_ACTION(FIRE_GRENADE, aamOnHold, "@ThrowGrenade", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("FIRE_GRENADE", ACTIONMAPS_NODEAD);
  //ADD_ACTION(CONCENTRATION,aamOnHold,"@Concentration",ACTIONTYPE_GAME,true) SetConfigToActionMap("CONCENTRATION", "default", "");
  ADD_ACTION(FLASHLIGHT, aamOnPress, "@ToggleFlashlight", ACTIONTYPE_GAME, true) SetConfigToActionMap("FLASHLIGHT", ACTIONMAPS_NODEAD);
  ADD_ACTION(CHANGE_VIEW, aamOnPress, "@SwitchView", ACTIONTYPE_GAME, true) SetConfigToActionMap("CHANGE_VIEW", "default", "vehicle", "");
  ADD_ACTION(TAKESCREENSHOT, aamOnPress, "@TakeScreenshot", ACTIONTYPE_GAME, true) SetConfigToActionMap("TAKESCREENSHOT", ACTIONMAPS_ALL);

  ADD_ACTION(MOVEMODE_TOGGLE, aamOnPress, "@CrouchToggle", ACTIONTYPE_MOVEMENT, true) SetConfigToActionMap("MOVEMODE_TOGGLE", ACTIONMAPS_NODEAD);
  ADD_ACTION(AIM_TOGGLE, aamOnPress, "@ToggleAim", ACTIONTYPE_COMBAT, true) SetConfigToActionMap("AIM_TOGGLE", "default", "zoom", "");

  //default action map
  //////////////////////////////////////////////////////////////////////
  IActionMap* pMap = m_pIActionMapManager->CreateActionMap("default");

  SetCommonKeyBindings(pMap);

  //change to distinct weapons
  pMap->BindAction(ACTION_WEAPON_0, eKI_0);
  pMap->BindAction(ACTION_WEAPON_1, eKI_1);
  pMap->BindAction(ACTION_WEAPON_2, eKI_2);
  pMap->BindAction(ACTION_WEAPON_3, eKI_3);
  pMap->BindAction(ACTION_WEAPON_4, eKI_4);
  /*	pMap->BindAction(ACTION_WEAPON_5,eKI_5);
  pMap->BindAction(ACTION_WEAPON_6,eKI_6);
  pMap->BindAction(ACTION_WEAPON_7,eKI_7);
  pMap->BindAction(ACTION_WEAPON_8,eKI_8);
  */
  //Scroll up through weapons
  pMap->BindAction(ACTION_PREV_WEAPON, eKI_PgUp);
  //Scroll down through weapons
  pMap->BindAction(ACTION_NEXT_WEAPON, eKI_PgDn);

  //lean (not active in zoom mode)
  //lean left
  pMap->BindAction(ACTION_LEANLEFT, eKI_Q);
  pMap->BindAction(ACTION_LEANLEFT, eKI_NP_7);

  //lean right
  pMap->BindAction(ACTION_LEANRIGHT, eKI_E);
  pMap->BindAction(ACTION_LEANRIGHT, eKI_NP_9);

  //fire (outside common key bindings because is not possible in binozoom)
  pMap->BindAction(ACTION_FIRE0, eKI_Mouse1);

  //jump (outside common key bindings because space is reserved for hold the breath
  //in zoom mode)
  pMap->BindAction(ACTION_JUMP, eKI_Space);
  pMap->BindAction(ACTION_JUMP, eKI_NP_Enter);

  //scroll up/down through weapons
  //mouse wheel up/down is reserved for zoom in other zoom mode
  pMap->BindAction(ACTION_NEXT_WEAPON, eKI_MouseWheelUp);
  pMap->BindAction(ACTION_PREV_WEAPON, eKI_MouseWheelDown);

  //binocular/motion tracker/listening device
  //(should not be enabled if already in zoom mode)

  //aimed mode/scoped mode (use zoom)
  pMap->BindAction(ACTION_ZOOM_TOGGLE, eKI_Mouse2);

  //drop weapon
  pMap->BindAction(ACTION_DROPWEAPON, eKI_J);

  //vision modes
  //binocular/motion tracker/listening device
  pMap->BindAction(ACTION_ITEM_0, eKI_B);

  // thermal vision
  pMap->BindAction(ACTION_ITEM_1, eKI_T);

  pMap->BindAction(ACTION_CHANGE_VIEW, eKI_F1);

  //////////////////////////////////////////////////////////////////////
  pMap = m_pIActionMapManager->CreateActionMap("zoom");

  SetCommonKeyBindings(pMap);

  // weapon switch
  pMap->BindAction(ACTION_WEAPON_1, eKI_1);
  pMap->BindAction(ACTION_WEAPON_2, eKI_2);
  pMap->BindAction(ACTION_WEAPON_3, eKI_3);
  pMap->BindAction(ACTION_WEAPON_4, eKI_4);

  //vision modes
  //binocular/motion tracker/listening device
  pMap->BindAction(ACTION_ITEM_0, eKI_B);

  //lean (not active in zoom mode)
  //lean left
  pMap->BindAction(ACTION_LEANLEFT, eKI_Q);
  pMap->BindAction(ACTION_LEANLEFT, eKI_NP_7);

  //lean right
  pMap->BindAction(ACTION_LEANRIGHT, eKI_E);
  pMap->BindAction(ACTION_LEANRIGHT, eKI_NP_9);
  //zoom
  pMap->BindAction(ACTION_ZOOM_IN, eKI_MouseWheelUp);
  pMap->BindAction(ACTION_ZOOM_IN, eKI_NP_Add);

  pMap->BindAction(ACTION_ZOOM_OUT, eKI_MouseWheelDown);
  pMap->BindAction(ACTION_ZOOM_OUT, eKI_NP_Substract);

  //fire (outside common key bindings because is not possible in binozoom)
  pMap->BindAction(ACTION_FIRE0, eKI_Mouse1);

  //hold the breath
  pMap->BindAction(ACTION_HOLDBREATH, eKI_Space);

  //aimed mode/scoped mode (use zoom)
  pMap->BindAction(ACTION_ZOOM_TOGGLE, eKI_Mouse2);

  // rmb is used for binocular, night vision & thermal vision
  pMap->BindAction(ACTION_ITEM_1, eKI_T); //thermal vision

  //////////////////////////////////////////////////////////////////////
  pMap = m_pIActionMapManager->CreateActionMap("binozoom");

  SetCommonKeyBindings(pMap);

  // weapon switch
  pMap->BindAction(ACTION_WEAPON_1, eKI_1);
  pMap->BindAction(ACTION_WEAPON_2, eKI_2);
  pMap->BindAction(ACTION_WEAPON_3, eKI_3);
  pMap->BindAction(ACTION_WEAPON_4, eKI_4);

  //vision modes
  //binocular/motion tracker/listening device
  pMap->BindAction(ACTION_ITEM_0, eKI_B);

  //lean (not active in zoom mode)
  //lean left
  pMap->BindAction(ACTION_LEANLEFT, eKI_Q);
  pMap->BindAction(ACTION_LEANLEFT, eKI_NP_7);

  //lean right
  pMap->BindAction(ACTION_LEANRIGHT, eKI_E);
  pMap->BindAction(ACTION_LEANRIGHT, eKI_NP_9);

  //change to distinct weapons(outside common key bindings because is not possible in zoom)
  /*
  pMap->BindAction(ACTION_WEAPON_0,eKI_0);
  pMap->BindAction(ACTION_WEAPON_1,eKI_1);
  pMap->BindAction(ACTION_WEAPON_2,eKI_2);
  pMap->BindAction(ACTION_WEAPON_3,eKI_3);
  pMap->BindAction(ACTION_WEAPON_4,eKI_4);
  pMap->BindAction(ACTION_WEAPON_5,eKI_5);
  pMap->BindAction(ACTION_WEAPON_6,eKI_6);
  pMap->BindAction(ACTION_WEAPON_7,eKI_7);
  pMap->BindAction(ACTION_WEAPON_8,eKI_8);
  //Scroll up through weapons
  pMap->BindAction(ACTION_PREV_WEAPON,eKI_PAGE_UP);
  //Scroll down through weapons
  pMap->BindAction(ACTION_NEXT_WEAPON,eKI_PAGE_DOWN);
  */

  //zoom
  pMap->BindAction(ACTION_ZOOM_IN, eKI_MouseWheelUp);
  pMap->BindAction(ACTION_ZOOM_IN, eKI_NP_Add);

  pMap->BindAction(ACTION_ZOOM_OUT, eKI_MouseWheelDown);
  pMap->BindAction(ACTION_ZOOM_OUT, eKI_NP_Substract);

  //jump (outside common key bindings because space is reserved for holding the breath
  //in zoom mode)
  pMap->BindAction(ACTION_JUMP, eKI_Space);
  pMap->BindAction(ACTION_JUMP, eKI_NP_Enter);

  //thermal vision
  pMap->BindAction(ACTION_ITEM_1, eKI_T);

  //fire (outside common key bindings because is not possible in binozoom)
  // right now used to scan the AI when in zoom mode
  //pMap->BindAction(ACTION_USE,eKI_LMB);

  //////////////////////////////////////////////////////////////////////
  pMap = m_pIActionMapManager->CreateActionMap("vehicle");

  SetCommonKeyBindings(pMap);

  //fire (outside common key bindings because is not possible in binozoom)
  pMap->BindAction(ACTION_FIRE0, eKI_Mouse1);

  // breaks (actually use - to jump out of the car)
  pMap->BindAction(ACTION_JUMP, eKI_Space);
  pMap->BindAction(ACTION_JUMP, eKI_NP_Enter);

  //accellerate
  pMap->BindAction(ACTION_VEHICLE_BOOST, eKI_Up);

  // switch between 1st and 3rd pesron while driving
  //pMap->BindAction(ACTION_CHANGE_VIEW,eKI_RMB);
  pMap->BindAction(ACTION_CHANGE_VIEW, eKI_F1);

  //thermal vision
  pMap->BindAction(ACTION_ITEM_1, eKI_T);

  //////////////////////////////////////////////////////////////////////
  pMap = m_pIActionMapManager->CreateActionMap("player_dead");

  pMap->BindAction(ACTION_FIRE0, eKI_Mouse1);

  pMap->BindAction(ACTION_MOVE_LEFT, eKI_Left);
  pMap->BindAction(ACTION_MOVE_RIGHT, eKI_Right);
  pMap->BindAction(ACTION_TAKESCREENSHOT, eKI_F12);
  //	pMap->BindAction(ACTION_QUICKLOAD,eKI_F6);
  pMap->BindAction(ACTION_FULLSCRN_TOOGLE, eKI_Enter, eMM_LAlt);
  pMap->BindAction(ACTION_FULLSCRN_TOOGLE, eKI_Enter, eMM_RAlt);


  //////////////////////////////////////////////////////////////////////
  //switch to default action map now

  m_pIActionMapManager->SetActionMap("default");
}

void CGame::SetCommonKeyBindings(IActionMap* pMap)
{
  //movements
  //strafe left

  //eKI_Minus,
  pMap->BindAction(ACTION_MOVE_LEFT, eKI_A);
  pMap->BindAction(ACTION_MOVE_LEFT, eKI_NP_4);
  pMap->BindAction(ACTION_MOVE_LEFT,eKI_XI_DPadLeft);

  //strafe right
  pMap->BindAction(ACTION_MOVE_RIGHT, eKI_D);
  pMap->BindAction(ACTION_MOVE_RIGHT, eKI_NP_6);
  pMap->BindAction(ACTION_MOVE_RIGHT,eKI_XI_DPadRight);

  //run forward
  pMap->BindAction(ACTION_MOVE_FORWARD, eKI_W);
  pMap->BindAction(ACTION_MOVE_FORWARD,eKI_NP_8);
  pMap->BindAction(ACTION_MOVE_FORWARD, eKI_NP_5);
  pMap->BindAction(ACTION_MOVE_FORWARD,eKI_XI_DPadUp);

#if 0
  pMap->BindAction(ACTION_MOVELR, eKI_J_AXIS_1);
  pMap->BindAction(ACTION_MOVEFB, eKI_J_AXIS_2);
#endif

  //run backward
  pMap->BindAction(ACTION_MOVE_BACKWARD, eKI_S);
  pMap->BindAction(ACTION_MOVE_BACKWARD,eKI_NP_5);
  pMap->BindAction(ACTION_MOVE_BACKWARD, eKI_NP_2);
  pMap->BindAction(ACTION_MOVE_BACKWARD,eKI_XI_DPadDown);

  //look around
  pMap->BindAction(ACTION_TURNLR,eKI_MouseX);
  pMap->BindAction(ACTION_TURNUD,eKI_MouseY);
  pMap->BindAction(ACTION_TURNLR, eKI_XI_ThumbRX);
  pMap->BindAction(ACTION_TURNUD, eKI_XI_ThumbRY);

  //reload
  pMap->BindAction(ACTION_RELOAD, eKI_R);

  //change firemode
  //pMap->BindAction(ACTION_FIREMODE,eKI_F);
  pMap->BindAction(ACTION_FIREMODE, eKI_X);
  pMap->BindAction(ACTION_FIREMODE, eKI_NumLock);

  //throw grenade
  pMap->BindAction(ACTION_FIRE_GRENADE, eKI_G);
  //pMap->BindAction(ACTION_FIRE_GRENADE, eKI_DIVIDE);
  //pMap->BindAction(ACTION_FIRE_GRENADE, eKI_NP_8);

  //use
  pMap->BindAction(ACTION_USE,eKI_Enter);
  pMap->BindAction(ACTION_USE, eKI_F);
  pMap->BindAction(ACTION_USE,eKI_NP_Enter);

  //cycle grenade
  pMap->BindAction(ACTION_CYCLE_GRENADE, eKI_H);

  //flashlight
  pMap->BindAction(ACTION_FLASHLIGHT, eKI_L);
  pMap->BindAction(ACTION_FLASHLIGHT, eKI_NP_Divide);

  ////run
  pMap->BindAction(ACTION_WALK, eKI_Z);
  //pMap->BindAction(ACTION_WALK,eKI_LEFT);

  //run sprint
  pMap->BindAction(ACTION_RUNSPRINT, eKI_LShift);

  //crouch
  pMap->BindAction(ACTION_MOVEMODE, eKI_LCtrl);
  //pMap->BindAction(ACTION_MOVEMODE,eKI_RIGHT);
  //pMap->BindAction(ACTION_MOVEMODE,eKI_C);
  pMap->BindAction(ACTION_MOVEMODE, eKI_NP_0);

  //prone
  pMap->BindAction(ACTION_MOVEMODE2, eKI_V);
  //pMap->BindAction(ACTION_MOVEMODE2,eKI_NP_1);
  //pMap->BindAction(ACTION_MOVEMODE2,eKI_PAGE_DOWN);

  //mission reminder
  pMap->BindAction(ACTION_SCORE_BOARD, eKI_Tab);

  //concentration feature
  //pMap->BindAction(ACTION_CONCENTRATION,eKI_K);

  //chat
  pMap->BindAction(ACTION_MESSAGEMODE, eKI_Y);
  pMap->BindAction(ACTION_MESSAGEMODE2, eKI_U);

  // save/load bindings
  // disable this conflicts with cutscene
  pMap->BindAction(ACTION_QUICKLOAD,eKI_F6);
  pMap->BindAction(ACTION_QUICKSAVE,eKI_F5);
  pMap->BindAction(ACTION_TAKESCREENSHOT, eKI_F12);

  pMap->BindAction(ACTION_FULLSCRN_TOOGLE, eKI_Enter, eMM_LAlt);
  pMap->BindAction(ACTION_FULLSCRN_TOOGLE, eKI_Enter, eMM_RAlt);

#ifdef _DEBUG
  // <<FIXME>> Hack only in debug mode
  pMap->BindAction(ACTION_SAVEPOS, eKI_F9);
  pMap->BindAction(ACTION_LOADPOS, eKI_F10);
#endif
}

//////////////////////////////////////////////////////////////////////////
// Bind a key to a action in a specified actionmap.
void CGame::BindAction(const char *sAction,const char *sKeys,const char *sActionMap, int iKeyPos)
{
	ActionsEnumMapItor itor;
	// find action
	itor=m_mapActionsEnum.find(sAction);
	if(itor==m_mapActionsEnum.end())
		return;
	IActionMap *pMap=NULL;
	// if no actionmap specified we use the default one
	if(!sActionMap)
		pMap=m_pIActionMapManager->GetActionMap("default");
	else
		pMap=m_pIActionMapManager->GetActionMap(sActionMap);
	// bind
	pMap->BindAction(itor->second.nId,sKeys, 0, iKeyPos);
}

//////////////////////////////////////////////////////////////////////////
// Binds a key (or other input) to a certain action (eg. JUMP) in all action-maps associated with this action
void CGame::BindActionMultipleMaps(const char *sAction,const char *sKeys, int iKeyPos)
{
	XBind Bind;
	//Bind.nKey=m_pSystem->GetIInput()->GetKeyID(sKeys);
	Bind.nKey=m_pSystem->GetIInput()->GetSymbolByName(sKeys)->keyId;
	// find action
	ActionsEnumMapItor It=m_mapActionsEnum.find(sAction);
	if (It!=m_mapActionsEnum.end())
	{
		ActionInfo &Info=It->second;
		for (Vec2StrIt Itor=Info.vecSetToActionMap.begin();Itor!=Info.vecSetToActionMap.end();++Itor)
		{
			for (ActionsEnumMapItor RemIt=m_mapActionsEnum.begin();RemIt!=m_mapActionsEnum.end();++RemIt)
			{
				if (RemIt->second.nId!=Info.nId)
				{
					for (Vec2StrIt RemItor=RemIt->second.vecSetToActionMap.begin();RemItor!=RemIt->second.vecSetToActionMap.end();++RemItor)
					{
						if (strcmp((*RemItor).c_str(), (*Itor).c_str())==0)
						{
							for (Vec2StrIt RemItor=RemIt->second.vecSetToActionMap.begin();RemItor!=RemIt->second.vecSetToActionMap.end();++RemItor)
							{
								IActionMap *pMap=m_pIActionMapManager->GetActionMap((*RemItor).c_str());
								if (pMap)
									pMap->RemoveBind(RemIt->second.nId, Bind, RemIt->second.ActivationMode);
							}
							goto NextAction;
						}
					}
				}
NextAction:; 
			}
			BindAction(sAction, sKeys, (*Itor).c_str(), iKeyPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Check if a action is triggered.
bool CGame::CheckForAction(const char *sAction)
{
	ActionsEnumMapItor itor;
	// find action
	itor=m_mapActionsEnum.find(sAction);
	if(itor==m_mapActionsEnum.end())
		return false;
	// triggered ?
	if(m_pIActionMapManager->CheckActionMap(itor->second.nId))
		return true;
	return false;
} 

//////////////////////////////////////////////////////////////////////
// Clears a action-flag (untrigger)
void CGame::ClearAction(const char *sAction)//<<FIXME>> remove
{

}

#if 0
#endif