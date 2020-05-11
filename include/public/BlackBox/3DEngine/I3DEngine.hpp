#ifndef CRY3DENGINEINTERFACE_H
#define CRY3DENGINEINTERFACE_H

#include <BlackBox/Core/Platform/Platform.hpp>


#ifdef BB_3DENGINE_EXPORTS
  #define BB_3DENGINEENGINE_API DLL_EXPORT
#else
  #define BB_3DENGINEENGINE_API DLL_IMPORT
#endif

// !!! Do not add any headers here !!!
#include <BlackBox/System/IProcess.hpp>
#include <BlackBox/3DEngine/IStatObj.hpp>
// !!! Do not add any headers here !!!

struct ISystem;
struct ITexPic;
struct CVars;
struct IMatInfo;
struct IEntityRender;

//////////////////////////////////////////////////////////////////////////
//! structure to pass statobj group properites
struct IStatInstGroup
{
	IStatInstGroup() 
	{ 
		pStatObj = 0;
		bHideability = 0;
		bPhysNonColl = 0;
		fBending = 0;
		bCastShadow = 0;
		bRecvShadow = 0;
		bPrecShadow = true;
		bUseAlphaBlending = 0;
		fSpriteDistRatio = 1.f;
		fShadowDistRatio = 1.f;
		fMaxViewDistRatio= 1.f;
		fBrightness = 1.f;
		bUpdateShadowEveryFrame = 0;
		nSpriteTexRes = 0;
		pMaterial = 0;
    fBackSideLevel = 1.f;
    bCalcLighting = true;
    bUseSprites = true;
		bFadeSize = true;
	}
	struct IStatObj * pStatObj;
	bool	bHideability;
	bool	bPhysNonColl;
	float fBending;
	bool	bCastShadow;
	bool	bRecvShadow;
	bool	bPrecShadow;
	bool	bUseAlphaBlending;
	float fSpriteDistRatio;
	float fShadowDistRatio;
	float fMaxViewDistRatio;
	float	fBrightness;
	bool  bUpdateShadowEveryFrame;
	int		nSpriteTexRes;
  float fBackSideLevel;
  bool  bCalcLighting;
  bool  bUseSprites;
	bool  bFadeSize;
	//! Override material for this instance group.
	IMatInfo *pMaterial;
	//! flags similar to entity render flags
	int m_dwRndFlags;
};

//////////////////////////////////////////////////////////////////////////
// Summary:
//     Provide information about the different VisArea volumes
struct IVisArea
{
	// Summary:
	//     Gets the last rendered frame id
	// Return Value:
	//     An int which contrain the frame id.
	virtual int GetVisFrameId() = 0;
	
	// Description:
	//     Gets a list of all the VisAreas which are connected to the current one. 
	// Arguments:
	//     pAreas               - Pointer to an array of IVisArea*
	//     nMaxConnNum          - The maximum of IVisArea to write in pAreas
	//     bSkipDisabledPortals - Ignore portals which are disabled
	// Return Value:
	//     An integer which hold the amount of VisArea found to be connected. If 
	//     the return is equal to nMaxConnNum, it's possible that not all 
	//     connected VisAreas were returned due to the restriction imposed by the 
	//     argument.
	// Summary:
	//     Gets all the areas which are connected to the current one
	virtual	int GetVisAreaConnections(IVisArea ** pAreas, int nMaxConnNum, bool bSkipDisabledPortals = false) = 0;

	// Summary:
	//     Determine if it's connected to an outdoor area
	// Return Value:
	//     Return true if the VisArea is connected to an outdoor area.
	virtual bool IsConnectedToOutdoor() = 0;

	// Summary:
	//     Gets the name
	// Note:
	//     The name is always returned in lower case.
	// Return Value:
	//     A null terminated char array containing the name of the VisArea.
	virtual const char * GetName() = 0;

	// Summary:
	//     Determine if this VisArea is a portal
	// Return Value:
	//     true if the VisArea is a portal, or false in the opposite case.
	virtual bool IsPortal() = 0;

	// Description: 
	//     Search for a specified VisArea to see if it's connected to the current 
	//     VisArea.
	// Arguments:
	//     pAnotherArea         - A specified VisArea to find
	//     nMaxRecursion        - The maximum number of recursion to do while searching
	//     bSkipDisabledPortals - Will avoid searching disabled VisAreas
	// Return Value:
	//     true if the VisArea was found.
	// Summary:
	//     Search for a specified VisArea
	virtual bool FindVisArea(IVisArea * pAnotherArea, int nMaxRecursion, bool bSkipDisabledPortals) = 0;

	// Summary:
	//     Determine if it's affected by outdoor lighting
	// Return Value:
	//     Return true if the VisArea if it's affected by outdoor lighting, else
	//     false will be returned.
	virtual bool IsAfectedByOutLights() = 0;
};

//////////////////////////////////////////////////////////////////////////
enum EVertsSharing
{
  evs_NoSharing=0,
  evs_ShareAndSortForCache=1,
};

// water level unknown
#define WATER_LEVEL_UNKNOWN -1000000

//! Interface to the 3d engine dll

// Summary:
//     Interface to the 3d Engine
struct I3DEngine : public IProcess
{
  //! Enable/Disable the 3d engine update and rendering process

  // Summary:
  //     Enable or disable the 3D Engine
  // Arguments:
  //     bEnable - true indicate the Engine should be enabled, while the 
  //               false would disable it
  virtual void Enable(bool bEnable) = 0;

  //! Initialize 3dengine (call once, after creations)

  // Summary:
  //     Initialize the 3D Engine
  // See Also:
  //     ShutDown
  // Note:
  //     Only call once, after creating the instance.
  virtual bool Init() = 0;

  // Summary:
  //     Set the path used to load levels
  // See Also:
  //     LoadLevel
  // Arguments:
  //     szFolderName - Should contains the folder to be used
  virtual void SetLevelPath(const char* szFolderName) = 0;

  // Description:
  //     Will load a level from the folder specified with SetLevelPath. If a 
  //     level is already loaded, the resources will be deleted before.
  // See Also:
  //     SetLevelPath
  // Arguments:
  //     szFolderName - Name of the subfolder to load
  //     szMissionName - Name of the mission
  //     bEditorMode - If called from the editor
  // Return Value:
  //     A boolean which indicate the result of the function; true is 
  //     succeed, or false if failled.
  // Summary:
  //     Load a level
  virtual bool LoadLevel(const char* szFolderName, const char* szMissionName, bool bEditorMode = false) = 0;

  // Summary:
  //     Update the 3D Engine 
  // Note:
  //     Should be called for every frame.
  virtual void Update() = 0;

  // Summary:
  //     Set the camera
  // See Also:
  //     Draw
  // Note:
  //     Must be called before Draw.
  // Arguments:
  //     cam - ...
  //     bToTheScreen - ...
  virtual void SetCamera(const CCamera& cam, bool bToTheScreen = true) = 0;

  // Summary:
  //     Draw the world
  // See Also:
  //     SetCamera
  virtual void Draw() = 0;
  //
  // Summary:
  //     Shutdown the 3D Engine
  // Arguemnts:
  //     bEditorMode - Indicate if the 3D Engine was used by the editor
  virtual void ShutDown(bool bEditorMode = false) = 0;

  // Summary:
  //     Delete the 3D Engine instance
  virtual void Release() = 0;

  //DOC-IGNORE-BEGIN
  //! Activates lighting for indoors (for debug only)
  virtual void ActivateLight(const char* szName, bool bActivate) = 0;
  //DOC-IGNORE-END

  /*! Load cgf file and create non animated object.
      Returns pointer to already loaded object with same name if found.
      Reference counting used */

      // Summary:
      //     Loads a static object from a cgf file
      // See Also:
      //     IStatObj
      // Arguments:
      //     szFileName -
      //     szGeomName -
      //     eVertsSharing -
      //     bLoadAdditionalInfo -
      // Return Value:
      //     A pointer to an object derived from IStatObj.
  virtual IStatObj* MakeObject(const char* szFileName, const char* szGeomName = 0,
    EVertsSharing eVertsSharing = evs_ShareAndSortForCache,
    bool bLoadAdditinalInfo = true,
    bool bKeepInLocalSpace = false) = 0;

  // Description:
  //     Will reduce the reference count of the static object. If this count 
  //     result in zero, the object will be deleted from memory.
  // Arguments:
  //     A pointer to a static object
  // Return Value:
  //     The value true is return if the object was valid, else false is returned.
  // Summary:
  //     Release the static object
  virtual bool ReleaseObject(IStatObj* pObject) = 0;

  // Summary:
  //     Creates an empty static object
  // Return Value:
  //     A pointer to a static object.
  virtual IStatObj* MakeObject() = 0;

  // Summary:
  //     Gets the amount of loaded objects
  // Return Value:
  //     An integer representing the amount of loaded objects.
  virtual int GetLoadedObjectCount() { return 0; }

  // Summary:
  //     Registers an entity to be rendered
  // Arguments:
  //     pEntity - The entity to render
  virtual void RegisterEntity(IEntityRender* pEntity) = 0;

  // Summary:
  //     Notices the 3D Engine to stop rendering a specified entity
  // Arguments:
  //     pEntity - The entity to stop render
  virtual bool UnRegisterEntity(IEntityRender* pEntity) = 0;

  //DOC-IGNORE-BEGIN
  /*! Unregister all entities in all sectors (or only one if specified)
    Returns true if specified entity was found */
  virtual bool UnRegisterInAllSectors(IEntityRender* pEntity = NULL) = 0;
  //
  //DOC-IGNORE-END
  /*! Set render call back to make possible to
    render user defined objects from outside of 3dengine (used by editor)*/
  virtual void SetRenderCallback(void (*pFunc)(void* pParams), void* pParams) = 0;

  // Summary:
  //     Set the current outdoor ambient color 
  virtual void SetOutdoorAmbientColor(Vec3d vColor) = 0;

  // Summary:
  //     Set to a new sky box
  // Arguments:
  //     szShaderName - Name of the shader used for the sky box
  virtual void SetSkyBox(const char* szShaderName) = 0;

  // Summary:
  //     Set the view distance
  // Arguments:
  //     fMaxViewDistance - Maximum view distance
  virtual void SetMaxViewDistance(float fMaxViewDistance) = 0;

  // Summary:
  //     Gets the view distance
  // Return Value:
  //     A float value representing the maximum view distance.
  virtual float GetMaxViewDistance() = 0;

  //! Set/Get fog params

  // Summary:
  //     Set the fog color
  virtual void SetFogColor(const Vec3& vFogColor) = 0;

  // Summary:
  //   Set the intensity of fog at a close distance
  // See Also:
  //   SetFogEnd
  // Arguments:
  //   fFogStart - Intensity at a close distance, by default 50
  virtual void SetFogStart(const float fFogStart) = 0;

  // Summary:
  //   Set the intensity of fog at a far distance
  // See Also:
  //   SetFogStart
  // Arguments:
  //   fFogEnd - Intensity at a close distance, by default 1500
  virtual void SetFogEnd(const float fFogEnd) = 0;

  // Summary:
  //     Gets the fog color
  virtual Vec3 GetFogColor() = 0;

  // Summary:
  //   Get the intensity of fog at a close distance
  // See Also:
  //   GetFogEnd
  virtual float GetFogStart() = 0;

  // Summary:
  //   Set the intensity of fog at a far distance
  // See Also:
  //   GetFogStart
  virtual float GetFogEnd() = 0;

  // Summary:
  //     Gets the interpolated terrain elevation for a specified location
  // Note:
  //     All x,y values are valid
  // Arguments:
  //     x - X coordinate of the location
  //     y - Y coordinate of the location
  // Return Value:
  //     A float which indicate the elevation level.
  virtual float GetTerrainElevation(float x, float y) = 0;

  // Summary:
  //     Gets the terrain elevation for a specified location
  // Note:
  //     Only values between 0 and WORLD_SIZE.
  // Arguments:
  //     x - X coordinate of the location
  //     y - Y coordinate of the location
  // Return Value:
  //     A float which indicate the elevation level.
  virtual float GetTerrainZ(int x, int y) = 0;

  // Summary:
  //     Gets the unit size of the terrain
  // Note:
  //     The value should currently be 2.
  // Return Value:
  //     A int value representing the terrain unit size in meters.
  virtual int   GetHeightMapUnitSize() = 0;

  //! Returns size of terrain in meters ( currently is 2048 )

  // Summary:
  //     Gets the size of the terrain
  // Note:
  //     The value should be 2048 by default.
  // Return Value:
  //     An int representing the terrain size in meters.
  virtual int   GetTerrainSize() = 0;


  //! Returns size of terrain sector in meters ( currently is 64 )

  // Summary:
  //     Gets the size of the terrain sectors
  // Note:
  //     The value should be 64 by default.
  // Return Value:
  //     An int representing the size of a sector in meters.
  virtual int   GetTerrainSectorSize() = 0;

  //! Maximum view distance in meters ( usualy 512 )

  // Summary:
  //     Gets the maximum view distance
  // Note:
  //     The value should be 512 by default.
  // Return Value:
  //     A float which represent the view distance in meters
  virtual float GetMaxViewDist() = 0;

  //DOC-IGNORE-BEGIN

    // Internal functions, mostly used by the editor, which won't be documented for now

    //! Places object at specified position (for editor)
  virtual bool AddStaticObject(int nObjectID, const Vec3& vPos, const float fScale, unsigned char ucBright = 255) = 0;
  //! Removes static object from specified position (for editor)
  virtual bool RemoveStaticObject(int nObjectID, const Vec3& vPos) = 0;
  //! On-demand physicalization of a static object
  virtual bool PhysicalizeStaticObject(void* pForeignData, int iForeignData, int iForeignFlags) = 0;
  //! Removes all static objects on the map (for editor)
  virtual void RemoveAllStaticObjects() = 0;
  //! Allows to set terrain surface type id for specified point in the map (for editor)
  virtual void SetTerrainSurfaceType(int x, int y, int nType) = 0; // from 0 to 6 - sur type ( 7 = hole )
  /*! Return terrain surface type id for specified point on the map
    Return -1 if point is outside of the map or if there is hole in terrain here ) */
  virtual int  GetTerrainSurfaceType(int x, int y) = 0; // from 0 to 6 - sur type ( 7 = hole )

//! Updates part of hight map (in terrain units, by default update only elevation)
  virtual void SetTerainHightMapBlock(int x1, int y1, int nSizeX, int nSizeY, unsigned short* TerrainBlock, unsigned short nUpdateMask = (((unsigned short)-1) & (~31))) = 0;
  //! Returns true if game modified terrain hight map since last update by editor
  virtual bool IsTerainHightMapModifiedByGame() = 0;

  //! returns terrain sector texture id, texture dimensions and disable streaming on this sector
  //! returns 0 in case of error (wrong SectorOrigin)
  virtual int LockTerrainSectorTexture(int nSectorOriginX, int nSectorOriginY, int& nTexDim) = 0;

  //! Set group parameters
  virtual bool SetStatInstGroup(int nGroupId, const IStatInstGroup& siGroup) = 0;

  //! Get group parameters
  virtual bool GetStatInstGroup(int nGroupId, IStatInstGroup& siGroup) = 0;

  //! returns dimensions of entire terrain texture
  virtual int GetTerrainTextureDim() = 0;

  //! Set burbed out flag
  virtual void SetTerrainBurnedOut(int x, int y, bool bBurnedOut) = 0;
  //! Get burbed out flag
  virtual bool IsTerrainBurnedOut(int x, int y) = 0;

  // Summary:
  //   Draws a quad on the entire screen using a specified shader
  // Note:
  //   Used for special night vision effect.
  // Arguments:
  //   szShaderName - Name of the shader to use
  virtual void SetScreenShader(const char* szShaderName) = 0;

#if 0
  //! Set physics material enumerator

  // Summary:
  //   Set the physics material enumerator
  // Arguments:
  //   pPhysMaterialEnumerator - The physics material enumarator to set
  virtual void SetPhysMaterialEnumerator(IPhysMaterialEnumerator* pPhysMaterialEnumerator) = 0;

  // Summary:
  //   Gets the physics material enumerator
  // Return Value:
  //   A pointer to an IPhysMaterialEnumerator derived object.
  virtual IPhysMaterialEnumerator* GetPhysMaterialEnumerator() = 0;
#endif

  //DOC-IGNORE-BEGIN
  //Internal functions

    //! Allows to enable fog in editor
  virtual void SetupDistanceFog() = 0;

  //! Load environment settings for specified mission
  virtual void LoadEnvironmentSettingsFromXML(const char* szMissionName, bool bEditorMode, const char* szMissionXMLString = 0, bool bUpdateLightingOnVegetations = true) = 0;

  //! Load detail texture and detail object settings from XML doc (load from current LevelData.xml if pDoc is 0)
  virtual void	LoadTerrainSurfacesFromXML(void* pDoc = NULL) = 0;

  //Unused

    //! Recalculate shore geometry
  virtual void UpdateBeaches() = 0;
  //DOC-IGNORE-END

    //! Returns true if point is in water

    // Summary:
    //   Determines if a specified position is in the water
    // Arguments:
    //   vPos - The position to evaluate
    // Return Value:
    //   A boolean with the value true if the position is in the water, else false is returned.
  virtual bool IsPointInWater(Vec3 vPos) = 0;

  //! Returns true if point is in the building
  //virtual bool IsPointInsideIndoors(const Vec3 & vPos) = 0;

  //! Creates new light source in the world to be used during this frame (or longer)

  // Summary:
  //   Creates a new dynamic light source
  // Description:
  //   The new light source will be used during this frame, and maybe longer.
  // Arguments:
  //   LSource - ...
  //   pEnt - ...
  //   nEntityLightId - ...
  //   pMatrix - ...
  virtual void AddDynamicLightSource(const class CDLight& LSource, IEntityRender* pEnt, int nEntityLightId = -1, const Matrix44* pMatrix = NULL) = 0;

  //! Make move/bend vegetations in specified area (not implemented yet)

  // Description:
  //   Physics applied to the area will apply to vegetations and allow it to move/blend.
  // Arguments:
  //   vPos - Center position to apply physics
  //   fRadius - Radius which specify the size of the area to apply physics
  //   fAmountOfForce - The amount of force, should be at least of 1.0f
  // Summary:
  //   Applies physics in a specified area
  virtual void ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) = 0;

  //DOC-IGNORE-BEGIN
  // Internal function used by the 3d engine and renderer
    //! Return sun position (if bMoveUp=true sun will be 30% higher, it makes shadows from objects not so long)
  virtual Vec3 GetSunPosition(bool bMoveUp = true) = 0;

  // Internal function used by the 3d engine and editor
    //! Returns light mask for this point (valid only during rendering stage)
  virtual unsigned int GetLightMaskFromPosition(const Vec3& vPos, float fRadius = 1.f) = 0;

  // Internal function used by the 3d engine
    //! Returns lighting level for this point
  virtual Vec3 GetAmbientColorFromPosition(const Vec3& vPos, float fRadius = 1.f) = 0;

  // Never used
    //! Returns fog volume id
  virtual int GetFogVolumeIdFromBBox(const Vec3& vBoxMin, const Vec3& vBoxMax) = 0;

  //! Return surface normal at specified position
  //virtual Vec3 GetTerrainSurfaceNormal(Vec3 vPos) = 0;

//Internal function used by 3d engine and editor

  //! Render shadows of objects into frame buffer and read this picture
  virtual bool MakeSectorLightMap(int nSectorOriginX, int nSectorOriginY, unsigned char* pImage, int nImageSize) = 0;

  //Internal function used by 3d engine and renderer
    //! get distance to the sector containig ocean water
  virtual float GetDistanceToSectorWithWater() = 0;
  //DOC-IGNORE-END

    //! allows to slowly replace skybox with bg(fog) color

    // Summary:
    //   Set an alpha value to apply to the sky box
    // Arguments:
    //   fAlpha - A value between 0.0f to 1.0f
  virtual void SetSkyBoxAlpha(float fAlpha /* from 0 to 1 */) = 0;

  // Summary:
  //   Add the level's path to a specified filename
  // Arguments:
  //   szFileName - The filename for which we need to add the path
  // Return Value:
  //   Full path for the filename; including the level path and the filename appended after.
  virtual const char* GetLevelFilePath(const char* szFileName) = 0;

  // Summary:
  //   Displays statistic on the 3d Engine
  // Arguments:
  //   fTextPosX - X position for the text
  //   fTextPosY - Y position for the text
  //   fTextStepY - Amount of pixels to distance each line
  virtual void DisplayInfo(float& fTextPosX, float& fTextPosY, float& fTextStepY) = 0;

  // Summary:
  //   Creates a new VisArea
  // Return Value:
  //   A pointer to a newly created VisArea object
  virtual IVisArea* CreateVisArea() = 0;

  //! Delete visarea

  // Summary:
  //   Deletes a VisArea
  // Arguments:
  //   pVisArea - A pointer to the VisArea to delete
  virtual void DeleteVisArea(IVisArea* pVisArea) = 0;

  //! set screen mask texture
  virtual void SetScreenMask(ITexPic* pMask) = 0;

  //! set screen fx type
  virtual void SetScreenFx(const char* pEffectName, int iActive) = 0;

  //! set current active camera focus position
  //virtual int SetCameraFocus(const Vec3 &pPos)=0;

  //! set screen fx parameter
  virtual void SetScreenFxParam(const char* pEffectName, const char* pEffectParam, void* pValue) = 0;

  //! get screen fx type
  virtual int GetScreenFx(const char* pEffectName) = 0;

  //! get screen fx parameter
  virtual int GetScreenFxParam(const char* pEffectName, const char* pEffectParam, void*& pValue) = 0;

  //! reset current screen effects
  virtual void ResetScreenFx(void) = 0;
};


extern "C" {
  // Create3DEngine function type definition
  typedef I3DEngine* (*PFNCREATE3DENGINE)(ISystem* pSystem, const char* szInterfaceVersion);

  //! Creates 3dengine instance

  // Description:
  //     Create an instance of the 3D Engine. It should usually be called by 
  //     ISystem::Init3DEngine.
  // See Also:
  //     I3DEngine, I3DEngine::Release
  // Arguments:
  //     ISystem            - Pointer to the current ISystem instance
  //     szInterfaceVersion - String version of with the build date
  // Summary:
  //     Create an instance of the 3D Engine
  BB_3DENGINEENGINE_API I3DEngine * Create3DEngine(ISystem	* pSystem,const char * szInterfaceVersion);
}

#endif
