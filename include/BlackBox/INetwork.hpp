#pragma once
#include <BlackBox/Platform.hpp>

#include <vector>				// STL vector<>

////////////////////////////////////////////////////////////////////////////////////////
// Interfaces
////////////////////////////////////////////////////////////////////////////////////////
struct IClient;
struct IServer;
struct IServerSnooper;
struct INETServerSnooper;
struct IClientSink;
struct IServerSlotFactory;
struct IServerSnooperSink;
struct INETServerSnooperSink;
struct IRConSystem;
struct ICompressionHelper;
class ICrySizer;
class CIPAddress;
class CStream;
struct ISystem;

//////////////////////////////////////////////////////////////////////
/*! class factory of the Network module
	@see ::CreateNetwork()
*/
struct INetwork
{
	//! \return local IPAddress (needed if we have several servers on one machine), 0.0.0.0 if not used
	virtual DWORD GetLocalIP() const = 0;

	//! also initialize Ubi.com integration (flaw in the UBI.com SDK - we would like to be able to set the IP later but they
	//! need it during initialization)
	//! \param dwLocalIP local IPAddress (needed if we have several servers on one machine)
	virtual void SetLocalIP(const char* szLocalIP) = 0;

	/*! create a client object and return the related interface
			@param pSink a pointer to an object the inplements IClientSink [the object that will receive all notification during the connection]
			@param bClient if true the client will be only able to connect to the local server and
				will use a "fake connection" (memory based)
			@return an IClient interface
	*/
	virtual IClient* CreateClient(IClientSink* pSink, bool bLocal = false) = 0;

	/*! create and start a server ,return the related interface
			@param pFactory a pointer to an object the inplements IServerSlotFactory
				[the object that will receive all notification during the lifetime of the server]
			@param nPort local IP port where the server will listen
			@return an IServer interface
	*/
	virtual IServer* CreateServer(IServerSlotFactory* pFactory, WORD nPort, bool local = false) = 0;

	//! create an RCon System (remote control system)
	virtual IRConSystem* CreateRConSystem() = 0;
	/*! create an internet server snooper
	@param pSink id of the error
	@return Interface to a server snooper
	*/
	virtual INETServerSnooper* CreateNETServerSnooper(INETServerSnooperSink* pSink) = 0;
	/*! create a server snooper
		@param pSink id of the error
		@return Interface to a server snooper
	*/
	virtual IServerSnooper* CreateServerSnooper(IServerSnooperSink* pSink) = 0;
#if 0
	/*! return the string representation of a socket error
		@param err id of the error
		@return string description of the error
	*/
	virtual const char* EnumerateError(NRESULT err) = 0;
#endif
	//! release the interface(and delete the object that implements it)
	virtual void Release() = 0;
	//!
	virtual void GetMemoryStatistics(ICrySizer* pSizer) = 0;
	//!
	//! \return interface pointer to the compression helper library, is always valid
	virtual ICompressionHelper* GetCompressionHelper() = 0;

	//! Submit to network list files, that must be matching on Client and Server.
	virtual void ClearProtectedFiles() = 0;
	virtual void AddProtectedFile(const char* sFilename) = 0;
	//!
	//! \return 0 if there is no server registered at this port
	virtual IServer* GetServerByPort(const WORD wPort) = 0;
	//! used to update things like the UBI.com services
	virtual void UpdateNetwork() = 0;
	//! currently used to update UBI.com info and check CDKey
	//! If it is a UBI type server we should the register, if we have already registered this will do nothing.
	//! \param szServerName must not be 0
	//! \param dwPlayerCount >0
	virtual void OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const WORD wPort) = 0;
	//! \return true=it's possible (e.g. logged into UBI.com), false=it's not possible
	virtual bool VerifyMultiplayerOverInternet() = 0;
	//! We have to tell Ubisoft that the client has successfully connected
	//! If ubisoft is not running this won't do anything.
	virtual void Client_ReJoinGameServer() = 0;
	//! \return 0 if there is no client
	virtual IClient* GetClient() = 0;

	//! Sets server's IP
	virtual void					SetUBIGameServerIP(const char* szAddress) = 0;
	//! Gets server's IP
	virtual const char* GetUBIGameServerIP(bool bLan) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////
/*! callback interface that must implement by the host that want to use IClient
*/
struct IClientSink
{
	/*! called by the client when the connection occur
	*/
	virtual void OnXConnect() = 0;
	/*! called by the client when the disconnection occur
		@param string representation of the disconnection cause
	*/
	virtual void OnXClientDisconnect(const char* szCause) = 0;
	/*! called by the client when the server send a contex setup.

		NOTES: for instance a context are all information that allow the client to load a
		level.
		A context setup is called every time the server want to load a new level.

		@param stmContext stream that contain the context informations(game dependent)
	*/
	virtual void OnXContextSetup(CStream& stmContext) = 0;
	/*! called by the client when some data is received
		@param stmContext stream that contain the data
	*/
	virtual void OnXData(CStream& stm) = 0;

	/*! called by the client when the server is very laggy (more than cl_timeout)
			that means that the client waits cl_timeout seconds, without any data from the server...
	*/
	virtual void OnXServerTimeout() = 0;
	/*! called by the client when the server responds, after a lot of time without doing so..
			that means that the client was treating the server as "timedout", and hoppefully waiting for it,
			and now, the server, magicaly responded...
	*/
	virtual void OnXServerRessurect() = 0;

	/*! called by the server when a timeout occurs..
			when a timeout is expected, because of the server loading for example,
			this function should return a number in milliseconds, that is the additional time to wait for the server.
			if not timeout is expected, this should return 0, and the normal timeout will take place.
	*/
	virtual unsigned int GetTimeoutCompensation() = 0;
	//!
	virtual void MarkForDestruct() = 0;
	//!
	virtual bool DestructIfMarked() = 0;
};


////////////////////////////////////////////////////////////////////////////////////////
/*! client interface
	this interface allow to connect and exchange data with a server

	REMARKS:
		when a disconnection occur the object that implements this interface
		CANNOT BE REUSED. This mean that the interface must be released
		and a new IClient must be created for each connection.
*/
struct IClient
{
	/*! start the connection to a server
		@param szIP address of the server can be an ip address like 134.122.345.3 or a symbolic www.stuff.com
		@param pbAuthorizationID must not be 0
		@param iAuthorizationSize >0
		--@param wPort the remote port of the server
	*/
	virtual void Connect(const char* szIP, WORD wPort, const BYTE* pbAuthorizationID, unsigned int iAuthorizationSize) = 0;
	/*! start disconnect from a server
		@param szCause cause of the disconneciton that will be send to the server
	*/
	virtual void Disconnect(const char* szCause) = 0;
	/*! send reliable data to the server
		@param stm the bitstream that store the data
	*/
	virtual void SendReliable(CStream& stm) = 0;
	/*! send unreliable data to the server
		@param stm the bitstream that store the data
	*/
	virtual void SendUnreliable(CStream& stm) = 0;
	/*! notify the server that the contex setup was received and the client now is ready to
		start to receive the data stream.
		usually called when the client finish to load the level.
		@param stm the bitstream that store the data that the server will receive(usally player name etc..)
	*/
	virtual void ContextReady(CStream& stm) = 0;
	/*! check if the client is ready to send data to the server
		@return true if the client is ready,false if not
	*/
	virtual bool IsReady() = 0;
	/*! called to update the client status
		@param nTime the current time in milliseconds
		@return true=this object is still exising, false=this object was destroyed

		REMARKS: to keep the connection working correctly this function must be called at least every frame
	*/
	virtual bool Update(unsigned int nTime) = 0;
	/*! get the average bandwidth used by the current connection
		@param fIncomingKbPerSec incoming kb per sec
		@param fOutgoingKbPerSec outgoing kb per sec
		@param nIncomingPackets per sec
		@param nOutgoingPackets per sec
	*/
	virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, DWORD& nIncomingPackets, DWORD& nOutgoingPackets) = 0;
	/*! release the interface(and delete the object that implements it)
	*/
	virtual void Release() = 0;
	/*! get the average round trip delay through client and server
		@return the average ping in milliseconds
	*/
	virtual unsigned int GetPing() = 0;
	//!
	virtual unsigned int GetRemoteTimestamp(unsigned int nTime) = 0;
	//!
	virtual unsigned int GetPacketsLostCount() = 0;
	//!
	virtual unsigned int GetUnreliablePacketsLostCount() = 0;
	//! returns IP of server.
	virtual CIPAddress GetServerIP() const = 0;
	//!
	virtual void InitiateCDKeyAuthorization(const bool inbCDAuthorization) = 0;
	//! \param pbAuthorizationID 0 if you wanna create a fake AuthorizationID, otherwise pointer to the AuthorizationID
	virtual void OnCDKeyAuthorization(BYTE* pbAuthorizationID) = 0;
	//!
	virtual void SetServerIP(const char* szServerIP) = 0;
};

#if 0
////////////////////////////////////////////////////////////////////////////////////////
/*!Server interface
*/
struct IServer
{
	/*! called to update the server status, this update all serverslots too
		@param nTime the current time in milliseconds

		REMARKS: to keep the connection working correctly this function must be called at least every frame
	*/
	virtual void Update(unsigned int nTime) = 0;
	//! release the interface and delete the implemetation
	virtual void Release() = 0;
	//! set a server veriable
	virtual void SetVariable(enum CryNetworkVarible eVarName, unsigned int nValue) = 0;
	/*! get the average bandwidth used by all active connections
		@param fIncomingKbPerSec incoming kb per sec
		@param fOutgoingKbPerSec outgoing kb per sec
		@param nIncomingPackets per sec
		@param nOutgoingPackets per sec
	*/
	virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, DWORD& nIncomingPackets, DWORD& nOutgoingPackets) = 0;
	/*!return the symbolic name of the localhost
		@return the symbolic name of the localhost
	*/
	virtual const char* GetHostName() = 0;
	//! \param inPacketID e.g. FT_CQP_RCON_COMMAND
	//! \param inpSink must not be 0
	virtual void RegisterPacketSink(const unsigned char inPacketID, INetworkPacketSink* inpSink) = 0;

	/*! set the security sink
			\param pSecurirySink pointer to a class that implements the IServerSecuritySink interface
	*/
	virtual void SetSecuritySink(IServerSecuritySink* pSecuritySink) = 0;

	/*!	check the state of an ip address before creating the slot
		\return the state of the ip (banned or not)
	*/
	virtual bool IsIPBanned(const unsigned int dwIP) = 0;

	/*! ban an ip address
		\param dwIP the ip address to ban
	*/
	virtual void BanIP(const unsigned int dwIP) = 0;

	/*! ban an ip address
		\param dwIP the ip address to ban
	*/
	virtual void UnbanIP(const unsigned int dwIP) = 0;

	//! time complexity: O(n) n=connected server slots
	//! \return 0 if there is no serverslot with this client (was never there or disconnected)
	virtual IServerSlot* GetServerSlotbyID(const unsigned char ucId) const = 0;

	//! to iterate through all clients (new clients ids are the lowest available at that time)
	virtual uint8 GetMaxClientID() const = 0;

	//! LAN/UBI/NET
	virtual EMPServerType GetServerType() const = 0;
};
#endif

// exports;
extern "C" {
	INetwork* CreateNetwork(ISystem* pSystem);
	typedef INetwork* (*PFNCREATENETWORK)(ISystem* pSystem);
}