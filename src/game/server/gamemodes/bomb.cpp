/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.				*/
#include "bomb.h"

#include <engine/shared/config.h>

#include <game/server/player.h>
#include <game/server/entities/character.h>

CGameControllerBOMB::CGameControllerBOMB(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_GameFlags = GAMEFLAG_SURVIVAL; // disable respawn
	m_pGameType = "BOMB";
}

void CGameControllerBOMB::Tick()
{
	// this is the main part of the gamemode, this function is run every tick
	IGameController::Tick();
}

void CGameControllerBOMB::OnCharacterSpawn(CCharacter *pChr)
{
	IGameController::OnCharacterSpawn(pChr);

	// prevent respawn
	pChr->GetPlayer()->m_RespawnDisabled = GetStartRespawnState();
}

bool CGameControllerBOMB::OnEntity(int Index, vec2 Pos)
{
	if(Index == ENTITY_SPAWN || Index == ENTITY_SPAWN_RED || Index == ENTITY_SPAWN_BLUE)
		return IGameController::OnEntity(Index, Pos);
	return false;
}

void CGameControllerBOMB::OnPlayerInfoChange(CPlayer *pPlayer)
{
	pPlayer->m_TeeInfos.m_aUseCustomColors[SKINPART_BODY] = true;
	pPlayer->m_TeeInfos.m_aUseCustomColors[SKINPART_DECORATION] = false;
	if(pPlayer->GetCharacter() && pPlayer->GetCharacter()->m_Bomb)
	{
		pPlayer->m_TeeInfos.m_aSkinPartColors[SKINPART_BODY] = 0;
		str_copy(pPlayer->m_TeeInfos.m_aaSkinPartNames[SKINPART_BODY], "bomb", 24);
		str_copy(pPlayer->m_TeeInfos.m_aaSkinPartNames[SKINPART_DECORATION], "bomb", 24);
	}
	else
	{
		pPlayer->m_TeeInfos.m_aSkinPartColors[SKINPART_BODY] = 16777215;
		str_copy(pPlayer->m_TeeInfos.m_aaSkinPartNames[SKINPART_BODY], "bear", 24);
		str_copy(pPlayer->m_TeeInfos.m_aaSkinPartNames[SKINPART_DECORATION], "hair", 24);
	}
	if(pPlayer->GetCharacter() && pPlayer->GetCharacter()->IsFrozen())
	{
		pPlayer->m_TeeInfos.m_aSkinPartColors[SKINPART_BODY] = 0x9BFF6B;
	}
	GameServer()->SendSkinChange(pPlayer->GetCID(), -1);
}

void CGameControllerBOMB::DoWincheckRound()
{
	// Count bombs and humans
	int aCount[2] = {0, 0};
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS &&
			(!GameServer()->m_apPlayers[i]->m_RespawnDisabled ||
			(GameServer()->m_apPlayers[i]->GetCharacter() && GameServer()->m_apPlayers[i]->GetCharacter()->IsAlive()))) // joined this round or already exploded
		{
			if(GameServer()->m_apPlayers[i]->GetCharacter() == NULL || GameServer()->m_apPlayers[i]->GetCharacter()->m_Bomb)
			{
				// NULL if not yet spawned
				aCount[BOMB] += 1;
			}
			else
			{
				aCount[HUMAN] += 1;
			}
		}
	}
	int NumAlive = aCount[BOMB] + aCount[HUMAN];
	if((Server()->Tick() - m_GameStartTick) >= m_GameInfo.m_TimeLimit*Server()->TickSpeed()*60)
	{
		if(aCount[HUMAN] == 0 && NumAlive != 1)
		{
			if(aCount[BOMB] == 0)
			{
				EndRound();
				return;
			}
			else if (m_SuddenDeath == 0)
			{
				m_SuddenDeath = 1;
			}
		}
		else
		{
			m_SuddenDeath = 0;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS &&
					GameServer()->m_apPlayers[i]->GetCharacter() &&
					GameServer()->m_apPlayers[i]->GetCharacter()->IsAlive())
				{
					bool Explode = false;
					// Give the last player points, even if it is a bomb
					if(GameServer()->m_apPlayers[i]->GetCharacter()->m_Bomb && NumAlive != 1 )
					{
						GameServer()->CreateExplosion(GameServer()->m_apPlayers[i]->m_ViewPos, i, WEAPON_GAME, false);
						GameServer()->CreateSound(GameServer()->m_apPlayers[i]->m_ViewPos, SOUND_GRENADE_EXPLODE);
						GameServer()->m_apPlayers[i]->GetCharacter()->Die(GameServer()->m_apPlayers[i]->GetCID(), WEAPON_GAME);
						Explode = true;
					}
					else
					{
						GameServer()->m_apPlayers[i]->m_Score += Config()->m_SvBombScoreSurvive; // Do Round Scoring
						if(aCount[HUMAN] > 1)
						{
							GameServer()->m_apPlayers[i]->GetCharacter()->m_Bomb = true;
							GameServer()->m_apPlayers[i]->GetCharacter()->m_NextBomb = true;
							OnPlayerInfoChange(GameServer()->m_apPlayers[i]);
						}
						else
						{
							GameServer()->m_apPlayers[i]->m_Score += Config()->m_SvBombScoreLastSurvivor;
						}
						GameServer()->m_apPlayers[i]->GetCharacter()->IncreaseHealth(10);
						GameServer()->m_apPlayers[i]->GetCharacter()->IncreaseArmor(10);
					}
					if(Explode)
					{
						GameServer()->SendBroadcast(Config()->m_SvBombExplodeMessage, -1);
					}
				}
			}
			if(aCount[HUMAN] == 1 || NumAlive == 1)
			{
				EndRound();
				return;
			}
			// Reset the timer to 20 sec
			m_GameStartTick = Server()->Tick() - (60 - Config()->m_SvBombTime)*Server()->TickSpeed();
		}
	}
	if(NumAlive == 1)
	{
		// Everyone but one has left the game
		EndRound();
		return;
	}
}

int CGameControllerBOMB::GetGameStartTick() const
{
	return m_GameStartTick + (60 - Config()->m_SvBombTime) * Server()->TickSpeed();
}

