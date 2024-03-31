
#include "SpellScripts_loader.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "Spell.h"
#include "SpellAuraDefines.h"

/*
SpellCorrections:
#include "SpellInfo.h"
#include "SpellAuras.h"
#include "Player.h"
#include "DBCStores.h"

*/

/*
//#include "Chat.h"
//#include "MapMgr.h"
//#include "Group.h"
//#include "GroupMgr.h"
//#include "Battleground.h"
//#include "BattlegroundMgr.h"
//#include "Config.h"
//#include "Configuration/Config.h"
//#include "World.h"
//#include "Pet.h"
//#include "SpellAuras.h"
*/



class BattlebornRealm : public PlayerScript
{
public:
    BattlebornRealm() : PlayerScript("BattlebornRealm") {}

    /*
    void OnCreate(Player* player) override
    {
        if (something here)
        {
            player->setCinematic(1);
            player->SetLevel(80);

            WorldLocation startLoc;
            startLoc.m_positionX = 4718.45f;
            startLoc.m_positionY = -1974.84f;
            startLoc.m_positionZ = 1086.91f;
            startLoc.m_orientation = 0.06f;
            startLoc.m_mapId  = 1;

            player->Relocate(&startLoc);
            player->ResetMap();
            player->SetMap(sMapMgr->CreateMap(1, player));

            player->SaveToDB(false,false);
        }
    }
    */

    // Primeiro login do char
    void OnFirstLogin(Player* player) override
    {
        player->LearnDefaultSkills();
        /*
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
            //player->learnSpell(750);	// Plate Mail (platemail ou plate?)
            // Mail
        */

        switch (player->getClass())
        {   // Backpack tem 16 slots (14 com Fishing pole e hearthstone). -4 das bags, sobra 10.

            case CLASS_PRIEST:
                player->AddItem(1179, 20); // Drink lvl 5
                player->AddItem(1205, 20); // Drink lvl 15
                player->AddItem(414, 20);  // Food lvl 5
                player->AddItem(422, 20);  // Food lvl 15

                player->learnSpell(57987);  // Glyph of Levitate
                break;

            case CLASS_MAGE:
                player->AddItem(1179, 20); // Drink lvl 5
                player->AddItem(1205, 20); // Drink lvl 15
                player->AddItem(414, 20);  // Food lvl 5
                player->AddItem(422, 20);  // Food lvl 15
                player->AddItem(17031, 50); // 17031 Rune of Teleportation
                player->AddItem(17032, 20); // Rune of Portals

                player->learnSpell(58661); // Conjure Refreshment Table
                player->learnSpell(57925); // Glyph of Slow Fall
                break;

            case CLASS_WARLOCK:
                player->AddItem(6265, 1000); // Soul Shard
                player->AddItem(1179, 20);  // Drink lvl 5
                player->AddItem(1205, 20);  // Drink lvl 15
                player->AddItem(414, 20);   // Food lvl 5
                player->AddItem(422, 20);   // Food lvl 15

                player->learnSpell(61993);  // Ritual of Summoning
                break;

            case CLASS_ROGUE:
                player->AddItem(414, 20);    // Food lvl 5
                player->AddItem(422, 20);    // Food lvl 15
                player->AddItem(3775, 1000); // Crippling Poison
                player->AddItem(5237, 1000); // Mind Numbing Poison 
                player->AddItem(6947, 200);  // Instant Poison Rank1 (level 20) - Deadly Poison e Wound só level 30/32

                // Glyph?
                break;

            case CLASS_DRUID:
                player->AddItem(1179, 20);  // Drink lvl 5
                player->AddItem(1205, 20);  // Drink lvl 15
                player->AddItem(414, 20);   // Food lvl 5
                player->AddItem(422, 20);   // Food lvl 15

                player->learnSpell(54824);  // Glyph of Swiftmend
                player->learnSpell(57857);  // Glyph of Unburdened Rebirth (minor)
                break;

            case CLASS_HUNTER:
                player->AddItem(1179, 20);  // Drink lvl 5
                player->AddItem(1205, 20);  // Drink lvl 15
                player->AddItem(414, 20);   // Food lvl 5
                player->AddItem(422, 20);   // Food lvl 15
                player->AddItem(2512, 800); // Rough Arrow (lvl 5)
                player->AddItem(3464, 3000);// Feathered Arrow

                player->learnSpell(57870);  // Glyph of Mend Pet
                break;

            case CLASS_SHAMAN:
                player->AddItem(46978, 1); // Totem of Earthen Ring
                player->AddItem(1179, 20); // Drink lvl 5
                player->AddItem(1205, 20); // Drink lvl 15
                player->AddItem(414, 20);  // Food lvl 5
                player->AddItem(422, 20);  // Food lvl 15

                player->learnSpell(58059); // glyph of Renewed Life (incarnation)
                player->learnSpell(58057); // glyph of Water Walking
                break;

            case CLASS_WARRIOR:
                player->AddItem(414, 20);  // Food lvl 5
                player->AddItem(422, 20);  // Food lvl 15

                player->learnSpell(58096);  // Glyph of bloodrage
                break;

            case CLASS_PALADIN:
                player->AddItem(1179, 20); // Drink lvl 5
                player->AddItem(1205, 20); // Drink lvl 15
                player->AddItem(414, 20);  // Food lvl 5
                player->AddItem(422, 20);  // Food lvl 15
                break;

            case CLASS_DEATH_KNIGHT:
                player->learnSpell(58680);  // Glyph of Horn of Winter (minor)
                player->learnSpell(60200);  // Glyph of Raise Dead (minor)
                break;

            // Da para todas as classes que nao estejam acima
            default: 
                //player->AddItem(43345, 1); // Dragon Hide Bag (22 slots)
                break;
        }

        // Learn dual spec
        player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
        player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());

        // Learn Secondary Profs
        player->learnSpell(3273);  // First Aid
        player->learnSpell(2550);  // Cooking
        player->learnSpell(7620);  // Fishing
        player->AddItem(45120, 1); // Basic Fishing Pole
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        // Dwarf/Tauren Paladin Mounts
        if (oldLevel < 40 && player->getLevel() == 40)
        {
            switch (player->GetTeamId())
            {
                case TEAM_ALLIANCE:
                    if (player->getRace() == RACE_DWARF && player->getClass() == CLASS_PALADIN)
                    {
                        player->learnSpell(85147); // Summon Dawnforge Ram (Dwarf Paladin Mount)
                    }
                    break;

                case TEAM_HORDE:
                    if (player->getRace() == RACE_TAUREN && player->getClass() == CLASS_PALADIN)
                    {
                        player->learnSpell(85145); // Summon Great Sunwalker Kodo (Tauren Paladin Mount)
                    }
                    break;

                default:
                    break;
            }
        }

        // Learn Create Soulwell Warlock
        if (oldLevel < 69 && player->getLevel() == 69)
        {
            switch (player->getClass())
            {
                case CLASS_WARLOCK:
                    player->learnSpell(58889);
                    break;

                default:
                    break;
            }
        }

        // Congrats on 80 + 500g
        if (oldLevel < 80 && player->getLevel() == 80)
        {
            std::ostringstream ss;
            ss << "|cffFFFF00[ |cffFFFF00P|cffFFFF00A|cffFFFF00R|cffFFFF00A|cffFFFF00B|cffFFFF00E|cffFFFF00N|cffFFFF00S|cffFFFF00! |cffFFFF00] : " << player->GetName() << " |cffFFFFFFalcançou o |cff4CFF00Level 80|cffFFFFFF!";
            sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());

            // Mandar 500g para o jogador
            player->ModifyMoney(500 * GOLD);

            // Mensagem adicional para o jogador que atingiu o nível 80
            std::ostringstream ss2;
            ss2 << "Parabéns por alcançar o nível 80, " << player->GetName() << "!";
            ss2 << "Você ganhou 500 gold";
            player->GetSession()->SendNotification(SERVER_MSG_STRING, ss2.str().c_str());
        }
    }

    void OnLogin(Player* player) override
    {
        switch (player->getClass())
        {
            case CLASS_WARRIOR:
            {
                uint32 battlestance = 2457;
                uint32 defensivestance = 71;
                uint32 berserkerstance = 2458;
                uint32 mortalstrike = 47486;
                uint32 bloodthirst = 23881;
                uint32 shockwave = 46968;

                if (!player->HasAura(battlestance) && !player->HasAura(defensivestance) && !player->HasAura(berserkerstance))
                {
                    if (player->HasSpell(mortalstrike))
                       player->AddAura(battlestance, player);
                    else if (player->HasSpell(bloodthirst))
                       player->AddAura(berserkerstance, player);
                    else if (player->HasSpell(shockwave))
                       player->AddAura(defensivestance, player);
                }
                break;
            }
            case CLASS_DEATH_KNIGHT:
            {
                uint32 bloodpresence = 48266;
                uint32 frostpresence = 48263;
                uint32 unholypresence = 48265;
                uint32 heartstrike = 55050;
                uint32 froststrike = 55268;
                uint32 scourgestrike = 55271;

                if (!player->HasAura(bloodpresence) && !player->HasAura(frostpresence) && !player->HasAura(unholypresence))
                {
                    if (player->HasSpell(heartstrike))
                        player->AddAura(frostpresence, player);
                    else if (player->HasSpell(froststrike) || player->HasSpell(scourgestrike))
                        player->AddAura(unholypresence, player);
                }
                break;
            }

            default:
                break;
        }
    }

    // Pedra/Refreshment table ao entrar em arena
    void OnMapChanged(Player* player) override
    {
        if (!player)
            return;

        MapEntry const* mEntry = sMapStore.LookupEntry(player->GetMapId());

        if (player->InArena() && mEntry->IsBattleArena() && !player->IsSpectator())
        {
            switch (player->getClass())
            {
                case CLASS_WARLOCK:
                    player->CastSpell(player, 58889, TRIGGERED_FULL_MASK); // warlock soulwell in arena
                    break;
                case CLASS_MAGE:
                    player->CastSpell(player, 58661, TRIGGERED_FULL_MASK); // mage refreshment table in arena
                    break;
                default:
                    break;
            }
        }
    }
};


enum Spells
{
    SPELL_STARFALL = 53201,
    SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS = 25742,
    SPELL_LAVA_BURST = 60043,
    SPELL_SHAMAN_FERAL_SPIRIT = 51533,
};

// Druid:
//
// mudar em spell_druid.cpp:
// // Remove tiger fury / mangle(bear)
// const uint32 TigerFury[6] = { 5217, 6793, 9845, 9846, 50212, 50213 }; - tirar o 9846, 50212, 50213 (p/ Tiger's Fury nao sair apos usar Berserk)
// 
// 83293 - Glyph of Omen of Clarity
class spell_druid_glyph_omen_of_clarity : public AuraScript
{
    PrepareAuraScript(spell_druid_glyph_omen_of_clarity);


    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActionTarget();
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();

        // Verificar se o alvo é um jogador ou pet
        if (actor && (actor->GetTypeId() == TYPEID_PLAYER || (actor->GetTypeId() == TYPEID_UNIT && actor->IsPet())))
        {
            return false;
        }

        // Verificar se a spell lançada é a 770 (Faerie Fire)
        if (spellInfo && spellInfo->Id == 770)
        {
            return false;
        }

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        // Implemente o comportamento após o proc aqui
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_druid_glyph_omen_of_clarity::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_druid_glyph_omen_of_clarity::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 83369 - Wild Charge
class spell_druid_wild_charge : public SpellScript
{
    PrepareSpellScript(spell_druid_wild_charge);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (caster && caster->HasUnitState(UNIT_STATE_ROOT))
        {
            return SPELL_FAILED_ROOTED;
        }

        if (caster->GetShapeshiftForm() == FORM_NONE)  // Intervene
        {
            caster->CastSpell(target, 83374, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_AQUA) // Sprint Aquatic
        {
            caster->CastSpell(target, 83370, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_CAT) // Charge Cat
        {
            caster->CastSpell(target, 49376, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_BEAR || caster->GetShapeshiftForm() == FORM_DIREBEAR) // Charge Bear
        {
            caster->CastSpell(target, 16979, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_TRAVEL) // Disengage Travel
        {
            caster->CastSpell(caster, 83371, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_MOONKIN) // Disengage Moonkin
        {
            caster->CastSpell(caster, 83372, true);
        }
        else if (caster->GetShapeshiftForm() == FORM_TREE)
        {
            return SPELL_FAILED_NOT_SHAPESHIFT;
        }
        else if (caster->GetShapeshiftForm() == FORM_FLIGHT || caster->GetShapeshiftForm() == FORM_FLIGHT_EPIC)
        {
            return SPELL_FAILED_NOT_FLYING;
        }

        return SPELL_FAILED_DONT_REPORT;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_druid_wild_charge::CheckCast);
    }
};

// 47994 - Cleave (Felguard)
class spell_warl_cleave_mortalwound : public SpellScript
{
    PrepareSpellScript(spell_warl_cleave_mortalwound);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (target->IsPlayer() || (target->GetOwner() && target->GetOwner()->IsPlayer())) // Verifica se o alvo é um jogador ou o pet de um jogador
        {
            caster->CastSpell(target, 25646, true); // Mortal Wound (15% redução de cura, acumula 2x)
        }

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warl_cleave_mortalwound::CheckCast);
    }
};

// 83028 - Skull Bash
class spell_druid_skull_bash : public SpellScript
{
    PrepareSpellScript(spell_druid_skull_bash);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (caster && caster->HasUnitState(UNIT_STATE_ROOT))
        {
            if (target && caster->GetDistance(target) < 1.25f)
            {
                return SPELL_CAST_OK;
            }
            else
            {
                return SPELL_FAILED_OUT_OF_RANGE;
            }
        }
        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_druid_skull_bash::CheckCast);
    }
};

// 83253 - Custom - Druid Balance -3s Starfall cooldown on Buff proc
class spell_druid_balance_starfall_custom_cdreduction : public AuraScript
{
    PrepareAuraScript(spell_druid_balance_starfall_custom_cdreduction);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_STARFALL });
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_STARFALL, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_druid_balance_starfall_custom_cdreduction::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 16864 - Omen of Clarity - override no script (delete o spell_dru_omen_of_clarity do db/world/spell_script-names, e é adicionado o spell_dru_omen_of_clarity_module
class spell_dru_omen_of_clarity_module : public AuraScript
{
    PrepareAuraScript(spell_dru_omen_of_clarity_module);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
        {
            return true;
        }

        if (spellInfo->IsPassive())
        {
            return false;
        }

        /*
        // Druid Forms (tem chance de procar Clearcasting)
        if (spellInfo->Id == 1066 || spellInfo->Id == 768 || spellInfo->Id == 783 || spellInfo->Id == 24858 || spellInfo->Id == 33891 || spellInfo->Id == 9634 || spellInfo->Id == 5487)
        {
            return true;
        }*/

        // Exceções: Starfall (Rank 4). Buff = 53201, Damage = 53195, mini AoE = 53190. - Ordem de cima pra baixo em codigo tem prioridade, mesmo que tenha 3 spells com o icon do starfall no script abaixo, se tiver uma exceçaoaqui, as 2 nao funcionarão.
        if (spellInfo->Id == 53190)
        {
            return false;
        }

#include <unordered_set>

        // Balance Spell Icons
        const uint32 balanceIconIDsToInclude[] =
        {
            689, 174, 20, 109, 2258, 44, 220,   // Bark, Cyclone, Root, Faerie Fire, Force of Nature, Hibernate, Hurricane. - Force of Nature n proca
            62, 1771, 225, 168, 2854, 1485, 53, // Innervate, Insect Swarm, Moonfire, Nature's Grasp, Starfall, Starfire, Thorns.
            15, 263,                            // Typhoon Dazzle/Damage, Wrath.                                            Add Earth and Moon? Eclipse? PvP set proc?
        };

        // Feral Spell Icons
        const uint32 feralIconIDsToInclude[] =
        {
            473, 2852, 957, 262, 958, 959, 960,   // Bash, Berserk, ChallengingR, Claw, Cower, Dash, DemoRoar.              - Cower, demo roar, charge cat n procam
            961, 1559, 3930, 1680, 50, 201, 2246, // Enrage, ChargeBear, ChargeCat, Bite, FrenzRegen, Growl, Lacerate.
            1681, 2312, 261, 495, 103, 494, 1531, // Maim, MangleCatBear, Maul, Pounce, Prowl, Rake, Ravage.
            108, 2865, 147, 3707, 1562, 1181,     // Rip, SavageRoar, Shred, SurvivalInstinct, SwipeBearCat, TigerFury.     Add Savage Defense? Infected Wounds? Primal Fury?
        };

        // Resto Spell Icons
        const uint32 restoIconIDsToInclude[] =
        {
            265, 194, 2435, 962, 2101, 123, // Abolish, CurePoison, Gift, HealingTouch, Lifeblooom, Mark.
            112, 2863, 24, 197, 64, 236,    // NS, nourish, rebirth, regrow, rejuv, remove curse.
            2256, 1917, 100, 2864,          // revive, swiftmend, tranquility, wild growth.                                 Add living seed?
        };

        std::unordered_set<uint32> SpellIconsToProc;

        for (const uint32 iconID : balanceIconIDsToInclude)
        {
            SpellIconsToProc.insert(iconID);
        }
        for (const uint32 iconID : feralIconIDsToInclude)
        {
            SpellIconsToProc.insert(iconID);
        }
        for (const uint32 iconID : restoIconIDsToInclude)
        {
            SpellIconsToProc.insert(iconID);
        }
        if (SpellIconsToProc.find(spellInfo->SpellIconID) != SpellIconsToProc.end())
        {
            if (spellInfo->SpellFamilyName == SPELLFAMILY_DRUID) // Só tem chance de procar os icons acima se forem de spellfamily_druid
            {
                return true;
            }
        }
        // Retorna falso para todas as outras condições, impedindo o proc.
        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dru_omen_of_clarity_module::CheckProc);
    }
};

// -50334 - Berserk
class spell_dru_berserk_module : public SpellScript
{
    PrepareSpellScript(spell_dru_berserk_module);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            // Remove tiger fury / mangle(bear)
            const uint32 TigerFury[6] = { 5217, 6793 }; // 50212, 50213 (tiger fury ultimos ranks)
            const uint32 DireMaul[6] = { 33878, 33986, 33987, 48563, 48564 };

            // remove aura
            for (auto& i : TigerFury)
                caster->RemoveAurasDueToSpell(i);

            // reset dire bear maul cd
            for (auto& i : DireMaul)
                caster->ToPlayer()->RemoveSpellCooldown(i, true);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dru_berserk_module::HandleAfterCast);
    }
};

// 64380, 65941 - Shattering Throw
class spell_warr_shattering_throw_module : public SpellScript
{
    PrepareSpellScript(spell_warr_shattering_throw_module);

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        if (Unit* caster = GetCaster())
        {
            // Check if the caster has the spell 83296 (Glyph of Shattering Throw)
            if (caster->HasAura(83296))
            {
                // Do not remove shields, but still display "immune to damage" part
                return;
            }
        }

        // remove shields, will still display immune to damage part
        if (Unit* target = GetHitUnit())
            target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw_module::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 20154, 21084 - Seal of Righteousness - melee proc dummy (addition ${$MWS*(0.022*$AP+0.044*$SPH)} damage)
class spell_pal_seal_of_righteousness_module : public AuraScript
{
    PrepareAuraScript(spell_pal_seal_of_righteousness_module);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS });
    }

    // Old:
    /*bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetProcTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive() && !eventInfo.GetTriggerAuraSpell() && (damageInfo->GetDamage() || (eventInfo.GetHitMask() & PROC_EX_ABSORB));
    }
    */
    // novo - agora damage do seal dá dano mesmo se o target tiver com absorb
    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // xinef: skip divine storm self hit (dummy) and righteous vengeance (0x20000000=
        return eventInfo.GetActor() != eventInfo.GetProcTarget() && (!eventInfo.GetSpellInfo() || !eventInfo.GetSpellInfo()->SpellFamilyFlags.HasFlag(0x20000000));
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        float ap = GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK);
        int32 holy = GetTarget()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
        holy += eventInfo.GetProcTarget()->SpellBaseDamageBonusTaken(SPELL_SCHOOL_MASK_HOLY);

        // Xinef: Libram of Divine Purpose
        if (AuraEffect* aurEffPaladin = GetTarget()->GetDummyAuraEffect(SPELLFAMILY_PALADIN, 2025, EFFECT_0))
        {
            holy += aurEffPaladin->GetAmount();
        }

        int32 bp = std::max<int32>(0, int32((ap * 0.022f + 0.044f * holy) * GetTarget()->GetAttackTime(BASE_ATTACK) / 1000));
        GetTarget()->CastCustomSpell(SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS, SPELLVALUE_BASE_POINT0, bp, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pal_seal_of_righteousness_module::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pal_seal_of_righteousness_module::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 605 - Mind Control
class spell_pri_mind_control_module : public AuraScript
{
    PrepareAuraScript(spell_pri_mind_control_module);

    void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetTarget())
            {
                uint32 duration = static_cast<uint32>(GetDuration());
                caster->SetInCombatWith(target, duration);
                target->SetInCombatWith(caster, duration);
            }
        }
    }

    void HandleRemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetTarget())
            {
                caster->SetCombatTimer(5550);
                target->SetCombatTimer(5550);
            }
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_pri_mind_control_module::HandleApplyEffect, EFFECT_0, SPELL_AURA_MOD_POSSESS, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_pri_mind_control_module::HandleRemoveEffect, EFFECT_0, SPELL_AURA_MOD_POSSESS, AURA_EFFECT_HANDLE_REAL);
    }
};

// 83089 (Holy Priest PW:S remove slow)
class spell_gen_remove_slow_impairing_auras : public SpellScript
{
    PrepareSpellScript(spell_gen_remove_slow_impairing_auras);

    void HandleScriptEffect(SpellEffIndex /* effIndex */)
    {
        GetHitUnit()->RemoveSlowImpairingAuras(true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_remove_slow_impairing_auras::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};


class spell_pri_shadowfiend_scaling_module : public AuraScript
{
    PrepareAuraScript(spell_pri_shadowfiend_scaling_module);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 40% of resistance from owner and 35% of armor (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 30% of intellect and 65% of stamina (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), stat == STAT_STAMINA ? 80 : 30); // no AC é 65 : 30, aumentei 1 pouco pra aumentar o hp
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 333% of SP as AP - 35.7% of damage increase per hit
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            amount = CalculatePct(std::max<int32>(0, shadow), 300); // xinef: deacrased to 300, including 15% from self buff
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 30% of SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            amount = CalculatePct(std::max<int32>(0, shadow), 30);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, aurEff->GetAuraType(), true, SPELL_BLOCK_TYPE_POSITIVE);
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_ATTACK_POWER)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
        else if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
    }

    void Register() override
    {
        if (m_scriptSpellId != 35661)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling_module::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35661 || m_scriptSpellId == 35662)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling_module::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 35661)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling_module::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling_module::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_pri_shadowfiend_scaling_module::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 83252 - Item - Shaman Custom Tier Set or Item Bonus (-4s cooldown sec no Feral Spirit ao usar Stormstrike)
class spell_sha_item_enhance_custom_bonus : public AuraScript
{
    PrepareAuraScript(spell_sha_item_enhance_custom_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_FERAL_SPIRIT });
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_SHAMAN_FERAL_SPIRIT, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_item_enhance_custom_bonus::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 83241 - Lava Burst!
class spell_gen_lava_burst_cd_reset : public SpellScript
{
    PrepareSpellScript(spell_gen_lava_burst_cd_reset);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_LAVA_BURST });
    }

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (caster->HasSpellCooldown(SPELL_LAVA_BURST))
            caster->RemoveSpellCooldown(SPELL_LAVA_BURST, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_lava_burst_cd_reset::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};




// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

/*

spelleffects.cpp: - Wild Charge
    //1891: Disengage
    m_caster->JumpTo(speedxy, speedz, m_spellInfo->SpellFamilyName != SPELLFAMILY_HUNTER);
    mudar p
    m_caster->JumpTo(speedxy, speedz, m_spellInfo->SpellFamilyName != SPELLFAMILY_HUNTER && m_spellInfo->Id != 83372);

SpellInfo.cpp

em baixo de
                if (HasAttribute(SPELL_ATTR0_CU_PICKPOCKET))
                {
                    Creature const* targetCreature = unitTarget->ToCreature();
                    if (!targetCreature)
                        return SPELL_FAILED_BAD_TARGETS;

                    if (!LootTemplates_Pickpocketing.HaveLootFor(targetCreature->GetCreatureTemplate()->pickpocketLootId))
                        return SPELL_FAILED_TARGET_NO_POCKETS;
                }

                add


                // Glyph of Shattering Throw
                if (Id == 64382 && caster->HasAura(83296)) {
                    if (target->GetTypeId() == TYPEID_PLAYER || target->GetTypeId() == TYPEID_UNIT && target->ToUnit()->IsPet())
                        return SPELL_FAILED_TARGET_IS_PLAYER;
                }


unit.cpp:

void Unit::RemoveSlowImpairingAuras(bool withSnare)
{
    if (withSnare)
        RemoveAurasWithMechanic(1 << MECHANIC_SNARE);

    // Snares
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (aura->GetSpellInfo()->Mechanic == MECHANIC_SNARE)
        {
            RemoveAura(iter);
            continue;
        }

        // Xinef: turn off snare auras by setting amount to 0 :)
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            if (((1 << i) & iter->second->GetEffectMask()) && aura->GetSpellInfo()->Effects[i].Mechanic == MECHANIC_SNARE)
                aura->GetEffect(i)->ChangeAmount(0);

        ++iter;
    }
}
unit.h:
em baixo de  "void RemoveMovementImpairingAuras(bool withRoot);"
void RemoveSlowImpairingAuras(bool withSnare);

*/

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

/*
Scripts Spells Cataclysm (Cataclysm Preservation)
Fel Flame:

// 77799 - Fel Flame - Updated to 4.3.4
class spell_warl_fel_flame : public SpellScript
{
    void OnHitTarget(SpellEffIndex /*effIndex* / (Sem espaço dps do *) )
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        Aura* aura = target->GetAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION, caster->GetGUID());
        if (!aura)
            aura = target->GetAura(SPELL_WARLOCK_IMMOLATE, caster->GetGUID());

        if (!aura)
            return;

        int32 newDuration = aura->GetDuration() + GetSpellInfo()->Effects[EFFECT_1].CalcValue() * 1000;
        aura->SetDuration(std::min(newDuration, aura->GetMaxDuration()));
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_warl_fel_flame::OnHitTarget, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};


Soulswap:


// 86121 - Soul Swap
class spell_warl_soul_swap : public SpellScriptLoader
{
    public:
        spell_warl_soul_swap() : SpellScriptLoader("spell_warl_soul_swap") { }

        class spell_warl_soul_swap_SpellScript : public SpellScript
        {
            bool Validate(SpellInfo const* /*spellInfo* /) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_WARLOCK_GLYPH_OF_SOUL_SWAP,
                        SPELL_WARLOCK_SOUL_SWAP_CD_MARKER,
                        SPELL_WARLOCK_SOUL_SWAP_OVERRIDE
                    });
            }

            void HandleHit(SpellEffIndex /*effIndex* /)
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_OVERRIDE, true);
                GetHitUnit()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_DOT_MARKER, true);
            }

            void Register() override
            {
                OnEffectHitTarget.Register(&spell_warl_soul_swap_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warl_soul_swap_SpellScript();
        }
};

// 86211 - Soul Swap Override - Also acts as a dot container
class spell_warl_soul_swap_override : public SpellScriptLoader
{
public:
    static char constexpr const ScriptName[] = "spell_warl_soul_swap_override";

    spell_warl_soul_swap_override() : SpellScriptLoader(ScriptName) { }

    class spell_warl_soul_swap_override_AuraScript : public AuraScript
    {
        bool Load() override
        {
            _swapCaster = nullptr;
            return true;
        }

        //! Forced to, pure virtual functions must have a body when linking
        void Register() override { }

    public:
        void AddDot(uint32 id) { _dotList.push_back(id); }
        std::list<uint32> const GetDotList() const { return _dotList; }
        Unit* GetOriginalSwapSource() const { return _swapCaster; }
        void SetOriginalSwapSource(Unit* victim) { _swapCaster = victim; }

    private:
        std::list<uint32> _dotList;
        Unit* _swapCaster;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_soul_swap_override_AuraScript();
    }
};
char constexpr const spell_warl_soul_swap_override::ScriptName[];

//! Soul Swap Copy Spells - 92795 - Simply copies spell IDs.
class spell_warl_soul_swap_dot_marker : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_dot_marker() : SpellScriptLoader("spell_warl_soul_swap_dot_marker") { }

    class spell_warl_soul_swap_dot_marker_SpellScript : public SpellScript
    {
        typedef spell_warl_soul_swap_override::spell_warl_soul_swap_override_AuraScript SoulSwapOverrideAuraScript;

        void HandleHit(SpellEffIndex effIndex)
        {
            Unit* swapVictim = GetCaster();
            Unit* warlock = GetHitUnit();
            if (!warlock || !swapVictim)
                return;

            flag96 classMask = GetSpellInfo()->Effects[effIndex].SpellClassMask;

            Unit::AuraApplicationMap const& appliedAuras = swapVictim->GetAppliedAuras();
            SoulSwapOverrideAuraScript* swapSpellScript = nullptr;
            if (Aura* swapOverrideAura = warlock->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                swapSpellScript = swapOverrideAura->GetScript<SoulSwapOverrideAuraScript>(spell_warl_soul_swap_override::ScriptName);

            if (swapSpellScript == nullptr)
                return;

            for (Unit::AuraApplicationMap::const_iterator itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
            {
                SpellInfo const* spellProto = itr->second->GetBase()->GetSpellInfo();
                if (itr->second->GetBase()->GetCaster() == warlock)
                    if (spellProto->SpellFamilyName == SPELLFAMILY_WARLOCK && (spellProto->SpellFamilyFlags & classMask))
                        swapSpellScript->AddDot(itr->first);
            }

            swapSpellScript->SetOriginalSwapSource(swapVictim);
        }

        void Register() override
        {
            OnEffectHitTarget.Register(&spell_warl_soul_swap_dot_marker_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_swap_dot_marker_SpellScript();
    }
};

// 86213 - Soul Swap Exhale
class spell_warl_soul_swap_exhale : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_exhale() : SpellScriptLoader("spell_warl_soul_swap_exhale") { }

    class spell_warl_soul_swap_exhale_SpellScript : public SpellScript
    {
        typedef spell_warl_soul_swap_override::spell_warl_soul_swap_override_AuraScript SoulSwapOverrideAuraScript;

        bool Validate(SpellInfo const* /*spellInfo* /) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_WARLOCK_SOUL_SWAP_MOD_COST,
                    SPELL_WARLOCK_SOUL_SWAP_OVERRIDE
                });
        }

        SpellCastResult CheckCast()
        {
            Unit* currentTarget = GetExplTargetUnit();
            Unit* swapTarget = nullptr;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                if (SoulSwapOverrideAuraScript* swapScript = swapOverride->GetScript<SoulSwapOverrideAuraScript>(spell_warl_soul_swap_override::ScriptName))
                    swapTarget = swapScript->GetOriginalSwapSource();

            // Soul Swap Exhale can't be cast on the same target than Soul Swap
            if (swapTarget && currentTarget && swapTarget == currentTarget)
                return SPELL_FAILED_BAD_TARGETS;

            return SPELL_CAST_OK;
        }

        void OnEffectHit(SpellEffIndex /*effIndex* /)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_MOD_COST, true);
            bool hasGlyph = GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_SOUL_SWAP);

            std::list<uint32> dotList;
            Unit* swapSource = nullptr;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
            {
                SoulSwapOverrideAuraScript* swapScript = swapOverride->GetScript<SoulSwapOverrideAuraScript>(spell_warl_soul_swap_override::ScriptName);
                if (!swapScript)
                    return;
                dotList = swapScript->GetDotList();
                swapSource = swapScript->GetOriginalSwapSource();
            }

            if (dotList.empty())
                return;

            for (std::list<uint32>::const_iterator itr = dotList.begin(); itr != dotList.end(); ++itr)
            {
                GetCaster()->AddAura(*itr, GetHitUnit());
                if (!hasGlyph && swapSource)
                    swapSource->RemoveAurasDueToSpell(*itr);
            }

            // Remove Soul Swap Exhale buff
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE);

            if (hasGlyph) // Add a cooldown on Soul Swap if caster has the glyph
                GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_CD_MARKER, false);
        }

        void Register() override
        {
            OnCheckCast.Register(&spell_warl_soul_swap_exhale_SpellScript::CheckCast);
            OnEffectHitTarget.Register(&spell_warl_soul_swap_exhale_SpellScript::OnEffectHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_swap_exhale_SpellScript();
    }
};

*/




void AddSpellScriptsScripts()
{
    // Battleborn Realm (OnLogin / OnFirstLogin etc)
    new BattlebornRealm();

    // novos:
    RegisterSpellScript(spell_druid_glyph_omen_of_clarity);
    RegisterSpellScript(spell_druid_wild_charge);
    RegisterSpellScript(spell_druid_skull_bash);
    RegisterSpellScript(spell_druid_balance_starfall_custom_cdreduction);
    RegisterSpellScript(spell_gen_remove_slow_impairing_auras);
    RegisterSpellScript(spell_sha_item_enhance_custom_bonus);
    RegisterSpellScript(spell_gen_lava_burst_cd_reset);
    RegisterSpellScript(spell_warl_cleave_mortalwound);

    // mudanças em scripts atuais:
    RegisterSpellScript(spell_dru_omen_of_clarity_module);
    RegisterSpellScript(spell_dru_berserk_module);
    RegisterSpellScript(spell_warr_shattering_throw_module);
    RegisterSpellScript(spell_pal_seal_of_righteousness_module);
    RegisterSpellScript(spell_pri_mind_control_module);
    RegisterSpellScript(spell_pri_shadowfiend_scaling_module);

}
