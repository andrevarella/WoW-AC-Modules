#include "Config.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "AccountMgr.h"
#include "Channel.h"

enum PlayerNotSpeak_Locale
{
    RECEIVER_NOT_SPEAK = 13000
};

class KargatumSC_PlayerNotSpeak : public PlayerScript
{
public:
	KargatumSC_PlayerNotSpeak() : PlayerScript("KargatumSC_PlayerNotSpeak") {}

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& /*msg*/, Player* receiver) override
	{
        if (!sConfigMgr->GetOption<bool>("PlayerNotSpeak.Enable", false))
            return;

        if (receiver->CanSpeak())
            return;

        uint64 MuteTime = receiver->GetSession()->m_muteTime;

        if (MuteTime == 0)
            return;

        std::string MuteTimeStr = secsToTimeString(MuteTime - time(NULL), true);
        std::string NameLink = ChatHandler(receiver->GetSession()).playerLink(receiver->GetName());

        ChatHandler(player->GetSession()).PSendSysMessage(RECEIVER_NOT_SPEAK, NameLink.c_str(), MuteTimeStr.c_str());
	}
};

/*
class Faction_Icon_Chat : public PlayerScript
{
public:
    Faction_Icon_Chat() : PlayerScript("Faction_Icon_Chat") {}

    void OnChat(Player* player, uint32 /*type*, uint32, std::string& msg, Channel* channel) override
    {
        if (!sConfigMgr->GetOption<bool>("CHANNEL_ICON_FACTION_ENABLE", true))
            return;

        if (!player || !channel)
            return;

        if (!sConfigMgr->GetOption<bool>("CHANNEL_ICON_FACTION_ONLYLFG", true) && !channel->IsLFG(), true)
            return;

        if (!!sConfigMgr->GetOption<bool>("CHANNEL_ICON_FACTION_GM_ENABLE", true) && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            return;

        std::string IconHorge = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18:-3:-3|t";
        std::string IconAlliance = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18:-3:-3|t";

        std::stringstream ssMsg;
        ssMsg << ((player->GetTeamId() == TEAM_HORDE) ? IconHorge : IconAlliance) << msg;
        msg = ssMsg.str();
    }
}; */

void AddPlayerNotSpeakScripts()
{
    new KargatumSC_PlayerNotSpeak();
    //new Faction_Icon_Chat;
}
