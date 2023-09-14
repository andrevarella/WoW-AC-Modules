/*

# Custom Login Modifications #

### Description ###
------------------------------------------------------------------------------------------------------------------
- All options can be enabled or disabled in config
- Player ([ Faction ] - Name - Logon/Logoff message) notification can be announced to the world
- New characters can receive items, bags, and class-specific heirlooms on first login
- New characters can receive class-specific skills on first login
- New characters can receive specialized skills on first login
- New characters can receive exalted rep with capital cities (Ambassador) on first login


### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: CustomLogin
- Config: Yes
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2018.12.19 - Add BOA specific config options
- v2018.12.01 - Fix PlayerAnnounce when exiting game
- v2017.08.25 - Clean up code, add rep gain, add config options
- v2017.08.01 - Release


### Credits ###
------------------------------------------------------------------------------------------------------------------
#### A module for AzerothCore by StygianTheBest ([stygianthebest.github.io](http://stygianthebest.github.io)) ####

###### Additional Credits include:
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


#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "GuildMgr.h"

bool LoginEnable = 1;
bool LoginAnnounceModule = 1;
bool LoginPlayerAnnounce = 1;
bool LoginBOA = 1;
bool LoginBOARing = 1;
bool LoginBOABags = 1;
bool LoginBOAShoulders = 1;
bool LoginBOAShoulders2 = 1;
bool LoginBOAChest = 1;
bool LoginBOAChest2 = 1;
bool LoginBOATrinket = 1;
bool LoginBOATrinket2 = 1;
bool LoginBOAWeapon = 1;
bool LoginBOAWeapon2 = 1;
bool LoginBOAWeapon3 = 1;
bool LoginSkills = 1;
bool LoginMountSpell = 1;
bool LoginReputation = 0;

class LoginConfig : public WorldScript
{
public:
    LoginConfig() : WorldScript("LoginConfig") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            // Load Configuration Settings
            SetInitialWorldSettings();
        }
    }

    // Load Configuration Settings
    void SetInitialWorldSettings()
    {
        LoginEnable = sConfigMgr->GetOption<bool>("Login.Enable", 1);
        LoginAnnounceModule = sConfigMgr->GetOption<bool>("Login.Announce", 1);
        LoginPlayerAnnounce = sConfigMgr->GetOption<bool>("Login.PlayerAnnounce", 1);
        LoginBOA = sConfigMgr->GetOption<bool>("Login.BoA", 1);
        LoginBOARing = sConfigMgr->GetOption<bool>("Login.BoA.Ring", 1);
        LoginBOABags = sConfigMgr->GetOption<bool>("Login.BoA.Bags",1);
        LoginBOAShoulders = sConfigMgr->GetOption<bool>("Login.BoA.Shoulders", 1);
        LoginBOAShoulders2 = sConfigMgr->GetOption<bool>("Login.BoA.Shoulders2",1);
        LoginBOAChest = sConfigMgr->GetOption<bool>("Login.BoA.Chest",1);
        LoginBOAChest2 = sConfigMgr->GetOption<bool>("Login.BoA.Chest2",1);
        LoginBOATrinket = sConfigMgr->GetOption<bool>("Login.BoA.Trinket",1);
        LoginBOATrinket2 = sConfigMgr->GetOption<bool>("Login.BoA.Trinket2",1);
        LoginBOAWeapon = sConfigMgr->GetOption<bool>("Login.BoA.Weapon",1);
        LoginBOAWeapon2 = sConfigMgr->GetOption<bool>("Login.BoA.Weapon2",1);
        LoginBOAWeapon3 = sConfigMgr->GetOption<bool>("Login.BoA.Weapon3",1);
        LoginSkills = sConfigMgr->GetOption<bool>("Login.Skills", 1);
        LoginMountSpell = sConfigMgr->GetOption<bool>("Login.MountSpell", 1);
        LoginReputation = sConfigMgr->GetOption<bool>("Login.Reputation", 1);
    }
};

class LoginAnnounce : public PlayerScript
{

public:

    LoginAnnounce() : PlayerScript("LoginAnnounce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (LoginEnable)
        {
            if (LoginAnnounceModule)
            {
                ChatHandler(player->GetSession()).SendSysMessage("");
            }
        }
    }
};

class CustomLogin : public PlayerScript
{

public:
    CustomLogin() : PlayerScript("CustomLogin") { }

    void OnFirstLogin(Player* player)
    {
        // If enabled..
        if (LoginEnable)
        {
            // If enabled, give heirloom and other items
            if (LoginBOA)
            {
                // Define Equipment
                uint32 shoulders = 0, chest = 0, trinket = 0, weapon = 0, weapon2 = 0, weapon3 = 0, shoulders2 = 0, chest2 = 0, trinket2 = 0;
                const uint32 bag = 43345;		// bag 22 slots obsidian
                const uint32 ring = 10050;		// Mooncloth Bag

                // Outfit the character with bags and heirlooms that match their class
                // NOTE: Some classes have more than one heirloom option per slot
                switch (player->getClass())
                {

                case CLASS_WARRIOR:
                    trinket = 10050;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_PALADIN:
                    trinket = 10050;
                    shoulders = 21177;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_HUNTER:
                    trinket = 10050;
                    shoulders = 31737;
                    weapon = 10050;
                    weapon3 = 34760; // Food/Drink lvl 70
                    break;

                case CLASS_ROGUE:
                    trinket = 10050;
                    weapon = 10050;
                    weapon2 = 10050;
                    shoulders = 3775; // Crippling Poison
                    chest = 22055; // wound poison V (lvl 64)
                    weapon3 = 22054; // Deadly Poison VII (lvl 70)
                    trinket2 = 21927; // Instant Poison VII (lvl 68)
                    chest2 = 34760;
                    break;

                case CLASS_PRIEST:
                    trinket = 10050;
                    shoulders = 17029;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_DEATH_KNIGHT:
                    trinket = 10050;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_SHAMAN:
                    trinket = 46978; // totem of earthen ring
                    //trinket = 10050;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_MAGE:
                    trinket = 10050;
                    shoulders = 17020;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_WARLOCK:
                    trinket = 10050;
                    chest = 5565;
                    shoulders = 6265;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                case CLASS_DRUID:
                    trinket = 10050;
                    shoulders = 22148;
                    weapon = 10050;
                    weapon2 = 10050;
                    weapon3 = 34760;
                    break;

                default:
                    break;
                }

                // Hand out the heirlooms. I prefer only the ring and trinkets for new characters.
                // TODO: Improve the code. No time, no care, no worries!
                switch (player->getClass())
                {

                case CLASS_PRIEST:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 255); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_DEATH_KNIGHT:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_PALADIN:
                    if (LoginBOATrinket) { player->AddItem(trinket, 1); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1000); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_WARRIOR:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_HUNTER:
                    if (LoginBOATrinket) { player->AddItem(trinket, 1); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 20000); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_MAGE:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 50); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 200); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_ROGUE:
                    if (LoginBOATrinket) { player->AddItem(trinket, 1); };
                    if (LoginBOATrinket) { player->AddItem(trinket2, 255); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1000); }
                    if (LoginBOAChest) { player->AddItem(chest, 255); }
                    if (LoginBOAChest2) { player->AddItem(chest2, 500); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 255); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_DRUID:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOATrinket) { player->AddItem(trinket2, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 75); }
                    if (LoginBOAShoulders2) { player->AddItem(shoulders2, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_WARLOCK:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOATrinket) { player->AddItem(trinket2, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1000); }
                    if (LoginBOAShoulders2) { player->AddItem(shoulders2, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 500); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                case CLASS_SHAMAN:
                    if (LoginBOATrinket) { player->AddItem(trinket, 1); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1); }
                    if (LoginBOAShoulders2) { player->AddItem(shoulders2, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAChest2) { player->AddItem(chest2, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOAWeapon2) { player->AddItem(weapon2, 1); }
                    if (LoginBOAWeapon3) { player->AddItem(weapon3, 500); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;

                default:
                    if (LoginBOATrinket) { player->AddItem(trinket, 2); };
                    if (LoginBOARing) { player->AddItem(ring, 1); }
                    if (LoginBOAShoulders) { player->AddItem(shoulders, 1); }
                    if (LoginBOAChest) { player->AddItem(chest, 1); }
                    if (LoginBOAWeapon) { player->AddItem(weapon, 1); }
                    if (LoginBOABags) { player->AddItem(bag, 1); }
                    break;
                }

                // Inform the player they have new items
                std::ostringstream ss;
                ss << "|cffFF8000[|cFFBDB76BCL|cffFF8000] You have received |cFFBDB76Ba surprise|cffFF8000 in your backpack!";
                ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
            }

            // If enabled, learn additional skills
            if (LoginSkills)
            {
                switch (player->getClass())
                {

                    /*
                        // Skill Reference

                        player->learnSpell(204);	// Defense
                        player->learnSpell(264);	// Bows
                        player->learnSpell(5011);	// Crossbow
                        player->learnSpell(674);	// Dual Wield
                        player->learnSpell(15590);	// Fists
                        player->learnSpell(266);	// Guns
                        player->learnSpell(196);	// Axes
                        player->learnSpell(198);	// Maces
                        player->learnSpell(201);	// Swords
                        player->learnSpell(750);	// Plate Mail
                        player->learnSpell(200);	// PoleArms
                        player->learnSpell(9116);	// Shields
                        player->learnSpell(197);	// 2H Axe
                        player->learnSpell(199);	// 2H Mace
                        player->learnSpell(202);	// 2H Sword
                        player->learnSpell(227);	// Staves
                        player->learnSpell(2567);	// Thrown
                    */

                //case CLASS_PALADIN:
                    //player->learnSpell(750);	// Plate Mail
                    //break;

                case CLASS_SHAMAN:
                    player->learnSpell(58059);  // glyph of renewed life
                    player->learnSpell(58057);  // glyph of water walking
                    break;

                case CLASS_WARRIOR:
                    player->learnSpell(58096);  // Glyph of bloodrage
                    break;

                case CLASS_HUNTER:
                    player->learnSpell(57870);  // Glyph of Mend Pet
                    break;

                //case CLASS_ROGUE:
                    //player->learnSpell(1784);	// Stealth
                    //break;

                case CLASS_DRUID:
                    player->learnSpell(54824);  // Glyph of Swiftmend
                    player->learnSpell(57857);  // Glyph of Unburdened Rebirth
                    break;

                case CLASS_MAGE:
                    player->learnSpell(57925);  // Glyph of Slow Fall
                    break;

                case CLASS_WARLOCK:
                    player->learnSpell(61993);  // Ritual of Summoning
                    break;

                case CLASS_PRIEST:
                    player->learnSpell(57987);  // Glyph of Levitate
                    break;

                case CLASS_DEATH_KNIGHT:
                    player->learnSpell(58680);  // Glyph of Horn of Winter
                    player->learnSpell(60200);  // Glyph of Raise Dead
                    break;

                default:
                    break;
                }

                // Inform the player they have new skills
                std::ostringstream ss;
                ss << "|cffFF8000[|cFFBDB76BCL|cffFF8000] You have been granted |cFFBDB76Badditional weapon skills|cffFF8000.";
                ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
            }

            // If enabled, learn mounts based on faction
            if (LoginMountSpell)
            {
                switch (player->GetTeamId())
                {

                case TEAM_ALLIANCE:

                    if (player->getRace() == RACE_DWARF)
                    {
                        /*player->learnSpell(23238); // Ram Riding
                        player->learnSpell(23239); // Ram Riding
                        player->learnSpell(23240); // Ram Riding*/

                        // Dwarf Pala Mount
                        if (player->getClass() == CLASS_PALADIN)
                        {
                            player->learnSpell(85147); // Summon Dawnforge Ram
                        }
                    }
                    // Alliance Mount
                    // player->learnSpell(32240);	// Snowy Gryphon
                    // player->learnSpell(54753);  // Mount White Polar Bear
                    /*if (player->getRace() == RACE_HUMAN)
                    {
                        player->learnSpell(23227); // Swift White Steed
                        player->learnSpell(23228); // Swift White Steed
                        player->learnSpell(23229); // Swift White Steed
                    }
                    else if (player->getRace() == RACE_NIGHTELF)
                    {
                        player->learnSpell(23221); // Nightsaber Riding
                        player->learnSpell(23219); // Ram Riding
                        player->learnSpell(23338); // Ram Riding
                    }
                    else if (player->getRace() == RACE_GNOME)
                    {
                        player->learnSpell(23225); // Mechanostrider Piloting
                        player->learnSpell(23223); // Ram Riding
                        player->learnSpell(23222); // Ram Riding
                    }
                    else if (player->getRace() == RACE_DRAENEI)
                    {
                        player->learnSpell(35713); // Elekk Riding
                        player->learnSpell(35712); // Ram Riding
                        player->learnSpell(35714); // Ram Riding*/
                        /*
                        // Draenei Pala Mount
                        if (player->getClass() == CLASS_PALADIN)
                        {
                            player->learnSpell(85148); // Summon Lightforged Ruinstrider
                            player->learnSpell(85146); // Summon Great Exarch's Elekk
                        }
                        */
                    //}
                    break;

                case TEAM_HORDE:

                    if (player->getRace() == RACE_TAUREN)
                    {
                        /*player->learnSpell(23249); // Kodo
                        player->learnSpell(23248); // Kodo
                        player->learnSpell(23247); // Kodo*/

                        // Tauren Pala Mount
                        if (player->getClass() == CLASS_PALADIN)
                        {
                            player->learnSpell(85145); // Summon Great Sunwalker Kodo
                        }
                    }
                    // Horde Mount
                    /*player->learnSpell(32243);	// tawny wind rider
                    if (player->getRace() == RACE_ORC)
                    {
                        player->learnSpell(23250); // Swift Wolves
                        player->learnSpell(23252); // Swift Wolves
                        player->learnSpell(23251); // Swift Wolves
                    }
                    else if (player->getRace() == RACE_UNDEAD_PLAYER)
                    {
                        player->learnSpell(17465); // Skeletal Warh
                        player->learnSpell(66846); // Skeletal Warh
                        player->learnSpell(23246); // Skeletal Warh
                    }
                    /*else if (player->getRace() == RACE_TROLL)
                    {
                        player->learnSpell(23241); // Raptor
                        player->learnSpell(23242); // Raptor
                        player->learnSpell(23243); // Raptor
                    }
                    else if (player->getRace() == RACE_BLOODELF)
                    {
                        player->learnSpell(35025); // Hawkstrider
                        player->learnSpell(33660); // Hawkstrider
                        player->learnSpell(35027); // Hawkstrider
                    }*/
                    break;
                default:
                    break;
                }
            }

            // If enabled.. set exalted factions (AzerothCore config for rep not working as of 2017-08-25)
            if (LoginReputation)
            {
                switch (player->GetTeamId())
                {

                    // Alliance Capital Cities
                case TEAM_ALLIANCE:
                    player->SetReputation(47, 999999);	// IronForge
                    player->SetReputation(72, 999999);	// Stormwind 
                    player->SetReputation(69, 999999);	// Darnassus
                    player->SetReputation(389, 999999);	// Gnomeregan
                    player->SetReputation(930, 999999);	// Exodar
                    player->SetReputation(1156, 999999);// Ashen Verdict
                    break;

                    // Horde Capital Cities
                case TEAM_HORDE:
                    player->SetReputation(68, 999999);	// Undercity
                    player->SetReputation(76, 999999);	// Orgrimmar
                    player->SetReputation(81, 999999);	// Thunder Bluff
                    player->SetReputation(530, 999999);	// DarkSpear
                    player->SetReputation(911, 999999);	// Silvermoon
                    player->SetReputation(1156, 999999);// Ashen Verdict
                    break;

                default:
                    break;
                }
            }
        }
    }

    void OnLogin(Player* player)
    {
        // If enabled..
        if (LoginEnable)
        {
            // If enabled..
            if (LoginPlayerAnnounce)
            {
                // Announce Player Login
                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    std::ostringstream ss;
                    ss << "|cffFFFFFF[|cff2897FF Alliance |cffFFFFFF]:|cff4CFF00 " << player->GetName() << "|cffFFFFFF has come online.";
                    sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
                }
                else
                {
                    std::ostringstream ss;
                    ss << "|cffFFFFFF[|cffFF0000 Horde |cffFFFFFF]:|cff4CFF00 " << player->GetName() << "|cffFFFFFF has come online.";
                    sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
                }
            }
        }
    }

    void OnLogout(Player *player)
    {
        if (LoginEnable)
        {
            // If enabled..
            if (LoginPlayerAnnounce)
            {
                // Announce Player Login
                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    std::ostringstream ss;
                    ss << "|cffFFFFFF[|cff2897FF Alliance |cffFFFFFF]|cff4CFF00 " << player->GetName() << "|cffFFFFFF has left the game.";
                    sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
                }
                else
                {
                    std::ostringstream ss;
                    ss << "|cffFFFFFF[|cffFF0000 Horde |cffFFFFFF]|cff4CFF00 " << player->GetName() << "|cffFFFFFF has left the game.";
                    sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
                }
            }
        }
    }
};

void AddCustomLoginScripts()
{
    new LoginConfig();
    new LoginAnnounce();
    new CustomLogin();
}
