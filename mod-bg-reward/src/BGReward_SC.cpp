/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Battleground.h"
#include "Configuration/Config.h"
#include "Chat.h"

class BGScript_BattlegroundsReward : public BGScript
{
public:
    BGScript_BattlegroundsReward() : BGScript("BGScript_BattlegroundsReward") {}

    void OnBattlegroundEndReward(Battleground* bg, Player* player, TeamId winnerTeamId) override
    {
        TeamId bgTeamId = player->GetBgTeamId();
        uint32 RewardCount = 0;

        if (sConfigMgr->GetBoolDefault("Battleground.Reward.Enable", true) && !bg->isArena())
        {
            if (bgTeamId == winnerTeamId)
                RewardCount = sConfigMgr->GetIntDefault("Battleground.Reward.WinnerTeam.Count", 2);
            else
                RewardCount = sConfigMgr->GetIntDefault("Battleground.Reward.LoserTeam.Count", 1);

            switch (player->GetZoneId())
            {
                case 3277:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.WS", 20558), RewardCount);
                    break;
                case 3358:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.AB", 20559), RewardCount);
                    break;
                case 3820:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.EY", 29024), RewardCount);
                    break;
                case 4710:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.IC", 47395), RewardCount);
                    break;
                case 4384:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.SA", 42425), RewardCount);
                    break;
                case 2597:
                    player->AddItem(sConfigMgr->GetIntDefault("Battleground.Reward.ItemID.AV", 20560), RewardCount);
                    break;
                default:
                    break;
            }
        }

        if (sConfigMgr->GetBoolDefault("Arena.Reward.Enable", true) && bg->isArena())
        {
            if (bgTeamId == winnerTeamId)
                RewardCount = sConfigMgr->GetIntDefault("Arena.Reward.WinnerTeam.Count", 2);
            else
                RewardCount = sConfigMgr->GetIntDefault("Arena.Reward.LoserTeam.Count", 1);

            if (bg->isRated()) // Rated Arena
            {
                switch (bg->GetArenaType())
                {
                case ARENA_TEAM_2v2:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "2v2", RewardCount);
                    break;
                case ARENA_TEAM_3v3:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "3v3", RewardCount);
                    break;
                case ARENA_TEAM_5v5:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "5v5", RewardCount);
                    break;
                }
            }
            else // Skirmish Arenas (apenas do lvl 79 pra baixo)
            {
                if (player->getLevel() <= 79)
                {
                    // Verifique se o jogador tem a aura com o ID 12345
                    if (player->HasAura(83025))
                    {
                        return;
                    }
                    if (bgTeamId == winnerTeamId) // Skirmish <= 79 Win
                    {
                        if (rand() % 100 < 75) // 75% de chance de ganhar a caixa de arena skirmish
                        {
                            player->AddItem(sConfigMgr->GetIntDefault("Arena.Reward.Skirmish.ItemID", 32545), 1);
                        }

                        float xpMultiplier = 1.0; // // Aplicar multiplicador de XP com base no nível do jogador
                        if (player->getLevel() >= 74)
                        {
                            xpMultiplier = 1.2; // 60k acho
                        }
                        if (player->getLevel() >= 77) // 75k atualmente (+50%)
                        {
                            xpMultiplier = 1.5;
                        }
                        int xpReward = (int)(50000 * xpMultiplier); // 50k XP para quem ganha
                        player->GiveXP(xpReward, nullptr);

                        int honorPoints = rand() % 36 + 115; // honor reward de 115 ate 150
                        player->ModifyHonorPoints(honorPoints);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);
                        int goldReward = rand() % 11 + 5; // 5 a 15 gold
                        player->ModifyMoney(goldReward);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d gold.", goldReward);
                    }
                    else // Skirmish Loss (Level <= 79)
                    {
                        if (player->HasAura(83025))
                        {
                            return;
                        }

                        // Aplicar multiplicador de XP com base no nível do jogador
                        float xpMultiplier = 1.0;
                        if (player->getLevel() >= 74)
                        {
                            xpMultiplier = 1.2;
                        }
                        if (player->getLevel() >= 77) // 52,5k XP se for 77+
                        {
                            xpMultiplier = 1.4;
                        }
                        int xpReward = (int)(35000 * xpMultiplier); // 35k XP para quem perde
                        player->GiveXP(xpReward, nullptr);

                        int honorPoints = rand() % 11 + 70; // honor reward de 70 ate 80
                        player->ModifyHonorPoints(honorPoints);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);
                        int goldReward = rand() % 6 + 4; // 4 a 9 gold
                        player->ModifyMoney(goldReward);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d gold.", goldReward);
                    }
                }
            }

        }

    }

    void ArenaRewardItem(Player* player, TeamId bgTeamId, TeamId winnerTeamId, std::string Type, uint32 RewardCount)
    {
        if (bgTeamId == winnerTeamId)
        {   // rated win
            if (player->HasAura(83025)) // Arena Preparation - nao da reward se a arena acabar e o player tiver com Arena Prep
            {
                return;
            }
            player->AddItem(sConfigMgr->GetIntDefault("Arena.Reward.Winner.ItemID." + Type, 32544), RewardCount); // Caixa de Arena (x2/x3)
            srand(time(NULL));
            int honorPoints = rand() % 76 + 175; // 175 até 250 honor  (o valor da esquerda é o que varia de 0 ate X, e o valor da direita é o numero base(minimo). ex: 51 e 100, o valor minimo é 100 e vai até 150
            player->ModifyHonorPoints(honorPoints);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);
            int goldReward = rand() % 11 + 10; // 10 a 20 gold
            player->ModifyMoney(goldReward);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d gold.", goldReward);
        }
        else
        {   // rated loss
            if (player->HasAura(83025))
            {
                return;
            }
            player->AddItem(sConfigMgr->GetIntDefault("Arena.Reward.Loser.ItemID." + Type, 29434), RewardCount);  // Badge of Justice
            srand(time(NULL));
            int honorPoints = rand() % 31 + 70; // Rated Loss - da entre 70 a 100 honor
            player->ModifyHonorPoints(honorPoints);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);
            int goldReward = rand() % 8 + 5; // 5 a 12 gold
            player->ModifyMoney(goldReward);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d gold.", goldReward);
        }
    }
};

void AddSC_BattlegroundsReward()
{
    new BGScript_BattlegroundsReward();
}
