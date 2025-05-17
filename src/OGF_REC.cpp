/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Cpyright 2024 Jesse Allen
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

//Filename    : OGF_REC.CPP
//Description : Read and write records to file and their objects
//Owner       : Jesse Allen

#include <OGF_REC.h>

#define ReadInt8(a) a=r->a
#define ReadInt16(a) a=SDL_SwapLE16(r->a)
#define ReadInt32(a) a=SDL_SwapLE32(r->a)
#define ReadFloat(a) a=SDL_SwapFloatLE(r->a)
#define ReadCall(a) a.read_record(&r->a)

#define ReadInt8Array(a,n) memcpy(&a,&r->a,n)
#define ReadInt16Array(a,n) for(int __i=0;__i<(n);__i++) ReadInt16(a[__i])
#define ReadInt32Array(a,n) for(int __i=0;__i<(n);__i++) ReadInt32(a[__i])
#define ReadFloatArray(a,n) copy_float_array((float*)a,(float*)r->a,n)
#define ReadCallArray(a,n) for(int __i=0;__i<(n);__i++) ReadCall(a[__i])

#define WriteInt8(a) r->a=a
#define WriteInt16(a) r->a=SDL_SwapLE16(a)
#define WriteInt32(a) r->a=SDL_SwapLE32(a)
#define WriteFloat(a) r->a=SDL_SwapFloatLE(a)
#define WriteCall(a) a.write_record(&r->a)
#define WriteZero(a) r->a=0

#define WriteInt8Array(a,n) memcpy(&r->a,&a,n)
#define WriteInt16Array(a,n) for(int __i=0;__i<(n);__i++) WriteInt16(a[__i])
#define WriteInt32Array(a,n) for(int __i=0; __i<(n); __i++) WriteInt32(a[__i])
#define WriteFloatArray(a,n) copy_float_array((float*)r->a,(float*)a,n)
#define WriteCallArray(a,n) for(int __i=0;__i<(n);__i++) WriteCall(a[__i])
#define WriteZeroArray(a,n) for(int __i=0;__i<(n);__i++) WriteZero(a[__i])
#define WriteZeroBytes(a,n) memset(&r->a,0,n)

static void copy_float_array(float *d, float *s, size_t n)
{
	for( size_t i=0; i<n; i++ )
		d[i] = SDL_SwapFloatLE(s[i]);
}

union GFRec gf_rec;

void GameFile::write_record(GameFileHeader *r)
{
	WriteInt32(class_size);
	WriteInt8Array(file_name, FilePath::MAX_FILE_PATH+1);
	WriteInt8Array(player_name, HUMAN_NAME_LEN+1);
	WriteInt8(race_id);
	WriteInt8(nation_color);
	WriteInt32(game_date);
	WriteInt32(file_date.dwLowDateTime);
	WriteInt32(file_date.dwHighDateTime);
	WriteInt16(terrain_set);
}

void GameFile::read_record(GameFileHeader *r)
{
	ReadInt32(class_size);
	ReadInt8Array(file_name, FilePath::MAX_FILE_PATH+1);
	ReadInt8Array(player_name, HUMAN_NAME_LEN+1);
	ReadInt8(race_id);
	ReadInt8(nation_color);
	ReadInt32(game_date);
	ReadInt32(file_date.dwLowDateTime);
	ReadInt32(file_date.dwHighDateTime);
	ReadInt16(terrain_set);
}

void FirmInfo::write_record(FirmInfoGF *r)
{
	WriteInt8(firm_id);
	WriteZeroBytes(name, NAME_LEN+1);
	WriteZeroBytes(short_name, SHORT_NAME_LEN+1);
	WriteZeroBytes(overseer_title, TITLE_LEN+1);
	WriteZeroBytes(worker_title, TITLE_LEN+1);
	WriteInt8(tera_type);
	WriteInt8(buildable);
	WriteInt8(live_in_town);
	WriteInt16(max_hit_points);
	WriteInt8(need_overseer);
	WriteInt8(need_worker);
	WriteInt16(setup_cost);
	WriteInt16(year_cost);
	WriteZero(first_build_id);
	WriteZero(build_count);
	WriteInt16(loc_width);
	WriteInt16(loc_height);
	WriteInt8(firm_skill_id);
	WriteInt8(firm_race_id);
	WriteInt8(is_linkable_to_town);
	WriteInt16(total_firm_count);
	WriteInt16Array(nation_firm_count_array, MAX_NATION);
	WriteInt8Array(nation_tech_level_array, MAX_NATION);
}

void FirmInfo::read_record(FirmInfoGF *r)
{
	ReadInt8(firm_id);
	// skip name
	// skip short_name
	// skip overseer_title
	// skip worker_title
	ReadInt8(tera_type);
	ReadInt8(buildable);
	ReadInt8(live_in_town);
	ReadInt16(max_hit_points);
	ReadInt8(need_overseer);
	ReadInt8(need_worker);
	ReadInt16(setup_cost);
	ReadInt16(year_cost);
	// skip first_build_id
	// skip build_count
	ReadInt16(loc_width);
	ReadInt16(loc_height);
	ReadInt8(firm_skill_id);
	ReadInt8(firm_race_id);
	ReadInt8(is_linkable_to_town);
	ReadInt16(total_firm_count);
	ReadInt16Array(nation_firm_count_array, MAX_NATION);
	ReadInt8Array(nation_tech_level_array, MAX_NATION);
}

void TechClass::write_record(TechClassGF *r)
{
	WriteInt16(class_id);
	WriteInt16(first_tech_id);
	WriteInt16(tech_count);
	WriteInt16(icon_index);
	WriteInt16Array(nation_research_firm_recno_array, MAX_NATION);
}

void TechClass::read_record(TechClassGF *r)
{
	ReadInt16(class_id);
	ReadInt16(first_tech_id);
	ReadInt16(tech_count);
	ReadInt16(icon_index);
	ReadInt16Array(nation_research_firm_recno_array, MAX_NATION);
}

void TechInfo::write_record(TechInfoGF *r)
{
	WriteInt16(tech_id);
	WriteInt16(class_id);
	WriteInt16(max_tech_level);
	WriteInt16(complex_level);
	WriteInt16(unit_id);
	WriteInt16(firm_id);
	WriteInt16(parent_unit_id);
	WriteInt16(parent_firm_id);
	WriteInt8(parent_level);
	WriteInt16(icon_index);
	WriteInt8Array(nation_tech_level_array, MAX_NATION);
	WriteInt8Array(nation_is_researching_array, MAX_NATION);
	WriteFloatArray(nation_research_progress_array, MAX_NATION);
}

void TechInfo::read_record(TechInfoGF *r)
{
	ReadInt16(tech_id);
	ReadInt16(class_id);
	ReadInt16(max_tech_level);
	ReadInt16(complex_level);
	ReadInt16(unit_id);
	ReadInt16(firm_id);
	ReadInt16(parent_unit_id);
	ReadInt16(parent_firm_id);
	ReadInt8(parent_level);
	ReadInt16(icon_index);
	ReadInt8Array(nation_tech_level_array, MAX_NATION);
	ReadInt8Array(nation_is_researching_array, MAX_NATION);
	ReadFloatArray(nation_research_progress_array, MAX_NATION);
}

void DynArray::write_record(DynArrayGF *r)
{
	WriteInt32(ele_num);
	WriteInt32(block_num);
	WriteInt32(cur_pos);
	WriteInt32(last_ele);
	WriteInt32(ele_size);
	WriteInt32(sort_offset);
	WriteInt8(sort_type);
	WriteZero(body_buf); // the unknown array type must be saved by derived users
}

void DynArray::read_record(DynArrayGF *r)
{
	ReadInt32(ele_num);
	ReadInt32(block_num);
	ReadInt32(cur_pos);
	ReadInt32(last_ele);
	ReadInt32(ele_size);
	ReadInt32(sort_offset);
	ReadInt8(sort_type);
	// skip body_buf // the unknown array type must be restored by derived users
}

void TalkMsg::write_record(TalkMsgGF *r)
{
	WriteInt16(talk_id);
	WriteInt16(talk_para1);
	WriteInt16(talk_para2);
	WriteInt32(date);
	WriteInt8(from_nation_recno);
	WriteInt8(to_nation_recno);
	WriteInt8(reply_type);
	WriteInt32(reply_date);
	WriteInt8(relation_status);
}

void TalkMsg::read_record(TalkMsgGF *r)
{
	ReadInt16(talk_id);
	ReadInt16(talk_para1);
	ReadInt16(talk_para2);
	ReadInt32(date);
	ReadInt8(from_nation_recno);
	ReadInt8(to_nation_recno);
	ReadInt8(reply_type);
	ReadInt32(reply_date);
	ReadInt8(relation_status);
}

void TalkChoice::write_record(TalkChoiceGF *r)
{
	WriteZero(str);
	WriteInt16(para);
}

void TalkChoice::read_record(TalkChoiceGF *r)
{
	str = NULL;
	ReadInt16(para);
}

void TalkRes::write_record(TalkResGF *r)
{
	WriteInt8(init_flag);
	WriteInt16(reply_talk_msg_recno);
	WriteCall(cur_talk_msg);
	WriteZero(choice_question);
	WriteZero(choice_question_second_line);
	WriteZero(talk_choice_count);
	WriteCallArray(talk_choice_array, MAX_TALK_CHOICE);
	WriteInt8Array(available_talk_id_array, MAX_TALK_TYPE);
	WriteInt16(cur_choice_id);
	WriteInt8(save_view_mode);
	WriteInt8(msg_add_nation_color);
	WriteZeroBytes(talk_msg_array, sizeof(DynArrayGF));
	WriteZeroBytes(empty_room_bytes, sizeof(DynArrayB_Unused));
}

void TalkRes::read_record(TalkResGF *r)
{
	ReadInt8(init_flag);
	ReadInt16(reply_talk_msg_recno);
	ReadCall(cur_talk_msg);
	choice_question = NULL;
	choice_question_second_line = NULL;
	talk_choice_count = 0;
	ReadCallArray(talk_choice_array, MAX_TALK_CHOICE);
	ReadInt8Array(available_talk_id_array, MAX_TALK_TYPE);
	ReadInt16(cur_choice_id);
	ReadInt8(save_view_mode);
	ReadInt8(msg_add_nation_color);
	// skip talk_msg_array
	// skip empty_room_bytes
}

void GodInfo::write_record(GodInfoGF *r)
{
	WriteInt8(god_id);
	WriteInt8(race_id);
	WriteInt8(unit_id);
	WriteInt16(exist_pray_points);
	WriteInt16(power_pray_points);
	WriteInt8(can_cast_power);
	WriteInt8(cast_power_range);
	WriteInt8Array(nation_know_array, MAX_NATION);
}

void GodInfo::read_record(GodInfoGF *r)
{
	ReadInt8(god_id);
	ReadInt8(race_id);
	ReadInt8(unit_id);
	ReadInt16(exist_pray_points);
	ReadInt16(power_pray_points);
	ReadInt8(can_cast_power);
	ReadInt8(cast_power_range);
	ReadInt8Array(nation_know_array, MAX_NATION);
}

void ColorRemap::write_record(ColorRemapGF *r)
{
	WriteInt8(main_color);
	WriteInt8Array(color_table, 256);
}

void ColorRemap::read_record(ColorRemapGF *r)
{
	ReadInt8(main_color);
	ReadInt8Array(color_table, 256);
}

void Game::write_record(GameGF *r)
{
	WriteInt8(init_flag);
	WriteInt8(started_flag);
	WriteInt8(game_mode);
	WriteInt8(game_has_ended);
	WriteCallArray(color_remap_array, MAX_COLOR_SCHEME+1);
}

void Game::read_record(GameGF *r)
{
	ReadInt8(init_flag);
	ReadInt8(started_flag);
	ReadInt8(game_mode);
	ReadInt8(game_has_ended);
	ReadCallArray(color_remap_array, MAX_COLOR_SCHEME+1);
}

void Config::write_record(ConfigGF *r)
{
	WriteInt16(difficulty_rating);
	WriteInt8(ai_nation_count);
	WriteInt16(start_up_cash);
	WriteInt16(ai_start_up_cash);
	WriteInt8(ai_aggressiveness);
	WriteInt16(start_up_independent_town);
	WriteInt16(start_up_raw_site);
	WriteInt8(difficulty_level);
	WriteInt8(explore_whole_map);
	WriteInt8(fog_of_war);
	WriteInt16(terrain_set);
	WriteInt16(latitude);
	WriteInt8(weather_effect);
	WriteInt8(land_mass);
	WriteInt8(new_independent_town_emerge);
	WriteInt8(independent_town_resistance);
	WriteInt8(random_event_frequency);
	WriteInt8(new_nation_emerge);
	WriteInt8(monster_type);
	WriteInt8(start_up_has_mine_nearby);
	WriteInt8(random_start_up);
	WriteInt8(goal_destroy_monster);
	WriteInt8(goal_population_flag);
	WriteInt8(goal_economic_score_flag);
	WriteInt8(goal_total_score_flag);
	WriteInt8(goal_year_limit_flag);
	WriteInt32(goal_population);
	WriteInt32(goal_economic_score);
	WriteInt32(goal_total_score);
	WriteInt32(goal_year_limit);
	WriteInt8(fire_spread_rate);
	WriteInt8(wind_spread_fire_rate);
	WriteInt8(fire_fade_rate);
	WriteInt8(fire_restore_prob);
	WriteInt8(rain_reduce_fire_rate);
	WriteInt8(fire_damage);
	WriteInt8(show_ai_info);
	WriteInt8(fast_build);
	WriteInt8(disable_ai_flag);
	WriteInt8(king_undie_flag);
	WriteInt8(race_id);
	WriteInt8Array(player_name, HUMAN_NAME_LEN+1);
	WriteInt8(player_nation_color);
	WriteInt8(expired_flag);
	WriteInt8(opaque_report);
	WriteInt8(disp_news_flag);
	WriteInt16(scroll_speed);
	WriteInt16(frame_speed);
	WriteInt8(help_mode);
	WriteInt8(disp_town_name);
	WriteInt8(disp_spy_sign);
	WriteInt8(show_all_unit_icon);
	WriteInt8(show_unit_path);
	WriteInt8(music_flag);
	WriteInt16(cd_music_volume);
	WriteInt16(wav_music_volume);
	WriteInt8(sound_effect_flag);
	WriteInt16(sound_effect_volume);
	WriteInt8(pan_control);
	WriteInt8(lightning_visual);
	WriteInt8(earthquake_visual);
	WriteInt8(rain_visual);
	WriteInt8(snow_visual);
	WriteInt8(snow_ground);
	WriteInt8(lightning_audio);
	WriteInt8(earthquake_audio);
	WriteInt8(rain_audio);
	WriteInt8(snow_audio);
	WriteInt8(wind_audio);
	WriteInt32(lightning_brightness);
	WriteInt32(cloud_darkness);
	WriteInt32(lightning_volume);
	WriteInt32(earthquake_volume);
	WriteInt32(rain_volume);
	WriteInt32(snow_volume);
	WriteInt32(wind_volume);
	WriteInt8(blacken_map);
	WriteInt8(explore_mask_method);
	WriteInt8(fog_mask_method);
}

void Config::read_record(ConfigGF *r)
{
	ReadInt16(difficulty_rating);
	ReadInt8(ai_nation_count);
	ReadInt16(start_up_cash);
	ReadInt16(ai_start_up_cash);
	ReadInt8(ai_aggressiveness);
	ReadInt16(start_up_independent_town);
	ReadInt16(start_up_raw_site);
	ReadInt8(difficulty_level);
	ReadInt8(explore_whole_map);
	ReadInt8(fog_of_war);
	ReadInt16(terrain_set);
	ReadInt16(latitude);
	ReadInt8(weather_effect);
	ReadInt8(land_mass);
	ReadInt8(new_independent_town_emerge);
	ReadInt8(independent_town_resistance);
	ReadInt8(random_event_frequency);
	ReadInt8(new_nation_emerge);
	ReadInt8(monster_type);
	ReadInt8(start_up_has_mine_nearby);
	ReadInt8(random_start_up);
	ReadInt8(goal_destroy_monster);
	ReadInt8(goal_population_flag);
	ReadInt8(goal_economic_score_flag);
	ReadInt8(goal_total_score_flag);
	ReadInt8(goal_year_limit_flag);
	ReadInt32(goal_population);
	ReadInt32(goal_economic_score);
	ReadInt32(goal_total_score);
	ReadInt32(goal_year_limit);
	ReadInt8(fire_spread_rate);
	ReadInt8(wind_spread_fire_rate);
	ReadInt8(fire_fade_rate);
	ReadInt8(fire_restore_prob);
	ReadInt8(rain_reduce_fire_rate);
	ReadInt8(fire_damage);
	ReadInt8(show_ai_info);
	ReadInt8(fast_build);
	ReadInt8(disable_ai_flag);
	ReadInt8(king_undie_flag);
	ReadInt8(race_id);
	ReadInt8Array(player_name, HUMAN_NAME_LEN+1);
	ReadInt8(player_nation_color);
	ReadInt8(expired_flag);
	ReadInt8(opaque_report);
	ReadInt8(disp_news_flag);
	ReadInt16(scroll_speed);
	ReadInt16(frame_speed);
	ReadInt8(help_mode);
	ReadInt8(disp_town_name);
	ReadInt8(disp_spy_sign);
	ReadInt8(show_all_unit_icon);
	ReadInt8(show_unit_path);
	ReadInt8(music_flag);
	ReadInt16(cd_music_volume);
	ReadInt16(wav_music_volume);
	ReadInt8(sound_effect_flag);
	ReadInt16(sound_effect_volume);
	ReadInt8(pan_control);
	ReadInt8(lightning_visual);
	ReadInt8(earthquake_visual);
	ReadInt8(rain_visual);
	ReadInt8(snow_visual);
	ReadInt8(snow_ground);
	ReadInt8(lightning_audio);
	ReadInt8(earthquake_audio);
	ReadInt8(rain_audio);
	ReadInt8(snow_audio);
	ReadInt8(wind_audio);
	ReadInt32(lightning_brightness);
	ReadInt32(cloud_darkness);
	ReadInt32(lightning_volume);
	ReadInt32(earthquake_volume);
	ReadInt32(rain_volume);
	ReadInt32(snow_volume);
	ReadInt32(wind_volume);
	ReadInt8(blacken_map);
	ReadInt8(explore_mask_method);
	ReadInt8(fog_mask_method);
}

void ChatInfo::write_record(ChatInfoGF *r)
{
	WriteInt32(received_date);
	WriteInt8(from_nation_recno);
	WriteInt8Array(chat_str, CHAT_STR_LEN+1);
}

void ChatInfo::read_record(ChatInfoGF *r)
{
	ReadInt32(received_date);
	ReadInt8(from_nation_recno);
	ReadInt8Array(chat_str, CHAT_STR_LEN+1);
}

void Info::write_record(InfoGF *r)
{
	WriteInt32(game_start_date);
	WriteInt32(game_date);
	WriteInt32(game_day);
	WriteInt32(game_month);
	WriteInt32(game_year);
	WriteInt32(goal_deadline);
	WriteInt16(goal_difficulty);
	WriteInt16(goal_score_bonus);
	WriteInt32(week_day);
	WriteInt32(year_day);
	WriteInt32(year_passed);
	WriteInt32(random_seed);
	WriteInt32(start_play_time);
	WriteInt32(total_play_time);
	WriteInt16(viewing_nation_recno);
	WriteInt16(viewing_spy_recno);
	WriteInt16(default_viewing_nation_recno);
	WriteInt16(browse_nation_recno);
	WriteInt16(browse_race_recno);
	WriteInt16(browse_firm_recno);
	WriteInt16(browse_income_recno);
	WriteInt16(browse_expense_recno);
	WriteInt16(browse_troop_recno);
	WriteInt16(browse_unit_recno);
	WriteInt16(browse_tech_recno);
	WriteInt16(browse_god_recno);
	WriteInt16(browse_town_recno);
	WriteInt16(browse_spy_recno);
	WriteInt16(browse_caravan_recno);
	WriteInt16(browse_ship_recno);
	WriteInt16(browse_talk_msg_recno);
	WriteInt16(browse_news_recno);
	WriteInt16(browse_ai_action_recno);
	WriteInt16(browse_ai_attack_recno);
	WriteInt8(nation_report_mode);
	WriteInt16(last_talk_nation_recno);
	WriteInt8(player_reply_mode);
	WriteInt8(chat_receiver_type);
	WriteInt8Array(player_chat_str, CHAT_STR_LEN+1);
	WriteCallArray(remote_chat_array, MAX_REMOTE_CHAT_STR);
}

void Info::read_record(InfoGF *r)
{
	ReadInt32(game_start_date);
	ReadInt32(game_date);
	ReadInt32(game_day);
	ReadInt32(game_month);
	ReadInt32(game_year);
	ReadInt32(goal_deadline);
	ReadInt16(goal_difficulty);
	ReadInt16(goal_score_bonus);
	ReadInt32(week_day);
	ReadInt32(year_day);
	ReadInt32(year_passed);
	ReadInt32(random_seed);
	ReadInt32(start_play_time);
	ReadInt32(total_play_time);
	ReadInt16(viewing_nation_recno);
	ReadInt16(viewing_spy_recno);
	ReadInt16(default_viewing_nation_recno);
	ReadInt16(browse_nation_recno);
	ReadInt16(browse_race_recno);
	ReadInt16(browse_firm_recno);
	ReadInt16(browse_income_recno);
	ReadInt16(browse_expense_recno);
	ReadInt16(browse_troop_recno);
	ReadInt16(browse_unit_recno);
	ReadInt16(browse_tech_recno);
	ReadInt16(browse_god_recno);
	ReadInt16(browse_town_recno);
	ReadInt16(browse_spy_recno);
	ReadInt16(browse_caravan_recno);
	ReadInt16(browse_ship_recno);
	ReadInt16(browse_talk_msg_recno);
	ReadInt16(browse_news_recno);
	ReadInt16(browse_ai_action_recno);
	ReadInt16(browse_ai_attack_recno);
	ReadInt8(nation_report_mode);
	ReadInt16(last_talk_nation_recno);
	ReadInt8(player_reply_mode);
	ReadInt8(chat_receiver_type);
	ReadInt8Array(player_chat_str, CHAT_STR_LEN+1);
	ReadCallArray(remote_chat_array, MAX_REMOTE_CHAT_STR);
}

void Power::write_record(PowerGF *r)
{
	WriteInt32(command_id);
	WriteInt32(command_unit_recno);
	WriteInt32(command_para);
	WriteInt8(win_opened);
	WriteInt8(enable_flag);
	WriteInt32Array(key_str_pos, MAX_KEY_STR);
}

void Power::read_record(PowerGF *r)
{
	ReadInt32(command_id);
	ReadInt32(command_unit_recno);
	ReadInt32(command_para);
	ReadInt8(win_opened);
	ReadInt8(enable_flag);
	ReadInt32Array(key_str_pos, MAX_KEY_STR);
}

void Weather::write_record(WeatherGF *r)
{
	WriteInt32(seed);
	WriteInt16(season_phase);
	WriteInt16(day_to_quake);
	WriteInt16(avg_temp);
	WriteInt16(temp_amp);
	WriteInt16(wind_spd);
	WriteInt32(high_wind_day);
	WriteInt16(wind_dir);
	WriteInt16(windy_speed);
	WriteInt16(tornado_count);
	WriteInt8(cur_cloud_str);
	WriteInt8(cur_cloud_len);
	WriteInt8(cur_cloud_type);
	WriteInt32(quake_frequency);
	WriteInt16(quake_x);
	WriteInt16(quake_y);
}

void Weather::read_record(WeatherGF *r)
{
	ReadInt32(seed);
	ReadInt16(season_phase);
	ReadInt16(day_to_quake);
	ReadInt16(avg_temp);
	ReadInt16(temp_amp);
	ReadInt16(wind_spd);
	ReadInt32(high_wind_day);
	ReadInt16(wind_dir);
	ReadInt16(windy_speed);
	ReadInt16(tornado_count);
	ReadInt8(cur_cloud_str);
	ReadInt8(cur_cloud_len);
	ReadInt8(cur_cloud_type);
	ReadInt32(quake_frequency);
	ReadInt16(quake_x);
	ReadInt16(quake_y);
}

void MagicWeather::write_record(MagicWeatherGF *r)
{
	WriteInt8(rain_str);
	WriteInt16(wind_spd);
	WriteInt16(wind_dir);
	WriteInt16(rain_day);
	WriteInt16(wind_day);
	WriteInt16(lightning_day);
}

void MagicWeather::read_record(MagicWeatherGF *r)
{
	ReadInt8(rain_str);
	ReadInt16(wind_spd);
	ReadInt16(wind_dir);
	ReadInt16(rain_day);
	ReadInt16(wind_day);
	ReadInt16(lightning_day);
}

void Location::write_record(LocationGF *r)
{
	WriteInt16(loc_flag);
	WriteInt16(terrain_id);
	WriteInt16(cargo_recno);
	WriteInt16(air_cargo_recno);
	WriteInt8(extra_para);
	WriteInt8(fire_level);
	WriteInt8(flammability);
	WriteInt8(power_nation_recno);
	WriteInt8(region_id);
	WriteInt8(visit_level);
}

void Location::read_record(LocationGF *r)
{
	ReadInt16(loc_flag);
	ReadInt16(terrain_id);
	ReadInt16(cargo_recno);
	ReadInt16(air_cargo_recno);
	ReadInt8(extra_para);
	ReadInt8(fire_level);
	ReadInt8(flammability);
	ReadInt8(power_nation_recno);
	ReadInt8(region_id);
	ReadInt8(visit_level);
}

void Sprite::write_record(SpriteGF *r)
{
	WriteZero(vtp); //zero

	WriteInt16(sprite_id);
	WriteInt16(sprite_recno);
	WriteInt8(mobile_type);
	WriteInt8(cur_action);
	WriteInt8(cur_dir);
	WriteInt8(cur_frame);
	WriteInt8(cur_attack);
	WriteInt8(final_dir);
	WriteInt8(turn_delay);
	WriteInt8(guard_count);
	WriteInt8(remain_attack_delay);
	WriteInt8(remain_frames_per_step);
	WriteInt16(cur_x);
	WriteInt16(cur_y);
	WriteInt16(go_x);
	WriteInt16(go_y);
	WriteInt16(next_x);
	WriteInt16(next_y);
	WriteZero(sprite_info);
}

void Sprite::read_record(SpriteGF *r)
{
	// skip vtp

	ReadInt16(sprite_id);
	ReadInt16(sprite_recno);
	ReadInt8(mobile_type);
	ReadInt8(cur_action);
	ReadInt8(cur_dir);
	ReadInt8(cur_frame);
	ReadInt8(cur_attack);
	ReadInt8(final_dir);
	ReadInt8(turn_delay);
	ReadInt8(guard_count);
	ReadInt8(remain_attack_delay);
	ReadInt8(remain_frames_per_step);
	ReadInt16(cur_x);
	ReadInt16(cur_y);
	ReadInt16(go_x);
	ReadInt16(go_y);
	ReadInt16(next_x);
	ReadInt16(next_y);
	// skip sprite_info
}

void AttackInfo::write_record(AttackInfoGF *r)
{
	WriteInt8(combat_level);
	WriteInt8(attack_delay);
	WriteInt8(attack_range);
	WriteInt8(attack_damage);
	WriteInt8(pierce_damage);
	WriteInt16(bullet_out_frame);
	WriteInt8(bullet_speed);
	WriteInt8(bullet_radius);
	WriteInt8(bullet_sprite_id);
	WriteInt8(dll_bullet_sprite_id);
	WriteInt8(eqv_attack_next);
	WriteInt16(min_power);
	WriteInt16(consume_power);
	WriteInt8(fire_radius);
	WriteInt16(effect_id);
}

void AttackInfo::read_record(AttackInfoGF *r)
{
	ReadInt8(combat_level);
	ReadInt8(attack_delay);
	ReadInt8(attack_range);
	ReadInt8(attack_damage);
	ReadInt8(pierce_damage);
	ReadInt16(bullet_out_frame);
	ReadInt8(bullet_speed);
	ReadInt8(bullet_radius);
	ReadInt8(bullet_sprite_id);
	ReadInt8(dll_bullet_sprite_id);
	ReadInt8(eqv_attack_next);
	ReadInt16(min_power);
	ReadInt16(consume_power);
	ReadInt8(fire_radius);
	ReadInt16(effect_id);
}

void ResultNode::write_record(ResultNodeGF *r)
{
	WriteInt16(node_x);
	WriteInt16(node_y);
}

void ResultNode::read_record(ResultNodeGF *r)
{
	ReadInt16(node_x);
	ReadInt16(node_y);
}

void Skill::write_record(SkillGF *r)
{
	WriteInt8(combat_level);
	WriteInt8(skill_id);
	WriteInt8(skill_level);
	WriteInt8(combat_level_minor);
	WriteInt8(skill_level_minor);
	WriteInt8(skill_potential);
}

void Skill::read_record(SkillGF *r)
{
	ReadInt8(combat_level);
	ReadInt8(skill_id);
	ReadInt8(skill_level);
	ReadInt8(combat_level_minor);
	ReadInt8(skill_level_minor);
	ReadInt8(skill_potential);
}

void TeamInfo::write_record(TeamInfoGF *r)
{
	WriteInt8(member_count);
	WriteInt16Array(member_unit_array, MAX_TEAM_MEMBER);
	WriteInt32(ai_last_request_defense_date);
}

void TeamInfo::read_record(TeamInfoGF *r)
{
	ReadInt8(member_count);
	ReadInt16Array(member_unit_array, MAX_TEAM_MEMBER);
	ReadInt32(ai_last_request_defense_date);
}

void Unit::write_record(UnitGF *r)
{
	WriteZero(vtp);

	// Sprite
	WriteInt16(sprite_id);
	WriteInt16(sprite_recno);
	WriteInt8(mobile_type);
	WriteInt8(cur_action);
	WriteInt8(cur_dir);
	WriteInt8(cur_frame);
	WriteInt8(cur_attack);
	WriteInt8(final_dir);
	WriteInt8(turn_delay);
	WriteInt8(guard_count);
	WriteInt8(remain_attack_delay);
	WriteInt8(remain_frames_per_step);
	WriteInt16(cur_x);
	WriteInt16(cur_y);
	WriteInt16(go_x);
	WriteInt16(go_y);
	WriteInt16(next_x);
	WriteInt16(next_y);
	WriteZero(sprite_info);

	// Unit
	WriteInt8(unit_id);
	WriteInt8(rank_id);
	WriteInt8(race_id);
	WriteInt8(nation_recno);
	WriteInt8(ai_unit);
	WriteInt16(name_id);
	WriteInt32(unit_group_id);
	WriteInt32(team_id);
	WriteInt8(selected_flag);
	WriteInt8(group_select_id);
	WriteInt8(waiting_term);
	WriteInt8(blocked_by_member);
	WriteInt8(swapping);
	WriteInt16(leader_unit_recno);
	WriteInt8(action_misc);
	WriteInt16(action_misc_para);
	WriteInt8(action_mode);
	WriteInt16(action_para);
	WriteInt16(action_x_loc);
	WriteInt16(action_y_loc);
	WriteInt8(action_mode2);
	WriteInt16(action_para2);
	WriteInt16(action_x_loc2);
	WriteInt16(action_y_loc2);
	WriteInt8Array(blocked_edge, 4);
	WriteInt8(attack_dir);
	WriteInt16(range_attack_x_loc);
	WriteInt16(range_attack_y_loc);
	WriteInt16(move_to_x_loc);
	WriteInt16(move_to_y_loc);
	WriteInt8(loyalty);
	WriteInt8(target_loyalty);
	WriteFloat(hit_points);
	WriteInt16(max_hit_points);
	WriteCall(skill);
	WriteInt8(unit_mode);
	WriteInt16(unit_mode_para);
	WriteInt16(spy_recno);
	WriteInt16(nation_contribution);
	WriteInt16(total_reward);
	WriteZero(attack_info_array);
	WriteInt8(attack_count);
	WriteInt8(attack_range);
	WriteInt16(cur_power);
	WriteInt16(max_power);
	r->has_result_node_array = result_node_array ? 0xdeadbeef : 0;
	WriteInt32(result_node_count);
	WriteInt16(result_node_recno);
	WriteInt16(result_path_dist);
	r->has_way_point_array = way_point_array ? 0xdeadbeef : 0;
	WriteInt16(way_point_array_size);
	WriteInt16(way_point_count);
	WriteInt16(ai_action_id);
	WriteInt8(original_action_mode);
	WriteInt16(original_action_para);
	WriteInt16(original_action_x_loc);
	WriteInt16(original_action_y_loc);
	WriteInt16(original_target_x_loc);
	WriteInt16(original_target_y_loc);
	WriteInt16(ai_original_target_x_loc);
	WriteInt16(ai_original_target_y_loc);
	WriteInt8(ai_no_suitable_action);
	WriteInt8(can_guard_flag);
	WriteInt8(can_attack_flag);
	WriteInt8(force_move_flag);
	WriteInt16(home_camp_firm_recno);
	WriteInt8(aggressive_mode);
	WriteInt8(seek_path_fail_count);
	WriteInt8(ignore_power_nation);
	r->has_team_info = team_info ? 0xdeadbeef : 0;
}

void Unit::read_record(UnitGF *r)
{
	// skip vtp

	// Sprite
	ReadInt16(sprite_id);
	ReadInt16(sprite_recno);
	ReadInt8(mobile_type);
	ReadInt8(cur_action);
	ReadInt8(cur_dir);
	ReadInt8(cur_frame);
	ReadInt8(cur_attack);
	ReadInt8(final_dir);
	ReadInt8(turn_delay);
	ReadInt8(guard_count);
	ReadInt8(remain_attack_delay);
	ReadInt8(remain_frames_per_step);
	ReadInt16(cur_x);
	ReadInt16(cur_y);
	ReadInt16(go_x);
	ReadInt16(go_y);
	ReadInt16(next_x);
	ReadInt16(next_y);
	// skip sprite_info

	// Unit
	ReadInt8(unit_id);
	ReadInt8(rank_id);
	ReadInt8(race_id);
	ReadInt8(nation_recno);
	ReadInt8(ai_unit);
	ReadInt16(name_id);
	ReadInt32(unit_group_id);
	ReadInt32(team_id);
	ReadInt8(selected_flag);
	ReadInt8(group_select_id);
	ReadInt8(waiting_term);
	ReadInt8(blocked_by_member);
	ReadInt8(swapping);
	ReadInt16(leader_unit_recno);
	ReadInt8(action_misc);
	ReadInt16(action_misc_para);
	ReadInt8(action_mode);
	ReadInt16(action_para);
	ReadInt16(action_x_loc);
	ReadInt16(action_y_loc);
	ReadInt8(action_mode2);
	ReadInt16(action_para2);
	ReadInt16(action_x_loc2);
	ReadInt16(action_y_loc2);
	ReadInt8Array(blocked_edge, 4);
	ReadInt8(attack_dir);
	ReadInt16(range_attack_x_loc);
	ReadInt16(range_attack_y_loc);
	ReadInt16(move_to_x_loc);
	ReadInt16(move_to_y_loc);
	ReadInt8(loyalty);
	ReadInt8(target_loyalty);
	ReadFloat(hit_points);
	ReadInt16(max_hit_points);
	ReadCall(skill);
	ReadInt8(unit_mode);
	ReadInt16(unit_mode_para);
	ReadInt16(spy_recno);
	ReadInt16(nation_contribution);
	ReadInt16(total_reward);
	// skip attack_info_array
	ReadInt8(attack_count);
	ReadInt8(attack_range);
	ReadInt16(cur_power);
	ReadInt16(max_power);
	result_node_array = r->has_result_node_array ? (ResultNode*)0xdeadbeef : 0;
	ReadInt32(result_node_count);
	ReadInt16(result_node_recno);
	ReadInt16(result_path_dist);
	way_point_array = r->has_way_point_array ? (ResultNode*)0xdeadbeef : 0;
	ReadInt16(way_point_array_size);
	ReadInt16(way_point_count);
	ReadInt16(ai_action_id);
	ReadInt8(original_action_mode);
	ReadInt16(original_action_para);
	ReadInt16(original_action_x_loc);
	ReadInt16(original_action_y_loc);
	ReadInt16(original_target_x_loc);
	ReadInt16(original_target_y_loc);
	ReadInt16(ai_original_target_x_loc);
	ReadInt16(ai_original_target_y_loc);
	ReadInt8(ai_no_suitable_action);
	ReadInt8(can_guard_flag);
	ReadInt8(can_attack_flag);
	ReadInt8(force_move_flag);
	ReadInt16(home_camp_firm_recno);
	ReadInt8(aggressive_mode);
	ReadInt8(seek_path_fail_count);
	ReadInt8(ignore_power_nation);
	team_info = r->has_team_info ? (TeamInfo*)0xdeadbeef : 0;
}

void CaravanStop::write_record(CaravanStopGF *r)
{
	// TradeStop
	WriteInt16(firm_recno);
	WriteInt16(firm_loc_x1);
	WriteInt16(firm_loc_y1);
	WriteInt8(pick_up_type);
	WriteInt8Array(pick_up_array, MAX_PICK_UP_GOODS);

	// CaravanStop
	WriteInt8(firm_id);
}

void CaravanStop::read_record(CaravanStopGF *r)
{
	// TradeStop
	ReadInt16(firm_recno);
	ReadInt16(firm_loc_x1);
	ReadInt16(firm_loc_y1);
	ReadInt8(pick_up_type);
	ReadInt8Array(pick_up_array, MAX_PICK_UP_GOODS);

	// CaravanStop
	ReadInt8(firm_id);
}

void UnitCaravan::write_derived_record(UnitCaravanGF *r)
{
	WriteInt16(caravan_id);
	WriteInt8(journey_status);
	WriteInt8(dest_stop_id);
	WriteInt8(stop_defined_num);
	WriteInt8(wait_count);
	WriteInt16(stop_x_loc);
	WriteInt16(stop_y_loc);
	WriteCallArray(stop_array, MAX_STOP_FOR_CARAVAN);
	WriteInt32(last_set_stop_date);
	WriteInt32(last_load_goods_date);
	WriteInt16Array(raw_qty_array, MAX_RAW);
	WriteInt16Array(product_raw_qty_array, MAX_PRODUCT);
}

void UnitCaravan::read_derived_record(UnitCaravanGF *r)
{
	ReadInt16(caravan_id);
	ReadInt8(journey_status);
	ReadInt8(dest_stop_id);
	ReadInt8(stop_defined_num);
	ReadInt8(wait_count);
	ReadInt16(stop_x_loc);
	ReadInt16(stop_y_loc);
	ReadCallArray(stop_array, MAX_STOP_FOR_CARAVAN);
	ReadInt32(last_set_stop_date);
	ReadInt32(last_load_goods_date);
	ReadInt16Array(raw_qty_array, MAX_RAW);
	ReadInt16Array(product_raw_qty_array, MAX_PRODUCT);
}

void UnitExpCart::write_derived_record(UnitExpCartGF *r)
{
	WriteInt8(triggered);
}

void UnitExpCart::read_derived_record(UnitExpCartGF *r)
{
	ReadInt8(triggered);
}

void UnitGod::write_derived_record(UnitGodGF *r)
{
	WriteInt16(god_id);
	WriteInt16(base_firm_recno);
	WriteInt8(cast_power_type);
	WriteInt16(cast_origin_x);
	WriteInt16(cast_origin_y);
	WriteInt16(cast_target_x);
	WriteInt16(cast_target_y);
}

void UnitGod::read_derived_record(UnitGodGF *r)
{
	ReadInt16(god_id);
	ReadInt16(base_firm_recno);
	ReadInt8(cast_power_type);
	ReadInt16(cast_origin_x);
	ReadInt16(cast_origin_y);
	ReadInt16(cast_target_x);
	ReadInt16(cast_target_y);
}

void ShipStop::write_record(ShipStopGF *r)
{
	// TradeStop
	WriteInt16(firm_recno);
	WriteInt16(firm_loc_x1);
	WriteInt16(firm_loc_y1);
	WriteInt8(pick_up_type);
	WriteInt8Array(pick_up_array, MAX_PICK_UP_GOODS);
}

void ShipStop::read_record(ShipStopGF *r)
{
	// TradeStop
	ReadInt16(firm_recno);
	ReadInt16(firm_loc_x1);
	ReadInt16(firm_loc_y1);
	ReadInt8(pick_up_type);
	ReadInt8Array(pick_up_array, MAX_PICK_UP_GOODS);
}

void UnitMarine::write_derived_record(UnitMarineGF *r)
{
	WriteCall(splash);
	WriteInt8(menu_mode);
	WriteInt8(extra_move_in_beach);
	WriteInt8(in_beach);
	WriteInt8(selected_unit_id);
	WriteInt16Array(unit_recno_array, MAX_UNIT_IN_SHIP);
	WriteInt8(unit_count);
	WriteInt8(journey_status);
	WriteInt8(dest_stop_id);
	WriteInt8(stop_defined_num);
	WriteInt8(wait_count);
	WriteInt16(stop_x_loc);
	WriteInt16(stop_y_loc);
	WriteInt8(auto_mode);
	WriteInt16(cur_firm_recno);
	WriteInt16(carry_goods_capacity);
	WriteCallArray(stop_array, MAX_STOP_FOR_SHIP);
	WriteInt16Array(raw_qty_array, MAX_RAW);
	WriteInt16Array(product_raw_qty_array, MAX_PRODUCT);
	WriteCall(ship_attack_info);
	WriteInt8(attack_mode_selected);
	WriteInt32(last_load_goods_date);
}

void UnitMarine::read_derived_record(UnitMarineGF *r)
{
	ReadCall(splash);
	ReadInt8(menu_mode);
	ReadInt8(extra_move_in_beach);
	ReadInt8(in_beach);
	ReadInt8(selected_unit_id);
	ReadInt16Array(unit_recno_array, MAX_UNIT_IN_SHIP);
	ReadInt8(unit_count);
	ReadInt8(journey_status);
	ReadInt8(dest_stop_id);
	ReadInt8(stop_defined_num);
	ReadInt8(wait_count);
	ReadInt16(stop_x_loc);
	ReadInt16(stop_y_loc);
	ReadInt8(auto_mode);
	ReadInt16(cur_firm_recno);
	ReadInt16(carry_goods_capacity);
	ReadCallArray(stop_array, MAX_STOP_FOR_SHIP);
	ReadInt16Array(raw_qty_array, MAX_RAW);
	ReadInt16Array(product_raw_qty_array, MAX_PRODUCT);
	ReadCall(ship_attack_info);
	ReadInt8(attack_mode_selected);
	ReadInt32(last_load_goods_date);
}

void UnitMonster::write_derived_record(UnitMonsterGF *r)
{
	WriteInt8(monster_action_mode);
}

void UnitMonster::read_derived_record(UnitMonsterGF *r)
{
	ReadInt8(monster_action_mode);
}

void UnitVehicle::write_derived_record(UnitVehicleGF *r)
{
	WriteInt16(solider_hit_points);
	WriteInt16(vehicle_hit_points);
}

void UnitVehicle::read_derived_record(UnitVehicleGF *r)
{
	ReadInt16(solider_hit_points);
	ReadInt16(vehicle_hit_points);
}

void AIRegion::write_record(AIRegionGF *r)
{
	WriteInt8(region_id);
	WriteInt8(town_count);
	WriteInt8(base_town_count);
}

void AIRegion::read_record(AIRegionGF *r)
{
	ReadInt8(region_id);
	ReadInt8(town_count);
	ReadInt8(base_town_count);
}

void ActionNode::write_record(ActionNodeGF *r)
{
	WriteInt8(action_mode);
	WriteInt8(action_type);
	WriteInt16(action_para);
	WriteInt16(action_para2);
	WriteInt16(action_id);
	WriteInt32(add_date);
	WriteInt16(unit_recno);
	WriteInt16(action_x_loc);
	WriteInt16(action_y_loc);
	WriteInt16(ref_x_loc);
	WriteInt16(ref_y_loc);
	WriteInt8(retry_count);
	WriteInt8(instance_count);
	WriteInt16Array(group_unit_array, MAX_ACTION_GROUP_UNIT);
	WriteInt8(processing_instance_count);
	WriteInt8(processed_instance_count);
	WriteInt32(next_retry_date);
}

void ActionNode::read_record(ActionNodeGF *r)
{
	ReadInt8(action_mode);
	ReadInt8(action_type);
	ReadInt16(action_para);
	ReadInt16(action_para2);
	ReadInt16(action_id);
	ReadInt32(add_date);
	ReadInt16(unit_recno);
	ReadInt16(action_x_loc);
	ReadInt16(action_y_loc);
	ReadInt16(ref_x_loc);
	ReadInt16(ref_y_loc);
	ReadInt8(retry_count);
	ReadInt8(instance_count);
	ReadInt16Array(group_unit_array, MAX_ACTION_GROUP_UNIT);
	ReadInt8(processing_instance_count);
	ReadInt8(processed_instance_count);
	ReadInt32(next_retry_date);
}

void AttackCamp::write_record(AttackCampGF *r)
{
	WriteInt16(firm_recno);
	WriteInt16(combat_level);
	WriteInt16(distance);
	WriteInt32(patrol_date);
}

void AttackCamp::read_record(AttackCampGF *r)
{
	ReadInt16(firm_recno);
	ReadInt16(combat_level);
	ReadInt16(distance);
	ReadInt32(patrol_date);
}

void NationRelation::write_record(NationRelationGF *r)
{
	WriteInt8(has_contact);
	WriteInt8(should_attack);
	WriteInt8(trade_treaty);
	WriteInt8(status);
	WriteInt32(last_change_status_date);
	WriteInt8(ai_relation_level);
	WriteInt8(ai_secret_attack);
	WriteInt8(ai_demand_trade_treaty);
	WriteFloat(good_relation_duration_rating);
	WriteInt16(started_war_on_us_count);
	WriteFloatArray(cur_year_import, IMPORT_TYPE_COUNT);
	WriteFloatArray(last_year_import, IMPORT_TYPE_COUNT);
	WriteFloatArray(lifetime_import, IMPORT_TYPE_COUNT);
	WriteInt32Array(last_talk_reject_date_array, MAX_TALK_TYPE);
	WriteInt32(last_military_aid_date);
	WriteInt32(last_give_gift_date);
	WriteInt16(total_given_gift_amount);
	WriteInt8(contact_msg_flag);
}

void NationRelation::read_record(NationRelationGF *r)
{
	ReadInt8(has_contact);
	ReadInt8(should_attack);
	ReadInt8(trade_treaty);
	ReadInt8(status);
	ReadInt32(last_change_status_date);
	ReadInt8(ai_relation_level);
	ReadInt8(ai_secret_attack);
	ReadInt8(ai_demand_trade_treaty);
	ReadFloat(good_relation_duration_rating);
	ReadInt16(started_war_on_us_count);
	ReadFloatArray(cur_year_import, IMPORT_TYPE_COUNT);
	ReadFloatArray(last_year_import, IMPORT_TYPE_COUNT);
	ReadFloatArray(lifetime_import, IMPORT_TYPE_COUNT);
	ReadInt32Array(last_talk_reject_date_array, MAX_TALK_TYPE);
	ReadInt32(last_military_aid_date);
	ReadInt32(last_give_gift_date);
	ReadInt16(total_given_gift_amount);
	ReadInt8(contact_msg_flag);
}

void Nation::write_record(NationGF *r)
{
	WriteZero(vtp);

	// NationBase
	WriteInt16(nation_recno);
	WriteInt8(nation_type);
	WriteInt8(race_id);
	WriteInt8(color_scheme_id);
	WriteInt8(nation_color);
	WriteInt16(king_unit_recno);
	WriteInt8(king_leadership);
	WriteInt32(nation_name_id);
	WriteInt8Array(nation_name_str, NATION_NAME_LEN+1);
	WriteInt32(player_id);
	WriteInt8(next_frame_ready);
	WriteInt16(last_caravan_id);
	WriteInt16(nation_firm_count);
	WriteInt32(last_build_firm_date);
	WriteInt8Array(know_base_array, MAX_RACE);
	WriteInt8Array(base_count_array, MAX_RACE);
	WriteInt8(is_at_war_today);
	WriteInt8(is_at_war_yesterday);
	WriteInt32(last_war_date);
	WriteInt16(last_attacker_unit_recno);
	WriteInt32(last_independent_unit_join_date);
	WriteInt8(cheat_enabled_flag);
	WriteFloat(cash);
	WriteFloat(food);
	WriteFloat(reputation);
	WriteFloat(kill_monster_score);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteFloat(cur_year_profit);
	WriteFloat(last_year_profit);
	WriteFloat(cur_year_fixed_income);
	WriteFloat(last_year_fixed_income);
	WriteFloat(cur_year_fixed_expense);
	WriteFloat(last_year_fixed_expense);
	WriteFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	WriteFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	WriteFloat(cur_year_income);
	WriteFloat(last_year_income);
	WriteFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloat(cur_year_expense);
	WriteFloat(last_year_expense);
	WriteFloat(cur_year_cheat);
	WriteFloat(last_year_cheat);
	WriteFloat(cur_year_food_in);
	WriteFloat(last_year_food_in);
	WriteFloat(cur_year_food_out);
	WriteFloat(last_year_food_out);
	WriteFloat(cur_year_food_change);
	WriteFloat(last_year_food_change);
	WriteFloat(cur_year_reputation_change);
	WriteFloat(last_year_reputation_change);
	WriteCallArray(relation_array, MAX_NATION);
	WriteInt8Array(relation_status_array, MAX_NATION);
	WriteInt8Array(relation_passable_array, MAX_NATION);
	WriteInt8Array(relation_should_attack_array, MAX_NATION);
	WriteInt8(is_allied_with_player);
	WriteInt32(total_population);
	WriteInt32(total_jobless_population);
	WriteInt32(total_unit_count);
	WriteInt32(total_human_count);
	WriteInt32(total_general_count);
	WriteInt32(total_weapon_count);
	WriteInt32(total_ship_count);
	WriteInt32(total_firm_count);
	WriteInt32(total_spy_count);
	WriteInt32(total_ship_combat_level);
	WriteInt16(largest_town_recno);
	WriteInt16(largest_town_pop);
	WriteInt16Array(raw_count_array, MAX_RAW);
	WriteInt16Array(last_unit_name_id_array, MAX_UNIT_TYPE);
	WriteInt32(population_rating);
	WriteInt32(military_rating);
	WriteInt32(economic_rating);
	WriteInt32(overall_rating);
	WriteInt32(enemy_soldier_killed);
	WriteInt32(own_soldier_killed);
	WriteInt32(enemy_civilian_killed);
	WriteInt32(own_civilian_killed);
	WriteInt32(enemy_weapon_destroyed);
	WriteInt32(own_weapon_destroyed);
	WriteInt32(enemy_ship_destroyed);
	WriteInt32(own_ship_destroyed);
	WriteInt32(enemy_firm_destroyed);
	WriteInt32(own_firm_destroyed);

	// Nation
	WriteZeroBytes(action_array, sizeof(DynArrayGF));
	WriteInt16(last_action_id);
	WriteZero(ai_town_array);
	WriteZero(ai_base_array);
	WriteZero(ai_mine_array);
	WriteZero(ai_factory_array);
	WriteZero(ai_camp_array);
	WriteZero(ai_research_array);
	WriteZero(ai_war_array);
	WriteZero(ai_harbor_array);
	WriteZero(ai_market_array);
	WriteZero(ai_inn_array);
	WriteZero(ai_general_array);
	WriteZero(ai_caravan_array);
	WriteZero(ai_ship_array);
	WriteInt16(ai_town_size);
	WriteInt16(ai_base_size);
	WriteInt16(ai_mine_size);
	WriteInt16(ai_factory_size);
	WriteInt16(ai_camp_size);
	WriteInt16(ai_research_size);
	WriteInt16(ai_war_size);
	WriteInt16(ai_harbor_size);
	WriteInt16(ai_market_size);
	WriteInt16(ai_inn_size);
	WriteInt16(ai_general_size);
	WriteInt16(ai_caravan_size);
	WriteInt16(ai_ship_size);
	WriteInt16(ai_town_count);
	WriteInt16(ai_base_count);
	WriteInt16(ai_mine_count);
	WriteInt16(ai_factory_count);
	WriteInt16(ai_camp_count);
	WriteInt16(ai_research_count);
	WriteInt16(ai_war_count);
	WriteInt16(ai_harbor_count);
	WriteInt16(ai_market_count);
	WriteInt16(ai_inn_count);
	WriteInt16(ai_general_count);
	WriteInt16(ai_caravan_count);
	WriteInt16(ai_ship_count);
	WriteInt16(ai_base_town_count);
	WriteInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	WriteCallArray(ai_region_array, MAX_AI_REGION);
	WriteInt8(ai_region_count);
	WriteInt8(pref_force_projection);
	WriteInt8(pref_military_development);
	WriteInt8(pref_economic_development);
	WriteInt8(pref_inc_pop_by_capture);
	WriteInt8(pref_inc_pop_by_growth);
	WriteInt8(pref_peacefulness);
	WriteInt8(pref_military_courage);
	WriteInt8(pref_territorial_cohesiveness);
	WriteInt8(pref_trading_tendency);
	WriteInt8(pref_allying_tendency);
	WriteInt8(pref_honesty);
	WriteInt8(pref_town_harmony);
	WriteInt8(pref_loyalty_concern);
	WriteInt8(pref_forgiveness);
	WriteInt8(pref_collect_tax);
	WriteInt8(pref_hire_unit);
	WriteInt8(pref_use_weapon);
	WriteInt8(pref_keep_general);
	WriteInt8(pref_keep_skilled_unit);
	WriteInt8(pref_diplomacy_retry);
	WriteInt8(pref_attack_monster);
	WriteInt8(pref_spy);
	WriteInt8(pref_counter_spy);
	WriteInt8(pref_food_reserve);
	WriteInt8(pref_cash_reserve);
	WriteInt8(pref_use_marine);
	WriteInt8(pref_unit_chase_distance);
	WriteInt8(pref_repair_concern);
	WriteInt8(pref_scout);
	WriteInt16(ai_capture_enemy_town_recno);
	WriteInt32(ai_capture_enemy_town_plan_date);
	WriteInt32(ai_capture_enemy_town_start_attack_date);
	WriteInt8(ai_capture_enemy_town_use_all_camp);
	WriteInt32(ai_last_defend_action_date);
	WriteInt16(ai_attack_target_x_loc);
	WriteInt16(ai_attack_target_y_loc);
	WriteInt16(ai_attack_target_nation_recno);
	WriteCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	WriteInt16(attack_camp_count);
	WriteInt16(lead_attack_camp_recno);
}

void Nation::read_record(NationGF *r)
{
	// skip vtp

	// NationBase
	ReadInt16(nation_recno);
	ReadInt8(nation_type);
	ReadInt8(race_id);
	ReadInt8(color_scheme_id);
	ReadInt8(nation_color);
	ReadInt16(king_unit_recno);
	ReadInt8(king_leadership);
	ReadInt32(nation_name_id);
	ReadInt8Array(nation_name_str, NATION_NAME_LEN+1);
	ReadInt32(player_id);
	ReadInt8(next_frame_ready);
	ReadInt16(last_caravan_id);
	ReadInt16(nation_firm_count);
	ReadInt32(last_build_firm_date);
	ReadInt8Array(know_base_array, MAX_RACE);
	ReadInt8Array(base_count_array, MAX_RACE);
	ReadInt8(is_at_war_today);
	ReadInt8(is_at_war_yesterday);
	ReadInt32(last_war_date);
	ReadInt16(last_attacker_unit_recno);
	ReadInt32(last_independent_unit_join_date);
	ReadInt8(cheat_enabled_flag);
	ReadFloat(cash);
	ReadFloat(food);
	ReadFloat(reputation);
	ReadFloat(kill_monster_score);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadFloat(cur_year_profit);
	ReadFloat(last_year_profit);
	ReadFloat(cur_year_fixed_income);
	ReadFloat(last_year_fixed_income);
	ReadFloat(cur_year_fixed_expense);
	ReadFloat(last_year_fixed_expense);
	ReadFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	ReadFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	ReadFloat(cur_year_income);
	ReadFloat(last_year_income);
	ReadFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloat(cur_year_expense);
	ReadFloat(last_year_expense);
	ReadFloat(cur_year_cheat);
	ReadFloat(last_year_cheat);
	ReadFloat(cur_year_food_in);
	ReadFloat(last_year_food_in);
	ReadFloat(cur_year_food_out);
	ReadFloat(last_year_food_out);
	ReadFloat(cur_year_food_change);
	ReadFloat(last_year_food_change);
	ReadFloat(cur_year_reputation_change);
	ReadFloat(last_year_reputation_change);
	ReadCallArray(relation_array, MAX_NATION);
	ReadInt8Array(relation_status_array, MAX_NATION);
	ReadInt8Array(relation_passable_array, MAX_NATION);
	ReadInt8Array(relation_should_attack_array, MAX_NATION);
	ReadInt8(is_allied_with_player);
	ReadInt32(total_population);
	ReadInt32(total_jobless_population);
	ReadInt32(total_unit_count);
	ReadInt32(total_human_count);
	ReadInt32(total_general_count);
	ReadInt32(total_weapon_count);
	ReadInt32(total_ship_count);
	ReadInt32(total_firm_count);
	ReadInt32(total_spy_count);
	ReadInt32(total_ship_combat_level);
	ReadInt16(largest_town_recno);
	ReadInt16(largest_town_pop);
	ReadInt16Array(raw_count_array, MAX_RAW);
	ReadInt16Array(last_unit_name_id_array, MAX_UNIT_TYPE);
	ReadInt32(population_rating);
	ReadInt32(military_rating);
	ReadInt32(economic_rating);
	ReadInt32(overall_rating);
	ReadInt32(enemy_soldier_killed);
	ReadInt32(own_soldier_killed);
	ReadInt32(enemy_civilian_killed);
	ReadInt32(own_civilian_killed);
	ReadInt32(enemy_weapon_destroyed);
	ReadInt32(own_weapon_destroyed);
	ReadInt32(enemy_ship_destroyed);
	ReadInt32(own_ship_destroyed);
	ReadInt32(enemy_firm_destroyed);
	ReadInt32(own_firm_destroyed);

	// Nation
	// skip action_array
	ReadInt16(last_action_id);
	// skip ai_town_array
	// skip ai_base_array
	// skip ai_mine_array
	// skip ai_factory_array
	// skip ai_camp_array
	// skip ai_research_array
	// skip ai_war_array
	// skip ai_harbor_array
	// skip ai_market_array
	// skip ai_inn_array
	// skip ai_general_array
	// skip ai_caravan_array
	// skip ai_ship_array
	ReadInt16(ai_town_size);
	ReadInt16(ai_base_size);
	ReadInt16(ai_mine_size);
	ReadInt16(ai_factory_size);
	ReadInt16(ai_camp_size);
	ReadInt16(ai_research_size);
	ReadInt16(ai_war_size);
	ReadInt16(ai_harbor_size);
	ReadInt16(ai_market_size);
	ReadInt16(ai_inn_size);
	ReadInt16(ai_general_size);
	ReadInt16(ai_caravan_size);
	ReadInt16(ai_ship_size);
	ReadInt16(ai_town_count);
	ReadInt16(ai_base_count);
	ReadInt16(ai_mine_count);
	ReadInt16(ai_factory_count);
	ReadInt16(ai_camp_count);
	ReadInt16(ai_research_count);
	ReadInt16(ai_war_count);
	ReadInt16(ai_harbor_count);
	ReadInt16(ai_market_count);
	ReadInt16(ai_inn_count);
	ReadInt16(ai_general_count);
	ReadInt16(ai_caravan_count);
	ReadInt16(ai_ship_count);
	ReadInt16(ai_base_town_count);
	ReadInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	ReadCallArray(ai_region_array, MAX_AI_REGION);
	ReadInt8(ai_region_count);
	ReadInt8(pref_force_projection);
	ReadInt8(pref_military_development);
	ReadInt8(pref_economic_development);
	ReadInt8(pref_inc_pop_by_capture);
	ReadInt8(pref_inc_pop_by_growth);
	ReadInt8(pref_peacefulness);
	ReadInt8(pref_military_courage);
	ReadInt8(pref_territorial_cohesiveness);
	ReadInt8(pref_trading_tendency);
	ReadInt8(pref_allying_tendency);
	ReadInt8(pref_honesty);
	ReadInt8(pref_town_harmony);
	ReadInt8(pref_loyalty_concern);
	ReadInt8(pref_forgiveness);
	ReadInt8(pref_collect_tax);
	ReadInt8(pref_hire_unit);
	ReadInt8(pref_use_weapon);
	ReadInt8(pref_keep_general);
	ReadInt8(pref_keep_skilled_unit);
	ReadInt8(pref_diplomacy_retry);
	ReadInt8(pref_attack_monster);
	ReadInt8(pref_spy);
	ReadInt8(pref_counter_spy);
	ReadInt8(pref_food_reserve);
	ReadInt8(pref_cash_reserve);
	ReadInt8(pref_use_marine);
	ReadInt8(pref_unit_chase_distance);
	ReadInt8(pref_repair_concern);
	ReadInt8(pref_scout);
	ReadInt16(ai_capture_enemy_town_recno);
	ReadInt32(ai_capture_enemy_town_plan_date);
	ReadInt32(ai_capture_enemy_town_start_attack_date);
	ReadInt8(ai_capture_enemy_town_use_all_camp);
	ReadInt32(ai_last_defend_action_date);
	ReadInt16(ai_attack_target_x_loc);
	ReadInt16(ai_attack_target_y_loc);
	ReadInt16(ai_attack_target_nation_recno);
	ReadCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	ReadInt16(attack_camp_count);
	ReadInt16(lead_attack_camp_recno);
}

void NationArray::write_record(NationArrayGF *r)
{
	WriteInt16(nation_count);
	WriteInt16(ai_nation_count);
	WriteInt32(last_del_nation_date);
	WriteInt32(last_new_nation_date);
	WriteInt32(max_nation_population);
	WriteInt32(all_nation_population);
	WriteInt16(independent_town_count);
	WriteInt16Array(independent_town_count_race_array, MAX_RACE);
	WriteInt32(max_nation_units);
	WriteInt32(max_nation_humans);
	WriteInt32(max_nation_generals);
	WriteInt32(max_nation_weapons);
	WriteInt32(max_nation_ships);
	WriteInt32(max_nation_spies);
	WriteInt32(max_nation_firms);
	WriteInt32(max_nation_tech_level);
	WriteInt32(max_population_rating);
	WriteInt32(max_military_rating);
	WriteInt32(max_economic_rating);
	WriteInt32(max_reputation);
	WriteInt32(max_kill_monster_score);
	WriteInt32(max_overall_rating);
	WriteInt16(max_population_nation_recno);
	WriteInt16(max_military_nation_recno);
	WriteInt16(max_economic_nation_recno);
	WriteInt16(max_reputation_nation_recno);
	WriteInt16(max_kill_monster_nation_recno);
	WriteInt16(max_overall_nation_recno);
	WriteInt32(last_alliance_id);
	WriteInt32(nation_peace_days);
	WriteInt16(player_recno);
	WriteZero(player_ptr);
	WriteInt8Array(nation_color_array, MAX_NATION+1);
	WriteInt8Array(nation_power_color_array, MAX_NATION+2);
	WriteInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void NationArray::read_record(NationArrayGF *r)
{
	ReadInt16(nation_count);
	ReadInt16(ai_nation_count);
	ReadInt32(last_del_nation_date);
	ReadInt32(last_new_nation_date);
	ReadInt32(max_nation_population);
	ReadInt32(all_nation_population);
	ReadInt16(independent_town_count);
	ReadInt16Array(independent_town_count_race_array, MAX_RACE);
	ReadInt32(max_nation_units);
	ReadInt32(max_nation_humans);
	ReadInt32(max_nation_generals);
	ReadInt32(max_nation_weapons);
	ReadInt32(max_nation_ships);
	ReadInt32(max_nation_spies);
	ReadInt32(max_nation_firms);
	ReadInt32(max_nation_tech_level);
	ReadInt32(max_population_rating);
	ReadInt32(max_military_rating);
	ReadInt32(max_economic_rating);
	ReadInt32(max_reputation);
	ReadInt32(max_kill_monster_score);
	ReadInt32(max_overall_rating);
	ReadInt16(max_population_nation_recno);
	ReadInt16(max_military_nation_recno);
	ReadInt16(max_economic_nation_recno);
	ReadInt16(max_reputation_nation_recno);
	ReadInt16(max_kill_monster_nation_recno);
	ReadInt16(max_overall_nation_recno);
	ReadInt32(last_alliance_id);
	ReadInt32(nation_peace_days);
	ReadInt16(player_recno);
	// skip player_ptr
	ReadInt8Array(nation_color_array, MAX_NATION+1);
	ReadInt8Array(nation_power_color_array, MAX_NATION+2);
	ReadInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void Version_1_Nation::write_record(Version_1_NationGF *r)
{
	WriteZero(vtp);

	// Version_1_NationBase
	WriteInt16(nation_recno);
	WriteInt8(nation_type);
	WriteInt8(race_id);
	WriteInt8(color_scheme_id);
	WriteInt8(nation_color);
	WriteInt16(king_unit_recno);
	WriteInt8(king_leadership);
	WriteInt32(nation_name_id);
	WriteInt8Array(nation_name_str, NATION_NAME_LEN+1);
	WriteInt32(player_id);
	WriteInt8(next_frame_ready);
	WriteInt16(last_caravan_id);
	WriteInt16(nation_firm_count);
	WriteInt32(last_build_firm_date);
	WriteInt8Array(know_base_array, VERSION_1_MAX_RACE);
	WriteInt8Array(base_count_array, VERSION_1_MAX_RACE);
	WriteInt8(is_at_war_today);
	WriteInt8(is_at_war_yesterday);
	WriteInt32(last_war_date);
	WriteInt16(last_attacker_unit_recno);
	WriteInt32(last_independent_unit_join_date);
	WriteInt8(cheat_enabled_flag);
	WriteFloat(cash);
	WriteFloat(food);
	WriteFloat(reputation);
	WriteFloat(kill_monster_score);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteFloat(cur_year_profit);
	WriteFloat(last_year_profit);
	WriteFloat(cur_year_fixed_income);
	WriteFloat(last_year_fixed_income);
	WriteFloat(cur_year_fixed_expense);
	WriteFloat(last_year_fixed_expense);
	WriteFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	WriteFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	WriteFloat(cur_year_income);
	WriteFloat(last_year_income);
	WriteFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloat(cur_year_expense);
	WriteFloat(last_year_expense);
	WriteFloat(cur_year_cheat);
	WriteFloat(last_year_cheat);
	WriteFloat(cur_year_food_in);
	WriteFloat(last_year_food_in);
	WriteFloat(cur_year_food_out);
	WriteFloat(last_year_food_out);
	WriteFloat(cur_year_food_change);
	WriteFloat(last_year_food_change);
	WriteFloat(cur_year_reputation_change);
	WriteFloat(last_year_reputation_change);
	WriteCallArray(relation_array, MAX_NATION);
	WriteInt8Array(relation_status_array, MAX_NATION);
	WriteInt8Array(relation_passable_array, MAX_NATION);
	WriteInt8Array(relation_should_attack_array, MAX_NATION);
	WriteInt8(is_allied_with_player);
	WriteInt32(total_population);
	WriteInt32(total_jobless_population);
	WriteInt32(total_unit_count);
	WriteInt32(total_human_count);
	WriteInt32(total_general_count);
	WriteInt32(total_weapon_count);
	WriteInt32(total_ship_count);
	WriteInt32(total_firm_count);
	WriteInt32(total_spy_count);
	WriteInt32(total_ship_combat_level);
	WriteInt16(largest_town_recno);
	WriteInt16(largest_town_pop);
	WriteInt16Array(raw_count_array, MAX_RAW);
	WriteInt16Array(last_unit_name_id_array, VERSION_1_MAX_UNIT_TYPE);
	WriteInt32(population_rating);
	WriteInt32(military_rating);
	WriteInt32(economic_rating);
	WriteInt32(overall_rating);
	WriteInt32(enemy_soldier_killed);
	WriteInt32(own_soldier_killed);
	WriteInt32(enemy_civilian_killed);
	WriteInt32(own_civilian_killed);
	WriteInt32(enemy_weapon_destroyed);
	WriteInt32(own_weapon_destroyed);
	WriteInt32(enemy_ship_destroyed);
	WriteInt32(own_ship_destroyed);
	WriteInt32(enemy_firm_destroyed);
	WriteInt32(own_firm_destroyed);

	// Version_1_Nation
	WriteZeroBytes(action_array, sizeof(DynArrayGF));
	WriteInt16(last_action_id);
	WriteZero(ai_town_array);
	WriteZero(ai_base_array);
	WriteZero(ai_mine_array);
	WriteZero(ai_factory_array);
	WriteZero(ai_camp_array);
	WriteZero(ai_research_array);
	WriteZero(ai_war_array);
	WriteZero(ai_harbor_array);
	WriteZero(ai_market_array);
	WriteZero(ai_inn_array);
	WriteZero(ai_general_array);
	WriteZero(ai_caravan_array);
	WriteZero(ai_ship_array);
	WriteInt16(ai_town_size);
	WriteInt16(ai_base_size);
	WriteInt16(ai_mine_size);
	WriteInt16(ai_factory_size);
	WriteInt16(ai_camp_size);
	WriteInt16(ai_research_size);
	WriteInt16(ai_war_size);
	WriteInt16(ai_harbor_size);
	WriteInt16(ai_market_size);
	WriteInt16(ai_inn_size);
	WriteInt16(ai_general_size);
	WriteInt16(ai_caravan_size);
	WriteInt16(ai_ship_size);
	WriteInt16(ai_town_count);
	WriteInt16(ai_base_count);
	WriteInt16(ai_mine_count);
	WriteInt16(ai_factory_count);
	WriteInt16(ai_camp_count);
	WriteInt16(ai_research_count);
	WriteInt16(ai_war_count);
	WriteInt16(ai_harbor_count);
	WriteInt16(ai_market_count);
	WriteInt16(ai_inn_count);
	WriteInt16(ai_general_count);
	WriteInt16(ai_caravan_count);
	WriteInt16(ai_ship_count);
	WriteInt16(ai_base_town_count);
	WriteInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	WriteCallArray(ai_region_array, MAX_AI_REGION);
	WriteInt8(ai_region_count);
	WriteInt8(pref_force_projection);
	WriteInt8(pref_military_development);
	WriteInt8(pref_economic_development);
	WriteInt8(pref_inc_pop_by_capture);
	WriteInt8(pref_inc_pop_by_growth);
	WriteInt8(pref_peacefulness);
	WriteInt8(pref_military_courage);
	WriteInt8(pref_territorial_cohesiveness);
	WriteInt8(pref_trading_tendency);
	WriteInt8(pref_allying_tendency);
	WriteInt8(pref_honesty);
	WriteInt8(pref_town_harmony);
	WriteInt8(pref_loyalty_concern);
	WriteInt8(pref_forgiveness);
	WriteInt8(pref_collect_tax);
	WriteInt8(pref_hire_unit);
	WriteInt8(pref_use_weapon);
	WriteInt8(pref_keep_general);
	WriteInt8(pref_keep_skilled_unit);
	WriteInt8(pref_diplomacy_retry);
	WriteInt8(pref_attack_monster);
	WriteInt8(pref_spy);
	WriteInt8(pref_counter_spy);
	WriteInt8(pref_food_reserve);
	WriteInt8(pref_cash_reserve);
	WriteInt8(pref_use_marine);
	WriteInt8(pref_unit_chase_distance);
	WriteInt8(pref_repair_concern);
	WriteInt8(pref_scout);
	WriteInt16(ai_capture_enemy_town_recno);
	WriteInt32(ai_capture_enemy_town_plan_date);
	WriteInt32(ai_capture_enemy_town_start_attack_date);
	WriteInt8(ai_capture_enemy_town_use_all_camp);
	WriteInt32(ai_last_defend_action_date);
	WriteInt16(ai_attack_target_x_loc);
	WriteInt16(ai_attack_target_y_loc);
	WriteInt16(ai_attack_target_nation_recno);
	WriteCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	WriteInt16(attack_camp_count);
	WriteInt16(lead_attack_camp_recno);
}

void Version_1_Nation::read_record(Version_1_NationGF *r)
{
	// skip vtp

	// Version_1_NationBase
	ReadInt16(nation_recno);
	ReadInt8(nation_type);
	ReadInt8(race_id);
	ReadInt8(color_scheme_id);
	ReadInt8(nation_color);
	ReadInt16(king_unit_recno);
	ReadInt8(king_leadership);
	ReadInt32(nation_name_id);
	ReadInt8Array(nation_name_str, NATION_NAME_LEN+1);
	ReadInt32(player_id);
	ReadInt8(next_frame_ready);
	ReadInt16(last_caravan_id);
	ReadInt16(nation_firm_count);
	ReadInt32(last_build_firm_date);
	ReadInt8Array(know_base_array, VERSION_1_MAX_RACE);
	ReadInt8Array(base_count_array, VERSION_1_MAX_RACE);
	ReadInt8(is_at_war_today);
	ReadInt8(is_at_war_yesterday);
	ReadInt32(last_war_date);
	ReadInt16(last_attacker_unit_recno);
	ReadInt32(last_independent_unit_join_date);
	ReadInt8(cheat_enabled_flag);
	ReadFloat(cash);
	ReadFloat(food);
	ReadFloat(reputation);
	ReadFloat(kill_monster_score);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadFloat(cur_year_profit);
	ReadFloat(last_year_profit);
	ReadFloat(cur_year_fixed_income);
	ReadFloat(last_year_fixed_income);
	ReadFloat(cur_year_fixed_expense);
	ReadFloat(last_year_fixed_expense);
	ReadFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	ReadFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	ReadFloat(cur_year_income);
	ReadFloat(last_year_income);
	ReadFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloat(cur_year_expense);
	ReadFloat(last_year_expense);
	ReadFloat(cur_year_cheat);
	ReadFloat(last_year_cheat);
	ReadFloat(cur_year_food_in);
	ReadFloat(last_year_food_in);
	ReadFloat(cur_year_food_out);
	ReadFloat(last_year_food_out);
	ReadFloat(cur_year_food_change);
	ReadFloat(last_year_food_change);
	ReadFloat(cur_year_reputation_change);
	ReadFloat(last_year_reputation_change);
	ReadCallArray(relation_array, MAX_NATION);
	ReadInt8Array(relation_status_array, MAX_NATION);
	ReadInt8Array(relation_passable_array, MAX_NATION);
	ReadInt8Array(relation_should_attack_array, MAX_NATION);
	ReadInt8(is_allied_with_player);
	ReadInt32(total_population);
	ReadInt32(total_jobless_population);
	ReadInt32(total_unit_count);
	ReadInt32(total_human_count);
	ReadInt32(total_general_count);
	ReadInt32(total_weapon_count);
	ReadInt32(total_ship_count);
	ReadInt32(total_firm_count);
	ReadInt32(total_spy_count);
	ReadInt32(total_ship_combat_level);
	ReadInt16(largest_town_recno);
	ReadInt16(largest_town_pop);
	ReadInt16Array(raw_count_array, MAX_RAW);
	ReadInt16Array(last_unit_name_id_array, VERSION_1_MAX_UNIT_TYPE);
	ReadInt32(population_rating);
	ReadInt32(military_rating);
	ReadInt32(economic_rating);
	ReadInt32(overall_rating);
	ReadInt32(enemy_soldier_killed);
	ReadInt32(own_soldier_killed);
	ReadInt32(enemy_civilian_killed);
	ReadInt32(own_civilian_killed);
	ReadInt32(enemy_weapon_destroyed);
	ReadInt32(own_weapon_destroyed);
	ReadInt32(enemy_ship_destroyed);
	ReadInt32(own_ship_destroyed);
	ReadInt32(enemy_firm_destroyed);
	ReadInt32(own_firm_destroyed);

	// Version_1_Nation
	// skip action_array
	ReadInt16(last_action_id);
	// skip ai_town_array
	// skip ai_base_array
	// skip ai_mine_array
	// skip ai_factory_array
	// skip ai_camp_array
	// skip ai_research_array
	// skip ai_war_array
	// skip ai_harbor_array
	// skip ai_market_array
	// skip ai_inn_array
	// skip ai_general_array
	// skip ai_caravan_array
	// skip ai_ship_array
	ReadInt16(ai_town_size);
	ReadInt16(ai_base_size);
	ReadInt16(ai_mine_size);
	ReadInt16(ai_factory_size);
	ReadInt16(ai_camp_size);
	ReadInt16(ai_research_size);
	ReadInt16(ai_war_size);
	ReadInt16(ai_harbor_size);
	ReadInt16(ai_market_size);
	ReadInt16(ai_inn_size);
	ReadInt16(ai_general_size);
	ReadInt16(ai_caravan_size);
	ReadInt16(ai_ship_size);
	ReadInt16(ai_town_count);
	ReadInt16(ai_base_count);
	ReadInt16(ai_mine_count);
	ReadInt16(ai_factory_count);
	ReadInt16(ai_camp_count);
	ReadInt16(ai_research_count);
	ReadInt16(ai_war_count);
	ReadInt16(ai_harbor_count);
	ReadInt16(ai_market_count);
	ReadInt16(ai_inn_count);
	ReadInt16(ai_general_count);
	ReadInt16(ai_caravan_count);
	ReadInt16(ai_ship_count);
	ReadInt16(ai_base_town_count);
	ReadInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	ReadCallArray(ai_region_array, MAX_AI_REGION);
	ReadInt8(ai_region_count);
	ReadInt8(pref_force_projection);
	ReadInt8(pref_military_development);
	ReadInt8(pref_economic_development);
	ReadInt8(pref_inc_pop_by_capture);
	ReadInt8(pref_inc_pop_by_growth);
	ReadInt8(pref_peacefulness);
	ReadInt8(pref_military_courage);
	ReadInt8(pref_territorial_cohesiveness);
	ReadInt8(pref_trading_tendency);
	ReadInt8(pref_allying_tendency);
	ReadInt8(pref_honesty);
	ReadInt8(pref_town_harmony);
	ReadInt8(pref_loyalty_concern);
	ReadInt8(pref_forgiveness);
	ReadInt8(pref_collect_tax);
	ReadInt8(pref_hire_unit);
	ReadInt8(pref_use_weapon);
	ReadInt8(pref_keep_general);
	ReadInt8(pref_keep_skilled_unit);
	ReadInt8(pref_diplomacy_retry);
	ReadInt8(pref_attack_monster);
	ReadInt8(pref_spy);
	ReadInt8(pref_counter_spy);
	ReadInt8(pref_food_reserve);
	ReadInt8(pref_cash_reserve);
	ReadInt8(pref_use_marine);
	ReadInt8(pref_unit_chase_distance);
	ReadInt8(pref_repair_concern);
	ReadInt8(pref_scout);
	ReadInt16(ai_capture_enemy_town_recno);
	ReadInt32(ai_capture_enemy_town_plan_date);
	ReadInt32(ai_capture_enemy_town_start_attack_date);
	ReadInt8(ai_capture_enemy_town_use_all_camp);
	ReadInt32(ai_last_defend_action_date);
	ReadInt16(ai_attack_target_x_loc);
	ReadInt16(ai_attack_target_y_loc);
	ReadInt16(ai_attack_target_nation_recno);
	ReadCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	ReadInt16(attack_camp_count);
	ReadInt16(lead_attack_camp_recno);
}

void Version_1_NationArray::write_record(Version_1_NationArrayGF *r)
{
	WriteInt16(nation_count);
	WriteInt16(ai_nation_count);
	WriteInt32(last_del_nation_date);
	WriteInt32(last_new_nation_date);
	WriteInt32(max_nation_population);
	WriteInt32(all_nation_population);
	WriteInt16(independent_town_count);
	WriteInt16Array(independent_town_count_race_array, VERSION_1_MAX_RACE);
	WriteInt32(max_nation_units);
	WriteInt32(max_nation_humans);
	WriteInt32(max_nation_generals);
	WriteInt32(max_nation_weapons);
	WriteInt32(max_nation_ships);
	WriteInt32(max_nation_spies);
	WriteInt32(max_nation_firms);
	WriteInt32(max_nation_tech_level);
	WriteInt32(max_population_rating);
	WriteInt32(max_military_rating);
	WriteInt32(max_economic_rating);
	WriteInt32(max_reputation);
	WriteInt32(max_kill_monster_score);
	WriteInt32(max_overall_rating);
	WriteInt16(max_population_nation_recno);
	WriteInt16(max_military_nation_recno);
	WriteInt16(max_economic_nation_recno);
	WriteInt16(max_reputation_nation_recno);
	WriteInt16(max_kill_monster_nation_recno);
	WriteInt16(max_overall_nation_recno);
	WriteInt32(last_alliance_id);
	WriteInt32(nation_peace_days);
	WriteInt16(player_recno);
	WriteZero(player_ptr);
	WriteInt8Array(nation_color_array, MAX_NATION+1);
	WriteInt8Array(nation_power_color_array, MAX_NATION+2);
	WriteInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void Version_1_NationArray::read_record(Version_1_NationArrayGF *r)
{
	ReadInt16(nation_count);
	ReadInt16(ai_nation_count);
	ReadInt32(last_del_nation_date);
	ReadInt32(last_new_nation_date);
	ReadInt32(max_nation_population);
	ReadInt32(all_nation_population);
	ReadInt16(independent_town_count);
	ReadInt16Array(independent_town_count_race_array, VERSION_1_MAX_RACE);
	ReadInt32(max_nation_units);
	ReadInt32(max_nation_humans);
	ReadInt32(max_nation_generals);
	ReadInt32(max_nation_weapons);
	ReadInt32(max_nation_ships);
	ReadInt32(max_nation_spies);
	ReadInt32(max_nation_firms);
	ReadInt32(max_nation_tech_level);
	ReadInt32(max_population_rating);
	ReadInt32(max_military_rating);
	ReadInt32(max_economic_rating);
	ReadInt32(max_reputation);
	ReadInt32(max_kill_monster_score);
	ReadInt32(max_overall_rating);
	ReadInt16(max_population_nation_recno);
	ReadInt16(max_military_nation_recno);
	ReadInt16(max_economic_nation_recno);
	ReadInt16(max_reputation_nation_recno);
	ReadInt16(max_kill_monster_nation_recno);
	ReadInt16(max_overall_nation_recno);
	ReadInt32(last_alliance_id);
	ReadInt32(nation_peace_days);
	ReadInt16(player_recno);
	// skip player_ptr
	ReadInt8Array(nation_color_array, MAX_NATION+1);
	ReadInt8Array(nation_power_color_array, MAX_NATION+2);
	ReadInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void Bullet::write_record(BulletGF *r)
{
	WriteZero(vtp);

	// Sprite
	WriteInt16(sprite_id);
	WriteInt16(sprite_recno);
	WriteInt8(mobile_type);
	WriteInt8(cur_action);
	WriteInt8(cur_dir);
	WriteInt8(cur_frame);
	WriteInt8(cur_attack);
	WriteInt8(final_dir);
	WriteInt8(turn_delay);
	WriteInt8(guard_count);
	WriteInt8(remain_attack_delay);
	WriteInt8(remain_frames_per_step);
	WriteInt16(cur_x);
	WriteInt16(cur_y);
	WriteInt16(go_x);
	WriteInt16(go_y);
	WriteInt16(next_x);
	WriteInt16(next_y);
	WriteZero(sprite_info);

	// Bullet
	WriteInt8(parent_type);
	WriteInt16(parent_recno);
	WriteInt8(target_mobile_type);
	WriteFloat(attack_damage);
	WriteInt16(damage_radius);
	WriteInt16(nation_recno);
	WriteInt8(fire_radius);
	WriteInt16(origin_x);
	WriteInt16(origin_y);
	WriteInt16(target_x_loc);
	WriteInt16(target_y_loc);
	WriteInt8(cur_step);
	WriteInt8(total_step);
}

void Bullet::read_record(BulletGF *r)
{
	// skip vtp

	// Sprite
	ReadInt16(sprite_id);
	ReadInt16(sprite_recno);
	ReadInt8(mobile_type);
	ReadInt8(cur_action);
	ReadInt8(cur_dir);
	ReadInt8(cur_frame);
	ReadInt8(cur_attack);
	ReadInt8(final_dir);
	ReadInt8(turn_delay);
	ReadInt8(guard_count);
	ReadInt8(remain_attack_delay);
	ReadInt8(remain_frames_per_step);
	ReadInt16(cur_x);
	ReadInt16(cur_y);
	ReadInt16(go_x);
	ReadInt16(go_y);
	ReadInt16(next_x);
	ReadInt16(next_y);
	// skip sprite_info

	// Bullet
	ReadInt8(parent_type);
	ReadInt16(parent_recno);
	ReadInt8(target_mobile_type);
	ReadFloat(attack_damage);
	ReadInt16(damage_radius);
	ReadInt16(nation_recno);
	ReadInt8(fire_radius);
	ReadInt16(origin_x);
	ReadInt16(origin_y);
	ReadInt16(target_x_loc);
	ReadInt16(target_y_loc);
	ReadInt8(cur_step);
	ReadInt8(total_step);
}

void BulletHoming::write_derived_record(BulletHomingGF *r)
{
	WriteInt8(max_step);
	WriteInt8(target_type);
	WriteInt16(target_recno);
	WriteInt16(speed);
	WriteInt16(origin2_x);
	WriteInt16(origin2_y);
}

void BulletHoming::read_derived_record(BulletHomingGF *r)
{
	ReadInt8(max_step);
	ReadInt8(target_type);
	ReadInt16(target_recno);
	ReadInt16(speed);
	ReadInt16(origin2_x);
	ReadInt16(origin2_y);
}

void Projectile::write_derived_record(ProjectileGF *r)
{
	WriteFloat(z_coff);
	WriteCall(act_bullet);
	WriteCall(bullet_shadow);
}

void Projectile::read_derived_record(ProjectileGF *r)
{
	ReadFloat(z_coff);
	ReadCall(act_bullet);
	ReadCall(bullet_shadow);
}

void Worker::write_record(WorkerGF *r)
{
	WriteInt8(race_id);
	WriteInt8(unit_id);
	WriteInt16(town_recno);
	WriteInt16(name_id);
	WriteInt8(skill_id);
	WriteInt8(skill_level);
	WriteInt8(skill_level_minor);
	WriteInt8(skill_potential);
	WriteInt8(combat_level);
	WriteInt8(combat_level_minor);
	WriteInt16(spy_recno);
	WriteInt8(rank_id);
	WriteInt8(worker_loyalty);
	WriteInt16(hit_points);
	WriteInt16(extra_para);
}

void Worker::read_record(WorkerGF *r)
{
	ReadInt8(race_id);
	ReadInt8(unit_id);
	ReadInt16(town_recno);
	ReadInt16(name_id);
	ReadInt8(skill_id);
	ReadInt8(skill_level);
	ReadInt8(skill_level_minor);
	ReadInt8(skill_potential);
	ReadInt8(combat_level);
	ReadInt8(combat_level_minor);
	ReadInt16(spy_recno);
	ReadInt8(rank_id);
	ReadInt8(worker_loyalty);
	ReadInt16(hit_points);
	ReadInt16(extra_para);
}

void Firm::write_record(FirmGF *r)
{
	WriteZero(vtp); //zero

	WriteInt8(firm_id);
	WriteInt16(firm_build_id);
	WriteInt16(firm_recno);
	WriteInt8(firm_ai);
	WriteInt8(ai_processed);
	WriteInt8(ai_status);
	WriteInt8(ai_link_checked);
	WriteInt8(ai_sell_flag);
	WriteInt8(race_id);
	WriteInt16(nation_recno);
	WriteInt16(closest_town_name_id);
	WriteInt16(firm_name_instance_id);
	WriteInt16(loc_x1);
	WriteInt16(loc_y1);
	WriteInt16(loc_x2);
	WriteInt16(loc_y2);
	WriteInt16(abs_x1);
	WriteInt16(abs_y1);
	WriteInt16(abs_x2);
	WriteInt16(abs_y2);
	WriteInt16(center_x);
	WriteInt16(center_y);
	WriteInt8(region_id);
	WriteInt8(cur_frame);
	WriteInt8(remain_frame_delay);
	WriteFloat(hit_points);
	WriteFloat(max_hit_points);
	WriteInt8(under_construction);
	WriteInt8(firm_skill_id);
	WriteInt16(overseer_recno);
	WriteInt16(overseer_town_recno);
	WriteInt16(builder_recno);
	WriteInt8(builder_region_id);
	WriteFloat(productivity);
	WriteZero(worker_array);
	WriteInt8(worker_count);
	WriteInt8(selected_worker_id);
	WriteInt8(player_spy_count);
	WriteInt8(sabotage_level);
	WriteInt8(linked_firm_count);
	WriteInt8(linked_town_count);
	WriteInt16Array(linked_firm_array, MAX_LINKED_FIRM_FIRM);
	WriteInt16Array(linked_town_array, MAX_LINKED_FIRM_TOWN);
	WriteInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_FIRM);
	WriteInt8Array(linked_town_enable_array, MAX_LINKED_FIRM_TOWN);
	WriteFloat(last_year_income);
	WriteFloat(cur_year_income);
	WriteInt32(setup_date);
	WriteInt8(should_set_power);
	WriteInt32(last_attacked_date);
	WriteInt8(should_close_flag);
	WriteInt8(no_neighbor_space);
	WriteInt8(ai_should_build_factory_count);
}

void Firm::read_record(FirmGF *r)
{
	// skip vtp

	ReadInt8(firm_id);
	ReadInt16(firm_build_id);
	ReadInt16(firm_recno);
	ReadInt8(firm_ai);
	ReadInt8(ai_processed);
	ReadInt8(ai_status);
	ReadInt8(ai_link_checked);
	ReadInt8(ai_sell_flag);
	ReadInt8(race_id);
	ReadInt16(nation_recno);
	ReadInt16(closest_town_name_id);
	ReadInt16(firm_name_instance_id);
	ReadInt16(loc_x1);
	ReadInt16(loc_y1);
	ReadInt16(loc_x2);
	ReadInt16(loc_y2);
	ReadInt16(abs_x1);
	ReadInt16(abs_y1);
	ReadInt16(abs_x2);
	ReadInt16(abs_y2);
	ReadInt16(center_x);
	ReadInt16(center_y);
	ReadInt8(region_id);
	ReadInt8(cur_frame);
	ReadInt8(remain_frame_delay);
	ReadFloat(hit_points);
	ReadFloat(max_hit_points);
	ReadInt8(under_construction);
	ReadInt8(firm_skill_id);
	ReadInt16(overseer_recno);
	ReadInt16(overseer_town_recno);
	ReadInt16(builder_recno);
	ReadInt8(builder_region_id);
	ReadFloat(productivity);
	// skip worker_array
	ReadInt8(worker_count);
	ReadInt8(selected_worker_id);
	ReadInt8(player_spy_count);
	ReadInt8(sabotage_level);
	ReadInt8(linked_firm_count);
	ReadInt8(linked_town_count);
	ReadInt16Array(linked_firm_array, MAX_LINKED_FIRM_FIRM);
	ReadInt16Array(linked_town_array, MAX_LINKED_FIRM_TOWN);
	ReadInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_FIRM);
	ReadInt8Array(linked_town_enable_array, MAX_LINKED_FIRM_TOWN);
	ReadFloat(last_year_income);
	ReadFloat(cur_year_income);
	ReadInt32(setup_date);
	ReadInt8(should_set_power);
	ReadInt32(last_attacked_date);
	ReadInt8(should_close_flag);
	ReadInt8(no_neighbor_space);
	ReadInt8(ai_should_build_factory_count);
}

void FirmBase::write_derived_record(FirmBaseGF *r)
{
	WriteInt16(god_id);
	WriteInt16(god_unit_recno);
	WriteFloat(pray_points);
}

void FirmBase::read_derived_record(FirmBaseGF *r)
{
	ReadInt16(god_id);
	ReadInt16(god_unit_recno);
	ReadFloat(pray_points);
}

void DefenseUnit::write_record(DefenseUnitGF *r)
{
	WriteInt16(unit_recno);
	WriteInt8(status);
}

void DefenseUnit::read_record(DefenseUnitGF *r)
{
	ReadInt16(unit_recno);
	ReadInt8(status);
}

void FirmCamp::write_derived_record(FirmCampGF *r)
{
	WriteCallArray(defense_array, MAX_WORKER+1);
	WriteInt8(employ_new_worker);
	WriteInt16(defend_target_recno);
	WriteInt8(defense_flag);
	WriteInt8(patrol_unit_count);
	WriteInt16Array(patrol_unit_array, MAX_WORKER+1);
	WriteInt8(coming_unit_count);
	WriteInt16Array(coming_unit_array, MAX_WORKER+1);
	WriteInt16(ai_capture_town_recno);
	WriteInt8(ai_recruiting_soldier);
	WriteInt8(is_attack_camp);
}

void FirmCamp::read_derived_record(FirmCampGF *r)
{
	ReadCallArray(defense_array, MAX_WORKER+1);
	ReadInt8(employ_new_worker);
	ReadInt16(defend_target_recno);
	ReadInt8(defense_flag);
	ReadInt8(patrol_unit_count);
	ReadInt16Array(patrol_unit_array, MAX_WORKER+1);
	ReadInt8(coming_unit_count);
	ReadInt16Array(coming_unit_array, MAX_WORKER+1);
	ReadInt16(ai_capture_town_recno);
	ReadInt8(ai_recruiting_soldier);
	ReadInt8(is_attack_camp);
}

void FirmFactory::write_derived_record(FirmFactoryGF *r)
{
	WriteInt32(product_raw_id);
	WriteFloat(stock_qty);
	WriteFloat(max_stock_qty);
	WriteFloat(raw_stock_qty);
	WriteFloat(max_raw_stock_qty);
	WriteFloat(cur_month_production);
	WriteFloat(last_month_production);
	WriteInt16(next_output_link_id);
	WriteInt16(next_output_firm_recno);
}

void FirmFactory::read_derived_record(FirmFactoryGF *r)
{
	ReadInt32(product_raw_id);
	ReadFloat(stock_qty);
	ReadFloat(max_stock_qty);
	ReadFloat(raw_stock_qty);
	ReadFloat(max_raw_stock_qty);
	ReadFloat(cur_month_production);
	ReadFloat(last_month_production);
	ReadInt16(next_output_link_id);
	ReadInt16(next_output_firm_recno);
}

void FirmHarbor::write_derived_record(FirmHarborGF *r)
{
	WriteInt16Array(ship_recno_array, MAX_SHIP_IN_HARBOR);
	WriteInt16(ship_count);
	WriteInt16(build_unit_id);
	WriteInt32(start_build_frame_no);
	WriteInt8Array(build_queue_array, MAX_BUILD_SHIP_QUEUE);
	WriteInt8(build_queue_count);
	WriteInt8(land_region_id);
	WriteInt8(sea_region_id);
	WriteInt8(link_checked);
	WriteInt8(linked_mine_num);
	WriteInt8(linked_factory_num);
	WriteInt8(linked_market_num);
	WriteInt16Array(linked_mine_array, MAX_LINKED_FIRM_FIRM);
	WriteInt16Array(linked_factory_array, MAX_LINKED_FIRM_FIRM);
	WriteInt16Array(linked_market_array, MAX_LINKED_FIRM_FIRM);
}

void FirmHarbor::read_derived_record(FirmHarborGF *r)
{
	ReadInt16Array(ship_recno_array, MAX_SHIP_IN_HARBOR);
	ReadInt16(ship_count);
	ReadInt16(build_unit_id);
	ReadInt32(start_build_frame_no);
	ReadInt8Array(build_queue_array, MAX_BUILD_SHIP_QUEUE);
	ReadInt8(build_queue_count);
	ReadInt8(land_region_id);
	ReadInt8(sea_region_id);
	ReadInt8(link_checked);
	ReadInt8(linked_mine_num);
	ReadInt8(linked_factory_num);
	ReadInt8(linked_market_num);
	ReadInt16Array(linked_mine_array, MAX_LINKED_FIRM_FIRM);
	ReadInt16Array(linked_factory_array, MAX_LINKED_FIRM_FIRM);
	ReadInt16Array(linked_market_array, MAX_LINKED_FIRM_FIRM);
}

void InnUnit::write_record(InnUnitGF *r)
{
	WriteInt8(unit_id);
	WriteCall(skill);
	WriteInt16(hire_cost);
	WriteInt16(stay_count);
	WriteInt16(spy_recno);
}

void InnUnit::read_record(InnUnitGF *r)
{
	ReadInt8(unit_id);
	ReadCall(skill);
	ReadInt16(hire_cost);
	ReadInt16(stay_count);
	ReadInt16(spy_recno);
}

void FirmInn::write_derived_record(FirmInnGF *r)
{
	WriteInt16(next_skill_id);
	WriteCallArray(inn_unit_array, MAX_INN_UNIT);
	WriteInt16(inn_unit_count);
}

void FirmInn::read_derived_record(FirmInnGF *r)
{
	ReadInt16(next_skill_id);
	ReadCallArray(inn_unit_array, MAX_INN_UNIT);
	ReadInt16(inn_unit_count);
}

void MarketGoods::write_record(MarketGoodsGF *r)
{
	WriteInt8(raw_id);
	WriteInt8(product_raw_id);
	WriteInt16(input_firm_recno);
	WriteFloat(stock_qty);
	WriteFloat(cur_month_supply);
	WriteFloat(last_month_supply);
	WriteFloat(month_demand);
	WriteFloat(cur_month_sale_qty);
	WriteFloat(last_month_sale_qty);
	WriteFloat(cur_year_sales);
	WriteFloat(last_year_sales);
}

void MarketGoods::read_record(MarketGoodsGF *r)
{
	ReadInt8(raw_id);
	ReadInt8(product_raw_id);
	ReadInt16(input_firm_recno);
	ReadFloat(stock_qty);
	ReadFloat(cur_month_supply);
	ReadFloat(last_month_supply);
	ReadFloat(month_demand);
	ReadFloat(cur_month_sale_qty);
	ReadFloat(last_month_sale_qty);
	ReadFloat(cur_year_sales);
	ReadFloat(last_year_sales);
}

void FirmMarket::write_derived_record(FirmMarketGF *r)
{
	WriteFloat(max_stock_qty);
	WriteCallArray(market_goods_array, MAX_MARKET_GOODS);
	WriteZeroArray(market_raw_array, MAX_RAW);
	WriteZeroArray(market_product_array, MAX_PRODUCT);
	WriteInt16(next_output_link_id);
	WriteInt16(next_output_firm_recno);
	WriteInt32(no_linked_town_since_date);
	WriteInt32(last_import_new_goods_date);
	WriteInt8(restock_type);
}

void FirmMarket::read_derived_record(FirmMarketGF *r)
{
	ReadFloat(max_stock_qty);
	ReadCallArray(market_goods_array, MAX_MARKET_GOODS);
	// skip market_raw_array
	// skip market_product_array
	ReadInt16(next_output_link_id);
	ReadInt16(next_output_firm_recno);
	ReadInt32(no_linked_town_since_date);
	ReadInt32(last_import_new_goods_date);
	ReadInt8(restock_type);
}

void FirmMine::write_derived_record(FirmMineGF *r)
{
	WriteInt16(raw_id);
	WriteInt16(site_recno);
	WriteFloat(reserve_qty);
	WriteFloat(stock_qty);
	WriteFloat(max_stock_qty);
	WriteInt16(next_output_link_id);
	WriteInt16(next_output_firm_recno);
	WriteFloat(cur_month_production);
	WriteFloat(last_month_production);
}

void FirmMine::read_derived_record(FirmMineGF *r)
{
	ReadInt16(raw_id);
	ReadInt16(site_recno);
	ReadFloat(reserve_qty);
	ReadFloat(stock_qty);
	ReadFloat(max_stock_qty);
	ReadInt16(next_output_link_id);
	ReadInt16(next_output_firm_recno);
	ReadFloat(cur_month_production);
	ReadFloat(last_month_production);
}

void MonsterInFirm::write_record(MonsterInFirmGF *r)
{
	WriteInt8(monster_id);
	WriteInt8(_unused);
	WriteInt16(mobile_unit_recno);
	WriteInt8(combat_level);
	WriteInt16(hit_points);
	WriteInt16(max_hit_points);
	WriteInt8(soldier_monster_id);
	WriteInt8(soldier_count);
}

void MonsterInFirm::read_record(MonsterInFirmGF *r)
{
	ReadInt8(monster_id);
	ReadInt8(_unused);
	ReadInt16(mobile_unit_recno);
	ReadInt8(combat_level);
	ReadInt16(hit_points);
	ReadInt16(max_hit_points);
	ReadInt8(soldier_monster_id);
	ReadInt8(soldier_count);
}

void FirmMonster::write_derived_record(FirmMonsterGF *r)
{
	WriteInt16(monster_id);
	WriteInt16(monster_general_count);
	WriteInt8(monster_aggressiveness);
	WriteInt8(defending_king_count);
	WriteInt8(defending_general_count);
	WriteInt8(defending_soldier_count);
	WriteCall(monster_king);
	WriteCallArray(monster_general_array, MAX_MONSTER_GENERAL_IN_FIRM);
	WriteInt8(waiting_soldier_count);
	WriteInt16Array(waiting_soldier_array, MAX_WAITING_SOLDIER);
	WriteInt8(monster_nation_relation);
	WriteInt16(defend_target_recno);
	WriteInt8(patrol_unit_count);
	WriteInt16Array(patrol_unit_array, MAX_SOLDIER_PER_GENERAL+1);
}

void FirmMonster::read_derived_record(FirmMonsterGF *r)
{
	ReadInt16(monster_id);
	ReadInt16(monster_general_count);
	ReadInt8(monster_aggressiveness);
	ReadInt8(defending_king_count);
	ReadInt8(defending_general_count);
	ReadInt8(defending_soldier_count);
	ReadCall(monster_king);
	ReadCallArray(monster_general_array, MAX_MONSTER_GENERAL_IN_FIRM);
	ReadInt8(waiting_soldier_count);
	ReadInt16Array(waiting_soldier_array, MAX_WAITING_SOLDIER);
	ReadInt8(monster_nation_relation);
	ReadInt16(defend_target_recno);
	ReadInt8(patrol_unit_count);
	ReadInt16Array(patrol_unit_array, MAX_SOLDIER_PER_GENERAL+1);
}

void FirmResearch::write_derived_record(FirmResearchGF *r)
{
	WriteInt16(tech_id);
	WriteFloat(complete_percent);
}

void FirmResearch::read_derived_record(FirmResearchGF *r)
{
	ReadInt16(tech_id);
	ReadFloat(complete_percent);
}

void FirmWar::write_derived_record(FirmWarGF *r)
{
	WriteInt16(build_unit_id);
	WriteInt32(last_process_build_frame_no);
	WriteFloat(build_progress_days);
	WriteInt8Array(build_queue_array, MAX_BUILD_QUEUE);
	WriteInt8(build_queue_count);
}

void FirmWar::read_derived_record(FirmWarGF *r)
{
	ReadInt16(build_unit_id);
	ReadInt32(last_process_build_frame_no);
	ReadFloat(build_progress_days);
	ReadInt8Array(build_queue_array, MAX_BUILD_QUEUE);
	ReadInt8(build_queue_count);
}

void Site::write_record(SiteGF *r)
{
	WriteInt16(site_recno);
	WriteInt8(site_type);
	WriteInt16(object_id);
	WriteInt32(reserve_qty);
	WriteInt8(has_mine);
	WriteInt16(map_x_loc);
	WriteInt16(map_y_loc);
	WriteInt8(region_id);
}

void Site::read_record(SiteGF *r)
{
	ReadInt16(site_recno);
	ReadInt8(site_type);
	ReadInt16(object_id);
	ReadInt32(reserve_qty);
	ReadInt8(has_mine);
	ReadInt16(map_x_loc);
	ReadInt16(map_y_loc);
	ReadInt8(region_id);
}

void TownArray::write_record(TownArrayGF *r)
{
	WriteInt32Array(race_wander_pop_array, MAX_RACE);
}

void TownArray::read_record(TownArrayGF *r)
{
	ReadInt32Array(race_wander_pop_array, MAX_RACE);
}

void TownArray::read_record_v1(Version_1_TownArrayGF *r)
{
	ReadInt32Array(race_wander_pop_array, VERSION_1_MAX_RACE);
	for( int i=VERSION_1_MAX_RACE; i<MAX_RACE; i++ )
		race_wander_pop_array[i] = 0;
}

void Town::write_record(TownGF *r)
{
	WriteInt16(town_recno);
	WriteInt16(town_name_id);
	WriteInt16(nation_recno);
	WriteInt16(rebel_recno);
	WriteInt8(race_id);
	WriteInt32(setup_date);
	WriteInt8(ai_town);
	WriteInt8(ai_link_checked);
	WriteInt8(independ_town_nation_relation);
	WriteInt8(has_linked_own_camp);
	WriteInt8(has_linked_enemy_camp);
	WriteInt8(is_base_town);
	WriteInt16(loc_x1);
	WriteInt16(loc_y1);
	WriteInt16(loc_x2);
	WriteInt16(loc_y2);
	WriteInt16(abs_x1);
	WriteInt16(abs_y1);
	WriteInt16(abs_x2);
	WriteInt16(abs_y2);
	WriteInt16(center_x);
	WriteInt16(center_y);
	WriteInt8(region_id);
	WriteInt16(layout_id);
	WriteInt16(first_slot_id);
	WriteInt16Array(slot_object_id_array, MAX_TOWN_LAYOUT_SLOT);
	WriteInt16(population);
	WriteInt16(jobless_population);
	WriteInt16Array(max_race_pop_array, MAX_RACE);
	WriteInt16Array(race_pop_array, MAX_RACE);
	WriteInt8Array(race_pop_growth_array, MAX_RACE);
	WriteInt16Array(jobless_race_pop_array, MAX_RACE);
	WriteFloatArray(race_loyalty_array, MAX_RACE);
	WriteInt8Array(race_target_loyalty_array, MAX_RACE);
	WriteInt16Array(race_spy_count_array, MAX_RACE);
	WriteFloatArray(race_resistance_array, MAX_NATION*MAX_RACE);
	WriteInt8Array(race_target_resistance_array, MAX_NATION*MAX_RACE);
	WriteInt16(town_defender_count);
	WriteInt32(last_being_attacked_date);
	WriteFloat(received_hit_count);
	WriteInt8Array(train_queue_skill_array, MAX_TRAIN_QUEUE);
	WriteInt8Array(train_queue_race_array, MAX_TRAIN_QUEUE);
	WriteInt8(train_queue_count);
	WriteInt16(train_unit_recno);
	WriteInt32(train_unit_action_id);
	WriteInt32(start_train_frame_no);
	WriteInt16(defend_target_recno);
	WriteInt32(accumulated_collect_tax_penalty);
	WriteInt32(accumulated_reward_penalty);
	WriteInt32(accumulated_recruit_penalty);
	WriteInt32(accumulated_enemy_grant_penalty);
	WriteInt32(last_rebel_date);
	WriteInt16(independent_unit_join_nation_min_rating);
	WriteInt16(quality_of_life);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteInt8(town_combat_level);
	WriteInt8Array(has_product_supply, MAX_PRODUCT);
	WriteInt8(no_neighbor_space);
	WriteInt16(linked_firm_count);
	WriteInt16(linked_town_count);
	WriteInt16Array(linked_firm_array, MAX_LINKED_FIRM_TOWN);
	WriteInt16Array(linked_town_array, MAX_LINKED_TOWN_TOWN);
	WriteInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_TOWN);
	WriteInt8Array(linked_town_enable_array, MAX_LINKED_TOWN_TOWN);
}

void Town::read_record(TownGF *r)
{
	ReadInt16(town_recno);
	ReadInt16(town_name_id);
	ReadInt16(nation_recno);
	ReadInt16(rebel_recno);
	ReadInt8(race_id);
	ReadInt32(setup_date);
	ReadInt8(ai_town);
	ReadInt8(ai_link_checked);
	ReadInt8(independ_town_nation_relation);
	ReadInt8(has_linked_own_camp);
	ReadInt8(has_linked_enemy_camp);
	ReadInt8(is_base_town);
	ReadInt16(loc_x1);
	ReadInt16(loc_y1);
	ReadInt16(loc_x2);
	ReadInt16(loc_y2);
	ReadInt16(abs_x1);
	ReadInt16(abs_y1);
	ReadInt16(abs_x2);
	ReadInt16(abs_y2);
	ReadInt16(center_x);
	ReadInt16(center_y);
	ReadInt8(region_id);
	ReadInt16(layout_id);
	ReadInt16(first_slot_id);
	ReadInt16Array(slot_object_id_array, MAX_TOWN_LAYOUT_SLOT);
	ReadInt16(population);
	ReadInt16(jobless_population);
	ReadInt16Array(max_race_pop_array, MAX_RACE);
	ReadInt16Array(race_pop_array, MAX_RACE);
	ReadInt8Array(race_pop_growth_array, MAX_RACE);
	ReadInt16Array(jobless_race_pop_array, MAX_RACE);
	ReadFloatArray(race_loyalty_array, MAX_RACE);
	ReadInt8Array(race_target_loyalty_array, MAX_RACE);
	ReadInt16Array(race_spy_count_array, MAX_RACE);
	ReadFloatArray(race_resistance_array, MAX_NATION*MAX_RACE);
	ReadInt8Array(race_target_resistance_array, MAX_NATION*MAX_RACE);
	ReadInt16(town_defender_count);
	ReadInt32(last_being_attacked_date);
	ReadFloat(received_hit_count);
	ReadInt8Array(train_queue_skill_array, MAX_TRAIN_QUEUE);
	ReadInt8Array(train_queue_race_array, MAX_TRAIN_QUEUE);
	ReadInt8(train_queue_count);
	ReadInt16(train_unit_recno);
	ReadInt32(train_unit_action_id);
	ReadInt32(start_train_frame_no);
	ReadInt16(defend_target_recno);
	ReadInt32(accumulated_collect_tax_penalty);
	ReadInt32(accumulated_reward_penalty);
	ReadInt32(accumulated_recruit_penalty);
	ReadInt32(accumulated_enemy_grant_penalty);
	ReadInt32(last_rebel_date);
	ReadInt16(independent_unit_join_nation_min_rating);
	ReadInt16(quality_of_life);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadInt8(town_combat_level);
	ReadInt8Array(has_product_supply, MAX_PRODUCT);
	ReadInt8(no_neighbor_space);
	ReadInt16(linked_firm_count);
	ReadInt16(linked_town_count);
	ReadInt16Array(linked_firm_array, MAX_LINKED_FIRM_TOWN);
	ReadInt16Array(linked_town_array, MAX_LINKED_TOWN_TOWN);
	ReadInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_TOWN);
	ReadInt8Array(linked_town_enable_array, MAX_LINKED_TOWN_TOWN);
}

void Version_1_Town::write_record(Version_1_TownGF *r)
{
	WriteInt16(town_recno);
	WriteInt16(town_name_id);
	WriteInt16(nation_recno);
	WriteInt16(rebel_recno);
	WriteInt8(race_id);
	WriteInt32(setup_date);
	WriteInt8(ai_town);
	WriteInt8(ai_link_checked);
	WriteInt8(independ_town_nation_relation);
	WriteInt8(has_linked_own_camp);
	WriteInt8(has_linked_enemy_camp);
	WriteInt8(is_base_town);
	WriteInt16(loc_x1);
	WriteInt16(loc_y1);
	WriteInt16(loc_x2);
	WriteInt16(loc_y2);
	WriteInt16(abs_x1);
	WriteInt16(abs_y1);
	WriteInt16(abs_x2);
	WriteInt16(abs_y2);
	WriteInt16(center_x);
	WriteInt16(center_y);
	WriteInt8(region_id);
	WriteInt16(layout_id);
	WriteInt16(first_slot_id);
	WriteInt16Array(slot_object_id_array, MAX_TOWN_LAYOUT_SLOT);
	WriteInt16(population);
	WriteInt16(jobless_population);
	WriteInt16Array(max_race_pop_array, VERSION_1_MAX_RACE);
	WriteInt16Array(race_pop_array, VERSION_1_MAX_RACE);
	WriteInt8Array(race_pop_growth_array, VERSION_1_MAX_RACE);
	WriteInt16Array(jobless_race_pop_array, VERSION_1_MAX_RACE);
	WriteFloatArray(race_loyalty_array, VERSION_1_MAX_RACE);
	WriteInt8Array(race_target_loyalty_array, VERSION_1_MAX_RACE);
	WriteInt16Array(race_spy_count_array, VERSION_1_MAX_RACE);
	WriteFloatArray(race_resistance_array, MAX_NATION*VERSION_1_MAX_RACE);
	WriteInt8Array(race_target_resistance_array, MAX_NATION*VERSION_1_MAX_RACE);
	WriteInt16(town_defender_count);
	WriteInt32(last_being_attacked_date);
	WriteFloat(received_hit_count);
	WriteInt8Array(train_queue_skill_array, MAX_TRAIN_QUEUE);
	WriteInt8Array(train_queue_race_array, MAX_TRAIN_QUEUE);
	WriteInt8(train_queue_count);
	WriteInt16(train_unit_recno);
	WriteInt32(train_unit_action_id);
	WriteInt32(start_train_frame_no);
	WriteInt16(defend_target_recno);
	WriteInt32(accumulated_collect_tax_penalty);
	WriteInt32(accumulated_reward_penalty);
	WriteInt32(accumulated_recruit_penalty);
	WriteInt32(accumulated_enemy_grant_penalty);
	WriteInt32(last_rebel_date);
	WriteInt16(independent_unit_join_nation_min_rating);
	WriteInt16(quality_of_life);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteInt8(town_combat_level);
	WriteInt8Array(has_product_supply, MAX_PRODUCT);
	WriteInt8(no_neighbor_space);
	WriteInt16(linked_firm_count);
	WriteInt16(linked_town_count);
	WriteInt16Array(linked_firm_array, MAX_LINKED_FIRM_TOWN);
	WriteInt16Array(linked_town_array, MAX_LINKED_TOWN_TOWN);
	WriteInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_TOWN);
	WriteInt8Array(linked_town_enable_array, MAX_LINKED_TOWN_TOWN);
}

void Version_1_Town::read_record(Version_1_TownGF *r)
{
	ReadInt16(town_recno);
	ReadInt16(town_name_id);
	ReadInt16(nation_recno);
	ReadInt16(rebel_recno);
	ReadInt8(race_id);
	ReadInt32(setup_date);
	ReadInt8(ai_town);
	ReadInt8(ai_link_checked);
	ReadInt8(independ_town_nation_relation);
	ReadInt8(has_linked_own_camp);
	ReadInt8(has_linked_enemy_camp);
	ReadInt8(is_base_town);
	ReadInt16(loc_x1);
	ReadInt16(loc_y1);
	ReadInt16(loc_x2);
	ReadInt16(loc_y2);
	ReadInt16(abs_x1);
	ReadInt16(abs_y1);
	ReadInt16(abs_x2);
	ReadInt16(abs_y2);
	ReadInt16(center_x);
	ReadInt16(center_y);
	ReadInt8(region_id);
	ReadInt16(layout_id);
	ReadInt16(first_slot_id);
	ReadInt16Array(slot_object_id_array, MAX_TOWN_LAYOUT_SLOT);
	ReadInt16(population);
	ReadInt16(jobless_population);
	ReadInt16Array(max_race_pop_array, VERSION_1_MAX_RACE);
	ReadInt16Array(race_pop_array, VERSION_1_MAX_RACE);
	ReadInt8Array(race_pop_growth_array, VERSION_1_MAX_RACE);
	ReadInt16Array(jobless_race_pop_array, VERSION_1_MAX_RACE);
	ReadFloatArray(race_loyalty_array, VERSION_1_MAX_RACE);
	ReadInt8Array(race_target_loyalty_array, VERSION_1_MAX_RACE);
	ReadInt16Array(race_spy_count_array, VERSION_1_MAX_RACE);
	ReadFloatArray(race_resistance_array, MAX_NATION*VERSION_1_MAX_RACE);
	ReadInt8Array(race_target_resistance_array, MAX_NATION*VERSION_1_MAX_RACE);
	ReadInt16(town_defender_count);
	ReadInt32(last_being_attacked_date);
	ReadFloat(received_hit_count);
	ReadInt8Array(train_queue_skill_array, MAX_TRAIN_QUEUE);
	ReadInt8Array(train_queue_race_array, MAX_TRAIN_QUEUE);
	ReadInt8(train_queue_count);
	ReadInt16(train_unit_recno);
	ReadInt32(train_unit_action_id);
	ReadInt32(start_train_frame_no);
	ReadInt16(defend_target_recno);
	ReadInt32(accumulated_collect_tax_penalty);
	ReadInt32(accumulated_reward_penalty);
	ReadInt32(accumulated_recruit_penalty);
	ReadInt32(accumulated_enemy_grant_penalty);
	ReadInt32(last_rebel_date);
	ReadInt16(independent_unit_join_nation_min_rating);
	ReadInt16(quality_of_life);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadInt8(town_combat_level);
	ReadInt8Array(has_product_supply, MAX_PRODUCT);
	ReadInt8(no_neighbor_space);
	ReadInt16(linked_firm_count);
	ReadInt16(linked_town_count);
	ReadInt16Array(linked_firm_array, MAX_LINKED_FIRM_TOWN);
	ReadInt16Array(linked_town_array, MAX_LINKED_TOWN_TOWN);
	ReadInt8Array(linked_firm_enable_array, MAX_LINKED_FIRM_TOWN);
	ReadInt8Array(linked_town_enable_array, MAX_LINKED_TOWN_TOWN);
}

void Tornado::write_record(TornadoGF *r)
{
	WriteZero(vtp); //zero

	// Sprite
	WriteInt16(sprite_id);
	WriteInt16(sprite_recno);
	WriteInt8(mobile_type);
	WriteInt8(cur_action);
	WriteInt8(cur_dir);
	WriteInt8(cur_frame);
	WriteInt8(cur_attack);
	WriteInt8(final_dir);
	WriteInt8(turn_delay);
	WriteInt8(guard_count);
	WriteInt8(remain_attack_delay);
	WriteInt8(remain_frames_per_step);
	WriteInt16(cur_x);
	WriteInt16(cur_y);
	WriteInt16(go_x);
	WriteInt16(go_y);
	WriteInt16(next_x);
	WriteInt16(next_y);
	WriteZero(sprite_info);

	// Tornado
	WriteFloat(attack_damage);
	WriteInt16(life_time);
	WriteInt16(dmg_offset_x);
	WriteInt16(dmg_offset_y);
}

void Tornado::read_record(TornadoGF *r)
{
	// skip vtp

	// Sprite
	ReadInt16(sprite_id);
	ReadInt16(sprite_recno);
	ReadInt8(mobile_type);
	ReadInt8(cur_action);
	ReadInt8(cur_dir);
	ReadInt8(cur_frame);
	ReadInt8(cur_attack);
	ReadInt8(final_dir);
	ReadInt8(turn_delay);
	ReadInt8(guard_count);
	ReadInt8(remain_attack_delay);
	ReadInt8(remain_frames_per_step);
	ReadInt16(cur_x);
	ReadInt16(cur_y);
	ReadInt16(go_x);
	ReadInt16(go_y);
	ReadInt16(next_x);
	ReadInt16(next_y);
	// skip sprite_info

	// Tornado
	ReadFloat(attack_damage);
	ReadInt16(life_time);
	ReadInt16(dmg_offset_x);
	ReadInt16(dmg_offset_y);
}

void Rebel::write_record(RebelGF *r)
{
	WriteInt16(rebel_recno);
	WriteInt16(leader_unit_recno);
	WriteInt8(action_mode);
	WriteInt16(action_para);
	WriteInt16(action_para2);
	WriteInt16(mobile_rebel_count);
	WriteInt16(town_recno);
	WriteInt8(hostile_nation_bits);
}

void Rebel::read_record(RebelGF *r)
{
	ReadInt16(rebel_recno);
	ReadInt16(leader_unit_recno);
	ReadInt8(action_mode);
	ReadInt16(action_para);
	ReadInt16(action_para2);
	ReadInt16(mobile_rebel_count);
	ReadInt16(town_recno);
	ReadInt8(hostile_nation_bits);
}

void SnowGroundArray::write_record(SnowGroundArrayGF *r)
{
	WriteInt32(seed);
	WriteInt32(snow_thick);
	WriteInt32(snow_pattern);
}

void SnowGroundArray::read_record(SnowGroundArrayGF *r)
{
	ReadInt32(seed);
	ReadInt32(snow_thick);
	ReadInt32(snow_pattern);
}

void RegionInfo::write_record(RegionInfoGF *r)
{
	WriteInt8(region_id);
	WriteInt8(region_stat_id);
	r->region_type = SDL_SwapLE32(region_type);
	WriteInt32(adj_offset_bit);
	WriteInt32(region_size);
	WriteInt16(center_x);
	WriteInt16(center_y);
}

void RegionInfo::read_record(RegionInfoGF *r)
{
	ReadInt8(region_id);
	ReadInt8(region_stat_id);
	region_type = (RegionType)SDL_SwapLE32(r->region_type);
	ReadInt32(adj_offset_bit);
	ReadInt32(region_size);
	ReadInt16(center_x);
	ReadInt16(center_y);
}

void RegionPath::write_record(RegionPathGF *r)
{
	WriteInt8(sea_region_id);
	WriteInt8(land_region_stat_id);
}

void RegionPath::read_record(RegionPathGF *r)
{
	ReadInt8(sea_region_id);
	ReadInt8(land_region_stat_id);
}

void RegionStat::write_record(RegionStatGF *r)
{
	WriteInt8(region_id);
	WriteInt8Array(nation_is_present_array, MAX_NATION);
	WriteInt8(nation_presence_count);
	WriteInt16Array(firm_type_count_array, MAX_FIRM_TYPE);
	WriteInt16Array(firm_nation_count_array, MAX_NATION);
	WriteInt16Array(camp_nation_count_array, MAX_NATION);
	WriteInt16Array(mine_nation_count_array, MAX_NATION);
	WriteInt16Array(harbor_nation_count_array, MAX_NATION);
	WriteInt16(total_firm_count);
	WriteInt16Array(town_nation_count_array, MAX_NATION);
	WriteInt16Array(base_town_nation_count_array, MAX_NATION);
	WriteInt16(independent_town_count);
	WriteInt16(total_town_count);
	WriteInt16Array(nation_population_array, MAX_NATION);
	WriteInt16Array(nation_jobless_population_array, MAX_NATION);
	WriteInt16Array(unit_nation_count_array, MAX_NATION);
	WriteInt16(independent_unit_count);
	WriteInt16(total_unit_count);
	WriteInt16(site_count);
	WriteInt16(raw_count);
	WriteCallArray(reachable_region_array, MAX_REACHABLE_REGION_PER_STAT);
	WriteInt8(reachable_region_count);
}

void RegionStat::read_record(RegionStatGF *r)
{
	ReadInt8(region_id);
	ReadInt8Array(nation_is_present_array, MAX_NATION);
	ReadInt8(nation_presence_count);
	ReadInt16Array(firm_type_count_array, MAX_FIRM_TYPE);
	ReadInt16Array(firm_nation_count_array, MAX_NATION);
	ReadInt16Array(camp_nation_count_array, MAX_NATION);
	ReadInt16Array(mine_nation_count_array, MAX_NATION);
	ReadInt16Array(harbor_nation_count_array, MAX_NATION);
	ReadInt16(total_firm_count);
	ReadInt16Array(town_nation_count_array, MAX_NATION);
	ReadInt16Array(base_town_nation_count_array, MAX_NATION);
	ReadInt16(independent_town_count);
	ReadInt16(total_town_count);
	ReadInt16Array(nation_population_array, MAX_NATION);
	ReadInt16Array(nation_jobless_population_array, MAX_NATION);
	ReadInt16Array(unit_nation_count_array, MAX_NATION);
	ReadInt16(independent_unit_count);
	ReadInt16(total_unit_count);
	ReadInt16(site_count);
	ReadInt16(raw_count);
	ReadCallArray(reachable_region_array, MAX_REACHABLE_REGION_PER_STAT);
	ReadInt8(reachable_region_count);
}

void RegionArray::write_record(RegionArrayGF *r)
{
	WriteInt32(init_flag);
	WriteZero(region_info_array);
	WriteInt32(region_info_count);
	WriteZero(region_stat_array);
	WriteInt32(region_stat_count);
	WriteZero(connect_bits);
	WriteInt8Array(region_sorted_array, MAX_REGION);
}

void RegionArray::read_record(RegionArrayGF *r)
{
	ReadInt32(init_flag);
	// skip region_info_array
	ReadInt32(region_info_count);
	// skip region_stat_array
	ReadInt32(region_stat_count);
	// skip connect_bits
	ReadInt8Array(region_sorted_array, MAX_REGION);
}

void News::write_record(NewsGF *r)
{
	WriteInt8(id);
	WriteInt8(type);
	WriteInt32(news_date);
	WriteInt8(nation_color1);
	WriteInt8(nation_color2);
	WriteInt8(nation_race_id1);
	WriteInt8(nation_race_id2);
	WriteInt32(nation_name_id1);
	WriteInt32(nation_name_id2);
	WriteInt16(short_para1);
	WriteInt16(short_para2);
	WriteInt16(short_para3);
	WriteInt16(short_para4);
	WriteInt16(short_para5);
	WriteInt8(loc_type);
	WriteInt16(loc_type_para);
	WriteInt16(loc_type_para2);
	WriteInt16(loc_x);
	WriteInt16(loc_y);
}

void News::read_record(NewsGF *r)
{
	ReadInt8(id);
	ReadInt8(type);
	ReadInt32(news_date);
	ReadInt8(nation_color1);
	ReadInt8(nation_color2);
	ReadInt8(nation_race_id1);
	ReadInt8(nation_race_id2);
	ReadInt32(nation_name_id1);
	ReadInt32(nation_name_id2);
	ReadInt16(short_para1);
	ReadInt16(short_para2);
	ReadInt16(short_para3);
	ReadInt16(short_para4);
	ReadInt16(short_para5);
	ReadInt8(loc_type);
	ReadInt16(loc_type_para);
	ReadInt16(loc_type_para2);
	ReadInt16(loc_x);
	ReadInt16(loc_y);
}
