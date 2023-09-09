#include "Configuration/Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include <time.h>

using namespace Acore::ChatCommands;

enum WeekendXP
{
    SETTING_WEEKEND_XP_RATE = 0,
    SETTING_WEEKEND_XP_DISABLE = 1,

    LANG_CMD_WEEKEND_XP_SET   = 11120,
    LANG_CMD_WEEKEND_XP_ERROR = 11121,

    WD_FRIDAY   = 5,
    WD_SATURDAY = 6,
    WD_SUNDAY   = 0,
};

class weekendxp_commandscript : public CommandScript
{
public:
    weekendxp_commandscript() : CommandScript("weekendxp_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "weekendxp rate", HandleSetXPBonusCommand, SEC_PLAYER, Console::No },
        };

        return commandTable;
    }

    static bool HandleSetXPBonusCommand(ChatHandler* handler, int8 rate)
    {
        Player* player = handler->GetPlayer();

        int8 maxRate = sConfigMgr->GetOption<int8>("XPWeekend.MaxAllowedRate", 2);

        if (rate <= 0 || rate > maxRate)
        {
            handler->PSendSysMessage(LANG_CMD_WEEKEND_XP_ERROR, maxRate);
            handler->SetSentErrorMessage(true);
            return true;
        }

        player->UpdatePlayerSetting("mod-double-xp-weekend", SETTING_WEEKEND_XP_RATE, rate);
        handler->PSendSysMessage(LANG_CMD_WEEKEND_XP_SET, rate);

        return true;
    }
};

class DoubleXpWeekend : public PlayerScript
{
public:
    DoubleXpWeekend() : PlayerScript("DoubleXpWeekend") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("XPWeekend.Announce", false))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("This server is running the |cff4CFF00Double XP Weekend |rmodule.");
        }
    }
           

    void OnGiveXP(Player* player, uint32& amount, Unit* victim, uint8 /*xpSource*/) override
    {
        if (!IsEventActive())
        {
            return;
        }

        if (sConfigMgr->GetOption<bool>("XPWeekend.QuestOnly", false) && victim && victim->GetTypeId() == TYPEID_UNIT && !victim->ToCreature()->hasLootRecipient())
        {
            return;
        }

        if (player->getLevel() >= sConfigMgr->GetOption<uint32>("XPWeekend.MaxLevel", 80))
        {
            return;
        }

        amount *= GetExperienceRate(player);

        if (player->InBattleground())
        {
            if (player->getLevel() <= 9)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.1-9", 1);
            else if (player->getLevel() <= 19)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.10-19", 2);
            else if (player->getLevel() <= 29)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.20-29", 3);
            else if (player->getLevel() <= 39)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.30-39", 4);
            else if (player->getLevel() <= 49)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.40-49", 5);
            else if (player->getLevel() <= 59)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.50-59", 6);
            else if (player->getLevel() <= 69)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.60-69", 7);
            else if (player->getLevel() <= 79)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.70-74", 8);
            else if (player->getLevel() <= 79)
                amount *= sConfigMgr->GetOption<uint32>("XPWeekend.BattlegroundRate.75-79", 8);
        }
        else if (sConfigMgr->GetOption<bool>("Dynamic.XP.Rate", true))
        {
            if (player->getLevel() <= 9)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.1-9", 1);
            else if (player->getLevel() <= 19)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.10-19", 2);
            else if (player->getLevel() <= 29)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.20-29", 3);
            else if (player->getLevel() <= 39)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.30-39", 4);
            else if (player->getLevel() <= 49)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.40-49", 5);
            else if (player->getLevel() <= 59)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.50-59", 6);
            else if (player->getLevel() <= 69)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.60-69", 7);
            else if (player->getLevel() <= 79)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.70-79", 8);
        }
    }


    int8 GetExperienceRate(Player * player) const
    {
        int8 rate = sConfigMgr->GetOption<int8>("XPWeekend.xpAmount", 2);

        int8 individualRate = player->GetPlayerSetting("mod-double-xp-weekend", SETTING_WEEKEND_XP_RATE).value;

        if (player->GetPlayerSetting("mod-double-xp-weekend", SETTING_WEEKEND_XP_DISABLE).value)
        {
            return 1;
        }

        // If individualxp setting is enabled... and a rate was set, overwrite it.
        if (sConfigMgr->GetOption<bool>("XPWeekend.IndividualXPEnabled", false) && individualRate)
        {
            rate = individualRate;
        }

        // Prevent returning 0% rate.
        return rate ? rate : 1;
    }

    bool IsEventActive() const
    {
        if (sConfigMgr->GetOption<bool>("XPWeekend.AlwaysEnabled", false))
            return true;
            
        if (!sConfigMgr->GetOption<bool>("XPWeekend.Enabled", false))
            return false;

        time_t t = time(nullptr);
        tm* now = localtime(&t);

        return now->tm_wday == WD_FRIDAY || now->tm_wday == WD_SATURDAY || now->tm_wday == WD_SUNDAY;
    }
};

void AdddoublexpScripts()
{
    new DoubleXpWeekend();
    new weekendxp_commandscript();
}
