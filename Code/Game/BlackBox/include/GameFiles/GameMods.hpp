
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
//	
//	File: GameMods.h
//  Description:	Implements game modding functions. 
// 
//	History:
//	- 13/1/2004: Created by Marco Corbetta and Timur Davidenko
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#ifndef __GameMods_h__
#define __GameMods_h__

#include <BlackBox/Core/IGame.hpp>
#include <vector>
#include <string>

class CGame;
struct ILog;

//////////////////////////////////////////////////////////////////////////
typedef std::vector<SGameModDescription*> lstMods;
typedef lstMods::iterator lstModsIt;

/*!	Implement IGameMods interface to access and manipulate Game Mods.
*/
//////////////////////////////////////////////////////////////////////////
class CGameMods : public IGameMods
{
public:
	CGameMods( CGame *pGame );
	~CGameMods();
	
	// IGameMods implementation.
	virtual const SGameModDescription* GetModDescription( const char *sModName ) const;
	virtual const char* GetCurrentMod() const;
	virtual bool SetCurrentMod( const char *sModName,bool bNeedsRestart=false );
	virtual const char* GetModPath(const char *szSource);	

	//! Array of mod descriptions.
	lstMods m_mods;

private:

	//! Go thru Mods directory and find out what mods are installed.
	SGameModDescription* Find( const char *sModName ) const;
	void	ScanMods();	
	void	ClearMods();
	void	CloseMod(SGameModDescription *pMod);

	bool	ParseModDescription(const char *szFolder,SGameModDescription*pMod);
	bool	GetValue(const char *szKey,const char *szText,char *szRes);
	
	CGame* m_pGame;
	ISystem *m_pSystem;
	ILog		*m_pILog;

	// Name of the mod currently active.
  std::string m_sCurrentMod;
	// Current mod
	SGameModDescription *m_pMod;
	// Used to return the path to other modules
	std::string m_sReturnPath;
};

#endif // __GameMods_h__

