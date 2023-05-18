#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Chat.h"
#include "BattlegroundMgr.h"
#include "DisableMgr.h"
#include "Common.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "SocialMgr.h"
#include "Player.h"
#include "Battleground.h"
#include "Define.h"
#include "ScriptedCreature.h"
#include "ItemTemplate.h"
#include "Unit.h"
#include "Player.h"
#include "LuaEngine.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"
#include "GlobalMethods.h"
#include "ObjectMethods.h"
#include "WorldObjectMethods.h"
#include "UnitMethods.h"
#include "PlayerMethods.h"
#include "CreatureMethods.h"
#include "GroupMethods.h"
#include "GuildMethods.h"
#include "GameObjectMethods.h"
#include "ElunaQueryMethods.h"
#include "AuraMethods.h"
#include "ItemMethods.h"
#include "WorldPacketMethods.h"
#include "SpellMethods.h"
#include "QuestMethods.h"
#include "MapMethods.h"
#include "CorpseMethods.h"
#include "VehicleMethods.h"
#include "BattleGroundMethods.h"
#include "ChatHandlerMethods.h"
#include "AchievementMethods.h"
#include "Log.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
//#define sBattlegroundMgr ACE_Singleton<BattlegroundMgr, ACE_Null_Mutex>::instance()

class challenge_commands : public CommandScript
{
public:
    challenge_commands() : CommandScript("challenge_commands") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "challenge", SEC_PLAYER, true, &HandleChallengeCommand, "" },
        };
        return commandTable;
    }

    static bool HandleChallengeCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        Player* target = NULL;
        Battleground* arena = NULL;

        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        if (!player->IsAlive() || !target->IsAlive())
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060You or your Target is dead.|r");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // lordlook crash protection
        if (target->IsBeingTeleported() || target->IsBeingTeleportedFar() || target->IsBeingTeleportedNear() || target->isBeingLoaded())
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060Your target is still loading. Try again in abit.|r");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Players in queue are unable to challenge
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA); // All Arenas
        BattlegroundTypeId bgTypeId = bg->GetBgTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, bg->GetArenaType());

        if (player->GetBattlegroundQueueIndex(bgQueueTypeId))
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060You're already in a queue. Please remove any Battleground or Arena queues.|r");
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        // Players are unable to challenge queued players
        if (target->GetBattlegroundQueueIndex(bgQueueTypeId))
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060The player is in an Battleground or Arena queue.|r");
            //handler->PSendSysMessage("The player is already in queue.", target->GetName());
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        // Cannot challenge someone when fighting
        if (player->IsInCombat())
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060You are in combat.|r");
            ClearGossipMenuFor(player);
            return false;
        }

        // Dont let challenge players in combat
        if (handler->extractPlayerTarget((char*)args, &target) && target->IsInCombat())
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060Target is in combat.|r");
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        // Cannot challenge players which have ignored you
        if (handler->extractPlayerTarget((char*)args, &target) && target->GetSocial()->HasIgnore(player->GetGUID()))
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060The player %s is ignoring you, you cannot challenge players that are ignoring you.|r", target->GetName().c_str());
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        if (target == player)
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060You cannot challenge yourself.|r");
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        if (!(arena = JoinPlayer(target, arena)))
        {
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060The player you have challenged is either in a Battleground or Arena queue or in a Battleground or Arena.|r");
            handler->SetSentErrorMessage(true);
            ClearGossipMenuFor(player);
            return false;
        }

        if (!JoinPlayer(player, arena))
        {
            if (target->challengeData)
            {
                target->challengeData->removeEvent->Execute(0, 0);
                delete (target->challengeData->removeEvent);
                delete target->challengeData;
                target->challengeData = NULL;
            }

            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n |cFF9370DB[Challenge System]|r");
            player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n \n \n \n |cffff6060The player is in a Battleground or Arena.|r");
            handler->SetSentErrorMessage(true);
            return false;
        }

        player->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n You want to challenge %s to arena.", target->GetName().c_str());
        target->GetSession()->SendAreaTriggerMessage("\n \n \n \n \n \n Player %s wants to challenge you to arena.", player->GetName().c_str());
        ChatHandler(target->GetSession()).PSendSysMessage("If you do not wish to be challenged by %s add him to your ignore list. And the player will no longer be able to challenge you.", player->GetName().c_str());
        return true;
    }

    /*
    static bool HandleChallengePlayersCommand(ChatHandler* handler, const char* args)
    {
    Player *targetOne = NULL;
    Player *targetTwo = NULL;

    char *targetOne_name_str = strtok((char*)args, " ");
    char *targetTwo_name_str = strtok((char*)args, " ");

    if (!targetOne_name_str || !targetTwo_name_str)
    return false;

    std::string targetOne_name = targetOne_name_str;
    std::string targetTwo_name = targetOne_name_str;

    if (!normalizePlayerName(targetOne_name) && !normalizePlayerName(targetTwo_name))
    return false;

    Battleground *arena = NULL;

    if (!handler->extractPlayerTarget((char*)args, &targetOne) || !handler->extractPlayerTarget((char*)args, &targetTwo))
    {
    handler->PSendSysMessage("Can't find player.");
    handler->SetSentErrorMessage(true);
    return false;
    }

    if (targetOne_name == targetTwo_name)
    {
    handler->PSendSysMessage("Your opponents should be a different players.");
    handler->SetSentErrorMessage(true);
    return false;
    }

    if (!targetOne || !targetTwo)
    {
    handler->PSendSysMessage("Can't find some of the players.");
    handler->SetSentErrorMessage(true);
    return false;
    }

    if (!(arena = JoinPlayer(targetOne, arena)))
    {
    handler->PSendSysMessage("Cant join to challenge queue when already queued.");
    handler->SetSentErrorMessage(true);
    return false;
    }

    if (!JoinPlayer(targetTwo, arena))
    {
    if (targetOne->challengeData)
    {
    targetOne->challengeData->removeEvent->Execute(0, 0);
    delete (targetOne->challengeData->removeEvent);
    delete targetOne->challengeData;
    targetOne->challengeData = NULL;
    }

    handler->PSendSysMessage("Cant join to challenge queue when players already queued.");
    handler->SetSentErrorMessage(true);
    return false;
    }

    return true;
    }*/

private:
    static Battleground* JoinPlayer(Player* player, Battleground* arena)
    {
        // ignore if we already in BG or BG queue
        if (player->InBattleground())
            return NULL;

        uint8 arenatype = ARENA_TYPE_2v2;
        uint32 matchmakerRating = 0;

        //check existance
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
        if (!bg)
        {
            //sLog->outError("Battleground: template bg (all arenas) not found");
            return NULL;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
        {
            //ChatHandler(player).PSendSysMessage(LANG_ARENA_DISABLED);
            return NULL;
        }

        BattlegroundTypeId bgTypeId = bg->GetBgTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
        if (!bracketEntry)
            return NULL;

        // check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            //player is already in this queue
            return NULL;
        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
            return NULL;

        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId, false);

        WorldPacket data;
        // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, 0, 0, arenatype, 0);
        player->GetSession()->SendPacket(&data);
        // TC_LOG_INFO("server loading", "Battleground: player joined queue for arena, skirmish, bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, player->GetGUIDLow(), player->GetName());

        sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        GroupQueueInfo* ginfo = new GroupQueueInfo;
        ginfo->BgTypeId = bgTypeId;
        ginfo->ArenaType = arenatype;
        ginfo->ArenaTeamId = 0;
        ginfo->IsRated = 0;
        ginfo->IsInvitedToBGInstanceGUID = 0;
        ginfo->JoinTime = getMSTime();
        ginfo->RemoveInviteTime = 0;
        ginfo->Team = player->GetTeamId();
        ginfo->ArenaTeamRating = 0;
        ginfo->ArenaMatchmakerRating = 0;
        ginfo->OpponentsTeamRating = 0;
        ginfo->OpponentsMatchmakerRating = 0;
        ginfo->Players.clear();

        GroupQueueInfo* info = new GroupQueueInfo;
        info->GroupInfo = ginfo;
        info->LastOnlineTime = getMSTime();

        ginfo->Players[player->GetGUID()] = info;

        if (!arena)
        {
            arena = sBattlegroundMgr->CreateNewBattleground(bgTypeId, bracketEntry, arenatype, true);
            arena->SetRated(false);
            arena->SetChallenge(true);

            player->challengeData = new ChallengeData;
            player->challengeData->bg = arena;
            player->challengeData->ginfo = ginfo;

            InviteGroupToBG(ginfo, arena, ALLIANCE);
        }
        else
        {
            player->challengeData = new ChallengeData;
            player->challengeData->bg = arena;
            player->challengeData->ginfo = ginfo;

            InviteGroupToBG(ginfo, arena, HORDE);
            arena->StartBattleground();
            if (!sBattlegroundMgr->HasBattleground(arena))
                sBattlegroundMgr->AddBattleground(ginfo->IsInvitedToBGInstanceGUID, bgTypeId, arena);
            //sBattlegroundMgr->AddBattleground->ginfo->IsInvitedToBGInstanceGUID, bgTypeId, arena;
        }

        return arena;
    }

    static bool InviteGroupToBG(GroupQueueInfo* ginfo, Battleground* bg, uint32 side)
    {
        // set side if needed
        if (side)
            ginfo->Team = side;

        if (!ginfo->IsInvitedToBGInstanceGUID)
        {
            // not yet invited
            // set invitation
            ginfo->IsInvitedToBGInstanceGUID = bg->GetInstanceID();
            BattlegroundTypeId bgTypeId = bg->GetBgTypeID();
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, bg->GetArenaType());

            // set ArenaTeamId for rated matches
            if (bg->isArena() && bg->isRated())
                bg->SetArenaTeamIdForTeam(ginfo->Team, ginfo->ArenaTeamId);

            ginfo->RemoveInviteTime = getMSTime() + INVITE_ACCEPT_WAIT_TIME;

            // loop through the players
            for (std::map<uint64, GroupQueueInfo*>::iterator itr = ginfo->Players.begin(); itr != ginfo->Players.end(); ++itr)
            {
                // get the player
                Player* player = ObjectAccessor::FindPlayer(itr->first);
                // if offline, skip him, this should not happen - player is removed from queue when he logs out
                if (!player)
                    continue;

                // set invited player counters
                bg->IncreaseInvitedCount(ginfo->Team);

                player->SetInviteForBattlegroundQueueType(bgQueueTypeId, ginfo->IsInvitedToBGInstanceGUID);

                BGQueueRemoveEvent* removeEvent = new BGQueueRemoveEvent(player->GetGUID(), ginfo->IsInvitedToBGInstanceGUID, bgTypeId, bgQueueTypeId, ginfo->RemoveInviteTime);
                player->challengeData->removeEvent = removeEvent;

                WorldPacket data;

                uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

                //TC_LOG_INFO("server loading", "Battleground: invited player %s (%u) to BG instance %u queueindex %u bgtype %u, I can't help it if they don't press the enter battle button.", player->GetName(), player->GetGUIDLow(), bg->GetInstanceID(), queueSlot, bg->GetTypeID());

                // send status packet
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_JOIN, INVITE_ACCEPT_WAIT_TIME, 0, ginfo->ArenaType, 0);
                player->GetSession()->SendPacket(&data);
            }
            return true;
        }

        return false;
    }

};

void AddChallengeArenaScripts()
{
    new challenge_commands();
}

