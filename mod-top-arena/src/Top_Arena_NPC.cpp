

#include "ScriptMgr.h"
#include <cstring>
#include <string.h>
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "Player.h"
#include "Chat.h"
#include "ScriptedGossip.h"

enum ArenaRankActionIds {
    ARENA_2V2_LADDER = GOSSIP_ACTION_INFO_DEF + 1,
    ARENA_3V3_LADDER = GOSSIP_ACTION_INFO_DEF + 2,
    ARENA_5V5_LADDER = GOSSIP_ACTION_INFO_DEF + 3,
    ARENA_GOODBYE = GOSSIP_ACTION_INFO_DEF + 4,
    ARENA_NOOP = 1,
    ARENA_START_TEAM_LOOKUP = GOSSIP_ACTION_INFO_DEF + 5,
    
};

enum ArenaRankOptions {
    ARENA_MAX_RESULTS = 30,
};

enum ArenaGossipText {
    ARENA_GOSSIP_HELLO = 11201,
    ARENA_GOSSIP_NOTEAMS = 11202,
    ARENA_GOSSIP_TEAMS = 11203,
    ARENA_GOSSIP_TEAM_LOOKUP = 11204,
    
};

class Top_Arena_NPC : public CreatureScript
{
    private:
        uint32 optionToTeamType(uint32 option) {
            uint32 teamType;
            switch(option) {
                case ARENA_2V2_LADDER: teamType = 2; break;
                case ARENA_3V3_LADDER: teamType = 3; break;
                case ARENA_5V5_LADDER: teamType = 5; break;
            }
            return teamType;
        }
        uint32 teamTypeToOption(uint32 teamType) {
            uint32 option;
            switch(teamType) {
                case 2: option = ARENA_2V2_LADDER; break;
                case 3: option = ARENA_3V3_LADDER; break;
                case 5: option = ARENA_5V5_LADDER; break;
            }
            return option;
        }
        
        std::string raceToString(uint8 race) {
            std::string race_s = "Unknown";
            switch (race)
            {
            case RACE_HUMAN:            race_s = "|TInterface\\icons\\Achievement_character_human_female:24|t";       break;
            case RACE_ORC:              race_s = "|TInterface\\icons\\Achievement_character_orc_male:24|t";         break;
            case RACE_DWARF:            race_s = "|TInterface\\icons\\Achievement_character_dwarf_male:24|t";       break;
            case RACE_NIGHTELF:         race_s = "|TInterface\\icons\\Achievement_character_nightelf_female:24|t";   break;
            case RACE_UNDEAD_PLAYER:    race_s = "|TInterface\\icons\\Achievement_character_undead_male:24|t";      break;
            case RACE_TAUREN:           race_s = "|TInterface\\icons\\Achievement_character_tauren_male:24|t";      break;
            case RACE_GNOME:            race_s = "|TInterface\\icons\\Achievement_character_gnome_female:24|t";       break;
            case RACE_TROLL:            race_s = "|TInterface\\icons\\Achievement_character_troll_male:24|t";       break;
            case RACE_BLOODELF:         race_s = "|TInterface\\icons\\Achievement_character_bloodelf_female:24|t";   break;
            case RACE_DRAENEI:          race_s = "|TInterface\\icons\\Achievement_character_draenei_female:24|t";     break;
            }
            return race_s;
        }
        
        std::string classToString(uint8 Class) {
            std::string Class_s = "Unknown";
            switch (Class)
            {
            case CLASS_WARRIOR:         Class_s = "|TInterface\\icons\\inv_sword_27.png:24|t";        break;
            case CLASS_PALADIN:         Class_s = "|TInterface\\icons\\inv_hammer_01.png:24|t";        break;
            case CLASS_HUNTER:          Class_s = "||TInterface\\icons\\inv_weapon_bow_07.png:24";         break;
            case CLASS_ROGUE:           Class_s = "|TInterface\\icons\\inv_throwingknife_04.png:24|t";          break;
            case CLASS_PRIEST:          Class_s = "|TInterface\\icons\\inv_staff_30.png:24|t";         break;
            case CLASS_DEATH_KNIGHT:    Class_s = "|TInterface\\icons\\spell_deathknight_classicon.png:24|t";   break;
            case CLASS_SHAMAN:          Class_s = "|TInterface\\icons\\inv_jewelry_talisman_04.png:24|t";         break;
            case CLASS_MAGE:            Class_s = "|TInterface\\icons\\inv_staff_13.png:24|t";           break;
            case CLASS_WARLOCK:         Class_s = "|TInterface\\icons\\spell_nature_drowsy.png:24|t";        break;
            case CLASS_DRUID:           Class_s = "|TInterface\\icons\\inv_misc_monsterclaw_04.png:24|t";          break;
            }
            return Class_s;
        }

        //TO DO

        /*std::string getPlayerStatus(ObjectGuid guid) {
            Player *player = ObjectAccessor::FindPlayer(guid);
            if(!player)
                return "Offline";
            return "Online";
        }*/
        
        std::string getWinPercent(uint32 wins, uint32 losses) {
            uint32 totalGames = wins + losses;
            if (totalGames == 0)
                return "0%";
            
            std::stringstream buf;
            uint32 percentage = (wins * 100) / totalGames;
            buf << percentage << "%";
            return buf.str();
        }
        
    public:
        Top_Arena_NPC() : CreatureScript("arenatop"){}
        
        bool OnGossipHello(Player *player, Creature *creature) {
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_arena_2v2_7:30|t Ranking 2v2\n", GOSSIP_SENDER_MAIN, ARENA_2V2_LADDER);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_arena_3v3_7:30|t Ranking 3v3\n", GOSSIP_SENDER_MAIN, ARENA_3V3_LADDER);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "|TInterface/Icons/Achievement_arena_5v5_7:30|t Ranking 5v5\n", GOSSIP_SENDER_MAIN, ARENA_5V5_LADDER);
            
            SendGossipMenuFor(player, ARENA_GOSSIP_HELLO, creature->GetGUID());
            
            return true;
        }
        
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction) {
            player->PlayerTalkClass->ClearMenus();
            
            switch(uiAction) {
                case ARENA_GOODBYE:
                {
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case ARENA_2V2_LADDER:
                case ARENA_3V3_LADDER:
                case ARENA_5V5_LADDER:
                {
                    uint32 teamType = optionToTeamType(uiAction);
                    QueryResult result = CharacterDatabase.Query(
                            "SELECT arenaTeamId, name, rating, seasonWins, seasonGames - seasonWins "
                            "FROM arena_team WHERE type = '{}' ORDER BY rating DESC LIMIT {};", teamType, ARENA_MAX_RESULTS
                            );
                    
                    if(!result) {
                        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Farewell.", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
                        SendGossipMenuFor(player, ARENA_GOSSIP_NOTEAMS, creature->GetGUID());
                    } else {
                        //uint64 rowCount = result->GetRowCount();
                        std::string name;
                        uint32 teamId, rating, seasonWins, seasonLosses, rank = 1;
                        do {
                            Field *fields = result->Fetch();
                            teamId = fields[0].Get<uint32>();
                            name = fields[1].Get<std::string>();
                            rating = fields[2].Get<uint32>();
                            seasonWins = fields[3].Get<uint32>();
                            seasonLosses = fields[4].Get<uint32>();
                            
                            std::stringstream buffer;
                            buffer << rank << ". [|r|cffffff00 " << rating << "|cFF000000 ]|r |cff0000ff " << name; // Segunda Pagina
                            buffer << " |cFF000000(|cFF008B8B" << seasonWins << "|cFF000000-|cFF008B8B" << seasonLosses << "|cFF000000)";
                            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, ARENA_START_TEAM_LOOKUP + teamId);
                            
                            rank++;
                        } while(result->NextRow());
                        
                        SendGossipMenuFor(player, ARENA_GOSSIP_TEAMS, creature->GetGUID());
                    }
                    break;
                }
                default:
                {
                    if (uiAction > ARENA_START_TEAM_LOOKUP) 
                    {
                        uint32 teamId = uiAction - ARENA_START_TEAM_LOOKUP;
                        
                        // lookup team
                        QueryResult result = CharacterDatabase.Query("SELECT "
                            "`name`,"                 // 0
                            "`rating`,"  // 1
                            "`seasonWins`,"   // 2
                            "`seasonGames`"   // 3
                            " - " // 3
                            "`seasonWins`," // 3
                            "`weekWins`,"  // 4
                            "`weekGames`"   // 5
                            " - " // 5
                            "`weekWins`,"  // 5
                            "`rank`,"   // 6
                            "`captainGuid`,"   // 7
                            "`type` "   // 8
                            "FROM `arena_team` WHERE `arenaTeamId` = '{}'", teamId);

                        // no team found
                        if(!result) {
                            player->GetSession()->SendNotification("Arena team not found...");
                            player->PlayerTalkClass->SendCloseGossip();
                            return true;
                        }
                        
                        // populate the results
                        Field *fields = result->Fetch();
                        std::string name = fields[0].Get<std::string>();
                        uint32 rating = fields[1].Get<uint32>();
                        uint32 seasonWins = fields[2].Get<uint32>();
                        uint32 seasonLosses = fields[3].Get<uint32>();
                        uint32 weekWins = fields[4].Get<uint32>();
                        uint32 weekLosses = fields[5].Get<uint32>();
                        uint32 rank = fields[6].Get<uint32>();
                        uint32 captainGuid = fields[7].Get<uint32>();
                        uint32 type = fields[8].Get<uint32>();
                        uint32 parentOption = teamTypeToOption(type);
                        
                        std::string seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
                        std::string weekWinPercentage = getWinPercent(weekWins, weekLosses);
                        
                        std::stringstream buf;
                        buf << " Team:|r  |cff0000ff" << name << "\n |cFF000000Rating: |r|cffffff00" << rating << "   |cFF000000(Rank " << rank << " in " << type << "v" << type << ")"
                            "\n Week: |cFF008B8B " << weekWins << "|cFF000000-|cFF008B8B" << weekLosses << "|cFF000000 (" << weekWinPercentage << ") " "| Season: |cFF008B8B" << seasonWins << "|cFF000000-|cFF008B8B" << seasonLosses << " |cFF000000(" << seasonWinPercentage << ")" "\n ______________________________________";
                        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                        
                        QueryResult members = CharacterDatabase.Query( "SELECT  a.guid, a.personalRating, a.weekWins, a.weekGames - a.weekWins, a.seasonWins, a.seasonGames - a.seasonWins, c.name, c.race, c.class, c.level FROM arena_team_member a LEFT JOIN characters c ON c.guid = a.guid WHERE arenaTeamId = '{}' ORDER BY a.guid = '{}' DESC, a.seasonGames DESC, c.name ASC", teamId, captainGuid);
                        if(!members) {
                            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "No team members found...?", GOSSIP_SENDER_MAIN, parentOption);
                        } else {
                            uint32 memberPos = 1;
                            uint32 memberCount = members->GetRowCount();
                            uint32 guid, personalRating, level;
                            std::string name, race, Class;
                            
                            buf.str("");
                            buf << memberCount << " team " << ((memberCount == 1) ? "member" : " members") << " found:";
                            AddGossipItemFor(player, GOSSIP_ICON_CHAT, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                            
                            do {
                                // populate fields
                                fields = members->Fetch();
                                guid = fields[0].Get<uint32>();
                                personalRating = fields[1].Get<uint32>();
                                weekWins= fields[2].Get<uint32>();
                                weekLosses = fields[3].Get<uint32>();
                                seasonWins = fields[4].Get<uint32>();
                                seasonLosses = fields[5].Get<uint32>();
                                name = fields[6].Get<std::string>();
                                race = raceToString(fields[7].Get<uint8>());
                                Class = classToString(fields[8].Get<uint8>());
                                level = fields[9].Get<uint32>();
                                
                                seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
                                weekWinPercentage = getWinPercent(weekWins, weekLosses);
                                
                                // TODO: add output
                                // buf.str(""); // clear it
                                // buf << memberPos << ". "; 
                                // if (guid == captainGuid) 
                                //    buf <<  "Team Captain ";
                                // buf << name; //<< ", " << getPlayerStatus(guid); TO DO
                                
                                // AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                                buf.str("");
                                buf << "" << Class << "" << race << "|r|cff0000ff " << name << "|r|cffffff00 " << personalRating << " Personal Rating"
                                    "\n\n |cFF000000Week: |cFF008B8B" << weekWins << "|cFF000000-|cFF008B8B" << weekLosses << "|cFF000000 (" << weekWinPercentage << ")" " |cFF000000| Season: |cFF008B8B" << seasonWins << "|cFF000000-|cFF008B8B" << seasonLosses << " |cFF000000(" << seasonWinPercentage << ")";
                                AddGossipItemFor(player, GOSSIP_ICON_DOT, buf.str(), GOSSIP_SENDER_MAIN, parentOption);

                                memberPos++;
                            } while(members->NextRow());
                            
                        }
                        
                        buf.str("");
                        buf << "\nGo back to " << type << "v" << type << " ladder!";
                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                        SendGossipMenuFor(player, ARENA_GOSSIP_TEAM_LOOKUP, creature->GetGUID());
                    }
                }
            }
            
            return true;
        }
};

void AddTopArenaScripts()
{
    new Top_Arena_NPC();
}
