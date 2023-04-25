/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "UpdateFields.h"

class BGS_BGLevel : public BGScript
{
private:
    void RestoreOriginalLevel(Player* player)
    {
        ObjectGuid playerGUID = player->GetGUID();
        QueryResult result = CharacterDatabase.Query("SELECT PlayerID, original_level, original_xp FROM custom_bg_level WHERE PlayerID = {}", player->GetGUID().GetCounter());
        if (result)
        {
            CharacterDatabase.Execute("DELETE FROM custom_bg_level WHERE PlayerID = {}", player->GetGUID().GetCounter());
            player->SetLevel((*result)[1].Get<uint8>(), true);
            player->SetUInt32Value(PLAYER_XP, (*result)[2].Get<uint32>());
        }
    }

    void SetToBGLevel(Player* player)
    {
        uint32 levelToSet = sConfigMgr->GetOption<uint32>("CustomBGLevel.Level", 80);
        CharacterDatabase.Execute("INSERT INTO custom_bg_level (PlayerID, original_level, original_xp) VALUES ({}, {}, {})", player->GetGUID().GetCounter(), player->getLevel(), player->GetUInt32Value(PLAYER_XP));
        player->SetLevel(levelToSet, true);
    }

public:
    BGS_BGLevel() : BGScript("Battleground_BGLevel") { }

    void OnBattlegroundAddPlayer(Battleground* /*bg*/, Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("CustomBGLevel.Enable", false))
        {
            SetToBGLevel(player);
        }
    }

    void OnBattlegroundRemovePlayerAtLeave(Battleground* /*bg*/, Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("CustomBGLevel.Enable", false))
        {
            RestoreOriginalLevel(player);
        }
    }
};

// Add player scripts
class PS_BGLevel : public PlayerScript
{
private:
    void RestoreOriginalLevel(Player* player)
    {
        ObjectGuid playerGUID = player->GetGUID();
        QueryResult result = CharacterDatabase.Query("SELECT PlayerID, original_level, original_xp FROM custom_bg_level WHERE PlayerID = {}", player->GetGUID().GetCounter());
        if (result)
        {
            CharacterDatabase.Execute("DELETE FROM custom_bg_level WHERE PlayerID = {}", player->GetGUID().GetCounter());
            player->SetLevel((*result)[1].Get<uint8>(), true);
            player->SetUInt32Value(PLAYER_XP, (*result)[2].Get<uint32>());
        }
    }

    void SetToBGLevel(Player* player)
    {
        uint32 levelToSet = sConfigMgr->GetOption<uint32>("CustomBGLevel.Level", 80);
        CharacterDatabase.Execute("INSERT INTO custom_bg_level (PlayerID, original_level, original_xp) VALUES ({}, {}, {})", player->GetGUID().GetCounter(), player->getLevel(), player->GetUInt32Value(PLAYER_XP));
        player->SetLevel(levelToSet, true);
    }

public:
    PS_BGLevel() : PlayerScript("Player_BGLevel") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("CustomBGLevel.Enable", false))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Custom BG Level is Enabled");
            if (!player->InBattleground())
            {
                RestoreOriginalLevel(player);
            }
        }
    }
};

// Add all scripts in one
void AddSC_mod_bg_set_level()
{
    new PS_BGLevel();
    new BGS_BGLevel();
}
