/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_BOMB_H
#define GAME_SERVER_GAMEMODES_BOMB_H
#include <game/server/gamecontroller.h>
#include <game/server/gamecontext.h>

class CGameControllerBOMB : public IGameController
{
public:
	CGameControllerBOMB(class CGameContext *pGameServer);
	virtual void Tick();
	// add more virtual functions here if you wish
	virtual void OnCharacterSpawn(CCharacter *pChr);
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual void DoWincheckRound();
	virtual void OnPlayerInfoChange(CPlayer *pPlayer);

	virtual int GetGameStartTick() const;

private:
	enum {
		BOMB,
		HUMAN,
	};
};
#endif
