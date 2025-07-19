/*
 * Ten Kingdoms: Ancient Adversaries
 *
 * Copyright 2024 Jesse Allen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//Filename    : OGF_REC.H
//Description : File records for game objects

#ifndef __OGF_REC_H
#define __OGF_REC_H

#include <stdint.h>

#include <OBULLET.h>
#include <OB_HOMIN.h>
#include <OB_PROJ.h>
#include <OF_BASE.h>
#include <OF_CAMP.h>
#include <OF_FACT.h>
#include <OF_HARB.h>
#include <OF_INN.h>
#include <OF_MARK.h>
#include <OF_MINE.h>
#include <OF_MONS.h>
#include <OF_RESE.h>
#include <OF_WAR.h>
#include <OGAME.h>
#include <OGF_V1.h>
#include <OGFILE.h>
#include <OGODRES.h>
#include <ONATIONB.h>
#include <ONEWS.h>
#include <OPOWER.h>
#include <OREBEL.h>
#include <OREGION.h>
#include <OREGIONS.h>
#include <OSITE.h>
#include <OSNOWG.h>
#include <OTALKRES.h>
#include <OTECHRES.h>
#include <OTORNADO.h>
#include <OTOWN.h>
#include <OU_CARA.h>
#include <OU_CART.h>
#include <OU_GOD.h>
#include <OU_MARI.h>
#include <OU_MONS.h>
#include <OU_VEHI.h>
#include <OWEATHER.h>

#pragma pack(1)
typedef GameFileDate GameFileDateGF;

struct GameFileHeader
{
	uint32_t             class_size;
	int8_t               file_name[FilePath::MAX_FILE_PATH+1];
	int8_t               player_name[HUMAN_NAME_LEN+1];
	int8_t               race_id;
	int8_t               nation_color;
	int32_t              game_date;
	GameFileDateGF       file_date;
	int16_t              terrain_set;
	char                 note[128]; // User note for savegame
};

struct FirmInfoGF
{
	int8_t               firm_id;
	int8_t               name[FirmInfo::NAME_LEN+1]; //zero
	int8_t               short_name[FirmInfo::SHORT_NAME_LEN+1]; //zero
	int8_t               overseer_title[FirmInfo::TITLE_LEN+1]; //zero
	int8_t               worker_title[FirmInfo::TITLE_LEN+1]; //zero
	int8_t               tera_type;
	int8_t               buildable;
	int8_t               live_in_town;
	int16_t              max_hit_points;
	int8_t               need_overseer;
	int8_t               need_worker;
	int16_t              setup_cost;
	int16_t              year_cost;
	int16_t              first_build_id; //zero
	int16_t              build_count; //zero
	int16_t              loc_width;
	int16_t              loc_height;
	int8_t               firm_skill_id;
	int8_t               firm_race_id;
	int8_t               is_linkable_to_town;
	int16_t              total_firm_count;
	int16_t              nation_firm_count_array[MAX_NATION];
	int8_t               nation_tech_level_array[MAX_NATION];
};

struct TechClassGF
{
	int16_t              class_id;
	int16_t              first_tech_id;
	int16_t              tech_count;
	int16_t              icon_index;
	int16_t              nation_research_firm_recno_array[MAX_NATION];
};

struct TechInfoGF
{
	int16_t              tech_id;
	int16_t              class_id;
	int16_t              max_tech_level;
	int16_t              complex_level;
	int16_t              unit_id;
	int16_t              firm_id;
	int16_t              parent_unit_id;
	int16_t              parent_firm_id;
	int8_t               parent_level;
	int16_t              icon_index;
	int8_t               nation_tech_level_array[MAX_NATION];
	int8_t               nation_is_researching_array[MAX_NATION];
	float                nation_research_progress_array[MAX_NATION];
};

struct DynArrayGF
{
	int32_t              ele_num;
	int32_t              block_num;
	int32_t              cur_pos;
	int32_t              last_ele;
	int32_t              ele_size;
	int32_t              sort_offset;
	int8_t               sort_type;
	uint32_t             body_buf; //zero
};

struct DynArrayB_Unused
{
	int8_t               bytes[10]; //zero
};

struct TalkMsgGF
{
	int16_t              talk_id;
	int16_t              talk_para1;
	int16_t              talk_para2;
	int32_t              date;
	int8_t               from_nation_recno;
	int8_t               to_nation_recno;
	int8_t               reply_type;
	int32_t              reply_date;
	int8_t               relation_status;
};

struct TalkChoiceGF
{
	uint32_t             str; //zero
	int16_t              para;
};

struct TalkResGF
{
	int8_t               init_flag;
	int16_t              reply_talk_msg_recno;
	TalkMsgGF            cur_talk_msg;
	uint32_t             choice_question; //zero
	uint32_t             choice_question_second_line; //zero
	int16_t              talk_choice_count; //zero
	TalkChoiceGF         talk_choice_array[MAX_TALK_CHOICE];
	int8_t               available_talk_id_array[MAX_TALK_TYPE];
	int16_t              cur_choice_id;
	int8_t               save_view_mode;
	int8_t               msg_add_nation_color;
	DynArrayGF           talk_msg_array; //zero
	DynArrayB_Unused     empty_room_bytes; //zero
};

struct GodInfoGF
{
	int8_t               god_id;
	int8_t               race_id;
	int8_t               unit_id;
	int16_t              exist_pray_points;
	int16_t              power_pray_points;
	int8_t               can_cast_power;
	int8_t               cast_power_range;
	int8_t               nation_know_array[MAX_NATION];
};

struct ColorRemapGF
{
	int8_t               main_color;
	int8_t               color_table[256];
};

struct GameGF
{
	int8_t               init_flag;
	int8_t               started_flag;
	int8_t               game_mode;
	int8_t               game_has_ended;
	ColorRemapGF         color_remap_array[MAX_COLOR_SCHEME+1];
};

struct ConfigGF
{
	int16_t              difficulty_rating;
	int8_t               ai_nation_count;
	int16_t              start_up_cash;
	int16_t              ai_start_up_cash;
	int8_t               ai_aggressiveness;
	int16_t              start_up_independent_town;
	int16_t              start_up_raw_site;
	int8_t               difficulty_level;
	int8_t               explore_whole_map;
	int8_t               fog_of_war;
	int16_t              terrain_set;
	int16_t              latitude;
	int8_t               weather_effect;
	int8_t               land_mass;
	int8_t               new_independent_town_emerge;
	int8_t               independent_town_resistance;
	int8_t               random_event_frequency;
	int8_t               new_nation_emerge;
	int8_t               monster_type;
	int8_t               start_up_has_mine_nearby;
	int8_t               random_start_up;
	int8_t               goal_destroy_monster;
	int8_t               goal_population_flag;
	int8_t               goal_economic_score_flag;
	int8_t               goal_total_score_flag;
	int8_t               goal_year_limit_flag;
	int32_t              goal_population;
	int32_t              goal_economic_score;
	int32_t              goal_total_score;
	int32_t              goal_year_limit;
	int8_t               fire_spread_rate;
	int8_t               wind_spread_fire_rate;
	int8_t               fire_fade_rate;
	int8_t               fire_restore_prob;
	int8_t               rain_reduce_fire_rate;
	int8_t               fire_damage;
	int8_t               show_ai_info;
	int8_t               fast_build;
	int8_t               disable_ai_flag;
	int8_t               king_undie_flag;
	int8_t               race_id;
	int8_t               player_name[HUMAN_NAME_LEN+1];
	int8_t               player_nation_color;
	int8_t               expired_flag;
	int8_t               opaque_report;
	int8_t               disp_news_flag;
	int16_t              scroll_speed;
	int16_t              frame_speed;
	int8_t               help_mode;
	int8_t               disp_town_name;
	int8_t               disp_spy_sign;
	int8_t               show_all_unit_icon;
	int8_t               show_unit_path;
	int8_t               music_flag;
	int16_t              cd_music_volume;
	int16_t              wav_music_volume;
	int8_t               sound_effect_flag;
	int16_t              sound_effect_volume;
	int8_t               pan_control;
	int8_t               lightning_visual;
	int8_t               earthquake_visual;
	int8_t               rain_visual;
	int8_t               snow_visual;
	int8_t               snow_ground;
	int8_t               lightning_audio;
	int8_t               earthquake_audio;
	int8_t               rain_audio;
	int8_t               snow_audio;
	int8_t               wind_audio;
	int32_t              lightning_brightness;
	int32_t              cloud_darkness;
	int32_t              lightning_volume;
	int32_t              earthquake_volume;
	int32_t              rain_volume;
	int32_t              snow_volume;
	int32_t              wind_volume;
	int8_t               blacken_map;
	int8_t               explore_mask_method;
	int8_t               fog_mask_method;
};

struct ChatInfoGF
{
	int32_t              received_date;
	int8_t               from_nation_recno;
	int8_t               chat_str[CHAT_STR_LEN+1];
};

struct InfoGF
{
	int32_t              game_start_date;
	int32_t              game_date;
	int32_t              game_day;
	int32_t              game_month;
	int32_t              game_year;
	int32_t              goal_deadline;
	int16_t              goal_difficulty;
	int16_t              goal_score_bonus;
	int32_t              week_day;
	int32_t              year_day;
	int32_t              year_passed;
	int32_t              random_seed;
	uint32_t             start_play_time;
	uint32_t             total_play_time;
	int16_t              viewing_nation_recno;
	int16_t              viewing_spy_recno;
	int16_t              default_viewing_nation_recno;
	int16_t              browse_nation_recno;
	int16_t              browse_race_recno;
	int16_t              browse_firm_recno;
	int16_t              browse_income_recno;
	int16_t              browse_expense_recno;
	int16_t              browse_troop_recno;
	int16_t              browse_unit_recno;
	int16_t              browse_tech_recno;
	int16_t              browse_god_recno;
	int16_t              browse_town_recno;
	int16_t              browse_spy_recno;
	int16_t              browse_caravan_recno;
	int16_t              browse_ship_recno;
	int16_t              browse_talk_msg_recno;
	int16_t              browse_news_recno;
	int16_t              browse_ai_action_recno;
	int16_t              browse_ai_attack_recno;
	int8_t               nation_report_mode;
	int16_t              last_talk_nation_recno;
	int8_t               player_reply_mode;
	int8_t               chat_receiver_type;
	int8_t               player_chat_str[CHAT_STR_LEN+1];
	ChatInfoGF           remote_chat_array[MAX_REMOTE_CHAT_STR];
};

struct PowerGF
{
	int32_t              command_id;
	int32_t              command_unit_recno;
	int32_t              command_para;
	int8_t               win_opened;
	int8_t               enable_flag;
	int32_t              key_str_pos[MAX_KEY_STR];
};

struct WeatherGF
{
	uint32_t             seed;
	int16_t              season_phase;
	int16_t              day_to_quake;
	int16_t              avg_temp;
	int16_t              temp_amp;
	int16_t              wind_spd;
	int32_t              high_wind_day;
	int16_t              wind_dir;
	int16_t              windy_speed;
	int16_t              tornado_count;
	int8_t               cur_cloud_str;
	int8_t               cur_cloud_len;
	int8_t               cur_cloud_type;
	int32_t              quake_frequency;
	int16_t              quake_x;
	int16_t              quake_y;
};

struct MagicWeatherGF
{
	int8_t               rain_str;
	int16_t              wind_spd;
	int16_t              wind_dir;
	int16_t              rain_day;
	int16_t              wind_day;
	int16_t              lightning_day;
};

struct LocationGF
{
	uint16_t             loc_flag;
	int16_t              terrain_id;
	int16_t              cargo_recno;
	int16_t              air_cargo_recno;
	uint8_t              extra_para;
	int8_t               fire_level;
	int8_t               flammability;
	int8_t               power_nation_recno;
	uint8_t              region_id;
	uint8_t              visit_level;
};

struct SpriteGF
{
	uint32_t             vtp; //zero

	int16_t              sprite_id;
	int16_t              sprite_recno;
	int8_t               mobile_type;
	uint8_t              cur_action;
	uint8_t              cur_dir;
	uint8_t              cur_frame;
	uint8_t              cur_attack;
	uint8_t              final_dir;
	int8_t               turn_delay;
	int8_t               guard_count;
	uint8_t              remain_attack_delay;
	uint8_t              remain_frames_per_step;
	int16_t              cur_x;
	int16_t              cur_y;
	int16_t              go_x;
	int16_t              go_y;
	int16_t              next_x;
	int16_t              next_y;
	uint32_t             sprite_info; //zero
};

struct AttackInfoGF
{
	uint8_t              combat_level;
	uint8_t              attack_delay;
	uint8_t              attack_range;
	uint8_t              attack_damage;
	uint8_t              pierce_damage;
	int16_t              bullet_out_frame;
	int8_t               bullet_speed;
	int8_t               bullet_radius;
	int8_t               bullet_sprite_id;
	int8_t               dll_bullet_sprite_id;
	int8_t               eqv_attack_next;
	int16_t              min_power;
	int16_t              consume_power;
	int8_t               fire_radius;
	int16_t              effect_id;
};

struct ResultNodeGF
{
	int16_t              node_x;
	int16_t              node_y;
};

struct SkillGF
{
	int8_t               combat_level;
	int8_t               skill_id;
	int8_t               skill_level;
	uint8_t              combat_level_minor;
	uint8_t              skill_level_minor;
	uint8_t              skill_potential;
};

struct TeamInfoGF
{
	int8_t               member_count;
	int16_t              member_unit_array[MAX_TEAM_MEMBER];
	int32_t              ai_last_request_defense_date;
};

struct UnitGF
{
	uint32_t             vtp; //zero

	// Sprite
	int16_t              sprite_id;
	int16_t              sprite_recno;
	int8_t               mobile_type;
	uint8_t              cur_action;
	uint8_t              cur_dir;
	uint8_t              cur_frame;
	uint8_t              cur_attack;
	uint8_t              final_dir;
	int8_t               turn_delay;
	int8_t               guard_count;
	uint8_t              remain_attack_delay;
	uint8_t              remain_frames_per_step;
	int16_t              cur_x;
	int16_t              cur_y;
	int16_t              go_x;
	int16_t              go_y;
	int16_t              next_x;
	int16_t              next_y;
	uint32_t             sprite_info; //zero

	// Unit
	int8_t               unit_id;
	int8_t               rank_id;
	int8_t               race_id;
	int8_t               nation_recno;
	int8_t               ai_unit;
	uint16_t             name_id;
	uint32_t             unit_group_id;
	uint32_t             team_id;
	int8_t               selected_flag;
	int8_t               group_select_id;
	int8_t               waiting_term;
	int8_t               blocked_by_member;
	int8_t               swapping;
	int16_t              leader_unit_recno;
	int8_t               action_misc;
	int16_t              action_misc_para;
	int8_t               action_mode;
	int16_t              action_para;
	int16_t              action_x_loc;
	int16_t              action_y_loc;
	int8_t               action_mode2;
	int16_t              action_para2;
	int16_t              action_x_loc2;
	int16_t              action_y_loc2;
	int8_t               blocked_edge[4];
	uint8_t              attack_dir;
	int16_t              range_attack_x_loc;
	int16_t              range_attack_y_loc;
	int16_t              move_to_x_loc;
	int16_t              move_to_y_loc;
	int8_t               loyalty;
	int8_t               target_loyalty;
	float                hit_points;
	int16_t              max_hit_points;
	SkillGF              skill;
	int8_t               unit_mode;
	int16_t              unit_mode_para;
	int16_t              spy_recno;
	int16_t              nation_contribution;
	int16_t              total_reward;
	uint32_t             attack_info_array; //zero
	int8_t               attack_count;
	int8_t               attack_range;
	int16_t              cur_power;
	int16_t              max_power;
	uint32_t             has_result_node_array;
	int32_t              result_node_count;
	int16_t              result_node_recno;
	int16_t              result_path_dist;
	uint32_t             has_way_point_array;
	int16_t              way_point_array_size;
	int16_t              way_point_count;
	uint16_t             ai_action_id;
	int8_t               original_action_mode;
	int16_t              original_action_para;
	int16_t              original_action_x_loc;
	int16_t              original_action_y_loc;
	int16_t              original_target_x_loc;
	int16_t              original_target_y_loc;
	int16_t              ai_original_target_x_loc;
	int16_t              ai_original_target_y_loc;
	int8_t               ai_no_suitable_action;
	int8_t               can_guard_flag;
	int8_t               can_attack_flag;
	int8_t               force_move_flag;
	int16_t              home_camp_firm_recno;
	int8_t               aggressive_mode;
	int8_t               seek_path_fail_count;
	int8_t               ignore_power_nation;
	uint32_t             has_team_info;
};

struct CaravanStopGF
{
	// TradeStop
	int16_t              firm_recno;
	int16_t              firm_loc_x1;
	int16_t              firm_loc_y1;
	int8_t               pick_up_type;
	int8_t               pick_up_array[MAX_PICK_UP_GOODS];

	// CaravanStop
	int8_t               firm_id;
};

struct UnitCaravanGF
{
	int16_t              caravan_id;
	int8_t               journey_status;
	int8_t               dest_stop_id;
	int8_t               stop_defined_num;
	int8_t               wait_count;
	int16_t              stop_x_loc;
	int16_t              stop_y_loc;
	CaravanStopGF        stop_array[MAX_STOP_FOR_CARAVAN];
	int32_t              last_set_stop_date;
	int32_t              last_load_goods_date;
	int16_t              raw_qty_array[MAX_RAW];
	int16_t              product_raw_qty_array[MAX_PRODUCT];
};

struct UnitExpCartGF
{
	int8_t               triggered;
};

struct UnitGodGF
{
	int16_t              god_id;
	int16_t              base_firm_recno;
	int8_t               cast_power_type;
	int16_t              cast_origin_x;
	int16_t              cast_origin_y;
	int16_t              cast_target_x;
	int16_t              cast_target_y;
};

struct ShipStopGF
{
	// TradeStop
	int16_t              firm_recno;
	int16_t              firm_loc_x1;
	int16_t              firm_loc_y1;
	int8_t               pick_up_type;
	int8_t               pick_up_array[MAX_PICK_UP_GOODS];
};

struct UnitMarineGF
{
	SpriteGF             splash;
	int8_t               menu_mode;
	int8_t               extra_move_in_beach;
	int8_t               in_beach;
	int8_t               selected_unit_id;
	int16_t              unit_recno_array[MAX_UNIT_IN_SHIP];
	int8_t               unit_count;
	int8_t               journey_status;
	int8_t               dest_stop_id;
	int8_t               stop_defined_num;
	int8_t               wait_count;
	int16_t              stop_x_loc;
	int16_t              stop_y_loc;
	int8_t               auto_mode;
	int16_t              cur_firm_recno;
	int16_t              carry_goods_capacity;
	ShipStopGF           stop_array[MAX_STOP_FOR_SHIP];
	int16_t              raw_qty_array[MAX_RAW];
	int16_t              product_raw_qty_array[MAX_PRODUCT];
	AttackInfoGF         ship_attack_info;
	uint8_t              attack_mode_selected;
	int32_t              last_load_goods_date;
};

struct UnitMonsterGF
{
	int8_t               monster_action_mode;
};

struct UnitVehicleGF
{
	int16_t              solider_hit_points;
	int16_t              vehicle_hit_points;
};

struct AIRegionGF
{
	uint8_t              region_id;
	int8_t               town_count;
	int8_t               base_town_count;
};

struct ActionNodeGF
{
	int8_t               action_mode;
	int8_t               action_type;
	int16_t              action_para;
	int16_t              action_para2;
	uint16_t             action_id;
	int32_t              add_date;
	int16_t              unit_recno;
	int16_t              action_x_loc;
	int16_t              action_y_loc;
	int16_t              ref_x_loc;
	int16_t              ref_y_loc;
	int8_t               retry_count;
	int8_t               instance_count;
	int16_t              group_unit_array[ActionNode::MAX_ACTION_GROUP_UNIT];
	int8_t               processing_instance_count;
	int8_t               processed_instance_count;
	int32_t              next_retry_date;
};

struct AttackCampGF
{
	int16_t              firm_recno;
	int16_t              combat_level;
	int16_t              distance;
	int32_t              patrol_date;
};

struct NationRelationGF
{
	int8_t               has_contact;
	int8_t               should_attack;
	int8_t               trade_treaty;
	int8_t               status;
	int32_t              last_change_status_date;
	int8_t               ai_relation_level;
	int8_t               ai_secret_attack;
	int8_t               ai_demand_trade_treaty;
	float                good_relation_duration_rating;
	int16_t              started_war_on_us_count;
	float                cur_year_import[IMPORT_TYPE_COUNT];
	float                last_year_import[IMPORT_TYPE_COUNT];
	float                lifetime_import[IMPORT_TYPE_COUNT];
	int32_t              last_talk_reject_date_array[MAX_TALK_TYPE];
	int32_t              last_military_aid_date;
	int32_t              last_give_gift_date;
	int16_t              total_given_gift_amount;
	int8_t               contact_msg_flag;
};

struct NationGF
{
	uint32_t             vtp;

	// NationBase
	int16_t              nation_recno;
	int8_t               nation_type;
	int8_t               race_id;
	int8_t               color_scheme_id;
	int8_t               nation_color;
	int16_t              king_unit_recno;
	int8_t               king_leadership;
	int32_t              nation_name_id;
	int8_t               nation_name_str[NationBase::NATION_NAME_LEN+1];
	uint32_t             player_id;
	int8_t               next_frame_ready;
	int16_t              last_caravan_id;
	int16_t              nation_firm_count;
	int32_t              last_build_firm_date;
	int8_t               know_base_array[MAX_RACE];
	int8_t               base_count_array[MAX_RACE];
	int8_t               is_at_war_today;
	int8_t               is_at_war_yesterday;
	int32_t              last_war_date;
	int16_t              last_attacker_unit_recno;
	int32_t              last_independent_unit_join_date;
	int8_t               cheat_enabled_flag;
	float                cash;
	float                food;
	float                reputation;
	float                kill_monster_score;
	int16_t              auto_collect_tax_loyalty;
	int16_t              auto_grant_loyalty;
	float                cur_year_profit;
	float                last_year_profit;
	float                cur_year_fixed_income;
	float                last_year_fixed_income;
	float                cur_year_fixed_expense;
	float                last_year_fixed_expense;
	float                cur_year_income_array[INCOME_TYPE_COUNT];
	float                last_year_income_array[INCOME_TYPE_COUNT];
	float                cur_year_income;
	float                last_year_income;
	float                cur_year_expense_array[EXPENSE_TYPE_COUNT];
	float                last_year_expense_array[EXPENSE_TYPE_COUNT];
	float                cur_year_expense;
	float                last_year_expense;
	float                cur_year_cheat;
	float                last_year_cheat;
	float                cur_year_food_in;
	float                last_year_food_in;
	float                cur_year_food_out;
	float                last_year_food_out;
	float                cur_year_food_change;
	float                last_year_food_change;
	float                cur_year_reputation_change;
	float                last_year_reputation_change;
	NationRelationGF     relation_array[MAX_NATION];
	int8_t               relation_status_array[MAX_NATION];
	int8_t               relation_passable_array[MAX_NATION];
	int8_t               relation_should_attack_array[MAX_NATION];
	int8_t               is_allied_with_player;
	int32_t              total_population;
	int32_t              total_jobless_population;
	int32_t              total_unit_count;
	int32_t              total_human_count;
	int32_t              total_general_count;
	int32_t              total_weapon_count;
	int32_t              total_ship_count;
	int32_t              total_firm_count;
	int32_t              total_spy_count;
	int32_t              total_ship_combat_level;
	int16_t              largest_town_recno;
	int16_t              largest_town_pop;
	int16_t              raw_count_array[MAX_RAW];
	uint16_t             last_unit_name_id_array[MAX_UNIT_TYPE];
	int32_t              population_rating;
	int32_t              military_rating;
	int32_t              economic_rating;
	int32_t              overall_rating;
	int32_t              enemy_soldier_killed;
	int32_t              own_soldier_killed;
	int32_t              enemy_civilian_killed;
	int32_t              own_civilian_killed;
	int32_t              enemy_weapon_destroyed;
	int32_t              own_weapon_destroyed;
	int32_t              enemy_ship_destroyed;
	int32_t              own_ship_destroyed;
	int32_t              enemy_firm_destroyed;
	int32_t              own_firm_destroyed;

	// Nation
	DynArrayGF           action_array;
	uint16_t             last_action_id;
	uint32_t             ai_town_array; //zero
	uint32_t             ai_base_array; //zero
	uint32_t             ai_mine_array; //zero
	uint32_t             ai_factory_array; //zero
	uint32_t             ai_camp_array; //zero
	uint32_t             ai_research_array; //zero
	uint32_t             ai_war_array; //zero
	uint32_t             ai_harbor_array; //zero
	uint32_t             ai_market_array; //zero
	uint32_t             ai_inn_array; //zero
	uint32_t             ai_general_array; //zero
	uint32_t             ai_caravan_array; //zero
	uint32_t             ai_ship_array; //zero
	int16_t              ai_town_size;
	int16_t              ai_base_size;
	int16_t              ai_mine_size;
	int16_t              ai_factory_size;
	int16_t              ai_camp_size;
	int16_t              ai_research_size;
	int16_t              ai_war_size;
	int16_t              ai_harbor_size;
	int16_t              ai_market_size;
	int16_t              ai_inn_size;
	int16_t              ai_general_size;
	int16_t              ai_caravan_size;
	int16_t              ai_ship_size;
	int16_t              ai_town_count;
	int16_t              ai_base_count;
	int16_t              ai_mine_count;
	int16_t              ai_factory_count;
	int16_t              ai_camp_count;
	int16_t              ai_research_count;
	int16_t              ai_war_count;
	int16_t              ai_harbor_count;
	int16_t              ai_market_count;
	int16_t              ai_inn_count;
	int16_t              ai_general_count;
	int16_t              ai_caravan_count;
	int16_t              ai_ship_count;
	int16_t              ai_base_town_count;
	int16_t              firm_should_close_array[MAX_FIRM_TYPE];
	AIRegionGF           ai_region_array[MAX_AI_REGION];
	int8_t               ai_region_count;
	int8_t               pref_force_projection;
	int8_t               pref_military_development;
	int8_t               pref_economic_development;
	int8_t               pref_inc_pop_by_capture;
	int8_t               pref_inc_pop_by_growth;
	int8_t               pref_peacefulness;
	int8_t               pref_military_courage;
	int8_t               pref_territorial_cohesiveness;
	int8_t               pref_trading_tendency;
	int8_t               pref_allying_tendency;
	int8_t               pref_honesty;
	int8_t               pref_town_harmony;
	int8_t               pref_loyalty_concern;
	int8_t               pref_forgiveness;
	int8_t               pref_collect_tax;
	int8_t               pref_hire_unit;
	int8_t               pref_use_weapon;
	int8_t               pref_keep_general;
	int8_t               pref_keep_skilled_unit;
	int8_t               pref_diplomacy_retry;
	int8_t               pref_attack_monster;
	int8_t               pref_spy;
	int8_t               pref_counter_spy;
	int8_t               pref_food_reserve;
	int8_t               pref_cash_reserve;
	int8_t               pref_use_marine;
	int8_t               pref_unit_chase_distance;
	int8_t               pref_repair_concern;
	int8_t               pref_scout;
	int16_t              ai_capture_enemy_town_recno;
	int32_t              ai_capture_enemy_town_plan_date;
	int32_t              ai_capture_enemy_town_start_attack_date;
	int8_t               ai_capture_enemy_town_use_all_camp;
	int32_t              ai_last_defend_action_date;
	int16_t              ai_attack_target_x_loc;
	int16_t              ai_attack_target_y_loc;
	int16_t              ai_attack_target_nation_recno;
	AttackCampGF         attack_camp_array[MAX_SUITABLE_ATTACK_CAMP];
	int16_t              attack_camp_count;
	int16_t              lead_attack_camp_recno;
};

struct NationArrayGF
{
	int16_t              nation_count;
	int16_t              ai_nation_count;
	int32_t              last_del_nation_date;
	int32_t              last_new_nation_date;
	int32_t              max_nation_population;
	int32_t              all_nation_population;
	int16_t              independent_town_count;
	int16_t              independent_town_count_race_array[MAX_RACE];
	int32_t              max_nation_units;
	int32_t              max_nation_humans;
	int32_t              max_nation_generals;
	int32_t              max_nation_weapons;
	int32_t              max_nation_ships;
	int32_t              max_nation_spies;
	int32_t              max_nation_firms;
	int32_t              max_nation_tech_level;
	int32_t              max_population_rating;
	int32_t              max_military_rating;
	int32_t              max_economic_rating;
	int32_t              max_reputation;
	int32_t              max_kill_monster_score;
	int32_t              max_overall_rating;
	int16_t              max_population_nation_recno;
	int16_t              max_military_nation_recno;
	int16_t              max_economic_nation_recno;
	int16_t              max_reputation_nation_recno;
	int16_t              max_kill_monster_nation_recno;
	int16_t              max_overall_nation_recno;
	int32_t              last_alliance_id;
	int32_t              nation_peace_days;
	int16_t              player_recno;
	uint32_t             player_ptr; //zero
	int8_t               nation_color_array[MAX_NATION+1];
	int8_t               nation_power_color_array[MAX_NATION+2];
	int8_t               human_name_array[HUMAN_NAME_LEN+1][MAX_NATION];
};

struct Version_1_NationGF
{
	uint32_t             vtp;

	// NationBase
	int16_t              nation_recno;
	int8_t               nation_type;
	int8_t               race_id;
	int8_t               color_scheme_id;
	int8_t               nation_color;
	int16_t              king_unit_recno;
	int8_t               king_leadership;
	int32_t              nation_name_id;
	int8_t               nation_name_str[NationBase::NATION_NAME_LEN+1];
	uint32_t             player_id;
	int8_t               next_frame_ready;
	int16_t              last_caravan_id;
	int16_t              nation_firm_count;
	int32_t              last_build_firm_date;
	int8_t               know_base_array[VERSION_1_MAX_RACE];
	int8_t               base_count_array[VERSION_1_MAX_RACE];
	int8_t               is_at_war_today;
	int8_t               is_at_war_yesterday;
	int32_t              last_war_date;
	int16_t              last_attacker_unit_recno;
	int32_t              last_independent_unit_join_date;
	int8_t               cheat_enabled_flag;
	float                cash;
	float                food;
	float                reputation;
	float                kill_monster_score;
	int16_t              auto_collect_tax_loyalty;
	int16_t              auto_grant_loyalty;
	float                cur_year_profit;
	float                last_year_profit;
	float                cur_year_fixed_income;
	float                last_year_fixed_income;
	float                cur_year_fixed_expense;
	float                last_year_fixed_expense;
	float                cur_year_income_array[INCOME_TYPE_COUNT];
	float                last_year_income_array[INCOME_TYPE_COUNT];
	float                cur_year_income;
	float                last_year_income;
	float                cur_year_expense_array[EXPENSE_TYPE_COUNT];
	float                last_year_expense_array[EXPENSE_TYPE_COUNT];
	float                cur_year_expense;
	float                last_year_expense;
	float                cur_year_cheat;
	float                last_year_cheat;
	float                cur_year_food_in;
	float                last_year_food_in;
	float                cur_year_food_out;
	float                last_year_food_out;
	float                cur_year_food_change;
	float                last_year_food_change;
	float                cur_year_reputation_change;
	float                last_year_reputation_change;
	NationRelationGF     relation_array[MAX_NATION];
	int8_t               relation_status_array[MAX_NATION];
	int8_t               relation_passable_array[MAX_NATION];
	int8_t               relation_should_attack_array[MAX_NATION];
	int8_t               is_allied_with_player;
	int32_t              total_population;
	int32_t              total_jobless_population;
	int32_t              total_unit_count;
	int32_t              total_human_count;
	int32_t              total_general_count;
	int32_t              total_weapon_count;
	int32_t              total_ship_count;
	int32_t              total_firm_count;
	int32_t              total_spy_count;
	int32_t              total_ship_combat_level;
	int16_t              largest_town_recno;
	int16_t              largest_town_pop;
	int16_t              raw_count_array[MAX_RAW];
	uint16_t             last_unit_name_id_array[VERSION_1_MAX_UNIT_TYPE];
	int32_t              population_rating;
	int32_t              military_rating;
	int32_t              economic_rating;
	int32_t              overall_rating;
	int32_t              enemy_soldier_killed;
	int32_t              own_soldier_killed;
	int32_t              enemy_civilian_killed;
	int32_t              own_civilian_killed;
	int32_t              enemy_weapon_destroyed;
	int32_t              own_weapon_destroyed;
	int32_t              enemy_ship_destroyed;
	int32_t              own_ship_destroyed;
	int32_t              enemy_firm_destroyed;
	int32_t              own_firm_destroyed;

	// Nation
	DynArrayGF           action_array;
	uint16_t             last_action_id;
	uint32_t             ai_town_array; //zero
	uint32_t             ai_base_array; //zero
	uint32_t             ai_mine_array; //zero
	uint32_t             ai_factory_array; //zero
	uint32_t             ai_camp_array; //zero
	uint32_t             ai_research_array; //zero
	uint32_t             ai_war_array; //zero
	uint32_t             ai_harbor_array; //zero
	uint32_t             ai_market_array; //zero
	uint32_t             ai_inn_array; //zero
	uint32_t             ai_general_array; //zero
	uint32_t             ai_caravan_array; //zero
	uint32_t             ai_ship_array; //zero
	int16_t              ai_town_size;
	int16_t              ai_base_size;
	int16_t              ai_mine_size;
	int16_t              ai_factory_size;
	int16_t              ai_camp_size;
	int16_t              ai_research_size;
	int16_t              ai_war_size;
	int16_t              ai_harbor_size;
	int16_t              ai_market_size;
	int16_t              ai_inn_size;
	int16_t              ai_general_size;
	int16_t              ai_caravan_size;
	int16_t              ai_ship_size;
	int16_t              ai_town_count;
	int16_t              ai_base_count;
	int16_t              ai_mine_count;
	int16_t              ai_factory_count;
	int16_t              ai_camp_count;
	int16_t              ai_research_count;
	int16_t              ai_war_count;
	int16_t              ai_harbor_count;
	int16_t              ai_market_count;
	int16_t              ai_inn_count;
	int16_t              ai_general_count;
	int16_t              ai_caravan_count;
	int16_t              ai_ship_count;
	int16_t              ai_base_town_count;
	int16_t              firm_should_close_array[MAX_FIRM_TYPE];
	AIRegionGF           ai_region_array[MAX_AI_REGION];
	int8_t               ai_region_count;
	int8_t               pref_force_projection;
	int8_t               pref_military_development;
	int8_t               pref_economic_development;
	int8_t               pref_inc_pop_by_capture;
	int8_t               pref_inc_pop_by_growth;
	int8_t               pref_peacefulness;
	int8_t               pref_military_courage;
	int8_t               pref_territorial_cohesiveness;
	int8_t               pref_trading_tendency;
	int8_t               pref_allying_tendency;
	int8_t               pref_honesty;
	int8_t               pref_town_harmony;
	int8_t               pref_loyalty_concern;
	int8_t               pref_forgiveness;
	int8_t               pref_collect_tax;
	int8_t               pref_hire_unit;
	int8_t               pref_use_weapon;
	int8_t               pref_keep_general;
	int8_t               pref_keep_skilled_unit;
	int8_t               pref_diplomacy_retry;
	int8_t               pref_attack_monster;
	int8_t               pref_spy;
	int8_t               pref_counter_spy;
	int8_t               pref_food_reserve;
	int8_t               pref_cash_reserve;
	int8_t               pref_use_marine;
	int8_t               pref_unit_chase_distance;
	int8_t               pref_repair_concern;
	int8_t               pref_scout;
	int16_t              ai_capture_enemy_town_recno;
	int32_t              ai_capture_enemy_town_plan_date;
	int32_t              ai_capture_enemy_town_start_attack_date;
	int8_t               ai_capture_enemy_town_use_all_camp;
	int32_t              ai_last_defend_action_date;
	int16_t              ai_attack_target_x_loc;
	int16_t              ai_attack_target_y_loc;
	int16_t              ai_attack_target_nation_recno;
	AttackCampGF         attack_camp_array[MAX_SUITABLE_ATTACK_CAMP];
	int16_t              attack_camp_count;
	int16_t              lead_attack_camp_recno;
};

struct Version_1_NationArrayGF
{
	int16_t              nation_count;
	int16_t              ai_nation_count;
	int32_t              last_del_nation_date;
	int32_t              last_new_nation_date;
	int32_t              max_nation_population;
	int32_t              all_nation_population;
	int16_t              independent_town_count;
	int16_t              independent_town_count_race_array[VERSION_1_MAX_RACE];
	int32_t              max_nation_units;
	int32_t              max_nation_humans;
	int32_t              max_nation_generals;
	int32_t              max_nation_weapons;
	int32_t              max_nation_ships;
	int32_t              max_nation_spies;
	int32_t              max_nation_firms;
	int32_t              max_nation_tech_level;
	int32_t              max_population_rating;
	int32_t              max_military_rating;
	int32_t              max_economic_rating;
	int32_t              max_reputation;
	int32_t              max_kill_monster_score;
	int32_t              max_overall_rating;
	int16_t              max_population_nation_recno;
	int16_t              max_military_nation_recno;
	int16_t              max_economic_nation_recno;
	int16_t              max_reputation_nation_recno;
	int16_t              max_kill_monster_nation_recno;
	int16_t              max_overall_nation_recno;
	int32_t              last_alliance_id;
	int32_t              nation_peace_days;
	int16_t              player_recno;
	uint32_t             player_ptr; //zero
	int8_t               nation_color_array[MAX_NATION+1];
	int8_t               nation_power_color_array[MAX_NATION+2];
	int8_t               human_name_array[HUMAN_NAME_LEN+1][MAX_NATION];
};

struct BulletGF
{
	uint32_t             vtp; //zero

	// Sprite
	int16_t              sprite_id;
	int16_t              sprite_recno;
	int8_t               mobile_type;
	uint8_t              cur_action;
	uint8_t              cur_dir;
	uint8_t              cur_frame;
	uint8_t              cur_attack;
	uint8_t              final_dir;
	int8_t               turn_delay;
	int8_t               guard_count;
	uint8_t              remain_attack_delay;
	uint8_t              remain_frames_per_step;
	int16_t              cur_x;
	int16_t              cur_y;
	int16_t              go_x;
	int16_t              go_y;
	int16_t              next_x;
	int16_t              next_y;
	uint32_t             sprite_info; //zero

	// Bullet
	int8_t               parent_type;
	int16_t              parent_recno;
	int8_t               target_mobile_type;
	float                attack_damage;
	int16_t              damage_radius;
	int16_t              nation_recno;
	int8_t               fire_radius;
	int16_t              origin_x;
	int16_t              origin_y;
	int16_t              target_x_loc;
	int16_t              target_y_loc;
	int8_t               cur_step;
	int8_t               total_step;
};

struct BulletHomingGF
{
	int8_t               max_step;
	int8_t               target_type;
	int16_t              target_recno;
	int16_t              speed;
	int16_t              origin2_x;
	int16_t              origin2_y;
};

struct ProjectileGF
{
	float                z_coff;
	SpriteGF             act_bullet;
	SpriteGF             bullet_shadow;
};

struct WorkerGF
{
	int8_t               race_id;
	int8_t               unit_id;
	int16_t              town_recno;
	uint16_t             name_id;
	int8_t               skill_id;
	int8_t               skill_level;
	uint8_t              skill_level_minor;
	uint8_t              skill_potential;
	int8_t               combat_level;
	uint8_t              combat_level_minor;
	int16_t              spy_recno;
	int8_t               rank_id;
	int8_t               worker_loyalty;
	int16_t              hit_points;
	int16_t              extra_para;
};

struct FirmGF
{
	uint32_t             vtp; //zero

	int8_t               firm_id;
	int16_t              firm_build_id;
	int16_t              firm_recno;
	int8_t               firm_ai;
	int8_t               ai_processed;
	int8_t               ai_status;
	int8_t               ai_link_checked;
	int8_t               ai_sell_flag;
	int8_t               race_id;
	int16_t              nation_recno;
	int16_t              closest_town_name_id;
	int16_t              firm_name_instance_id;
	int16_t              loc_x1;
	int16_t              loc_y1;
	int16_t              loc_x2;
	int16_t              loc_y2;
	int16_t              abs_x1;
	int16_t              abs_y1;
	int16_t              abs_x2;
	int16_t              abs_y2;
	int16_t              center_x;
	int16_t              center_y;
	uint8_t              region_id;
	int8_t               cur_frame;
	int8_t               remain_frame_delay;
	float                hit_points;
	float                max_hit_points;
	int8_t               under_construction;
	int8_t               firm_skill_id;
	int16_t              overseer_recno;
	int16_t              overseer_town_recno;
	int16_t              builder_recno;
	uint8_t              builder_region_id;
	float                productivity;
	uint32_t             worker_array; //zero
	int8_t               worker_count;
	int8_t               selected_worker_id;
	int8_t               player_spy_count;
	uint8_t              sabotage_level;
	int8_t               linked_firm_count;
	int8_t               linked_town_count;
	int16_t              linked_firm_array[MAX_LINKED_FIRM_FIRM];
	int16_t              linked_town_array[MAX_LINKED_FIRM_TOWN];
	int8_t               linked_firm_enable_array[MAX_LINKED_FIRM_FIRM];
	int8_t               linked_town_enable_array[MAX_LINKED_FIRM_TOWN];
	float                last_year_income;
	float                cur_year_income;
	int32_t              setup_date;
	int8_t               should_set_power;
	int32_t              last_attacked_date;
	int8_t               should_close_flag;
	int8_t               no_neighbor_space;
	int8_t               ai_should_build_factory_count;
};

struct FirmBaseGF
{
	int16_t              god_id;
	int16_t              god_unit_recno;
	float                pray_points;
};

struct DefenseUnitGF
{
	int16_t              unit_recno;
	int8_t               status;
};

struct FirmCampGF
{
	DefenseUnitGF        defense_array[MAX_WORKER+1];
	int8_t               employ_new_worker;
	int16_t              defend_target_recno;
	int8_t               defense_flag;
	int8_t               patrol_unit_count;
	int16_t              patrol_unit_array[MAX_WORKER+1];
	int8_t               coming_unit_count;
	int16_t              coming_unit_array[MAX_WORKER+1];
	int16_t              ai_capture_town_recno;
	int8_t               ai_recruiting_soldier;
	int8_t               is_attack_camp;
};

struct FirmFactoryGF
{
	int32_t              product_raw_id;
	float                stock_qty;
	float                max_stock_qty;
	float                raw_stock_qty;
	float                max_raw_stock_qty;
	float                cur_month_production;
	float                last_month_production;
	int16_t              next_output_link_id;
	int16_t              next_output_firm_recno;
};

struct FirmHarborGF
{
	int16_t              ship_recno_array[MAX_SHIP_IN_HARBOR];
	int16_t              ship_count;
	int16_t              build_unit_id;
	uint32_t             start_build_frame_no;
	int8_t               build_queue_array[MAX_BUILD_SHIP_QUEUE];
	int8_t               build_queue_count;
	uint8_t              land_region_id;
	uint8_t              sea_region_id;
	int8_t               link_checked;
	int8_t               linked_mine_num;
	int8_t               linked_factory_num;
	int8_t               linked_market_num;
	int16_t              linked_mine_array[MAX_LINKED_FIRM_FIRM];
	int16_t              linked_factory_array[MAX_LINKED_FIRM_FIRM];
	int16_t              linked_market_array[MAX_LINKED_FIRM_FIRM];
};

struct InnUnitGF
{
	int8_t               unit_id;
	SkillGF              skill;
	int16_t              hire_cost;
	int16_t              stay_count;
	int16_t              spy_recno;
};

struct FirmInnGF
{
	int16_t              next_skill_id;
	InnUnitGF            inn_unit_array[MAX_INN_UNIT];
	int16_t              inn_unit_count;
};

struct MarketGoodsGF
{
	int8_t               raw_id;
	int8_t               product_raw_id;
	int16_t              input_firm_recno;
	float                stock_qty;
	float                cur_month_supply;
	float                last_month_supply;
	float                month_demand;
	float                cur_month_sale_qty;
	float                last_month_sale_qty;
	float                cur_year_sales;
	float                last_year_sales;
};

struct FirmMarketGF
{
	float                max_stock_qty;
	MarketGoodsGF        market_goods_array[MAX_MARKET_GOODS];
	uint32_t             market_raw_array[MAX_RAW]; //zero
	uint32_t             market_product_array[MAX_PRODUCT]; //zero
	int16_t              next_output_link_id;
	int16_t              next_output_firm_recno;
	int32_t              no_linked_town_since_date;
	int32_t              last_import_new_goods_date;
	int8_t               restock_type;
};

struct FirmMineGF
{
	int16_t              raw_id;
	int16_t              site_recno;
	float                reserve_qty;
	float                stock_qty;
	float                max_stock_qty;
	int16_t              next_output_link_id;
	int16_t              next_output_firm_recno;
	float                cur_month_production;
	float                last_month_production;
};

struct MonsterInFirmGF
{
	int8_t               monster_id;
	int8_t               _unused;
	int16_t              mobile_unit_recno;
	int8_t               combat_level;
	int16_t              hit_points;
	int16_t              max_hit_points;
	int8_t               soldier_monster_id;
	int8_t               soldier_count;
};

struct FirmMonsterGF
{
	int16_t              monster_id;
	int16_t              monster_general_count;
	int8_t               monster_aggressiveness;
	int8_t               defending_king_count;
	int8_t               defending_general_count;
	int8_t               defending_soldier_count;
	MonsterInFirmGF      monster_king;
	MonsterInFirmGF      monster_general_array[MAX_MONSTER_GENERAL_IN_FIRM];
	int8_t               waiting_soldier_count;
	int16_t              waiting_soldier_array[FirmMonster::MAX_WAITING_SOLDIER];
	int8_t               monster_nation_relation;
	int16_t              defend_target_recno;
	int8_t               patrol_unit_count;
	int16_t              patrol_unit_array[MAX_SOLDIER_PER_GENERAL+1];
};

struct FirmResearchGF
{
	int16_t              tech_id;
	float                complete_percent;
};

struct FirmWarGF
{
	int16_t              build_unit_id;
	uint32_t             last_process_build_frame_no;
	float                build_progress_days;
	int8_t               build_queue_array[MAX_BUILD_QUEUE];
	int8_t               build_queue_count;
};

struct SiteGF
{
	int16_t              site_recno;
	int8_t               site_type;
	int16_t              object_id;
	int32_t              reserve_qty;
	int8_t               has_mine;
	int16_t              map_x_loc;
	int16_t              map_y_loc;
	uint8_t              region_id;
};

struct TownArrayGF
{
	int32_t              race_wander_pop_array[MAX_RACE];
};

struct Version_1_TownArrayGF
{
	int32_t              race_wander_pop_array[VERSION_1_MAX_RACE];
};

struct TownGF
{
	int16_t              town_recno;
	int16_t              town_name_id;
	int16_t              nation_recno;
	int16_t              rebel_recno;
	int8_t               race_id;
	int32_t              setup_date;
	int8_t               ai_town;
	int8_t               ai_link_checked;
	int8_t               independ_town_nation_relation;
	int8_t               has_linked_own_camp;
	int8_t               has_linked_enemy_camp;
	int8_t               is_base_town;
	int16_t              loc_x1;
	int16_t              loc_y1;
	int16_t              loc_x2;
	int16_t              loc_y2;
	int16_t              abs_x1;
	int16_t              abs_y1;
	int16_t              abs_x2;
	int16_t              abs_y2;
	int16_t              center_x;
	int16_t              center_y;
	uint8_t              region_id;
	int16_t              layout_id;
	int16_t              first_slot_id;
	int16_t              slot_object_id_array[MAX_TOWN_LAYOUT_SLOT];
	int16_t              population;
	int16_t              jobless_population;
	int16_t              max_race_pop_array[MAX_RACE];
	int16_t              race_pop_array[MAX_RACE];
	uint8_t              race_pop_growth_array[MAX_RACE];
	int16_t              jobless_race_pop_array[MAX_RACE];
	float                race_loyalty_array[MAX_RACE];
	int8_t               race_target_loyalty_array[MAX_RACE];
	int16_t              race_spy_count_array[MAX_RACE];
	float                race_resistance_array[MAX_RACE][MAX_NATION];
	int8_t               race_target_resistance_array[MAX_RACE][MAX_NATION];
	int16_t              town_defender_count;
	int32_t              last_being_attacked_date;
	float                received_hit_count;
	int8_t               train_queue_skill_array[MAX_TRAIN_QUEUE];
	int8_t               train_queue_race_array[MAX_TRAIN_QUEUE];
	int8_t               train_queue_count;
	int16_t              train_unit_recno;
	int32_t              train_unit_action_id;
	uint32_t             start_train_frame_no;
	int16_t              defend_target_recno;
	int32_t              accumulated_collect_tax_penalty;
	int32_t              accumulated_reward_penalty;
	int32_t              accumulated_recruit_penalty;
	int32_t              accumulated_enemy_grant_penalty;
	int32_t              last_rebel_date;
	int16_t              independent_unit_join_nation_min_rating;
	int16_t              quality_of_life;
	int16_t              auto_collect_tax_loyalty;
	int16_t              auto_grant_loyalty;
	int8_t               town_combat_level;
	int8_t               has_product_supply[MAX_PRODUCT];
	int8_t               no_neighbor_space;
	int16_t              linked_firm_count;
	int16_t              linked_town_count;
	int16_t              linked_firm_array[MAX_LINKED_FIRM_TOWN];
	int16_t              linked_town_array[MAX_LINKED_TOWN_TOWN];
	int8_t               linked_firm_enable_array[MAX_LINKED_FIRM_TOWN];
	int8_t               linked_town_enable_array[MAX_LINKED_TOWN_TOWN];
};

struct Version_1_TownGF
{
	int16_t              town_recno;
	int16_t              town_name_id;
	int16_t              nation_recno;
	int16_t              rebel_recno;
	int8_t               race_id;
	int32_t              setup_date;
	int8_t               ai_town;
	int8_t               ai_link_checked;
	int8_t               independ_town_nation_relation;
	int8_t               has_linked_own_camp;
	int8_t               has_linked_enemy_camp;
	int8_t               is_base_town;
	int16_t              loc_x1;
	int16_t              loc_y1;
	int16_t              loc_x2;
	int16_t              loc_y2;
	int16_t              abs_x1;
	int16_t              abs_y1;
	int16_t              abs_x2;
	int16_t              abs_y2;
	int16_t              center_x;
	int16_t              center_y;
	uint8_t              region_id;
	int16_t              layout_id;
	int16_t              first_slot_id;
	int16_t              slot_object_id_array[MAX_TOWN_LAYOUT_SLOT];
	int16_t              population;
	int16_t              jobless_population;
	int16_t              max_race_pop_array[VERSION_1_MAX_RACE];
	int16_t              race_pop_array[VERSION_1_MAX_RACE];
	uint8_t              race_pop_growth_array[VERSION_1_MAX_RACE];
	int16_t              jobless_race_pop_array[VERSION_1_MAX_RACE];
	float                race_loyalty_array[VERSION_1_MAX_RACE];
	int8_t               race_target_loyalty_array[VERSION_1_MAX_RACE];
	int16_t              race_spy_count_array[VERSION_1_MAX_RACE];
	float                race_resistance_array[MAX_NATION][VERSION_1_MAX_RACE];
	int8_t               race_target_resistance_array[MAX_NATION][VERSION_1_MAX_RACE];
	int16_t              town_defender_count;
	int32_t              last_being_attacked_date;
	float                received_hit_count;
	int8_t               train_queue_skill_array[MAX_TRAIN_QUEUE];
	int8_t               train_queue_race_array[MAX_TRAIN_QUEUE];
	int8_t               train_queue_count;
	int16_t              train_unit_recno;
	int32_t              train_unit_action_id;
	uint32_t             start_train_frame_no;
	int16_t              defend_target_recno;
	int32_t              accumulated_collect_tax_penalty;
	int32_t              accumulated_reward_penalty;
	int32_t              accumulated_recruit_penalty;
	int32_t              accumulated_enemy_grant_penalty;
	int32_t              last_rebel_date;
	int16_t              independent_unit_join_nation_min_rating;
	int16_t              quality_of_life;
	int16_t              auto_collect_tax_loyalty;
	int16_t              auto_grant_loyalty;
	int8_t               town_combat_level;
	int8_t               has_product_supply[MAX_PRODUCT];
	int8_t               no_neighbor_space;
	int16_t              linked_firm_count;
	int16_t              linked_town_count;
	int16_t              linked_firm_array[MAX_LINKED_FIRM_TOWN];
	int16_t              linked_town_array[MAX_LINKED_TOWN_TOWN];
	int8_t               linked_firm_enable_array[MAX_LINKED_FIRM_TOWN];
	int8_t               linked_town_enable_array[MAX_LINKED_TOWN_TOWN];
};

struct TornadoGF
{
	uint32_t             vtp; //zero

	// Sprite
	int16_t              sprite_id;
	int16_t              sprite_recno;
	int8_t               mobile_type;
	uint8_t              cur_action;
	uint8_t              cur_dir;
	uint8_t              cur_frame;
	uint8_t              cur_attack;
	uint8_t              final_dir;
	int8_t               turn_delay;
	int8_t               guard_count;
	uint8_t              remain_attack_delay;
	uint8_t              remain_frames_per_step;
	int16_t              cur_x;
	int16_t              cur_y;
	int16_t              go_x;
	int16_t              go_y;
	int16_t              next_x;
	int16_t              next_y;
	uint32_t             sprite_info; //zero

	// Tornado
	float                attack_damage;
	int16_t              life_time;
	int16_t              dmg_offset_x;
	int16_t              dmg_offset_y;
};

struct RebelGF
{
	int16_t              rebel_recno;
	int16_t              leader_unit_recno;
	int8_t               action_mode;
	int16_t              action_para;
	int16_t              action_para2;
	int16_t              mobile_rebel_count;
	int16_t              town_recno;
	int8_t               hostile_nation_bits;
};

struct SnowGroundArrayGF
{
	uint32_t             seed;
	int32_t              snow_thick;
	int32_t              snow_pattern;
};

struct RegionInfoGF
{
	uint8_t              region_id;
	uint8_t              region_stat_id;
	int32_t              region_type;
	int32_t              adj_offset_bit;
	int32_t              region_size;
	int16_t              center_x;
	int16_t              center_y;
};

struct RegionPathGF
{
	uint8_t              sea_region_id;
	uint8_t              land_region_stat_id;
};

struct RegionStatGF
{
	uint8_t              region_id;
	int8_t               nation_is_present_array[MAX_NATION];
	int8_t               nation_presence_count;
	int16_t              firm_type_count_array[MAX_FIRM_TYPE];
	int16_t              firm_nation_count_array[MAX_NATION];
	int16_t              camp_nation_count_array[MAX_NATION];
	int16_t              mine_nation_count_array[MAX_NATION];
	int16_t              harbor_nation_count_array[MAX_NATION];
	int16_t              total_firm_count;
	int16_t              town_nation_count_array[MAX_NATION];
	int16_t              base_town_nation_count_array[MAX_NATION];
	int16_t              independent_town_count;
	int16_t              total_town_count;
	int16_t              nation_population_array[MAX_NATION];
	int16_t              nation_jobless_population_array[MAX_NATION];
	int16_t              unit_nation_count_array[MAX_NATION];
	int16_t              independent_unit_count;
	int16_t              total_unit_count;
	int16_t              site_count;
	int16_t              raw_count;
	RegionPathGF         reachable_region_array[MAX_REACHABLE_REGION_PER_STAT];
	int8_t               reachable_region_count;
};

struct RegionArrayGF
{
	int32_t              init_flag;
	uint32_t             region_info_array; //zero
	int32_t              region_info_count;
	uint32_t             region_stat_array; //zero
	int32_t              region_stat_count;
	uint32_t             connect_bits; //zero
	uint8_t              region_sorted_array[MAX_REGION];
};

struct NewsGF
{
	int8_t               id;
	int8_t               type;
	int32_t              news_date;
	int8_t               nation_color1;
	int8_t               nation_color2;
	int8_t               nation_race_id1;
	int8_t               nation_race_id2;
	int32_t              nation_name_id1;
	int32_t              nation_name_id2;
	int16_t              short_para1;
	int16_t              short_para2;
	int16_t              short_para3;
	int16_t              short_para4;
	int16_t              short_para5;
	int8_t               loc_type;
	int16_t              loc_type_para;
	uint16_t             loc_type_para2;
	int16_t              loc_x;
	int16_t              loc_y;
};
#pragma pack()


union GFRec
{
	BulletGF bullet;
	BulletHomingGF bullet_homing;
	ConfigGF config;
	DynArrayGF dyn_array;
	FirmGF firm;
	FirmBaseGF firm_base;
	FirmCampGF firm_camp;
	FirmFactoryGF firm_factory;
	FirmHarborGF firm_harbor;
	FirmInnGF firm_inn;
	FirmMarketGF firm_market;
	FirmMineGF firm_mine;
	FirmMonsterGF firm_monster;
	FirmResearchGF firm_research;
	FirmWarGF firm_war;
	GameGF game;
	GameFileHeader game_file;
	InfoGF info;
	MagicWeatherGF magic_weather;
	NationGF nation;
	NationArrayGF nation_array;
	ProjectileGF projectile;
	PowerGF power;
	RebelGF rebel;
	RegionArrayGF region_array;
	SnowGroundArrayGF snow_ground_array;
	TalkResGF talk_res;
	TeamInfoGF team_info;
	TornadoGF tornado;
	TownGF town;
	TownArrayGF town_array;
	UnitGF unit;
	UnitCaravanGF unit_caravan;
	UnitExpCartGF unit_exp_cart;
	UnitGodGF unit_god;
	UnitMarineGF unit_marine;
	UnitMonsterGF unit_monster;
	UnitVehicleGF unit_vehicle;
	WeatherGF weather;
	Version_1_NationGF nation_v1;
	Version_1_NationArrayGF nation_array_v1;
	Version_1_TownGF town_v1;
	Version_1_TownArrayGF town_array_v1;
};

extern GFRec gf_rec;

#endif
