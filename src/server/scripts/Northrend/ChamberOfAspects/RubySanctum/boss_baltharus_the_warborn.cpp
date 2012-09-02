/*ScriptDataStart
SDName: boss_baltharus_the_warborn.cpp
SDCategory: Northrend/ChamberOfAspects/RubySanctum
SDAuthor: MindBreaker
SDComplete: 100%
SDComment:
ScriptDataEnd*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "ruby_sanctum.h"

enum Texts
{
    SAY_INTRO = 0, // Your power wanes, ancient one.... Soon you will join your friends.
    SAY_AGGRO = 1, // Ah, the entertainment has arrived.
    SAY_KILL = 2, // Baltharus leaves no survivors! - This world has enough heroes.
    SAY_CLONE = 3, // Twice the pain and half the fun.
    SAY_DEATH = 4, // I... didn't see that coming....

	// Sanctum Guard Xerestrasza
	SAY_XERESTRASZA_0 = 0,
    SAY_XERESTRASZA_1 = 1,
    SAY_XERESTRASZA_2 = 2,
    SAY_XERESTRASZA_3 = 3,
    SAY_XERESTRASZA_4 = 4,
    SAY_XERESTRASZA_5 = 5,
    SAY_XERESTRASZA_6 = 6,
    SAY_XERESTRASZA_7 = 7,
    SAY_XERESTRASZA_INTRO = 8,
};

enum Spells
{
    SPELL_BARRIER_CHANNEL = 76221,
    SPELL_ENERVATING_BRAND = 74502,
    SPELL_SIPHONED_MIGHT = 74507,
    SPELL_CLEAVE = 40504,
    SPELL_BLADE_TEMPEST = 75125,
    SPELL_CLONE = 74511,
    SPELL_REPELLING_WAVE = 74509,
    SPELL_CLEAR_DEBUFFS = 34098,
    SPELL_SPAWN_EFFECT = 64195,
};

enum Events
{
    EVENT_BLADE_TEMPEST = 1,
    EVENT_CLEAVE = 2,
    EVENT_ENERVATING_BRAND = 3,
	EVENT_CLONE = 4,
    EVENT_INTRO_TALK = 5,
	EVENT_OOC_CHANNEL = 6,

	// Sanctum Guard Xerestrasza
    EVENT_XERESTRASZA_OUTRO_1 = 7,
    EVENT_XERESTRASZA_OUTRO_2 = 8,
    EVENT_XERESTRASZA_OUTRO_3 = 9,
    EVENT_XERESTRASZA_OUTRO_4 = 10,
    EVENT_XERESTRASZA_OUTRO_5 = 11,
    EVENT_XERESTRASZA_OUTRO_6 = 12,
    EVENT_XERESTRASZA_OUTRO_7 = 13,
};

enum Actions
{
    ACTION_CLONE = 1,
};

enum Phases
{
    PHASE_ALL = 0,
    PHASE_INTRO = 1,
    PHASE_COMBAT = 2,

    PHASE_INTRO_MASK = 1 << PHASE_INTRO,
};

Position const SanctumGuardXerestraszaOutroPos = {3151.236f, 379.8733f, 86.31996f, 0.0f};

class boss_baltharus_the_warborn : public CreatureScript
{
    public:
        boss_baltharus_the_warborn() : CreatureScript("boss_baltharus_the_warborn") { }

        struct boss_baltharus_the_warbornAI : public BossAI
        {
            boss_baltharus_the_warbornAI(Creature* creature) : BossAI(creature, DATA_BALTHARUS_THE_WARBORN)
            {
				_introDone = false;
            }

            void Reset()
            {
                _Reset();
				events.SetPhase(PHASE_INTRO);
				events.ScheduleEvent(EVENT_OOC_CHANNEL, 1, 0, PHASE_INTRO);
                _cloneCount = RAID_MODE<uint8>(1, 2, 2, 2);
				instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetMaxHealth());
            }

            void EnterCombat(Unit* /*who*/)
            {
                me->InterruptNonMeleeSpells(false);
                _EnterCombat();
                events.SetPhase(PHASE_COMBAT);
                events.ScheduleEvent(EVENT_CLEAVE, 11000, 0, PHASE_COMBAT);
                events.ScheduleEvent(EVENT_ENERVATING_BRAND, 13000, 0, PHASE_COMBAT);
                events.ScheduleEvent(EVENT_BLADE_TEMPEST, 15000, 0, PHASE_COMBAT);
                Talk(SAY_AGGRO);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
                if (Creature* xerestrasza = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_XERESTRASZA_EVENT)))
					xerestrasza->AI()->DoAction(ACTION_XERESTRASZA_OUTRO);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->SetHealth(me->GetHealth());
                summon->CastSpell(summon, SPELL_SPAWN_EFFECT, true);
            }

			void DoAction(int32 const action)
            {
                switch(action)
                {
                    case ACTION_BALTHARUS_INTRO:
                        if (_introDone)
                            return;
                        _introDone = true;
                        me->setActive(true);
                        events.ScheduleEvent(EVENT_INTRO_TALK, 7000, 0, PHASE_INTRO);
                        break;
                    case ACTION_CLONE:
						me->RemoveAurasDueToSpell(SPELL_BLADE_TEMPEST);
						events.ScheduleEvent(EVENT_CLONE, 1000, 0, PHASE_COMBAT);
						break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                switch (GetDifficulty())
				{
					case RAID_DIFFICULTY_10MAN_NORMAL:
					case RAID_DIFFICULTY_10MAN_HEROIC:
						if (me->HealthBelowPctDamaged(50, damage) && _cloneCount == 1)
							DoAction(ACTION_CLONE);
						break;
					case RAID_DIFFICULTY_25MAN_NORMAL:
					case RAID_DIFFICULTY_25MAN_HEROIC:
						if (me->HealthBelowPctDamaged(66, damage) && _cloneCount == 2)
							DoAction(ACTION_CLONE);
						else if (me->HealthBelowPctDamaged(33, damage) && _cloneCount == 1)
							DoAction(ACTION_CLONE);
						break;
					default:
						break;
                }

				if (me->GetHealth() - damage > 0)
					instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetHealth() - damage);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() && !(events.GetPhaseMask() & PHASE_INTRO_MASK))
                    return;

				if (!(events.GetPhaseMask() & PHASE_INTRO_MASK))
					me->SetHealth(instance->GetData(DATA_BALTHARUS_SHARED_HEALTH));

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING) && !(events.GetPhaseMask() & PHASE_INTRO_MASK))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_TALK:
                            Talk(SAY_INTRO);
                            break;
						case EVENT_OOC_CHANNEL:
							if (Creature* channelTarget = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CRYSTAL_CHANNEL_TARGET)))
                                DoCast(channelTarget, SPELL_BARRIER_CHANNEL);
                            break;
						case EVENT_CLONE:
							DoCast(me, SPELL_CLEAR_DEBUFFS);
							DoCast(me, SPELL_CLONE);
							DoCast(me, SPELL_REPELLING_WAVE);
							Talk(SAY_CLONE);
							--_cloneCount;
							break;
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, 24000, 0, PHASE_COMBAT);
                            break;
                        case EVENT_BLADE_TEMPEST:
                            DoCast(me, SPELL_BLADE_TEMPEST);
                            events.ScheduleEvent(EVENT_BLADE_TEMPEST, 24000, 0, PHASE_COMBAT);
                            break;
                        case EVENT_ENERVATING_BRAND:
                            for (uint8 i = 0; i < RAID_MODE<uint8>(4, 8, 8, 10); i++)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 45.0f, true))
                                    DoCast(target, SPELL_ENERVATING_BRAND);
                            events.ScheduleEvent(EVENT_ENERVATING_BRAND, 26000, 0, PHASE_COMBAT);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 _cloneCount;
            bool _introDone;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<boss_baltharus_the_warbornAI>(creature);
        }
};

class npc_sanctum_guard_xerestrasza : public CreatureScript
{
    public:
        npc_sanctum_guard_xerestrasza() : CreatureScript("npc_sanctum_guard_xerestrasza") { }

        struct npc_sanctum_guard_xerestraszaAI : public ScriptedAI
        {
            npc_sanctum_guard_xerestraszaAI(Creature* creature) : ScriptedAI(creature)
            {
				_isIntro = true;
                _introDone = false;
            }

            void Reset()
            {
                _events.Reset();
                me->RemoveFlag(UNIT_NPC_FLAGS, GOSSIP_OPTION_QUESTGIVER);
            }

			void DoAction(int32 const action)
			{
				switch (action)
				{
					case ACTION_XERESTRASZA_INTRO:
						me->setActive(true);
						Talk(SAY_XERESTRASZA_INTRO);
						break;
					case ACTION_XERESTRASZA_OUTRO:
					{
						_isIntro = false;

						Talk(SAY_XERESTRASZA_0);
						me->GetMotionMaster()->MovePoint(0, SanctumGuardXerestraszaOutroPos);

						_events.Reset();
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_1, 16000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_2, 25000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_3, 32000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_4, 42000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_5, 51000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_6, 61000);
						_events.ScheduleEvent(EVENT_XERESTRASZA_OUTRO_7, 69000);
						break;
					}
				}
			}

            void UpdateAI(uint32 const diff)
            {
                if (_isIntro)
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_XERESTRASZA_OUTRO_1:
                            Talk(SAY_XERESTRASZA_1);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_2:
                            Talk(SAY_XERESTRASZA_2);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_3:
                            Talk(SAY_XERESTRASZA_3);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_4:
                            Talk(SAY_XERESTRASZA_4);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_5:
                            Talk(SAY_XERESTRASZA_5);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_6:
                            Talk(SAY_XERESTRASZA_6);
                            break;
                        case EVENT_XERESTRASZA_OUTRO_7:
                            me->SetFlag(UNIT_NPC_FLAGS, GOSSIP_OPTION_QUESTGIVER);
                            Talk(SAY_XERESTRASZA_7);
                            me->setActive(false);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
            bool _isIntro;
            bool _introDone;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_sanctum_guard_xerestraszaAI>(creature);
        }
};

class npc_baltharus_the_warborn_clone : public CreatureScript
{
    public:
        npc_baltharus_the_warborn_clone() : CreatureScript("npc_baltharus_the_warborn_clone") { }

        struct npc_baltharus_the_warborn_cloneAI : public ScriptedAI
        {
            npc_baltharus_the_warborn_cloneAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = (InstanceScript*)creature->GetInstanceScript();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoZoneInCombat();
                _events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 10000));
                _events.ScheduleEvent(EVENT_BLADE_TEMPEST, urand(18000, 25000));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                // Setting DATA_BALTHARUS_SHARED_HEALTH to 0 when killed would bug the boss.
                if (_instance && me->GetHealth() - damage > 0)
                    _instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetHealth() - damage);
            }

            void JustDied(Unit* killer)
            {
                // This is here because DamageTaken wont trigger if the damage is deadly.
                if (_instance)
                    if (Creature* baltarhus = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_BALTHARUS_THE_WARBORN)))
                        killer->Kill(baltarhus);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_instance)
                    me->SetHealth(_instance->GetData(DATA_BALTHARUS_SHARED_HEALTH));

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            _events.ScheduleEvent(EVENT_CLEAVE, 24000);
                            break;
                        case EVENT_BLADE_TEMPEST:
                            DoCastVictim(SPELL_BLADE_TEMPEST);
                            _events.ScheduleEvent(EVENT_BLADE_TEMPEST, 24000);
                           break;
                        default:
                            break;
                    }
               }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_baltharus_the_warborn_cloneAI>(creature);
        }
};

class spell_baltharus_enervating_brand : public SpellScriptLoader
{
    public:
        spell_baltharus_enervating_brand() : SpellScriptLoader("spell_baltharus_enervating_brand") { }

        class spell_baltharus_enervating_brand_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_baltharus_enervating_brand_AuraScript);

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();
                uint32 triggerSpellId = GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                target->CastSpell(target, triggerSpellId, true);

				if (Unit* caster = GetCaster())
                    if (target->GetDistance(caster) <= 12.0f)
                        target->CastSpell(caster, SPELL_SIPHONED_MIGHT, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_baltharus_enervating_brand_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_baltharus_enervating_brand_AuraScript();
        }
};

class EnervatingBrandSelector
{
    public:
        explicit EnervatingBrandSelector(Unit* caster) : _caster(caster) {}

        bool operator()(Unit* unit)
        {
            if (_caster->GetDistance(unit) > 12.0f)
                return true;

            if (unit->GetTypeId() != TYPEID_PLAYER)
                return true;

            return false;
        }

    private:
        Unit* _caster;
};

class spell_baltharus_enervating_brand_trigger : public SpellScriptLoader
{
    public:
        spell_baltharus_enervating_brand_trigger() : SpellScriptLoader("spell_baltharus_enervating_brand_trigger") { }

        class spell_baltharus_enervating_brand_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_baltharus_enervating_brand_trigger_SpellScript);

            void FilterTargets(std::list<Unit*>& unitList)
            {
                unitList.remove_if(EnervatingBrandSelector(GetCaster()));
                unitList.push_back(GetCaster());
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_baltharus_enervating_brand_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_AREA_ALLY_SRC);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_baltharus_enervating_brand_trigger_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_AREA_ALLY_SRC);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_baltharus_enervating_brand_trigger_SpellScript();
        }
};

class at_baltharus_plateau : public AreaTriggerScript
{
    public:
        at_baltharus_plateau() : AreaTriggerScript("at_baltharus_plateau") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            // Only trigger once
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (Creature* xerestrasza = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_XERESTRASZA_EVENT)))
                    xerestrasza->AI()->DoAction(ACTION_XERESTRASZA_INTRO);

                if (Creature* baltharus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_BALTHARUS_THE_WARBORN)))
                    baltharus->AI()->DoAction(ACTION_BALTHARUS_INTRO);
            }

            return true;
        }
};

void AddSC_boss_baltharus_the_warborn()
{
    new boss_baltharus_the_warborn();
	new npc_sanctum_guard_xerestrasza();
	new npc_baltharus_the_warborn_clone();
    new spell_baltharus_enervating_brand();
	new spell_baltharus_enervating_brand_trigger();
	new at_baltharus_plateau();
}
