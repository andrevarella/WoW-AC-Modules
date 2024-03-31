#include "ScriptMgr.h"
#include "Player.h"
#include "Pet.h"
#include "Config.h"
#include "Configuration/Config.h"
#include "Group.h"
#include "Unit.h"
#include "World.h"
#include "WorldPacket.h"

//#include "Formulas.h"
//#include "Chat.h"

/*
*/

bool EncounterResetCDsEnable = true;

enum CreaturesIds
{
    // * Raid - ICC
    MARROWGAR = 36612,
    LADYDEATHWHISPER = 36855,
    GUNSHIPALLIANCE = 37540, // the skybreaker
    GUNSHIPHORDE = 37215,    // orgrim's hammer
    SAURFANG = 37813,
    FESTERGUT = 36626,
    ROTFACE = 36627,
    PROFESSORPUTRICIDE = 36678,
    PRINCE1 = 37970,
    PRINCE2 = 37972,
    PRINCE3 = 37973,
    BLOODQUEEN = 1234,
    //VALITHRIA - no codigo do boss em /scripts cpp
    SINDRAGOSA = 36853,
    LICHKING = 36597,

    // * Raid - ruby sanctum
    SAVIANARAGEFIRE = 39747,
    BALTHARUS = 39751,
    GENERALZARITHRIAN = 39746,
    HALION1 = 39863, // de fora ou de dentro
    HALION2 = 40142, // de fora ou de dentro

    // * Raid - toc 25
    ICEHOWL = 34797,
    JARAXXUS = 34780,
    //Faction Champion's = cpp
    FJOLALIGHTBANE = 34497,
    EYDISDARKBANE = 34496,
    ANUBARAK = 34564,

    // * Raid - onyxia's lair
    ONYXIA = 10184,

    // * Raid - Ulduar - TODO!
    FLAMELEVIATHAN = 33113,
    IGNIS = 33118,
    RAZORSCALE = 33186,
    KOLOGARN = 32930,
    AURIAYA = 33515,
    GENERALVEZAXZ = 33271,
    XT002 = 33293,
    YOGGSARON = 33288,
    // Bosses that don't die and need to be added on their JustDied .cpp:
    // Thorim, Hodir, Freya, Mimiron, Algalon. Iron Council dies but it's 3 bosses so we need to reset only after the encounter ends through cpp aswell (boss_assembly_of_iron.cpp)

    // * Raid - Naxxramas - n testado
    PATCHWERK = 16028,
    GROBBULUS = 15931,
    GLUTH = 15932,
    THADDIUS = 15928,
    ANUBREKHAN = 15956,
    GRANDWIDOEFAERLINA = 15953,
    MAEXXNA = 15952,
    INSTRUCTORRAZUVIOUS = 16061,
    GOTHIK = 16060,
    //FORHOURSEMEN = boss_four_horsemen.cpp
    NOTHPLAGUEBRINGER = 15954,
    HEIGANUNCLEAN = 15936,
    LOATHEB = 16011,
    SAPPHIRON = 15989,
    KELTHUZAD = 15990,

    // * Raid - eye of eternity - n testado
    MALYGOS = 28859,

    // * Raid - obsidian sanctum
    SARTHARION = 28860,

    // * Raid - vault of archavon
    ARCHAVON = 31125,
    EMALON = 33993,
    KORALON = 35013,
    TORAVON = 38433,

    // TBC Raids:

    // * Raid - Sunwell Plateau:
    KALECGOS = 24850,      // boss original
    KALECGOSDEMON = 24892, // Sathrovarr the Corruptor
    BRUTALLUS = 24882,
    FELMYST = 25038,
    ENTROPIUS = 25840, // (M'uru)
    // eredar twins = boss_eredar_twins.cpp, Kil Jaeden e Illidan tbm ?

    // * Raid - Black Temple
    WARLORDNAJENTUS = 22887,
    SUPREMUS = 22898,
    TERONGOREFIEND = 22871,
    GURTOGG = 22948,
    MOTHERSHAHRAZ = 22947,
    ESSENCEOFSOULS = 23420,
    SHADEOFAKAMA = 22841,
    GATHIOSTHESHATTERER = 22949,
    LADYMALANDE = 22951,           
    VERASDARKSHADOW = 22952,         
    HIGHNETHERMANCERZEREVOR = 22950,

    // * Raid - Hyjal  - n testado  - diminuir waves ?
    RAGEWINTERCHILL = 17767,
    ANETHERON = 17808,
    KAZROGAL = 17888,
    AZGALOR = 17842,
    ARCHIMONDE = 17968,

    // * Raid - Zul'Aman - n testado
    NALORAKK = 23576,
    AKILZON = 23574,
    JANALAI = 23578,
    HALAZZI = 23577,
    HEXLORDMALACRASS = 24239,
    ZULJIN = 23863,

    // * Raid - The Eye - n testado
    ALAR = 19514,
    VOIDREAVER = 19516,
    ASTROMANCER = 18805,
    KAELTHAS = 19622,

    // * Raid - Serpent Shrine Cavern - n testado
    HYDROSS = 21216,
    LURKERBELOW = 21217,
    LEOTHERAS = 21215,
    FATHOMLORDKARATHRESS = 21214,
    MOROGRIM = 21213,
    LADYVASHJ = 21212,

    // * Raid - Karazhan - n testado
    MOROES = 15687,
    MAIDEN = 16457,
    CURATOR = 15691,
    TERESTIAN = 15688,
    SHADEOFARAN = 16524,
    NETHERSPITE = 15689,
    NIGHTBANE = 17225,
    PRINCEMALCHEZAR = 15690,
    //attumen     - boss_midnight.cpp
    //opera event - bosses_opera.cpp
    //chess event - boss_chess_event.cpp

    // * Raid - Gruul's Lair - n testado
    GRULLDRAGONKILLER = 19044,
    //high king maulgar = boss_high_king_maulgar.cpp

    // * Raid - Magtheridon Lair's - n testado
    MAGTHERIDON = 17257,

    // Vanilla Raids:

    // * Raid - Blackwing's Lair - n testado
    RAZORGORE = 12435,
    VAELASTRASZ = 13020,
    BROODLORDLASH = 12017,
    FIREMAW = 11983,
    EBONROC = 14601,
    FLAMEGOR = 11981,
    CHROMAGGUS = 14020,
    NEFARIAN = 11583,

    // * Raid - Zul Gurub -  high priestress jeklik, high priest venoxis, high priestess mar'li, gahz'ranka, high priestess arlokk, jin'do the hexxer, hakkar
    BROODLORDMANDOKIR = 11382,
    HIGHPRIESTTEKAL = 14509,

    // * Raid - Ahn'Qiraj -  the prophet skeram, the bug family, battleguard sartura, fankriss, viscidus, princess huhuran, twin emperors, ouro, c'thun

    // * Raid - Molten Core - n testado
    LUCIFRON = 12118,
    MAGMADAR = 11982,
    GEHENNAS = 12259,
    GARR = 12057,
    SHAZZRAH = 12264,
    BARONGEDDON = 12056,
    GOLEMAGG = 11988,
    SULFURON = 12098,
    MAJORDOMO = 12018,
    RAGNAROS = 11502,
};

std::set<uint32_t> WotlkBossesIds = { MARROWGAR, LADYDEATHWHISPER, GUNSHIPALLIANCE, GUNSHIPHORDE, SAURFANG, FESTERGUT, ROTFACE, PROFESSORPUTRICIDE, PRINCE1, PRINCE2, PRINCE3, BLOODQUEEN, SINDRAGOSA, LICHKING
    ,SAVIANARAGEFIRE, BALTHARUS, GENERALZARITHRIAN, HALION1, HALION2
    ,ICEHOWL, JARAXXUS, FJOLALIGHTBANE, EYDISDARKBANE, ANUBARAK
    ,ONYXIA
    ,FLAMELEVIATHAN, IGNIS, RAZORSCALE, KOLOGARN, AURIAYA, GENERALVEZAXZ, XT002, YOGGSARON
    ,PATCHWERK, GROBBULUS, GLUTH, THADDIUS, ANUBREKHAN, GRANDWIDOEFAERLINA, MAEXXNA, INSTRUCTORRAZUVIOUS, GOTHIK, NOTHPLAGUEBRINGER, HEIGANUNCLEAN, LOATHEB, SAPPHIRON, KELTHUZAD
    ,MALYGOS, SARTHARION, ARCHAVON, EMALON, KORALON, TORAVON };

std::set<uint32_t> TbcBossesIds = { KALECGOS, KALECGOSDEMON, BRUTALLUS, FELMYST, ENTROPIUS,
    WARLORDNAJENTUS, SUPREMUS, TERONGOREFIEND, GURTOGG, MOTHERSHAHRAZ, ESSENCEOFSOULS, SHADEOFAKAMA, GATHIOSTHESHATTERER, LADYMALANDE, VERASDARKSHADOW, HIGHNETHERMANCERZEREVOR,
    RAGEWINTERCHILL, ANETHERON, KAZROGAL, AZGALOR, ARCHIMONDE,
    NALORAKK, AKILZON, JANALAI, HALAZZI, HEXLORDMALACRASS, ZULJIN,
    ALAR, VOIDREAVER ,ASTROMANCER, KAELTHAS,
    HYDROSS, LURKERBELOW, LEOTHERAS, FATHOMLORDKARATHRESS, MOROGRIM, LADYVASHJ,
    MOROES, MAIDEN, CURATOR, TERESTIAN, SHADEOFARAN, NETHERSPITE, NIGHTBANE, PRINCEMALCHEZAR,
    GRULLDRAGONKILLER, MAGTHERIDON };

std::set<uint32_t> VanillaBossesIds = { RAZORGORE, VAELASTRASZ, BROODLORDLASH, FIREMAW, EBONROC, FLAMEGOR, CHROMAGGUS, NEFARIAN,
    BROODLORDMANDOKIR, HIGHPRIESTTEKAL,
    LUCIFRON, MAGMADAR, GEHENNAS, GARR, SHAZZRAH, BARONGEDDON, GOLEMAGG, SULFURON, MAJORDOMO, RAGNAROS };


class EncounterResetCDs : public PlayerScript
{
public:
    EncounterResetCDs() : PlayerScript("EncounterResetCDs") {}

    void OnCreatureKill(Player * player, Creature * killed)  //override
    {
        if (!killed || killed->IsPlayer())
            return;

        if (!IsBossNPCFromList(killed) || player->InArena() || player->InBattleground())
            return;

        if (sConfigMgr->GetOption<bool>("EncounterResetCDs.Enable", true))
        {
            TriggerResetPlayerCDs(player, killed);
        }
    }
    void OnCreatureKilledByPet(Player * player, Creature * killed) //override
    {
        // Fix Crash vs Players (nao proca vs pet de players tbm)
        if (!killed || killed->IsPlayer()) // Verifica se killed não é nulo e se não é um jogador / Adiciona verificação para garantir que killed não é um jogador
            return;

        if (!IsBossNPCFromList(killed) || player->InArena() || player->InBattleground())
            return;

        if (sConfigMgr->GetOption<bool>("EncounterResetCDs.Enable", true))
        {
            TriggerResetPlayerCDs(player, killed);
        }
    }

    void ResetPlayerCDs(Player* player, Creature* killed, const int groupsize)
    {
        // Remove auras e reseta cooldowns ao matar o boss
        player->ResetPlayerCDonDeath(true);
        player->RemoveAurasDueToSpell(57723); // Sated (Bloodlust)
        player->RemoveAurasDueToSpell(57724); // Exhaustion (Heroism)
        player->RemoveAurasDueToSpell(25771); // Forbearance
        player->RemoveAurasDueToSpell(41425); // Hipothermia
        player->RemoveAurasDueToSpell(66233); // Ardent Defender (Prot paladin)
        player->RemoveAurasDueToSpell(61987); // Avenging Wrath Marker (server side forbearance) - pala
        player->RemoveAurasDueToSpell(79501); // Custom - Forbearance Custom (visual only) - pala
        player->RemoveAurasDueToSpell(79500); // Custom - Cheated Death (Custom visual only) - Rogue
        player->RemoveAurasDueToSpell(79503); // Custom - Reincarnation - Shaman
        player->RemoveAurasDueToSpell(79504); // Custom - Nature's Guardian
    }

    void TriggerResetPlayerCDs(Player* player, Creature* killed)
    {
        const int groupsize = GetNumInGroup(player); // Determine if it was a raid beatdown

        if (player->GetGroup())
            player->GetGroup()->DoForAllMembers([&](Player* groupMember)
            {
                ResetPlayerCDs(groupMember, killed, groupsize);
            });
        else
            ResetPlayerCDs(player, killed, groupsize);
    }

    bool IsBossNPCFromList(Creature* creature)
    {
        return WotlkBossesIds.find(creature->GetEntry()) != WotlkBossesIds.end()
            || TbcBossesIds.find(creature->GetEntry()) != TbcBossesIds.end()
            || VanillaBossesIds.find(creature->GetEntry()) != VanillaBossesIds.end();
    }

    // Get the player's group size
    int GetNumInGroup(Player* player)
    {
        int numInGroup = 1;
        const Group *group = player->GetGroup();
        if (group)
        {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            numInGroup = groupMembers.size();
        }
        return numInGroup;
    }
};

void EncounterResetCDsScripts()
{
    new EncounterResetCDs();
}
