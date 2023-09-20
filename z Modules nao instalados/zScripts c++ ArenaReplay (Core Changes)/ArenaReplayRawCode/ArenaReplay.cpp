//
// Created by romain-p on 17/10/2021.
//
#include "Player.h"
#include "Opcodes.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ScriptMgr.h"
#include <unordered_map>


std::vector<Opcodes> watchList = {
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


struct PacketRecord {uint32 timestamp; WorldPacket packet;};
struct MatchRecord {BattlegroundTypeId typeId; uint8 arenaTypeId; std::deque<PacketRecord> packets;};
std::unordered_map<uint32, MatchRecord> records;
std::unordered_map<uint64, MatchRecord> loadedReplays;

class ArenaReplayServerScript: public ServerScript {
public:
    ArenaReplayServerScript() : ServerScript("ArenaReplayServerScript") {}


    void OnPacketSend(WorldSession *session, WorldPacket &packet) override {
        if (session == nullptr || session->GetPlayer() == nullptr) return;

        Battleground *bg = session->GetPlayer()->GetBattleground();

        //ignore packet when no bg or casual games
        if (bg == nullptr || !bg->isArena() || !bg->isRated() || bg->isReplay()) return;
        //ignore packets until arena started
        if (bg->GetStatus() != BattlegroundStatus::STATUS_IN_PROGRESS) return;
        //record packets from 1 player of each team
        //iterate just in case a player leaves and used as reference
        for (auto it: bg->GetPlayers()) {
            if (it.second->GetBgTeamId() == session->GetPlayer()->GetBgTeamId()) {
                if (it.second->GetGUID() != session->GetPlayer()->GetGUID())
                    return; else break;
            }
        }
        //ignore packets not in watch list
        if (std::find(watchList.begin(), watchList.end(), packet.GetOpcode()) == watchList.end()) return;

        if (records.find(bg->GetInstanceID()) == records.end())
            records[bg->GetInstanceID()].packets.clear();
        MatchRecord &record = records[bg->GetInstanceID()];

        uint32 timestamp = bg->getAccurateTimeElapsed();
        record.typeId = bg->GetBgTypeID();
        record.arenaTypeId = bg->GetArenaType();

        //push back packet inside queue of matchId 0
        record.packets.push_back({timestamp, /* copy */ WorldPacket(packet)});
    }
};

class ArenaReplayBGScript: public BGScript {
public:
    ArenaReplayBGScript() : BGScript("ArenaReplayBGScript") {}

    void OnBattlegroundUpdate(Battleground *bg, uint32 diff) override {
        //save replay when a ranked arena is terminated
        if (!bg->isReplay() && bg->GetStatus() == BattlegroundStatus::STATUS_WAIT_LEAVE) {
            saveReplay(bg);
            return;
        }

        if (!bg->isReplay() || bg->GetStatus() != BattlegroundStatus::STATUS_IN_PROGRESS) return;

        //retrieve arena replay data
        auto it = loadedReplays.find(bg->replayId());
        if (it == loadedReplays.end()) return;
        MatchRecord &match = it->second;

        //if replay ends or spectator left > free arena replay data and/or kick player
        if (match.packets.empty() || bg->GetPlayers().empty()) {
            loadedReplays.erase(it);

            if (!bg->GetPlayers().empty())
                bg->GetPlayers().begin()->second->LeaveBattleground(bg);
            return;
        }

        //send replay data to spectator
        while(!match.packets.empty() && match.packets.front().timestamp <= bg->getAccurateTimeElapsed()) {
            if (bg->GetPlayers().empty())
                break;
            bg->GetPlayers().begin()->second->GetSession()->SendPacket(&match.packets.front().packet);
            match.packets.pop_front();
        }
    }

    void saveReplay(Battleground *bg) {
        uint32 id = bg->fightId();
        //wait until arena end
        if (id <= 0) return;

        //retrieve replay data
        auto it = records.find(bg->GetInstanceID());
        if (it == records.end()) return;
        MatchRecord &match = it->second;

        /** serialize arena replay data **/
        ByteBuffer buffer;
        uint32 headerSize;
        uint32 timestamp;
        for (auto it: match.packets) {
            headerSize = it.packet.size(); //header 4Bytes packet size
            timestamp = it.timestamp;

            buffer << headerSize; //4 bytes
            buffer << timestamp; //4 bytes
            buffer << it.packet.GetOpcode(); // 2 bytes
            if (headerSize > 0)
                buffer.append(it.packet.contents(), it.packet.size()); // headerSize bytes
        }
        /********************************/

        SQLTransaction tx = CharacterDatabase.BeginTransaction();
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ARENA_REPLAYS);
        stmt->setUInt32(0, id);
        stmt->setUInt32(1, uint32(match.arenaTypeId));
        stmt->setUInt32(2, uint32(match.typeId));
        stmt->setUInt32(3, buffer.size());
        stmt->setBinary(4, buffer.contentAsVector());
        tx->Append(stmt);
        CharacterDatabase.CommitTransaction(tx);

        //prevents duplicates
        bg->setFightId(0);
        records.erase(it);
    }
};

class ReplayGossip : public CreatureScript{
public:

    ReplayGossip() : CreatureScript("ReplayGossip") { }

    bool OnGossipHello(Player *player, Creature *creature) override {
        auto matchIds = loadLast10Replays();
        for (uint32 matchId: matchIds)
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Replay match "+std::to_string(matchId), GOSSIP_SENDER_MAIN, matchId);
        SendGossipMenuFor(player, 1775757, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action) override {
        player->PlayerTalkClass->ClearMenus();

        replayArenaMatch(player, action);
        CloseGossipMenuFor(player);
        return true;
    }
private:
    bool replayArenaMatch(Player *player, uint32 replayId) {
        auto handler = ChatHandler(player->GetSession());

        if (!loadReplayDataForPlayer(player, replayId))
            return false;

        MatchRecord record = loadedReplays[player->GetGUID()];

        Battleground* bg = sBattlegroundMgr->CreateNewBattleground(record.typeId, 80, 80, ArenaType(2), false);
        if (!bg) {
            handler.PSendSysMessage("Couldn't create arena map!");
            handler.SetSentErrorMessage(true);
            return false;
        }

        bg->toggleReplayArena(player->GetGUID());
        bg->StartBattleground();

        BattlegroundTypeId bgTypeId = bg->GetBgTypeID();

        TeamId teamId = Player::TeamIdForRace(player->getRace());
        player->SetEntryPoint();

        uint32 queueSlot = 0;
        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_IN_PROGRESS, 0, bg->GetStartTime(), bg->GetArenaType(), teamId);
        player->GetSession()->SendPacket(&data);

        player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId, queueSlot, true, false, teamId);
        sBattlegroundMgr->SendToBattleground(player, bg->GetInstanceID(), bgTypeId);

        handler.PSendSysMessage("Replay begins.");
        return true;
    }
    
    bool loadReplayDataForPlayer(Player *p, uint32 matchId) {
        PreparedStatement *stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ARENA_REPLAYS);
        stmt->setUInt32(0, matchId);

        PreparedQueryResult result = CharacterDatabase.Query(stmt);
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

        loadedReplays[p->GetGUID()] = std::move(record);
        return true;
    }

    std::vector<uint32> loadLast10Replays() {
        std::vector<uint32> records;
        PreparedStatement *stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_LAST_10_ARENA_REPLAYS);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);
        if (!result)
            return records;

        do {
            Field* fields = result->Fetch();
            if (!fields)
                return records;

            uint32 matchId = fields[0].GetUInt32();
            records.push_back(matchId);
        } while (result->NextRow());

        return records;
    }

    void deserializeMatchData(MatchRecord &record, Field *fields) {
        record.arenaTypeId = uint8(fields[1].GetUInt32());
        record.typeId = BattlegroundTypeId(fields[2].GetUInt32());


        std::vector<uint8> data = fields[4].GetBinary();
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

            record.packets.push_back({packetTimestamp, packet});
        }
    }
};

void addArenaReplayScript() {
    new ArenaReplayServerScript();
    new ArenaReplayBGScript();
    new ReplayGossip();
}