#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Chat.h"
#include "GameTime.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "InstanceScript.h"
#include "Pet.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SkillDiscovery.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Vehicle.h"
#include <array>
/// @todo: this import is not necessary for compilation and marked as unused by the IDE
//  however, for some reasons removing it would cause a damn linking issue
//  there is probably some underlying problem with imports which should properly addressed
//  see: https://github.com/azerothcore/azerothcore-wotlk/issues/9766
#include "GridNotifiersImpl.h"
#include "SpellMountScripts_loader.h"

enum Mounts
{
    SPELL_COLD_WEATHER_FLYING = 54197,

    // --------------------------------------------------------------------------------------- \\

    // Blazing Hippogryph
    SPELL_BLAZING_HIPPOGRYPH_100 = 84410, // NEW SPELL (NEED PATCH)
    SPELL_BLAZING_HIPPOGRYPH_150 = 74854,
    SPELL_BLAZING_HIPPOGRYPH_280 = 74855,

    // Mudanças em alguns scripts já existentes (precisa do patch)

    // X-53 Touring Rocket
    SPELL_X53_TOURING_ROCKET_100 = 84457, // NEW SPELL (NEED PATCH)
    SPELL_X53_TOURING_ROCKET_150 = 75957,
    SPELL_X53_TOURING_ROCKET_280 = 75972,
    SPELL_X53_TOURING_ROCKET_310 = 76154,

    // --------------------------------------------------------------------------------------- \\

    // spells novas(custom), precisa do patch!
    // spells vanilla/tbc/wotlk:

    // Onyxian
    SPELL_ONYXIAN_100 = 84412,
    SPELL_ONYXIAN_310 = 69395,

    // Ashes of Alar
    SPELL_ASHES_100 = 84414,
    SPELL_ASHES_310 = 40192,

    // Mimiron's
    SPELL_MIMIRON_100 = 84416,
    SPELL_MIMIRON_310 = 63796,

    // PvP S1 Glad
    SPELL_S1_100 = 84418,
    SPELL_S1_310 = 37015,

    // PvP S2 Glad
    SPELL_S2_100 = 84420,
    SPELL_S2_310 = 44744,

    // PvP S3 Glad
    SPELL_S3_100 = 84422,
    SPELL_S3_310 = 49193,

    // PvP S4 Glad
    SPELL_S4_100 = 84424,
    SPELL_S4_310 = 58615,

    // PvP S5 Glad
    SPELL_S5_100 = 84426,
    SPELL_S5_310 = 64927,

    // PvP S6 Glad
    SPELL_S6_100 = 84428,
    SPELL_S6_310 = 65439,

    // PvP S7 Glad
    SPELL_S7_100 = 84430,
    SPELL_S7_310 = 67336,

    // PvP S8 Glad
    SPELL_S8_100 = 84432,
    SPELL_S8_310 = 71810,

    // Red Dragonhawk
    SPELL_REDDRAGONHAWK_100 = 84434,
    SPELL_REDDRAGONHAWK_310 = 61997,

    // Blue Dragonhawk
    SPELL_BLUEDRAGONHAWK_100 = 84436,
    SPELL_BLUEDRAGONHAWK_310 = 61996,

    // Sunreaver Dragonhawk
    SPELL_SUNREAVERDRAG_100 = 84438,
    SPELL_SUNREAVERDRAG_280 = 66088,

    // Silver Covenant Hippogryph
    SPELL_SILVERCOVENANT_100 = 84440,
    SPELL_SILVERCOVENANT_280 = 66087,

    // Argent Hippogryph
    SPELL_ARGENTHIPPO_100 = 84442,
    SPELL_ARGENTHIPPO_280 = 63844,

    // Cenarion War Hippogryph
    SPELL_CENARIONWAR_100 = 84444,
    SPELL_CENARIONWAR_280 = 43927,

    // Time-Lost Proto Drake
    SPELL_TIMELOST_100 = 84446,
    SPELL_TIMELOST_280 = 60002,

    // Blue Proto Drake
    SPELL_BLUEPROTO_100 = 84448,
    SPELL_BLUEPROTO_280 = 59996,

    // Red Proto Drake
    SPELL_REDPROTO_100 = 84450,
    SPELL_REDPROTO_280 = 59961,

    // Ironbound Proto Drake
    SPELL_IRONBOUNDPROTODRAKE_100 = 84452,
    SPELL_IRONBOUNDPROTODRAKE_310 = 63956,

    // Rusted Proto Drake
    SPELL_RUSTEDPROTODRAKE_100 = 84454,
    SPELL_RUSTEDPROTODRAKE_310 = 63963,

    // Violet Proto Drake
    SPELL_VIOLETPROTODRAKE_100 = 84459,
    SPELL_VIOLETPROTODRAKE_310 = 60024,

    // Plagued Proto Drake
    SPELL_PLAGUEDPROTODRAKE_100 = 84461,
    SPELL_PLAGUEDPROTODRAKE_310 = 60021,

    // Black Proto Drake
    SPELL_BLACKPROTODRAKE_100 = 84463,
    SPELL_BLACKPROTODRAKE_310 = 59976,

    // Green Proto Drake
    SPELL_GREENPROTODRAKE_100 = 84465,
    SPELL_GREENPROTODRAKE_280 = 61294,

    // Albino Drake
    SPELL_ALBINODRAKE_100 = 84467,
    SPELL_ALBINODRAKE_280 = 60025,

    // Bronze Drake
    SPELL_BRONZEDRAKE_100 = 84469,
    SPELL_BRONZEDRAKE_280 = 59569,

    // Azure Drake
    SPELL_AZUREDRAKE_100 = 84471,
    SPELL_AZUREDRAKE_280 = 59567,

    // Twilight Purple Drake
    SPELL_TWILIGHTPURPLEDRAKE_100 = 84473,
    SPELL_TWILIGHTPURPLEDRAKE_280 = 84474,

    // Twilight Drake
    SPELL_TWILIGHTDRAKE_100 = 84477,
    SPELL_TWILIGHTDRAKE_280 = 59571,

    // Black Drake
    SPELL_BLACKDRAKE_100 = 84479,
    SPELL_BLACKDRAKE_280 = 59650,

    // Red Drake
    SPELL_REDDRAKE_100 = 84481,
    SPELL_REDDRAKE_280 = 59570,

    // Blue Drake
    SPELL_BLUEDRAKE_100 = 84483,
    SPELL_BLUEDRAKE_280 = 59568,

    // Onyx Netherwing Drake
    SPELL_NETHERWING1_100 = 84485,
    SPELL_NETHERWING1_280 = 41513,

    // Purple Netherwing 2 Drake
    SPELL_NETHERWING2_100 = 84487,
    SPELL_NETHERWING2_280 = 41516,

    // Cobalt Netherwing 3 Drake
    SPELL_NETHERWING3_100 = 84489,
    SPELL_NETHERWING3_280 = 41515,

    // Violet Netherwing 4 Drake
    SPELL_NETHERWING4_100 = 84491,
    SPELL_NETHERWING4_280 = 41518,

    // Veriridan Netherwing 5 Drake
    SPELL_NETHERWING5_100 = 84493,
    SPELL_NETHERWING5_280 = 41517,

    // Azure Netherwing 6 Drake
    SPELL_NETHERWING6_100 = 84495,
    SPELL_NETHERWING6_280 = 41514,

    // Turbo-Charged Flying Machine
    SPELL_TURBOCHARGED_100 = 84497,
    SPELL_TURBOCHARGED_280 = 44151,

    // Magnificent Flying Carpet
    SPELL_MAGNIFICENTCARPET_100 = 84499,
    SPELL_MAGNIFICENTCARPET_280 = 61309,

    // Frosty Flying Carpet
    SPELL_FROSTYFLYING_100 = 84501,
    SPELL_FROSTYFLYING_280 = 75596,

    // Flying Carpet
    SPELL_FLYINGCARPET_100 = 84503,
    SPELL_FLYINGCARPET_280 = 61451,

    // Purple Riding Nether-Ray
    SPELL_NETHERRAY1_100 = 84505,
    SPELL_NETHERRAY1_280 = 39801,

    // Blue Riding Nether-Ray
    SPELL_NETHERRAY2_100 = 84507,
    SPELL_NETHERRAY2_280 = 39803,

    // Silver Riding Nether-Ray
    SPELL_NETHERRAY3_100 = 84509,
    SPELL_NETHERRAY3_280 = 39802,

    // Red Riding Nether-Ray
    SPELL_NETHERRAY4_100 = 84511,
    SPELL_NETHERRAY4_280 = 39800,

    // Green Riding Nether-Ray
    SPELL_NETHERRAY5_100 = 84513,
    SPELL_NETHERRAY5_280 = 39798,

    // X-51
    SPELL_X51_100 = 84406,
    SPELL_X51_280 = 46197,

    // X-51 Xtreme
    SPELL_X51XT_100 = 84408,
    SPELL_X51XT_280 = 46199,


    // --------------------------------------------------------------------------------------- \\
    //         MOUNTS Cata/MoP/WoD/Legion/BFA/Shadowlands/Dragonflight abaixo:
    // --------------------------------------------------------------------------------------- \\

    // Flying Sabers // ------- \\
    // Ban-lu
    SPELL_BAN_LU_100 = 84635,
    SPELL_BAN_LU_310 = 84636,

    // Luminous Starseeker
    SPELL_LUMINOUS_STARSEEKER_100 = 84638,
    SPELL_LUMINOUS_STARSEEKER_310 = 84639,

    // Mystic runesaber
    SPELL_MYSTIC_RUNESABER_100 = 84650,
    SPELL_MYSTIC_RUNESABER_310 = 84651,

    // Arcanist's manasaber
    SPELL_ARCANIST_MANASABER_100 = 84641,
    SPELL_ARCANIST_MANASABER_310 = 84642,

    // Silverwind larion
    SPELL_SILVERWIND_LARION_100 = 84644,
    SPELL_SILVERWIND_LARION_310 = 84645,

    // Highwind darkmane
    SPELL_HIGHWIND_DARKMANE_100 = 84647,
    SPELL_HIGHWIND_DARKMANE_310 = 84648,

    // Wen lo
    SPELL_WEN_LO_100 = 84653,
    SPELL_WEN_LO_310 = 84654,

    // Ash'adar
    SPELL_ASH_ADAR_100 = 85100,
    SPELL_ASH_ADAR_310 = 85101,

    // Flying Horses // ------- \\

    // Ghastly Charger
    SPELL_GHASTLYCHARGER_100 = 84516,
    SPELL_GHASTLYCHARGER_310 = 84517,

    // Tempestuous Skystallion
    SPELL_TEMPESSKYTAL_100 = 84710,
    SPELL_TEMPESSKYTAL_310 = 84711,

    // Swift Windsteed
    SPELL_SWIFTWINDST_100 = 85094,
    SPELL_SWIFTWINDST_310 = 85095,

    // Flying Birds // ------- \\

    //Clutch of jikun
    SPELL_CLUTCH_OF_JIKUN_100 = 84656,
    SPELL_CLUTCH_OF_JIKUN_310 = 84657,

    //Clutch of hali
    SPELL_CLUTCH_OF_HALI_100 = 84659,
    SPELL_CLUTCH_OF_HALI_310 = 84660,

    //Crimson phoenix
    SPELL_CRIMSON_PHOENIX_100 = 84662,
    SPELL_CRIMSON_PHOENIX_310 = 84663,

    //Ashen phoenix
    SPELL_ASHEN_PHOENIX_100 = 84671,
    SPELL_ASHEN_PHOENIX_310 = 84672,

    //Violet phoenix
    SPELL_VIOLET_PHOENIX_100 = 84713,
    SPELL_VIOLET_PHOENIX_310 = 84714,

    //Flameward gryph
    SPELL_FLAMEWARD_GRYPH_100 = 84665,
    SPELL_FLAMEWARD_GRYPH_310 = 84666,

    //Teldrassil gryph
    SPELL_TELDRASSIL_GRYPH_100 = 84668,
    SPELL_TELDRASSIL_GRYPH_310 = 84669,

    //Stormcrow
    SPELL_STORMCROW_100 = 84677,
    SPELL_STORMCROW_310 = 84678,

    //Solar spirehawk
    SPELL_SOLAR_SPIREHAWK_100 = 84674,
    SPELL_SOLAR_SPIREHAWK_310 = 84675,

    //Sharkbait
    SPELL_SHARKBAIT_100 = 84683,
    SPELL_SHARKBAIT_310 = 84684,

    //Squawks
    SPELL_SQUAWKS_100 = 84686,
    SPELL_SQUAWKS_310 = 84687,

    //Quawks
    SPELL_QUAWKS_100 = 84692,
    SPELL_QUAWKS_310 = 84693,

    //Blu
    SPELL_BLU_100 = 84689,
    SPELL_BLU_310 = 84690,

    //Murderous omen
    SPELL_MURDEROUS_OMEN_100 = 84695,
    SPELL_MURDEROUS_OMEN_310 = 84696,

    //Spectral phoenix
    SPELL_SPECTRAL_PHOENIX_100 = 84707,
    SPELL_SPECTRAL_PHOENIX_310 = 84708,

    //Dark phoenix
    SPELL_DARK_PHOENIX_100 = 84704,
    SPELL_DARK_PHOENIX_310 = 84705,

    //Sapphire skyblazer
    SPELL_SAPPHIRE_SKYBLAZER_100 = 84716,
    SPELL_SAPPHIRE_SKYBLAZER_310 = 84717,

    //Amber skyblazer
    SPELL_AMBER_SKYBLAZER_100 = 84719,
    SPELL_AMBER_SKYBLAZER_310 = 84720,

    //Armored red dragonhawk
    SPELL_ARMORED_RED_DRAGONHAWK_100 = 84701,
    SPELL_ARMORED_RED_DRAGONHAWK_310 = 84702,

    //Armored blue dragonhawk
    SPELL_ARMORED_BLUE_DRAGONHAWK_100 = 84698,
    SPELL_ARMORED_BLUE_DRAGONHAWK_310 = 84699,

    //Grand armored gryphon
    SPELL_GRAND_ARMORED_GRYPHON_100 = 85077,
    SPELL_GRAND_ARMORED_GRYPHON_310 = 85078,

    //Grand armored wyvern :
    SPELL_GRAND_ARMORED_WYVERN_100 = 85080,
    SPELL_GRAND_ARMORED_WYVERN_310 = 85081,

    // Rockets // ------- \\
    // Orgrimmar Interceptor
    SPELL_ORGRIMMAR_INTERCEPTOR_100 = 84722,
    SPELL_ORGRIMMAR_INTERCEPTOR_310 = 84723,

    // stormwind skychaser
    SPELL_STORMWIND_SKYCHASER_100 = 84725,
    SPELL_STORMWIND_SKYCHASER_310 = 84726,

    // the dreadwake
    SPELL_DREADWAKE_100 = 84728,
    SPELL_DREADWAKE_310 = 84729,

    // darkmoon dirigible
    SPELL_DARKMOON_DIRIGIBLE_100 = 84731,
    SPELL_DARKMOON_DIRIGIBLE_310 = 84732,

    // aerial unit r21
    SPELL_AERIAL_UNIT_R21_100 = 84743,
    SPELL_AERIAL_UNIT_R21_310 = 84744,

    // explorer jungle hopper
    SPELL_EXPLORER_JUNGLE_HOPPER_100 = 84740,
    SPELL_EXPLORER_JUNGLE_HOPPER_310 = 84741,

    // depleted kyparium rocket
    SPELL_DEPLETED_KYPARIUM_ROCKET_100 = 84734,
    SPELL_DEPLETED_KYPARIUM_ROCKET_310 = 84735,

    // geosync world spinner
    SPELL_GEOSYNC_WORLD_SPINNER_100 = 84737,
    SPELL_GEOSYNC_WORLD_SPINNER_310 = 84738,

    // jade pandaren kite string
    SPELL_JADE_PANDAREN_KITE_STRING_100 = 84997,
    SPELL_JADE_PANDAREN_KITE_STRING_310 = 84998,

    // pandaren kite alliance
    SPELL_PANDAREN_KITE_ALLIANCE_100 = 85004,
    SPELL_PANDAREN_KITE_ALLIANCE_310 = 85005,

    // pandaren kite horde
    SPELL_PANDAREN_KITE_HORDE_100 = 85001,
    SPELL_PANDAREN_KITE_HORDE_310 = 85002,

    // blue flying cloud
    SPELL_BLUE_FLYING_CLOUD_100 = 84771,
    SPELL_BLUE_FLYING_CLOUD_310 = 84772,

    // red flying cloud
    SPELL_RED_FLYING_CLOUD_100 = 84767,
    SPELL_RED_FLYING_CLOUD_310 = 84768,

    // gold flying cloud
    SPELL_GOLD_FLYING_CLOUD_100 = 84774,
    SPELL_GOLD_FLYING_CLOUD_310 = 84775,

    // archmage prism discFrost
    SPELL_ARCHMAGE_PRISM_DISCFROST_100 = 84764,
    SPELL_ARCHMAGE_PRISM_DISCFROST_310 = 84765,

    // archmage prism discFire
    SPELL_ARCHMAGE_PRISM_DISCFIRE_100 = 84761,
    SPELL_ARCHMAGE_PRISM_DISCFIRE_310 = 84762,

    // archmage prism discArcane
    SPELL_ARCHMAGE_PRISM_DISCARCANE_100 = 84758,
    SPELL_ARCHMAGE_PRISM_DISCARCANE_310 = 84759,

    // Outros // ------- \\

    // HIVEMIND
    SPELL_THE_HIVEMIND_100 = 84801,
    SPELL_THE_HIVEMIND_310 = 84802,

    // FATHOM DWELLER
    SPELL_FATHOM_DWELLER_100 = 84804,
    SPELL_FATHOM_DWELLER_310 = 84805,

    // SHAMAN MOUNT ENHANCE
    SPELL_SHAMAN_MOUNT_ENHANCE_100 = 84536,
    SPELL_SHAMAN_MOUNT_ENHANCE_310 = 84537,

    // SHAMAN MOUNT BLUE
    SPELL_SHAMAN_MOUNT_BLUE_100 = 84781,
    SPELL_SHAMAN_MOUNT_BLUE_310 = 84782,

    //SHAMAN MOUINT PURPLE
    SPELL_SHAMAN_MOUNT_PURPLE_100 = 84784,
    SPELL_SHAMAN_MOUNT_PURPLE_310 = 84785,

    // SHAMAN MOUNT SAND
    SPELL_SHAMAN_MOUNT_SAND_100 = 84777,
    SPELL_SHAMAN_MOUNT_SAND_310 = 84778,

    // IRON SKYREAVER
    SPELL_IRON_SKYREAVER_100 = 84787,
    SPELL_IRON_SKYREAVER_310 = 84788,

    //GRUSOME FLAYEDWING
    SPELL_GRUESOME_FLAYEDWING_100 = 84791,
    SPELL_GRUESOME_FLAYEDWING_310 = 84792,

    //SILVERTIP DREDWING
    SPELL_SILVERTIP_DREDWING_100 = 84794,
    SPELL_SILVERTIP_DREDWING_310 = 84795,

    // UNDERCITY PLAGUEBAT
    SPELL_UNDERCITY_PLAGUEBAT_100 = 85086,
    SPELL_UNDERCITY_PLAGUEBAT_310 = 85087,

    // HARVESTER DREDWING
    SPELL_HARVESTER_DREDWING_100 = 84797,
    SPELL_HARVESTER_DREDWING_310 = 84798,

    // ARMORED BLOODWING
    SPELL_ARMORED_BLOODWING_100 = 85083,
    SPELL_ARMORED_BLOODWING_310 = 85084,

    // Dragao Chines // ------- \\

    // Heart of the Aspects
    SPELL_HEART_OF_THE_ASPECTS_100 = 84827,
    SPELL_HEART_OF_THE_ASPECTS_310 = 84828,

    // Abyss Worm
    SPELL_ABYSS_WORM_100 = 84831,
    SPELL_ABYSS_WORM_310 = 84832,

    // Riddler Mind - Worm
    SPELL_RIDDLER_MIND_WORM_100 = 84834,
    SPELL_RIDDLER_MIND_WORM_310 = 84835,

    // Nazjatar Blood Serpent
    SPELL_NAZJATAR_BLOOD_SERPENT_100 = 84837,
    SPELL_NAZJATAR_BLOOD_SERPENT_310 = 84838,

    // Magenta Cloud Serpent
    SPELL_MAGENTA_CLOUD_SERPENT_100 = 84824,
    SPELL_MAGENTA_CLOUD_SERPENT_310 = 84825,

    // Nether Gorged Greatwyrm
    SPELL_NETHER_GORGED_GREATWYRM_100 = 84841,
    SPELL_NETHER_GORGED_GREATWYRM_310 = 84842,

    // Nether Gorged Greatwyrm Black
    SPELL_NETHER_GORGED_GREATWYRM_BLACK_100 = 84847,
    SPELL_NETHER_GORGED_GREATWYRM_BLACK_310 = 84848,

    // Nether Gorged Greatwyrm Silver
    SPELL_NETHER_GORGED_GREATWYRM_SILVER_100 = 84844,
    SPELL_NETHER_GORGED_GREATWYRM_SILVER_310 = 84845,

    // Nether Gorged Greatwyrm Purple
    SPELL_NETHER_GORGED_GREATWYRM_PURPLE_100 = 84851,
    SPELL_NETHER_GORGED_GREATWYRM_PURPLE_310 = 84852,

    // Dragoes // ------- \\

    // steamscale incinerator
    SPELL_STEAMSCALE_INCINERATOR_100 = 84854,
    SPELL_STEAMSCALE_INCINERATOR_310 = 84855,

    // tangled dreamweaver
    SPELL_TANGLED_DREAMWEAVER_100 = 84857,
    SPELL_TANGLED_DREAMWEAVER_310 = 84858,

    // battlelord bloodthirsty 1
    SPELL_BATTLELORD_BLOODTHIRSTY_1_100 = 84861,
    SPELL_BATTLELORD_BLOODTHIRSTY_1_310 = 84862,

    // battlelord bloodthirsty 2
    SPELL_BATTLELORD_BLOODTHIRSTY_2_100 = 84864,
    SPELL_BATTLELORD_BLOODTHIRSTY_2_310 = 84865,

    // battlelord bloodthirsty 3
    SPELL_BATTLELORD_BLOODTHIRSTY_3_100 = 84867,
    SPELL_BATTLELORD_BLOODTHIRSTY_3_310 = 84868,

    // red galakras
    SPELL_RED_GALAKRAS_100 = 84871,
    SPELL_RED_GALAKRAS_310 = 84872,

    // blue galakras
    SPELL_BLUE_GALAKRAS_100 = 84874,
    SPELL_BLUE_GALAKRAS_310 = 84875,

    // black galakras
    SPELL_BLACK_GALAKRAS_100 = 84877,
    SPELL_BLACK_GALAKRAS_310 = 84878,

    // razorscale galakras
    SPELL_RAZORSCALE_GALAKRAS_100 = 84881,
    SPELL_RAZORSCALE_GALAKRAS_310 = 84882,

    // rusted galakras
    SPELL_RUSTED_GALAKRAS_100 = 84884,
    SPELL_RUSTED_GALAKRAS_310 = 84885,

    // black korkron protodrake
    SPELL_BLACK_KORKRON_PROTO_DRAKE_100 = 84887,
    SPELL_BLACK_KORKRON_PROTO_DRAKE_310 = 84888,

    // spawn of galakras
    SPELL_SPAWN_OF_GALAKRAS_100 = 84891,
    SPELL_SPAWN_OF_GALAKRAS_310 = 84892,

    // time lost galakras
    SPELL_TIME_LOST_GALAKRAS_100 = 84894,
    SPELL_TIME_LOST_GALAKRAS_310 = 84895,

    // green galakras
    SPELL_GREEN_GALAKRAS_100 = 84897,
    SPELL_GREEN_GALAKRAS_310 = 84898,

    // violet galakras
    SPELL_VIOLET_GALAKRAS_100 = 84904,
    SPELL_VIOLET_GALAKRAS_310 = 84905,

    // plagued galakras
    SPELL_PLAGUED_GALAKRAS_100 = 84901,
    SPELL_PLAGUED_GALAKRAS_310 = 84902,

    // mottled drake
    SPELL_MOTTLED_DRAKE_100 = 84911,
    SPELL_MOTTLED_DRAKE_310 = 84912,

    // twilight avenger
    SPELL_TWILIGHT_AVENGER_100 = 84914,
    SPELL_TWILIGHT_AVENGER_310 = 84915,

    // emerald drake
    SPELL_EMERALD_DRAKE_100 = 84917,
    SPELL_EMERALD_DRAKE_310 = 84918,

    // enchanted fey dragon
    SPELL_ENCHANTED_FEY_DRAGON_100 = 84921,
    SPELL_ENCHANTED_FEY_DRAGON_310 = 84922,

    // uncorrupted voidwing
    SPELL_UNCORRUPTED_VOIDWING_100 = 84924,
    SPELL_UNCORRUPTED_VOIDWING_310 = 84925,

    // void dragon mount
    SPELL_VOID_DRAGON_MOUNT_100 = 84927,
    SPELL_VOID_DRAGON_MOUNT_310 = 84928,

    // valarjar stormwing
    SPELL_VALARJAR_STORMWING_100 = 84954,
    SPELL_VALARJAR_STORMWING_310 = 84955,

    // island thunderscale
    SPELL_ISLAND_THUNDERSCALE_100 = 84937,
    SPELL_ISLAND_THUNDERSCALE_310 = 84938,

    // teal stormdragon
    SPELL_TEAL_STORMDRAGON_100 = 84100,
    SPELL_TEAL_STORMDRAGON_310 = 84101,

    // deathlord vilebrood green
    SPELL_DEATHLORD_VILEBROOD_GREEN_100 = 84957,
    SPELL_DEATHLORD_VILEBROOD_GREEN_310 = 84958,

    // deathlord vilebrood blue
    SPELL_DEATHLORD_VILEBROOD_BLUE_100 = 84961,
    SPELL_DEATHLORD_VILEBROOD_BLUE_310 = 84962,

    // deathlord vilebrood red
    SPELL_DEATHLORD_VILEBROOD_RED_100 = 84964,
    SPELL_DEATHLORD_VILEBROOD_RED_310 = 84965,

    // tarecgosa
    SPELL_TARECGOSA_100 = 84967,
    SPELL_TARECGOSA_310 = 84968,

    // drake of east wind
    SPELL_DRAKE_OF_EAST_WIND_100 = 84971,
    SPELL_DRAKE_OF_EAST_WIND_310 = 84972,

    // drake of south wind
    SPELL_DRAKE_OF_SOUTH_WIND_100 = 84974,
    SPELL_DRAKE_OF_SOUTH_WIND_310 = 84975,

    // drake of four winds
    SPELL_DRAKE_OF_FOUR_WINDS_100 = 84977,
    SPELL_DRAKE_OF_FOUR_WINDS_310 = 84978,

    // drake of north wind
    SPELL_DRAKE_OF_NORTH_WIND_100 = 84981,
    SPELL_DRAKE_OF_NORTH_WIND_310 = 84982,

    // sylverian dreamer
    SPELL_SYLVERIAN_DREAMER_100 = 84984,
    SPELL_SYLVERIAN_DREAMER_310 = 84985,

    // sylverian dreamer green
    SPELL_SYLVERIAN_DREAMER_GREEN_100 = 84987,
    SPELL_SYLVERIAN_DREAMER_GREEN_310 = 84988,

    // sylverian dreamer red
    SPELL_SYLVERIAN_DREAMER_RED_100 = 84991,
    SPELL_SYLVERIAN_DREAMER_RED_310 = 84992,

    // sylverian dreamer orange
    SPELL_SYLVERIAN_DREAMER_ORANGE_100 = 84994,
    SPELL_SYLVERIAN_DREAMER_ORANGE_310 = 84995,

    // PvP Mounts // ------- \\

    // Custom Malevolent Cloud Serp
    SPELL_CUSTOM_MALEVOLENT_CLOUD_SERP_100 = 85017,
    SPELL_CUSTOM_MALEVOLENT_CLOUD_SERP_310 = 85018,

    // Custom Tyrannical Cloud Serp
    SPELL_CUSTOM_TYRANNICAL_CLOUD_SERP_100 = 85024,
    SPELL_CUSTOM_TYRANNICAL_CLOUD_SERP_310 = 85025,

    // Custom Violet Gladiator Serp
    SPELL_CUSTOM_VIOLET_GLADIATOR_SERP_100 = 85037,
    SPELL_CUSTOM_VIOLET_GLADIATOR_SERP_310 = 85038,

    // Custom Green Gladiator Serp
    SPELL_CUSTOM_GREEN_GLADIATOR_SERP_100 = 85034,
    SPELL_CUSTOM_GREEN_GLADIATOR_SERP_310 = 85035,

    // custom vindictive glad
    SPELL_CUSTOM_VINDICTIVE_GLAD_100 = 85041,
    SPELL_CUSTOM_VINDICTIVE_GLAD_310 = 85042,

    // custom fearless glad
    SPELL_CUSTOM_FEARLESS_GLAD_100 = 85044,
    SPELL_CUSTOM_FEARLESS_GLAD_310 = 85045,

    // custom cruel glad
    SPELL_CUSTOM_CRUEL_GLAD_100 = 85047,
    SPELL_CUSTOM_CRUEL_GLAD_310 = 85048,

    // custom dominant glad
    SPELL_CUSTOM_DOMINANT_GLAD_100 = 85051,
    SPELL_CUSTOM_DOMINANT_GLAD_310 = 85052,

    // demonic gladiator glad
    SPELL_DEMONIC_GLADIATOR_GLAD_100 = 85091,
    SPELL_DEMONIC_GLADIATOR_GLAD_310 = 85092,

    // bfa pale proto drake
    SPELL_BFA_PALE_PROTO_DRAKE_100 = 85054,
    SPELL_BFA_PALE_PROTO_DRAKE_310 = 85055,

    // unchained glad souleater Green
    SPELL_UNCHAINED_GLAD_SOULEATER_GREEN_100 = 84031,
    SPELL_UNCHAINED_GLAD_SOULEATER_GREEN_310 = 84032,

    // crimson gladiator drake
    SPELL_CRIMSON_GLADIATOR_DRAKE_100 = 85057,
    SPELL_CRIMSON_GLADIATOR_DRAKE_310 = 85058,

    // DF S2 GLAD
    SPELL_DF_S2_GLAD_100 = 85061,
    SPELL_DF_S2_GLAD_310 = 85062,

    // DF S3 GLAD
    SPELL_DF_S3_GLAD_100 = 85064,
    SPELL_DF_S3_GLAD_310 = 85065,

    // DF S4 GLAD
    SPELL_DF_S4_GLAD_100 = 85067,
    SPELL_DF_S4_GLAD_310 = 85068,

    // DF S5 GLAD
    SPELL_DF_S5_GLAD_100 = 85071,
    SPELL_DF_S5_GLAD_310 = 85072,

    // DF S6 GLAD
    SPELL_DF_S6_GLAD_100 = 85074,
    SPELL_DF_S6_GLAD_310 = 85075,

    // Highland Drake Grey
    SPELL_HIGHLAND_DRAKE_GREY_100 = 85150,
    SPELL_HIGHLAND_DRAKE_GREY_310 = 85151,

    // Highland Drake Yellow
    SPELL_HIGHLAND_DRAKE_YELLOW_100 = 85153,
    SPELL_HIGHLAND_DRAKE_YELLOW_310 = 85154,

    // Highland Drake Red
    SPELL_HIGHLAND_DRAKE_RED_100 = 85156,
    SPELL_HIGHLAND_DRAKE_RED_310 = 85157,

    // Highland Drake Blue
    SPELL_HIGHLAND_DRAKE_BLUE_100 = 85159,
    SPELL_HIGHLAND_DRAKE_BLUE_310 = 85160,

    // Highland Drake Black
    SPELL_HIGHLAND_DRAKE_BLACK_100 = 85166,
    SPELL_HIGHLAND_DRAKE_BLACK_310 = 85167,

    // Highland Drake Green
    SPELL_HIGHLAND_DRAKE_GREEN_100 = 85169,
    SPELL_HIGHLAND_DRAKE_GREEN_310 = 85170,

    // --------------------------------------------------------------------------------------- \\
    // --------------------------------------------------------------------------------------- \\
    // Mounts Cata 1:

    // VICIOUS Gladiator's Mount
    SPELL_VICIOUSGLAD_100 = 84055,
    SPELL_VICIOUSGLAD_310 = 84056,

    // RUTHLESS Gladiator's Mount
    SPELL_RUTHLESSGLAD_100 = 84058,
    SPELL_RUTHLESSGLAD_310 = 84059,

    // CATACLYSMIC Gladiator's Mount
    SPELL_CATAGLAD_100 = 84061,
    SPELL_CATAGLAD_310 = 84062,

    // VICIOUS Gladiator's Mount Recolour BRONZE
    SPELL_VICIOUSGLADBRONZE_100 = 84085,
    SPELL_VICIOUSGLADBRONZE_310 = 84086,

    // VICIOUS Gladiator's Mount Recolour MOTTLED
    SPELL_VICIOUSGLADMOTTLED_100 = 84064,
    SPELL_VICIOUSGLADMOTTLED_310 = 84065,

    // VICIOUS Gladiator's Mount Recolour Blue
    SPELL_VICIOUSGLADBLUE_100 = 84067,
    SPELL_VICIOUSGLADBLUE_310 = 84068,

    // VICIOUS Gladiator's Mount Recolour Azure
    SPELL_VICIOUSGLADAZURE_100 = 84070,
    SPELL_VICIOUSGLADAZURE_310 = 84071,

    // VICIOUS Gladiator's Mount Recolour Aqua
    SPELL_VICIOUSGLADAQUA_100 = 84073,
    SPELL_VICIOUSGLADAQUA_310 = 84074,

    // VICIOUS Gladiator's Mount Recolour Emerald
    SPELL_VICIOUSGLADEMERALD_100 = 84076,
    SPELL_VICIOUSGLADEMERALD_310 = 84077,

    // VICIOUS Gladiator's Mount Recolour Albino
    SPELL_VICIOUSGLADALBINO_100 = 84079,
    SPELL_VICIOUSGLADALBINO_310 = 84080,

    // VICIOUS Gladiator's Mount Recolour Black
    SPELL_VICIOUSGLADBLACK_100 = 84082,
    SPELL_VICIOUSGLADBLACK_310 = 84083,

    // VICIOUS Gladiator's Mount Recolour Pink
    SPELL_VICIOUSGLADPINK_100 = 84088,
    SPELL_VICIOUSGLADPINK_310 = 84089,

    // VICIOUS Gladiator's Mount Recolour Red
    SPELL_VICIOUSGLADRED_100 = 84091,
    SPELL_VICIOUSGLADRED_310 = 84092,

    // Tyrael's Charger
    SPELL_TYRAELCHARGER_60 = 84011,
    SPELL_TYRAELCHARGER_100 = 84012,
    SPELL_TYRAELCHARGER_150 = 84013,
    SPELL_TYRAELCHARGER_310 = 84014,

    // Feldrake
    SPELL_FELDRAKE_100 = 84133,
    SPELL_FELDRAKE_310 = 84134,

    // Winged Guardian
    SPELL_WINGEDGUARDIAN_100 = 84158,
    SPELL_WINGEDGUARDIAN_310 = 84159,

    // PUREBLOOD FIRE HAWK
    SPELL_PUREBLOODFIREHAWK_100 = 84167,
    SPELL_PUREBLOODFIREHAWK_310 = 84168,

    // FELFIRE HAWK
    SPELL_FELFIREHAWK_100 = 84170,
    SPELL_FELFIREHAWK_310 = 84171,

    // CORRUPTED FIRE HAWK
    SPELL_CORRUPTEDFIREHAWK_100 = 84173,
    SPELL_CORRUPTEDFIREHAWK_310 = 84174,

    // BLAZING DRAKE
    SPELL_BLAZINGDRAKE_100 = 84118,
    SPELL_BLAZINGDRAKE_310 = 84119,

    // LIFE-BINDER HANDMAIDEN
    SPELL_LIFEBINDERHANDMAIDEN_100 = 84124,
    SPELL_LIFEBINDERHANDMAIDEN_310 = 84125,

    // TWILIGHT HARBINGER
    SPELL_TWILIGHTHARBINGER_100 = 84124,
    SPELL_TWILIGHTHARBINGER_310 = 84125,

    // CATA FLYING PANTHER
    SPELL_CATAFLYINGPANTHER_100 = 84300,
    SPELL_CATAFLYINGPANTHER_310 = 84301,

    // --------------------------------------------------------------------------------------- \\
	// Mounts MoP 1:

    // MOP JC PANTHER BLACK
    SPELL_JCPANTHERBLACK_100 = 84183,
    SPELL_JCPANTHERBLACK_310 = 84184,

    // MOP JC PANTHER RED
    SPELL_JCPANTHERRED_100 = 84186,
    SPELL_JCPANTHERRED_310 = 84187,

    // MOP JC PANTHER BLUE
    SPELL_JCPANTHERBLUE_100 = 84189,
    SPELL_JCPANTHERBLUE_310 = 84190,

    // MOP JC PANTHER GREEN
    SPELL_JCPANTHERGREEN_100 = 84192,
    SPELL_JCPANTHERGREEN_310 = 84193,

    // MOP JC PANTHER YELLOW
    SPELL_JCPANTHERYELLOW_100 = 84195,
    SPELL_JCPANTHERYELLOW_310 = 84196,

    // Hearthsteed
    SPELL_HEARTHSTEED_100 = 84161,
    SPELL_HEARTHSTEED_310 = 84162,

    // Cindermane Charger
    SPELL_CINDERMANECHARGER_100 = 84164,
    SPELL_CINDERMANECHARGER_310 = 84165,

    // WARFORGED NIGHTMARE
    SPELL_WARFORGEDNIGHTMARE_100 = 84219,
    SPELL_WARFORGEDNIGHTMARE_310 = 84220,

    // HEAVENLY ONYX CLOUD SERPENT
    SPELL_HEAVENLYONYX_100 = 84222,
    SPELL_HEAVENLYONYX_310 = 84223,

    // HEAVENLY GOLDEN CLOUD SERPENT
    SPELL_HEAVENLYGOLDEN_100 = 84225,
    SPELL_HEAVENLYGOLDEN_310 = 84226,

    // HEAVENLY BLUE CLOUD SERPENT
    SPELL_HEAVENLYBLUE_100 = 84228,
    SPELL_HEAVENLYBLUE_310 = 84229,

    // HEAVENLY GREEN CLOUD SERPENT
    SPELL_HEAVENLYGREEN_100 = 84231,
    SPELL_HEAVENLYGREEN_310 = 84232,

    // HEAVENLY RED CLOUD SERPENT
    SPELL_HEAVENLYRED_100 = 84234,
    SPELL_HEAVENLYRED_310 = 84235,

    // THUNDERING COBALT CLOUD
    SPELL_THUNDERINGCOBALT_100 = 84237,
    SPELL_THUNDERINGCOBALT_310 = 84238,

    // RAJANI WARSERPENT
    SPELL_RAJANIWARSERPENT_100 = 84240,
    SPELL_RAJANIWARSERPENT_310 = 84241,

    // THUNDERING AUGUST CLOUD
    SPELL_THUNDERINGAUGUST_100 = 84243,
    SPELL_THUNDERINGAUGUST_310 = 84244,

    // THUNDERING JADE CLOUD
    SPELL_THUNDERINGJADE_100 = 84246,
    SPELL_THUNDERINGJADE_310 = 84247,

    // THUNDERING ONYX CLOUD
    SPELL_THUNDERINGONYX_100 = 84249,
    SPELL_THUNDERINGONYX_310 = 84250,

    // THUNDERING RUBY CLOUD
    SPELL_THUNDERINGRUBY_100 = 84252,
    SPELL_THUNDERINGRUBY_310 = 84253,

    // S12 SERPENT
    SPELL_S12GLAD_100 = 84255,
    SPELL_S12GLAD_310 = 84256,

    // S13 SERPENT
    SPELL_S13GLAD_100 = 84258,
    SPELL_S13GLAD_310 = 84259,

    // S14 SERPENT
    SPELL_S14GLAD_100 = 84261,
    SPELL_S14GLAD_310 = 84262,

    // S15 SERPENT
    SPELL_S15GLAD_100 = 84264,
    SPELL_S15GLAD_310 = 84265,


    // Astral Cloud Serpent
    SPELL_ASTRALCLOUD_100 = 84403,
    SPELL_ASTRALCLOUD_310 = 84404,


    // --------------------------------------------------------------------------------------- \\
	// Mounts WoD 1:

    // INFINITE TIMEREAVER
    SPELL_INFINITETIMEREAVER_100 = 84127,
    SPELL_INFINITETIMEREAVER_310 = 84128,

    // DREAD RAVEN
    SPELL_DREADRAVEN_100 = 84198,
    SPELL_DREADRAVEN_310 = 84199,

    // RAVAGER MOUNT
    SPELL_RAVAGERMOUNT_100 = 84303,
    SPELL_RAVAGERMOUNT_310 = 84304,

    // Purple Dread Raven
    SPELL_PURPLEDREADRAVEN_100 = 84680,
    SPELL_PURPLEDREADRAVEN_310 = 84681,

    // --------------------------------------------------------------------------------------- \\
	// Mounts Legion 1:

    // Vindictive Gladiator's Mount
    SPELL_VINDICTIVEGLAD_100 = 84034,
    SPELL_VINDICTIVEGLAD_310 = 84035,

    // FEARLESS Gladiator's Mount
    SPELL_FEARLESSGLAD_100 = 84037,
    SPELL_FEARLESSGLAD_310 = 84038,

    // CRUEL Gladiator's Mount
    SPELL_CRUELGLAD_100 = 84040,
    SPELL_CRUELGLAD_310 = 84041,

    // FEROCIOUS Gladiator's Mount
    SPELL_FEROCIOUSGLAD_100 = 84043,
    SPELL_FEROCIOUSGLAD_310 = 84044,

    // FIERCE Gladiator's Mount
    SPELL_FIERCEGLAD_100 = 84046,
    SPELL_FIERCEGLAD_310 = 84047,

    // DOMINANT Gladiator's Mount
    SPELL_DOMINANTGLAD_100 = 84049,
    SPELL_DOMINANTGLAD_310 = 84050,

    // DEMONIC Gladiator's Mount
    SPELL_DEMONICGLAD_100 = 84052,
    SPELL_DEMONICGLAD_310 = 84053,

    // Huntmaster's Loyal Wolfhawk 1
    SPELL_HUNTMASTERLOYALWOLFHAWK1_100 = 84148,
    SPELL_HUNTMASTERLOYALWOLFHAWK1_310 = 84149,

    // Huntmaster's Loyal Wolfhawk 2
    SPELL_HUNTMASTERLOYALWOLFHAWK2_100 = 84152,
    SPELL_HUNTMASTERLOYALWOLFHAWK2_310 = 84153,

    // Huntmaster's Loyal Wolfhawk 3
    SPELL_HUNTMASTERLOYALWOLFHAWK3_100 = 84155,
    SPELL_HUNTMASTERLOYALWOLFHAWK3_310 = 84156,

    // LEGION DRAKE Gladiator's Dark Blue
    SPELL_LEGIONDARKBLUEGLAD_100 = 84094,
    SPELL_LEGIONDARKBLUEGLAD_310 = 84095,

    // LEGION DRAKE Gladiator's Blue
    SPELL_LEGIONBLUEGLAD_100 = 84097,
    SPELL_LEGIONBLUEGLAD_310 = 84098,

    // LEGION DRAKE Gladiator's LIGHT
    SPELL_LEGIONGLADLIGHT_100 = 84100,
    SPELL_LEGIONGLADLIGHT_310 = 84101,

    // LEGION DRAKE Gladiator's WHITE
    SPELL_LEGIONGLADWHITE_100 = 84103,
    SPELL_LEGIONGLADWHITE_310 = 84104,

    // LEGION DRAKE Gladiator's EMERALD
    SPELL_LEGIONGLADEMERALD_100 = 84106,
    SPELL_LEGIONGLADEMERALD_310 = 84107,

    // LEGION DRAKE Gladiator's GREEN
    SPELL_LEGIONGLADGREEN_100 = 84109,
    SPELL_LEGIONGLADGREEN_310 = 84110,

    // LEGION DRAKE Gladiator's BLACK
    SPELL_LEGIONGLADBLACK_100 = 84112,
    SPELL_LEGIONGLADBLACK_310 = 84113,

    // LEGION DRAKE Gladiator's RED
    SPELL_LEGIONGLADRED_100 = 84115,
    SPELL_LEGIONGLADRED_310 = 84116,

    // FELHOUND MOUNT RED
    SPELL_FELHOUNDMOUNTRED_100 = 84176,
    SPELL_FELHOUNDMOUNTRED_310 = 84177,

    // FELHOUND MOUNT PURPLE
    SPELL_FELHOUNDMOUNTPURPLE_100 = 84179,
    SPELL_FELHOUNDMOUNTPURPLE_310 = 84180,

    // WARLOCK DREADSTEED GREEN
    SPELL_DREADSTEEDGREEN_100 = 84306,
    SPELL_DREADSTEEDGREEN_310 = 84307,

    // WARLOCK DREADSTEED RED
    SPELL_DREADSTEEDRED_100 = 84309,
    SPELL_DREADSTEEDRED_310 = 84310,

    // WARLOCK DREADSTEED PURPLE
    SPELL_DREADSTEEDPURPLE_100 = 84312,
    SPELL_DREADSTEEDPURPLE_310 = 84313,

    // UNDEAD KARAZHAN DRAKE
    SPELL_KARAZHANDRAKE_100 = 84201,
    SPELL_KARAZHANDRAKE_310 = 84202,

    // FARSEER RAGING TEMPEST (FIRE)
    SPELL_RAGINGTEMPESTFIRE_100 = 84294,
    SPELL_RAGINGTEMPESTFIRE_310 = 84295,

    // FARSEER RAGING TEMPEST (RESTO)
    SPELL_RAGINGTEMPESTRESTO_100 = 84297,
    SPELL_RAGINGTEMPESTRESTO_310 = 84298,



    // Paladin Highlord Blue
    SPELL_PALADINHIGHLORDBLUE_100 = 85127,
    SPELL_PALADINHIGHLORDBLUE_310 = 85128,

    // Paladin Highlord Purple
    SPELL_PALADINHIGHLORDPURPLE_100 = 85121,
    SPELL_PALADINHIGHLORDPURPLE_310 = 85122,

    // Paladin Highlord Yellow
    SPELL_PALADINHIGHLORDYELLOW_100 = 85131,
    SPELL_PALADINHIGHLORDYELLOW_310 = 85132,

    // Paladin Highlord Red
    SPELL_PALADINHIGHLORDRED_100 = 85124,
    SPELL_PALADINHIGHLORDRED_310 = 85125,

    // Black Serpent of Nzoth
    SPELL_NZOTHSERP_100 = 85162,
    SPELL_NZOTHSERP_310 = 85163,

    // Sunwarmed Furline
    SPELL_SUNWARMEDFURLINE_100 = 85114,
    SPELL_SUNWARMEDFURLINE_310 = 85115,

    // Divine kiss of ohn ahra
    SPELL_DIVINEKISSOFOHNAHRA_100 = 85111,
    SPELL_DIVINEKISSOFOHNAHRA_310 = 85112,

    // Zenet Hatclhing
    SPELL_ZENETHATCHLING_100 = 85137,
    SPELL_ZENETHATCHLING_310 = 85138,

    // Zenet Hatclhing (Blue)
    SPELL_ZENETHATCHLINGBLUE_100 = 85142,
    SPELL_ZENETHATCHLINGBLUE_310 = 85143,

    // Vulpine Familiar
    SPELL_VULPINEFAMILIAR_100 = 85117,
    SPELL_VULPINEFAMILIAR_310 = 85118,

    // Vulpine Familiar (Yellow Recolor)
    SPELL_VULPINEFAMILIARYELLOW_100 = 85134,
    SPELL_VULPINEFAMILIARYELLOW_310 = 85135,


    // --------------------------------------------------------------------------------------- \\
	// Mounts BFA 1:

    // Squeakers
    SPELL_SQUEAKERS_100 = 84374,
    SPELL_SQUEAKERS_310 = 84375,

    // GLACIAL TIDESTORM - 84206
    SPELL_GLACIALTIDESTORM_100 = 84207,
    SPELL_GLACIALTIDESTORM_310 = 84208,

    // GLACIAL TIDESTORM GREEN - 84209
    SPELL_TIDESTORMGREEN_100 = 84210,
    SPELL_TIDESTORMGREEN_310 = 84211,

    // GLACIAL TIDESTORM PURPLE - 84212
    SPELL_TIDESTORMPURPLE_100 = 84213,
    SPELL_TIDESTORMPURPLE_310 = 84214,

    // GLACIAL TIDESTORM RED - 84215
    SPELL_TIDESTORMRED_100 = 84216,
    SPELL_TIDESTORMRED_310 = 84217,

    // DREAD GLADIATOR'S PROTO DRAKE
    SPELL_DREADGLAD_100 = 84267,
    SPELL_DREADGLAD_310 = 84268,

    // SINISTER GLADIATOR'S PROTO DRAKE
    SPELL_SINISTERGLADIATOR_100 = 84270,
    SPELL_SINISTERGLADIATOR_310 = 84271,

    // NOTORIOUS GLADIATOR'S PROTO DRAKE
    SPELL_NOTORIOUSGLADIATOR_100 = 84273,
    SPELL_NOTORIOUSGLADIATOR_310 = 84274,

    // CORRUPTED GLADIATOR'S PROTO DRAKE
    SPELL_CORRUPTEDGLADIATOR_100 = 84276,
    SPELL_CORRUPTEDGLADIATOR_310 = 84277,

    // OBSIDIAN WORLDBREAKER
    SPELL_OBSIDIANWORLDBREAKER_100 = 84130,
    SPELL_OBSIDIANWORLDBREAKER_310 = 84131,

    // --------------------------------------------------------------------------------------- \\
	// Mounts Shadowlands 1:
    // Sinful Gladiator's Mount
    SPELL_SINFULGLAD_100 = 84016,
    SPELL_SINFULGLAD_310 = 84017,

    // Unchained Gladiator's Mount
    SPELL_UNCHAINEDGLAD_100 = 84028,
    SPELL_UNCHAINEDGLAD_310 = 84029,

    // Cosmic Gladiator's Mount
    SPELL_COSMICGLAD_100 = 84022,
    SPELL_COSMICGLAD_310 = 84023,

    // Eternal Gladiator's Mount
    SPELL_ETERNALGLAD_100 = 84025,
    SPELL_ETERNALGLAD_310 = 84026,

    // Tazavesh Gearglider
    SPELL_TAZAVESHGEARGLIDER_100 = 84136,
    SPELL_TAZAVESHGEARGLIDER_310 = 84137,

    // Pilfered Gearglider
    SPELL_PILFEREDGEARGLIDER_100 = 84139,
    SPELL_PILFEREDGEARGLIDER_310 = 84140,

    // Cartel Master Gearglider
    SPELL_CARTELMASTERGEARGLIDER_100 = 84142,
    SPELL_CARTELMASTERGEARGLIDER_310 = 84143,

    // Cartel Master Gearglider (Silver)
    SPELL_CARTELMASTERGEARGLIDERSILVER_100 = 84145,
    SPELL_CARTELMASTERGEARGLIDERSILVER_310 = 84146,

    // VENGEANCE
    SPELL_VENGEANCE_100 = 84204,
    SPELL_VENGEANCE_310 = 84205,

    // Mechagon Peacekeeper
    SPELL_MECHAGONPEACEK_100 = 85173,
    SPELL_MECHAGONPEACEK_310 = 85174,

    // Grotto Netherwing Drake
    SPELL_GROTTONETHERWING_100 = 85205,
    SPELL_GROTTONETHERWING_310 = 85206,

    // Elementium Drake
    SPELL_ELEMENTIUMDRAKE_100 = 85199,
    SPELL_ELEMENTIUMDRAKE_310 = 85200,

    // Azure Worldchiller
    SPELL_AZUREWORLDCH_100 = 85178,
    SPELL_AZUREWORLDCH_310 = 85179,

    // Amalgam of Rage
    SPELL_AMALGAMRAGE_100 = 85196,
    SPELL_AMALGAMRAGE_310 = 85197,

    // Pale Gravewing
    SPELL_GRAVEWINGPALE_100 = 85202,
    SPELL_GRAVEWINGPALE_310 = 85203,

    // Sinfall Gravewing
    SPELL_GRAVEWINGSINFALL_100 = 85181,
    SPELL_GRAVEWINGSINFALL_310 = 85182,

    // Obsidian Gravewing
    SPELL_GRAVEWINGOBSIDIAN_100 = 85184,
    SPELL_GRAVEWINGOBSIDIAN_310 = 85185,

    // Raptora Swooper
    SPELL_RAPTORASWOOPER_100 = 85190,
    SPELL_RAPTORASWOOPER_310 = 85191,

    // Mawdapted Raptora
    SPELL_MAWDAPTEDRAPTORA_100 = 85187,
    SPELL_MAWDAPTEDRAPTORA_310 = 85188,

    // Desertwing Hunter
    SPELL_DESERTWINGHUNTER_100 = 85193,
    SPELL_DESERTWINGHUNTER_310 = 85194,

    // SOULTWISTED DEATHWALKER
    SPELL_SOULTWISTEDDEATHWALKER_100 = 84279,
    SPELL_SOULTWISTEDDEATHWALKER_310 = 84280,

    // SINTOUCHED DEATHWALKER
    SPELL_SINTOUCHEDDEATHWALKER_100 = 84282,
    SPELL_SINTOUCHEDDEATHWALKER_310 = 84283,

    // SOULTWISTED DEATHWALKER RECOLOR
    SPELL_SOULTWISTEDDEATHWALKERRECOLOR_100 = 84285,
    SPELL_SOULTWISTEDDEATHWALKERRECOLOR_310 = 84286,

    // Wastewarped DEATHWALKER
    SPELL_WASTEWARPEDDEATHWALKER_100 = 84288,
    SPELL_WASTEWARPEDDEATHWALKER_310 = 84289,

    // RESTORATION DEATHWALKER
    SPELL_RESTORATIONDEATHWALKER_100 = 84291,
    SPELL_RESTORATIONDEATHWALKER_310 = 84292

};


class spell_gen_mount : public SpellScript
{
    PrepareSpellScript(spell_gen_mount);

public:
    spell_gen_mount(uint32 mount0, uint32 mount60, uint32 mount100, uint32 mount150, uint32 mount280, uint32 mount310) : SpellScript(),
        _mount0(mount0), _mount60(mount60), _mount100(mount100), _mount150(mount150), _mount280(mount280), _mount310(mount310) { }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        if (_mount0 && !sSpellMgr->GetSpellInfo(_mount0))
            return false;
        if (_mount60 && !sSpellMgr->GetSpellInfo(_mount60))
            return false;
        if (_mount100 && !sSpellMgr->GetSpellInfo(_mount100))
            return false;
        if (_mount150 && !sSpellMgr->GetSpellInfo(_mount150))
            return false;
        if (_mount280 && !sSpellMgr->GetSpellInfo(_mount280))
            return false;
        if (_mount310 && !sSpellMgr->GetSpellInfo(_mount310))
            return false;
        return true;
    }

    void HandleMount(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        if (Player* target = GetHitPlayer())
        {
            uint32 petNumber = target->GetTemporaryUnsummonedPetNumber();
            target->SetTemporaryUnsummonedPetNumber(0);

            // Prevent stacking of mounts and client crashes upon dismounting
            target->RemoveAurasByType(SPELL_AURA_MOUNTED, ObjectGuid::Empty, GetHitAura());

            // Triggered spell id dependent on riding skill and zone
            bool canFly = false;
            uint32 map = GetVirtualMapForMapAndZone(target->GetMapId(), target->GetZoneId());
            if (map == 530 || (map == 571 && target->HasSpell(SPELL_COLD_WEATHER_FLYING)))
                canFly = true;

            AreaTableEntry const* area = sAreaTableStore.LookupEntry(target->GetAreaId());
            // Xinef: add battlefield check
            Battlefield* Bf = sBattlefieldMgr->GetBattlefieldToZoneId(target->GetZoneId());
            if ((area && canFly && (area->flags & AREA_FLAG_NO_FLY_ZONE)) || (Bf && !Bf->CanFlyIn()))
                canFly = false;

            uint32 mount = 0;
            switch (target->GetBaseSkillValue(SKILL_RIDING))
            {
            case 0:
                mount = _mount0;
                break;
            case 75:
                mount = _mount60;
                break;
            case 150:
                mount = _mount100;
                break;
            case 225:
                if (canFly)
                    mount = _mount150;
                else
                    mount = _mount100;
                break;
            case 300:
                if (canFly)
                {
                    if (_mount310 && target->Has310Flyer(false))
                        mount = _mount310;
                    else
                        mount = _mount280;
                }
                else
                    mount = _mount100;
                break;
            default:
                break;
            }

            if (mount)
            {
                PreventHitAura();
                target->CastSpell(target, mount, true);
            }

            if (petNumber)
                target->SetTemporaryUnsummonedPetNumber(petNumber);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_mount::HandleMount, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
    }

private:
    uint32 _mount0;
    uint32 _mount60;
    uint32 _mount100;
    uint32 _mount150;
    uint32 _mount280;
    uint32 _mount310;
};


void AddSpellMountScriptsScripts()
{

    // Essas 2 originalmente já existem, mas só tem a opção de 150/280/310 (precisa do patch), adicionando a opção de 100%:
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_x53_touring_rocket_new", 0, 0, SPELL_X53_TOURING_ROCKET_100, SPELL_X53_TOURING_ROCKET_150, SPELL_X53_TOURING_ROCKET_280, SPELL_X53_TOURING_ROCKET_310);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_blazing_hippogryph_new", 0, 0, SPELL_BLAZING_HIPPOGRYPH_100, SPELL_BLAZING_HIPPOGRYPH_150, SPELL_BLAZING_HIPPOGRYPH_280, 0);

    // --------------------------------------------------------------------------- \\
    // New Ones:

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_onyxian", 0, 0, SPELL_ONYXIAN_100, 0, SPELL_ONYXIAN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ashes", 0, 0, SPELL_ASHES_100, 0, SPELL_ASHES_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_mimiron", 0, 0, SPELL_MIMIRON_100, 0, SPELL_MIMIRON_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s1", 0, 0, SPELL_S1_100, 0, SPELL_S1_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s2", 0, 0, SPELL_S2_100, 0, SPELL_S2_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s3", 0, 0, SPELL_S3_100, 0, SPELL_S3_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s4", 0, 0, SPELL_S4_100, 0, SPELL_S4_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s5", 0, 0, SPELL_S5_100, 0, SPELL_S5_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s6", 0, 0, SPELL_S6_100, 0, SPELL_S6_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s7", 0, 0, SPELL_S7_100, 0, SPELL_S7_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_s8", 0, 0, SPELL_S8_100, 0, SPELL_S8_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_redDRAGONHAWK", 0, 0, SPELL_REDDRAGONHAWK_100, 0, SPELL_REDDRAGONHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLUEDRAGONHAWK", 0, 0, SPELL_BLUEDRAGONHAWK_100, 0, SPELL_BLUEDRAGONHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SUNREAVERDRAG", 0, 0, SPELL_SUNREAVERDRAG_100, 0, SPELL_SUNREAVERDRAG_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SILVERCOVENANT", 0, 0, SPELL_SILVERCOVENANT_100, 0, SPELL_SILVERCOVENANT_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ARGENTHIPPO", 0, 0, SPELL_ARGENTHIPPO_100, 0, SPELL_ARGENTHIPPO_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CENARIONWAR", 0, 0, SPELL_CENARIONWAR_100, 0, SPELL_CENARIONWAR_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TIMELOST", 0, 0, SPELL_TIMELOST_100, 0, SPELL_TIMELOST_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLUEPROTO", 0, 0, SPELL_BLUEPROTO_100, 0, SPELL_BLUEPROTO_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_REDPROTO", 0, 0, SPELL_REDPROTO_100, 0, SPELL_REDPROTO_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_IRONBOUNDPROTODRAKE", 0, 0, SPELL_IRONBOUNDPROTODRAKE_100, 0, SPELL_IRONBOUNDPROTODRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RUSTEDPROTODRAKE", 0, 0, SPELL_RUSTEDPROTODRAKE_100, 0, SPELL_RUSTEDPROTODRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VIOLETPROTODRAKE", 0, 0, SPELL_VIOLETPROTODRAKE_100, 0, SPELL_VIOLETPROTODRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PLAGUEDPROTODRAKE", 0, 0, SPELL_PLAGUEDPROTODRAKE_100, 0, SPELL_PLAGUEDPROTODRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLACKPROTODRAKE", 0, 0, SPELL_BLACKPROTODRAKE_100, 0, SPELL_BLACKPROTODRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GREENPROTODRAKE", 0, 0, SPELL_GREENPROTODRAKE_100, 0, SPELL_GREENPROTODRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ALBINODRAKE", 0, 0, SPELL_ALBINODRAKE_100, 0, SPELL_ALBINODRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BRONZEDRAKE", 0, 0, SPELL_BRONZEDRAKE_100, 0, SPELL_BRONZEDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_AZUREDRAKE", 0, 0, SPELL_AZUREDRAKE_100, 0, SPELL_AZUREDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TWILIGHTPURPLEDRAKE", 0, 0, SPELL_TWILIGHTPURPLEDRAKE_100, 0, SPELL_TWILIGHTPURPLEDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TWILIGHTDRAKE", 0, 0, SPELL_TWILIGHTDRAKE_100, 0, SPELL_TWILIGHTDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLACKDRAKE", 0, 0, SPELL_BLACKDRAKE_100, 0, SPELL_BLACKDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_REDDRAKE", 0, 0, SPELL_REDDRAKE_100, 0, SPELL_REDDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLUEDRAKE", 0, 0, SPELL_BLUEDRAKE_100, 0, SPELL_BLUEDRAKE_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING1", 0, 0, SPELL_NETHERWING1_100, 0, SPELL_NETHERWING1_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING2", 0, 0, SPELL_NETHERWING2_100, 0, SPELL_NETHERWING2_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING3", 0, 0, SPELL_NETHERWING3_100, 0, SPELL_NETHERWING3_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING4", 0, 0, SPELL_NETHERWING4_100, 0, SPELL_NETHERWING4_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING5", 0, 0, SPELL_NETHERWING5_100, 0, SPELL_NETHERWING5_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERWING6", 0, 0, SPELL_NETHERWING6_100, 0, SPELL_NETHERWING6_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TURBOCHARGED", 0, 0, SPELL_TURBOCHARGED_100, 0, SPELL_TURBOCHARGED_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_MAGNIFICENTCARPET", 0, 0, SPELL_MAGNIFICENTCARPET_100, 0, SPELL_MAGNIFICENTCARPET_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FROSTYFLYING", 0, 0, SPELL_FROSTYFLYING_100, 0, SPELL_FROSTYFLYING_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FLYINGCARPET", 0, 0, SPELL_FLYINGCARPET_100, 0, SPELL_FLYINGCARPET_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERRAY1", 0, 0, SPELL_NETHERRAY1_100, 0, SPELL_NETHERRAY1_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERRAY2", 0, 0, SPELL_NETHERRAY2_100, 0, SPELL_NETHERRAY2_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERRAY3", 0, 0, SPELL_NETHERRAY3_100, 0, SPELL_NETHERRAY3_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERRAY4", 0, 0, SPELL_NETHERRAY4_100, 0, SPELL_NETHERRAY4_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NETHERRAY5", 0, 0, SPELL_NETHERRAY5_100, 0, SPELL_NETHERRAY5_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_X51", 0, 0, SPELL_X51_100, 0, SPELL_X51_280, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_X51XT", 0, 0, SPELL_X51XT_100, 0, SPELL_X51XT_280, 0);

    // --------------------------------------------------------------------------- \\
    // Flying Sabers |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BANLU", 0, 0, SPELL_BAN_LU_100, 0, SPELL_BAN_LU_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LUMINOUSSTAR", 0, 0, SPELL_LUMINOUS_STARSEEKER_100, 0, SPELL_LUMINOUS_STARSEEKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_MYSTICRSABER", 0, 0, SPELL_MYSTIC_RUNESABER_100, 0, SPELL_MYSTIC_RUNESABER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ARCANISTMANASABER", 0, 0, SPELL_ARCANIST_MANASABER_100, 0, SPELL_ARCANIST_MANASABER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SILVERWINDLARION", 0, 0, SPELL_SILVERWIND_LARION_100, 0, SPELL_SILVERWIND_LARION_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHWINDDARKMANE", 0, 0, SPELL_HIGHWIND_DARKMANE_100, 0, SPELL_HIGHWIND_DARKMANE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_WENLO", 0, 0, SPELL_WEN_LO_100, 0, SPELL_WEN_LO_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ASHADAR", 0, 0, SPELL_ASH_ADAR_100, 0, SPELL_ASH_ADAR_310, 0);

    // Flying Horses |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GHASTLYCHARGER", 0, 0, SPELL_GHASTLYCHARGER_100, 0, SPELL_GHASTLYCHARGER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TEMPESTSKYTALION", 0, 0, SPELL_TEMPESSKYTAL_100, 0, SPELL_TEMPESSKYTAL_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SWIFTWINDSTEED", 0, 0, SPELL_SWIFTWINDST_100, 0, SPELL_SWIFTWINDST_310, 0);

    // Flying Birds |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CLUTCHOFJIKUN", 0, 0, SPELL_CLUTCH_OF_JIKUN_100, 0, SPELL_CLUTCH_OF_JIKUN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CLUTCH_OF_HALI", 0, 0, SPELL_CLUTCH_OF_HALI_100, 0, SPELL_CLUTCH_OF_HALI_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CRIMSON_PHOENIX", 0, 0, SPELL_CRIMSON_PHOENIX_100, 0, SPELL_CRIMSON_PHOENIX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ASHEN_PHOENIX", 0, 0, SPELL_ASHEN_PHOENIX_100, 0, SPELL_ASHEN_PHOENIX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_VIOLET_PHOENIX", 0, 0, SPELL_VIOLET_PHOENIX_100, 0, SPELL_VIOLET_PHOENIX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_FLAMEWARD_GRYPH", 0, 0, SPELL_FLAMEWARD_GRYPH_100, 0, SPELL_FLAMEWARD_GRYPH_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TELDRASSIL_GRYPH", 0, 0, SPELL_TELDRASSIL_GRYPH_100, 0, SPELL_TELDRASSIL_GRYPH_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_STORMCROW", 0, 0, SPELL_STORMCROW_100, 0, SPELL_STORMCROW_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SOLAR_SPIREHAWK", 0, 0, SPELL_SOLAR_SPIREHAWK_100, 0, SPELL_SOLAR_SPIREHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SHARKBAIT", 0, 0, SPELL_SHARKBAIT_100, 0, SPELL_SHARKBAIT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SQUAWKS", 0, 0, SPELL_SQUAWKS_100, 0, SPELL_SQUAWKS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_QUAWKS", 0, 0, SPELL_QUAWKS_100, 0, SPELL_QUAWKS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BLU", 0, 0, SPELL_BLU_100, 0, SPELL_BLU_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_MURDEROUS_OMEN", 0, 0, SPELL_MURDEROUS_OMEN_100, 0, SPELL_MURDEROUS_OMEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SPECTRAL_PHOENIX", 0, 0, SPELL_SPECTRAL_PHOENIX_100, 0, SPELL_SPECTRAL_PHOENIX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DARK_PHOENIX", 0, 0, SPELL_DARK_PHOENIX_100, 0, SPELL_DARK_PHOENIX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SAPPHIRE_SKYBLAZER", 0, 0, SPELL_SAPPHIRE_SKYBLAZER_100, 0, SPELL_SAPPHIRE_SKYBLAZER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_AMBER_SKYBLAZER", 0, 0, SPELL_AMBER_SKYBLAZER_100, 0, SPELL_AMBER_SKYBLAZER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARMORED_RED_DRAGONHAWK", 0, 0, SPELL_ARMORED_RED_DRAGONHAWK_100, 0, SPELL_ARMORED_RED_DRAGONHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARMORED_BLUE_DRAGONHAWK", 0, 0, SPELL_ARMORED_BLUE_DRAGONHAWK_100, 0, SPELL_ARMORED_BLUE_DRAGONHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GRAND_ARMORED_GRYPHON", 0, 0, SPELL_GRAND_ARMORED_GRYPHON_100, 0, SPELL_GRAND_ARMORED_GRYPHON_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GRAND_ARMORED_WYVERN", 0, 0, SPELL_GRAND_ARMORED_WYVERN_100, 0, SPELL_GRAND_ARMORED_WYVERN_310, 0);

    // Rockets |||  |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ORGRIMMAR_INTERCEPTOR", 0, 0, SPELL_ORGRIMMAR_INTERCEPTOR_100, 0, SPELL_ORGRIMMAR_INTERCEPTOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_STORMWIND_SKYCHASER", 0, 0, SPELL_STORMWIND_SKYCHASER_100, 0, SPELL_STORMWIND_SKYCHASER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DREADWAKE", 0, 0, SPELL_DREADWAKE_100, 0, SPELL_DREADWAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DARKMOON_DIRIGIBLE", 0, 0, SPELL_DARKMOON_DIRIGIBLE_100, 0, SPELL_DARKMOON_DIRIGIBLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_AERIAL_UNIT_R21", 0, 0, SPELL_AERIAL_UNIT_R21_100, 0, SPELL_AERIAL_UNIT_R21_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_EXPLORER_JUNGLE_HOPPER", 0, 0, SPELL_EXPLORER_JUNGLE_HOPPER_100, 0, SPELL_EXPLORER_JUNGLE_HOPPER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DEPLETED_KYPARIUM_ROCKET", 0, 0, SPELL_DEPLETED_KYPARIUM_ROCKET_100, 0, SPELL_DEPLETED_KYPARIUM_ROCKET_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GEOSYNC_WORLD_SPINNER", 0, 0, SPELL_GEOSYNC_WORLD_SPINNER_100, 0, SPELL_GEOSYNC_WORLD_SPINNER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_JADE_PANDAREN_KITE_STRING", 0, 0, SPELL_JADE_PANDAREN_KITE_STRING_100, 0, SPELL_JADE_PANDAREN_KITE_STRING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_PANDAREN_KITE_ALLIANCE", 0, 0, SPELL_PANDAREN_KITE_ALLIANCE_100, 0, SPELL_PANDAREN_KITE_ALLIANCE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_PANDAREN_KITE_HORDE", 0, 0, SPELL_PANDAREN_KITE_HORDE_100, 0, SPELL_PANDAREN_KITE_HORDE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BLUE_FLYING_CLOUD", 0, 0, SPELL_BLUE_FLYING_CLOUD_100, 0, SPELL_BLUE_FLYING_CLOUD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_RED_FLYING_CLOUD", 0, 0, SPELL_RED_FLYING_CLOUD_100, 0, SPELL_RED_FLYING_CLOUD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GOLD_FLYING_CLOUD", 0, 0, SPELL_GOLD_FLYING_CLOUD_100, 0, SPELL_GOLD_FLYING_CLOUD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARCHMAGE_PRISM_DISCFROST", 0, 0, SPELL_ARCHMAGE_PRISM_DISCFROST_100, 0, SPELL_ARCHMAGE_PRISM_DISCFROST_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARCHMAGE_PRISM_DISCFIRE", 0, 0, SPELL_ARCHMAGE_PRISM_DISCFIRE_100, 0, SPELL_ARCHMAGE_PRISM_DISCFIRE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARCHMAGE_PRISM_DISCARCANE", 0, 0, SPELL_ARCHMAGE_PRISM_DISCARCANE_100, 0, SPELL_ARCHMAGE_PRISM_DISCARCANE_310, 0);

    // Outros |||  |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_THE_HIVEMIND", 0, 0, SPELL_THE_HIVEMIND_100, 0, SPELL_THE_HIVEMIND_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_FATHOM_DWELLER", 0, 0, SPELL_FATHOM_DWELLER_100, 0, SPELL_FATHOM_DWELLER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SHAMAN_MOUNT_ENHANCE", 0, 0, SPELL_SHAMAN_MOUNT_ENHANCE_100, 0, SPELL_SHAMAN_MOUNT_ENHANCE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SHAMAN_MOUNT_BLUE", 0, 0, SPELL_SHAMAN_MOUNT_BLUE_100, 0, SPELL_SHAMAN_MOUNT_BLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SHAMAN_MOUNT_PURPLE", 0, 0, SPELL_SHAMAN_MOUNT_PURPLE_100, 0, SPELL_SHAMAN_MOUNT_PURPLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SHAMAN_MOUNT_SAND", 0, 0, SPELL_SHAMAN_MOUNT_SAND_100, 0, SPELL_SHAMAN_MOUNT_SAND_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_IRON_SKYREAVER", 0, 0, SPELL_IRON_SKYREAVER_100, 0, SPELL_IRON_SKYREAVER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GRUESOME_FLAYEDWING", 0, 0, SPELL_GRUESOME_FLAYEDWING_100, 0, SPELL_GRUESOME_FLAYEDWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SILVERTIP_DREDWING", 0, 0, SPELL_SILVERTIP_DREDWING_100, 0, SPELL_SILVERTIP_DREDWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_UNDERCITY_PLAGUEBAT", 0, 0, SPELL_UNDERCITY_PLAGUEBAT_100, 0, SPELL_UNDERCITY_PLAGUEBAT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_HARVESTER_DREDWING", 0, 0, SPELL_HARVESTER_DREDWING_100, 0, SPELL_HARVESTER_DREDWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ARMORED_BLOODWING", 0, 0, SPELL_ARMORED_BLOODWING_100, 0, SPELL_ARMORED_BLOODWING_310, 0);

    // Dragoes Chineses |||  |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_HEART_OF_THE_ASPECTS", 0, 0, SPELL_HEART_OF_THE_ASPECTS_100, 0, SPELL_HEART_OF_THE_ASPECTS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ABYSS_WORM", 0, 0, SPELL_ABYSS_WORM_100, 0, SPELL_ABYSS_WORM_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_RIDDLER_MIND_WORM", 0, 0, SPELL_RIDDLER_MIND_WORM_100, 0, SPELL_RIDDLER_MIND_WORM_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_NAZJATAR_BLOOD_SERPENT", 0, 0, SPELL_NAZJATAR_BLOOD_SERPENT_100, 0, SPELL_NAZJATAR_BLOOD_SERPENT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_MAGENTA_CLOUD_SERPENT", 0, 0, SPELL_MAGENTA_CLOUD_SERPENT_100, 0, SPELL_MAGENTA_CLOUD_SERPENT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_NETHER_GORGED_GREATWYRM", 0, 0, SPELL_NETHER_GORGED_GREATWYRM_100, 0, SPELL_NETHER_GORGED_GREATWYRM_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_NETHER_GORGED_GREATWYRM_BLACK", 0, 0, SPELL_NETHER_GORGED_GREATWYRM_BLACK_100, 0, SPELL_NETHER_GORGED_GREATWYRM_BLACK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_NETHER_GORGED_GREATWYRM_SILVER", 0, 0, SPELL_NETHER_GORGED_GREATWYRM_SILVER_100, 0, SPELL_NETHER_GORGED_GREATWYRM_SILVER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_NETHER_GORGED_GREATWYRM_PURPLE", 0, 0, SPELL_NETHER_GORGED_GREATWYRM_PURPLE_100, 0, SPELL_NETHER_GORGED_GREATWYRM_PURPLE_310, 0);

    // Dragoes |||  |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_STEAMSCALE_INCINERATOR", 0, 0, SPELL_STEAMSCALE_INCINERATOR_100, 0, SPELL_STEAMSCALE_INCINERATOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TANGLED_DREAMWEAVER", 0, 0, SPELL_TANGLED_DREAMWEAVER_100, 0, SPELL_TANGLED_DREAMWEAVER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BATTLELORD_BLOODTHIRSTY_1", 0, 0, SPELL_BATTLELORD_BLOODTHIRSTY_1_100, 0, SPELL_BATTLELORD_BLOODTHIRSTY_1_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BATTLELORD_BLOODTHIRSTY_2", 0, 0, SPELL_BATTLELORD_BLOODTHIRSTY_2_100, 0, SPELL_BATTLELORD_BLOODTHIRSTY_2_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BATTLELORD_BLOODTHIRSTY_3", 0, 0, SPELL_BATTLELORD_BLOODTHIRSTY_3_100, 0, SPELL_BATTLELORD_BLOODTHIRSTY_3_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_RED_GALAKRAS", 0, 0, SPELL_RED_GALAKRAS_100, 0, SPELL_RED_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BLUE_GALAKRAS", 0, 0, SPELL_BLUE_GALAKRAS_100, 0, SPELL_BLUE_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BLACK_GALAKRAS", 0, 0, SPELL_BLACK_GALAKRAS_100, 0, SPELL_BLACK_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_RAZORSCALE_GALAKRAS", 0, 0, SPELL_RAZORSCALE_GALAKRAS_100, 0, SPELL_RAZORSCALE_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_RUSTED_GALAKRAS", 0, 0, SPELL_RUSTED_GALAKRAS_100, 0, SPELL_RUSTED_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BLACK_KORKRON_PROTO_DRAKE", 0, 0, SPELL_BLACK_KORKRON_PROTO_DRAKE_100, 0, SPELL_BLACK_KORKRON_PROTO_DRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SPAWN_OF_GALAKRAS", 0, 0, SPELL_SPAWN_OF_GALAKRAS_100, 0, SPELL_SPAWN_OF_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TIME_LOST_GALAKRAS", 0, 0, SPELL_TIME_LOST_GALAKRAS_100, 0, SPELL_TIME_LOST_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_GREEN_GALAKRAS", 0, 0, SPELL_GREEN_GALAKRAS_100, 0, SPELL_GREEN_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_VIOLET_GALAKRAS", 0, 0, SPELL_VIOLET_GALAKRAS_100, 0, SPELL_VIOLET_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_PLAGUED_GALAKRAS", 0, 0, SPELL_PLAGUED_GALAKRAS_100, 0, SPELL_PLAGUED_GALAKRAS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_MOTTLED_DRAKE", 0, 0, SPELL_MOTTLED_DRAKE_100, 0, SPELL_MOTTLED_DRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TWILIGHT_AVENGER", 0, 0, SPELL_TWILIGHT_AVENGER_100, 0, SPELL_TWILIGHT_AVENGER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_EMERALD_DRAKE", 0, 0, SPELL_EMERALD_DRAKE_100, 0, SPELL_EMERALD_DRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ENCHANTED_FEY_DRAGON", 0, 0, SPELL_ENCHANTED_FEY_DRAGON_100, 0, SPELL_ENCHANTED_FEY_DRAGON_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_UNCORRUPTED_VOIDWING", 0, 0, SPELL_UNCORRUPTED_VOIDWING_100, 0, SPELL_UNCORRUPTED_VOIDWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_VOID_DRAGON_MOUNT", 0, 0, SPELL_VOID_DRAGON_MOUNT_100, 0, SPELL_VOID_DRAGON_MOUNT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_VALARJAR_STORMWING", 0, 0, SPELL_VALARJAR_STORMWING_100, 0, SPELL_VALARJAR_STORMWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_ISLAND_THUNDERSCALE", 0, 0, SPELL_ISLAND_THUNDERSCALE_100, 0, SPELL_ISLAND_THUNDERSCALE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TEAL_STORMDRAGON", 0, 0, SPELL_TEAL_STORMDRAGON_100, 0, SPELL_TEAL_STORMDRAGON_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DEATHLORD_VILEBROOD_GREEN", 0, 0, SPELL_DEATHLORD_VILEBROOD_GREEN_100, 0, SPELL_DEATHLORD_VILEBROOD_GREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DEATHLORD_VILEBROOD_BLUE", 0, 0, SPELL_DEATHLORD_VILEBROOD_BLUE_100, 0, SPELL_DEATHLORD_VILEBROOD_BLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DEATHLORD_VILEBROOD_RED", 0, 0, SPELL_DEATHLORD_VILEBROOD_RED_100, 0, SPELL_DEATHLORD_VILEBROOD_RED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_TARECGOSA", 0, 0, SPELL_TARECGOSA_100, 0, SPELL_TARECGOSA_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DRAKE_OF_EAST_WIND", 0, 0, SPELL_DRAKE_OF_EAST_WIND_100, 0, SPELL_DRAKE_OF_EAST_WIND_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DRAKE_OF_SOUTH_WIND", 0, 0, SPELL_DRAKE_OF_SOUTH_WIND_100, 0, SPELL_DRAKE_OF_SOUTH_WIND_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DRAKE_OF_FOUR_WINDS", 0, 0, SPELL_DRAKE_OF_FOUR_WINDS_100, 0, SPELL_DRAKE_OF_FOUR_WINDS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DRAKE_OF_NORTH_WIND", 0, 0, SPELL_DRAKE_OF_NORTH_WIND_100, 0, SPELL_DRAKE_OF_NORTH_WIND_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SYLVERIAN_DREAMER", 0, 0, SPELL_SYLVERIAN_DREAMER_100, 0, SPELL_SYLVERIAN_DREAMER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SYLVERIAN_DREAMER_GREEN", 0, 0, SPELL_SYLVERIAN_DREAMER_GREEN_100, 0, SPELL_SYLVERIAN_DREAMER_GREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SYLVERIAN_DREAMER_RED", 0, 0, SPELL_SYLVERIAN_DREAMER_RED_100, 0, SPELL_SYLVERIAN_DREAMER_RED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_SYLVERIAN_DREAMER_ORANGE", 0, 0, SPELL_SYLVERIAN_DREAMER_ORANGE_100, 0, SPELL_SYLVERIAN_DREAMER_ORANGE_310, 0);

    // PvP Mounts |||  |||  |||

    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_MALEVOLENT_CLOUD_SERP", 0, 0, SPELL_CUSTOM_MALEVOLENT_CLOUD_SERP_100, 0, SPELL_CUSTOM_MALEVOLENT_CLOUD_SERP_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_TYRANNICAL_CLOUD_SERP", 0, 0, SPELL_CUSTOM_TYRANNICAL_CLOUD_SERP_100, 0, SPELL_CUSTOM_TYRANNICAL_CLOUD_SERP_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_VIOLET_GLADIATOR_SERP", 0, 0, SPELL_CUSTOM_VIOLET_GLADIATOR_SERP_100, 0, SPELL_CUSTOM_VIOLET_GLADIATOR_SERP_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_GREEN_GLADIATOR_SERP", 0, 0, SPELL_CUSTOM_GREEN_GLADIATOR_SERP_100, 0, SPELL_CUSTOM_GREEN_GLADIATOR_SERP_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_VINDICTIVE_GLAD", 0, 0, SPELL_CUSTOM_VINDICTIVE_GLAD_100, 0, SPELL_CUSTOM_VINDICTIVE_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_FEARLESS_GLAD", 0, 0, SPELL_CUSTOM_FEARLESS_GLAD_100, 0, SPELL_CUSTOM_FEARLESS_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_CRUEL_GLAD", 0, 0, SPELL_CUSTOM_CRUEL_GLAD_100, 0, SPELL_CUSTOM_CRUEL_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CUSTOM_DOMINANT_GLAD", 0, 0, SPELL_CUSTOM_DOMINANT_GLAD_100, 0, SPELL_CUSTOM_DOMINANT_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DEMONIC_GLADIATOR_GLAD", 0, 0, SPELL_DEMONIC_GLADIATOR_GLAD_100, 0, SPELL_DEMONIC_GLADIATOR_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_BFA_PALE_PROTO_DRAKE", 0, 0, SPELL_BFA_PALE_PROTO_DRAKE_100, 0, SPELL_BFA_PALE_PROTO_DRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_UNCHAINED_GLAD_SOULEATER_GREEN", 0, 0, SPELL_UNCHAINED_GLAD_SOULEATER_GREEN_100, 0, SPELL_UNCHAINED_GLAD_SOULEATER_GREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_CRIMSON_GLADIATOR_DRAKE", 0, 0, SPELL_CRIMSON_GLADIATOR_DRAKE_100, 0, SPELL_CRIMSON_GLADIATOR_DRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DF_S2_GLAD", 0, 0, SPELL_DF_S2_GLAD_100, 0, SPELL_DF_S2_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DF_S3_GLAD", 0, 0, SPELL_DF_S3_GLAD_100, 0, SPELL_DF_S3_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DF_S4_GLAD", 0, 0, SPELL_DF_S4_GLAD_100, 0, SPELL_DF_S4_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DF_S5_GLAD", 0, 0, SPELL_DF_S5_GLAD_100, 0, SPELL_DF_S5_GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "SPELL_DF_S6_GLAD", 0, 0, SPELL_DF_S6_GLAD_100, 0, SPELL_DF_S6_GLAD_310, 0);

    // Mounts Cata:
    // Mounts MoP:
    // Mounts WoD:
    // Mounts Legion:
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONDARKBLUEGLAD", 0, 0, SPELL_LEGIONDARKBLUEGLAD_100, 0, SPELL_LEGIONDARKBLUEGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONBLUEGLAD", 0, 0, SPELL_LEGIONBLUEGLAD_100, 0, SPELL_LEGIONBLUEGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONGLADWHITE", 0, 0, SPELL_LEGIONGLADWHITE_100, 0, SPELL_LEGIONGLADWHITE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONGLADEMERALD", 0, 0, SPELL_LEGIONGLADEMERALD_100, 0, SPELL_LEGIONGLADEMERALD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONGLADGREEN", 0, 0, SPELL_LEGIONGLADGREEN_100, 0, SPELL_LEGIONGLADGREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONGLADBLACK", 0, 0, SPELL_LEGIONGLADBLACK_100, 0, SPELL_LEGIONGLADBLACK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LEGIONGLADRED", 0, 0, SPELL_LEGIONGLADRED_100, 0, SPELL_LEGIONGLADRED_310, 0);
    // Mounts BFA:
    // Mounts Shadowlands:
    // Mounts DF:
    // Mounts Cataclysm+:
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_tyraelcharger", 0, SPELL_TYRAELCHARGER_60, SPELL_TYRAELCHARGER_100, SPELL_TYRAELCHARGER_150, SPELL_TYRAELCHARGER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_sinfulglad", 0, 0, SPELL_SINFULGLAD_100, 0, SPELL_SINFULGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_UNCHAINEDGLAD", 0, 0, SPELL_UNCHAINEDGLAD_100, 0, SPELL_UNCHAINEDGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_COSMICGLAD", 0, 0, SPELL_COSMICGLAD_100, 0, SPELL_COSMICGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ETERNALGLAD", 0, 0, SPELL_ETERNALGLAD_100, 0, SPELL_ETERNALGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VINDICTIVEGLAD", 0, 0, SPELL_VINDICTIVEGLAD_100, 0, SPELL_VINDICTIVEGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FEARLESSGLAD", 0, 0, SPELL_FEARLESSGLAD_100, 0, SPELL_FEARLESSGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CRUELGLAD", 0, 0, SPELL_CRUELGLAD_100, 0, SPELL_CRUELGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FEROCIOUSGLAD", 0, 0, SPELL_FEROCIOUSGLAD_100, 0, SPELL_FEROCIOUSGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FIERCEGLAD", 0, 0, SPELL_FIERCEGLAD_100, 0, SPELL_FIERCEGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DOMINANTGLAD", 0, 0, SPELL_DOMINANTGLAD_100, 0, SPELL_DOMINANTGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLAD", 0, 0, SPELL_VICIOUSGLAD_100, 0, SPELL_VICIOUSGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RUTHLESSGLAD", 0, 0, SPELL_RUTHLESSGLAD_100, 0, SPELL_RUTHLESSGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CATAGLAD", 0, 0, SPELL_CATAGLAD_100, 0, SPELL_CATAGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADBLUE", 0, 0, SPELL_VICIOUSGLADBLUE_100, 0, SPELL_VICIOUSGLADBLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADAZURE", 0, 0, SPELL_VICIOUSGLADAZURE_100, 0, SPELL_VICIOUSGLADAZURE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADAQUA", 0, 0, SPELL_VICIOUSGLADAQUA_100, 0, SPELL_VICIOUSGLADAQUA_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADEMERALD", 0, 0, SPELL_VICIOUSGLADEMERALD_100, 0, SPELL_VICIOUSGLADEMERALD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADALBINO", 0, 0, SPELL_VICIOUSGLADALBINO_100, 0, SPELL_VICIOUSGLADALBINO_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADBLACK", 0, 0, SPELL_VICIOUSGLADBLACK_100, 0, SPELL_VICIOUSGLADBLACK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADPINK", 0, 0, SPELL_VICIOUSGLADPINK_100, 0, SPELL_VICIOUSGLADPINK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADMOTTLED", 0, 0, SPELL_VICIOUSGLADMOTTLED_100, 0, SPELL_VICIOUSGLADMOTTLED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADBRONZE", 0, 0, SPELL_VICIOUSGLADBRONZE_100, 0, SPELL_VICIOUSGLADBRONZE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VICIOUSGLADRED", 0, 0, SPELL_VICIOUSGLADRED_100, 0, SPELL_VICIOUSGLADRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_BLAZINGDRAKE", 0, 0, SPELL_BLAZINGDRAKE_100, 0, SPELL_BLAZINGDRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_LIFEBINDERHANDMAIDEN", 0, 0, SPELL_LIFEBINDERHANDMAIDEN_100, 0, SPELL_LIFEBINDERHANDMAIDEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TWILIGHTHARBINGER", 0, 0, SPELL_TWILIGHTHARBINGER_100, 0, SPELL_TWILIGHTHARBINGER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_INFINITETIMEREAVER", 0, 0, SPELL_INFINITETIMEREAVER_100, 0, SPELL_INFINITETIMEREAVER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_OBSIDIANWORLDBREAKER", 0, 0, SPELL_OBSIDIANWORLDBREAKER_100, 0, SPELL_OBSIDIANWORLDBREAKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FELDRAKE", 0, 0, SPELL_FELDRAKE_100, 0, SPELL_FELDRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TAZAVESHGEARGLIDER", 0, 0, SPELL_TAZAVESHGEARGLIDER_100, 0, SPELL_TAZAVESHGEARGLIDER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PILFEREDGEARGLIDER", 0, 0, SPELL_PILFEREDGEARGLIDER_100, 0, SPELL_PILFEREDGEARGLIDER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CARTELMASTERGEARGLIDER", 0, 0, SPELL_CARTELMASTERGEARGLIDER_100, 0, SPELL_CARTELMASTERGEARGLIDER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CARTELMASTERGEARGLIDERSILVER", 0, 0, SPELL_CARTELMASTERGEARGLIDERSILVER_100, 0, SPELL_CARTELMASTERGEARGLIDERSILVER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HUNTMASTERLOYALWOLFHAWK1", 0, 0, SPELL_HUNTMASTERLOYALWOLFHAWK1_100, 0, SPELL_HUNTMASTERLOYALWOLFHAWK1_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HUNTMASTERLOYALWOLFHAWK2", 0, 0, SPELL_HUNTMASTERLOYALWOLFHAWK2_100, 0, SPELL_HUNTMASTERLOYALWOLFHAWK2_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HUNTMASTERLOYALWOLFHAWK3", 0, 0, SPELL_HUNTMASTERLOYALWOLFHAWK3_100, 0, SPELL_HUNTMASTERLOYALWOLFHAWK3_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_WINGEDGUARDIAN", 0, 0, SPELL_WINGEDGUARDIAN_100, 0, SPELL_WINGEDGUARDIAN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEARTHSTEED", 0, 0, SPELL_HEARTHSTEED_100, 0, SPELL_HEARTHSTEED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CINDERMANECHARGER", 0, 0, SPELL_CINDERMANECHARGER_100, 0, SPELL_CINDERMANECHARGER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PUREBLOODFIREHAWK", 0, 0, SPELL_PUREBLOODFIREHAWK_100, 0, SPELL_PUREBLOODFIREHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FELFIREHAWK", 0, 0, SPELL_FELFIREHAWK_100, 0, SPELL_FELFIREHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CORRUPTEDFIREHAWK", 0, 0, SPELL_CORRUPTEDFIREHAWK_100, 0, SPELL_CORRUPTEDFIREHAWK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FELHOUNDMOUNTRED", 0, 0, SPELL_FELHOUNDMOUNTRED_100, 0, SPELL_FELHOUNDMOUNTRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_FELHOUNDMOUNTPURPLE", 0, 0, SPELL_FELHOUNDMOUNTPURPLE_100, 0, SPELL_FELHOUNDMOUNTPURPLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_JCPANTHERBLACK", 0, 0, SPELL_JCPANTHERBLACK_100, 0, SPELL_JCPANTHERBLACK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_JCPANTHERRED", 0, 0, SPELL_JCPANTHERRED_100, 0, SPELL_JCPANTHERRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_JCPANTHERBLUE", 0, 0, SPELL_JCPANTHERBLUE_100, 0, SPELL_JCPANTHERBLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_JCPANTHERGREEN", 0, 0, SPELL_JCPANTHERGREEN_100, 0, SPELL_JCPANTHERGREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_JCPANTHERYELLOW", 0, 0, SPELL_JCPANTHERYELLOW_100, 0, SPELL_JCPANTHERYELLOW_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DREADRAVEN", 0, 0, SPELL_DREADRAVEN_100, 0, SPELL_DREADRAVEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_KARAZHANDRAKE", 0, 0, SPELL_KARAZHANDRAKE_100, 0, SPELL_KARAZHANDRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VENGEANCE", 0, 0, SPELL_VENGEANCE_100, 0, SPELL_VENGEANCE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GLACIALTIDESTORM", 0, 0, SPELL_GLACIALTIDESTORM_100, 0, SPELL_GLACIALTIDESTORM_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TIDESTORMGREEN", 0, 0, SPELL_TIDESTORMGREEN_100, 0, SPELL_TIDESTORMGREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TIDESTORMPURPLE", 0, 0, SPELL_TIDESTORMPURPLE_100, 0, SPELL_TIDESTORMPURPLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_TIDESTORMRED", 0, 0, SPELL_TIDESTORMRED_100, 0, SPELL_TIDESTORMRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_WARFORGEDNIGHTMARE", 0, 0, SPELL_WARFORGEDNIGHTMARE_100, 0, SPELL_WARFORGEDNIGHTMARE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEAVENLYONYX", 0, 0, SPELL_HEAVENLYONYX_100, 0, SPELL_HEAVENLYONYX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEAVENLYGOLDEN", 0, 0, SPELL_HEAVENLYGOLDEN_100, 0, SPELL_HEAVENLYGOLDEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEAVENLYBLUE", 0, 0, SPELL_HEAVENLYBLUE_100, 0, SPELL_HEAVENLYBLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEAVENLYGREEN", 0, 0, SPELL_HEAVENLYGREEN_100, 0, SPELL_HEAVENLYGREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HEAVENLYRED", 0, 0, SPELL_HEAVENLYRED_100, 0, SPELL_HEAVENLYRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_THUNDERINGCOBALT", 0, 0, SPELL_THUNDERINGCOBALT_100, 0, SPELL_THUNDERINGCOBALT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RAJANIWARSERPENT", 0, 0, SPELL_RAJANIWARSERPENT_100, 0, SPELL_RAJANIWARSERPENT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_THUNDERINGAUGUST", 0, 0, SPELL_THUNDERINGAUGUST_100, 0, SPELL_THUNDERINGAUGUST_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_THUNDERINGJADE", 0, 0, SPELL_THUNDERINGJADE_100, 0, SPELL_THUNDERINGJADE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_THUNDERINGONYX", 0, 0, SPELL_THUNDERINGONYX_100, 0, SPELL_THUNDERINGONYX_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_THUNDERINGRUBY", 0, 0, SPELL_THUNDERINGRUBY_100, 0, SPELL_THUNDERINGRUBY_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_S12GLAD", 0, 0, SPELL_S12GLAD_100, 0, SPELL_S12GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_S13GLAD", 0, 0, SPELL_S13GLAD_100, 0, SPELL_S13GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_S14GLAD", 0, 0, SPELL_S14GLAD_100, 0, SPELL_S14GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_S15GLAD", 0, 0, SPELL_S15GLAD_100, 0, SPELL_S15GLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DREADGLAD", 0, 0, SPELL_DREADGLAD_100, 0, SPELL_DREADGLAD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SINISTERGLADIATOR", 0, 0, SPELL_SINISTERGLADIATOR_100, 0, SPELL_SINISTERGLADIATOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NOTORIOUSGLADIATOR", 0, 0, SPELL_NOTORIOUSGLADIATOR_100, 0, SPELL_NOTORIOUSGLADIATOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CORRUPTEDGLADIATOR", 0, 0, SPELL_CORRUPTEDGLADIATOR_100, 0, SPELL_CORRUPTEDGLADIATOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SOULTWISTEDDEATHWALKER", 0, 0, SPELL_SOULTWISTEDDEATHWALKER_100, 0, SPELL_SOULTWISTEDDEATHWALKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SINTOUCHEDDEATHWALKER", 0, 0, SPELL_SINTOUCHEDDEATHWALKER_100, 0, SPELL_SINTOUCHEDDEATHWALKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SOULTWISTEDDEATHWALKERRECOLOR", 0, 0, SPELL_SOULTWISTEDDEATHWALKERRECOLOR_100, 0, SPELL_SOULTWISTEDDEATHWALKERRECOLOR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_WASTEWARPEDDEATHWALKER", 0, 0, SPELL_WASTEWARPEDDEATHWALKER_100, 0, SPELL_WASTEWARPEDDEATHWALKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RESTORATIONDEATHWALKER", 0, 0, SPELL_RESTORATIONDEATHWALKER_100, 0, SPELL_RESTORATIONDEATHWALKER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RAGINGTEMPESTFIRE", 0, 0, SPELL_RAGINGTEMPESTFIRE_100, 0, SPELL_RAGINGTEMPESTFIRE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RAGINGTEMPESTRESTO", 0, 0, SPELL_RAGINGTEMPESTRESTO_100, 0, SPELL_RAGINGTEMPESTRESTO_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_CATAFLYINGPANTHER", 0, 0, SPELL_CATAFLYINGPANTHER_100, 0, SPELL_CATAFLYINGPANTHER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RAVAGERMOUNT", 0, 0, SPELL_RAVAGERMOUNT_100, 0, SPELL_RAVAGERMOUNT_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DREADSTEEDGREEN", 0, 0, SPELL_DREADSTEEDGREEN_100, 0, SPELL_DREADSTEEDGREEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DREADSTEEDRED", 0, 0, SPELL_DREADSTEEDRED_100, 0, SPELL_DREADSTEEDRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DREADSTEEDPURPLE", 0, 0, SPELL_DREADSTEEDPURPLE_100, 0, SPELL_DREADSTEEDPURPLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SQUEAKERS", 0, 0, SPELL_SQUEAKERS_100, 0, SPELL_SQUEAKERS_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ASTRALCLOUD", 0, 0, SPELL_ASTRALCLOUD_100, 0, SPELL_ASTRALCLOUD_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PURPLEDREADRAVEN", 0, 0, SPELL_PURPLEDREADRAVEN_100, 0, SPELL_PURPLEDREADRAVEN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_GREY", 0, 0, SPELL_HIGHLAND_DRAKE_GREY_100, 0, SPELL_HIGHLAND_DRAKE_GREY_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_YELLOW", 0, 0, SPELL_HIGHLAND_DRAKE_YELLOW_100, 0, SPELL_HIGHLAND_DRAKE_YELLOW_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_RED", 0, 0, SPELL_HIGHLAND_DRAKE_RED_100, 0, SPELL_HIGHLAND_DRAKE_RED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_BLUE", 0, 0, SPELL_HIGHLAND_DRAKE_BLUE_100, 0, SPELL_HIGHLAND_DRAKE_BLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_BLACK", 0, 0, SPELL_HIGHLAND_DRAKE_BLACK_100, 0, SPELL_HIGHLAND_DRAKE_BLACK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_HIGHLAND_DRAKE_GREEN", 0, 0, SPELL_HIGHLAND_DRAKE_GREEN_100, 0, SPELL_HIGHLAND_DRAKE_GREEN_310, 0);

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PALADINHIGHLORDBLUE", 0, 0, SPELL_PALADINHIGHLORDBLUE_100, 0, SPELL_PALADINHIGHLORDBLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PALADINHIGHLORDPURPLE", 0, 0, SPELL_PALADINHIGHLORDPURPLE_100, 0, SPELL_PALADINHIGHLORDPURPLE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PALADINHIGHLORDYELLOW", 0, 0, SPELL_PALADINHIGHLORDYELLOW_100, 0, SPELL_PALADINHIGHLORDYELLOW_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_PALADINHIGHLORDRED", 0, 0, SPELL_PALADINHIGHLORDRED_100, 0, SPELL_PALADINHIGHLORDRED_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_NZOTHSERP", 0, 0, SPELL_NZOTHSERP_100, 0, SPELL_NZOTHSERP_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_SUNWARMEDFURLINE", 0, 0, SPELL_SUNWARMEDFURLINE_100, 0, SPELL_SUNWARMEDFURLINE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DIVINEKISSOFOHNAHRA", 0, 0, SPELL_DIVINEKISSOFOHNAHRA_100, 0, SPELL_DIVINEKISSOFOHNAHRA_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ZENETHATCHLING", 0, 0, SPELL_ZENETHATCHLING_100, 0, SPELL_ZENETHATCHLING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ZENETHATCHLINGBLUE", 0, 0, SPELL_ZENETHATCHLINGBLUE_100, 0, SPELL_ZENETHATCHLINGBLUE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VULPINEFAMILIAR", 0, 0, SPELL_VULPINEFAMILIAR_100, 0, SPELL_VULPINEFAMILIAR_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_VULPINEFAMILIARYELLOW", 0, 0, SPELL_VULPINEFAMILIARYELLOW_100, 0, SPELL_VULPINEFAMILIARYELLOW_310, 0);

    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_MECHAGONPEACEK", 0, 0, SPELL_MECHAGONPEACEK_100, 0, SPELL_MECHAGONPEACEK_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GROTTONETHERWING", 0, 0, SPELL_GROTTONETHERWING_100, 0, SPELL_GROTTONETHERWING_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_ELEMENTIUMDRAKE", 0, 0, SPELL_ELEMENTIUMDRAKE_100, 0, SPELL_ELEMENTIUMDRAKE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_AZUREWORLDCH", 0, 0, SPELL_AZUREWORLDCH_100, 0, SPELL_AZUREWORLDCH_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_AMALGAMRAGE", 0, 0, SPELL_AMALGAMRAGE_100, 0, SPELL_AMALGAMRAGE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GRAVEWINGPALE", 0, 0, SPELL_GRAVEWINGPALE_100, 0, SPELL_GRAVEWINGPALE_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GRAVEWINGSINFALL", 0, 0, SPELL_GRAVEWINGSINFALL_100, 0, SPELL_GRAVEWINGSINFALL_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_GRAVEWINGOBSIDIAN", 0, 0, SPELL_GRAVEWINGOBSIDIAN_100, 0, SPELL_GRAVEWINGOBSIDIAN_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_RAPTORASWOOPER", 0, 0, SPELL_RAPTORASWOOPER_100, 0, SPELL_RAPTORASWOOPER_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_MAWDAPTEDRAPTORA", 0, 0, SPELL_MAWDAPTEDRAPTORA_100, 0, SPELL_MAWDAPTEDRAPTORA_310, 0);
    RegisterSpellScriptWithArgs(spell_gen_mount, "spell_DESERTWINGHUNTER", 0, 0, SPELL_DESERTWINGHUNTER_100, 0, SPELL_DESERTWINGHUNTER_310, 0);
}
