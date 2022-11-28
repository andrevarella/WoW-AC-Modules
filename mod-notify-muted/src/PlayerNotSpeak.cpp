#include "Config.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "Player.h"

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

void AddPlayerNotSpeakScripts()
{
	new KargatumSC_PlayerNotSpeak();
}
