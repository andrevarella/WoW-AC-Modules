/*
<--------------------------------------------------------------------------->
- Developer(s): Lille Carl, Grim/Render
- Complete: %100
- ScriptName: 'AccountAchievements'
- Comment: Tested and Works.
- Orginial Creator: Lille Carl
- Edited: Render/Grim
<--------------------------------------------------------------------------->
*/

#include "Config.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"

class AccountAchievements : public PlayerScript
{
	static const bool limitrace = true; // This set to true will only achievements from chars on the same team, do what you want. NOT RECOMMANDED TO BE CHANGED!!!
	static const bool limitlevel = false; // This checks the player's level and will only add achievements to players of that level.
	int minlevel = 80; // It's set to players of the level 60. Requires limitlevel to be set to true.
	int setlevel = 1; // Dont Change

public:
	AccountAchievements() : PlayerScript("AccountAchievements") { }

	void OnLogin(Player* pPlayer)
	{
		if (sConfigMgr->GetOption<bool>("Account.Achievements.Enable", true))
        {
			if (sConfigMgr->GetOption<bool>("Account.Achievements.Announce", true))
            {
                ChatHandler(pPlayer->GetSession()).SendSysMessage("This server is running the |cff4CFF00AccountAchievements |rmodule.");
            }

			std::vector<uint32> Guids;
			QueryResult result1 = CharacterDatabase.Query("SELECT guid, race FROM characters WHERE account = {}", pPlayer->GetSession()->GetAccountId());
			if (!result1)
				return;

			do
			{
				Field* fields = result1->Fetch();

				uint32 race = fields[1].Get<uint8>();

				if ((Player::TeamIdForRace(race) == Player::TeamIdForRace(pPlayer->getRace())) || !limitrace)
					Guids.push_back(result1->Fetch()[0].Get<uint32>());

			} while (result1->NextRow());

			std::vector<uint32> Achievement;

			for (auto& i : Guids)
			{
				QueryResult result2 = CharacterDatabase.Query("SELECT achievement FROM character_achievement WHERE guid = {}", i);
				if (!result2)
					continue;

				do
				{
					Achievement.push_back(result2->Fetch()[0].Get<uint32>());
				} while (result2->NextRow());
			}

			for (auto& i : Achievement)
			{
				auto sAchievement = sAchievementStore.LookupEntry(i);
					AddAchievements(pPlayer, sAchievement->ID);
			}
		}
	}

	void AddAchievements(Player* player, uint32 AchievementID)
	{
		if (sConfigMgr->GetOption<bool>("Account.Achievements.Enable", true))
        {
            // Gladiators
            static const std::vector<uint32> excludedAchievements = { 2091, 418, 419, 420, 3336, 3436, 3758, 4599, 10003, 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 10012, 10013, 10014, 10015, 10016, 10017, 10018, 10019, 10020, 10021, 10022, 10023, 10024, 10025, 10026, 10054, 10055, 10056, 10085, 10086, 10087, 10088, 10089};
            if (std::find(excludedAchievements.begin(), excludedAchievements.end(), AchievementID) != excludedAchievements.end())
            {
                return; // Skip adding excluded PvP/Gladiator achievements
            }

            // Mounts Achiev
            static const std::vector<uint32> excludedMountAchievements = {
                3096,
                3756,
                3757,
                4600,
                415,
                431,
                729,
                980,
                880,
                881,
                882,
                883,
                884,
                885,
                886, // teste
                887, // teste2
                888,
                430,
            };
            if (std::find(excludedMountAchievements.begin(), excludedMountAchievements.end(), AchievementID) != excludedMountAchievements.end())
            {
                return; // Skip adding excluded Mounts achievements
            }

            // Realm First Achievs
            static const std::vector<uint32> excludedRFAchievements = { 457, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 1404, 1405, 1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1400, 456, 1402, 3117, 3259, 4078, 4576, 1463, 10028 };
            if (std::find(excludedRFAchievements.begin(), excludedRFAchievements.end(), AchievementID) != excludedRFAchievements.end())
            {
                return; // Skip adding excluded Realm First achievements
            }

			if (limitlevel)
				setlevel = minlevel;

			if (player->getLevel() >= setlevel)
				player->CompletedAchievement(sAchievementStore.LookupEntry(AchievementID));
		}
	}
};

void AddAccountAchievementsScripts()
{
	new AccountAchievements;
}
