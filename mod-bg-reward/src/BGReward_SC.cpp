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

        if (sConfigMgr->GetOption<bool>("Battleground.Reward.Enable", true) && !bg->isArena())
        {
            if (bgTeamId == winnerTeamId)
                RewardCount = sConfigMgr->GetOption<int32>("Battleground.Reward.WinnerTeam.Count", 2);
            else
                RewardCount = sConfigMgr->GetOption<int32>("Battleground.Reward.LoserTeam.Count", 1);

            switch (player->GetZoneId())
            {
                case 3277:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.WS", 20558), RewardCount);
                    break;
                case 3358:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.AB", 20559), RewardCount);
                    break;
                case 3820:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.EY", 29024), RewardCount);
                    break;
                case 4710:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.IC", 47395), RewardCount);
                    break;
                case 4384:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.SA", 42425), RewardCount);
                    break;
                case 2597:
                    player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.AV", 20560), RewardCount);
                    break;
                default:
                    break;
            }
        }

        if (sConfigMgr->GetOption<bool>("Arena.Reward.Enable", true) && bg->isArena())
        {

            if (bgTeamId == winnerTeamId)
                RewardCount = sConfigMgr->GetOption<int32>("Arena.Reward.WinnerTeam.Count", 2);
            else
                RewardCount = sConfigMgr->GetOption<int32>("Arena.Reward.LoserTeam.Count", 1);

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
            else // Skirmish Arenas
            {
                if (player->getLevel() <= 79) // (apenas do lvl 79 pra baixo)
                {
                    if (player->HasAura(83025))
                    {
                        return;
                    }

                    int xpWinReward = 10000;
                    int xpLossReward = 4500;

                    if (10 <= player->getLevel() && player->getLevel() <= 14) // (<= é igual ou menor)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.10to14.Win", 10000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.10to14.Loss", 4500);
                    }
                    if (15 <= player->getLevel() && player->getLevel() <= 19)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.15to19.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.15to19.Loss", 6000);
                    }
                    if (20 <= player->getLevel() && player->getLevel() <= 24)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.20to24.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.20to24.Loss", 6000);
                    }
                    if (25 <= player->getLevel() && player->getLevel() <= 29)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.25to29.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.25to29.Loss", 6000);
                    }
                    if (30 <= player->getLevel() && player->getLevel() <= 34)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.30to34.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.30to34.Loss", 6000);
                    }
                    if (35 <= player->getLevel() && player->getLevel() <= 39)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.35to39.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.35to39.Loss", 6000);
                    }
                    if (40 <= player->getLevel() && player->getLevel() <= 44)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.40to44.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.40to44.Loss", 6000);
                    }
                    if (45 <= player->getLevel() && player->getLevel() <= 49)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.45to49.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.45to49.Loss", 6000);
                    }
                    if (50 <= player->getLevel() && player->getLevel() <= 54)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.50to54.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.50to54.Loss", 6000);
                    }
                    if (55 <= player->getLevel() && player->getLevel() <= 59)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.55to59.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.55to59.Loss", 6000);
                    }
                    if (60 <= player->getLevel() && player->getLevel() <= 64)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.60to64.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.60to64.Loss", 6000);
                    }
                    if (65 <= player->getLevel() && player->getLevel() <= 69)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.65to69.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.65to69.Loss", 6000);
                    }
                    if (70 <= player->getLevel() && player->getLevel() <= 74)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.70to74.Win", 14000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.70to74.Loss", 6000);
                    }
                    else if (75 <= player->getLevel() && player->getLevel() <= 79)
                    {
                        xpWinReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.75to79.Win", 50000);
                        xpLossReward = sConfigMgr->GetIntDefault("Arena.Reward.XP.75to79.Loss", 27500);
                    }

                    if (bgTeamId == winnerTeamId) // Skirmish Win <= Level 79
                    {
                        player->GiveXP(xpWinReward, nullptr);

                        if (70 <= player->getLevel() && player->getLevel() <= 79) // Verifica se o nível está entre 70 e 79
                        {
                            if (rand() % 100 < 75) // 75% de chance de ganhar a caixa de arena skirmish
                            {
                                player->AddItem(sConfigMgr->GetIntDefault("Arena.Reward.Skirmish.Item70.79", 32545), 1);
                            }
                        }

                        int honorPoints;
                        if (10 <= player->getLevel() && player->getLevel() <= 19)
                        {
                            honorPoints = rand() % 11 + 15; // honor reward de 15 ate 25 honor
                        }
                        if (20 <= player->getLevel() && player->getLevel() <= 29)
                        {
                            honorPoints = rand() % 9 + 35; // honor reward de 27 ate 35 honor
                        }
                        if (30 <= player->getLevel() && player->getLevel() <= 39)
                        {
                            honorPoints = rand() % 9 + 37; // honor reward de 37 ate 45 honor
                        }
                        if (40 <= player->getLevel() && player->getLevel() <= 49)
                        {
                            honorPoints = rand() % 9 + 47; // honor reward de 47 ate 55 honor
                        }
                        if (50 <= player->getLevel() && player->getLevel() <= 59)
                        {
                            honorPoints = rand() % 11 + 55; // honor reward de 55 ate 65 honor
                        }
                        if (60 <= player->getLevel() && player->getLevel() <= 69)
                        {
                            honorPoints = rand() % 16 + 70; // honor reward de 70 ate 85 honor
                        }
                        else if (70 <= player->getLevel() && player->getLevel() <= 79)
                        {
                            honorPoints = rand() % 11 + 95; // honor reward de 95 ate 105 honor
                        }
                        player->ModifyHonorPoints(honorPoints);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);

                        // Gold:
                        int goldReward;
                        if (10 <= player->getLevel() && player->getLevel() <= 19)
                        {
                            goldReward = rand() % 1 + 1; // 1 a 2 gold
                        }
                        if (20 <= player->getLevel() && player->getLevel() <= 29)
                        {
                            goldReward = rand() % 2 + 3; // 3 a 4 gold
                        }
                        if (30 <= player->getLevel() && player->getLevel() <= 39)
                        {
                            goldReward = rand() % 2 + 4; // 4 a 5 gold
                        }
                        if (40 <= player->getLevel() && player->getLevel() <= 49)
                        {
                            goldReward = rand() % 2 + 5; // 5 a 6 gold
                        }
                        if (50 <= player->getLevel() && player->getLevel() <= 59)
                        {
                            goldReward = rand() % 3 + 6; // 6 a 8 gold
                        }
                        if (60 <= player->getLevel() && player->getLevel() <= 69)
                        {
                            goldReward = rand() % 5 + 8; // 8 a 12 gold
                        }
                        else if (70 <= player->getLevel() && player->getLevel() <= 79)
                        {
                            goldReward = rand() % 5 + 14; // 14 a 18 gold
                        }
                        player->ModifyMoney(goldReward);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d gold.", goldReward);
                    }
                    else // Skirmish Loss <= Level 79
                    {
                        player->GiveXP(xpLossReward, nullptr);

                        int honorPoints;
                        if (10 <= player->getLevel() && player->getLevel() <= 19)
                        {
                            honorPoints = rand() % 11 + 7; // honor reward de 7 ate 14 honor
                        }
                        if (20 <= player->getLevel() && player->getLevel() <= 29)
                        {
                            honorPoints = rand() % 6 + 15; // honor reward de 15 ate 20 honor
                        }
                        if (30 <= player->getLevel() && player->getLevel() <= 39)
                        {
                            honorPoints = rand() % 6 + 20; // honor reward de 20 ate 25 honor
                        }
                        if (40 <= player->getLevel() && player->getLevel() <= 49)
                        {
                            honorPoints = rand() % 6 + 25; // honor reward de 25 ate 30 honor
                        }
                        if (50 <= player->getLevel() && player->getLevel() <= 59)
                        {
                            honorPoints = rand() % 6 + 30; // honor reward de 30 ate 35 honor
                        }
                        if (60 <= player->getLevel() && player->getLevel() <= 69)
                        {
                            honorPoints = rand() % 16 + 40; // honor reward de 40 ate 55 honor
                        }
                        else if (70 <= player->getLevel() && player->getLevel() <= 79)
                        {
                            honorPoints = rand() % 11 + 70; // honor reward de 60 ate 70 honor
                        }
                        player->ModifyHonorPoints(honorPoints);
                        ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %d honor points.", honorPoints);

                        // Gold:
                        int goldReward;
                        if (10 <= player->getLevel() && player->getLevel() <= 19)
                        {
                            goldReward = rand() % 1 + 1; // 1 a 2 gold
                        }
                        if (20 <= player->getLevel() && player->getLevel() <= 29)
                        {
                            goldReward = rand() % 2 + 3; // 3 a 4 gold
                        }
                        if (30 <= player->getLevel() && player->getLevel() <= 39)
                        {
                            goldReward = rand() % 2 + 4; // 4 a 5 gold
                        }
                        if (40 <= player->getLevel() && player->getLevel() <= 49)
                        {
                            goldReward = rand() % 2 + 5; // 5 a 6 gold
                        }
                        if (50 <= player->getLevel() && player->getLevel() <= 59)
                        {
                            goldReward = rand() % 3 + 6; // 6 a 8 gold
                        }
                        if (60 <= player->getLevel() && player->getLevel() <= 69)
                        {
                            goldReward = rand() % 5 + 8; // 8 a 12 gold
                        }
                        else if (70 <= player->getLevel() && player->getLevel() <= 79)
                        {
                            goldReward = rand() % 6 + 4; // 4 a 9
                        }
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
            if (player->HasAura(83025)) // Arena Preparation
            {
                return;
            }
            player->AddItem(sConfigMgr->GetOption<int32>("Arena.Reward.Winner.ItemID." + Type, 32544), RewardCount); // Caixa de Arena (x2/x3)
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
            player->AddItem(sConfigMgr->GetOption<int32>("Arena.Reward.Loser.ItemID." + Type, 29434), RewardCount);  // Badge of Justice
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
