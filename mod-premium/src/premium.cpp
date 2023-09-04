#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "Config.h"

enum Vendors
{
    NPC_VENDOR_A = 54,
    NPC_VENDOR_H = 3163,
    NPC_ARENA_A = 32330,
    NPC_ARENA_H = 32330,
    NPC_ArenaTeam_A = 93072,
    NPC_ArenaTeam_H = 93072,
    NPC_PETTRAINER_A = 10088,
    NPC_PETTRAINER_H = 10088,
    NPC_TRANSMOG_A = 94132,
    NPC_TRANSMOG_H = 94132,
    NPC_REAGENT_A = 80019,
    NPC_REAGENT_H = 80019,
    NPC_FOOD_A = 32403,
    NPC_FOOD_H = 32403,
    NPC_AUCTION_H = 9856,
    NPC_AUCTION_A = 8670
};

enum Trainers
{
    // Alliance
    DRUID_A = 5504,
    HUNTER_A = 5515,
    MAGE_A = 5497,
    PALADIN_A = 928,
    PRIEST_A = 376,
    ROGUE_A = 918,
    SHAMAN_A = 20407,
    WARLOCK_A = 461,
    WARRIOR_A = 5479,

    // Horde
    DRUID_H = 3033,
    HUNTER_H = 3406,
    MAGE_H = 5883,
    PALADIN_H = 23128,
    PRIEST_H = 3045,
    ROGUE_H = 3401,
    SHAMAN_H = 3344,
    WARLOCK_H = 3324,
    WARRIOR_H = 3354,

    DEATHKNIGHT_AH  = 28472
};

enum Mounts
{
    HUMAN_MOUNT     = 470,
    ORC_MOUNT       = 6653,
    GNOME_MOUNT     = 17454,
    NIGHTELF_MOUNT  = 8394,
    DWARF_MOUNT     = 6899,
    UNEAD_MOUNT     = 17463,
    TAUREN_MOUNT    = 64657,
    TROLL_MOUNT     = 8395,
    BLOODELF_MOUNT  = 35022,
    DRAENEI_MOUNT   = 34406
};

class premium_account : public ItemScript
{
public:
    premium_account() : ItemScript("premium_account") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (player->IsInCombat())
            return false;

        if (player->InBattleground())
        {
            std::ostringstream ss2;
            ss2 << "Item Proibido de usar em BG";
            player->GetSession()->SendNotification(SERVER_MSG_STRING, ss2.str().c_str());
            return false;
        }

        ClearGossipMenuFor(player);

        if (sConfigMgr->GetBoolDefault("Arena", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_arena_2v2_7.png:29|t Joinar Arena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);

        if (sConfigMgr->GetOption<bool>("Morph", true))
        {
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Ability_rogue_disguise.png:29|t Morph Aleatorio (21 disponíveis)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:22:22:0:0|t Remover Morph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }

        if (sConfigMgr->GetBoolDefault("Transmogrifier", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_boss_nexus_prince_shaffar.png:29|t Transmogrifier", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);

        if (sConfigMgr->GetOption<bool>("Mount", true))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT_16, "Mount", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

        if (sConfigMgr->GetBoolDefault("Bank", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Inv_misc_enggizmos_17.png:29|t Banco", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        if (sConfigMgr->GetBoolDefault("Auction House", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Inv_misc_coin_02.png:29|t Auction House", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

        if (sConfigMgr->GetBoolDefault("MailBox", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Inv_letter_09.png:29|t Correio", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

        if (sConfigMgr->GetBoolDefault("Extras", true))
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_bg_returnxflags_def_wsg.png:29|t Extras", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

        return false; // Cast the spell on use normally
    }

    void OnGossipSelect(Player* player, Item* item, uint32 /*sender*/, uint32 action) override
    {
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1: /*Morph*/
            {
                CloseGossipMenuFor(player);
                ApplyRandomMorph(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 2: /*Demorph*/
            {
                player->DeMorph();
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3: /*Show Bank*/
            {
                player->GetSession()->SendShowBank(player->GetGUID());
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 4: /*Mail Box*/
            {
                player->GetSession()->SendShowMailBox(player->GetGUID());
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 5: /*Vendor*/
            {
                uint32 vendorId = 0;
                std::string salute;
                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    vendorId = NPC_VENDOR_A;
                    salute = "Greetings";
                } else {
                    vendorId = NPC_VENDOR_H;
                    salute = "Zug zug";
                }

                SummonTempNPC(player, vendorId, salute.c_str());
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 6: /*Mount*/
            {
                CloseGossipMenuFor(player);
                switch (player->getRace())
                {
                    case RACE_HUMAN:         player->CastSpell(player, HUMAN_MOUNT); break;
                    case RACE_ORC:           player->CastSpell(player, ORC_MOUNT); break;
                    case RACE_GNOME:         player->CastSpell(player, GNOME_MOUNT); break;
                    case RACE_NIGHTELF:      player->CastSpell(player, NIGHTELF_MOUNT); break;
                    case RACE_DWARF:         player->CastSpell(player, DWARF_MOUNT); break;
                    case RACE_DRAENEI:       player->CastSpell(player, DRAENEI_MOUNT); break;
                    case RACE_UNDEAD_PLAYER: player->CastSpell(player, UNEAD_MOUNT); break;
                    case RACE_TAUREN:        player->CastSpell(player, TAUREN_MOUNT); break;
                    case RACE_TROLL:         player->CastSpell(player, TROLL_MOUNT); break;
                    case RACE_BLOODELF:      player->CastSpell(player, BLOODELF_MOUNT); break;
                }
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 7: /*Auction House*/
            {
                uint32 auctionId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    auctionId = NPC_AUCTION_H;
                }
                else
                {
                    auctionId = NPC_AUCTION_A;
                }

                SummonTempNPC(player, auctionId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 10: /*Arena*/
            {
                uint32 arenaId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    arenaId = NPC_ARENA_H;
                }
                else
                {
                    arenaId = NPC_ARENA_A;
                }

                SummonTempNPC(player, arenaId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 11: /*Create Arena Team*/
            {
                uint32      ArenaTeamId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    ArenaTeamId = NPC_ArenaTeam_H;
                }
                else
                {
                    ArenaTeamId = NPC_ArenaTeam_A;
                }

                SummonTempNPC(player, ArenaTeamId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 13: /*Hunter Pet Trainer*/
            {
                uint32      PETTRAINERId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    PETTRAINERId = NPC_PETTRAINER_H;
                }
                else
                {
                    PETTRAINERId = NPC_PETTRAINER_A;
                }

                SummonTempNPC(player, PETTRAINERId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 15: /*NPC Reagents*/
            {
                uint32      REAGENTId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    REAGENTId = NPC_REAGENT_H;
                }
                else
                {
                    REAGENTId = NPC_REAGENT_A;
                }

                SummonTempNPC(player, REAGENTId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 16: /*NPC Foods*/
            {
                uint32      FOODId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    FOODId = NPC_FOOD_H;
                }
                else
                {
                    FOODId = NPC_FOOD_A;
                }

                SummonTempNPC(player, FOODId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 17: /*NPC Transmog*/
            {
                uint32      TRANSMOGId = 0;
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    TRANSMOGId = NPC_TRANSMOG_H;
                }
                else
                {
                    TRANSMOGId = NPC_TRANSMOG_A;
                }

                SummonTempNPC(player, TRANSMOGId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 8: /* Class Trainers*/
            {
                uint32 trainerId = 0;
                switch (player->getClass())
                {
                    case CLASS_ROGUE:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? ROGUE_A : ROGUE_H;
                        break;
                    case CLASS_WARRIOR:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARRIOR_A : WARRIOR_H;
                        break;
                    case CLASS_PRIEST:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PRIEST_A : PRIEST_H;
                        break;
                    case CLASS_MAGE:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? MAGE_A : MAGE_H;
                        break;
                    case CLASS_PALADIN:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PALADIN_A : PALADIN_H;
                        break;
                    case CLASS_HUNTER:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? HUNTER_A : HUNTER_H;
                        break;
                    case CLASS_DRUID:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? DRUID_A : DRUID_H;
                        break;
                    case CLASS_SHAMAN:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? SHAMAN_A : SHAMAN_H;
                        break;
                    case CLASS_WARLOCK:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARLOCK_A : WARLOCK_H;
                        break;
                    case CLASS_DEATH_KNIGHT:
                        trainerId = DEATHKNIGHT_AH;
                        break;
                }

                SummonTempNPC(player, trainerId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 9: /*Player Interactions*/
            {
                ClearGossipMenuFor(player);

                if (sConfigMgr->GetBoolDefault("Food & Water", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Inv_thanksgiving_turkey.png:29|t Agua e Comida", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);

                if (sConfigMgr->GetBoolDefault("Reagent Vendor", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Spell_holy_prayeroffortitude.png:29|t Reagentes", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);

                if (sConfigMgr->GetBoolDefault("Vendor", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Trade_blacksmithing.png:29|t Reparar Armadura", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

                if (sConfigMgr->GetBoolDefault("Class Trainer", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Spell_holy_spiritualguidence.png:29|t Treinador de Classe", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);

                if (sConfigMgr->GetBoolDefault("Pet Trainer", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Ability_hunter_beastcall.png:29|t Treinador de Pet (Hunter)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);

                if (sConfigMgr->GetBoolDefault("Create.Arena.Team", true))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Inv_scroll_11.png:29|t Criar Time de Arena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);

                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
                break;
            }
        }
    }

    void ApplyRandomMorph(Player* player)
    {
        uint32 random = (urand(1, 22)); // Change this line when adding more morphs
        {
            switch (random)
            {
            case 1: player->SetDisplayId(20368); break;     // BE Male        
            case 2: player->SetDisplayId(20370); break;     // BE Female
            case 3: player->SetDisplayId(20580); break;     // Gnome Male
            case 4: player->SetDisplayId(20320); break;     // Gnome Female
            case 5: player->SetDisplayId(20319); break;     // Tauren Male
            case 6: player->SetDisplayId(20584); break;     // Tauren Female
            case 7: player->SetDisplayId(20323); break;     // Draenei Female
            case 8: player->SetDisplayId(20321); break;     // Troll Male
            case 9: player->SetDisplayId(20317); break;     // Dwarf Male
            case 10: player->SetDisplayId(20316); break;    // Orc FeMale
            case 11: player->SetDisplayId(17662); break;    // Undead T2 PvP
            case 12: player->SetDisplayId(3533); break;     // Undead Male Set Diferente
            case 13: player->SetDisplayId(3529); break;     // Undead Female Set Diferente
            case 14: player->SetDisplayId(25037); break;    // Human Pirate Male
            case 15: player->SetDisplayId(25042); break;    // Undead Pirate
            case 16: player->SetDisplayId(25048); break;    // Human Pirate Female
            case 17: player->SetDisplayId(25053); break;    // Undead Pirate Female
            case 18: player->SetDisplayId(25054); break;    // Human Pirate Female
            case 19: player->SetDisplayId(7550); break;     // Caveira Noggenfogger
            case 20: player->SetDisplayId(25067); break;     // Orc Male
            case 21: player->SetDisplayId(25049); break;     // Nelf Pirata female
            case 22: player->SetDisplayId(25045); break;     // Dwarf pirata Female

                default:
                    break;
            }
        }
    }

    void SummonTempNPC(Player* player, uint32 entry, const char* salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sConfigMgr->GetOption<int32>("Premium.NpcDuration", 20) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 20;

        Creature* npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());

        if (salute && !(salute[0] == '\0'))
            npc->Whisper(salute, LANG_UNIVERSAL, player, false);
    }
};

void AddPremiumAccount()
{
    new premium_account();
}
