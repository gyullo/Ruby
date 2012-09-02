/*ScriptDataStart
SDName: ruby_sanctum.cpp
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 100%
SDComment:
ScriptDataEnd*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ruby_sanctum.h"

enum Spells
{
	// Charscale Assaulter
	SPELL_CLEAVE					= 15284,
	SPELL_SHOCKWAVE					= 75417,
	SPELL_SHOCKWAVE_25MAN			= 75418,

	// Charscale Commander
	SPELL_MORTAL_STRIKE				= 13737,
	SPELL_RALLYING_SHOUT			= 75414,

	// Charscale Elite
	SPELL_SKULL_CRACK				= 15621,

	// Charscale Invoker
	SPELL_FLAME_WAVE				= 75413,
	SPELL_SCORCH					= 75412,
	SPELL_SCORCH_25MAN				= 75419,
};

enum Events
{
	EVENT_CLEAVE			= 1,
	EVENT_SHOCKWAVE			= 2,
	EVENT_MORTAL_STRIKE		= 3,
	EVENT_SKULL_CRACK		= 4,
	EVENT_FLAME_WAVE		= 5,
	EVENT_SCORCH			= 6,
};

class npc_charscale_assaulter : public CreatureScript
{
	public:
		npc_charscale_assaulter() : CreatureScript("npc_charscale_assaulter") { }

		struct npc_charscale_assaulterAI : public ScriptedAI
		{
			npc_charscale_assaulterAI(Creature* creature) : ScriptedAI(creature)
			{
			}

			void Reset()
			{
				_events.Reset();
				_events.ScheduleEvent(EVENT_CLEAVE, urand(9000, 13000));
				_events.ScheduleEvent(EVENT_SHOCKWAVE, urand(15000, 20000));
			}

			void JustDied(Unit* /*killer*/)
			{
				_events.Reset();
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				_events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

				while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
						case EVENT_CLEAVE:
							DoCastVictim(SPELL_CLEAVE);
							_events.ScheduleEvent(EVENT_CLEAVE, urand(9000, 13000));
							break;
						case EVENT_SHOCKWAVE:
							if (Is25ManRaid())
								DoCastVictim(SPELL_SHOCKWAVE_25MAN);
							else
								DoCastVictim(SPELL_SHOCKWAVE);
							_events.ScheduleEvent(EVENT_SHOCKWAVE, urand(20000, 25000));
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
			return GetRubySanctumAI<npc_charscale_assaulterAI>(creature);
		}
};

class npc_charscale_commander : public CreatureScript
{
	public:
		npc_charscale_commander() : CreatureScript("npc_charscale_commander") { }

		struct npc_charscale_commanderAI : public ScriptedAI
		{
			npc_charscale_commanderAI(Creature* creature) : ScriptedAI(creature)
			{
			}

			void Reset()
			{
				_events.Reset();
			}

			void EnterCombat(Unit* /*who*/)
			{
				_events.Reset();
				_events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(9000, 13000));
				DoCast(SPELL_RALLYING_SHOUT);
			}

			void JustDied(Unit* /*killer*/)
			{
				_events.Reset();
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				_events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
					return;

				if (!me->HasAura(SPELL_RALLYING_SHOUT))
					DoCast(SPELL_RALLYING_SHOUT);

				while (uint32 eventId = _events.ExecuteEvent())
                {
					if (eventId == EVENT_MORTAL_STRIKE)
					{
						DoCastVictim(SPELL_MORTAL_STRIKE);
						_events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(9000, 13000));
					}
				}

				DoMeleeAttackIfReady();
			}

		private:
			EventMap _events;
		};

		CreatureAI* GetAI(Creature* creature) const
		{
			return GetRubySanctumAI<npc_charscale_commanderAI>(creature);
		}
};

class npc_charscale_elite : public CreatureScript
{
	public:
		npc_charscale_elite() : CreatureScript("npc_charscale_elite") { }

		struct npc_charscale_eliteAI : public ScriptedAI
		{
			npc_charscale_eliteAI(Creature* creature) : ScriptedAI(creature)
			{
			}

			void Reset()
			{
				_events.Reset();
				_events.ScheduleEvent(EVENT_SKULL_CRACK, urand(9000, 13000));
			}

			void JustDied(Unit* /*killer*/)
			{
				_events.Reset();
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				_events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
					return;

				while (uint32 eventId = _events.ExecuteEvent())
                {
					if (eventId == EVENT_SKULL_CRACK)
					{
						DoCastVictim(SPELL_SKULL_CRACK);
						_events.ScheduleEvent(EVENT_SKULL_CRACK, urand(14000, 19000));
					}
				}

				DoMeleeAttackIfReady();
			}

		private:
			EventMap _events;
		};

		CreatureAI* GetAI(Creature* creature) const
		{
			return GetRubySanctumAI<npc_charscale_eliteAI>(creature);
		}
};

class npc_charscale_invoker : public CreatureScript
{
	public:
		npc_charscale_invoker() : CreatureScript("npc_charscale_invoker") { }

		struct npc_charscale_invokerAI : public ScriptedAI
		{
			npc_charscale_invokerAI(Creature* creature) : ScriptedAI(creature)
			{
			}

			void Reset()
			{
				_events.Reset();
				_events.ScheduleEvent(EVENT_FLAME_WAVE, 7000);
				_events.ScheduleEvent(EVENT_SCORCH, 10000);
			}

			void JustDied(Unit* /*killer*/)
			{
				_events.Reset();
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				_events.Update(diff);

				if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

				while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
						case EVENT_FLAME_WAVE:
							DoCast(me, SPELL_FLAME_WAVE);
							_events.ScheduleEvent(EVENT_FLAME_WAVE, 25000);
							break;
						case EVENT_SCORCH:
							if (Is25ManRaid())
								DoCastVictim(SPELL_SCORCH_25MAN);
							else
								DoCastVictim(SPELL_SCORCH);
							_events.ScheduleEvent(EVENT_SCORCH, 5000);
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
			return GetRubySanctumAI<npc_charscale_invokerAI>(creature);
		}
};

void AddSC_ruby_sanctum()
{
	new npc_charscale_assaulter();
	new npc_charscale_commander();
	new npc_charscale_elite();
	new npc_charscale_invoker();
}
