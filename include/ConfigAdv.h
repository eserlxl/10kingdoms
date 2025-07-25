/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Copyright 2019 Jesse Allen
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

// Filename : ConfigAdv.h
// Description : Advanced Config

#ifndef __CONFIGADV_H
#define __CONFIGADV_H

#include <GAMEDEF.h>
#include <stdint.h>

class ConfigAdv
{
public:
	enum {
		FLAG_DEBUG_VER = 1,
		FLAG_DEVEL_VER = 2,
		FLAG_CKSUM_REQ = 4,
		FLAG_UNKNOWN_BUILD = 8,
	};

	enum {
		LOCALE_LEN = 40,
	};

	uint32_t		checksum;
	uint32_t		flags;

	// firm settings
	char			firm_ai_enable_think_spy_capture;
	char			firm_mobilize_civilian_aggressive;
	char			firm_migrate_stricter_rules;

	// bug fix settings
	char			fix_ai_consider_trade_treaty;
	char			fix_path_blocked_by_team;
	char			fix_recruit_dec_loyalty;
	char			fix_sea_travel_final_move;
	char			fix_town_unjob_worker;
	int			fix_world_warp_slop;

	// game settings
	int			game_load_default_frame_speed;
	int			game_new_default_frame_speed;

	// game file settings
	char			game_file_patching;

	// locale settings
	char			locale[LOCALE_LEN+1];

	// mine settings
	char			mine_unlimited_reserve;

	// monster settings
	char			monster_alternate_attack_curve;
	int			monster_attack_divisor;

	// nation settings
	char			nation_ai_defeat_when_no_towns;
	char			nation_ai_no_treaty_with_biggest;
	char			nation_ai_unite_min_relation_level;
	int			nation_start_god_level;
	int			nation_start_tech_inc_all_level;

	// news settings
	char			news_notify_complete;

	// race settings
	char			race_random_list[MAX_RACE];
	int			race_random_list_max;
	char			race_create_all;

	// remote settings
	char		        remote_compare_object_crc;
	char			remote_compare_random_seed;

	// scenario settings
	char			scenario_config;

	// town settings
	int			town_ai_emerge_nation_pop_limit;
	int			town_ai_emerge_town_pop_limit;
	char			town_migration;
	char			town_loyalty_qol;

	// unit settings
	char			unit_ai_team_help;
	char			unit_finish_attack_move;
	char			unit_loyalty_require_local_leader;
	char			unit_allow_path_power_mode;
	char			unit_spy_fixed_target_loyalty;
	char			unit_target_move_range_cycle;

	// vga settings
	char			vga_allow_highdpi;
	char			vga_full_screen;
	char			vga_full_screen_desktop;
	char			vga_keep_aspect_ratio;
	char			vga_pause_on_focus_loss;

	int			vga_window_width;
	int			vga_window_height;

	// wall settings
	char			wall_building_allowed;

public:
	ConfigAdv();
	~ConfigAdv();

	int			init();
	int			load(char *filename);
	void			reset();
	int			set(char *name, char *value);

private:
	void			update_check_sum(char *name, char *value);
};

//------------------------------------------//

extern ConfigAdv config_adv;

#endif
