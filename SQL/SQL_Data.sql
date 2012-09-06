-- Baltharus the Warborn
DELETE FROM `areatrigger_scripts` WHERE `entry`=5867;
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES
(5867, 'at_baltharus_plateau');
UPDATE `creature` SET `spawntimesecs`=604800 WHERE `id`=39751;
UPDATE `creature_template` SET `npcflag`=2, `ScriptName`='npc_sanctum_guard_xerestrasza' WHERE `entry`=40429;
UPDATE `creature_template` SET `lootid`=39751, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35, `ScriptName`= 'boss_baltharus_the_warborn' WHERE `entry`=39751;
UPDATE `creature_template` SET `exp`=2, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=70 WHERE `entry`=39920;
UPDATE `creature_template` SET `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=70, `ScriptName`='npc_baltharus_the_warborn_clone' WHERE `entry`=39899;
DELETE FROM `creature_text` WHERE `entry` IN (39751,40429);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(39751, 0, 0, 'Your power wanes, ancient one.... Soon you will join your friends.', 14, 0, 100, 0, 0, 17525, 'Baltharus the Warborn - SAY_INTRO'),
(39751, 1, 0, 'Ah, the entertainment has arrived.', 14, 0, 100, 0, 0, 17520, 'Baltharus the Warborn - SAY_AGGRO'),
(39751, 2, 0, 'Baltharus leaves no survivors!', 14, 0, 100, 0, 0, 17521, 'Baltharus the Warborn - SAY_KILL'),
(39751, 2, 1, 'This world has enough heroes.', 14, 0, 100, 0, 0, 17522, 'Baltharus the Warborn - SAY_KILL'),
(39751, 3, 0, 'Twice the pain and half the fun.', 14, 0, 100, 0, 0, 17524, 'Baltharus the Warborn - SAY_SPLIT'),
(39751, 4, 1, 'I... didn''t see that coming....', 14, 0, 100, 0, 0, 17523, 'Baltharus the Warborn - SAY_DEATH'),
-- Sanctum Guard Xerestrasza
(40429, 0, 0, 'Thank you! I could not have held out for much longer.... A terrible thing has happened here.', 14, 0, 100, 5, 0, 17491, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT'),
(40429, 1, 0, 'We believed the Sanctum was well-fortified, but we were not prepared for the nature of this assault.', 12, 0, 100, 1, 0, 17492, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_1'),
(40429, 2, 0, 'The Black dragonkin materialized from thin air, and set upon us before we could react.', 12, 0, 100, 1, 0, 17493, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_2'),
(40429, 3, 0, 'We did not stand a chance. As my brethren perished around me, I managed to retreat here and bar the entrance.', 12, 0, 100, 1, 0, 17494, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_3'),
(40429, 4, 0, 'They slaughtered us with cold efficiency, but the true focus of their interest seemed to be the eggs kept here in the Sanctum.', 12, 0, 100, 1, 0, 17495, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_4'),
(40429, 5, 0, 'The commander of the forces on the ground here is a cruel brute named Zarithrian, but I fear there are greater powers at work.', 12, 0, 100, 1, 0, 17496, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_5'),
(40429, 6, 0, 'In their initial assault, I caught a glimpse of their true leader, a fearsome full-grown twilight dragon.', 12, 0, 100, 1, 0, 17497, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_6'),
(40429, 7, 0, 'I know not the extent of their plans, heroes, but I know this: They cannot be allowed to succeed!', 12, 0, 100, 5, 0, 17498, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_EVENT_7'),
(40429, 8, 0, 'Help! I am trapped within this tree! I require aid!', 14, 0, 100, 5, 0, 17490, 'Sanctum Guardian Xerestrasza  - SAY_XERESTRASZA_INTRO');
-- Delete Smart AI for Baltarhus the Warborn Clone
SET @ENTRY := 39899; -- NPC entry
UPDATE `creature_template` SET `AIName`= '' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (74502, 74505) AND `ScriptName` IN ('spell_baltharus_enervating_brand', 'spell_baltharus_enervating_brand_trigger');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(74502, 'spell_baltharus_enervating_brand'),
(74505, 'spell_baltharus_enervating_brand_trigger');

-- Saviana Ragefire
UPDATE `creature` SET `spawntimesecs`=604800 WHERE `id`=39747;
UPDATE `creature_template` SET `lootid`=39747, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35, `ScriptName`='boss_saviana_ragefire' WHERE `entry`=39747; -- Saviana Ragefire 10 mode
UPDATE `creature_template` SET `exp`=2, `lootid`=39747, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35 WHERE `entry`=39823; -- Saviana Ragefire 25 mode
DELETE FROM `creature_text` WHERE `entry`=39747;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(39747, 0, 0, 'You will sssuffer for this intrusion!', 1, 0, 100, 0, 0, 17528, 'Saviana Ragefire - SAY_AGGRO'),
(39747, 1, 0, 'As it should be...', 1, 0, 100, 0, 0, 17529, 'Saviana Ragefire - SAY_KILL'),
(39747, 1, 1, 'Halion will be pleased.', 1, 0, 100, 0, 0, 17530, 'Saviana Ragefire - SAY_KILL'),
(39747, 2, 0, 'Burn in the master''s flame!', 1, 0, 100, 0, 0, 17532, 'Saviana Ragefire - SAY_CONFLAGRATION'),
(39747, 3, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Saviana Ragefire - EMOTE_ENRAGE');
DELETE FROM `spell_script_names` WHERE `spell_id` =74453;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(74453, 'spell_saviana_flame_beacon');

-- General Zarithrian
UPDATE `creature_template` SET `unit_flags`=64, `faction_A`=103, `faction_H`=103, `lootid`=39746, `Health_mod`=297.5, `mindmg`=497,`maxdmg`=676,`attackpower`=795,`dmg_multiplier`=35, `ScriptName`='boss_general_zarithrian' WHERE `entry`=39746; -- General Zarithrian 10 mode
UPDATE `creature_template` SET `exp`=2, `unit_flags`=64, `faction_A`=103, `faction_H`=103, `lootid`=39746, `Health_mod`=1010.97, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35 WHERE `entry`=39805; -- General Zarithrian 25 mode
UPDATE `creature_template` SET `exp`=2, `minlevel`=82, `maxlevel`=82, `faction_A`=14, `faction_H`=14 WHERE `entry` IN (39814, 39815); -- Onyx Flamecaller 10 & 25 Modes don't know about melle dmg
UPDATE `creature_template` SET `ScriptName`='npc_onyx_flamecaller' WHERE `entry`=39814;
UPDATE `creature_template` SET `InhabitType`=3 WHERE `entry`=39794;
UPDATE `creature` SET `spawntimesecs`=604800 WHERE `id`=39746;
DELETE FROM `creature_text` WHERE `entry` = 39746;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(39746, 0, 0, 'Alexstrasza has chosen capable allies... A pity that I must END YOU!', 1, 0, 100, 0, 0, 17512, 'General Zarithrian - SAY_AGGRO'),
(39746, 1, 0, 'You thought you stood a chance?', 1, 0, 100, 0, 0, 17513, 'General Zarithrian - SAY_KILL'),
(39746, 1, 1, 'It''s for the best.', 1, 0, 100, 0, 0, 17514, 'General Zarithrian - SAY_KILL'),
(39746, 2, 0, 'Turn them to ash, minions!', 1, 0, 100, 0, 0, 17516, 'General Zarithrian - SAY_FLAMECALLER'),
(39746, 3, 0, 'HALION! I...', 1, 0, 100, 0, 0, 17515, 'General Zarithrian - SAY_DEATH');
DELETE FROM `spell_script_names` WHERE `spell_id`=74398 AND `ScriptName`='spell_zarithrian_summon_flamecaller';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(74398, 'spell_zarithrian_summon_flamecaller');

-- Halion
UPDATE `creature` SET `spawntimesecs`=604800 WHERE `id`=39863;
-- Copyright (C) kaelima <https://github.com/kaelima/>  and more updates from me(MindBreaker)
UPDATE `creature_template` SET `exp`=2, `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35, `ScriptName`='boss_halion' WHERE `entry`=39863;
UPDATE `creature_template` SET `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35, `RegenHealth`=0, `ScriptName`='boss_halion_twilight' WHERE `entry`=40142;
UPDATE `creature_template` SET `ScriptName`='npc_halion_controller' WHERE `entry`=40146;
UPDATE `creature_template` SET `ScriptName`='npc_meteor', `flags_extra`=130 WHERE `entry`=40029;
UPDATE `creature_template` SET `ScriptName`='npc_meteor_flame', `flags_extra`=130 WHERE `entry` IN (40041, 40042, 40043, 40044);
UPDATE `creature_template` SET `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35, `ScriptName`='npc_living_inferno' WHERE `entry`=40681;
UPDATE `creature_template` SET `mindmg`=497, `maxdmg`=676, `attackpower`=795, `dmg_multiplier`=35 WHERE `entry`=40682;
UPDATE `creature_template` SET `flags_extra`=130 WHERE `entry`=40055;
UPDATE `creature_template` SET `exp`=2 WHERE `entry` IN (39864,39944,39945);
UPDATE `creature_template` SET `scale`=1,`exp`=2,`baseattacktime`=2000,`unit_flags`=33554432, `flags_extra`=130 WHERE `entry`=40135; -- Consumption
UPDATE `creature_template` SET `scale`=1,`flags_extra`=130 WHERE `entry`=40001;
UPDATE `creature_model_info` SET `bounding_radius`=3.8,`combat_reach`=7.6,`gender`=2 WHERE `modelid`=16946;
UPDATE `creature_template_addon` SET `auras`=74629 WHERE `entry`=40001; -- Combustion
DELETE FROM `creature_template_addon` WHERE `entry`=40135;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(40135, 0, 0, 0x1, 0, 0, 74803); -- Consumption
DELETE FROM `creature_text` WHERE `entry` = 39863;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(39863, 0, 0, 'Meddlesome insects! You''re too late: The Ruby Sanctum''s lost.', 1, 0, 100, 0, 0, 17499, 'Halion - SAY_ARRIVAL'),
(39863, 1, 0, 'Your world teeters on the brink of annihilation. You will ALL bear witness to the coming of a new age of DESTRUCTION!', 1, 0, 100, 0, 0, 17500, 'Halion - SAY_AGGRO'),
(39863, 2, 0, 'The heavens burn!', 1, 0, 100, 0, 0, 17505, 'Halion - SAY_METEOR_STRIKE'),
(39863, 3, 0, 'You will find only suffering within the realm of twilight! Enter if you dare!', 1, 0, 100, 0, 0, 17507, 'Halion - SAY_PHASE_2'),
(39863, 4, 0, 'Beware the shadow!', 1, 0, 100, 0, 0, 17506, 'Halion - SAY_SPHERE_PULSE'),
(39863, 5, 0, 'The orbiting spheres pulse with dark energy!', 16, 0, 100, 0, 0, 0, 'Halion Twilight - EMOTE_SPHERE_PULSE'),
(39863, 6, 0, 'I am the light and the darkness! Cower, mortals, before the herald of Deathwing!', 1, 0, 100, 0, 0, 17508, 'Halion - SAY_PHASE_3'),
(39863, 7, 0, 'Not good enough.', 1, 0, 100, 0, 0, 17504, 'Halion - SAY_BERSERK'),
(39863, 8, 0, 'Another "hero" falls.', 1, 0, 100, 0, 0, 17501, 'Halion - SAY_KILL'),
(39863, 9, 0, 'Relish this victory, mortals, for it will be your last! This world will burn with the master''s return!', 1, 0, 100, 0, 0, 17503, 'Halion - SAY_DEATH');
SET @GUID := 1000000; -- Not like Trinity Core (maybe trinitycore will don't add this and if a use the next id free in DB they will overwrite it with other fix from them)
DELETE FROM `gameobject` WHERE `guid`=@GUID;
INSERT INTO `gameobject` VALUES (@GUID, 203624, 724, 15, 32, '3154.99', '535.637', '72.8887', '3.14159', 0, 0, 0, 0, 120, 0, 1);
UPDATE `gameobject_template` SET `data10`=75074, `ScriptName`='' WHERE `entry`=202794;
UPDATE `gameobject_template` SET `faction`=35, `data10`=0, `ScriptName`='go_physical_portal' WHERE `entry`=202795;
DELETE FROM `spelldifficulty_dbc` WHERE `id` BETWEEN 5000 AND 5006;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (74802, 74630, 75882, 75883, 75884, 75874, 75875, 75876);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(74630, 74607, 1, 'Halion Combustion Damage Aura trigger knock back'),
(75882, 74607, 1, 'Halion Combustion Damage Aura trigger knock back'),
(75883, 74607, 1, 'Halion Combustion Damage Aura trigger knock back'),
(75884, 74607, 1, 'Halion Combustion Damage Aura trigger knock back'),
(74802, 74799, 1, 'Halion Consumption Damage Aura trigger knock back'),
(75874, 74799, 1, 'Halion Consumption Damage Aura trigger knock back'),
(75875, 74799, 1, 'Halion Consumption Damage Aura trigger knock back'),
(75876, 74799, 1, 'Halion Consumption Damage Aura trigger knock back');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (76006, 74641, 74562, 74567, 74792, 74795);
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_cosmetic_fire_pillar';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_meteor_strike_marker';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_fiery_combustion';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_mark_of_combustion';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_soul_consumption';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_halion_mark_of_consumption';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(76006, 'spell_halion_cosmetic_fire_pillar'),
(74641, 'spell_halion_meteor_strike_marker'),
(74562, 'spell_halion_fiery_combustion'),
(74567, 'spell_halion_mark_of_combustion'),
(74792, 'spell_halion_soul_consumption'),
(74795, 'spell_halion_mark_of_consumption');

-- Instance data
SET @ENTRY := 40400;
UPDATE `creature` SET `spawntimesecs`=86400 WHERE `id` IN (@ENTRY+17, @ENTRY+19, @ENTRY+21, @ENTRY+23);
DELETE FROM `creature_loot_template` WHERE `entry` IN (39746, 39747, 39751);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39751, 49426, 100, 1, 0, 1, 1),
(39747, 49426, 100, 1, 0, 1, 1),
(39746, 49426, 100, 1, 0, 1, 1);
SET @ENTRY := 40400;
/*Leave as comment until I will get more dataz
UPDATE `creature_template` SET `mindmg`=, `maxdmg`=, `attackpower`=, `dmg_multiplier`=, `ScriptName`='npc_charscale_assaulter' WHERE `entry`=@ENTRY+19;
UPDATE `creature_template` SET `mindmg`=, `maxdmg`=, `attackpower`=, `dmg_multiplier`=, `ScriptName`='npc_charscale_elite' WHERE `entry`=@ENTRY+21
UPDATE `creature_template` SET `mindmg`=, `maxdmg`=, `attackpower`=, `dmg_multiplier`=, `ScriptName`='npc_charscale_commander' WHERE `entry`=@ENTRY+23;
UPDATE `creature_template` SET `mindmg`=, `maxdmg`=, `attackpower`=, `dmg_multiplier`=, `ScriptName`='npc_charscale_invoker' WHERE `entry`=@ENTRY+17;
*/
UPDATE `instance_template` SET `script`='instance_ruby_sanctum' WHERE `map`=724;
