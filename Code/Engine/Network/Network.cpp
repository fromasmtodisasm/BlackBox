#include "Common/ClientServer.h"

#include "Client.hpp"
#include "Network.hpp"
#include "Server.hpp"

#include <BlackBox/Core/ICompressionHelper.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Network/IPAddress.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <SDL2/SDL_net.h>
#include <functional>
#include <map>
#include <sstream>

#include <BlackBox/Core/Utils.hpp>

CNetwork g_Network;

class CCompressionHelper : public ICompressionHelper
{
public:
	CCompressionHelper(CNetwork* pNetwork)
	    : m_pNetwork(pNetwork)
	{
	}

public:
	CNetwork*    m_pNetwork;

	// Inherited via ICompressionHelper
	virtual bool Write(CStream& outStream, const unsigned char inChar) override
	{
		assert(0);
		return false;
	}
	virtual bool Read(CStream& inStream, unsigned char& outChar) override
	{
		assert(0);
		return false;
	}
	virtual bool Write(CStream& outStream, const char* inszString) override
	{
		auto len = strlen(inszString) + 1;
		return outStream.WriteBits((BYTE*)inszString, BYTES2BITS(len));
	}
	virtual bool Read(CStream& inStream, char* outszString, const DWORD indwStringSize) override
	{
		DWORD i  = 0;
		bool  ok = false;
		for (; i < indwStringSize && !inStream.EOS(); i++)
		{
			if (inStream.ReadBits((BYTE*)(outszString + i), BYTES2BITS(sizeof(char))))
			{
				if (outszString[i] == 0)
				{
					outszString[i + 1] = 0;
					ok                 = true;
					break;
				}
			}
			else
			{
				return false;
			}
		}
		return ok;
	}
};

CNetwork::CNetwork()
    : m_pSystem(nullptr)
{
}

CNetwork::~CNetwork()
{
	SAFE_DELETE(m_pCompressionHelper);
	network::ShutdownSockets();
}

bool CNetwork::Init(ISystem* pSystem)
{
	if (!network::InitSockets())
	{
		Env::Log()->Log("Winsock initialization failed");
		return false;
	}
	m_pCompressionHelper = new CCompressionHelper(this);
	Env::Log()->Log("Winsock initialized");
	return true;
}

uint32_t CNetwork::GetLocalIP() const
{
	return m_LocalIP.GetAsUINT();
}

void CNetwork::SetLocalIP(const char* szLocalIP)
{
	m_LocalIP = CIPAddress(m_LocalPort, szLocalIP);
}

IClient* CNetwork::CreateClient(IClientSink* pSink, bool bLocal)
{
	m_pClient = new CNetworkClient(this, pSink, bLocal);
	return m_pClient;
}

IServer* CNetwork::CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
{
	//FIXME: temporary solution
	assert(m_pServer == nullptr);
	m_pServer = new CNetworkServer(*this, pFactory, nPort, local);
	if (m_pServer->Start())
	{
		m_PortToServerMap[nPort] = m_pServer;
		return m_pServer;
	}
	m_pServer->Release();
	return nullptr;

}

IRConSystem* CNetwork::CreateRConSystem()
{
	return nullptr;
}

INETServerSnooper* CNetwork::CreateNETServerSnooper(INETServerSnooperSink* pSink)
{
	return nullptr;
}

IServerSnooper* CNetwork::CreateServerSnooper(IServerSnooperSink* pSink)
{
	return nullptr;
}

const char* CNetwork::EnumerateError(uint32_t err)
{
	return "Undefined error";
}

void CNetwork::Release()
{
	delete this;
}

void CNetwork::GetMemoryUsage(ICrySizer* pSizer)
{
}

ICompressionHelper* CNetwork::GetCompressionHelper()
{
	return m_pCompressionHelper;
}

void CNetwork::ClearProtectedFiles()
{
}

void CNetwork::AddProtectedFile(const char* sFilename)
{
}

IServer* CNetwork::GetServerByPort(const uint16_t wPort)
{
	if (auto it = m_PortToServerMap.find(wPort); it != m_PortToServerMap.end())
	{
		return it->second;	
	}
	return nullptr;
}

void CNetwork::UpdateNetwork()
{
}

void CNetwork::OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort)
{
}

bool CNetwork::VerifyMultiplayerOverInternet()
{
	return false;
}

void CNetwork::Client_ReJoinGameServer()
{
}

IClient* CNetwork::GetClient()
{
	return m_pClient;
}

void CNetwork::SetUBIGameServerIP(const char* szAddress)
{
}

const char* CNetwork::GetUBIGameServerIP(bool bLan)
{
	return nullptr;
}

void CNetwork::UnregisterClient(CNetworkClient* pClient)
{
	m_pClient = nullptr;
}

void CNetwork::UnregisterServer(CNetworkServer* pServer)
{
	m_pServer = nullptr;
}

NETWORK_API INetwork* CreateNetwork(ISystem* pSystem)
{
	if (!g_Network.Init(pSystem))
	{
		g_Network.Release();
	}
	return &g_Network;
}
