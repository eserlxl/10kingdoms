#include "OGFILE.h"
#include "OGAME.h"
#include "OSYS.h"
Sys sys;
#include "ConfigAdv.h"
#include "PlayerStats.h"
#include "OGAMHALL.h"
#include "OOPTMENU.h"
#include "OINGMENU.h"
#include "CmdLine.h"
#include "OWEATHER.h"
#include "OPOWER.h"
#include "OWORLD.h"
#include "OGAMESET.h"
#include "OBATTLE.h"
#include "FilePath.h"
#include "OCONFIG.h"
#include "OOPTMENU.h"
#include "OINGMENU.h"
#include "OGAMHALL.h"
#include "LocaleRes.h"
#include "file_util.h" // For File
#include "dbglog.h"

Weather           weather, weather_forecast[MAX_WEATHER_FORECAST];
MagicWeather      magic_weather;
Config            config;
Game              game;
GameSet           game_set;         // no constructor
Battle            battle;
Power             power;
World             world;
char              scenario_file_name[FilePath::MAX_FILE_PATH+1];
GameFileArray     game_file_array;
GameFile          game_file;
nsPlayerStats::PlayerStats playerStats;
HallOfFame        hall_of_fame;
OptionMenu        option_menu;
InGameMenu        in_game_menu;
CmdLine           cmd_line;
ConfigAdv         config_adv;

//----------- Global Variables -----------//

char game_design_mode=0;
char game_demo_mode=0;
char debug2_enable_flag=0;
File seedCompareFile;
char debug_seed_status_flag=0;
int  debug_sim_game_type = 0;
int  unit_search_node_used=0;
short nation_hand_over_flag=0;
int     unit_search_tries = 0;        // the number of tries used in the current searching
char    unit_search_tries_flag = 0;   // indicate num of tries is set, reset after searching

char  new_config_dat_flag=0; 