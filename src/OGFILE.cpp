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

//Filename    : OGFILE.CPP
//Description : Object Game file, save game and restore game

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_WINDOWS
#include <objbase.h>
#endif

#include <OGFILE.h>
#include <OFILE.h>
#include <OSTR.h>
#include <OMOUSECR.h>
#include <OTALKRES.h>
#include <ONATION.h>
#include <OWORLD.h>
#include <OPOWER.h>
#include <OGAME.h>
#include <OTownNetwork.h>
#include <OINFO.h>
#include <OSYS.h>
#include <OAUDIO.h>
#include <OMUSIC.h>
#include <dbglog.h>
#include "gettext.h"
#include <OGF_REC.h>
#include "posix_string_compat.h"

DBGLOG_DEFAULT_CHANNEL(GameFile);

enum { ERROR_NONE = 0,
	ERROR_CREATE,
	ERROR_WRITE_HEADER,
	ERROR_WRITE_DATA,
	ERROR_OPEN,
	ERROR_FILE_HEADER,
	ERROR_FILE_FORMAT,
	ERROR_FILE_CORRUPTED,
};

static int last_status = ERROR_NONE;


//-------- Begin of function GameFile::save_game --------//
//
// Saves the current game under the given filePath.
// On error, returns false.
//
int GameFile::save_game(const char* fileName)
{
	FilePath fullPath(sys.dir_config);
	File file;
	bool fileOpened = false;

	last_status = ERROR_NONE;

	if( fileName )
		strcpy(file_name, fileName);

	fullPath += file_name;
	if( fullPath.error_flag )
	{
		last_status = ERROR_CREATE;
		return 0;
	}

	int rc = file.file_create(fullPath, 0, 1); // 0=tell File don't handle error itself
												// 1=allow the writing size and the read size to be different
	if( !rc )
		last_status = ERROR_CREATE;
	else
		fileOpened = true;

	if( rc )
	{
		save_process();      // process game data before saving the game

		rc = write_game_header(&file);    // write saved game header information

		if( !rc )
			last_status = ERROR_WRITE_HEADER;

		if( rc )
		{
			rc = write_file(&file);

			if( !rc )
				last_status = ERROR_WRITE_DATA;
		}
	}

	file.file_close();

	//------- when saving error ---------//

	if( !rc )
	{
		if (fileOpened) remove( fullPath );         // delete the file as it is not complete
	}

	return rc != 0;
}
//--------- End of function GameFile::save_game --------//


//-------- Begin of function GameFile::load_game --------//
//
// return : <int> 1 - loaded successfully.
//                0 - not loaded.
//               -1 - error and partially loaded
//
int GameFile::load_game(const char* base_path, const char* fileName)
{
	FilePath fullPath(base_path);
	File file;
	int  rc=1;

	power.win_opened=1;				// to disable power.mouse_handler()

	int oldCursor = mouse_cursor.get_icon();
	mouse_cursor.set_icon( CURSOR_WAITING );

	int powerEnableFlag = power.enable_flag;

	last_status = ERROR_NONE;

	if( fileName )
		strcpy(file_name, fileName);

	fullPath += file_name;
	if( fullPath.error_flag )
	{
		last_status = ERROR_OPEN;
		return 0;
	}

	if( rc && !file.file_open(fullPath, 0, 1) ) // 0=tell File don't handle error itself
	{
		rc = 0;
		last_status = ERROR_OPEN;
	}

	//-------- read in the GameFile class --------//

	if( rc )
	{
		if( !file.file_read(&gf_rec, sizeof(GameFileHeader)) )	// read the whole object from the saved game file
		{
			rc = 0;
			last_status = ERROR_FILE_HEADER;
		}

		read_record(&gf_rec.game_file);
		strncpy(note, gf_rec.game_file.note, sizeof(note));
		note[sizeof(note)-1] = '\0';

		if( !validate_header() )
		{
			rc = 0;
			last_status = ERROR_FILE_FORMAT;
		}
	}

	//--------------------------------------------//
																  // 1=allow the writing size and the read size to be different
	if( rc )
	{
		config.terrain_set = terrain_set;

		game.deinit(1);		// deinit last game first, 1-it is called during loading of a game
		game.init(1);			// init game

		//-------- read in saved game ----------//

		switch( read_file(&file) )
		{
		case 1:
			rc = 1;
			break;
		case -1:
			rc = 0;		// consider cancel load game
			last_status = ERROR_FILE_FORMAT;
			break;
		case 0:
		default:
			rc = -1;
			last_status = ERROR_FILE_CORRUPTED;
		}

		if( rc > 0 )
		{
			load_process();           // process game data after loading the game
			
			//------- create the town network --------//
			town_network_array.recreate_after_load();
		}
	}

	file.file_close();

	power.enable_flag = powerEnableFlag;

	mouse_cursor.restore_icon( oldCursor );

	power.win_opened=0;

	//---------------------------------------//

	if (rc > 0)
	{
		strncpy(scenario_file_name, file_name, FilePath::MAX_FILE_PATH);
		scenario_file_name[FilePath::MAX_FILE_PATH] = 0;
	}

	return rc;
}
//--------- End of function GameFile::load_game --------//


//------- Begin of function GameFile::set_file_name -------//
//
// Set the game file name of current save game, called by
// GameFile::save_game().
//
// e.g. ENLI_001.SAV - the first saved game of the group "Enlight Enterprise"
//
void GameFile::set_file_name()
{
	enum { NAME_PREFIX_LEN = 4,    // Maximum 4 characters in name prefix, e.g. "ENLIG" for "Enlight Enterprise"
			 NAME_NUMBER_LEN = 3  };

	String str, str2;
	int    i;
	char   nameChar;
	const char* baseName;        // the long name which the file name is based on
	char   addStr[] = "0";       // as a small string for adding to the large string

	baseName = (~nation_array)->king_name();

	//--------- add the group name prfix ----------//

	for( i=0 ; i<(int) strlen(baseName) && (int) str.len() < NAME_PREFIX_LEN ; i++ )
	{
		nameChar = misc.upper(baseName[i]);

		if( ( nameChar >= 'A' && nameChar <= 'Z' ) ||
			 ( nameChar >= '0' && nameChar <= '9' ) )
		{
			addStr[0] = nameChar;

			str += addStr;
		}
	}

	//----- add tailing characters if prefix len < NAME_PREFIX_LEN+1 ---//

	while( str.len() < NAME_PREFIX_LEN+1 )       // +1 is the "_" between the name and the number
		str += "_";

	//---- find the saved game number for this saved game ----//

	int       curNumber, lastNumber=0;
	GameFile* gameFile;

	for( i=1 ; i<=game_file_array.size() ; i++ )
	{
		gameFile = game_file_array[i];

		// ##### begin Gilbert 3/10 ########//
		// if( memcmp(gameFile->file_name, str, NAME_PREFIX_LEN)==0 )
		if( strnicmp(gameFile->file_name, str, NAME_PREFIX_LEN)==0 )
		// ##### end Gilbert 3/10 ########//
		{
			//------------------------------------------------//
			//
			// if there is a free number in the middle of the list
			// (left by being deleted game), use this number.
			//
			//------------------------------------------------//

			curNumber = atoi( gameFile->file_name+NAME_PREFIX_LEN+1 );      // +1 is to pass the "_" between the name and the number

			if( curNumber > lastNumber+1 )   // normally, curNumber should be lastNumber+1
				break;

			lastNumber = curNumber;
		}
	}

	//------- add saved game number after the prefix --------//

	str2 = lastNumber+1;    // use the next number after the last number

	for( i=NAME_NUMBER_LEN-str2.len() ; i>0 ; i-- )   // add "0" before the number if the len of the number < NAME_NUMBER_LEN
		str += "0";

	str += str2;
	str += ".SAV";

	//----- copy the string to file_name ------//

	strncpy( file_name, str, FilePath::MAX_FILE_PATH );

	file_name[FilePath::MAX_FILE_PATH] = 0;
}
//--------- End of function GameFile::set_file_name -------//


//-------- Begin of function GameFile::read_game_header --------//
//
// Reads the header of the given file and returns 1 if successful.
//
int GameFile::read_game_header(const char* filePath)
{
	File file;

	if( !file.file_open(filePath, 0, 1) )
		return 0;

	if( !file.file_read(&gf_rec, sizeof(GameFileHeader)) )
	{
		file.file_close();
		return 0;
	}

	read_record(&gf_rec.game_file);
	strncpy(note, gf_rec.game_file.note, sizeof(note));
	note[sizeof(note)-1] = '\0';

	if( !validate_header() )
	{
		file.file_close();
		return 0;
	}

	file.file_close();
	return 1;
}
//--------- End of function GameFile::read_game_header --------//


//-------- Begin of function GameFile::save_process -------//
//
// Make the game data ready for saving game
//
// Called before saving the game
//
void GameFile::save_process()
{
	//--------- set the total playing time --------//

	info.total_play_time += misc.get_time()-info.start_play_time;

	info.start_play_time  = misc.get_time();
}
//--------- End of function GameFile::save_process -------//


//-------- Begin of function GameFile::load_process -------//
//
// Make the game data ready after loading game
//
// Called after loading the game
//
void GameFile::load_process()
{
	info.start_play_time = misc.get_time();       // the time player start playing the game
	config.disable_ai_flag = 0;

	//-- if the player is in the diplomatic message screen, rebuild the talk choice list --//

	if( sys.view_mode==MODE_NATION && info.nation_report_mode==NATION_REPORT_TALK )
		talk_res.set_talk_choices();

	mouse_cursor.set_frame(0);		// to fix a frame bug with loading game

	// reflect the effect of config.music_flag, config.wav_music_volume
	audio.set_wav_volume(config.sound_effect_volume);
	if( config.music_flag )
	{
		if( music.is_playing() )
		{
			music.change_volume(config.wav_music_volume);
		}
	}
	else
	{
		music.stop();
	}

	firm_array.update_firm_links();
	town_array.update_town_links();
}
//--------- End of function GameFile::load_process -------//


static void posix_to_filetime(time_t t, GameFileDate* gameFileDate)
{
	t *= 10000000;
	t += 11644473600000 * 10000;
	gameFileDate->dwLowDateTime = (uint32_t)t;
	gameFileDate->dwHighDateTime = (uint32_t)(t >> 32);
}


//------- Begin of function GameFile::write_game_header -------//
//
// Write saved game header info to the saved game file.
//
// Return : <int> 1 - file written successfully
//                0 - not successful
//
int GameFile::write_game_header(File* filePtr)
{
	class_size = sizeof(GameFileHeader);

	Nation* playerNation = ~nation_array;

	strncpy( player_name, playerNation->king_name(), HUMAN_NAME_LEN );
	player_name[HUMAN_NAME_LEN] = '\0';

	race_id      = playerNation->race_id;
	nation_color = playerNation->nation_color;
	terrain_set  = config.terrain_set;

	game_date    = info.game_date;

	//----- set the file date ------//

#ifdef USE_WINDOWS
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, (FILETIME*)&file_date);
#elif defined USE_POSIX
	posix_to_filetime(time(NULL), &file_date);
#else
	file_date.dwLowDateTime = 0;
	file_date.dwHighDateTime = 0;
#endif

	//------- write GameFile to the saved game file -------//

	strncpy(gf_rec.game_file.note, note, sizeof(gf_rec.game_file.note));
	gf_rec.game_file.note[sizeof(gf_rec.game_file.note)-1] = '\0';
	write_record(&gf_rec.game_file);
	return filePtr->file_write(&gf_rec, sizeof(GameFileHeader));     // write the whole object to the saved game file
}
//--------- End of function GameFile::write_game_header -------//


//--------- Begin of function GameFile::validate_header -------//
int GameFile::validate_header()
{
	return class_size == sizeof(GameFileHeader) && terrain_set > 0;
}
//--------- End of function GameFile::validate_header -------//


//--------- Begin of function GameFile::status_str -------//
const char *GameFile::status_str()
{
	switch(last_status)
	{
	case ERROR_NONE:
		return _("Success");
	case ERROR_CREATE:
		return _("Error creating saved game file");
	case ERROR_WRITE_HEADER:
		return _("Error creating saved game header");
	case ERROR_WRITE_DATA:
		return _("Error writing saved game data");
	case ERROR_OPEN:
		return _("Cannot open save game file");
	case ERROR_FILE_HEADER:
		return _("Cannot read file header");
	case ERROR_FILE_FORMAT:
		return _("Incompatible save game");
	case ERROR_FILE_CORRUPTED:
		return _("Load game error");
	}
	err_here();
	return "";
}
//--------- End of function GameFile::status_str -------//
