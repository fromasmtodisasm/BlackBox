-- : ./AI/Packs/Sound/mutantvoice.lua
-- TEMPLATE
-- DO NOT MODIFY THIS TEMPLATE
--
-- This template has all the readability signals currently in use.  
-- Change the <Template> string to the pack name
-- 
-- To switch between alternate sound responses add a new set of 
-- bracketed parameters, making sure there is a comma after the bracket eg,
--				{
--				PROBABILITY = 500,
--				soundFile = "SOUNDS/<full path to wav file>",
--				Volume = 255,
--				min = 12,
--				max = 300,
--				sound_unscalable = 1,
--				},
-- Probability determines how often a wave is played in response to the signal
-- To allow for possibility of no sound in response to a signal, sounds should sum
-- to less than 1000. eg. if you dont want sound to be played every single time 
-- player receives an order might have total PROBABILITY for all sounds = 500.
--------------------------------------------------
--    Created By: <your_name>
--   Description: <short_description>
--------------------------	
SOUNDPACK.mutantvoice = {
				
	CALL_ALARM = {
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/alarm_alone_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/alarm_alone_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/alarm_alone_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/alarm_alone_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	CALL_ALARM_GROUP = {
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/alarm_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/alarm_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/alarm_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	FIRST_HOSTILE_CONTACT = {
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	FIRST_HOSTILE_CONTACT_GROUP = {
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/alert_to_combat_group_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
--	INTERESTED_TO_IDLE = {
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_4.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_5.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_6.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_7.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_8.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_9.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_10.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_11.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_12.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_13.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_14.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_15.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 62,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_alone_16.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--			},
--			
--	INTERESTED_TO_IDLE_GROUP = {
--				{
--				PROBABILITY = 200,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_group_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 200,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_group_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 200,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_group_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 200,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_group_4.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 200,
--				soundFile = "languages/voicepacks/mutantvoice/alert_to_idle_group_5.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--			},
			
	THREATEN = {
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_7.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_8.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/combat_alone_9.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	THREATEN_GROUP = {
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_group_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	ENEMY_TARGET_LOST = {
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_7.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 125,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_alone_8.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	ENEMY_TARGET_LOST_GROUP = {
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/combat_to_alert_group_6.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
						
	RETREATING_NOW = {
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_retreat_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_retreat_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_retreat_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
					
	PROVIDING_COVER = {
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_suppress_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_suppress_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 333,
				soundFile = "languages/voicepacks/mutantvoice/comm_suppress_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
												
	--receiving and throwing grenade?
	FIRE_IN_THE_HOLE = {
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/grenade_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/grenade_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/grenade_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/grenade_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
						
--	IDLE_TO_THREATENED = {
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_4.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_4.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 111,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_5.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--			},
--			
--	IDLE_TO_THREATENED_GROUP = {
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_1.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_2.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--				{
--				PROBABILITY = 166,
--				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_3.wav",
--				Volume = 255,
--				min = 12,
--				max = 200,
--				sound_unscalable = 0,
--				},
--			},
			
	IDLE_TO_INTERESTED = {
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_alone_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 111,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_alone_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	IDLE_TO_INTERESTED_GROUP = {
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_heard_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 166,
				soundFile = "languages/voicepacks/mutantvoice/idle_to_alert_seen_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	
	
	FRIEND_DEATH = {
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	FRIEND_DEATH_GROUP = {
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 250,
				soundFile = "languages/voicepacks/mutantvoice/mandown_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	GETTING_SHOT_AT = {
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	GETTING_SHOT_AT_GROUP = {
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/receive_fire_group_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	GET_REINFORCEMENTS = {
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_tell_get_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_tell_get_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_tell_get_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_tell_get_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_tell_get_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
			
	--filippo: this readability signal is not yet used by the AI
	CALL_REINFORCEMENTS = {
				--??
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_talk_to_1.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_talk_to_2.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_talk_to_3.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_talk_to_4.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
				{
				PROBABILITY = 200,
				soundFile = "languages/voicepacks/mutantvoice/reinforce_talk_to_5.wav",
				Volume = 255,
				min = 12,
				max = 200,
				sound_unscalable = 0,
				},
			},
}