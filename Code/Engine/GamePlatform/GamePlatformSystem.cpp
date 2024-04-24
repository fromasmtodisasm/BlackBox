#include "pch.hpp"
#include <BlackBox/Core/Platform/platform_impl.inl>

#include "GamePlatformSystem.hpp"

#define _ACH_ID(id, name)       \
	{                           \
		id, #id, name, "", 0, 0 \
	}

// определяем достижения
enum EAchievements
{
	TEST_ACHIEVEMENT_1_0 = 0,
#if 0
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
#endif
};

// массив достижений, содержащий данные о достижениях и их состоянии
Achievement_t g_Achievements[] =
{
		_ACH_ID(TEST_ACHIEVEMENT_1_0, "10 Hits To Box"),
#if 0
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
#endif
};

CSteamAchievements::CSteamAchievements(Achievement_t* Achievements, int NumAchievements)
	: m_iAppID(0)
	, m_bInitialized(false)
	, m_CallbackUserStatsReceived(this, &CSteamAchievements::OnUserStatsReceived)
	, m_CallbackUserStatsStored(this, &CSteamAchievements::OnUserStatsStored)
	, m_CallbackAchievementStored(this, &CSteamAchievements::OnAchievementStored)
{
	m_iAppID = SteamUtils()->GetAppID();
	m_pAchievements = Achievements;
	m_iNumAchievements = NumAchievements;
	RequestStats();
}

CSteamAchievements::~CSteamAchievements()
{
}

bool CSteamAchievements::RequestStats()
{
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	return SteamUserStats()->RequestCurrentStats();
}

bool CSteamAchievements::SetAchievement(const char* ID)
{
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	return false;
}

void CSteamAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			CryLog("Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// �������� ����������
			for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
			{
				Achievement_t& ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			CryLog(buffer);
		}
	}
}

void CSteamAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			CryLog("Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			CryLog(buffer);
		}
	}
}

void CSteamAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	if (m_iAppID == pCallback->m_nGameID)
	{
		CryLog("Stored Achievement for Steam\n");
	}
}

IGAMEPLATFORM_API IGamePlatform* CreateGamePlatformInterface(ISystem* pSystem)
{
	return new CSteamPlatform();
}

inline bool CSteamPlatform::Init()
{

	// инициализируем Steam
	bool bRet = SteamAPI_Init();
	if (!bRet)
	{
		if (!SteamAPI_IsSteamRunning()) {
			CryFatalError("Steam not running");
		}
	}

	// создаем объект SteamAchievements, если инициализация Steam удалась
	if (bRet)
	{
		m_pAchievements = new CSteamAchievements(g_Achievements, 1);
		// Получить имена профилей Steam текущих пользователей.
		const char* name = SteamFriends()->GetPersonaName();
		Env::Log()->Log("person name: %s", name);
	}

	Env::Log()->Log("steam api init: %d", bRet);
	return bRet;
}
