/*ScriptDataStart
SDName: boss_general_zarithria.cpp
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 85%
SDComment: -don't know if it spawn 2 minions
ScriptDataEnd*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ruby_sanctum.h"

enum Texts
{
	SAY_AGGRO					= 0,
	SAY_KILL					= 1,
	SAY_FLAMECALLER				= 2,
	SAY_DEATH					= 3,
};

enum Spells
{
	SPELL_CLEAVE_ARMOR					= 74367,
	SPELL_INTIMIDATING_ROAR				= 74384,
	SPELL_SUMMON_FLAMECALLER			= 74398,
	SPELL_BLAST_NOVA					= 74392,
	SPELL_BLAST_NOVA_25					= 74393,
	SPELL_LAVA_GOUT						= 74394,
	SPELL_LAVA_GOUT_25					= 74395,
};

enum Events
{
	EVENT_CLEAVE_ARMOR					= 1,
	EVENT_INTIMIDATING_ROAR				= 2,
	EVENT_SUMMON_FLAMECALLER			= 3,
	EVENT_BLAST_NOVA					= 4,
	EVENT_LAVA_GOUT						= 5,
};

class boss_general_zarithrian : public CreatureScript
{
	public:
		boss_general_zarithrian() : CreatureScript("boss_general_zarithrian") { }

		struct boss_general_zarithrianAI : public BossAI
		{
			boss_general_zarithrianAI(Creature* creature) : BossAI(creature, DATA_GENERAL_ZARITHRIAN)
			{
			}

			void Reset()
			{
				_Reset();
			}
			
			void EnterCombat(Unit* who)
			{
				if (!instance->CheckRequiredBosses(DATA_GENERAL_ZARITHRIAN, who->ToPlayer()))
				{
					EnterEvadeMode();
					return;
				}

				_EnterCombat();
				Talk(SAY_AGGRO);
				events.ScheduleEvent(EVENT_CLEAVE_ARMOR, 15000);
				events.ScheduleEvent(EVENT_INTIMIDATING_ROAR, 33000);
				events.ScheduleEvent(EVENT_SUMMON_FLAMECALLER, 45000);
			}

			void JustDied(Unit* /*killer*/)
			{				
				_JustDied();
				Talk(SAY_DEATH);
			}

			void JustReachedHome()
			{
				summons.DespawnAll();
				instance->SetBossState(DATA_GENERAL_ZARITHRIAN, FAIL);
			}

			void KilledUnit(Unit* victim)
			{
				if (victim->GetTypeId() == TYPEID_PLAYER)
					Talk(SAY_KILL);
			}

			void JustSummoned(Creature* summon)
			{
				summons.Summon(summon);
				if (me->isInCombat())
				{
					DoZoneInCombat(summon);
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
						summon->GetMotionMaster()->MoveChase(target);
				}
			}

			void MovementInform(uint32 /*type*/, uint32 /*id*/)
			{
				if (me->GetPositionX() > 3060.0f)
				{
					me->GetMotionMaster()->MoveTargetedHome();
					EnterEvadeMode();
				}
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
					return;

				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
						case EVENT_CLEAVE_ARMOR:
							DoCastVictim(SPELL_CLEAVE_ARMOR);
							events.ScheduleEvent(EVENT_CLEAVE_ARMOR, 15000);
							break;
						case EVENT_INTIMIDATING_ROAR:
							DoCast(me, SPELL_INTIMIDATING_ROAR, true);
							events.ScheduleEvent(EVENT_INTIMIDATING_ROAR, 33000);
							break;
						case EVENT_SUMMON_FLAMECALLER:
							if (Creature* spawnStalker1 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_1)))
								spawnStalker1->AI()->DoCast(spawnStalker1, SPELL_SUMMON_FLAMECALLER);
							if (Creature* spawnStalker2 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_2)))
								spawnStalker2->AI()->DoCast(spawnStalker2, SPELL_SUMMON_FLAMECALLER);
							Talk(SAY_FLAMECALLER);
							events.ScheduleEvent(EVENT_SUMMON_FLAMECALLER, 45000);
							break;
					}
				}

				DoMeleeAttackIfReady();
			}
		};

		CreatureAI* GetAI(Creature* creature) const
		{
			return GetRubySanctumAI<boss_general_zarithrianAI>(creature);
		}
};

class npc_onyx_flamecaller : public CreatureScript
{
	public:
		npc_onyx_flamecaller() : CreatureScript("npc_onyx_flamecaller") { }

		struct npc_onyx_flamecallerAI : public ScriptedAI
		{
			npc_onyx_flamecallerAI(Creature* creature) : ScriptedAI(creature)
			{
			}

			void Reset()
			{
				_events.Reset();
				_events.ScheduleEvent(EVENT_BLAST_NOVA, 7000);
				_events.ScheduleEvent(EVENT_LAVA_GOUT, 10000);
			}

			void JustDied(Unit* /*killer*/)
			{
				_events.Reset();
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				if (me->HasUnitState(UNIT_STAT_CASTING))
					return;

				while (uint32 eventId = _events.ExecuteEvent())
				{
					switch (eventId)
					{
						case EVENT_LAVA_GOUT:
							if (Is25ManRaid())
								DoCastVictim(SPELL_LAVA_GOUT_25);
							else
								DoCastVictim(SPELL_LAVA_GOUT);
							_events.ScheduleEvent(EVENT_LAVA_GOUT, 2000);
							break;
						case EVENT_BLAST_NOVA:
							if (Is25ManRaid())
								DoCastAOE(SPELL_BLAST_NOVA_25);
							else
								DoCastAOE(SPELL_BLAST_NOVA);
							_events.ScheduleEvent(EVENT_BLAST_NOVA, 20000);
							break;
					}
				}

				DoMeleeAttackIfReady();
			}

		private:
			EventMap _events;
		};

		CreatureAI* GetAI(Creature* creature) const
		{
			return GetRubySanctumAI<npc_onyx_flamecallerAI>(creature);
		}
};

class spell_zarithrian_summon_flamecaller : SpellScriptLoader
{
	public:
		spell_zarithrian_summon_flamecaller() : SpellScriptLoader("spell_zarithrian_summon_flamecaller") { }

		class spell_zarithrian_summon_flamecaller_SpellScript : public SpellScript
		{
			PrepareSpellScript(spell_zarithrian_summon_flamecaller_SpellScript);

			void HandleSummon(SpellEffIndex effIndex)
			{
				PreventHitDefaultEffect(effIndex);
				Unit* caster = GetCaster();

				uint32 entry = uint32(GetSpellInfo()->EffectMiscValue[effIndex]);
				SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(uint32(GetSpellInfo()->EffectMiscValueB[effIndex]));
				uint32 duration = uint32(GetSpellDuration(GetSpellInfo()));

				Position pos;
				caster->GetPosition(&pos);

				TempSummon* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster, GetSpellInfo()->Id);

				InstanceScript* instance = caster->GetInstanceScript();
				Creature* generalZarithrian = Unit::GetCreature(*caster, instance->GetData64(DATA_GENERAL_ZARITHRIAN));
				summon->SetCreatorGUID(generalZarithrian->GetGUID());
				generalZarithrian->AI()->JustSummoned(summon);
			}

			void Register()
			{
				OnEffect += SpellEffectFn(spell_zarithrian_summon_flamecaller_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
			}
		};

		SpellScript* GetSpellScript() const
		{
			return new spell_zarithrian_summon_flamecaller_SpellScript();
		}
};

void AddSC_boss_general_zarithrian()
{
	new boss_general_zarithrian();
	new npc_onyx_flamecaller();
	new spell_zarithrian_summon_flamecaller();
}
