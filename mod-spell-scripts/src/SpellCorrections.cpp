#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "SpellAuraDefines.h"
#include "SpellAuras.h"
#include "Player.h"
#include "DBCStores.h"

class SpellCorrections : public GlobalScript
{
public:
    SpellCorrections() : GlobalScript("SpellCorrections") { }

    inline void ApplySpellFix(std::initializer_list<uint32> spellIds, void(*fix)(SpellInfo*))
    {
        for (uint32 spellId : spellIds)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (!spellInfo)
            {
                LOG_ERROR("sql.sql", "Spell info correction specified for non-existing spell {}", spellId);
                continue;
            }

            fix(const_cast<SpellInfo*>(spellInfo));
        }
    }

    void OnLoadSpellCustomAttr(SpellInfo* spellInfo) override
    {
        // em SpellInfoCorrections, remover 47859, // Warlock - Death Coil (Rank 5) para nao ter 'spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;'

        // todo: (talvez sim, talvez nao)
        // SPELL_ATTR5_IGNORE_AREA_EFFECT_PVP_CHECK add em spells AoE p n ligar PvP em elwynn/duel etc ?
        // aspects n saem qnd morre?
        // formas de druid n saem qnd morre/castaveis enqnt ta morto? (é um druid poha, faz sentido)
        // seals n saem qnd morre
        // armor de mage/lock
        // auras de pala (crusader eu acho q da pra castar qnd ta morto)
        // stances war/dk ?
        // mais flasks/elixir/foods ?

        // Feral Charge, Piercing Howl e Druid Demoralizing Roar e warr Demo Shout nao pode dar dodge ou miss 
        ApplySpellFix({ 45334, 12323, 48560, 47437, 25203, 25202 }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx |= SPELL_ATTR0_NO_ACTIVE_DEFENSE;
            spellInfo->AttributesEx3 |= SPELL_ATTR3_ALWAYS_HIT;
            spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
        });

        // Arena Dampening (-5% heal) (74410)
        // Dampening/Dementia depois de 8m (83026) || Dampening/Dementia a cada 8 seg (83027) || Dementia +1% Dmg/Heal/stack (41406)
        // Arena Prep Haste (83025) || Battleground Prep Haste (83008)
        ApplySpellFix({ 74410, 41406, 83026, 83027, 83025, 83008 }, [](SpellInfo* spellInfo)
        {
            spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_CHANGE_MAP;
        });

        /* - n funciona (Weakened Soul n stackar com diferentes casters)
        ApplySpellFix({ 6788 }, [](SpellInfo* spellInfo) // Weakened Soul
        {
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DOT_STACKING_RULE;
        });*/

        // Spells / Procs nao dao combat mais (supress_target_procs fixou o bug do Cower q msm com no_threat e 
        ApplySpellFix
        ({
            48542, // Revitalize (rdruid)
            12654, // Ignite dot
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx |= SPELL_ATTR1_NO_THREAT;               // Does not generate threat DESCRIPTION Also does not cause target to engage
            spellInfo->AttributesEx2 |= SPELL_ATTR2_NO_INITIAL_THREAD;      // Unknown attribute 22@Attr2 DESCRIPTION Ambush, Backstab, Cheap Shot, Death Grip, Garrote, Judgements, Mutilate, Pounce, Ravage, Shiv, Shred
            spellInfo->AttributesEx3 |= SPELL_ATTR3_SUPRESS_TARGET_PROCS;   // No initial aggro
            //spellInfo->AttributesEx3 |= SPELL_ATTR3_SUPRESS_CASTER_PROCS; // Cannot trigger procs (?? n sei pra q isso)
        });


        // Pra poderem usar sob efeito de BoP
        ApplySpellFix
        ({
             48266, // Blood presen -- add o prevention type none no patch.dbc nessas spells tambem
             48266, // Blood presen 
             48263, // frost presence
             48265, // uh presence
             45529, // blood tap
             20594, // stoneform
             26297, // berserking
             20572, // blood fury
             20589, // escape artist
             22842, // frenzied regen
             52610, // savage roar
             871,   // shield wall
             71,    // def stance
             2458,  // berserker stance
             2457,  // battle stance
             11305  // Sprint
        }, [](SpellInfo* spellInfo)
        {
             spellInfo->PreventionType = SPELL_PREVENTION_TYPE_NONE;
        });

        ApplySpellFix({
             69832, // Unstable Ooze Explosion
             69833, // Unstable Ooze Explosion
             69839, // Unstable Ooze Explosion
             71209, // Unstable Ooze Explosion
             73029, // Unstable Ooze Explosion
             73030, // Unstable Ooze Explosion
             69832, // Unstable Ooze Explosion
             69833, // Unstable Ooze Explosion
             69839, // Unstable Ooze Explosion
             71209, // Unstable Ooze Explosion
             73029, // Unstable Ooze Explosion
             73030  // Unstable Ooze Explosion
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
        });


        // Spells que nao dao miss
        ApplySpellFix({
            20549, // War Stomp
            49576, // Death Grip
            56222, // Taunt DK 
            6795,  // Taunt Bear Form
            5209,  // Taunt Bear Form AoE
            3600,  // Earthbind slow
            30449, // Spellsteal
            988,   // Dispel Magic
            32375, // Mass Dispel
            10890, // psychic Scream rank 4/4
            355    // Taunt Warrior
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx3 |= SPELL_ATTR3_ALWAYS_HIT;
        });

        // Spells que nao dao resist
        ApplySpellFix({
            64044, // psychic Horror
            15487, // Silence 
            605,   // Mind Control
            32996, // Shadow word Death r2
            48157, // Shadow word Death r3
            48158, // Shadow word Death r4
            8012,  // Purge
            51514, // Hex
            58799, // Frostbrand Weapon 
            59159, // Thunderstorm
            25457, // Flame Shock
            49232, // Flame Shock
            49233, // Flame Shock R9
            25464, // Frost Shock r5 
            49235, // Frost Shock r6
            49236, // Frost Shock r7
            60103, // Lava Lash
            31589, // Slow (mage arcane) 
            12826, // Polymorph
            44572, // Deep Freeze
            42945, // Blast Wave 
            42950, // Dragon Breath
            48819, // Consec 
            48806, // Hammer of Wrath
            25742, // Seal of Righteoussness Horda e Ally
            20187, // Judg of righteousness Horda e Ally 
            31803, // Seal of Vengeance DoT (ally)
            42463, // Seal of Vengeance 5 Stack damage
            31804, // Judgement of Vengeance
            53739, // Seal of Corruption (horda) 5 stack damage
            53733, // Judgement of Corruption
            53742, // Seal of Corruption DoT
            61411, // Shield of Righteousness
            53595, // Hammer (prot)
            48827, // Avenger's shield
            48817, // Holy Wrath
            57970, // Deadly Poison
            57981, // Anesthetic 
            5760,  // Mind Numbing
            57975, // Wound Poison
            3409,  // Crippling
            58180, // Infected Wounds r2
            58181, // Infected Wounds r3
            9853,  // Root R6 
            26989, // R7
            53308, // R8
            53312, // N grasp
            53313, // N Grasp Root
            27009, // N Grasp r7
            27010, // N grasp root dot
            18658, // Hibernate 
            16857, // FFF
            60089, // FFF dmg
            8921,  // Moonfire r1
            26988, // Mf 12
            48462, // Mf 13
            48463, // Mf r14 
            48467, // Hurricane
            48466, // Hurricane 2
            116,   // Frostbolt r1
            205,   // Fb r2
            6215,  // Fear wlock
            17928, // Howl r2 
            5484,  // Howl r1
            47847, // Sfury
            5138,  // Drain Mana
            27220, // Drain Life
            47857, // Drain Life
            22703, // Inferno stun 
            30546, // Sburn r8
            47826, // Sburn r9
            54053, // Shadowbite
            19647, // Spell Lock
            2139,  // Counterspell
            27217, // Drain Soul  
            47855, // Drain Mana
            48011, // Devour Magic
            27228, // Elements
            47865, // Elements 2
            47897, // Sflame
            47960, // Sflame DoT  
            11719, // Tongues
            18223, // Exhaustion
            27218, // Agony
            47863, // Agony 2/3
            47864, // Agony rank 3/3
            47812, // Corruption rank 9/r10 
            51209, // Hungering Cold
            45524, // Chains of Ice
            47528, // Mind Freeze
            48806, // Strangulate 
            71904  // Chaos Bane
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx4 |= SPELL_ATTR4_NO_CAST_LOG;
        });

        // Spells que nao dao miss e resist (caso tenha os 2 atributos, faz ignorar cloak of shadows, entao colocar apenas spells n essenciais tipo taunt, banish, etc 
        ApplySpellFix({
            28730, // Arcane Torrent racial 
            67485, // Taunt pala
            18647, // Banish 
            30910, // Curse of Doom rank 1
            47867, // Curse of Doom rank 2
            453,   // Mind Soothe 
            10909, // Mind Vision 
            57994, // Wind Shear
            73778  // Ice Pulse 25h  
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx3 |= SPELL_ATTR3_ALWAYS_HIT;
            spellInfo->AttributesEx4 |= SPELL_ATTR4_NO_CAST_LOG;
        });

        // Sem Delay Auto Attack ao castar essas spells (nao ativa o timer de auto attack)
        ApplySpellFix({
        48463, // Moonfire - 
        33786, // Cyc
        18658, // hibernate
        53308, // Roots
        53312, // natures grasp
        22812, // Bark
        48378, // healing touch
        48443, // Regrowth
        48441, // rejuv
        48451, // lifebloom
        50464, // nourish
        48469, // mark of the wild
        53307, // thorns 
        8921,  // MF rank1
        2893,  // abolish poison
        48470, // gift of the wild
        48477, // rebirth
        768,   // cat form
        9634,  // bear
        783,   // travel
        24858, // moonkin
        48468, // insect swarm
        33831, // force of nature
        53201, // starfall
        61384, // typhoon 
        53251, // wild growth
        61384, // typhoon
        51533, // Feral Spirit
        8012,  // purge
        49273, // healing wave
        49238, // lightning bolt
        49271, // chain lightning
        59159, // thunderstorm
        61301, // riptide
        49236, // frost shock
        51514, // hex
        59547, // gift of the naaru 
        2782,  // remove curse Druid
        19506, // trueshot aura
        5384,  // feign death
        48068, // renew
        48173, // desperate prayer
        48078, // holy nova
        552,   // abolish
        48066, // power word shield
        586,   // fade
        48158, // SWD
        48125, // SWPain
        48300, // devouring pain
        988,   // dispel magic
        6346,  // fear ward
        48168, // inner fire
        33206, // Pain Supp
        53007, // penance
        48089, // circle of healing 
        45529, // blood tap
        59514, // haunt
        17928, // howl
        6215,  // fear
        57946, // life tap
        1455,  // life tap rank 2
        47843, // UA
        47847, // shadowfury
        47241, // Metamorphosis
        47813, // corruption
        47864, // agony
        18223, // exhaustion
        47865, // elements
        11719, // tongues
        47860, // coil
        47857, // drain life
        5138,  // drain mana
        47855, // drain soul 
        47809, // sbolt
        61290, // shadowflame
        47815, // searing pain
        17847, // banish
        47889, // demon armor
        47893, // fel armor
        45529, // detect inv
        45529, // shadow ward
        691,   // summon felhunter
        48785, // flash of light
        48801, // exorcism
        53563, // beacon of light 
        1953,  // blink
        44781, // arcane barrage
        31589, // slow
        30449, // spellsteal
        12826, // poly
        2139,  // cs
        130,   // slow fall
        12043, // POM
        2139,  // cs
        42921, // arcane explosion
        42873, // fire blast
        42917, // frost nova
        42931, // cone of cold
        12472, // icy veins
        31687, // summon water ele
        42914, // ice lance
        43020, // mana shield
        55360, // living bomb
        42950, // dragons breath
        475, // dispel curse
        59542, // racial draenei 
        20594, // stoneform
        58984  // shadowmeld - add outras raciais purge etc
        }, [](SpellInfo* spellInfo)
        {
            spellInfo->AttributesEx6 |= SPELL_ATTR6_DOESNT_RESET_SWING_TIMER_IF_INSTANT;
        });

        switch (spellInfo->Id)
        {
        // Damage Shield (prot warr) - com chance de dar crit, mas acho q esse fix n funciona, e talvez não é necessario tbm ja tem chance de bater 60% a mais
        /*case 59653:
            spellInfo->SpellLevel = 0;
            spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
            break;
        // Cat Form - nao funciona aparentemente
        case 768:
            spellInfo->AttributesEx7 |= SPELL_ATTR7_RESET_SWING_TIMER_AT_SPELL_START;
            break;
        */

        /*
        // Entrapment fix
        ApplySpellFix({ 19185, 64803, 64804 }, [](SpellInfo* spellInfo)
        {
            spellInfo->Effects[EFFECT_0].TargetA = SpellImplicitTargetInfo(TARGET_DEST_TARGET_ENEMY);
            spellInfo->Effects[EFFECT_0].TargetB = SpellImplicitTargetInfo(TARGET_UNIT_DEST_AREA_ENEMY);
            spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(EFFECT_RADIUS_100_YARDS);  // EFFECT_RADIUS_100_YARDS     = 12     -      EFFECT_RADIUS_50_YARDS      = 27,
            
            // De acordo com as DBCs blizzlikes:
            //spellInfo->Effects[EFFECT_0].TargetA = SpellImplicitTargetInfo(TARGET_SRC_CASTER);
            //spellInfo->Effects[EFFECT_0].TargetB = SpellImplicitTargetInfo(TARGET_UNIT_SRC_AREA_ENEMY);

        });*/

        // Spell transformar em Caveira - allow to mount while transformed
        case 16591:
            spellInfo->Attributes &= ~SPELL_ATTR0_NO_IMMUNITIES;
            break;

        // Ultrasafe Toshley Station - reseta cd ao entrar em arena e o debuff continua ao entrar em arena (eu acho)
        case 36941:
        case 36897:
            spellInfo->AttributesEx4 |= SPELL_ATTR4_ALLOW_ENETRING_ARENA;
            spellInfo->AttributesEx4 |= SPELL_ATTR4_IGNORE_DEFAULT_ARENA_RESTRICTIONS;
            break;

        // Dispel UA Rank 5 (talvez todos ranks?)
        case 31117:
            spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            break;

        // cloak of shadows
        case 31224:
            spellInfo->Effects[EFFECT_0].BasePoints = -750;
            break;

        // Fish Feast/Well Fed nao sai após morrer
        case 57399:
            spellInfo->AttributesEx3 |= SPELL_ATTR3_ALLOW_AURA_WHILE_DEAD;
            break;

        // Magma totem (rank 7) - instant tick on use
        case 58736:
            spellInfo->AttributesEx5 |= SPELL_ATTR5_EXTRA_INITIAL_PERIOD;
            break;

        // hunter's mark - dá override no ApplySpellFix de SpellInfoCorrections.cpp (em spell_damage_class_magic)
        case 53338:
            spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
            spellInfo->AttributesEx3 |= SPELL_ATTR3_ALWAYS_HIT;
            break;
        /* Na core ta dispel_none, acho estranho isso
        // Combustion - override 
        case 11129:
            spellInfo->Dispel = DISPEL_MAGIC;
            break;*/

        // Flare Activation Speed - override
        case 1543:
            spellInfo->Speed = 10.0f;
            break;
        }
    }
};

void AddSpellCorrectionsScripts()
{
    new SpellCorrections();
}
