#pragma once
#ifndef _INETWORK_H_
#define _INETWORK_H_

#ifdef NETWORK_EXPORTS
  #define NETWORK_API DLL_EXPORT
#else
  #define NETWORK_API DLL_IMPORT
#endif

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/System/IBitStream.hpp>

#include <vector>				// STL vector<>

#define DEFAULT_SERVERPORT				49001
#define DEFAULT_SERVERPORT_STR		"49001"

#define SERVER_MULTICAST_PORT	5678
//<<FIXME>> It can be changed
#define SERVER_MULTICAST_ADDRESS	"234.5.6.7"

#define SERVER_QUERY_MAX_PACKETS	(8)
#define SERVER_QUERY_PACKET_SIZE	(1120)

//////////////////////////////////////////////////////////////////////
enum CryNetworkVarible
{
	cnvDataStreamTimeout=0
};

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
struct ICrySizer;
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
  virtual uint32_t GetLocalIP() const = 0;

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
  virtual IServer* CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local = false) = 0;

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
  /*! return the std::string representation of a socket error
    @param err id of the error
    @return std::string description of the error
  */
  virtual const char* EnumerateError(uint32_t err) = 0;
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
  virtual IServer* GetServerByPort(const uint16_t wPort) = 0;
  //! used to update things like the UBI.com services
  virtual void UpdateNetwork() = 0;
  //! currently used to update UBI.com info and check CDKey
  //! If it is a UBI type server we should the register, if we have already registered this will do nothing.
  //! \param szServerName must not be 0
  //! \param dwPlayerCount >0
  virtual void OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort) = 0;
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
    @param std::string representation of the disconnection cause
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
  virtual void Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize) = 0;
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
  virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) = 0;
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
  virtual void OnCDKeyAuthorization(uint8_t* pbAuthorizationID) = 0;
  //!
  virtual void SetServerIP(const char* szServerIP) = 0;
};

//////////////////////////////////////////////////////////////////////
struct IServerSnooper
{
  /*! query the LAN for servers
  */
  virtual void SearchForLANServers(unsigned int nTime = 0) = 0;
  virtual void Update(unsigned int nTime) = 0;
  //! release the interface(and delete the object that implements it)
  virtual void Release() = 0;
};

//////////////////////////////////////////////////////////////////////
struct IServerSnooperSink
{
  /*! called by the client when some server is found
    @param ip IP address of the found server
    @param stmServerInfo stream containing all server informations(game dependent)
  */
  virtual void OnServerFound(CIPAddress& ip, const std::string& szServerInfoString, int ping) = 0;
};

//////////////////////////////////////////////////////////////////////
struct INetworkPacketSink
{
  virtual void OnReceivingPacket(const unsigned char inPacketID, CStream& stmPacket, CIPAddress& ip) = 0;
};

//////////////////////////////////////////////////////////////////////
struct INETServerSnooper
{
  //! query internet servers for info
  virtual void Update(unsigned int dwTime) = 0;
  //!
  virtual void AddServer(const CIPAddress& ip) = 0;
  //!
  virtual void AddServerList(const std::vector<CIPAddress>& vIP) = 0;
  //! release the interface(and delete the object that implements it)
  virtual void Release() = 0;
  //! clear the current list of servers
  virtual void ClearList() = 0;
};

//////////////////////////////////////////////////////////////////////
struct INETServerSnooperSink
{
  /*! called by the client when some server is found
  @param ip IP address of the found server
  @param stmServerInfo stream containing all serer informations(game dependent)
  */
  virtual void OnNETServerFound(const CIPAddress& ip, const std::string& szServerInfoString, int ping) = 0;

  /*! called by the client when some server timedout
  @param ip IP address of the dead server
  */
  virtual void OnNETServerTimeout(const CIPAddress& ip) = 0;
};

//////////////////////////////////////////////////////////////////////
//! interface to control servers remotely
struct IRConSystem
{
  //! query response packets
  //! Can specify optional client, to get server ip from.
  virtual void Update(unsigned int dwTime, IClient* pClient = NULL) = 0;
  //! release the interface(and delete the object that implements it)
  virtual void Release() = 0;
  //!
  virtual void ExecuteRConCommand(const char* inszCommand) = 0;
  //!
  virtual void OnServerCreated(IServer* inpServer) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////
//! callback interface that must implement by the host that want to use ISererSlot
struct IServerSlotSink
{
  //! called by the serverslot when the connection occur
  virtual void OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize) = 0;
  /*! called by the serverslot when the disconnection occur
    @param std::string representation of the disconnection cause
  */
  virtual void OnXServerSlotDisconnect(const char* szCause) = 0;
  /*! called by the serverslot when the client send a "context ready"
    @param stm bitstream that store the data sent by the client as answer of the context setup(usally player name etc..)
  */
  virtual void OnContextReady(CStream& stm) = 0; //<<FIXME>> add some level validation code
  /*! called by the serverslot when some data is received
    @param stm bitstream that store the received data
  */
  virtual void OnData(CStream& stm) = 0;
  //!
  virtual void OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID,
    unsigned int uiGlobalIDSize) = 0;
};

//////////////////////////////////////////////////////////////////////
struct SServerSlotBandwidthStats
{
  //! constructor
  SServerSlotBandwidthStats()
  {
    Reset();
  }

  unsigned int		m_nReliableBitCount;				//!<
  unsigned int		m_nReliablePacketCount;			//!<
  unsigned int		m_nUnreliableBitCount;			//!<
  unsigned int		m_nUnreliablePacketCount;		//!<

  void Reset()
  {
    m_nReliableBitCount = 0;
    m_nReliablePacketCount = 0;
    m_nUnreliableBitCount = 0;
    m_nUnreliablePacketCount = 0;
  }
};

////////////////////////////////////////////////////////////////////////////////////////
/*! server slot interface
  The server slot is the endpoint of a client connection on the server-side. Besically for
  every remote client a server slot exist on the server.
*/
struct IServerSlot
{
  /*! set the host object that will receive all server slot notifications
    @param pSink poiter to an object thath implements IServerSlotSink
  */
  virtual void Advise(IServerSlotSink* pSink) = 0;
  /*! disconnect the client associated to this server slot
    @param szCause cause of the disconneciton that will be send to the client
  */
  virtual void Disconnect(const char* szCause) = 0;
  /*! send a context setup to the client
    @param stm bitstream that store the context information(usually level name etc..)
  */
  virtual bool ContextSetup(CStream& stm) = 0;
  /*! send reliable data to the client
    @param stm the bitstream that store the data
  */
  virtual void SendReliable(CStream& stm, bool bSecondaryChannel = false) = 0;
  /*! send unreliable data to the client
    @param stm the bitstream that store the data
  */
  virtual void SendUnreliable(CStream& stm) = 0;
  /*! check if the server slot is ready to send data to the client
    @return true if the serverslot is ready,false if not
  */
  virtual bool IsReady() = 0;
  /*! get the unique id that identify the server slot on a server
    @return ID of the serverslot
  */
  virtual unsigned char GetID() = 0;

  // Return IP in integer form.
  virtual unsigned int GetClientIP() const = 0;
  //! release the interface(and delete the object that implements it)
  virtual void Release() = 0;
  /*! get the average round trip delay through client and server
    @return the average ping in milliseconds
  */
  virtual unsigned int GetPing() = 0;
  //!
  virtual unsigned int GetPacketsLostCount() = 0;
  //!
  virtual unsigned int GetUnreliablePacketsLostCount() = 0;
  //! used for bandwidth calculations (to adjust the bandwidth)
  virtual void ResetBandwidthStats() = 0;
  //! used for bandwidth calculations (to adjust the bandwidth)
  virtual void GetBandwidthStats(SServerSlotBandwidthStats& out) const = 0;
  //! just calles OnXPlayerAuthorization of the corresponding game specific object
  virtual void OnPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID,
    unsigned int uiGlobalIDSize) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////
//the application must implement this class
struct IServerSlotFactory
{
  virtual bool CreateServerSlot(IServerSlot* pIServerSlot) = 0;

  //! \return true=success, false otherwise
  //! fill the given std::string with server infos
  //! \note do not overwrite the std::string, just append to it
  virtual bool GetServerInfoStatus(std::string& szServerStatus) = 0;
  virtual bool GetServerInfoStatus(std::string& szName, std::string& szGameType, std::string& szMap, std::string& szVersion, bool* pbPassword, int* piPlayers, int* piMaxPlayers) = 0;
  virtual bool GetServerInfoRules(std::string& szServerRules) = 0;
  virtual bool GetServerInfoPlayers(std::string* vszStrings[4], int& nStrings) = 0;
  //! Called when someone sends XML request to server, this function must fill sResponse std::string with XML response.
  virtual bool ProcessXMLInfoRequest(const char* sRequest, const char* sRespone, int nResponseMaxLength) = 0;
};

//////////////////////////////////////////////////////////////////////
// the application should implement this class
struct IServerSecuritySink
{
  enum CheaterType
  {
    CHEAT_NOT_RESPONDING,
    CHEAT_NET_PROTOCOL,
    CHEAT_MODIFIED_FILE,
    CHEAT_MODIFIED_CODE,
    CHEAT_MODIFIED_VARS,
  };
  struct SSlotInfo
  {
    char playerName[32];
    int score;
    int deaths;
  };

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

  /*! Report cheating user.
   *
   */
  virtual void CheaterFound(const unsigned int dwIP, int type, const char* sMsg) = 0;

  /*! Request slot information from the game.
   *
   */
  virtual bool GetSlotInfo(const unsigned int dwIP, SSlotInfo& info, int nameOnly) = 0;
};

//////////////////////////////////////////////////////////////////////////
enum EMPServerType
{
  eMPST_LAN = 0,                        //!< LAN
  eMPST_NET,                        //!< e.g. ASE
  eMPST_UBI,                        //!< UBI.com
};

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
  virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) = 0;
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
  virtual uint8_t GetMaxClientID() const = 0;

  //! LAN/UBI/NET
  virtual EMPServerType GetServerType() const = 0;
};

// exports;
extern "C" {
  NETWORK_API INetwork* CreateNetwork(ISystem* pSystem);
  typedef INetwork* (*PFNCREATENETWORK)(ISystem* pSystem);
}

#endif //_INETWORK_H_
