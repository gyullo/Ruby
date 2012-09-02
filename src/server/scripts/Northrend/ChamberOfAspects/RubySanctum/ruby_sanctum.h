/*ScriptDataStart
SDName: ruby_sanctum.h
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 100%
SDComment:
ScriptDataEnd*/

#ifndef RUBY_SANCTUM_H_
#define RUBY_SANCTUM_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"

#define RSScriptName "instance_ruby_sanctum"
uint32 const EncounterCount = 4;

Position const HalionSpawnPos = {3155.969f, 536.3088f, 72.8890f, 6.281543f};

enum DataTypes
{
    // Encounter States
    DATA_BALTHARUS_THE_WARBORN			= 0,
	DATA_SAVIANA_RAGEFIRE				= 1,
    DATA_GENERAL_ZARITHRIAN				= 2,
    DATA_HALION							= 3,
	DATA_HALION_TWILIGHT				= 4,

    // Additional Data
	DATA_XERESTRASZA_EVENT				= 5,
    DATA_CRYSTAL_CHANNEL_TARGET			= 6,
	DATA_BALTHARUS_SHARED_HEALTH		= 7,
	DATA_ZARITHRIAN_SPAWN_STALKER_1		= 8,
	DATA_ZARITHRIAN_SPAWN_STALKER_2		= 9,
	DATA_FLAME_RING						= 10,
	DATA_TWILIGHT_RING					= 11,
	DATA_HALION_SHARED_HEALTH			= 12,
	DATA_HALION_CONTROLLER				= 13,
	DATA_BURNING_TREE_1					= 14,
	DATA_BURNING_TREE_2					= 15,
	DATA_BURNING_TREE_3					= 16,
	DATA_BURNING_TREE_4					= 17,
};

enum CreaturesIds
{
    // Baltharus the Warborn
    NPC_BALTHARUS_THE_WARBORN				= 39751,
    NPC_BALTHARUS_THE_WARBORN_CLONE			= 39899,
	NPC_CRYSTAL_CHANNEL_TARGET				= 26712,
	NPC_SANCTUM_GUARD_XERESTRASZA			= 40429,

	// Saviana Ragefire
    NPC_SAVIANA_RAGEFIRE					= 39747,

    // General Zarithrian
    NPC_GENERAL_ZARITHRIAN					= 39746,
    NPC_ONYX_FLAMECALLER					= 39814,
    NPC_ZARITHRIAN_SPAWN_STALKER			= 39794,

    // Halion
    NPC_HALION								= 39863,
    NPC_HALION_TWILIGHT						= 40142,
    NPC_HALION_CONTROLLER					= 40146,
	NPC_HALION_COMBUSTION					= 40001,
	NPC_HALION_CONSUMPTION					= 40135,
    NPC_LIVING_INFERNO						= 40681,
    NPC_LIVING_EMBER						= 40683,
    NPC_ORB_CARRIER							= 40081,
    NPC_ORB_ROTATION_FOCUS					= 40091,
    NPC_SHADOW_ORB_N						= 40083,
    NPC_SHADOW_ORB_S						= 40100,
	NPC_METEOR								= 40029,
	NPC_METEOR_FLAME						= 40041,
	NPC_METEOR_FLAME_2						= 40042,
	NPC_METEOR_FLAME_3						= 40043,
	NPC_METEOR_FLAME_4						= 40044,
	NPC_METEOR_FLAME_5						= 40055,
};

enum SharedTexts
{
	SAY_ARRIVAL			= 0,
};

enum SharedSpells
{
	SPELL_FIERY_EXPLOSION		= 76010,
	SPELL_COSMETIC_FIRE_PILLAR	= 76006,
};

enum SharedActions
{
	// Baltharus the Warborn
	ACTION_BALTHARUS_INTRO		= -3975100,
	ACTION_XERESTRASZA_INTRO	= -4042900,
	ACTION_XERESTRASZA_OUTRO	= -4042901,

	// Halion
	ACTION_METEOR_STRIKE		= -3986300,
	ACTION_METEOR_BURN			= -3986301,
	ACTION_START_INTRO			= -3986302,
};

enum GameObjectsIds
{
    GO_FIRE_FIELD					= 203005,
	GO_FLAME_WALLS					= 203006,
	GO_BURNING_TREE_1				= 203034,
	GO_BURNING_TREE_2				= 203035,
	GO_BURNING_TREE_3				= 203036,
	GO_BURNING_TREE_4				= 203037,
	GO_FLAME_RING					= 203007,
	GO_TWILIGHT_RING				= 203624,
};

enum WorldStatesRS
{
    WORLDSTATE_CORPOREALITY_MATERIAL = 5049,
    WORLDSTATE_CORPOREALITY_TWILIGHT = 5050,
    WORLDSTATE_CORPOREALITY_TOGGLE = 5051,
};

template<class AI>
CreatureAI* GetRubySanctumAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == GetScriptId(RSScriptName))
                return new AI(creature);
    return NULL;
}

#endif // RUBY_SANCTUM_H_
