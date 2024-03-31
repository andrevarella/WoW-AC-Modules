//
// Created by romain-p on 17/10/2021.
//
#include "Player.h"
#include "Opcodes.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Chat.h"
#include <unordered_map>

#include "ArenaTeamMgr.h"
#include "ObjectMgr.h"


// AAAAAAA BEFORE
// A
// B
// C



std::vector<Opcodes> watchList =
{
        SMSG_NOTIFICATION,
        SMSG_AURA_UPDATE,
        SMSG_WORLD_STATE_UI_TIMER_UPDATE,
        SMSG_COMPRESSED_UPDATE_OBJECT,
        SMSG_AURA_UPDATE_ALL,
        SMSG_NAME_QUERY_RESPONSE,
        SMSG_DESTROY_OBJECT,
        MSG_MOVE_START_FORWARD,
        MSG_MOVE_SET_FACING,
        MSG_MOVE_HEARTBEAT,
        MSG_MOVE_JUMP,
        SMSG_MONSTER_MOVE,
        MSG_MOVE_FALL_LAND,
        SMSG_PERIODICAURALOG,
        SMSG_ARENA_UNIT_DESTROYED,
        MSG_MOVE_START_STRAFE_RIGHT,
        MSG_MOVE_STOP_STRAFE,
        MSG_MOVE_START_STRAFE_LEFT,
        MSG_MOVE_STOP,
        MSG_MOVE_START_BACKWARD,
        MSG_MOVE_START_TURN_LEFT,
        MSG_MOVE_STOP_TURN,
        MSG_MOVE_START_TURN_RIGHT,
        SMSG_SPELL_START,
        SMSG_SPELL_GO,
        SMSG_FORCE_RUN_SPEED_CHANGE,
        SMSG_ATTACKSTART,
        SMSG_POWER_UPDATE,
        SMSG_ATTACKERSTATEUPDATE,
        SMSG_SPELLDAMAGESHIELD,
        SMSG_SPELLHEALLOG,
        SMSG_SPELLENERGIZELOG,
        SMSG_SPELLNONMELEEDAMAGELOG,
        SMSG_ATTACKSTOP,
        SMSG_SPELLLOGEXECUTE,
        SMSG_EMOTE,
        SMSG_SPELL_DELAYED,
        SMSG_AI_REACTION,
        SMSG_PET_NAME_QUERY_RESPONSE,
        SMSG_CANCEL_AUTO_REPEAT,
        SMSG_UPDATE_OBJECT,
        SMSG_FORCE_FLIGHT_SPEED_CHANGE,
        SMSG_GAMEOBJECT_QUERY_RESPONSE,
        SMSG_FORCE_SWIM_SPEED_CHANGE,
        SMSG_GAMEOBJECT_DESPAWN_ANIM,
        SMSG_CANCEL_COMBAT
};


struct PacketRecord { uint32 timestamp; WorldPacket packet; };
struct MatchRecord { BattlegroundTypeId typeId; uint8 arenaTypeId; uint32 mapId; std::deque<PacketRecord> packets; };
std::unordered_map<uint32, MatchRecord> records;
std::unordered_map<uint64, MatchRecord> loadedReplays;



class BGReplayServerScript : public ServerScript {
public:
    BGReplayServerScript() : ServerScript("BGReplayServerScript") {}


    bool CanPacketSend(WorldSession* session, WorldPacket& packet) override
    {
        if (session == nullptr || session->GetPlayer() == nullptr) return true;

        Battleground* bg = session->GetPlayer()->GetBattleground();

        if (!bg) return true;

        uint32 replayId = bg->GetReplayID();

        //ignore packet when no bg or casual games
        if (replayId > 0) return true;

        //ignore packets until arena started
        if (bg->GetStatus() != BattlegroundStatus::STATUS_IN_PROGRESS) // is rated ?
            return true;

        //record packets from 1 player of each team
        //iterate just in case a player leaves and used as reference
        for (auto it : bg->GetPlayers())
        {
            if (it.second->GetBgTeamId() == session->GetPlayer()->GetBgTeamId())
            {
                if (it.second->GetGUID() != session->GetPlayer()->GetGUID())
                    return true;
                else break;
            }
        }


        //ignore packets not in watch list
        if (std::find(watchList.begin(), watchList.end(), packet.GetOpcode()) == watchList.end()) return true;





        if (records.find(bg->GetInstanceID()) == records.end())
            records[bg->GetInstanceID()].packets.clear();
        MatchRecord& record = records[bg->GetInstanceID()];

        /*
        if (bg != nullptr)
        {
            const auto& players = bg->GetPlayers();
            for (const auto& playerPair : players)
            {
                Player* player = playerPair.second;
                if (player != nullptr)
                {
                    // Obtém o ArenaType do Battleground
                    uint8 arenaType = bg->GetArenaType();

                    // Utiliza o ArenaType para obter o ArenaTeamId do jogador
                    uint32 arenaTeamId = player->GetArenaTeamId(arenaType);
                    // Agora você pode usar o "arenaTeamId" de acordo com suas necessidades
                }
            }
        }*/
        // Suponha que a variável arenaType já esteja disponível ou seja obtida de alguma forma
        /*
        uint8 arenaType = bg->GetArenaType(); // Exemplo de obtenção de arenaType a partir do objeto Battleground
        uint32 arenaTeamId = session->GetPlayer()->GetArenaTeamId(arenaType); // Utilize o arenaType para obter o ArenaTeamId
        record.arenateamID = arenaTeamId; // Agora você pode atribuir o arenaTeamId ao record.arenateamID
         // bg winner*/


        uint32 timestamp = bg->GetStartTime();
        record.typeId = bg->GetBgTypeID();
        record.arenaTypeId = bg->GetArenaType();
        record.mapId = bg->GetMapId();
        //push back packet inside queue of matchId 0
        record.packets.push_back({ timestamp, /* copy */ WorldPacket(packet) });
        return true;
    }
};

class BGReplayBGScript : public BGScript {
public:
    BGReplayBGScript() : BGScript("BGReplayBGScript") {}

    void OnBattlegroundUpdate(Battleground* bg, uint32 diff) override
    {
        uint32 replayId = bg->GetReplayID();
        if (replayId == 0) return;
        int32 startDelayTime = bg->GetStartDelayTime();
        if (startDelayTime > 3000)
        {
            bg->SetStartDelayTime(3000);
            bg->SetStartTime(bg->GetStartTime() + (startDelayTime - 3000));
        }
        if (bg->GetStatus() != BattlegroundStatus::STATUS_IN_PROGRESS) return;

        //retrieve arena replay data
        auto it = loadedReplays.find(replayId);
        if (it == loadedReplays.end()) return;
        MatchRecord& match = it->second;

        //if replay ends or spectator left > free arena replay data and/or kick player
        if (match.packets.empty() || bg->GetPlayers().empty()) {
            loadedReplays.erase(it);

            if (!bg->GetPlayers().empty())
                bg->GetPlayers().begin()->second->LeaveBattleground(bg);
            return;
        }

        //send replay data to spectator
        while (!match.packets.empty() && match.packets.front().timestamp <= bg->GetStartTime())
        {
            if (bg->GetPlayers().empty())
                break;
            bg->GetPlayers().begin()->second->GetSession()->SendPacket(&match.packets.front().packet);
            match.packets.pop_front();
        }
    }


    void OnBattlegroundEnd(Battleground* bg, TeamId winnerTeamId) override
    {
        uint32 replayId = bg->GetReplayID();
        //save replay when a bg ends
        if (replayId <= 0)
        {
            saveReplay(bg);
            return;
        }
    }

    void saveReplay(Battleground* bg)
    {
        /*
        if (!bg->isRated())
            return;*/

        //retrieve replay data
        auto it = records.find(bg->GetInstanceID());
        if (it == records.end()) return;
        MatchRecord& match = it->second;

        /** serialize arena replay data **/
        ByteBuffer buffer;
        uint32 headerSize;
        uint32 timestamp;
        for (auto it : match.packets) {
            headerSize = it.packet.size(); //header 4Bytes packet size
            timestamp = it.timestamp;

            buffer << headerSize; //4 bytes
            buffer << timestamp; //4 bytes
            buffer << it.packet.GetOpcode(); // 2 bytes
            if (headerSize > 0)
                buffer.append(it.packet.contents(), it.packet.size()); // headerSize bytes
        }
        /********************************/


        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BG_REPLAYS);
        stmt->SetData<uint32>(0, uint32(match.arenaTypeId));
        stmt->SetData<uint32>(1, uint32(match.typeId));
        stmt->SetData<uint32>(2, buffer.size());
        stmt->SetBinary(3, buffer.contentsAsVector());
        stmt->SetData<uint32>(4, bg->GetMapId());
        CharacterDatabase.Execute(stmt);

        records.erase(it);
    }

};

class ReplayGossip : public CreatureScript {
public:

    ReplayGossip() : CreatureScript("ReplayGossip") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay 2v2 Matches", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay 3v3 Matches", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay 5v5 Matches", GOSSIP_SENDER_MAIN, 3);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT,   "Replay a Match ID",  GOSSIP_SENDER_MAIN, 0, "Enter the Match ID", 0, true);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Favorites Matches", GOSSIP_SENDER_MAIN, 4);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case 1: // Replay 2v2 Matches
            player->PlayerTalkClass->SendCloseGossip();
            ShowLastReplays2v2(player, creature);
            break;
        case 2: // Replay 3v3 Matches
            player->PlayerTalkClass->SendCloseGossip();
            ShowLastReplays3v3(player, creature);
            break;
        case 3: // Replay 5v5 Matches
            player->PlayerTalkClass->SendCloseGossip();
            ShowLastReplays5v5(player, creature);
            break;
        case 4: // Saved Replays
            player->PlayerTalkClass->SendCloseGossip();
            ShowSavedReplays(player, creature);
            break;
        case GOSSIP_ACTION_INFO_DEF: // "Back"
            OnGossipHello(player, creature); // goes back the main menu
            break;

        default:
            //if (action >= GOSSIP_ACTION_INFO_DEF + 10 && action < GOSSIP_ACTION_INFO_DEF + 99999999) // Replay selected arena
            if (action >= GOSSIP_ACTION_INFO_DEF + 10) // Replay selected arena
            {
                return replayArenaMatch(player, action - (GOSSIP_ACTION_INFO_DEF + 10));  // Replays arenas id between 10 and 99999999999
                break;
            }
            /*
            if (action >= GOSSIP_ACTION_INFO_DEF + 99999999) // Replay bookmarked matches
            {
                uint32 replayId = action - (GOSSIP_ACTION_INFO_DEF + 99999999);       // Saved Replays are ids > 99999999999
                replayArenaMatch(player, replayId);
                break;
            */
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
    {
        if (action == 0) // "Replay a Match ID"
        {
            if (!code)
            {
                return false;
            }
            uint32 replayId;
            try
            {
                replayId = std::stoi(code);
            }
            catch (...)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Invalid Match ID.");
                return false;
            }
            CloseGossipMenuFor(player);
            return replayArenaMatch(player, replayId);
        }
        else if (action == 5) // "Add a Favorite Match"
        {
            if (!code)
                return false;
            CloseGossipMenuFor(player);
            try
            {
                uint32 NumeroDigitado = std::stoi(code);
                BookmarkMatch(player->GetGUID().GetCounter(), NumeroDigitado);
                return true;
            }
            catch (...)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("ID do replay inválido.");
                return false;
            }
        }
        return false;
    }

private:

    void ShowSavedReplays(Player* player, Creature* creature, bool firstPage = true)
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Bookmark a Match ID", GOSSIP_SENDER_MAIN, 5, "Enter the Match ID", 0, true);

        std::string sortOrder = (firstPage) ? "ASC" : "DESC";
        QueryResult result = CharacterDatabase.Query("SELECT id FROM character_bg_replays WHERE savedBy LIKE '%" + std::to_string(player->GetGUID().GetCounter()) + "%' ORDER BY id " + sortOrder + " LIMIT 29");
        if (!result)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "No saved replays found.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            //const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 99999999; // valor adicionado em OnGossipSelect (99999999 + id do match)
            const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 10; // valor adicionado em OnGossipSelect (10 + id do match)
            do
            {
                Field* fields = result->Fetch();
                if (!fields)
                    break;

                uint32 matchId = fields[0].Get<uint32>();
                AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, actionOffset + matchId);
            } while (result->NextRow());
        }

        if (firstPage)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Next Page", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
    void ShowSavedReplaysPage2(Player* player, Creature* creature)
    {
        QueryResult result = CharacterDatabase.Query("SELECT id FROM character_bg_replays WHERE savedBy LIKE '%" + std::to_string(player->GetGUID().GetCounter()) + "%' ORDER BY id DESC LIMIT 30, 30");
        if (!result)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "No saved replays found.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            //const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 99999999;
            const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 10;
            do
            {
                Field* fields = result->Fetch();
                if (!fields)
                    break;

                uint32 matchId = fields[0].Get<uint32>();
                AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, actionOffset + matchId);
            } while (result->NextRow());
        }

        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Previous Page", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowLastReplays2v2(Player* player, Creature* creature)
    {
        auto matchIds = loadLast10Replays2v2();

        const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 10;

        if (matchIds.empty())
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "No replays found for 2v2.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            for (uint32 matchId : matchIds)
            {
                AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, actionOffset + matchId);
            }
        }
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
    void ShowLastReplays3v3(Player* player, Creature* creature)
    {
        auto matchIds = loadLast10Replays3v3();

        const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 10;

        if (matchIds.empty())
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "No replays found for 3v3.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            for (uint32 matchId : matchIds)
            {
                AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, actionOffset + matchId);
            }
        }
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
    void ShowLastReplays5v5(Player* player, Creature* creature)
    {
        auto matchIds = loadLast10Replays5v5();

        const uint32 actionOffset = GOSSIP_ACTION_INFO_DEF + 10;

        if (matchIds.empty())
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "No replays found for 5v5.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            for (uint32 matchId : matchIds)
            {
                AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match " + std::to_string(matchId), GOSSIP_SENDER_MAIN, actionOffset + matchId);
            }
        }
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    std::vector<uint32> loadLast10Replays2v2()
    {
        std::vector<uint32> records;
        QueryResult result = CharacterDatabase.Query("SELECT id FROM character_bg_replays WHERE arenaTypeId = 2 ORDER BY id DESC LIMIT 10");
        if (!result)
            return records;

        do {
            Field* fields = result->Fetch();
            if (!fields)
                return records;

            uint32 matchId = fields[0].Get<uint32>();
            records.push_back(matchId);
        } while (result->NextRow());

        return records;
    }
    std::vector<uint32> loadLast10Replays3v3()
    {
        std::vector<uint32> records;
        QueryResult result = CharacterDatabase.Query("SELECT id FROM character_bg_replays WHERE arenaTypeId = 3 ORDER BY id DESC LIMIT 10");
        if (!result)
            return records;

        do {
            Field* fields = result->Fetch();
            if (!fields)
                return records;

            uint32 matchId = fields[0].Get<uint32>();
            records.push_back(matchId);
        } while (result->NextRow());

        return records;
    }
    std::vector<uint32> loadLast10Replays5v5()
    {
        std::vector<uint32> records;
        QueryResult result = CharacterDatabase.Query("SELECT id FROM character_bg_replays WHERE arenaTypeId = 5 ORDER BY id DESC LIMIT 10");
        if (!result)
            return records;

        do {
            Field* fields = result->Fetch();
            if (!fields)
                return records;

            uint32 matchId = fields[0].Get<uint32>();
            records.push_back(matchId);
        } while (result->NextRow());

        return records;
    }

    void BookmarkMatch(uint64 playerGuid, uint32 code)
    {
        std::string query = "UPDATE character_bg_replays SET savedBy = CONCAT(savedBy, '  " + std::to_string(playerGuid) + "') WHERE id = " + std::to_string(code);
        CharacterDatabase.Execute(query.c_str());
    }

    bool replayArenaMatch(Player* player, uint32 replayId)
    {
        auto handler = ChatHandler(player->GetSession());

        if (!loadReplayDataForPlayer(player, replayId))
            return false;

        MatchRecord record = loadedReplays[player->GetGUID().GetCounter()];

        Battleground* bg = sBattlegroundMgr->CreateNewBattleground(record.typeId, GetBattlegroundBracketByLevel(record.mapId, sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)), record.arenaTypeId, false);
        if (!bg) {
            handler.PSendSysMessage("Couldn't create arena map!");
            handler.SetSentErrorMessage(true);
            return false;
        }

        bg->SetReplayID(player->GetGUID().GetCounter());
        player->SetPendingSpectatorForBG(bg->GetInstanceID());
        bg->StartBattleground();

        BattlegroundTypeId bgTypeId = bg->GetBgTypeID();

        TeamId teamId = Player::TeamIdForRace(player->getRace());

        uint32 queueSlot = 0;
        WorldPacket data;

        player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId, queueSlot, true, false, TEAM_NEUTRAL);
        player->SetEntryPoint();
        sBattlegroundMgr->SendToBattleground(player, bg->GetInstanceID(), bgTypeId);
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_IN_PROGRESS, 0, bg->GetStartTime(), bg->GetArenaType(), teamId);
        player->GetSession()->SendPacket(&data);
        handler.PSendSysMessage("Replay begins.");
        return true;
    }

    bool loadReplayDataForPlayer(Player* p, uint32 matchId) {
        QueryResult result = CharacterDatabase.Query("SELECT id, arenaTypeId, typeId, contentSize, contents, mapId FROM character_bg_replays where id =  {}", matchId);
        if (!result) {
            ChatHandler(p->GetSession()).PSendSysMessage("Replay data not found.");
            return false;
        }

        Field* fields = result->Fetch();
        if (!fields) {
            ChatHandler(p->GetSession()).PSendSysMessage("Replay data not found.");
            return false;
        }
        MatchRecord record;
        deserializeMatchData(record, fields);

        loadedReplays[p->GetGUID().GetCounter()] = std::move(record);
        return true;
    }

    void deserializeMatchData(MatchRecord& record, Field* fields) {
        record.arenaTypeId = uint8(fields[1].Get<uint32>());
        record.typeId = BattlegroundTypeId(fields[2].Get<uint32>());
        int size = uint32(fields[3].Get<uint32>());
        std::vector<uint8> data = fields[4].Get<Binary>();
        record.mapId = uint32(fields[5].Get<uint32>());
        ByteBuffer buffer;
        buffer.append(&data[0], data.size());

        /** deserialize replay binary data **/
        uint32 packetSize;
        uint32 packetTimestamp;
        uint16 opcode;
        while (buffer.rpos() <= buffer.size() - 1) {
            buffer >> packetSize;
            buffer >> packetTimestamp;
            buffer >> opcode;

            WorldPacket packet(opcode, packetSize);

            if (packetSize > 0) {
                std::vector<uint8> tmp(packetSize, 0);
                buffer.read(&tmp[0], packetSize);
                packet.append(&tmp[0], packetSize);
            }

            record.packets.push_back({ packetTimestamp, packet });
        }
    }
};

void AddBGReplayScripts()
{

    new BGReplayServerScript();
    new BGReplayBGScript();
    new ReplayGossip();

}
