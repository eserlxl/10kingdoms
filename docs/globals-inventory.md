# Global Variables Inventory

Generated: Sun Jan 18 10:05:32 +03 2026

## globals.cpp

  - `Sys sys;`
  - `MagicWeather      magic_weather;`
  - `Config            config;`
  - `Game              game;`
  - `GameSet           game_set;         // no constructor`
  - `Battle            battle;`
  - `Power             power;`
  - `World             world;`
  - `GameFileArray     game_file_array;`
  - `GameFile          game_file;`
  - `HallOfFame        hall_of_fame;`
  - `OptionMenu        option_menu;`
  - `InGameMenu        in_game_menu;`
  - `CmdLine           cmd_line;`
  - `ConfigAdv         config_adv;`
  - `char game_design_mode=0;`
  - `char game_demo_mode=0;`
  - `char debug2_enable_flag=0;`
  - `File seedCompareFile;`
  - `char debug_seed_status_flag=0;`
  - `int  debug_sim_game_type = 0;`
  - `int  unit_search_node_used=0;`
  - `short nation_hand_over_flag=0;`
  - `int     unit_search_tries = 0;        // the number of tries used in the current searching`
  - `char    unit_search_tries_flag = 0;   // indicate num of tries is set, reset after searching`
  - `char  new_config_dat_flag=0; `

## Global Variables in Source Files

### globals.cpp
  - `Sys sys;`
  - `MagicWeather      magic_weather;`
  - `Config            config;`
  - `Game              game;`
  - `GameSet           game_set;         // no constructor`

### OBUTTCUS.cpp
  - `ButtonCustom customButton1;`

### OSE.cpp
  - `const default_vol_limit = 20;`
  - `const default_vol_drop = 100;`
  - `const default_pan_drop = 100;`

### AM.cpp
  - `Error             err;              // constructor only call set_new_handler()d`
  - `Mouse             mouse;`
  - `MouseCursor       mouse_cursor;`
  - `DateInfo          date;`
  - `Vga               vga;`

### PlayerStats.cpp
  - `extern Sys sys;			// For the config directory`

## Singleton Patterns


## Static Variables

### dbglog.cpp
  - `static struct __dbglog_channel dbglog_settings[DEBUG_LOG_MAX_CHANNELS];`
  - `static int debug_log_initialized = 0;`

### OU_GOD.cpp
  - `static Button3D button_cast, button_cast2;`

### OREMOTE2.cpp
  - `static int validate_queue(char* queueBuf, int queuedSize)`

### OHELP.cpp
  - `	static String str;`

### OBULLET.cpp
  - `static char spiral_x[SCAN_RANGE*SCAN_RANGE] = `
  - `static char spiral_y[SCAN_RANGE*SCAN_RANGE] = `

### OFIRMIF.cpp
  - `static Button3D button_sell, button_destruct, button_builder, button_request_builder;`
  - `static short  	 pop_disp_y1;`
  - `static char     worker_id_array[MAX_WORKER];`
  - `static Firm*    cur_firm_ptr;`
  - `static void disp_worker_hit_points(int x1, int y1, int x2, int hitPoints, int maxHitPoints);`
  - `static int sort_worker_id_function(const void *a, const void *b);`
  - `	static char  lastUnderConstruction;`
  - `	static short lastFirmRecno;`
  - `	static char  lastShouldShowInfo;`
  - `	static short lastPlayerSpyCount;`

### OSYS3.cpp
  - `static long	random_seed_backup_table[MAX_SEED_TABLE_SIZE];`
  - `static int	random_seed_writen_pos;`
  - `static int	random_seed_backup_pos;`
  - `static int	random_seed_backup_table_data_size;`
  - `static char file_opened_flag = 0;`
  - `static long	match_seed;`

### OERROR.cpp
  - `static void new_func_handler();`
  - `static char error_flag=0;		// prevent error message dead loop`
  - `static void new_func_handler()`

### OR_RANK.cpp
  - `static VBrowseIF 	browse_nation;`
  - `static int		 	nation_rank_data_array[MAX_RANK_TYPE][MAX_NATION];`
  - `static void  put_nation_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int   nation_filter(int recNo=0);`
  - `static void  disp_score();`
  - `static void	 disp_goal();`
  - `static void	 disp_play_time(int y1);`
  - `static void put_heading(char justify, int x1, int y1, int x2, int y2, const char *textPtr);`
  - `static void disp_score()`
  - `	static const char* rankStrArray[] =`

### OR_TECH.cpp
  - `static VBrowseIF browse_tech, browse_god;`
  - `static int  tech_filter(int recNo=0);`
  - `static int  god_filter(int recNo=0);`
  - `static void put_tech_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_god_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void disp_owned_scroll();`
  - `static void disp_scroll(int x, int y, int raceId);`
  - `static void put_heading(char justify, int x1, int y1, int x2, int y2, const char *textPtr);`
  - `static int tech_filter(int recNo)`
  - `static int god_filter(int recNo)`

### OF_CAMP.cpp
  - `static Button3D button_patrol, button_reward, button_defense;`
  - `static void disp_debug_info(FirmCamp* firmPtr, int refreshFlag);`
  - `static void disp_debug_info(FirmCamp* firmPtr, int refreshFlag)`

### LocaleRes.cpp
  - `static String lc_all_str;`
  - `static String lc_name_str;`

### OFIRMA.cpp
  - `static unsigned long	last_firm_ai_profile_time = 0L;`
  - `static unsigned long	firm_ai_profile_time = 0L;`
  - `static unsigned long	last_firm_profile_time = 0L;`
  - `static unsigned long	firm_profile_time = 0L;`
  - `	static unsigned long lastDrawTime = misc.get_time();`

### OERRCTRL.cpp
  - `static String debugStr;`
  - `	static int simError = 1;`

### OWORLD.cpp
  - `static char northPierTera[3][3] = { {2,2,2},{2,2,2},{3,1,3} };`
  - `static char southPierTera[3][3] = { {3,1,3},{2,2,2},{2,2,2} };`
  - `static char westPierTera[3][3] = { {2,2,3},{2,2,1},{2,2,3} };`
  - `static char eastPierTera[3][3] = { {3,2,2},{1,2,2},{3,2,2} };`

### CmdLine.cpp
  - `static int have_arg(int i, int argc, const char *option)`
  - `static int set_startup_mode(StartupMode mode)`

### OUNITI.cpp
  - `static char		idle_detect_has_unit;`
  - `static char		idle_detect_has_firm;`
  - `static char		idle_detect_has_town;`
  - `static char		idle_detect_has_wall;`
  - `static short	idle_detect_target_unit_recno;`
  - `static short	idle_detect_target_firm_recno;`
  - `static short	idle_detect_target_town_recno;`
  - `static short	idle_detect_target_wall_x1;`
  - `static short	idle_detect_target_wall_y1;`
  - `static int		idle_detect_default_mode;`

### OPOWER.cpp
  - `static short nation_divide_order[MAX_NATION+1] = {1, 2, 3, 4, 5, 6, 7, 0};`
  - `static bool is_unit_higher_rank(int recno1, int recno2)`
  - `static void divide_by_nation(short *nationUnitCount, short *selectedArray, int selectedCount)`
  - `	static short nationUnitCount[MAX_NATION+1]; // plus one for independent nation`
  - `		static char mobileTypeList[mobileTypeCount] = { UNIT_AIR, UNIT_LAND, UNIT_SEA };`

### OF_CAMP2.cpp
  - `	static short interval_days_array[] = { 60, 30, 20, 10 };`

### OTUTOR.cpp
  - `static Button button_new_tutor, button_quit_tutor;`
  - `static Button button_restart, button_prev, button_next;`
  - `static Button3D button_sample;`
  - `static int text_start_line, text_disp_lines, text_max_lines;`
  - `static Button3D textScrollUp, textScrollDown;`

### OSPRITEA.cpp
  - `static int num_of_unit;`

### OGFILE3.cpp
  - `static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize);`
  - `static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize);`
  - `static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize)`
  - `static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize)`

### OF_MINE.cpp
  - `static Button3D	button_vacate_firm;`

### OSERES.cpp
  - `static int seinfo_cmp(const void *r1, const void *r2)`

### OMP_CRC.cpp
  - `static union`

### OW_WALL.cpp
  - `	static int init_build_wall_seq = 0;`
  - `	static char build_wall_x_seq[SPACING*SPACING];`
  - `	static char build_wall_y_seq[SPACING*SPACING];`
  - `	static int	next_build_wall_seq;`

### OBOX.cpp
  - `   static char resultStr[RESULT_STR_LEN+1];`

### OGAMSCEN.cpp
  - `static void init_scenario_var(ScenInfo* scenInfo);`
  - `static int sort_scenario_func(const void *arg1, const void *arg2);`
  - `static void init_scenario_var(ScenInfo* scenInfo)`

### ONEWS.cpp
  - `static Font* news_font_ptr = &font_news;`

### OVGABUF2.cpp
  - `	static char tempBuffer[TEMP_BUFFER_SIZE];`
  - `	static unsigned char lineColor[8] = `

### ORACERES.cpp
  - `	static String str;`

### OR_TRADE.cpp
  - `static VBrowseIF browse_caravan, browse_ship, browse_firm;`
  - `static char mode_unit, mode_firm;`
  - `static short selected_unit_recno, selected_harbor_recno, unit_x, unit_y, browse_firm_recno, idle_caravans, idle_firms;`
  - `static Button button_copy;`
  - `static const char* firm_mode_str_array[MAX_FIRM_REPORT_MODE] =`
  - `static const char* unit_mode_str_array[MAX_UNIT_REPORT_MODE] =`
  - `static int  is_caravan_route_idle(UnitCaravan* unitPtr);`
  - `static int  is_firm_idle(Firm* firmPtr);`
  - `static void create_caravan_list();`
  - `static void create_firm_list();`

### OGFILE2.cpp
  - `static int loaded_random_seed;`
  - `static int dynarray_short_write_file(File* filePtr, DynArray* a)`
  - `static int dynarray_short_read_file(File* filePtr, DynArray* a)`

### OTALKRES.cpp
  - `static String nation_name_str_array[MAX_NATION];`
  - `static char* select_nation_color(char nation_color);`
  - `	static const char* talkMsgArray[] =`
  - `		static const char* qtyStrArray[] = { "500.", "1000.", "2000.", "4000." };`
  - `		static short qtyArray[] = { 500, 1000, 2000, 4000 };`
  - `		static const char* priceStrArray[] = { "$5.", "$10.", "$15.", "$20." };`
  - `		static short priceArray[] = { 5, 10, 15, 20 };`
  - `	static const char* tributeStrArray[] = { "$500.", "$1000.", "$2000.", "$3000.", "$4000." };`
  - `	static short tributeAmtArray[] = { 500, 1000, 2000, 3000, 4000 };`
  - `		static const char* verStrArray[] = { "Mark I", "Mark II", "Mark III" };`

### OMISC.cpp
  - `static char		move_around_table_x[MOVE_AROUND_TABLE_SIZE] = {0};`
  - `static char		move_around_table_y[MOVE_AROUND_TABLE_SIZE] = {0};`
  - `static short	move_around_table_size = 0;`
  - `   static char outBuf[35];`
  - `   static char longBuf[25];`
  - `   static char outBuf[35];`
  - `   static char strBuf[25];`
  - `	static unsigned char multi_to_win_table[] = `
  - `   static String str;`
  - `	static long int bitFlag[20] = {0x000001, 0x000002, 0x000004, 0x000008, 0x000010, 0x000020, 0x000040, 0x000080,`

### OU_MARIT.cpp
  - `static char		mprocessed_raw_qty_array[MAX_LINKED_FIRM_FIRM][MAX_RAW];	// 1 for not unload but can up load, 2 for unload but not up load`
  - `static char		mprocessed_product_raw_qty_array[MAX_LINKED_FIRM_FIRM][MAX_PRODUCT];	// ditto`
  - `static char		linked_mine_num;`
  - `static char		linked_factory_num;`
  - `static char		linked_market_num;`
  - `static short	linked_mine_array[MAX_LINKED_FIRM_FIRM];`
  - `static short	linked_factory_array[MAX_LINKED_FIRM_FIRM];`
  - `static short	linked_market_array[MAX_LINKED_FIRM_FIRM];`
  - `static char		empty_slot_position_array[MAX_LINKED_FIRM_FIRM];`
  - `static char		firm_selected_array[MAX_LINKED_FIRM_FIRM];`

### OAI_GRAN.cpp
  - `			static short aidAmountArray[] = { 500, 1000, 2000 };`
  - `			static short aidAmountArray[] = { 500, 1000, 2000 };`

### OSPREDBG.cpp
  - `static ResultNode*	debugSrePtr; // for debug only`
  - `static ResultNode*	debugSreNode1;`
  - `static ResultNode*	debugSreNode2;`
  - `static int				debugSreCount;`
  - `static int				debugSreVX, debugSreVY;	 // for debug only`

### OR_NAT.cpp
  - `static const char* report_mode_str_array[MAX_NATION_REPORT_MODE] =`
  - `static const char* chat_receiver_str_array[MAX_CHAT_RECEIVER_TYPE] =`
  - `static VBrowseIF 	 browse_nation, browse_talk_msg;`
  - `static ButtonGroup button_allow_attack(2);`
  - `static ButtonGroup button_chat_receiver(MAX_CHAT_RECEIVER_TYPE);`
  - `static int init_get_chat = 0;`
  - `static GetA 		 get_chat;`
  - `static void put_nation_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_talk_msg_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int  nation_filter(int recNo=0);`

### OOPTMENU.cpp
  - `static char race_table[MAX_RACE_TABLE] =		// race translation table`
  - `static char reverse_race_table[MAX_RACE_TABLE] =		// race translation table`
  - `static void disp_virtual_button(ButtonCustom *button, int);`
  - `static void disp_slide_bar(SlideBar *slideBar, int);`
  - `static void disp_virtual_button(ButtonCustom *button, int)`
  - `static void disp_slide_bar(SlideBar *slideBar, int)`

### OGAMEND.cpp
  - `static int  disp_score(int winFlag);`
  - `static void disp_goal_str(int winNationRecno);`
  - `static void disp_losing_str(int surrenderToNationRecno);`
  - `static void disp_retire_str();`
  - `static void disp_ranking();`
  - `static void disp_stat();`
  - `static void put_stat(int y, const char* desStr, const char* dispStr);`
  - `static void put_stat(int y, const char* desStr, int dispValue);`
  - `static void put_ranking(int y, int nationRecno);`
  - `static void put_heading(char justify, int x1, int y1, int x2, int y2, const char *textPtr);`

### OGAMMENU.cpp
  - `	static int accSectionWeight[MAX_SECTION+1] = { 0, 60, 100 };`

### ODATE.cpp
  - `static int month_tot[]=`
  - `   static char strBuf[16];`
  - `   static String str;`
  - `	static char strBuf[6] = "00:00";`

### OTALKMSG.cpp
  - `static char talk_msg_reply_needed_array[] =`
  - `    static String str;`
  - `    static String str;`
  - `    static String str;`
  - `    static String str;`
  - `    static String str;`
  - `    static char colorCodeStr[] = " @COL0";`
  - `	static char colorCodeStr[] = " @COL0";`

### OSTR.cpp
  - `static char work_buf[MAX_STR_LEN+1];`

### OR_AI.cpp
  - `static const char* action_mode_str_array[] =`
  - `static VBrowseIF browse_action, browse_attack;`
  - `static int		  total_population, total_peasant;`
  - `static Nation*	  cur_nation_ptr;`
  - `static void put_action_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_attack_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void disp_other_info();`
  - `static void put_action_rec(int recNo, int x, int y, int refreshFlag)`
  - `static void put_attack_rec(int recNo, int x, int y, int refreshFlag)`
  - `static void disp_other_info()`

### OTOWNIF.cpp
  - `static VBrowseIF  browse_race, browse_spy;`
  - `static Button3D   button_recruit, button_train, button_tax, button_grant;`
  - `static Button3D   button_spy, button_cancel, button_spy_mobilize,`
  - `static Button3D	button_cancel_training;`
  - `static ButtonCustom  button_cancel3;`
  - `static Button		button_loyalty_array[BUTTON_LOYALTY_COUNT];`
  - `static Button		button_loyalty_disabled;`
  - `static Button		button_cancel2;`
  - `static ButtonCustom button_skill[MAX_TRAINABLE_SKILL];`
  - `static ButtonCustom button_queue_skill[MAX_TRAINABLE_SKILL];`

### OFONT.cpp
  - `static int text_line_count;     // used by text_width() & text_height() only`

### OTOWN.cpp
  - `static char random_race();`
  - `	static int lastLayoutId=0;`
  - `static char random_race()`

### OTALKRE2.cpp
  - `static void delete_msg_in_reverse(TalkMsg *talkMsg);`
  - `static void delete_msg_in_reverse(TalkMsg *talkMsg)`

### OAI_MAIN.cpp
  - `	static short intervalDaysArray[] = { 90, 30, 15, 15 };`

### OGAMCRED.cpp
  - `static void disp_credits_2();`
  - `static void disp_credits_3();`
  - `static void disp_credits_4();`
  - `static void disp_credits_5();`
  - `static void disp_credits_6();`
  - `static void disp_credits(int y, const char** creditNameArray);`
  - `static void disp_credits_2()`
  - `	static char* credit_des_array[] =`
  - `	static char* credit_name_array[] =`
  - `static void disp_credits_3()`

### OFLAME.cpp
  - `	static unsigned char lastShadeBase = 0;`
  - `	static unsigned char colorTable[256];`

### OTALKFRE.cpp
  - `static String str;						// these vars don't have to be saved as their are only used by msg_str() for passing vars to other functions`
  - `static short  viewing_nation_recno;`
  - `static char   should_disp_reply;`
  - `static char   disp_second_line;`

### OGAME.cpp
  - `	static ColorRemapMethod remap_method_array[] =`
  - `   static int main_color_array[] =`

### OR_TOWN.cpp
  - `static VBrowseIF browse_town, browse_firm;`
  - `static int		  total_population, total_peasant;`
  - `static int  	  firm_income_array[MAX_FIRM_TYPE];`
  - `static int  	  total_firm_cost, total_firm_income, total_firm_count;`
  - `static float	  total_expense;`
  - `static void put_town_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_firm_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int  town_filter(int recNo=0);`
  - `static int  firm_filter(int recNo=0);`
  - `static void	disp_total();`

### OR_ECO.cpp
  - `static VBrowseIF browse_income, browse_expense;`
  - `static void put_income_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_expense_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void disp_total();`
  - `static void disp_total()`
  - `static void put_income_rec(int recNo, int x, int y, int refreshFlag)`
  - `	static const char* income_des_array[INCOME_TYPE_COUNT] =`
  - `static void put_expense_rec(int recNo, int x, int y, int refreshFlag)`
  - `	static const char* expense_des_array[EXPENSE_TYPE_COUNT] =`

### misc_uuid.cpp
  - `static void gettimeofday (struct timeval *tv, void *dummy)`
  - `static int getuid (void)`
  - `static int get_random_fd(void)`
  - `	static int	fd = -2;`
  - `static void uuid_pack(const struct uuid *uu, guuid_t ptr)`
  - `static void uuid_unpack(const guuid_t in, struct uuid *uu)`
  - `static const char *fmt_lower =`
  - `static const char *fmt_upper =`
  - `static void uuid_unparse_x(const guuid_t uu, char *out, const char *fmt)`

### OAI_TALK.cpp
  - `static int has_sent_same_msg(TalkMsg* talkMsg);`
  - `static int has_sent_same_msg(TalkMsg* talkMsgPtr)`

### OAI_DIPL.cpp
  - `	static short buyQtyArray[] = { 500, 1000, 2000, 4000 };`
  - `	static short tributeAmountArray[] = { 500, 1000 };`
  - `	static int amtArray[] = { 5000, 7500, 10000, 15000, 20000, 30000, 40000, 50000 };`

### OUNITAM.cpp
  - `static short		square_size, not_tested_loc, rec_height, rec_width;`
  - `static int			move_scale; // use integer for intrgral division`
  - `static int			x, y;`
  - `static short		lower_right_case, upper_left_case;	// indicate which case should be used`
  - `static int			*distance, *sorted_member, *sorted_distance;`
  - `static char			*done_flag;`
  - `static short		*filtering_unit_array;`
  - `static int			filtering_unit_count;`
  - `static short		*filtered_unit_array;`
  - `static int			filtered_unit_count;`

### OUNITATB.cpp
  - `static char attack_x_offset11[ATTACK_DIR][8] // target size 1x1`
  - `static char attack_x_offset12[ATTACK_DIR][10] // target size 1x2`
  - `static char attack_x_offset13[ATTACK_DIR][12] // target size 1x3`
  - `static char attack_x_offset14[ATTACK_DIR][14] // target size 1x4`
  - `static char attack_x_offset21[ATTACK_DIR][10] // target size 2x1`
  - `static char attack_x_offset22[ATTACK_DIR][12] // target size 2x2`
  - `static char attack_x_offset23[ATTACK_DIR][14] // target size 2x3`
  - `static char attack_x_offset24[ATTACK_DIR][16] // target size 2x4`
  - `static char attack_x_offset31[ATTACK_DIR][12] // target size 3x1`
  - `static char attack_x_offset32[ATTACK_DIR][14] // target size 3x2`

### OTUTOR2.cpp
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int);`
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int)`

### OR_NEWS.cpp
  - `static VBrowseIF browse_news;`
  - `static void put_news_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_news_rec(int recNo, int x, int y, int refreshFlag)`
  - `	static int maxDateWidth = 90;`

### OBUTT3D.cpp
  - `static char save_back_buf[BUTTON_ACTION_WIDTH * BUTTON_ACTION_HEIGHT + 4];`

### OWORLD_M.cpp
  - `	static int squareFrameCount=0, squareFrameStep=1;`

### OWARPT.cpp
  - `	static unsigned char dotColor[DRAW_PHASE_PERIOD/2] = `

### OBATTLE.cpp
  - `static int is_space(int xLoc1, int yLoc1, int xLoc2, int yLoc2, char mobileType);`
  - `static char random_race();`
  - `static char random_race_time();`
  - `static int is_space(int xLoc1, int yLoc1, int xLoc2, int yLoc2, char mobileType)`
  - `static char random_race()`
  - `static char random_race_time()`

### OF_WAR.cpp
  - `static Button3D button_select_build;`
  - `static Button3D	button_cancel_build;`
  - `static char     war_menu_mode;`
  - `static char     disable_refresh=0;`
  - `static short	 added_count;`
  - `static short	 button_unit_id[MAX_WEAPON_TYPE];`
  - `static ButtonCustom button_weapon[MAX_WEAPON_TYPE];`
  - `static ButtonCustom button_queue_weapon[MAX_WEAPON_TYPE];`
  - `static int queue_weapon_selected = -1;`
  - `static ButtonCustom button_cancel;`

### multiplayer.cpp
  - `static int sort_players(const void *a, const void *b)`
  - `	static unsigned long last_broadcast_time;`
  - `	static int poll_time;`
  - `static int sort_session_id(const void *a, const void *b)`
  - `static int sort_session_name(const void *a, const void *b)`

### OFIRMIF2.cpp
  - `static VBrowseIF  browse_spy;`
  - `static Button3D   button_spy_menu, button_spy_mobilize, button_spy_action, button_spy_reward,`
  - `static Firm*      firm_ptr;`
  - `static int  spy_filter(int recNo=0);`
  - `static void put_spy_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int  get_player_spy_recno(int firmRecno);`
  - `	static int lastSpyCount;`
  - `static int spy_filter(int recNo)`
  - `static void put_spy_rec(int recNo, int x, int y, int refreshFlag)`
  - `static int get_player_spy_recno(int firmRecno)`

### OMUSIC.cpp
  - `static const char *music_file[] =`
  - `static const char *music_file[] =`

### AM.cpp
  - `static void signal_handler(int signal)`
  - `static void extra_error_handler();`
  - `static void extra_error_handler()`

### OPLASMA.cpp
  - `   static struct sub subx, suby;`
  - `	static short min_sea_count_array[] = { 1600, 700, 200 };`

### OUNITAT2.cpp
  - `static void invalidate_attack_target(Unit *unitPtr);`
  - `static void invalidate_attack_target(Unit *unitPtr)`

### OLZW.cpp
  - `static unsigned short BITS_MASK[] = `

### OGAMEMP.cpp
  - `static char race_table[MAX_RACE] =		// race translation table`
  - `static char reverse_race_table[MAX_RACE] =		// race translation table`
  - `static char sub_game_mode;		// 0 = new multiplayer game, 1 = load multiplayer game`
  - `static void disp_virtual_button(ButtonCustom *, int);`
  - `static void disp_virtual_tick(ButtonCustom *, int);`
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int);`
  - `static void pregame_disconnect_handler(uint32_t playerId);`
  - `static void ingame_disconnect_handler(uint32_t playerId);`
  - `static void pregame_disconnect_handler(uint32_t playerId)`
  - `static void ingame_disconnect_handler(uint32_t playerId)`

### OU_MARIS.cpp
  - `	static char dummyBuffer[MAX_GOODS_SELECT_BUTTON];`

### OFIRMDRW.cpp
  - `static Point slot_point_array[] =`

### OGAMSCE2.cpp
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int);`
  - `static void draw_checkbox(int x, int y, CHECKBOX_STATE checked);`
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int)`

### OF_MONS.cpp
  - `static char current_monster_action_mode;`
  - `static int fryhtan_attacks_per_six_months(int numOfLairs);`
  - `static int fryhtan_random_attack();`
  - `static void disp_worker_hit_points(int x1, int y1, int x2, int hitPoints, int maxHitPoints);`
  - `	static String str;`
  - `static int fryhtan_attacks_per_six_months(int numOfLairs)`
  - `static int fryhtan_random_attack()`
  - `static void disp_worker_hit_points(int x1, int y1, int x2, int hitPoints, int maxHitPoints)`

### ONATIONA.cpp
  - `static unsigned long	last_nation_ai_profile_time = 0L;`
  - `static unsigned long	nation_ai_profile_time = 0L;`
  - `static unsigned long	last_nation_profile_time = 0L;`
  - `static unsigned long	nation_profile_time = 0L;`
  - `	static char humanNameOneWord[HUMAN_NAME_LEN+1];`
  - `	static unsigned long lastDrawTime = misc.get_time();`

### OTRANSL.cpp
  - `static int sort_translate_table( const void *a, const void *b );`
  - `static int sort_translate_table( const void *a, const void *b )`
  - `	static unsigned char multi_to_win_table[] = `

### ONATIONB.cpp
  - `static int succeed_king_loyalty_change(int thisRaceId, int newKingRaceId, int oldKingRaceId);`
  - `	static int start_up_cash_array[] = { 4000, 7000, 12000, 20000 };`
  - `	static String str;`
  - `	static String str;`
  - `static int succeed_king_loyalty_change(int thisRaceId, int newKingRaceId, int oldKingRaceId)`
  - `	static String str;`
  - `	static String str;`

### OSYS.cpp
  - `static void test_lzw();`
  - `static void locate_king_general(int rankId);`
  - `static void locate_spy();`
  - `static void locate_ship();`
  - `static void locate_camp();`
  - `static int  locate_ship_in_harbor();`
  - `static int  locate_visible_ship();`
  - `static int  detect_scenario_cheat_key(unsigned scanCode, unsigned skeyState);`
  - `static int  get_mouse_loc_in_zoom_map(int &x, int &y);`
  - `static char random_race();`

### ODIR.cpp
  - `static int sort_file_function( const void *a, const void *b );`
  - `static int sort_file_function( const void *a, const void *b )`

### OGENMAP.cpp
  - `	static TerrainTypeCode ta[6] = { TERRAIN_OCEAN,TERRAIN_OCEAN,TERRAIN_OCEAN,`
  - `	static SubTerrainMask sta[6] = { BOTTOM_MASK, MIDDLE_MASK, TOP_MASK,`
  - `static RegionType walkable;					// to save stack space`
  - `static unsigned char regionId;`

### OSPATH.cpp
  - `static Location*  world_loc_matrix;`
  - `static int		   cur_stack_pos=0;`
  - `static Node* 	   stack_array[MAX_STACK_NUM];`
  - `static uint32_t	   group_id;`
  - `static short	   search_mode;`
  - `static char	  	   mobile_type;`
  - `static char			seek_nation_recno;`
  - `static int			attack_range;	// used in search_mode = SEARCH_MODE_ATTACK_UNIT_BY_RANGE`
  - `static short		target_recno;	// used in search_mode = SEARCH_MODE_TO_ATTACK or SEARCH_MODE_TO_VEHICLE, get from miscNo`
  - `static uint8_t		region_id;		// used in search_mode = SEARCH_MODE_TO_LAND_FOR_SHIP`

### OSPYA.cpp
  - `static const char* 	secret_report_str_array[] = { N_("Kingdoms"), N_("Villages"), N_("Economy"), N_("Trade"), N_("Military"), N_("Technology"), N_("Espionage") };`
  - `static char	 	secret_view_mode_array[]  = { MODE_NATION, MODE_TOWN, MODE_ECONOMY, MODE_TRADE, MODE_MILITARY, MODE_TECH, MODE_SPY };`
  - `static char	 	secret_view_skill_array[] = { 40, 20, 30, 30, 50, 40, 90 };`
  - `static Button	button_secret_report_array[SECRET_REPORT_COUNT];`
  - `static Button	button_secret_report_cancel;`

### OGF_REC.cpp
  - `static void copy_float_array(float *d, float *s, size_t n)`

### OU_CARAT.cpp
  - `static char		 processed_raw_qty_array[MAX_RAW];					// 1 for not unload but can up load, 2 for unload but not up load`
  - `static char		 processed_product_raw_qty_array[MAX_PRODUCT];	// ditto`

### OF_MARK.cpp
  - `static Point section_point_array[] =`
  - `static Point slot_point_array[] =`
  - `static Button3D button_hire_caravan;`
  - `static Button 	 button_clear_stock[MAX_MARKET_GOODS];`
  - `static Button   button_switch_restock;`
  - `	static char lastNoTrade;`
  - `	static char* 	last_bitmap_array[MAX_MARKET_GOODS];`

### OUNIT.cpp
  - `   static String str;`

### OU_CARAS.cpp
  - `	static char dummyBuffer[MAX_GOODS_SELECT_BUTTON];`

### OREGION.cpp
  - `static int sort_region_function( const void *a, const void *b );`
  - `static int sort_region_function( const void *a, const void *b )`

### OINGMENU.cpp
  - `static const char *land_mass_msg[] =`

### OUNITA.cpp
  - `static void put_profile(int y, const char* dispDes, int dispValue);`
  - `	static unsigned long lastDrawTime = misc.get_time();`
  - `static void put_profile(int y, const char* dispDes, int dispValue)`

### OF_RESE.cpp
  - `static Button3D button_select_research;`
  - `static char     research_menu_mode;`
  - `static char     disable_refresh=0;`
  - `static ButtonCustom	button_research_array[MAX_RESEARCH_OPTION];`
  - `static ButtonCustom	button_cancel;`
  - `static int added_count;			// no. of buttons in button_research_array`
  - `static Button3D	button_vacate_firm;`
  - `static void i_disp_research_button(ButtonCustom *, int);`
  - `static void set_all_research(int techId);`
  - `	static short lastTechId=0;`

### OF_HARB.cpp
  - `static VBrowseIF 		browse_ship;`
  - `static Button3D		button_build, button_sail;`
  - `static Button3D		button_cancel_build;`
  - `static ButtonGroup	button_mode(2);`
  - `static short                 button_unit_id[MAX_SHIP_TYPE];`
  - `static ButtonCustom     button_ship[MAX_SHIP_TYPE];`
  - `static ButtonCustom     button_queue_ship[MAX_SHIP_TYPE];`
  - `static int queue_ship_selected = -1;`
  - `static int				added_count;`
  - `static ButtonCustom	button_cancel;`

### OLIGHTN.cpp
  - `static double sqr(double x);`
  - `static double sqr(double x)`

### OTOWNA.cpp
  - `static unsigned long	last_town_ai_profile_time = 0L;`
  - `static unsigned long	town_ai_profile_time = 0L;`
  - `static unsigned long	last_town_profile_time = 0L;`
  - `static unsigned long	town_profile_time = 0L;`
  - `static char random_race();`
  - `	static unsigned long lastDrawTime = misc.get_time();`
  - `static char random_race()`

### OF_BASE.cpp
  - `static Button3D   button_invoke, button_reward;`
  - `static Button3D	button_vacate_firm;`

### OWORLD_Z.cpp
  - `static Rain 		rain;`
  - `static YLightning lightning;`
  - `static Snow			snow;`
  - `static int init_fire = -10;						// reset on new game and load game`
  - `static int sort_display_function( const void *a, const void *b );`
  - `static void get_zoom_pos(short posX, short posY, short& xPoint, short& yPoint)`
  - `static void draw_unit_path_on_zoom_map(int displayLayer)`
  - `static void draw_unit_way_point_on_zoom_map()`
  - `static int sort_display_function( const void *a, const void *b )`

### OAI_ATTK.cpp
  - `static int sort_attack_camp_function( const void *a, const void *b );`
  - `static int sort_attack_camp_function( const void *a, const void *b )`

### OCONFIG.cpp
  - `static char  table_ai_nation_count[5] = { 2, 4, 6, 6, 6 };`
  - `static short table_start_up_cash[5] = { OPTION_HIGH, OPTION_HIGH, OPTION_MODERATE, OPTION_MODERATE, OPTION_LOW };`
  - `static short table_ai_start_up_cash[5] = { OPTION_LOW, OPTION_MODERATE, OPTION_MODERATE, OPTION_HIGH, OPTION_VERY_HIGH };`
  - `static char  table_ai_aggressiveness[5] = { OPTION_LOW, OPTION_LOW, OPTION_MODERATE, OPTION_HIGH, OPTION_VERY_HIGH };`
  - `static short table_start_up_independent_town[5] = { 30, 30, 15, 15, 7 };`
  - `static short table_start_up_raw_site[5] = { 7, 6, 6, 4, 3, };`
  - `static char  table_explore_whole_map[5] = { 1, 1, 1, 1, 0 };`
  - `static char  table_fog_of_war[5] = { 0, 0, 0, 1, 1 };`
  - `static char  table_new_independent_town_emerge[5] = { 1, 1, 1, 1, 1 };`
  - `static char  table_independent_town_resistance[5] = { OPTION_LOW, OPTION_LOW, OPTION_MODERATE, OPTION_HIGH, OPTION_HIGH };`

### OINFO.cpp
  - `static const char* skill_name_array[] =`
  - `static	char *save_buf_1 = NULL;`
  - `static	char *save_buf_1b = NULL;`
  - `static	char *save_buf_2 = NULL;`
  - `static	char *save_buf_3 = NULL;`
  - `static	char *save_buf_4 = NULL;`
  - `	static String str;`
  - `	static String str;`

### OGFILEA.cpp
  - `static char    action_mode;`
  - `static short	browse_recno;`
  - `static short	browse_top_recno;`
  - `static short	menu_x1, menu_y1;`
  - `static int     sort_game_file_function( const void *a, const void *b );`
  - `static void    disp_scroll_bar_func(SlideVBar *scroll, int);`
  - `		static int debug_counter = 0;`
  - `static time_t filetime_to_posix(const GameFileDate* gameFileDate)`
  - `static int sort_game_file_function( const void *a, const void *b )`
  - `static void disp_scroll_bar_func(SlideVBar *scroll, int)`

### OUNITIF.cpp
  - `static Button3D 		button_build;`
  - `static ButtonCustom 	button_build_array[MAX_FIRM_TYPE];`
  - `static char 			button_build_flag[MAX_FIRM_TYPE];`
  - `static Button3D 		button_settle;`
  - `static Button3D 		button_assign;`
  - `static Button3D 		button_promote, button_demote;`
  - `static Button3D 		button_reward;`
  - `static Button3D 		button_return_camp;`
  - `static Button3D 		button_cancel;`
  - `static Button   		button_cancel2;`

### ConfigAdv.cpp
  - `static const char *keyevent_map[] = {`
  - `static int read_int(char *in, int *out);`
  - `static int read_bool(char *in, char *out);`
  - `static int read_key(char *in, char **out, KeyEventMap *event);`
  - `static int read_int(char *in, int *out)`
  - `static int read_bool(char *in, char *out)`
  - `static int read_key(char *in, char **out, KeyEventMap *event)`

### OTALKSPA.cpp
  - `static String str;						// these vars don't have to be saved as their are only used by msg_str() for passing vars to other functions`
  - `static short  viewing_nation_recno;`
  - `static char   should_disp_reply;`
  - `static char   disp_second_line;`

### OROCKRES.cpp
  - `	static RockAnimInfo unanimatedInfo = { 1, 99, 1, 1, };`

### OGAMMAIN.cpp
  - `	static OptionInfo main_option_array[MAIN_OPTION_COUNT] =`
  - `					static char* scaled_buf = nullptr;`
  - `					static int scaled_buf_size = 0;`
  - `	static OptionInfo single_player_option_array[SINGLE_PLAYER_OPTION_COUNT] =`
  - `	static char single_player_option_flag[SINGLE_PLAYER_OPTION_COUNT] =`
  - `	static OptionInfo single_player_option_array[SINGLE_PLAYER_OPTION_COUNT] =`
  - `	static char single_player_option_flag[SINGLE_PLAYER_OPTION_COUNT] =`
  - `	static OptionInfo multi_player_option_array[MULTI_PLAYER_OPTION_COUNT] =`
  - `	static char multi_player_option_flag[MULTI_PLAYER_OPTION_COUNT] =`

### wav_stream.cpp
  - `static bool read_format_header(InputStream *in, FormatHeader *hdrp)`

### openal_audio.cpp
  - `static bool check_al(int line)`
  - `static inline float millibels_to_ratio(long cb)`
  - `static inline long ratio_to_millibels(float ratio)`
  - `static void set_source_panning(ALuint source, int panning)`
  - `static void set_source_volume(ALuint source, int volume_millibels)`
  - `static typename M::key_type max_key(`
  - `static typename M::key_type unused_key(const M *map)`

### OTALKGER.cpp
  - `static String str;						// these vars don't have to be saved as their are only used by msg_str() for passing vars to other functions`
  - `static short  viewing_nation_recno;`
  - `static char   should_disp_reply;`
  - `static char   disp_second_line;`

### WebService.cpp
  - `static size_t WriteMemoryCallback(char *contents, size_t size, size_t nmemb, std::string *buffer)`

### ONEWSENG.cpp
  - `static NewsInfo news_info_array[] =`
  - `static String str;`
  - `	static String str;`
  - `	static String str;`
  - `	static String str;`
  - `	static String str;`
  - `	static char colorCodeStr[] = " @COL0";`
  - `	static char colorCodeStr[] = " @COL0";`

### OU_MONS.cpp
  - `static char random_race();`
  - `	static String str;`
  - `static char random_race()`

### OUNITAAT.cpp
  - `static short *unit_processed_array;						// store those processed unit sprite_recno`
  - `static short unit_processed_count;						// count the number processed units`
  - `static short *dir_array_ptr[ATTACK_DIR];				// store units' sprite_recno in each direction`
  - `static short dir_array_count[ATTACK_DIR];				// num of unit in each direction`
  - `static char unreachable_table[MAX_UNIT_SURROUND_SIZE][MAX_UNIT_SURROUND_SIZE]; // table shared for all attackers`
  - `	static void debug_analyse_result_check(int resultNum, int width, int height)`
  - `	static char surround_loc[MAX_TARGET_SIZE][MAX_TARGET_SIZE] // width, height`
  - `	static char xIncreTable[4] = {  1,  0, -1,  0};`
  - `	static char yIncreTable[4] = {  0,  1,  0, -1};`
  - `	static char leftXIncreTable[4] = {  1,  0, -1,  0};`

### OREMOTEM.cpp
  - `static MsgProcessFP msg_process_function_array[] =`
  - `static void validate_selected_unit_array(short* selectedUnitArray, short& selectedCount);`
  - `static short validate_firm(short firmRecno, unsigned flags = 0);`
  - `static short validate_town(short townRecno, unsigned flags = 0);`
  - `	static int32_t lastRemoteSeed = -1;`
  - `	static short lastNation = 0x7fff;`
  - `static void validate_selected_unit_array(short* selectedUnitArray, short& selectedUnitCount)`
  - `static short validate_firm(short firmRecno, unsigned flags)`
  - `static short validate_town(short townRecno, unsigned flags)`

### OVIDEO.cpp
  - `static long FAR PASCAL video_win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);`
  - `static void create_dummy_window(HINSTANCE hInstance);`
  - `static void create_dummy_window(HINSTANCE hInstance)`
  - `static long FAR PASCAL video_win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)`

### OSPRITE2.cpp
  - `static int check_dir1, check_dir2;`
  - `static short move_x_pixel_array[] = { 0, ZOOM_LOC_WIDTH, ZOOM_LOC_WIDTH, ZOOM_LOC_WIDTH, 0, -ZOOM_LOC_WIDTH, -ZOOM_LOC_WIDTH, -ZOOM_LOC_WIDTH };`
  - `static short move_y_pixel_array[] = { -ZOOM_LOC_HEIGHT, -ZOOM_LOC_HEIGHT, 0, ZOOM_LOC_HEIGHT, ZOOM_LOC_HEIGHT, ZOOM_LOC_HEIGHT, 0, -ZOOM_LOC_HEIGHT };`
  - `	static char turn_amount[10] = {60, 30, 20, 15, 12, 10, 9, 8, 7, 6};`
  - `	static short vector_x_array[] = {  0,  1, 1, 1, 0, -1, -1, -1 };	// default vectors, temporary only`
  - `	static short vector_y_array[] = { -1, -1, 0, 1, 1,  1,  0, -1 };`

### OU_CARA.cpp
  - `static Button 			button_set_stop[MAX_STOP_FOR_CARAVAN];`
  - `static Button 			button_go_stop[MAX_STOP_FOR_CARAVAN];`
  - `static Button 			button_cancel_stop[MAX_STOP_FOR_CARAVAN];`
  - `static ButtonCustom	button_select_array[MAX_STOP_FOR_CARAVAN][MAX_GOODS_SELECT_BUTTON];`
  - `static void				i_disp_caravan_select_button(ButtonCustom *button, int repaintBody);`
  - `	static short last_firm_recno_array[MAX_STOP_FOR_CARAVAN];`
  - `static void i_disp_caravan_select_button(ButtonCustom *button, int repaintBody)`

### OW_FIRE.cpp
  - `static char bound_zero(char n)`

### OGAMSING.cpp
  - `static char option_mode;`
  - `static char race_table[MAX_RACE] =		// race translation table`
  - `static char reverse_race_table[MAX_RACE] =		// race translation table`
  - `static int	select_option();`
  - `static void disp_virtual_button(ButtonCustom *, int);`
  - `static void disp_virtual_tick(ButtonCustom *, int);`
  - `static int select_option()`
  - `	static short startTownArray[3] = { 7, 15, 30 };`
  - `				static short latitudeArray[3] = { 45, 70, 20 };`
  - `static void disp_virtual_button(ButtonCustom *button, int)`

### OFIRM.cpp
  - `static int remove_firm = 0; // true only when the firm is to be removed from the firm_array`
  - `	static String str;`

### OMOUSE.cpp
  - `static int update_x1, update_y1, update_x2, update_y2;          // coordination of the last double-buffer update area`
  - `static unsigned any_key_code_map[KEYEVENT_MAX];`
  - `static unsigned shift_key_code_map[KEYEVENT_MAX];`
  - `static unsigned ctrl_key_code_map[KEYEVENT_MAX];`
  - `static unsigned alt_key_code_map[KEYEVENT_MAX];`
  - `static void reset_key(KeyEventType key_event);`
  - `static unsigned long click_threshold = (long)(0.3 * 1000);`
  - `static void reset_key(KeyEventType key_event)`

### OF_FACT.cpp
  - `static Button3D	button_change_production;`
  - `static Button3D	button_vacate_firm;`

### OVGA.cpp
  - `static void init_dpi();`
  - `static int init_window_flags();`
  - `static void init_window_size();`
  - `   static Uint32 ticks = 0;`
  - `static void init_dpi()`
  - `static int init_window_flags()`
  - `static void init_window_size()`

### OU_MARIF.cpp
  - `static short  	   	unit_disp_y1, unit_info_disp_y1, stop_disp_y1;`
  - `static Button3D		button_unload_all;`
  - `static ButtonGroup	button_mode(2);`
  - `static Button			button_auto_trade;`
  - `static Button 			button_set_stop[MAX_STOP_FOR_SHIP];`
  - `static Button 			button_go_stop[MAX_STOP_FOR_SHIP];`
  - `static Button 			button_cancel_stop[MAX_STOP_FOR_SHIP];`
  - `static ButtonCustom	button_select_array[MAX_STOP_FOR_SHIP][MAX_GOODS_SELECT_BUTTON];`
  - `static void				i_disp_marine_select_button(ButtonCustom *button, int repaintBody);`
  - `	static  short last_unit_id_array[MAX_UNIT_IN_SHIP];`

### OR_MIL.cpp
  - `static VBrowseIF browse_troop;`
  - `static VBrowseIF browse_unit;`
  - `static void put_troop_rec(int recNo, int x, int y, int refreshFlag);`
  - `static void put_unit_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int  troop_filter(int recNo=0);`
  - `static int  unit_filter(int recNo=0);`
  - `static void	disp_troop_total();`
  - `static void disp_unit_total();`
  - `static void put_heading(char justify, int x1, int y1, int x2, int y2, const char *textPtr);`
  - `static void disp_troop_total()`

### ONEWS2.cpp
  - `	static News news;`

### OW_PLANT.cpp
  - `static short opt_temp[3] = { 32, 25, 28 };		// tropical,temperate and both`

### OU_MARI.cpp
  - `	static short height[WAVE_CYCLE] = { 4,3,2,1,0,1,2,3 };`
  - `	static char offset[3] = {0, 1, -1};`
  - `	static short vector_x_array[] = { 0,  1, 1, 1, 0, -1, -1, -1};	// default vectors, temporary only`
  - `	static short vector_y_array[] = {-1, -1, 0, 1, 1,  1,  0, -1};`

### OFIRMIF3.cpp
  - `static short bribe_amount_array[] = { 500, 1000, 1500, 2000, 3000, 4000 };`
  - `static Button3D button_cancel;`
  - `static void disp_bribe_button(int y, int bribeAmount, int buttonUp);`
  - `static void disp_bribe_button(int y, int bribeAmount, int buttonUp)`

### OSYS2.cpp
  - `static int		  report_disp_frame_no;`
  - `static Button3D  button_menu;`
  - `	static char viewModeArray[] =`
  - `	static short highLightX[MODE_TO_DISPLAY_COUNT] = {  0,  62, 124, 186,  7,  68, 129, 192};`
  - `	static short highLightY[MODE_TO_DISPLAY_COUNT] = {  0,   0,   0,   0, 19,  19,  19,  19};`
  - `	static short darkenX[MODE_TO_DISPLAY_COUNT] = {  7,  69, 132, 195,  13,  75, 139, 201};`
  - `	static short darkenY[MODE_TO_DISPLAY_COUNT] = {  8,   8,   8,   8,  29,  29,  29,  29};`

### OF_INN.cpp
  - `static VBrowseIF 	browse_hire;`
  - `static Button3D	button_hire;`
  - `static int			last_hire_count;`
  - `static FirmInn* 	firm_inn_ptr;`
  - `static void put_hire_rec(int recNo, int x, int y, int refreshFlag);`
  - `static char random_race();`
  - `static void put_hire_rec(int recNo, int x, int y, int refreshFlag)`
  - `static char random_race()`

### OTALKENG.cpp
  - `static String str;						// these vars don't have to be saved as their are only used by msg_str() for passing vars to other functions`
  - `static short  viewing_nation_recno;`
  - `static char   should_disp_reply;`
  - `static char   disp_second_line;`

### OTOWNAI.cpp
  - `	static short buildRatingArray[MAX_RACE];`

### OUNITM.cpp
  - `static short move_x_pixel_array[] = { 0, ZOOM_LOC_WIDTH, ZOOM_LOC_WIDTH, ZOOM_LOC_WIDTH, 0, -ZOOM_LOC_WIDTH, -ZOOM_LOC_WIDTH, -ZOOM_LOC_WIDTH };`
  - `static short move_y_pixel_array[] = { -ZOOM_LOC_HEIGHT, -ZOOM_LOC_HEIGHT, 0, ZOOM_LOC_HEIGHT, ZOOM_LOC_HEIGHT, ZOOM_LOC_HEIGHT, 0, -ZOOM_LOC_HEIGHT };`
  - `static short cycle_wait_unit_index;`
  - `static short *cycle_wait_unit_array;`
  - `static short cycle_wait_unit_array_def_size;`
  - `static short cycle_wait_unit_array_multipler;`
  - `static char	 move_action_call_flag=0; // avoid calling move_to_my_loc() if this function is called from move_to() chain`
  - `	static int counter = 0;`

### OGAMENCY.cpp
  - `static int sub_class_count_array[ENCYC_CLASS_COUNT] = `
  - `static const char *race_name[] =`
  - `static const char *weapon_name[] =`
  - `static const char *ship_name[] =`
  - `static const char *firm_name[] =`
  - `static const char *god_name[] =`
  - `static const char *monster_name[] =`
  - `static char monster_page_index[FRYHTAN_MAX_PAGE+1] = { 0, 7, 13, 17 };`
  - `static const char* button_name_array[ENCYC_CLASS_COUNT] =`
  - `static const char* monster_button_name_array[FRYHTAN_MAX_PAGE] = `

### OR_SPY.cpp
  - `static VBrowseIF browse_spy;`
  - `static void put_spy_rec(int recNo, int x, int y, int refreshFlag);`
  - `static int  spy_filter(int recNo=0);`
  - `static void	disp_total();`
  - `static void disp_total()`
  - `static int spy_filter(int recNo)`
  - `static void put_spy_rec(int recNo, int x, int y, int refreshFlag)`

### OUNITAT.cpp
  - `	static unsigned short bitFlag[16] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,`

### OTERRAIN.cpp
  - `static int terrain_type_color_array[TOTAL_TERRAIN_TYPE] =		// the color of each terrain type on the small map`
  - `static const char* map_tile_name_array[TOTAL_TERRAIN_TYPE] =		// the color of each terrain type on the small map`
  - `static char* map_tile_ptr_array[TOTAL_TERRAIN_TYPE];`
  - `static int terrain_type_min_height_array[TOTAL_TERRAIN_TYPE][3] =		// the color of each terrain type on the small map`

### OGFILE.cpp
  - `static int last_status = ERROR_NONE;`
  - `static void posix_to_filetime(time_t t, GameFileDate* gameFileDate)`

