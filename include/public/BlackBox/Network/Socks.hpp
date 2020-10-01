#pragma once

#define CRY_ASSERT ASSERT
#define PREFAST_SUPPRESS_WARNING 
namespace CrySock
{
//	///////////////////////////////////////////////////////////////////////////////
//		//! All socket errors should be -ve since a 0 or +ve result indicates success.
enum eCrySockError
{

	eCSE_NO_ERROR         = 0, //!< No error reported.

	eCSE_EACCES           = -1,  //!< Tried to establish a connection to an invalid address (such as a broadcast address).
	eCSE_EADDRINUSE       = -2,  //!< Specified address already in use.
	eCSE_EADDRNOTAVAIL    = -3,  //!< Invalid address was specified.
	eCSE_EAFNOSUPPORT     = -4,  //!< Invalid socket type (or invalid protocol family).
	eCSE_EALREADY         = -5,  //!< Connection is being established (if the function is called again in non-blocking state).
	eCSE_EBADF            = -6,  //!< Invalid socket number specified.
	eCSE_ECONNABORTED     = -7,  //!< Connection was aborted.
	eCSE_ECONNREFUSED     = -8,  //!< Connection refused by destination.
	eCSE_ECONNRESET       = -9,  //!< Connection was reset (TCP only).
	eCSE_EFAULT           = -10, //!< Invalid socket number specified.
	eCSE_EHOSTDOWN        = -11, //!< Other end is down and unreachable.
	eCSE_EINPROGRESS      = -12, //!< Action is already in progress (when non-blocking).
	eCSE_EINTR            = -13, //!< A blocking socket call was cancelled.
	eCSE_EINVAL           = -14, //!< Invalid argument or function call.
	eCSE_EISCONN          = -15, //!< Specified connection is already established.
	eCSE_EMFILE           = -16, //!< No more socket descriptors available.
	eCSE_EMSGSIZE         = -17, //!< Message size is too large.
	eCSE_ENETUNREACH      = -18, //!< Destination is unreachable.
	eCSE_ENOBUFS          = -19, //!< Insufficient working memory.
	eCSE_ENOPROTOOPT      = -20, //!< Invalid combination of 'level' and 'optname'.
	eCSE_ENOTCONN         = -21, //!< Specified connection is not established.
	eCSE_ENOTINITIALISED  = -22, //!< Socket layer not initialised (e.g. need to call WSAStartup() on Windows).
	eCSE_EOPNOTSUPP       = -23, //!< Socket type cannot accept connections.
	eCSE_EPIPE            = -24, //!< The writing side of the socket has already been closed.
	eCSE_EPROTONOSUPPORT  = -25, //!< Invalid protocol family.
	eCSE_ETIMEDOUT        = -26, //!< TCP resend timeout occurred.
	eCSE_ETOOMANYREFS     = -27, //!< Too many multicast addresses specified.
	eCSE_EWOULDBLOCK      = -28, //!< Time out occurred when attempting to perform action.
	eCSE_EWOULDBLOCK_CONN = -29, //!< Only applicable to connect() - a non-blocking connection has been attempted and is in progress.
	                             //
	eCSE_MISC_ERROR       = -1000
};

}; //CrySock

#define INCLUDED_FROM_CRY_SOCKS_H
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <WinSock2.h>
#include <ws2tcpip.h>

#define CRY_INVALID_SOCKET INVALID_SOCKET
#define CRY_SOCKET_ERROR   SOCKET_ERROR

#include <cstring>
#define LOCAL_FALLBACK_DOMAIN ".intern.crytek.de" // On connection error try to fallback to this domain

///////////////////////////////////////////////////////////////////////////////

// Type wrappers for sockets

typedef sockaddr      CRYSOCKADDR;
typedef sockaddr_in   CRYSOCKADDR_IN;
typedef socklen_t     CRYSOCKLEN_T;
typedef SOCKET        CRYSOCKET;
typedef in_addr       CRYINADDR;
typedef unsigned long CRYINADDR_T;
typedef hostent       CRYHOSTENT;

// Type wrappers for sockets
typedef fd_set  CRYFD_SET;
typedef timeval CRYTIMEVAL;

///////////////////////////////////////////////////////////////////////////////

namespace CrySock
{
///////////////////////////////////////////////////////////////////////////////
// Forward declarations
static eCrySockError TranslateLastSocketError();
static int           TranslateToSocketError(eCrySockError socketError);

static eCrySockError TranslateInvalidSocket(CRYSOCKET s);
static eCrySockError TranslateSocketError(int socketError);

///////////////////////////////////////////////////////////////////////////////

static int GetLastSocketError()
{
	return WSAGetLastError();
}

///////////////////////////////////////////////////////////////////////////////

static CRYSOCKET socket(int af, int type, int protocol)
{
	return ::socket(af, type, protocol);
}

///////////////////////////////////////////////////////////////////////////////

//! Create default INET socket.
static CRYSOCKET socketinet()
{
	return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

///////////////////////////////////////////////////////////////////////////////

static int setsockopt(CRYSOCKET s, int level, int optname, const char* optval, CRYSOCKLEN_T optlen)
{
	return ::setsockopt(s, level, optname, optval, optlen);
}

///////////////////////////////////////////////////////////////////////////////

static int closesocket(CRYSOCKET s)
{
	return ::closesocket(s);
}

///////////////////////////////////////////////////////////////////////////////

static int shutdown(CRYSOCKET s, int how)
{
	return ::shutdown(s, how);
}

///////////////////////////////////////////////////////////////////////////////

static int getsockname(CRYSOCKET s, CRYSOCKADDR* addr, CRYSOCKLEN_T* addrlen)
{
	return ::getsockname(s, addr, addrlen);
}

///////////////////////////////////////////////////////////////////////////////

static int connect(CRYSOCKET s, const CRYSOCKADDR* addr, CRYSOCKLEN_T addrlen)
{
	return ::connect(s, addr, addrlen);
}

///////////////////////////////////////////////////////////////////////////////

static int listen(CRYSOCKET s, int backlog)
{
	return ::listen(s, backlog);
}

///////////////////////////////////////////////////////////////////////////////

static CRYSOCKET accept(CRYSOCKET s, CRYSOCKADDR* addr, CRYSOCKLEN_T* addrlen)
{
	return ::accept(s, addr, addrlen);
}

///////////////////////////////////////////////////////////////////////////////

static int send(CRYSOCKET s, const char* buf, int len, int flags)
{
	return ::send(s, buf, len, flags);
}

///////////////////////////////////////////////////////////////////////////////

static int recv(CRYSOCKET s, char* buf, int len, int flags)
{
	return ::recv(s, buf, len, flags);
}

///////////////////////////////////////////////////////////////////////////////

static int recvfrom(CRYSOCKET s, char* buf, int len, int flags, CRYSOCKADDR* addr, CRYSOCKLEN_T* addrLen)
{
	return ::recvfrom(s, buf, len, flags, addr, addrLen);
}

///////////////////////////////////////////////////////////////////////////////

static int sendto(CRYSOCKET s, char* buf, int len, int flags, CRYSOCKADDR* addr, CRYSOCKLEN_T addrLen)
{
	return ::sendto(s, buf, len, flags, addr, addrLen);
}

///////////////////////////////////////////////////////////////////////////////

static int SetRecvTimeout(CRYSOCKET s, const int seconds, const int microseconds)
{
	struct timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = microseconds;
	return CrySock::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}

///////////////////////////////////////////////////////////////////////////////

static int SetSendTimeout(CRYSOCKET s, const int seconds, const int microseconds)
{
	struct timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = microseconds;
	return CrySock::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}

///////////////////////////////////////////////////////////////////////////////

//! \retval 1   Readable - use recv to get the data.
//! \retval 0   Timed out (exceeded passed timeout value).
//! \retval <0  An error occurred - see eCrySocketError.
static int IsRecvPending(CRYSOCKET s, CRYTIMEVAL* timeout)
{
	CRYFD_SET emptySet;
	FD_ZERO(&emptySet);

	CRYFD_SET readSet;
	FD_ZERO(&readSet);
	FD_SET(s, &readSet);

	int ret = ::select(static_cast<int>(s) + 1, &readSet, &emptySet, &emptySet, timeout);
	if (ret != SOCKET_ERROR)
	{
		ret = FD_ISSET(s, &readSet);
		if (ret != 0)
		{
			ret = 1;
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

//! \retval 1   Connected.
//! \retval 0   Timed out (exceeded passed timeout value).
//! \retval <0  An error occurred - see eCrySocketError.
static int WaitForWritableSocket(CRYSOCKET s, CRYTIMEVAL* timeout)
{
	CRYFD_SET emptySet;
	FD_ZERO(&emptySet);

	CRYFD_SET writeSet;
	FD_ZERO(&writeSet);
	FD_SET(s, &writeSet);

	int ret = ::select(static_cast<int>(s) + 1, &emptySet, &writeSet, &emptySet, timeout);

	if (ret != SOCKET_ERROR)
	{
		ret = FD_ISSET(s, &writeSet);
		if (ret != 0)
		{
			ret = 1;
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

//! IsRecvPending should be sufficient for most applications.
//! The correct value for the first parameter 'nfds' should be the highest
//! numbered socket (as passed to FD_SET) +1.
//! Only use this if you know what you're doing! Passing nfds as 0 on anything
//! other than WinSock is wrong!.
static int select(int nfds, CRYFD_SET* readfds, CRYFD_SET* writefds, CRYFD_SET* exceptfds, CRYTIMEVAL* timeout)
{
#if !defined(_RELEASE)
	if (nfds == 0)
	{
		CryFatalError("CrySock select detected first parameter is 0!  This *MUST* be fixed!");
	}
#endif

	return ::select(nfds, readfds, writefds, exceptfds, timeout);
}

///////////////////////////////////c////////////////////////////////////////////

static int bind(CRYSOCKET s, const CRYSOCKADDR* addr, CRYSOCKLEN_T addrlen)
{
	return ::bind(s, addr, addrlen);
}

///////////////////////////////////////////////////////////////////////////////

static bool MakeSocketNonBlocking(CRYSOCKET sock)
{
	unsigned long nTrue = 1;
	if (::ioctlsocket(sock, FIONBIO, &nTrue) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

static bool MakeSocketBlocking(CRYSOCKET sock)
{
	unsigned long nTrue = 0;
	if (::ioctlsocket(sock, FIONBIO, &nTrue) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////

static CRYHOSTENT* gethostbyname(const char* hostname)
{
	PREFAST_SUPPRESS_WARNING(4996);
	return ::gethostbyname(hostname);
}

///////////////////////////////////////////////////////////////////////////////

static int gethostname(char* name, int namelen)
{
	return ::gethostname(name, namelen);
}

///////////////////////////////////////////////////////////////////////////////

static CRYINADDR_T GetAddrForHostOrIP(const char* hostnameOrIP, uint32 timeoutMilliseconds = 0)
{
	uint32 ret = INADDR_NONE;
	CRYHOSTENT* pHost = gethostbyname(hostnameOrIP);
	if (pHost)
	{
		ret = ((in_addr*)(pHost->h_addr))->s_addr;
	}
	else
	{
		PREFAST_SUPPRESS_WARNING(4996);
		ret = ::inet_addr(hostnameOrIP);
	}
	return ret != INADDR_NONE ? ret : 0;
}

///////////////////////////////////////////////////////////////////////////////

static CRYINADDR_T inet_addr(const char* cp)
{
	PREFAST_SUPPRESS_WARNING(4996);
	return ::inet_addr(cp);
}

///////////////////////////////////////////////////////////////////////////////

static char* inet_ntoa(CRYINADDR in)
{
	PREFAST_SUPPRESS_WARNING(4996);
	return ::inet_ntoa(in);
}

///////////////////////////////////////////////////////////////////////////////

static uint32 DNSLookup(const char* hostname, uint32 timeoutMilliseconds = 200)
{
	CRYINADDR_T ret = CrySock::GetAddrForHostOrIP(hostname, timeoutMilliseconds);

	if (ret == 0 || ret == ~0)
	{
		char host[256];

		size_t hostlen = strlen(hostname);
		size_t domainlen = sizeof(LOCAL_FALLBACK_DOMAIN);
		if (hostlen + domainlen > sizeof(host) - 1)
		{
			CryLog("hostname too long to fallback to local domain: '%s'", hostname);
			return 0;
		}

		strcpy(host, hostname);
		strcat(host, LOCAL_FALLBACK_DOMAIN);

		host[hostlen + domainlen - 1] = 0;

		ret = CrySock::GetAddrForHostOrIP(host, timeoutMilliseconds);
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

static eCrySockError TranslateInvalidSocket(CRYSOCKET s)
{
	if (s == CRY_INVALID_SOCKET)
	{
		return TranslateLastSocketError();
	}
	return eCSE_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////

static eCrySockError TranslateSocketError(int socketError)
{
	if (socketError == CRY_SOCKET_ERROR)
	{
		return TranslateLastSocketError();
	}
	return eCSE_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////

static eCrySockError TranslateLastSocketError()
{
#define TRANSLATE(_from, _to) case (_from): \
  socketError = (_to); break;
	eCrySockError socketError = eCSE_NO_ERROR;
	int error = GetLastSocketError();
	switch (error)
	{
		TRANSLATE(0, eCSE_NO_ERROR);

		TRANSLATE(WSAEACCES, eCSE_EACCES);
		TRANSLATE(WSAEADDRINUSE, eCSE_EADDRINUSE);
		TRANSLATE(WSAEADDRNOTAVAIL, eCSE_EADDRNOTAVAIL);
		TRANSLATE(WSAEAFNOSUPPORT, eCSE_EAFNOSUPPORT);
		TRANSLATE(WSAEALREADY, eCSE_EALREADY);
		TRANSLATE(WSAEBADF, eCSE_EBADF);
		TRANSLATE(WSAECONNABORTED, eCSE_ECONNABORTED);
		TRANSLATE(WSAECONNREFUSED, eCSE_ECONNREFUSED);
		TRANSLATE(WSAECONNRESET, eCSE_ECONNRESET);
		TRANSLATE(WSAEFAULT, eCSE_EFAULT);
		TRANSLATE(WSAEHOSTDOWN, eCSE_EHOSTDOWN);
		TRANSLATE(WSAEINPROGRESS, eCSE_EINPROGRESS);
		TRANSLATE(WSAEINTR, eCSE_EINTR);
		TRANSLATE(WSAEINVAL, eCSE_EINVAL);
		TRANSLATE(WSAEISCONN, eCSE_EISCONN);
		TRANSLATE(WSAEMFILE, eCSE_EMFILE);
		TRANSLATE(WSAEMSGSIZE, eCSE_EMSGSIZE);
		TRANSLATE(WSAENETUNREACH, eCSE_ENETUNREACH);
		TRANSLATE(WSAENOBUFS, eCSE_ENOBUFS);
		TRANSLATE(WSAENOPROTOOPT, eCSE_ENOPROTOOPT);
		TRANSLATE(WSAENOTCONN, eCSE_ENOTCONN);
		TRANSLATE(WSAEOPNOTSUPP, eCSE_EOPNOTSUPP);
		TRANSLATE(WSAEPROTONOSUPPORT, eCSE_EPROTONOSUPPORT);
		TRANSLATE(WSAETIMEDOUT, eCSE_ETIMEDOUT);
		TRANSLATE(WSAETOOMANYREFS, eCSE_ETOOMANYREFS);
		TRANSLATE(WSAEWOULDBLOCK, eCSE_EWOULDBLOCK);

		TRANSLATE(WSANOTINITIALISED, eCSE_ENOTINITIALISED);
	default:
		CRY_ASSERT(false, string().Format("CrySock could not translate OS error code %x, treating as miscellaneous", error));
		socketError = eCSE_MISC_ERROR;
		break;
	}
#undef TRANSLATE

	return socketError;
}

///////////////////////////////////////////////////////////////////////////////

static int TranslateToSocketError(eCrySockError socketError)
{
#define TRANSLATE(_to, _from) case (_from): \
  error = (_to); break;                                            // reverse order of inputs (_to, _from) instead of (_from, _to) compared to TranslateLastSocketError
	int error = 0;
	switch (socketError)
	{
		TRANSLATE(0, eCSE_NO_ERROR);

		TRANSLATE(WSAEACCES, eCSE_EACCES);
		TRANSLATE(WSAEADDRINUSE, eCSE_EADDRINUSE);
		TRANSLATE(WSAEADDRNOTAVAIL, eCSE_EADDRNOTAVAIL);
		TRANSLATE(WSAEAFNOSUPPORT, eCSE_EAFNOSUPPORT);
		TRANSLATE(WSAEALREADY, eCSE_EALREADY);
		TRANSLATE(WSAEBADF, eCSE_EBADF);
		TRANSLATE(WSAECONNABORTED, eCSE_ECONNABORTED);
		TRANSLATE(WSAECONNREFUSED, eCSE_ECONNREFUSED);
		TRANSLATE(WSAECONNRESET, eCSE_ECONNRESET);
		TRANSLATE(WSAEFAULT, eCSE_EFAULT);
		TRANSLATE(WSAEHOSTDOWN, eCSE_EHOSTDOWN);
		TRANSLATE(WSAEINPROGRESS, eCSE_EINPROGRESS);
		TRANSLATE(WSAEINTR, eCSE_EINTR);
		TRANSLATE(WSAEINVAL, eCSE_EINVAL);
		TRANSLATE(WSAEISCONN, eCSE_EISCONN);
		TRANSLATE(WSAEMFILE, eCSE_EMFILE);
		TRANSLATE(WSAEMSGSIZE, eCSE_EMSGSIZE);
		TRANSLATE(WSAENETUNREACH, eCSE_ENETUNREACH);
		TRANSLATE(WSAENOBUFS, eCSE_ENOBUFS);
		TRANSLATE(WSAENOPROTOOPT, eCSE_ENOPROTOOPT);
		TRANSLATE(WSAENOTCONN, eCSE_ENOTCONN);
		TRANSLATE(WSAEOPNOTSUPP, eCSE_EOPNOTSUPP);
		TRANSLATE(WSAEPROTONOSUPPORT, eCSE_EPROTONOSUPPORT);
		TRANSLATE(WSAETIMEDOUT, eCSE_ETIMEDOUT);
		TRANSLATE(WSAETOOMANYREFS, eCSE_ETOOMANYREFS);
		TRANSLATE(WSAEWOULDBLOCK, eCSE_EWOULDBLOCK);

		TRANSLATE(WSANOTINITIALISED, eCSE_ENOTINITIALISED);
	default:
		CRY_ASSERT(false, string().Format("CrySock could not translate eCrySockError error code %x, treating as miscellaneous", socketError));
		break;
	}
#undef TRANSLATE

	return error;
}

///////////////////////////////////////////////////////////////////////////////

}; // CrySock

