#include <SteamHelper.hpp>

CSteamAchievements::CSteamAchievements(Achievement_t *Achievements, int NumAchievements):
 m_iAppID( 0 ),
 m_bInitialized( false ),
 m_CallbackUserStatsReceived( this, &CSteamAchievements::OnUserStatsReceived ),
 m_CallbackUserStatsStored( this, &CSteamAchievements::OnUserStatsStored ),
 m_CallbackAchievementStored( this, &CSteamAchievements::OnAchievementStored )
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
	// Steam загружен? ≈сли нет, получить статистики не удастс€.
	if ( NULL == SteamUserStats() || NULL == SteamUser() )
	{
		return false;
	}
	// ѕользователь залогинен?  ≈сли нет, получить статистики не удастс€.
	if ( !SteamUser()->BLoggedOn() )
	{
		return false;
	}
	// «апрос статистик.
	return SteamUserStats()->RequestCurrentStats();
}

bool CSteamAchievements::SetAchievement(const char* ID)
{
	// получен ли обратный вызов от Steam?
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// ≈сли нет, установить достижение не получитс€
	return false;
}

void CSteamAchievements::OnUserStatsReceived( UserStatsReceived_t *pCallback )
{
	// могут быть получены обратные вызовы со статистиками из других игр, игнорируйте их
	if ( m_iAppID == pCallback->m_nGameID )
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			OutputDebugString("Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// загрузка достижений
			for ( int iAch = 0; iAch < m_iNumAchievements; ++iAch )
			{
				Achievement_t &ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf( ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
					"name"));
				_snprintf( ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
					"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf( buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult );
			OutputDebugString( buffer );
		}
	}
}

void CSteamAchievements::OnUserStatsStored( UserStatsStored_t *pCallback )
{
	// могут быть получены обратные вызовы со статистиками из других игр, игнорируйте их
	if ( m_iAppID == pCallback->m_nGameID )
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			OutputDebugString( "Stored stats for Steam\n" );
		}
		else
		{
			char buffer[128];
			_snprintf( buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult );
			OutputDebugString( buffer );
		}
	}
}

void CSteamAchievements::OnAchievementStored( UserAchievementStored_t *pCallback )
{
     // могут быть получены обратные вызовы со статистиками из других игр, игнорируйте их
     if ( m_iAppID == pCallback->m_nGameID )
     {
          OutputDebugString( "Stored Achievement for Steam\n" );
     }
}
