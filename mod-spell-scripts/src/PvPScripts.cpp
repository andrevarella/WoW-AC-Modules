
/*
#include "BattlegroundBE.h"
#include "BattlegroundDS.h"
#include "BattlegroundMgr.h"
#include "BattlegroundNA.h"
#include "BattlegroundRL.h"
#include "BattlegroundRV.h"
#include "MapMgr.h"

#include "ArenaSpectator.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Config.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "BattlegroundHandler.cpp"
*/

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Chat.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "Pet.h"
#include "Configuration/Config.h"


// em lua da p usar:
// CanEquipItem, (InArenaQueue) + Get Map -> InArena - N deixa equipar qnd tiver dentro de arena. | N deixa equipar qnd tiver na fila. 
// CanJoinInArenaQueue - N deixa equipar qnd for joinar


class ArenaScripts : public BGScript
{
public:
    ArenaScripts() : BGScript("ArenaScripts") {}


    void OnBattlegroundUpdate(Battleground* bg, uint32 diff) override
    {
        int32 startDelayTime = bg->GetStartDelayTime();
        //uint32 startTime = bg->GetStartTime();

        // Verifica se algum jogador possui a aura SPELL_BG_STARTBG
        bool startBGAura = false;
        for (const auto& player : bg->GetPlayers())
        {
            if (!player.second->IsSpectator())
            {
                if (player.second->HasAura(83400))
                {
                    startBGAura = true;
                    break;
                }
            }
        }
        // Se a spell 83400 foi lançada e o startDelayTime for maior que 5000, ajusta o tempo
        if (startBGAura && startDelayTime > 5000)
        {
            bg->SetStartDelayTime(5000);
            bg->SetStartTime(bg->GetStartTime() + (startDelayTime - 5000));

            for (auto& playerPair : bg->GetPlayers())
            {
                Player* player = playerPair.second;
                player->RemoveAurasDueToSpell(44521); // Preparation (arena?)
                player->ResetAllPowers();
                player->ResetCooldownsPortaArena();   // Reset Cooldowns ao abrir a porta de BG
                player->RemoveAurasDueToSpell(57723); // Exhaustion
                player->RemoveAurasDueToSpell(57724); // Sated
                player->RemoveAurasDueToSpell(83008); // Preparation Haste (BG)
                player->RemoveAurasDueToSpell(2825);  // Bloodlust
                player->RemoveAurasDueToSpell(32182); // Heroism
                player->RemoveAurasDueToSpell(25771); // Forbearance
                player->RemoveAurasDueToSpell(61998); // Forbearance (server side)
                player->RemoveAurasDueToSpell(61987); // Avenging server side
                player->RemoveAurasDueToSpell(41425); // Hipothermia
                player->RemoveAurasDueToSpell(6788);  // Weakened Soul
                player->RemoveAurasDueToSpell(79501); // Avenging Forbearance (custom)
            }
        }

        // Modificar o Draw timer em Battleground.cpp "(GetStartTime() >= 21 * MINUTE * IN_MILLISECONDS) // pussywizard: 1min startup + 20min allowed duration (Draw!)"
        // Add "sScriptMgr->OnBattlegroundStart(this);" em baixo de "player->UpdateObjectVisibility(true);"


        // Dementia / dampening - Add Stacks on pet based on player stacks
        for (const auto& playerPair : bg->GetPlayers())
        {
            Player* player = playerPair.second;
            // Verifica se o jogador está efetivamente no mundo para evitar crashes
            if (player && player->IsInWorld() && !player->IsSpectator() && !player->IsGameMaster() && player->GetPet())
            {
                if ((bg->GetArenaType() == ARENA_TYPE_2v2 || bg->GetArenaType() == ARENA_TYPE_5v5) && (bg->GetStartTime() >= 11 * MINUTE * IN_MILLISECONDS)) // 1m startup + 10min
                {
                    Pet* pet = player->GetPet(); // Garante que o pet também está no mundo
                    if (pet && pet->IsInWorld())
                    {
                        Aura* demAura = player->GetAura(41406);
                        if (demAura)
                        {
                            Aura* pemAura = pet->GetAura(41406);
                            if (pemAura)
                            {
                                int32 playerStackAmount = demAura->GetStackAmount();
                                pemAura->ModStackAmount(playerStackAmount - pemAura->GetStackAmount());
                            }
                            else
                            {
                                pet->AddAura(41406, pet);
                                if (Aura* newPetAura = pet->GetAura(41406))
                                {
                                    newPetAura->SetStackAmount(demAura->GetStackAmount());
                                }
                            }
                        }
                    }
                }
                else if((bg->GetArenaType() == ARENA_TYPE_3v3) && (bg->GetStartTime() >= 9 * MINUTE * IN_MILLISECONDS))
                {
                    Pet* pet = player->GetPet();
                    if (pet && pet->IsInWorld())
                    {
                        Aura* demAura = player->GetAura(41406);
                        if (demAura)
                        {
                            Aura* pemAura = pet->GetAura(41406);
                            if (pemAura)
                            {
                                int32 playerStackAmount = demAura->GetStackAmount();
                                pemAura->ModStackAmount(playerStackAmount - pemAura->GetStackAmount());
                            }
                            else
                            {
                                pet->AddAura(41406, pet);
                                if (Aura* newPetAura = pet->GetAura(41406))
                                {
                                    newPetAura->SetStackAmount(demAura->GetStackAmount());
                                }
                            }
                        }
                    }
                }
                // else if((bg->GetArenaType() == SOLOQUEUE)
                // RemoveAura(32727) - Arena Preparation (da mod_invisibility)
                //  player->UpdateObjectVisibilityModule(true); / player->RemoveAurasDueToSpell(32727); (arena prep / da inv)
            }
        }
    }

    void OnBattlegroundStart(Battleground* bg) override
    {
        if (bg->isArena())
        {
            for (const auto& playerPair : bg->GetPlayers())
            {
                Player* player = playerPair.second;
                player->RemoveAurasDueToSpell(32727); // Preparation (arena?)
                //player->ResetAllPowers(); // ja tem no core acho
                player->ResetCooldownsPortaArena();   // Reset CDs ao começar a arena (exceção p/ Fear Ward, pets, ice barrier etc)
                player->RemoveAurasDueToSpell(83025); // Preparation haste (Arena)
                player->RemoveAurasDueToSpell(83008); // Preparation Haste (BG)
                player->RemoveAurasDueToSpell(57723); // Exhaustion
                player->RemoveAurasDueToSpell(57724); // Sated
                player->RemoveAurasDueToSpell(2825);  // Bloodlust
                player->RemoveAurasDueToSpell(32182); // Heroism
                player->RemoveAurasDueToSpell(25771); // Forbearance
                player->RemoveAurasDueToSpell(61998); // Forbearance (server side)
                player->RemoveAurasDueToSpell(61987); // Avenging server side
                player->RemoveAurasDueToSpell(41425); // Hipothermia
                player->RemoveAurasDueToSpell(6788);  // Weakened Soul
                player->RemoveAurasDueToSpell(79501); // Avenging Forbearance (custom)
                player->CastSpell(player, 83026);     // Dampening/Dementia depois de 8 min

                if (Pet* pet = player->GetPet())
                {
                    if (pet->IsInWorld())
                    {
                        pet->RemoveAurasDueToSpell(83025); // Preparation haste (Arena)
                        pet->RemoveAurasDueToSpell(57723); // Exhaustion
                        pet->RemoveAurasDueToSpell(57724); // Sated
                        pet->RemoveAurasDueToSpell(2825);  // Bloodlust
                        pet->RemoveAurasDueToSpell(32182); // Heroism
                        pet->RemoveAurasDueToSpell(25771); // Forbearance
                        pet->RemoveAurasDueToSpell(61998); // Forbearance (server side)
                        pet->RemoveAurasDueToSpell(6788);  // Weakened Soul
                        pet->RemoveAurasDueToSpell(48066); // Power Word: Shield (em pets)
                    }
                }
            }
        }
        // BGs
        else
        {
            for (const auto& playerPair : bg->GetPlayers())
            {
                Player* player = playerPair.second;
                player->RemoveAurasDueToSpell(44521); // Preparation (bg?)
                //player->ResetAllPowers();
                player->ResetCooldownsPortaArena();   // Reset Cooldowns ao abrir a porta de BG
                player->RemoveAurasDueToSpell(57723); // Exhaustion
                player->RemoveAurasDueToSpell(57724); // Sated
                player->RemoveAurasDueToSpell(83008); // Preparation Haste (BG)
                player->RemoveAurasDueToSpell(2825);  // Bloodlust
                player->RemoveAurasDueToSpell(32182); // Heroism
                player->RemoveAurasDueToSpell(25771); // Forbearance
                player->RemoveAurasDueToSpell(61998); // Forbearance (server side)
                player->RemoveAurasDueToSpell(61987); // Avenging server side
                player->RemoveAurasDueToSpell(41425); // Hipothermia
                player->RemoveAurasDueToSpell(6788);  // Weakened Soul
                player->RemoveAurasDueToSpell(79501); // Avenging Forbearance (custom)


                if (Pet* pet = player->GetPet())
                {
                    if (pet->IsInWorld())
                    {
                        pet->RemoveAurasDueToSpell(83008); // Preparation Haste (BG)
                        pet->RemoveAurasDueToSpell(57723); // Exhaustion
                        pet->RemoveAurasDueToSpell(57724); // Sated
                        pet->RemoveAurasDueToSpell(2825);  // Bloodlust
                        pet->RemoveAurasDueToSpell(32182); // Heroism
                        pet->RemoveAurasDueToSpell(25771); // Forbearance
                        pet->RemoveAurasDueToSpell(61998); // Forbearance (server side)
                    }
                }
            }
        }
    }


    void OnBattlegroundAddPlayer(Battleground* bg, Player* player) override
    {
        // UnEquip item ilvl 284 ao entrar ? (tem q ter check se o player tem bag))

        if (bg->isArena())
        {
            if (bg->GetStatus() == STATUS_WAIT_JOIN) // not started yet
            {
                player->CastSpell(player, 83025, true);  // Arena Preparation Haste - reduces all mana cost of spells. Adicionado ao entrar em arena.
            }
        }
        else // BGs
        {
            player->RemoveAurasDueToSpell(57723);
            player->RemoveAurasDueToSpell(57724);
            player->ResetAllPowers();
            player->ResetCooldownsPortaArena();

            //if (bg->GetStatus() == STATUS_WAIT_JOIN) // not started yet

            if (bg->GetStatus() != STATUS_IN_PROGRESS)
                player->CastSpell(player, 83008, true);   // Buff Prep BG (Sem custo reagent + 50% haste para se preparar mais rapido). Adicionado ao entrar em BG.
        }
    }


};

/*
// mudar p/ UpdateObjectVisibilityModule (tem que criar script em unit.cpp tbm acho)
void Player::UpdateObjectVisibility(bool forced, bool fromUpdate)
{
    // Prevent updating visibility if player is not in world (example: LoadFromDB sets drunkstate which updates invisibility while player is not in map)
    if (!IsInWorld())
        return;

    if (!forced)
        AddToNotify(NOTIFY_VISIBILITY_CHANGED);

    else if (!isBeingLoaded())
    {
        if (!fromUpdate) // pussywizard:
        {
            bRequestForcedVisibilityUpdate = true;
            return;
        }
        Unit::UpdateObjectVisibility(true);
        UpdateVisibilityForPlayer();
    }
}*/

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
                // credit quest PVP
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
                break;
            case 3358:
                player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.AB", 20559), RewardCount);
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
                break;
            case 3820:
                player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.EY", 29024), RewardCount);
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
                break;
            case 4710:
                player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.IC", 47395), RewardCount);
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
                break;
            case 4384:
                player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.SA", 42425), RewardCount);
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
                break;
            case 2597:
                player->AddItem(sConfigMgr->GetOption<int32>("Battleground.Reward.ItemID.AV", 20560), RewardCount);
                if ((player->GetQuestStatus(30014) == QUEST_STATUS_INCOMPLETE)) // Win 5 Battlegrounds Quest
                    player->KilledMonsterCredit(94176);
                if ((player->GetQuestStatus(30025) == QUEST_STATUS_INCOMPLETE)) // Win 1 Battleground Quest
                    player->KilledMonsterCredit(94177);
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

            /* // Add isso depois, pega a quantidade de slots livres do player, antes de dar caixa
            bool OnUse(Player* player, Item* item, const SpellCastTargets & /*targets*BARRA)
            {

                // Adding items
                uint32 noSpaceForCount = 0;
                int count = 1;

                // check space and find places
                ItemPosCountVec dest;
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->GetEntry(), count, &noSpaceForCount);
                if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
                    count -= noSpaceForCount;

                if (count == 0 || dest.empty())
                {
                    player->GetSession()->SendNotification("|cffff0000Cleanup your inventory, you need atleast 2 free slots in your bag in order to use the Arena Case.|r");
                    return false;
                }
            }
            */

            if (bg->isRated()) // Rated Arena
            {
                switch (bg->GetArenaType())
                {
                case ARENA_TEAM_2v2:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "2v2", RewardCount);
                    // credit quest PVP
                    if ((player->GetQuestStatus(30010) == QUEST_STATUS_INCOMPLETE)) // Win 10 Games in 2v2 Quest
                        player->KilledMonsterCredit(94171);
                    break;

                case ARENA_TEAM_3v3:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "3v3", RewardCount);
                    if ((player->GetQuestStatus(30011) == QUEST_STATUS_INCOMPLETE)) // Win 10 Games in 3v3 Quest
                        player->KilledMonsterCredit(94172);
                    break;

                case ARENA_TEAM_5v5:
                    ArenaRewardItem(player, bgTeamId, winnerTeamId, "5v5", RewardCount);
                    if ((player->GetQuestStatus(30012) == QUEST_STATUS_INCOMPLETE)) // Win 10 Games in 5v5 Quest
                        player->KilledMonsterCredit(94173);
                    break;

                default:
                    break;
                }
            }
            else // Skirmish Arenas
            {
                if (player->getLevel() <= 79) // (apenas do lvl 79 pra baixo)
                {
                    if (player->HasAura(83025)) // Arena Preparation
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



void AddPvPScripts()
{
    // BG Stuff
    new ArenaScripts();

    // BG Reward
    new BGScript_BattlegroundsReward();
}
