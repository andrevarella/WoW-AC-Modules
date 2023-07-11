#include "Config.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"

class AccountMounts : public PlayerScript
{
    static const bool limitrace = false; // This set to true will only learn mounts from chars on the same team, do what you want.
    static const bool limitlevel = false; // This checks the player's level and will only add achievements to players of that level.
    int minlevel = 60; // It's set to players of the level 60. Requires limitlevel to be set to true.
    int setlevel = 1; // Dont Change

public:
    AccountMounts() : PlayerScript("AccountMounts") { }

    void OnLogin(Player* pPlayer)
    {
        if (sConfigMgr->GetOption<bool>("Account.Mounts.Enable", true))
        {
            if (sConfigMgr->GetOption<bool>("Account.Mounts.Announce", false))
            {
                ChatHandler(pPlayer->GetSession()).SendSysMessage("This server is running the |cff4CFF00AccountMounts |rmodule.");
            }
            std::vector<uint32> Guids;
            uint32 playerGUID = pPlayer->GetGUID().GetCounter();
            QueryResult result1 = CharacterDatabase.Query("SELECT guid, race FROM characters WHERE account = {}", playerGUID);
            if (!result1)
                return;

            do
            {
                Field* fields = result1->Fetch();
    
                uint32 race = fields[1].Get<uint8>();

                if ((Player::TeamIdForRace(race) == Player::TeamIdForRace(pPlayer->getRace())) || !limitrace)
                    Guids.push_back(result1->Fetch()[0].Get<uint32>());

            } while (result1->NextRow());

            std::vector<uint32> Spells;

            for (auto& i : Guids)
            {
                QueryResult result2 = CharacterDatabase.Query("SELECT spell FROM character_spell WHERE guid = {}", i);
                if (!result2)
                    continue;

                do
                {
                    Spells.push_back(result2->Fetch()[0].Get<uint32>());
                } while (result2->NextRow());
            }

            for (auto& i : Spells)
            {
                auto sSpell = sSpellStore.LookupEntry(i);
                if (sSpell->EffectApplyAuraName[0] == SPELL_AURA_MOUNTED)
					pPlayer->learnSpell(sSpell->Id);
            }
        }
	}

    void AddMounts(Player* player, uint32 SpellID)
    {
        if (sConfigMgr->GetOption<bool>("Account.Mounts.Enable", true))
        {
            if (limitlevel)
                setlevel = minlevel;

            if (player->getLevel() >= setlevel)
                player->learnSpell(SpellID);

            //if (player->getLevel() >= setlevel)
                //player->learnSpell(sSpellStore.LookupEntry(SpellID));
        }
    }
};

void AddAccountMountsScripts()
{
    new AccountMounts;
}
