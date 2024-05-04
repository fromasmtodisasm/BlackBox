#include "pch.hpp"
//#include "CNP.h"
#include "ServerSnooper.h"

#ifdef _DEBUG
static char THIS_FILE[] = __FILE__;
#define DEBUG_CLIENTBLOCK new( _NORMAL_BLOCK, THIS_FILE, __LINE__) 
#define new DEBUG_CLIENTBLOCK
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LOCATION __FILE__ "(" STRINGIZE(__LINE__) ") : Warning: "

#define ADDSTRING(c, s)	{ (c) += s; c.push_back('\0'); }
#define ADDBOOL(c, b)	{ (c).push_back(b ? '\1' : '\0'); }
#define ADDCHAR(c, ch)	{ (c).push_back((char)ch); }
#if defined(WIN64)
#define ADDINT(c, i)	{ char t=(char)(i & 0x000000FF);c+=t;t=(char)((i&0x0000FF00)>>8);c+=t;t=(char)((i&0x00FF0000)>>16);c+=t;t=(char)((i&0xFF000000)>>24);c+=t;}
#else
#define ADDINT(c, i)	{ for(int j=0;j<4;j++) (c).push_back(((char *)&(i))[j]); }
#endif
#define ADDRULE(c, name, value)	(c) += name; (c).push_back('\0'); (c) += value; (c).push_back('\0');


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
CServerSnooper::CServerSnooper(void)
{
	m_pSink=NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
CServerSnooper::~CServerSnooper(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CServerSnooper::Init(IServerSnooperSink *pSink)
{
	m_pSink=pSink;
	#pragma message(LOCATION "Not implemented.")
#if 0
	if(NET_FAILED(m_socket.Create()))
		return false;
#endif
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CServerSnooper::SearchForLANServers(unsigned int nTime)
{
#pragma message(LOCATION "Not implemented.")
#if 0
	CStream stm;
	CQPInfoRequest cqpInfoRequest("status");

	CIPAddress ipMulticastAddress(SERVER_MULTICAST_PORT,SERVER_MULTICAST_ADDRESS);	
	cqpInfoRequest.Save(stm);
	m_nStartTime=nTime;
	m_socket.Send(stm.GetPtr(),BITS2BYTES(stm.GetSize()),&ipMulticastAddress);
#else
	CryError("Search For LAN Servers");
	CIPAddress ip(4000, "127.0.0.1");
	static struct CQPInfoResponse { string szResponse; } cqpInfoResponse;
	auto& response = cqpInfoResponse.szResponse;
	if(m_pSink)
		m_pSink->OnServerFound(ip, cqpInfoResponse.szResponse, m_nCurrentTime-m_nStartTime);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CServerSnooper::Update(unsigned int nTime)
{
#pragma message(LOCATION "Not implemented.")
#if 0
	int nRecvBytes;
	m_nCurrentTime=nTime;
	static CStream buf;
	static CIPAddress ipFrom;
	do{
		buf.Reset();
		nRecvBytes=0;
		m_socket.Receive(buf.GetPtr(),
			(int)BITS2BYTES(buf.GetAllocatedSize()),
			nRecvBytes,
			ipFrom);
/////////////////////////////////////////////////////////
		if(nRecvBytes>0)
		{
			buf.SetSize(BYTES2BITS(nRecvBytes));
			ProcessPacket(buf,ipFrom);
		}
	}while(nRecvBytes>0);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CServerSnooper::ProcessPacket(CStream &stmPacket,CIPAddress &ip)
{
#pragma message(LOCATION "Not implemented.")
#if 0
	CNP cnp;
	cnp.LoadAndSeekToZero(stmPacket);

	if(cnp.m_cFrameType == FT_CQP_INFO_RESPONSE)
	{
		CQPInfoResponse cqpInfoResponse;

		cqpInfoResponse.Load(stmPacket);

		if(m_pSink)
			m_pSink->OnServerFound(ip, cqpInfoResponse.szResponse, m_nCurrentTime-m_nStartTime);
	}
#endif
}
