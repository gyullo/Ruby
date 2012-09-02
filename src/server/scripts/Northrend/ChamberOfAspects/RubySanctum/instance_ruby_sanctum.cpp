/*ScriptDataStart
SDName: instance_ruby_sanctum.cpp
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 100%
SDComment:
ScriptDataEnd*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Map.h"
#include "PoolMgr.h"
#include "ruby_sanctum.h"

DoorData const doorData[] =
{
	{GO_FIRE_FIELD, DATA_BALTHARUS_THE_WARBORN, DOOR_TYPE_PASSAGE, BOUNDARY_E },
	{0, 0, DOOR_TYPE_ROOM, 0},
};

class instance_ruby_sanctum : public InstanceMapScript
{
	public:
		instance_ruby_sanctum() : InstanceMapScript (RSScriptName, 724) { }
	
		struct instance_ruby_sanctum_InstanceMapScript : public InstanceScript
		{
			instance_ruby_sanctum_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
			{
				SetBossNumber(EncounterCount);
				LoadDoorData(doorData);
				BaltharusTheWarbornGUID = 0;
				CrystalChannelTargetGUID = 0;
				SanctumGuardXerestraszaGUID = 0;
				SavianaRagefireGUID = 0;
				GeneralZarithrianGUID = 0;
				FlameWallsGUID = 0;
				HalionGUID = 0;
				HalionTwilightGUID = 0;
				HalionControllerGUID = 0;
				FlameRingGUID = 0;
				TwilightRingGUID = 0;
				memset(ZarithrianSpawnStalkerGUID, 0, 2*sizeof(uint64));
				memset(BurningTreeGUIDs, 0, 4*sizeof(uint64));
			}
			
			void OnCreatureCreate(Creature* creature, bool /*add*/)
			{
                Map::PlayerList const &players = instance->GetPlayers();
				
				switch (creature->GetEntry())
				{
					case NPC_BALTHARUS_THE_WARBORN:
						BaltharusTheWarbornGUID = creature->GetGUID();
						break;
					case NPC_CRYSTAL_CHANNEL_TARGET:
						CrystalChannelTargetGUID = creature->GetGUID();
						break;
					case NPC_SANCTUM_GUARD_XERESTRASZA:
						SanctumGuardXerestraszaGUID = creature->GetGUID();
						break;
					case NPC_SAVIANA_RAGEFIRE:
						SavianaRagefireGUID = creature->GetGUID();
						break;
					case NPC_GENERAL_ZARITHRIAN:
						GeneralZarithrianGUID = creature->GetGUID();
						break;
					case NPC_ZARITHRIAN_SPAWN_STALKER:
						if (!ZarithrianSpawnStalkerGUID[0])
							ZarithrianSpawnStalkerGUID[0] = creature->GetGUID();
						else
							ZarithrianSpawnStalkerGUID[1] = creature->GetGUID();
						break;
					case NPC_HALION:
						HalionGUID = creature->GetGUID();
						break;
					case NPC_HALION_TWILIGHT:
						HalionTwilightGUID = creature->GetGUID();
						break;
					case NPC_HALION_CONTROLLER:
						HalionControllerGUID = creature->GetGUID();
						break;
					default:
						break;
				}
			}

			void OnGameObjectCreate(GameObject* go)
			{
				switch (go->GetEntry())
				{
					case GO_FIRE_FIELD:
						AddDoor(go, true);
						break;
					case GO_FLAME_WALLS:
						FlameWallsGUID = go->GetGUID();
						if (GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE && GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
							HandleGameObject(FlameWallsGUID, true, go);
						break;
					case GO_FLAME_RING:
						FlameRingGUID = go->GetGUID();
						break;
					case GO_TWILIGHT_RING:
						TwilightRingGUID = go->GetGUID();
						break;
					case GO_BURNING_TREE_1:
						BurningTreeGUIDs[0] = go->GetGUID();
						break;
					case GO_BURNING_TREE_2:
						BurningTreeGUIDs[1] = go->GetGUID();
						break;
					case GO_BURNING_TREE_3:
						BurningTreeGUIDs[2] = go->GetGUID();
						break;
					case GO_BURNING_TREE_4:
						BurningTreeGUIDs[3] = go->GetGUID();
						break;
					default:
						break;
				}
			}

			void OnGameObjectRemove(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GO_FIRE_FIELD:
						AddDoor(go, false);
						break;
                    default:
                        break;
                }
            }

			uint64 GetData64(uint32 type)
			{
				switch (type)
				{
					case DATA_BALTHARUS_THE_WARBORN:
						return BaltharusTheWarbornGUID;	
					case DATA_CRYSTAL_CHANNEL_TARGET:
						return CrystalChannelTargetGUID;
					case DATA_XERESTRASZA_EVENT:
						return SanctumGuardXerestraszaGUID;
					case DATA_SAVIANA_RAGEFIRE:
						return SavianaRagefireGUID;
					case DATA_GENERAL_ZARITHRIAN:
						return GeneralZarithrianGUID;
					case DATA_ZARITHRIAN_SPAWN_STALKER_1:
						return ZarithrianSpawnStalkerGUID[0];
					case DATA_ZARITHRIAN_SPAWN_STALKER_2:
						return ZarithrianSpawnStalkerGUID[1];
					case DATA_HALION:
						return HalionGUID;
					case DATA_HALION_TWILIGHT:
						return HalionTwilightGUID;
					case DATA_HALION_CONTROLLER:
						return HalionControllerGUID;
					case DATA_FLAME_RING:
						return FlameRingGUID;
					case DATA_TWILIGHT_RING:
						return TwilightRingGUID;
					case DATA_BURNING_TREE_1:
						return BurningTreeGUIDs[0];
					case DATA_BURNING_TREE_2:
						return BurningTreeGUIDs[1];
					case DATA_BURNING_TREE_3:
						return BurningTreeGUIDs[2];
					case DATA_BURNING_TREE_4:
						return BurningTreeGUIDs[3];
					default:
						break;
				}

				return 0;
			}

			bool SetBossState(uint32 type, EncounterState state)
			{
				if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
					case DATA_BALTHARUS_THE_WARBORN:
						if (state == DONE)
						{
							if (GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE)
								HandleGameObject(FlameWallsGUID, true);
						}
						break;
					case DATA_SAVIANA_RAGEFIRE:
						if (state == DONE)
						{
							if (GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
								HandleGameObject(FlameWallsGUID, true);
						}
						break;
					case DATA_GENERAL_ZARITHRIAN:
						switch (state)
						{
							case DONE:
							{
								HandleGameObject(FlameWallsGUID, true);
								if (Creature* halionController = instance->SummonCreature(NPC_HALION_CONTROLLER, HalionSpawnPos, 0, 604800000, 0, 0))
									halionController->CastSpell(halionController, SPELL_COSMETIC_FIRE_PILLAR, true);

								if (Creature* halionTwilight = instance->SummonCreature(NPC_HALION_TWILIGHT, HalionSpawnPos, 0, 604800000, 0, 0))
									halionTwilight->SetPhaseMask(32, true);

								break;
							}
							case IN_PROGRESS:
								HandleGameObject(FlameWallsGUID, false);
								break;
							case FAIL:
								HandleGameObject(FlameWallsGUID, true);
								break;
							default:
								break;
						}
						break;
					case DATA_HALION:
						switch (state)
						{
							case FAIL:
								HandleGameObject(FlameRingGUID, true);
								break;
							case DONE:
								HandleGameObject(FlameRingGUID, true);
								break;
						}
						break;
					default:
						break;
				}

				return true;
			}

			void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_BALTHARUS_SHARED_HEALTH:
                        BaltharusSharedHealth = data;
                        break;
					case DATA_HALION_SHARED_HEALTH:
						HalionSharedHealth = data;
						break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_BALTHARUS_SHARED_HEALTH:
                        return BaltharusSharedHealth;
					case DATA_HALION_SHARED_HEALTH:
						return HalionSharedHealth;
                    default:
                        break;
                }

                return 0;
            }

			bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
			{
				if (player && player->isGameMaster())
					return true;

				switch (bossId)
				{
					case DATA_GENERAL_ZARITHRIAN:
						if (GetBossState(DATA_BALTHARUS_THE_WARBORN) != DONE || GetBossState(DATA_SAVIANA_RAGEFIRE) != DONE)
							return false;
						break;
					case DATA_HALION:
						if (GetBossState(DATA_BALTHARUS_THE_WARBORN) != DONE || GetBossState(DATA_SAVIANA_RAGEFIRE) != DONE 
							|| GetBossState(DATA_GENERAL_ZARITHRIAN) != DONE)
							return false;
						break;
					default:
						break;
				}

				return true;
			}

			std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R S " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str)
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'R' && dataHead2 == 'S')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

		private:
			uint64 BaltharusTheWarbornGUID;
			uint64 CrystalChannelTargetGUID;
			uint64 SanctumGuardXerestraszaGUID;
			uint64 SavianaRagefireGUID;
			uint64 GeneralZarithrianGUID;
			uint64 ZarithrianSpawnStalkerGUID[2];
			uint64 FlameWallsGUID;
			uint64 HalionGUID;
			uint64 HalionTwilightGUID;
			uint64 HalionControllerGUID;
			uint64 BurningTreeGUIDs[4];
			uint64 FlameRingGUID;
			uint64 TwilightRingGUID;
			uint32 BaltharusSharedHealth;
			uint32 HalionSharedHealth;
			};

		InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_ruby_sanctum_InstanceMapScript(map);
        }
};

void AddSC_instance_ruby_sanctum()
{
    new instance_ruby_sanctum();
}
