/*

# Congrats On Level #

#### A module for AzerothCore (https://github.com/azerothcore/mod-congrats-on-level)
#### Originally by [StygianTheBest](https://github.com/StygianTheBest/AzerothCore-Content/tree/master/Modules)
------------------------------------------------------------------------------------------------------------------


### Description ###
------------------------------------------------------------------------------------------------------------------
This module rewards players when they reach specific levels. It has the option to reward gold, two items, and a
buff or any combination. It also announces to the world when a player levels up. All rewards can be set in the
config file for quick modifications.


### Features ###
------------------------------------------------------------------------------------------------------------------
- Announces player level up to world
- Awards the player for gaining new levels
- The player forfeits the rewards if their bags are full


### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: CongratsOnLevel
- Config: Yes
    - Enable Module
    - Enable Module Announce
    - Set Items/Gold/Buffs Given At Each Level
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2022.04.09 - Fixed deprecation warnings
- v2017.08.06 - Release
- v2017.09.30 - Fix Level Display Bug. Update Strings.


### Credits ###
------------------------------------------------------------------------------------------------------------------
- [SoulSeekkor](https://github.com/SoulSeekkor)
- [LordPsyan](https://bitbucket.org/lordpsyan/lordpsyan-patches)
- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [AzerothCore Discord](https://discord.gg/gkt4y2x)
- [EMUDevs](https://youtube.com/user/EmuDevs)
- [AC-Web](http://ac-web.org/)
- [ModCraft.io](http://modcraft.io/)
- [OwnedCore](http://ownedcore.com/)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [AoWoW](https://wotlk.evowow.com/)


### License ###
------------------------------------------------------------------------------------------------------------------
- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "Configuration/Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"

class CongratsAnnounce : public PlayerScript
{

public:

    CongratsAnnounce() : PlayerScript("CongratsAnnounce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (sConfigMgr->GetOption<bool>("Congrats.Announce", true))
        {
            std::string messageModule = "";
            switch (player->GetSession()->GetSessionDbLocaleIndex())
            {
                case LOCALE_enUS:
                case LOCALE_koKR:
                case LOCALE_frFR:
                case LOCALE_deDE:
                case LOCALE_zhCN:
                case LOCALE_zhTW:
                case LOCALE_ruRU:
                {
                    messageModule = "This server is running the |cff4CFF00CongratsOnLevel |rmodule.";
                    break;
                }
                case LOCALE_esES:
                case LOCALE_esMX:
                {
                    messageModule = "Este servidor está ejecutando el módulo |cff4CFF00CongratsOnLevel|r";
                }
                default:
                    break;
            }
            ChatHandler(player->GetSession()).SendSysMessage(messageModule);
        }
    }
};

class CongratsOnLevel : public PlayerScript
{

public:
    CongratsOnLevel() : PlayerScript("CongratsOnLevel") { }

    // Level Up Rewards
    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        // If enabled...
        if (sConfigMgr->GetOption<bool>("Congrats.Enable", true))
        {

            std::string level;
            uint32 money, item1, item2, spell;

            if (oldLevel < 80 && player->getLevel() == 80)
            {
                // Level 80
                const uint32 L801 = sConfigMgr->GetOption<uint32>("Congrats.L801", 0);
                const uint32 L802 = sConfigMgr->GetOption<uint32>("Congrats.L802", 0);
                const uint32 L80M = sConfigMgr->GetOption<uint32>("Congrats.L80M", 0);
                const uint32 L80S = sConfigMgr->GetOption<uint32>("Congrats.L80S", 0);

                level = "80";
                item1 = L801;
                item2 = L802;
                money = L80M;
                spell = L80S;
            }

            // If enabled...
            if (sConfigMgr->GetOption<bool>("CongratsPerLevel.Enable", true))
            {
                // Issue a server notification for the player on level up.
                std::ostringstream ss;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        ss << "|cffFFFF00[ |cffFFFF00P|cffFFFF00A|cffFFFF00R|cffFFFF00A|cffFFFF00B|cffFFFF00E|cffFFFF00N|cffFFFF00S|cffFFFF00! |cffFFFF00] : |cff4CFF00 " << player->GetName() << " |cffFFFFFFalcançou o |cff4CFF00Level " << std::to_string(player->getLevel()) << "|cffFFFFFF!";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        ss << "|cffFFFFFF[ |cffFF0000F|cffFFA500E|cffFFFF00L|cff00FF00I|cff00FFFFC|cff6A5ACDI|cffFF00FFT|cff98FB98A|cff00FF00C|cff00FFFFI|cffFF0000O|cff00FF00N|cff00FFFFE|cffFF00FFS|cffFF0000! |cffFFFFFF] : |cff4CFF00 " << player->GetName() << " |cffFFFFFFha alcanzado |cff4CFF00el nivel " << std::to_string(player->getLevel()) << "|cffFFFFFF!";
                    }
                    default:
                        break;
                }
                sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
            }

            // If level is defined, they hit a reward level.
            if (!level.empty())
            {
                // Issue a server notification for the player on level up.
                std::ostringstream ss;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        ss << "|cffFFFF00[ |cffFFFF00P|cffFFFF00A|cffFFFF00R|cffFFFF00A|cffFFFF00B|cffFFFF00E|cffFFFF00N|cffFFFF00S|cffFFFF00! |cffFFFF00] : |cff4CFF00 " << player->GetName() << " |cffFFFFFFalcançou o |cff4CFF00Level " << level << "|cffFFFFFF!";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        ss << "|cffFFFFFF[ |cffFF0000F|cffFFA500E|cffFFFF00L|cff00FF00I|cff00FFFFC|cff6A5ACDI|cffFF00FFT|cff98FB98A|cff00FF00C|cff00FFFFI|cffFF0000O|cff00FF00N|cff00FFFFE|cffFF00FFS|cffFF0000! |cffFFFFFF] : |cff4CFF00 " << player->GetName() << " |cffFFFFFFha alcanzado |cff4CFF00el nivel " << level << "|cffFFFFFF!";
                    }
                    default:
                        break;
                }
                sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());

                // Give the items to the player
                player->AddItem(item1, 1);				// Defined Item 1
                player->AddItem(item2, 1);				// Defined Item 2

                // Give gold to the player
                player->ModifyMoney(money*GOLD);		// Defined Gold

                // Buff the player
                player->CastSpell(player, spell);		// Defined Spell

                // Issue a raid warning to the player
                std::ostringstream ss2;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        ss2 << "Parabens por alcançar o level " << level << " " << player->GetName() << "! Você ganhou " << money << " gold e alguns items!";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        ss2 << "¡Felicidades por el nivel " << level << " " << player->GetName() << " Se le ha concedido " << money << " oro y unos cuantos tesoros!";
                    }
                    default:
                        break;
                }
                player->GetSession()->SendNotification(SERVER_MSG_STRING, ss2.str().c_str());
                return;
            }
        }
    }
};

void AddCongratsOnLevelScripts()
{
	new CongratsAnnounce();
    new CongratsOnLevel();
}
