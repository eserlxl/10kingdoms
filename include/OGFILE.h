/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Copyright 1997,1998 Enlight Software Ltd.
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

//Filename    : OGFILE.H
//Description : Object Game file, save and restore game

#ifndef __OGFILE_H
#define __OGFILE_H

#include <cstdint>
#include <FilePath.h>

#ifndef __ONATION_H
#include <ONATION.h>
#endif

class File;
struct SaveGameInfo;

//-------- Define class GameFile -----------//

// Struct is binary compatible to FILETIME on Windows
#pragma pack(1)
struct GameFileDate {
	uint32_t dwLowDateTime;
	uint32_t dwHighDateTime;
};
#pragma pack()

//-------- Define class GameFile -----------//

struct GameFileHeader;

class GameFile
{
public:
	uint32_t     class_size;    // for version compare
	char         file_name[FilePath::MAX_FILE_PATH+1];

	char         player_name[HUMAN_NAME_LEN+1];

	char         race_id;
	char         nation_color;

	int          game_date;     // the game date of the saved game
	GameFileDate file_date;     // saving game date
	short        terrain_set;
	char         note[128];     // User note for savegame

public:
	int   save_game(const char* fileName=NULL);
	int   load_game(const char* base_path, const char* filePath=NULL);

	void  set_file_name();
	int   read_game_header(const char* filePath);

	const char *status_str();

private:
	int   validate_header();

	void  save_process();
	void  load_process();
	int   write_game_header(File* filePtr);

	int   write_file(File*);
	int   write_file_1(File*);
	int   write_file_2(File*);
	int   write_file_3(File*);

	int   read_file(File*);
	int   read_file_1(File*);
	int   read_file_2(File*);
	int   read_file_3(File*);

	void  write_book_mark(File* filePtr, short bookMark);
	int   read_book_mark(File* filePtr, short bookMark);

	void  write_record(GameFileHeader* r);
	void  read_record(GameFileHeader* r);
};

//------- Define class GameFileArray --------//

class GameFileArray : public DynArray
{
public:
	char     demo_format;       // whether write the game in shareware format or not (only selectable in design mode)

	//### begin alex 5/3 ###//
	short	load_file_game_version;	// game version of load file
	char	same_version;		// true if major version of the load game is same as that of the program
	//#### end alex 5/3 ####//

private:
	char     has_read_hall_of_fame;
	char     last_file_name[FilePath::MAX_FILE_PATH+1]; // (persisted via HallOfFame)

public:
	GameFileArray();

	void init(const char *extStr);
	void deinit();

	int  menu(int, int *recno=NULL);

	int  save_game()    { return menu(1); }
	int  load_game()    { return menu(2); }

	int save_new_game(const char* =NULL); // save a new game immediately without prompting menu

	GameFile* operator[](int recNo);

private:
	void set_file_name(char* /*out*/ fileName, int size);

	void disp_browse();
	static void disp_entry_info(const GameFile* entry, int x, int y);
	void load_all_game_header(const char *extStr);
	int  process_action(int=0);
	void del_game();

};

extern GameFileArray game_file_array;
extern GameFile game_file;

//-------------------------------------//

#endif
