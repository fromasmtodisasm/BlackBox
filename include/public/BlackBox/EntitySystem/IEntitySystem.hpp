#pragma once
////////////////////////////////////////////////////////////////////////////
// Forward declarations.
class cAIBrain;
class IPhysicalEntity;
class CryCharInstance;
struct IStatObj;
struct pe_cargeomparams;
struct pe_params_car;
struct IEntitySystemSink;

// Forward declare interfaces.
struct IEntitySystem;
struct IEntityCharacter;
struct IEntityContainer;
struct ISystem;
struct ISound;
struct IScriptSystem;
struct IEntityCamera;
struct ICryCharInstance;
struct AIObjectParameters;
struct IAIObject;
struct ILipSync;
struct IParticleEffect;
struct IServerSlot;
struct ICrySizer;

// !!! don't change the type !!!
typedef unsigned short EntityId; //! unique identifier for each entity instance

////////////////////////////////////////////////////////////////////////////
// !!! don't change the type !!!
typedef unsigned short EntityClassId; //! unique identifier for the entity class (defined in ClassRegistry.lua)
typedef ULONG_PTR BoneBindHandle;

class CEntityDesc;

////////////////////////////////////////////////////////////////////////////
// Misc
enum eMiscEnum
{
	PLAYER_MODEL_IDX	= 0,
	MAX_ANIMATED_MODELS = 2
};

////////////////////////////////////////////////////////////////////////////
// object types - bitmask 0-terrain 1-static, 2-sleeping, 3-physical, 4-living
enum PhysicalEntityFlag
{
	PHYS_ENTITY_STATIC	= (1 << 1),
	PHYS_ENTITY_DYNAMIC = (1 << 2) | (1 << 3),
	PHYS_ENTITY_LIVING	= (1 << 4),
	PHYS_ENTITY_ALL		= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4)
};

struct IEntity;

////////////////////////////////////////////////////////////////////////////
/*! Entity iterator interface. This interface is used to traverse trough all the entities in an entity system. In a way,
	this iterator works a lot like a stl iterator.
*/
struct IEntityIt
{
	virtual void AddRef() = 0;

	/*! Deletes this iterator and frees any memory it might have allocated.
	*/
	virtual void Release() = 0;

	/*! Check whether current iterator position is the end position.
		@return True if iterator at end position.
	*/
	virtual bool IsEnd() = 0;

	/*! Retrieves next entity
		@return The entity that the iterator points to before it goes to the next
	*/
	virtual IEntity* Next() = 0;

	/*! Positions the iterator at the begining of the entity list
	*/
	virtual void MoveFirst() = 0;
};

////////////////////////////////////////////////////////////////////////////
// types of dependance of entity update from visibility
enum EEntityUpdateVisLevel
{
	eUT_Always = 0,		 //! Always update entity.
	eUT_InViewRange,	 //! Only update entity if it is in view range.
	eUT_PotVisible,		 //! Only update entity if it is potentially visible.
	eUT_Visible,		 //! Only update entity if it is visible.
	eUT_Physics,		 //! Only update entity if it is need to be updated due to physics.
	eUT_PhysicsVisible,	 //! Only update entity if it is need to be updated due to physics or if it is visible.
	eUT_Never,			 //! Never update entity.
	eUT_PhysicsPostStep, //! Update only when PostStep is called from the physics
	eUT_Unconditional	 //! Update regardless of anything - this has to be explicitly set
};

////////////////////////////////////////////////////////////////////////////
/*! A callback interface for a class that wants to be aware when new entities are being spawned or removed. A class that implements
	this interface will be called everytime a new entity is spawned, removed, or when an entity container is to be spawned.
*/
struct IEntitySystemSink
{
	/*! This callback enables the class which implements this interface a way to spawn containers for the entity that is just in the
	process of spawning. Every entity class that has a container creates it here. NOTE: Although the container is being created here,
	it is not initialized yet (this will be done when the entity being spawned initializes itself).
	@param ed	Entity description of entity which would be contained in this container
	@param pEntity The entity that will hold this container
*/
	virtual void OnSpawnContainer(CEntityDesc& ed, IEntity* pEntity) = 0;

	/*! This callback is called when this entity has finished spawning. The entity has been created and added to the list of entities,
 but has not been initialized yet.
	@param e The entity that was just spawned
*/
	virtual void OnSpawn(IEntity* e, CEntityDesc& ed) = 0;

	/*! Called when an entity is being removed.
	@param e The entity that is being removed. This entity is still fully valid.
*/
	virtual void OnRemove(IEntity* e) = 0;

	virtual void OnBind(EntityId id, EntityId child, unsigned char param) = 0;

	virtual void OnUnbind(EntityId id, EntityId child, unsigned char param) = 0;
};

////////////////////////////////////////////////////////////////////////////
/*! Interface to the system that manages the entities in the game, their creation, deletion and upkeep. The entities are kept in a map
 indexed by their uniqie entity ID. The entity system updates only unbound entities every frame (bound entities are updated by their
 parent entities), and deletes the entities marked as garbage every frame before the update. The entity system also keeps track of entities
 that have to be drawn last and with more zbuffer resolution.
*/
struct IEntitySystem
{
	/*! Update entity system and all entities. This function executes once a frame.
*/
	virtual void Update() = 0;

	/*! Retrieves the script system interface.
	@return Script System Interface
	@see IScriptSystem
*/
	virtual IScriptSystem* GetScriptSystem() = 0;

	/*! Reset whole entity system, and destroy all entities.
*/
	virtual void Reset() = 0;

	/*! Spawns a new entity according to the data in the Entity Descriptor
	@param ed	Entity descriptor structure that describes what kind of entity needs to be spawned
	@param bAutoInit If true automatically initialize entity.
	@return The spawned entity if successfull, NULL if not.
	@see CEntityDesc
*/
	virtual IEntity* SpawnEntity(CEntityDesc& ed, bool bAutoInit = true) = 0;

	/*! Initialize entity if entity was spawned not initialized (with bAutoInit false in SpawnEntity)
		Used only by Editor, to setup properties & other things before initializing entity,
		do not use this directly.
		@param pEntity Pointer to just spawned entity object.
		@param ed	Entity descriptor structure that describes what kind of entity needs to be spawned.
		@return true if succesfully initialized entity.
*/
	virtual bool InitEntity(IEntity* pEntity, CEntityDesc& ed) = 0;

	/*! Retrieves entity from its unique id.
	@param id The unique ID of the entity required
	@return The entity if one with such an ID exists, and NULL if no entity could be matched with the id
*/
	virtual IEntity* GetEntity(EntityId id) = 0;

	/*! Set an entity as the player associated with this client, identified with the entity id
	@param id Unique Id of the entity which is to represent the player
*/
	//virtual	void SetMyPlayer( EntityId id ) = 0;

	/*! Get the entity id of the entity that represents the player.
	@return The entity id as an unsigned short*/

	//virtual EntityId GetMyPlayer() const = 0;

	/*! Find first entity with given name.
	@param name The name to look for
	@return The entity if found, 0 if failed
*/
	virtual IEntity* GetEntity(const char* sEntityName) = 0;
	//! obsolete
	virtual EntityId FindEntity(const char* name) const = 0;

	/*! Remove an entity by ID
	@param entity	The id of the entity to be removed
	@param bRemoveNode If true forces immidiate delete of entity, overwise will delete entity on next update.
*/
	virtual void RemoveEntity(EntityId entity, bool bRemoveNow = false) = 0;

	/*! Get number of entities stored in entity system.
	@return The number of entities
*/
	virtual int GetNumEntities() const = 0;

	/*! Get entity iterator. This iterator interface can be used to traverse all the entities in this entity system.
	@return An entityIterator
	@see IEntityIt
*/
	virtual IEntityIt* GetEntityIterator() = 0;

	/*! Get entity iterator of all visible entities. This iterator interface can be used to traverse all the visible entities in this entity system.
	bFromPrevFrame	- to get entities visible in previouse update
	@return An entityIterator
	@see IEntityIt
*/
	virtual IEntityIt* GetEntityInFrustrumIterator(bool bFromPrevFrame = false) = 0;

/*! Get all entities in specified radius.
		 physFlags is one or more of PhysicalEntityFlag.
	 @see PhysicalEntityFlag
*/
#if defined(LINUX)
#	undef vector //well this was previously defined in physics, so...
#endif
	virtual void GetEntitiesInRadius(const Vec3& origin, float radius, std::vector<IEntity*>& entities, int physFlags = PHYS_ENTITY_ALL) const = 0;

	/*! Add the sink of the entity system. The sink is a class which implements IEntitySystemSink.
	@param sink	Pointer to the sink
	@see IEntitySystemSink
*/
	virtual void SetSink(IEntitySystemSink* sink) = 0;

	virtual void PauseTimers(bool bPause, bool bResume = false) = 0;

	/*! Remove listening sink from the entity system. The sink is a class which implements IEntitySystemSink.
	@param sink	Pointer to the sink
	@see IEntitySystemSink
*/
	virtual void RemoveSink(IEntitySystemSink* sink) = 0;

	/*! Creates an IEntityCamera that can be attached to an entity
	@return Pointer to a new entity camera.
	@see IEntityCamera
*/
	virtual IEntityCamera* CreateEntityCamera() = 0;
	/*! Enable/Disable the Client-Side script calls
	@param bEnable true enable false disable
*/
	virtual void EnableClient(bool bEnable) = 0;
	/*! Enable/Disable the Server-Side script calls
	@param bEnable true enable false disable
*/
	virtual void EnableServer(bool bEnable) = 0;

	//virtual IEntityClonesMgr *CreateEntityClonesMgr()=0;
	/*! Destroys the entity system
*/
	virtual void Release() = 0;

	/*! Checks whether a given entity ID is already used
*/
	virtual bool IsIDUsed(EntityId nID) = 0;

	//! Calls reset for every entity, to reset its state
	virtual void ResetEntities() = 0;

	//! Puts the memory statistics of the entities into the given sizer object
	//! According to the specifications in interface ICrySizer
	virtual void GetMemoryStatistics(ICrySizer* pSizer) = 0;

	//! if this is set to true (usually non editor mode)
	//! only dynamic EntityID s are created, no longer static ones
	virtual void SetDynamicEntityIdMode(const bool bActivate) = 0;

	//! sets the default update level for the entities
	//! every entity spawned after this call will have the specified update level at creation time
	//! the entity can specify it differently later
	virtual void SetDefaultEntityUpdateLevel(EEntityUpdateVisLevel eDefault) = 0;

	//! Set entity system mode into precaching of resources.
	virtual void SetPrecacheResourcesMode(bool bPrecaching) = 0;

	//	virtual CIDGenerator* GetIDGenerator() = 0;
	virtual bool IsDynamicEntityId(EntityId id) = 0;
	virtual void MarkId(EntityId id)			= 0;
	virtual void ClearId(EntityId id)			= 0;
};
