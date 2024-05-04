#include "pch.hpp"
#include "NetServerSnooper.h"

void CNETServerSnooper::QueryServer(CIPAddress& ip)
{
}

void CNETServerSnooper::ProcessPacket(CStream& stmPacket, CIPAddress& ip)
{
}

void CNETServerSnooper::ProcessTimeout()
{
}

bool CNETServerSnooper::ProcessNext()
{
	return false;
}

CNETServerSnooper::CNETServerSnooper()
{
}

CNETServerSnooper::~CNETServerSnooper()
{
}

bool CNETServerSnooper::Create(ISystem* pSystem, INETServerSnooperSink* pSink)
{
	assert(pSystem);
	assert(pSink);

	m_pSystem = pSystem;
	m_pSink = pSink;

#if 0
	if (NET_FAILED(m_sSocket.Create()))
	{
		return 0;
	}
#endif

	cl_snoopcount = m_pSystem->GetIConsole()->GetCVar("cl_snoopcount");
	cl_snoopretries = m_pSystem->GetIConsole()->GetCVar("cl_snoopretries");
	cl_snooptimeout = m_pSystem->GetIConsole()->GetCVar("cl_snooptimeout");

	return 1;
}

void CNETServerSnooper::OnReceivingPacket(const unsigned char inPacketID, CStream& stmPacket, CIPAddress& ip)
{
}

void CNETServerSnooper::Release()
{
	m_hmServerTable.clear();
	delete this;
}

void CNETServerSnooper::Update(unsigned int dwTime)
{
	if (m_hmServerTable.empty())
	{
		return;
	}

	CRY_FUNCTION_NOT_IMPLEMENTED;
}

void CNETServerSnooper::AddServer(const CIPAddress& ip)
{
	CRY_FUNCTION_NOT_IMPLEMENTED;
}

void CNETServerSnooper::AddServerList(const std::vector<CIPAddress>& vIP)
{
}

void CNETServerSnooper::ClearList()
{
}
