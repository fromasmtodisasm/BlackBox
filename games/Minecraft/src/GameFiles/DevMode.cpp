#include <GameFiles/DevMode.hpp>

CDevMode::CDevMode()
{
	if (gEnv->pInput) gEnv->pInput->AddEventListener(this);
	m_bSlowDownGameSpeed = false;
	m_bHUD = false;
}

CDevMode::~CDevMode()
{
	if (gEnv->pInput)
		gEnv->pInput->RemoveEventListener(this);
}

void CDevMode::GotoTagPoint(int i)
{
	std::vector<STagFileEntry> tags = LoadTagFile();
	if (tags.size() > size_t(i))
	{
		STagFileEntry ent = tags[i];
		#if 0
		IActor* pActor = CCryAction::GetCryAction()->GetClientActor();
		if (!pActor) return;
		IEntity* pEntity = pActor->GetEntity();
		Quat rot;

		Vec3 pos = ent.pos;

		// pos loaded is a camera position, we must convert it into the player position.
		if (IMovementController* pMC = pActor->GetMovementController())
		{
			SMovementState ms;
			pMC->GetMovementState(ms);
			pos -= (ms.eyePosition - ms.pos);
		}

		rot.SetRotationXYZ(ent.ang);
		Vec3 scale = pEntity->GetScale();
		pEntity->SetPosRotScale(pos, rot, scale);
		pActor->SetViewRotation(rot);
		#endif
	}
}

void CDevMode::SaveTagPoint(int i)
{
	std::vector<STagFileEntry> tags = LoadTagFile();
	if (tags.size() <= size_t(i))
		tags.resize(i + 1);

	tags[i].pos = GetISystem()->GetViewCamera().GetPos();
	tags[i].ang = GetISystem()->GetViewCamera().GetAngles();

	SaveTagFile(tags);
}

bool CDevMode::OnInputEvent(const SInputEvent& evt)
{
	//CRY_PROFILE_FUNCTION(PROFILE_ACTION);
	bool handled = false;
	bool canCheat = true;	//CCryAction::GetCryAction()->CanCheat();

	#if 0
	IActor* pActor = CCryAction::GetCryAction()->GetClientActor();
	if (!pActor) return false;
	IEntity* pEntity = pActor->GetEntity();
	if (!pEntity) return false;
	#endif

	// tag-point functionality is provided by the editor already, so we should ignore it
	// when running in the editor
	if (!gEnv->IsEditor())
	{
		if ((evt.state == eIS_Pressed) && canCheat)
		{
			if (evt.modifiers & eMM_Ctrl)
			{
				if (evt.modifiers & eMM_Alt)
				{
					if (handled = (evt.keyId == eKI_F1)) SaveTagPoint(0);
					if (handled = (evt.keyId == eKI_F2)) SaveTagPoint(1);
					if (handled = (evt.keyId == eKI_F3)) SaveTagPoint(2);
					if (handled = (evt.keyId == eKI_F4)) SaveTagPoint(3);
					if (handled = (evt.keyId == eKI_F5)) SaveTagPoint(4);
					if (handled = (evt.keyId == eKI_F6)) SaveTagPoint(5);
					if (handled = (evt.keyId == eKI_F7)) SaveTagPoint(6);
					if (handled = (evt.keyId == eKI_F8)) SaveTagPoint(7);
					if (handled = (evt.keyId == eKI_F9)) SaveTagPoint(8);
					if (handled = (evt.keyId == eKI_F10)) SaveTagPoint(9);
					if (handled = (evt.keyId == eKI_F11)) SaveTagPoint(10);
					if (handled = (evt.keyId == eKI_F12)) SaveTagPoint(11);
				}
				else
				{
					if (handled = (evt.keyId == eKI_F1)) GotoTagPoint(0);
					if (handled = (evt.keyId == eKI_F2)) GotoTagPoint(1);
					if (handled = (evt.keyId == eKI_F3)) GotoTagPoint(2);
					if (handled = (evt.keyId == eKI_F4)) GotoTagPoint(3);
					if (handled = (evt.keyId == eKI_F5)) GotoTagPoint(4);
					if (handled = (evt.keyId == eKI_F6)) GotoTagPoint(5);
					if (handled = (evt.keyId == eKI_F7)) GotoTagPoint(6);
					if (handled = (evt.keyId == eKI_F8)) GotoTagPoint(7);
					if (handled = (evt.keyId == eKI_F9)) GotoTagPoint(8);
					if (handled = (evt.keyId == eKI_F10)) GotoTagPoint(9);
					if (handled = (evt.keyId == eKI_F11)) GotoTagPoint(10);
					if (handled = (evt.keyId == eKI_F12)) GotoTagPoint(11);
				}
			}
		}
	}
	else
	{
		// place commands which should only be dealt with in game mode here
	}

	// shared commands
	if (canCheat)
	{
		if (!handled && (evt.state == eIS_Pressed) && (evt.modifiers & eMM_Shift) != 0 && (evt.modifiers & eMM_Alt) != 0)
		{
			if (handled = (evt.keyId == eKI_NP_1)) GotoSpecialSpawnPoint(1);
			if (handled = (evt.keyId == eKI_NP_2)) GotoSpecialSpawnPoint(2);
			if (handled = (evt.keyId == eKI_NP_3)) GotoSpecialSpawnPoint(3);
			if (handled = (evt.keyId == eKI_NP_4)) GotoSpecialSpawnPoint(4);
			if (handled = (evt.keyId == eKI_NP_5)) GotoSpecialSpawnPoint(5);
			if (handled = (evt.keyId == eKI_NP_6)) GotoSpecialSpawnPoint(6);
			if (handled = (evt.keyId == eKI_NP_7)) GotoSpecialSpawnPoint(7);
			if (handled = (evt.keyId == eKI_NP_8)) GotoSpecialSpawnPoint(8);
			if (handled = (evt.keyId == eKI_NP_9)) GotoSpecialSpawnPoint(9);
		}
		else if (!handled && (evt.state == eIS_Pressed) && !(evt.modifiers & eMM_Modifiers))
		{
			if (handled = (evt.keyId == eKI_F2)) // go to next spawnpoint
			{
				#if 0
				//Vec3 oldPos = pEntity->GetWorldPos();
				if (gEnv->pScriptSystem->BeginCall("BasicActor", "OnNextSpawnPoint"))
				{
					gEnv->pScriptSystem->PushFuncParam(pEntity->GetScriptObject());
					gEnv->pScriptSystem->EndCall();

					#if 0
					if (gEnv->pStatoscope)
					{
						char buffer[100];
						Vec3 pos = pEntity->GetWorldPos();
						sprintf(buffer, "Teleported from (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)", oldPos.x, oldPos.y, oldPos.z, pos.x, pos.y, pos.z);
						//gEnv->pStatoscope->AddUserMarker("Player", buffer);
					}
					#endif
				}
				#endif
			}
		}
	}

	#if 0
	if (handled == false && evt.state == eIS_Pressed && (evt.modifiers & eMM_Alt) != 0)
	{
		if (evt.keyId == eKI_F7)
			SwitchSlowDownGameSpeed();
		else if (evt.keyId == eKI_F8)
			SwitchHUD();
	}
	#endif

	// AlexL: don't mark commands as handled for now. Would stop processing/propagating to other
	//        listeners. especially for non-modifier keys like f7,f8,f9 problematic
	return false; // handled;
}

std::vector<STagFileEntry> CDevMode::LoadTagFile()
{
	std::vector<STagFileEntry> out;

	FILE* finput = fopen(TagFileName().c_str(), "rt");
	if (finput)
	{
		while (!feof(finput))
		{
			STagFileEntry ent;
			if (6 == fscanf(finput, "%f,%f,%f,%f,%f,%f",
			                &ent.pos.x, &ent.pos.y, &ent.pos.z,
			                &ent.ang.x, &ent.ang.y, &ent.ang.z
			                ))
				out.push_back(ent);
			else
				break;
		}
		fclose(finput);
	}

	return out;
}

void CDevMode::SaveTagFile(const std::vector<STagFileEntry>& tags)
{
	FILE* foutput = fopen(TagFileName().c_str(), "wt");

	if (foutput)
	{
		for (std::vector<STagFileEntry>::const_iterator iter = tags.begin(); iter != tags.end(); ++iter)
		{
			fprintf(foutput, "%f,%f,%f,%f,%f,%f\n",
			        iter->pos.x, iter->pos.y, iter->pos.z,
			        iter->ang.x, iter->ang.y, iter->ang.z);
		}
		fclose(foutput);
	}
}

string CDevMode::TagFileName()
{
	return string("demo_tags.txt");
}

void CDevMode::GotoSpecialSpawnPoint(int i)
{
	char cmd[256];
	sprintf(cmd, "#g_localActor:SpawnAtSpawnPoint(\"SpawnPoint%d\")", i);
	gEnv->pConsole->ExecuteString(cmd);
}
