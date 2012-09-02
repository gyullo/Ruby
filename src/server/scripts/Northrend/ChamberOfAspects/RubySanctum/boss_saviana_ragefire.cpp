/*ScriptDataStart
SDName: boss_saviana_ragefire.cpp
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 98%
SDComment:
ScriptDataEnd*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "ruby_sanctum.h"

enum Texts
{
	SAY_AGGRO					= 0,
	SAY_KILL					= 1,
	SAY_CONFLAGRATION			= 2,
	EMOTE_ENRAGE				= 3,
};

enum Spells
{
	SPELL_FLAME_BREATH						= 74403,
	SPELL_FLAME_BREATH_25					= 74404,
	SPELL_FLAME_BEACON						= 74453,
	SPELL_CONFLAGRATION						= 74456,
	SPELL_ENRAGE							= 78722,
};

enum Events
{
	EVENT_GROUP_LAND_PHASE				= 1,
	EVENT_FLAME_BREATH					= 1,
	EVENT_AIR_PHASE						= 3,
	EVENT_ENRAGE						= 4,
	EVENT_LAND							= 5,
};

enum MovementPoints
{
	POINT_AIR_PHASE	= 14,
	POINT_LAND		= 15,
};

enum Misc
{
	SOUND_DEATH = 17531,
};

Position const SavianaLandPos = {3151.39f, 636.852f, 78.7396f, 4.68681f};
Position const SavianaAirPos = {3155.51f, 683.844f, 95.20f, 4.69f};;

class boss_saviana_ragefire : public CreatureScript
{
	public:
		boss_saviana_ragefire() : CreatureScript("boss_saviana_ragefire") { }

		struct boss_saviana_ragefireAI : public BossAI
		{
			boss_saviana_ragefireAI(Creature* creature) : BossAI(creature, DATA_SAVIANA_RAGEFIRE)
			{
			}

			void Reset()
			{
				_Reset();
			}

			void EnterCombat(Unit* /*killer*/)
			{
				_EnterCombat();
				Talk(SAY_AGGRO);
				events.ScheduleEvent(EVENT_ENRAGE, 7000, EVENT_GROUP_LAND_PHASE);
				events.ScheduleEvent(EVENT_FLAME_BREATH, 12000, EVENT_GROUP_LAND_PHASE);
				events.ScheduleEvent(EVENT_AIR_PHASE, 32000);
			}
			
			void JustDied(Unit* /*killer*/)
			{
				_JustDied();
				me->FallGround();
				me->PlayDirectSound(SOUND_DEATH);
			}

			void JustReachedHome()
			{
				instance->SetBossState(DATA_SAVIANA_RAGEFIRE, FAIL);
				me->SetFlying(false);
				me->RemoveUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
			}

			void KilledUnit(Unit* victim)
			{
				if (victim->GetTypeId() == TYPEID_PLAYER)
					Talk(SAY_KILL);
			}

			void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
					case POINT_LAND:
					{
						me->SetFlying(false);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
						me->SetReactState(REACT_DEFENSIVE);
                        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                            me->GetMotionMaster()->MovementExpired();
                        DoStartMovement(me->getVictim());
						DoCast(me, SPELL_ENRAGE, true);
						Talk(EMOTE_ENRAGE, me->GetGUID());
						break;
					}
					case POINT_AIR_PHASE:
						me->CastCustomSpell(SPELL_FLAME_BEACON, SPELLVALUE_MAX_TARGETS, RAID_MODE<int32>(2, 5, 2, 5), false);
						Talk(SAY_CONFLAGRATION);
						events.ScheduleEvent(EVENT_LAND, 8000);
						break;
					default:
						break;
				}
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim() || !CheckInRoom())
					return;

				events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
					return;

				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
						case EVENT_FLAME_BREATH:
							if (Is25ManRaid())
								DoCast(me, SPELL_FLAME_BREATH_25);
							else
								DoCast(me, SPELL_FLAME_BREATH);
							events.ScheduleEvent(EVENT_FLAME_BREATH, urand(20000, 25000), EVENT_GROUP_LAND_PHASE);
							break;
						case EVENT_ENRAGE:
							DoCast(me, SPELL_ENRAGE);
							Talk(EMOTE_ENRAGE, me->GetGUID());
							events.ScheduleEvent(EVENT_ENRAGE, urand(15000, 20000), EVENT_GROUP_LAND_PHASE);
							break;
						case EVENT_AIR_PHASE:
						{
							me->RemoveAurasDueToSpell(SPELL_ENRAGE);
                            me->SetFlying(true);
							me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                            me->SetReactState(REACT_PASSIVE);
							me->GetMotionMaster()->MovePoint(POINT_AIR_PHASE, SavianaAirPos);
							events.DelayEvents(8000, EVENT_GROUP_LAND_PHASE);
							events.RescheduleEvent(EVENT_ENRAGE, urand(40000, 45000), EVENT_GROUP_LAND_PHASE);
							events.ScheduleEvent(EVENT_AIR_PHASE, 50000);
							break;
						}
						case EVENT_LAND:
							me->GetMotionMaster()->MovePoint(POINT_LAND, SavianaLandPos);
							break;
						default:
							break;
					}
				}

				DoMeleeAttackIfReady();
			}
		};

		CreatureAI* GetAI(Creature* creature) const
		{
			return GetRubySanctumAI<boss_saviana_ragefireAI>(creature);
		}
};

class spell_saviana_flame_beacon : public SpellScriptLoader
{
    public:
        spell_saviana_flame_beacon() : SpellScriptLoader("spell_saviana_flame_beacon") { }

        class spell_saviana_flame_beacon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_saviana_flame_beacon_AuraScript);

            bool Validate(SpellEntry const* /*spell*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_CONFLAGRATION))
                    return false;
                return true;
            }

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
				caster->CastSpell(GetTarget(), SPELL_CONFLAGRATION, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_saviana_flame_beacon_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_saviana_flame_beacon_AuraScript();
        }
};

void AddSC_boss_saviana_ragefire()
{
	new boss_saviana_ragefire();
	new spell_saviana_flame_beacon();
}
