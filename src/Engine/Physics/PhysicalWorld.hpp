
class CPhysicalWorld : public IPhysicalWorld
{
	public:
	CPhysicalWorld(ISystem* pLog)
	{
	}
	/*! Inits world
		@param pconsole pointer of IConsole interace
	*/
	virtual void Init() = 0;

	virtual IGeomManager* GetGeomManager() = 0;
	virtual IPhysUtils* GetPhysUtils() = 0;

	/*! Shuts the world down
	*/
	virtual void Shutdown(int bDeleteGeometries = 1) = 0;
	/*! Destroys the world
	*/
	virtual void Release() = 0;

	/*! Initializes entity hash grid
		@param axisx id of grid x axis (0-world x,1-world y,2-world z)
		@param axisy id of grid y axis (0-world x,1-world y,2-world z)
		@param origin grid (0,0) in world CS
		@param nx number of cells in grid x direciton
		@param ny number of cells in grid y direciton
		@param stepx cell x dimension
		@param stepy cell y dimension
	*/
	virtual void SetupEntityGrid(int axisz, vectorf org, int nx,int ny, float stepx,float stepy) = 0;
	/*! Sets heightfield data
		@param phf
	*/
	virtual void SetHeightfieldData(const primitives::heightfield *phf) = 0;
	/*! Retrieves heightfield data
		@param phf
		@return 0 if no global heightfield, 1 otherwise
	*/
	virtual int GetHeightfieldData(primitives::heightfield *phf) = 0;
	/*! Retrieves pointer to physvars structure
		@return pointer to physvar structure
	*/
	virtual PhysicsVars *GetPhysVars() = 0;

	/*! Creates physical entity
		@param type entity type
		@param params initial params (as in entity SetParams)
		@param pforeigndata entity foreign data
		@param iforeigndata entity foreign data type identifier
		@return pointer to physical entity interface
	*/
	virtual IPhysicalEntity* CreatePhysicalEntity(pe_type type, pe_params* params=0, void *pforeigndata=0, int iforeigndata=0, int id=-1) = 0;
	virtual IPhysicalEntity* CreatePhysicalEntity(pe_type type, float lifeTime, pe_params* params=0, void *pForeignData=0,int iForeignData=0,
		int id=-1,IPhysicalEntity *pHostPlaceholder=0) = 0;
	virtual IPhysicalEntity *CreatePhysicalPlaceholder(pe_type type, pe_params* params=0, void *pForeignData=0,int iForeignData=0, int id=-1) = 0;
	/*! Destroys physical entity
		@param pent entity
		@param mode 0-normal destroy, 1-suspend, 2-restore from suspended state
		@return nonzero if success
	*/
	virtual int DestroyPhysicalEntity(IPhysicalEntity *pent, int mode=0) = 0;

	virtual int SetPhysicalEntityId(IPhysicalEntity *pent, int id, int bReplace=1) = 0;
	virtual int GetPhysicalEntityId(IPhysicalEntity *pent) = 0;
	virtual IPhysicalEntity* GetPhysicalEntityById(int id) = 0;

	/*! Sets surface parameters
		@param surface_idx surface identifier
		@param bounciness restitution coefficient (for pair of surfaces k = sum of their coefficients, clamped to [0..1]
		@param friction friction coefficient (for pair of surfaces k = sum of their coefficients, clamped to [0..inf)
		@param flags bitmask (see surface_flags enum)
		@return nonzero if success
	*/
	virtual int SetSurfaceParameters(int surface_idx, float bounciness,float friction, unsigned int flags=0) = 0;
	virtual int GetSurfaceParameters(int surface_idx, float &bounciness,float &friction, unsigned int &flags) = 0;

	/*! Perfomes a time step
		@param time_interval time interval
		@param flags entity types to update (ent_..; ent_deleted to purge deletion physics-on-demand state monitoring)
	*/
	virtual void TimeStep(float time_interval,int flags=ent_all|ent_deleted) = 0;
	/*! Returns current time of the physical world
		@return current time
	*/
	virtual float GetPhysicsTime() = 0;
	virtual int GetiPhysicsTime()  = 0;
	/*! Sets current time of the physical world
		@param time new time
	*/
	virtual void SetPhysicsTime(float time) = 0;
	virtual void SetiPhysicsTime(int itime) = 0;
	/*! Sets physical time that corresponds to the following server state snapshot
		@param time_snapshot physical time of the following server snapshot
	*/
	virtual void SetSnapshotTime(float time_snapshot,int iType=0) = 0;
	virtual void SetiSnapshotTime(int itime_snapshot,int iType=0) = 0;

	/*! Retrives list of entities that fall into a box
		@param ptmix,ptmax - box corners
		@param pList returned pointer to entity list
		@param objtypes	bitmask 0-static, 1-sleeping, 2-physical, 3-living
		@return number of entities
	*/
	virtual int GetEntitiesInBox(vectorf ptmin,vectorf ptmax, IPhysicalEntity **&pList, int objtypes) = 0;

	/*! Shoots ray into world
		@param origin origin
		@param dir direction*(ray length)
		@param objtypes	bitmask 0-terrain 1-static, 2-sleeping, 3-physical, 4-living
		@param flags a combination of rwi_flags
		@param hits destination hits array
		@param nmaxhits size of this array
		@param pskipent entity to skip
		@return number of collisions
	*/
	virtual int RayWorldIntersection(vectorf org,vectorf dir, int objtypes, unsigned int flags, ray_hit *hits,int nMaxHits,
		IPhysicalEntity *pSkipEnt=0,IPhysicalEntity *pSkipEntAux=0) = 0;

	/*! Freezes (resets velocities of) all physical, living, and detached entities
	*/
	virtual void ResetDynamicEntities() = 0;
	/*! Immediately destroys all physical, living, and detached entities; flushes the deleted entities
		All subsequent calls to DestroyPhysicalEntity for non-static entities are ignored until the next
		non-static entity is created
	*/
	virtual void DestroyDynamicEntities() = 0;
	/*! Forces deletion of all entities marked as deleted
	*/
	virtual void PurgeDeletedEntities() = 0;

	/*! Simulates physical explosion with k/(r^2) pressure distribution
		@param epicenter epicenter used for building the occlusion map
		@param epicenterImp epicenter used for applying impulse
		@param rmin all r<rmin are set to rmin to avoid singularity in center
		@param rmax clamps entities father than rmax
		@param r radius at which impulsive pressure is spesified
		@param impulsive_pressure_at_r impulsive pressure at r
		@param nOccRes resolution of occulision cubemap (0 to skip occlusion test)
		@param nGrow inflate dynamic objects' rasterized image by this amount
		@params rmin_occ subtract cube with this size (half length of its side) during rasterization
		@params pSkipEnts pointer to array of entities to skip
		@params nSkipEnts number of entities to skip
	*/
	virtual void SimulateExplosion(vectorf epicenter,vectorf epicenterImp, float rmin,float rmax, float r,float impulsive_pressure_at_r,
		int nOccRes=0,int nGrow=0,float rmin_occ=0.1f, IPhysicalEntity **pSkipEnts=0,int nSkipEnts=0,
		int iTypes=ent_rigid|ent_sleeping_rigid|ent_living|ent_independent) = 0;

	/*! Returns fraction of pent (0-1) that was exposed to the last explosion
	*/
	virtual float IsAffectedByExplosion(IPhysicalEntity *pent) = 0;

	virtual void DrawPhysicsHelperInformation(void (*DrawLineFunc)(float*,float*)) = 0;

	virtual int CollideEntityWithBeam(IPhysicalEntity *_pent, vectorf org,vectorf dir,float r, ray_hit *phit) = 0;
	virtual int RayTraceEntity(IPhysicalEntity *pient, vectorf origin,vectorf dir, ray_hit *pHit, pe_params_pos *pp=0) = 0;

	virtual void GetMemoryStatistics(ICrySizer *pSizer) = 0;

	virtual void SetPhysicsStreamer(IPhysicsStreamer *pStreamer) = 0;
	virtual void SetPhysicsEventClient(IPhysicsEventClient *pEventClient) = 0;
	virtual float GetLastEntityUpdateTime(IPhysicalEntity *pent) = 0;
	virtual int GetEntityProfileInfo(phys_profile_info *&pList) = 0;

	virtual int SerializeWorld(const char *fname, int bSave) = 0;
	virtual int SerializeGeometries(const char *fname, int bSave) = 0;

};
