/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Copyright 1997,1998 Enlight Software Ltd.
 * Copyright 2010 Unavowed <unavowed@vexillium.org>
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

//Filename    : OGFILE3.CPP
//Description : Object Game file, save game and restore game, part 3

#include <OUNIT.h>

#include <OBULLET.h>
#include <OB_PROJ.h>
#include <OFIRM.h>
#include <OGFILE.h>
#include <ONATION.h>
#include <ONEWS.h>
#include <OREBEL.h>
#include <OREGION.h>
#include <OREGIONS.h>
#include <OSITE.h>
#include <OSNOWG.h>
#include <OSPY.h>
#include <OTORNADO.h>
#include <OTOWN.h>
#include <OU_MARI.h>

#include <ConfigAdv.h>
#include <OGF_V1.h>
#include <OGF_REC.h>

//------- declare static functions -------//

static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize);
static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize);


//-------- Start of function UnitArray::write_file -------------//
//
int UnitArray::write_file(File* filePtr)
{
   int  i;
   Unit *unitPtr;

	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	int writeSize = size();
	// fprintf(stderr, "[DEBUG] UnitArray::write_file: Writing size()=%d units\n", writeSize);
	filePtr->file_put_short( writeSize  );  // no. of units in unit_array

	filePtr->file_put_short( selected_recno );
	filePtr->file_put_short( selected_count );
	filePtr->file_put_long ( cur_group_id   );
	filePtr->file_put_long ( cur_team_id    );
	filePtr->file_put_short(idle_blocked_unit_reset_count);
	filePtr->file_put_long (unit_search_tries);
	filePtr->file_put_short(unit_search_tries_flag);

	filePtr->file_put_short(visible_unit_count);
	// unused short*4
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);

	int deletedCount = 0;
	for( i=1; i<=writeSize ; i++ )
   {
      unitPtr = (Unit*) get_ptr(i);

      //----- write unitId or 0 if the unit is deleted -----//

      if( !unitPtr )    // the unit is deleted
      {
         // fprintf(stderr, "[DEBUG] UnitArray::write_file: Unit %d is deleted, writing 0\n", i);
         filePtr->file_put_short(0);
         deletedCount++;
      }
      else
      {
         //--------- write unit_id -------------//

         // fprintf(stderr, "[DEBUG] UnitArray::write_file: Writing unit %d, unitId=%d\n", i, unitPtr->unit_id);
         filePtr->file_put_short(unitPtr->unit_id);

         //------ write data in the base class ------//

         if( !unitPtr->write_file(filePtr) )
            return 0;

         //------ write data in the derived class ------//

         if( !unitPtr->write_derived_file(filePtr) )
				return 0;
      }
   }
   // long filePosBeforeEmptyRoom = filePtr->file_pos();
   // fprintf(stderr, "[DEBUG] UnitArray::write_file: File position before empty_room_array: %ld\n", filePosBeforeEmptyRoom);
   // fprintf(stderr, "[DEBUG] UnitArray::write_file: Wrote %d units (%d deleted), about to write empty_room_array\n", writeSize, deletedCount);
   // fprintf(stderr, "[DEBUG] UnitArray::write_file: Current empty_room_count=%d before writing\n", empty_room_count);

   //------- write empty room array --------//

   write_empty_room(filePtr);
   // long filePosAfterEmptyRoom = filePtr->file_pos();
   // fprintf(stderr, "[DEBUG] UnitArray::write_file: File position after empty_room_array: %ld (wrote %ld bytes)\n", 
   //    filePosAfterEmptyRoom, filePosAfterEmptyRoom - filePosBeforeEmptyRoom);
   // fprintf(stderr, "[DEBUG] UnitArray::write_file: Finished writing empty_room_array\n");

   return 1;
}
//--------- End of function UnitArray::write_file ---------------//


//-------- Start of function UnitArray::read_file -------------//
//
int UnitArray::read_file(File* filePtr)
{
	Unit*   unitPtr;
	int     i, unitId, emptyRoomCount=0;

	restart_recno    = filePtr->file_get_short();

	int unitCount    = filePtr->file_get_short();  // get no. of units from file
	// fprintf(stderr, "[DEBUG] UnitArray::read_file: unitCount=%d\n", unitCount);

	selected_recno   = filePtr->file_get_short();
	selected_count   = filePtr->file_get_short();
	cur_group_id     = filePtr->file_get_long();
	cur_team_id      = filePtr->file_get_long();
	idle_blocked_unit_reset_count = filePtr->file_get_short();
	unit_search_tries	= filePtr->file_get_long ();
	unit_search_tries_flag = (char) filePtr->file_get_short();

   visible_unit_count					= filePtr->file_get_short();
	// unused short*4
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();

   // long filePosBeforeUnits = filePtr->file_pos();
   // fprintf(stderr, "[DEBUG] UnitArray::read_file: File position before reading units: %ld\n", filePosBeforeUnits);
   
   for( i=1 ; i<=unitCount ; i++ )
   {
      // long filePosBeforeUnit = filePtr->file_pos();
      unitId = filePtr->file_get_short();
      // fprintf(stderr, "[DEBUG] Reading unit %d/%d: unitId=%d (0x%04x), file position: %ld\n", i, unitCount, unitId, unitId, filePosBeforeUnit);

      if( unitId==0 )  // the unit has been deleted
      {
         // fprintf(stderr, "[DEBUG] Unit %d is deleted (unitId=0), skipping unit data\n", i);
         // long currentPos = filePtr->file_pos();
         // fprintf(stderr, "[DEBUG] Unit %d deleted: file position after reading unitId=0: %ld\n", i, currentPos);
         
         // Check if this might be an old format that wrote unit data even for deleted units
         // Try to peek ahead: read the next short to see if it's a valid unitId or part of unit data
         // If it's > 0 and < 1000, it might be sprite_id from UnitGF (old format wrote unit data for deleted units)
         // long peekPos = filePtr->file_pos();
         // short peekValue = filePtr->file_get_short();
         // filePtr->file_seek(-2, SEEK_CUR); // Seek back
         
         // fprintf(stderr, "[DEBUG] Unit %d deleted: peeked next value=%d (0x%04x)\n", i, peekValue, peekValue);
         
         // If peekValue looks like it might be sprite_id (typically 1-1000 range) and we're not at the last unit,
         // this might be old format that wrote unit data for deleted units
         // if( peekValue > 0 && peekValue < 1000 && i < unitCount )
         // {
         //    fprintf(stderr, "[DEBUG] Unit %d deleted: Detected possible old format - next value %d might be sprite_id\n", i, peekValue);
         //    fprintf(stderr, "[DEBUG] Unit %d deleted: Old format likely wrote unit data for deleted units - need to skip it\n", i);
         //    // We can't easily skip without knowing unit_id, so we'll detect this later when reading the next unitId
         // }
         
         add_blank(1);     // it's a DynArrayB function
         emptyRoomCount++;
      }
      else
      {
         //----- validate unitId before creating unit -----------//
         if( unitId < 1 || unitId > unit_res.unit_info_count ) {
			// fprintf(stderr, "[DEBUG] Invalid unitId=%d (0x%04x), unit_info_count=%d, i=%d/%d - this suggests file corruption or misalignment\n", 
			// 	unitId, unitId, unit_res.unit_info_count, i, unitCount);
			
			// Try to recover: if unitId looks like it might be part of unit data (e.g., sprite_id = 256),
			// we might be misaligned due to old format writing unit data for deleted units.
			// Seek back 2 bytes (we just read the short) and try to read UnitGF to get the real unit_id
			if( unitId >= 256 && unitId < 1000 && i > 1 ) {
				// fprintf(stderr, "[DEBUG] Attempting recovery: unitId=%d might be sprite_id from misaligned read\n", unitId);
				long currentPos = filePtr->file_pos();
				// fprintf(stderr, "[DEBUG] Current file position: %ld, seeking back 2 bytes\n", currentPos);
				filePtr->file_seek(-2, SEEK_CUR); // Seek back to before we read the invalid unitId
				
				// Try to read UnitGF structure to get the actual unit_id
				UnitGF tempGF;
				if( filePtr->file_read(&tempGF, sizeof(UnitGF)) ) {
					// Extract unit_id from the structure (it's at offset after sprite fields)
					int8_t actualUnitId = tempGF.unit_id;
					// fprintf(stderr, "[DEBUG] Recovered: read UnitGF, actual unit_id=%d\n", actualUnitId);
					
					// Check if actualUnitId is valid
					// fprintf(stderr, "[DEBUG] Recovery: actual unit_id=%d, unit_info_count=%d\n", actualUnitId, unit_res.unit_info_count);
					if( actualUnitId >= 1 && actualUnitId <= unit_res.unit_info_count ) {
						// fprintf(stderr, "[DEBUG] Recovery successful! Actual unitId=%d, file was misaligned\n", actualUnitId);
						unitId = actualUnitId;
						
						// We've already read UnitGF into tempGF, so we need to process it and continue reading the rest
						// Create unit and use the already-read UnitGF data
						unitPtr = create_unit( unitId );
						unitPtr->unit_id = unitId;
						
						// Copy tempGF to global gf_rec and process it (Unit::read_file uses global gf_rec)
						memcpy(&gf_rec.unit, &tempGF, sizeof(UnitGF));
						unitPtr->read_record(&gf_rec.unit);
						
						// Now read the rest of the unit data (result_node_array, way_point_array, team_info)
						// This matches the logic in Unit::read_file after reading UnitGF
						// fprintf(stderr, "[DEBUG] Recovery: Processing rest of unit data for unitId=%d\n", unitId);
						
						// Read memory data (result_node_array, way_point_array, team_info)
						// Use a helper function or inline the logic from Unit::read_file
						// For now, create a temporary Unit and call its read_file to read the rest
						// Actually, we can't do that because we've already read UnitGF
						// Let's manually read the rest
						
						// Defensive: Only free if not nullptr, not 0xdeadbeef, and not a small integer
						auto safe_free = [](void*& ptr) {
							if (ptr && ptr != (void*)0xdeadbeef && (uintptr_t)ptr > 0x1000) {
								mem_del(ptr);
							}
							ptr = nullptr;
						};
						
						safe_free((void*&)unitPtr->result_node_array);
						if( unitPtr->result_node_count > 0 )
						{
							ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNode)*unitPtr->result_node_count);
							if( !filePtr->file_read(node_record_array, sizeof(ResultNodeGF)*unitPtr->result_node_count) )
							{
								mem_del(node_record_array);
								// fprintf(stderr, "[DEBUG] Recovery: Failed to read result_node_array\n");
								return 0;
							}
							unitPtr->result_node_array = (ResultNode*) mem_add(sizeof(ResultNode) * unitPtr->result_node_count);
							for( int j=0; j<unitPtr->result_node_count; j++ )
							{
								ResultNode *node = unitPtr->result_node_array+j;
								node->read_record(node_record_array+j);
							}
							mem_del(node_record_array);
						}
						
						// Read way_point_array
						safe_free((void*&)unitPtr->way_point_array);
						if( unitPtr->way_point_array_size > 0 )
						{
							ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*unitPtr->way_point_array_size);
							memset(node_record_array, 0, sizeof(ResultNodeGF)*unitPtr->way_point_array_size);
							if( !filePtr->file_read(node_record_array, sizeof(ResultNodeGF)*unitPtr->way_point_array_size) )
							{
								mem_del(node_record_array);
								// fprintf(stderr, "[DEBUG] Recovery: Failed to read way_point_array\n");
								return 0;
							}
							unitPtr->way_point_array = (ResultNode*) mem_add(sizeof(ResultNode)*unitPtr->way_point_array_size);
							memset(unitPtr->way_point_array, 0, sizeof(ResultNode)*unitPtr->way_point_array_size);
							for( int j=0; j<unitPtr->way_point_array_size; j++ )
							{
								ResultNode *node = unitPtr->way_point_array+j;
								node->read_record(node_record_array+j);
							}
							mem_del(node_record_array);
						}
						
						// Read team_info (use global gf_rec like Unit::read_file does)
						// Check has_team_info flag from UnitGF before reading TeamInfoGF
						safe_free((void*&)unitPtr->team_info);
						if( gf_rec.unit.has_team_info )
						{
							if( filePtr->file_read(&gf_rec, sizeof(TeamInfoGF)) )
							{
								unitPtr->team_info = (TeamInfo*) mem_add(sizeof(TeamInfo));
								memset(unitPtr->team_info, 0, sizeof(TeamInfo));
								unitPtr->team_info->read_record(&gf_rec.team_info);
							}
						}
						
						// Restore sprite_info
						if (unitPtr->sprite_id > 0) {
							if (unitPtr->sprite_id > sprite_res.sprite_info_count) {
								// fprintf(stderr, "[DEBUG] Recovery: Invalid sprite_id=%d\n", unitPtr->sprite_id);
							} else {
								unitPtr->sprite_info = sprite_res[unitPtr->sprite_id];
								if (unitPtr->sprite_info)
									unitPtr->sprite_info->load_bitmap_res();
							}
						}
						
						// Read derived class data
						if( !unitPtr->read_derived_file( filePtr ) )
						{
							// fprintf(stderr, "[DEBUG] Recovery: read_derived_file failed for unitId=%d\n", unitId);
							return 0;
						}
						
						unitPtr->fix_attack_info();
						// fprintf(stderr, "[DEBUG] Recovery: Successfully recovered and loaded unit %d\n", unitId);
						// Continue to next iteration
						continue;
					} else if( actualUnitId == 0 ) {
						// If actualUnitId is 0, this might be a deleted unit that had its data written
						// We need to skip the unit data to maintain alignment
						// fprintf(stderr, "[DEBUG] Recovery: Detected deleted unit (unit_id=0) with unit data written - this is the misalignment cause\n");
						// fprintf(stderr, "[DEBUG] Recovery: Need to skip unit data for this deleted unit to maintain alignment\n");
						// Skip the rest of UnitGF (we've already read it into tempGF)
						// Then skip result_node_array, way_point_array, team_info, and derived data
						// This is complex without knowing unit_id - for now, fail with clear message
						// fprintf(stderr, "[DEBUG] Recovery: File format mismatch - deleted units have unit data written.\n");
						// fprintf(stderr, "[DEBUG] Recovery: Please resave the game with the current version.\n");
						return 0;
					} else {
						// fprintf(stderr, "[DEBUG] Recovery failed: actual unit_id=%d is also invalid (out of range 1-%d)\n", 
						// 	actualUnitId, unit_res.unit_info_count);
						// fprintf(stderr, "[DEBUG] Recovery: This suggests severe file corruption or incompatible format\n");
					}
				} else {
					// fprintf(stderr, "[DEBUG] Recovery failed: could not read UnitGF structure\n");
				}
			}
			
			// fprintf(stderr, "[DEBUG] The save file appears to be from an incompatible version or corrupted.\n");
			// fprintf(stderr, "[DEBUG] Please resave the game with the current version, or use a save file created with this version.\n");
            return 0;
         }
         //----- create unit object -----------//
         unitPtr = create_unit( unitId );
         unitPtr->unit_id = unitId;

         long filePosBeforeUnitData = filePtr->file_pos();
         
         //---- read data in base class -----//

         if( !unitPtr->read_file( filePtr ) )
         {
			// fprintf(stderr, "[DEBUG] Unit::read_file failed for unitId=%d, i=%d\n", unitId, i);
            return 0;
		}
		// long filePosAfterUnitData = filePtr->file_pos();
		// fprintf(stderr, "[DEBUG] Unit %d (unitId=%d): read_file consumed %ld bytes\n", i, unitId, filePosAfterUnitData - filePosBeforeUnitData);

         //----- read data in derived class -----//

         if( !unitPtr->read_derived_file( filePtr ) )
         {
			// fprintf(stderr, "[DEBUG] Unit::read_derived_file failed for unitId=%d, i=%d\n", unitId, i);
            return 0;
		}
		// long filePosAfterDerived = filePtr->file_pos();
		// fprintf(stderr, "[DEBUG] Unit %d (unitId=%d): read_derived_file consumed %ld bytes\n", i, unitId, filePosAfterDerived - filePosAfterUnitData);

			unitPtr->fix_attack_info();
      }
   }
   // long filePosAfterAllUnits = filePtr->file_pos();
   // fprintf(stderr, "[DEBUG] UnitArray::read_file: Finished reading all %d units, total bytes read: %ld\n", unitCount, filePosAfterAllUnits - filePosBeforeUnits);

	//-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
	{
		DynArrayB::go(i);             // since UnitArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

   //------- read empty room array --------//
   // fprintf(stderr, "[DEBUG] UnitArray::read_file: About to read empty_room_array, expected emptyRoomCount=%d\n", emptyRoomCount);
   if( !read_empty_room(filePtr) )
   {
      // fprintf(stderr, "[DEBUG] UnitArray::read_file: read_empty_room failed\n");
      return 0;
   }
   // fprintf(stderr, "[DEBUG] UnitArray::read_file: Finished reading empty_room_array, empty_room_count=%d (expected %d)\n", empty_room_count, emptyRoomCount);
   
   // Log file position after reading empty_room_array
   // long filePosAfterEmptyRoom = filePtr->file_pos();
   // fprintf(stderr, "[DEBUG] UnitArray::read_file: File position after empty_room_array: %ld\n", filePosAfterEmptyRoom);
   
   // Defensive: Validate empty_room_count matches what we expect
   // If there's a significant mismatch (> 10 units difference), the file is likely corrupted or from incompatible version
   if( empty_room_count != emptyRoomCount )
   {
      int diff = abs(empty_room_count - emptyRoomCount);
      if( diff > 10 || empty_room_count > 100 )
      {
         // fprintf(stderr, "[DEBUG] UnitArray::read_file: ERROR - empty_room_count mismatch! Read %d but expected %d (diff=%d) - file may be corrupted or from incompatible version\n", 
         //    empty_room_count, emptyRoomCount, diff);
         // fprintf(stderr, "[DEBUG] UnitArray::read_file: This suggests the save file format is incompatible. Please resave the game with the current version.\n");
         return 0; // Fail the load
      }
      else
      {
         // fprintf(stderr, "[DEBUG] UnitArray::read_file: WARNING - empty_room_count mismatch! Read %d but expected %d (diff=%d) - minor difference, continuing\n", 
         //    empty_room_count, emptyRoomCount, diff);
      }
   }

   //------- verify the empty_room_array loading -----//

#ifdef DEBUG
   err_when( empty_room_count != emptyRoomCount );

   for( i=0 ; i<empty_room_count ; i++ )
   {
      if( !is_deleted( empty_room_array[i].recno ) )
         err_here();
   }
#endif

   // fprintf(stderr, "[DEBUG] UnitArray::read_file: Returning success\n");
   return 1;
}
//--------- End of function UnitArray::read_file ---------------//


//--------- Begin of function Unit::write_file ---------//
//
// Write data in derived class.
//
// If the derived Unit don't have any special data,
// just use Unit::write_file(), otherwise make its own derived copy of write_file()
//
int Unit::write_file(File* filePtr)
{
	write_record(&gf_rec.unit);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitGF)) )
		return 0;

	//--------------- write memory data ----------------//

	if( result_node_array )
	{
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNode)*result_node_count);
		for( int i=0; i<result_node_count; i++ )
		{
			ResultNode *node = result_node_array+i;
			node->write_record(node_record_array+i);
		}
		if( !filePtr->file_write(node_record_array, sizeof(ResultNodeGF)*result_node_count) )
		{
			mem_del(node_record_array);
			return 0;
		}
		mem_del(node_record_array);
	}

	//### begin alex 15/10 ###//
	if(way_point_array)
	{
		err_when(way_point_array_size==0 || way_point_array_size<way_point_count);
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*way_point_array_size);
		for( int i=0; i<way_point_array_size; i++ )
		{
			ResultNode *node = way_point_array+i;
			node->write_record(node_record_array+i);
		}
		if( !filePtr->file_write(node_record_array, sizeof(ResultNodeGF)*way_point_array_size) )
		{
			mem_del(node_record_array);
			return 0;
		}
		mem_del(node_record_array);
	}
	//#### end alex 15/10 ####//

	if( team_info )
	{
		team_info->write_record(&gf_rec.team_info);
		if( !filePtr->file_write(&gf_rec, sizeof(TeamInfoGF)) )
			return 0;
	}

	return 1;
}
//----------- End of function Unit::write_file ---------//


//--------- Begin of function Unit::read_file ---------//
//
int Unit::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitGF)) )
		return 0;
	read_record(&gf_rec.unit);

	//--------------- read in memory data ----------------//

	// Defensive: Only free if not nullptr, not 0xdeadbeef, and not a small integer
	auto safe_free = [](void*& ptr) {
		if (ptr && ptr != (void*)0xdeadbeef && (uintptr_t)ptr > 0x1000) {
			mem_del(ptr);
		}
		ptr = nullptr;
	};

	safe_free((void*&)result_node_array);
	// Defensive: Only allocate if count > 0
	if( result_node_count > 0 )
	{
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNode)*result_node_count);

		if( !filePtr->file_read(node_record_array, sizeof(ResultNodeGF)*result_node_count) )
		{
			mem_del(node_record_array);
			return 0;
		}
		
		result_node_array = (ResultNode*) mem_add(sizeof(ResultNode) * result_node_count);
		for( int i=0; i<result_node_count; i++ )
		{
			ResultNode *node = result_node_array+i;
			node->read_record(node_record_array+i);
		}
		mem_del(node_record_array);
	}

	//### begin alex 15/10 ###//
	safe_free((void*&)way_point_array);
	if( way_point_array_size > 0 )
	{
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*way_point_array_size);
		memset(node_record_array, 0, sizeof(ResultNodeGF)*way_point_array_size); // Patch: zero-initialize
		if( !filePtr->file_read(node_record_array, sizeof(ResultNodeGF)*way_point_array_size) )
		{
			mem_del(node_record_array);
			return 0;
		}
		way_point_array = (ResultNode*) mem_add(sizeof(ResultNode)*way_point_array_size);
		memset(way_point_array, 0, sizeof(ResultNode)*way_point_array_size); // Patch: zero-initialize
		for( int i=0; i<way_point_array_size; i++ )
		{
			ResultNode *node = way_point_array+i;
			node->read_record(node_record_array+i);
		}
		mem_del(node_record_array);
	}
	//#### end alex 15/10 ####//

	safe_free((void*&)team_info);
	// Check has_team_info flag from UnitGF before reading TeamInfoGF
	// This matches the write logic which only writes TeamInfoGF if team_info exists
	if( gf_rec.unit.has_team_info )
	{
		if( filePtr->file_read(&gf_rec, sizeof(TeamInfoGF)) )
		{
			team_info = (TeamInfo*) mem_add(sizeof(TeamInfo));
			memset(team_info, 0, sizeof(TeamInfo)); // Patch: zero-initialize
			team_info->read_record(&gf_rec.team_info);
		}
	}

	//----------- post-process the data read ----------//
	// Restore sprite_info from sprite_res if sprite_id is valid
	// This is necessary because sprite_info is not saved to file (it's a pointer)
	if (sprite_id > 0) {
		if (sprite_id > sprite_res.sprite_info_count) {
			// fprintf(stderr, "[DEBUG] Unit::read_file: Invalid sprite_id=%d, sprite_info_count=%d, unit_id=%d\n", 
			// 	sprite_id, sprite_res.sprite_info_count, unit_id);
			// Don't fail load, just skip sprite_info restoration
		} else {
			sprite_info = sprite_res[sprite_id];
			if (sprite_info) // Patch: defensive check
				sprite_info->load_bitmap_res();
		}
	}
	// If sprite_info is still NULL, it will be handled by our validation checks in other functions

	return 1;
}
//----------- End of function Unit::read_file ---------//


//--------- Begin of function Unit::write_derived_file ---------//
//
int Unit::write_derived_file(File* filePtr)
{
   //--- write data in derived class -----//

	int writeSize = unit_array.unit_class_size(unit_id)-sizeof(Unit);

   if( writeSize > 0 )
   {
      if( !filePtr->file_write( (char*) this + sizeof(Unit), writeSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Unit::write_derived_file ---------//


//--------- Begin of function Unit::read_derived_file ---------//
//
int Unit::read_derived_file(File* filePtr)
{
	//--- read data in derived class -----//

   int readSize = unit_array.unit_class_size(unit_id) - sizeof(Unit);

   if( readSize > 0 )
   {
      if( !filePtr->file_read( (char*) this + sizeof(Unit), readSize ) )
         return 0;
	}

   return 1;
}
//----------- End of function Unit::read_derived_file ---------//


//--------- Begin of function UnitCaravan::write_derived_file ---------//
int UnitCaravan::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_caravan);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitCaravanGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitCaravan::write_derived_file ---------//


//--------- Begin of function UnitCaravan::read_derived_file ---------//
int UnitCaravan::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitCaravanGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_caravan);
	return 1;
}
//--------- End of function UnitCaravan::read_derived_file ---------//


//--------- Begin of function UnitExpCart::write_derived_file ---------//
int UnitExpCart::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_exp_cart);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitExpCartGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitExpCart::write_derived_file ---------//


//--------- Begin of function UnitExpCart::read_derived_file ---------//
int UnitExpCart::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitExpCartGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_exp_cart);
	return 1;
}
//--------- End of function UnitExpCart::read_derived_file ---------//


//--------- Begin of function UnitGod::write_derived_file ---------//
int UnitGod::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_god);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitGodGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitGod::write_derived_file ---------//


//--------- Begin of function UnitGod::read_derived_file ---------//
int UnitGod::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitGodGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_god);
	return 1;
}
//--------- End of function UnitGod::read_derived_file ---------//


//--------- Begin of function UnitMarine::write_derived_file ---------//
int UnitMarine::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_marine);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitMarineGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMarine::write_derived_file ---------//


//--------- Begin of function UnitMarine::read_derived_file ---------//
int UnitMarine::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitMarineGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_marine);

	// ------- post-process the data read --------//
	splash.sprite_info = sprite_res[splash.sprite_id];
	splash.sprite_info->load_bitmap_res();

	return 1;
}
//--------- End of function UnitMarine::read_derived_file ---------//


//--------- Begin of function UnitMonster::write_derived_file ---------//
int UnitMonster::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_monster);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitMonsterGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMonster::write_derived_file ---------//


//--------- Begin of function UnitMonster::read_derived_file ---------//
int UnitMonster::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitMonsterGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_monster);
	return 1;
}
//--------- End of function UnitMonster::read_derived_file ---------//


//--------- Begin of function UnitVehicle::write_derived_file ---------//
int UnitVehicle::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_vehicle);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitVehicleGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitVehicle::write_derived_file ---------//


//--------- Begin of function UnitVehicle::read_derived_file ---------//
int UnitVehicle::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitVehicleGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_vehicle);
	return 1;
}
//--------- End of function UnitVehicle::read_derived_file ---------//


//*****//


//-------- Start of function BulletArray::write_file -------------//
//
int BulletArray::write_file(File* filePtr)
{
	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	int    i, emptyRoomCount=0;;
	Bullet *bulletPtr;

	filePtr->file_put_short( size() );  // no. of bullets in bullet_array

	for( i=1; i<=size() ; i++ )
	{
		bulletPtr = (Bullet*) get_ptr(i);

		//----- write bulletId or 0 if the bullet is deleted -----//

		if( !bulletPtr )    // the bullet is deleted
		{
			filePtr->file_put_short(0);
			emptyRoomCount++;
		}
		else
		{
			filePtr->file_put_short(bulletPtr->sprite_id);      // there is a bullet in this record

			//------ write data in the base class ------//

			if( !bulletPtr->write_file(filePtr) )
				return 0;

			//------ write data in the derived class -------//

			if( !bulletPtr->write_derived_file(filePtr) )
				return 0;
		}
	}

	//------- write empty room array --------//

	write_empty_room(filePtr);

	//------- verify the empty_room_array loading -----//

#ifdef DEBUG
	err_when( empty_room_count != emptyRoomCount );

   for( i=0 ; i<empty_room_count ; i++ )
   {
		if( !is_deleted( empty_room_array[i].recno ) )
         err_here();
   }
#endif

	return 1;
}
//--------- End of function BulletArray::write_file -------------//


//-------- Start of function BulletArray::read_file -------------//
//
int BulletArray::read_file(File* filePtr)
{
	restart_recno    = filePtr->file_get_short();

	int     i, bulletRecno, bulletCount, emptyRoomCount=0, spriteId;
	Bullet* bulletPtr;

	bulletCount = filePtr->file_get_short();  // get no. of bullets from file

	for( i=1 ; i<=bulletCount ; i++ )
	{
		spriteId = filePtr->file_get_short();
		if( spriteId == 0 )
		{
			add_blank(1);     // it's a DynArrayB function

			emptyRoomCount++;
		}
		else
		{
			//----- create bullet object -----------//

			bulletRecno = create_bullet(spriteId);
			bulletPtr   = bullet_array[bulletRecno];

         //----- read data in base class --------//

         if( !bulletPtr->read_file( filePtr ) )
            return 0;

			//----- read data in derived class -----//

			if( !bulletPtr->read_derived_file( filePtr ) )
				return 0;
      }
	}

   //-------- linkout() those record added by add_blank() ----------//
	//-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

   for( i=1 ; i<=size() ; i++ )
   {
      DynArrayB::go(i);             // since BulletArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

	//------- read empty room array --------//

	read_empty_room(filePtr);

	//------- verify the empty_room_array loading -----//

#ifdef DEBUG
	err_when( empty_room_count != emptyRoomCount );

	for( i=0 ; i<empty_room_count ; i++ )
	{
		if( !is_deleted( empty_room_array[i].recno ) )
			err_here();
	}
#endif

	return 1;
}
//--------- End of function BulletArray::read_file ---------------//


//--------- Begin of function Bullet::write_file ---------//
//
int Bullet::write_file(File* filePtr)
{
	write_record(&gf_rec.bullet);
	if( !filePtr->file_write(&gf_rec, sizeof(BulletGF)) )
		return 0;
	return 1;
}
//----------- End of function Bullet::write_file ---------//


//--------- Begin of function Bullet::read_file ---------//
//
int Bullet::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(BulletGF)) )
		return 0;
	read_record(&gf_rec.bullet);

	//------------ post-process the data read ----------//

	sprite_info = sprite_res[sprite_id];

	sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Bullet::read_file ---------//


//----------- Begin of function Bullet::write_derived_file ---------//
int Bullet::write_derived_file(File *filePtr)
{
	//--- write data in derived class -----//

	int writeSize = bullet_array.bullet_class_size(sprite_id)-sizeof(Bullet);

	if( writeSize > 0 )
	{
		if( !filePtr->file_write( (char*) this + sizeof(Bullet), writeSize ) )
			return 0;
	}

	return 1;

}
//----------- End of function Bullet::write_derived_file ---------//


//----------- Begin of function Bullet::read_derived_file ---------//
int Bullet::read_derived_file(File *filePtr)
{
	//--- read data in derived class -----//

	int readSize = bullet_array.bullet_class_size(sprite_id) - sizeof(Bullet);

	if( readSize > 0 )
	{
		if( !filePtr->file_read( (char*) this + sizeof(Bullet), readSize ) )
			return 0;
	}

	return 1;
}
//----------- End of function Bullet::read_derived_file ---------//


//--------- Begin of function BulletHoming::write_derived_file ---------//
int BulletHoming::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.bullet_homing);
	if( !filePtr->file_write(&gf_rec, sizeof(BulletHomingGF)) )
		return 0;
	return 1;
}
//--------- End of function BulletHoming::write_derived_file ---------//


//--------- Begin of function BulletHoming::read_derived_file ---------//
int BulletHoming::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(BulletHomingGF)) )
		return 0;
	read_derived_record(&gf_rec.bullet_homing);
	return 1;
}
//--------- End of function BulletHoming::read_derived_file ---------//


//----------- Begin of function Projectile::write_derived_file ---------//
int Projectile::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.projectile);
	if( !filePtr->file_write(&gf_rec, sizeof(ProjectileGF)) )
		return 0;
	return 1;
}
//----------- End of function Projectile::write_derived_file ---------//


//----------- Begin of function Projectile::read_derived_file ---------//
int Projectile::read_derived_file(File *filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(ProjectileGF)) )
		return 0;
	read_derived_record(&gf_rec.projectile);

	//----------- post-process the data read ----------//
	act_bullet.sprite_info = sprite_res[act_bullet.sprite_id];
	act_bullet.sprite_info->load_bitmap_res();
	bullet_shadow.sprite_info = sprite_res[bullet_shadow.sprite_id];
	bullet_shadow.sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Projectile::read_derived_file ---------//

//*****//

//-------- Start of function FirmArray::write_file -------------//
//
int FirmArray::write_file(File* filePtr)
{
   int  i;
   Firm *firmPtr;

   filePtr->file_put_short( size()  );  // no. of firms in firm_array
   filePtr->file_put_short( process_recno );
	filePtr->file_put_short( selected_recno );

	filePtr->file_put_short( Firm::firm_menu_mode );
	filePtr->file_put_short( Firm::action_spy_recno );
	filePtr->file_put_short( Firm::bribe_result );
	filePtr->file_put_short( Firm::assassinate_result );

	for( i=1; i<=size() ; i++ )
   {
      firmPtr = (Firm*) get_ptr(i);

      //----- write firmId or 0 if the firm is deleted -----//

      if( !firmPtr )    // the firm is deleted
		{
         filePtr->file_put_short(0);
      }
      else
      {
         //--------- write firm_id -------------//

         filePtr->file_put_short(firmPtr->firm_id);

         //------ write data in base class --------//

			if( !firmPtr->write_file(filePtr) )
				return 0;

         //------ write data in derived class ------//

         if( !firmPtr->write_derived_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

	write_empty_room(filePtr);

   return 1;
}
//--------- End of function FirmArray::write_file ---------------//


//-------- Start of function FirmArray::read_file -------------//
//
int FirmArray::read_file(File* filePtr)
{
	Firm*   firmPtr;
	int     i, firmId, firmRecno;

	int firmCount      = filePtr->file_get_short();  // get no. of firms from file
	process_recno      = filePtr->file_get_short();
	selected_recno     = filePtr->file_get_short();

	Firm::firm_menu_mode  	 = (char) filePtr->file_get_short();
	Firm::action_spy_recno   = filePtr->file_get_short();
	Firm::bribe_result    	 = (char) filePtr->file_get_short();
	Firm::assassinate_result = (char) filePtr->file_get_short();

   for( i=1 ; i<=firmCount ; i++ )
   {
      firmId = filePtr->file_get_short();

      if( firmId==0 )  // the firm has been deleted
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create firm object -----------//

         firmRecno = create_firm( firmId );
         firmPtr   = firm_array[firmRecno];

         //---- read data in base class -----//

         if( !firmPtr->read_file(filePtr) )
            return 0;

         //----- read data in derived class -----//

         if( !firmPtr->read_derived_file( filePtr ) )
            return 0;
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since FirmArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

   //------- read empty room array --------//

   read_empty_room(filePtr);

   return 1;
}
//--------- End of function FirmArray::read_file ---------------//


//--------- Begin of function Firm::write_file ---------//
//
int Firm::write_file(File* filePtr)
{
	write_record(&gf_rec.firm);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmGF)) )
		return 0;

	//--------- write worker_array ---------//

	if( worker_array )
	{
		WorkerGF *worker_record_array = (WorkerGF*) mem_add( MAX_WORKER*sizeof(WorkerGF) );

		for( int i=0; i<MAX_WORKER; i++ )
		{
			Worker *workerPtr = worker_array+i;
			workerPtr->write_record(worker_record_array+i);
		}
		if( !filePtr->file_write(worker_record_array, MAX_WORKER*sizeof(WorkerGF)) )
		{
			mem_del(worker_record_array);
			return 0;
		}
		mem_del(worker_record_array);
	}

	return 1;
}
//----------- End of function Firm::write_file ---------//


//--------- Begin of function Firm::read_file ---------//
//
int Firm::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmGF)) )
		return 0;
	read_record(&gf_rec.firm);

	if( !game_file_array.same_version && firm_id > FIRM_BASE )
		firm_build_id += MAX_RACE - VERSION_1_MAX_RACE;

	//--------- read worker_array ---------//

	if( firm_res[firm_id]->need_worker )
	{
		WorkerGF *worker_record_array = (WorkerGF*) mem_add( MAX_WORKER*sizeof(WorkerGF) );
		worker_array = (Worker*) mem_add( MAX_WORKER*sizeof(Worker) );

		if( !filePtr->file_read(worker_record_array, MAX_WORKER*sizeof(WorkerGF)) )
		{
			mem_del(worker_record_array);
			return 0;
		}
		for( int i=0; i<MAX_WORKER; i++ )
		{
			Worker *workerPtr = worker_array+i;
			workerPtr->read_record(worker_record_array+i);
		}
		mem_del(worker_record_array);

		sort_worker(); // if this one selected, refresh interface
	}

	return 1;
}
//----------- End of function Firm::read_file ---------//


//--------- Begin of function Firm::write_derived_file ---------//
//
// Write data in derived class.
//
// If the derived Firm don't have any special data,
// just use Firm::write_file(), otherwise make its own derived copy of write_file()
//
int Firm::write_derived_file(File* filePtr)
{
   //--- write data in derived class -----//

   int writeSize = firm_array.firm_class_size(firm_id)-sizeof(Firm);

   if( writeSize > 0 )
   {
      if( !filePtr->file_write( (char*) this + sizeof(Firm), writeSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Firm::write_derived_file ---------//


//--------- Begin of function Firm::read_derived_file ---------//
//
// Read data in derived class.
//
// If the derived Firm don't have any special data,
// just use Firm::read_file(), otherwise make its own derived copy of read_file()
//
int Firm::read_derived_file(File* filePtr)
{
   //--- read data in derived class -----//

   int readSize = firm_array.firm_class_size(firm_id)-sizeof(Firm);

   if( readSize > 0 )
   {
      if( !filePtr->file_read( (char*) this + sizeof(Firm), readSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Firm::read_derived_file ---------//


//--------- Begin of function FirmBase::write_derived_file ---------//
int FirmBase::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_base);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmBaseGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmBase::write_derived_file ---------//


//--------- Begin of function FirmBase::read_derived_file ---------//
int FirmBase::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmBaseGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_base);
	return 1;
}
//--------- End of function FirmBase::read_derived_file ---------//


//--------- Begin of function FirmCamp::write_derived_file ---------//
int FirmCamp::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_camp);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmCampGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmCamp::write_derived_file ---------//


//--------- Begin of function FirmCamp::read_derived_file ---------//
int FirmCamp::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmCampGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_camp);
	return 1;
}
//--------- End of function FirmCamp::read_derived_file ---------//


//--------- Begin of function FirmFactory::write_derived_file ---------//
int FirmFactory::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_factory);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmFactoryGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmFactory::write_derived_file ---------//


//--------- Begin of function FirmFactory::read_derived_file ---------//
int FirmFactory::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmFactoryGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_factory);
	return 1;
}
//--------- End of function FirmFactory::read_derived_file ---------//


//--------- Begin of function FirmHarbor::write_derived_file ---------//
int FirmHarbor::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_harbor);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmHarborGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmHarbor::write_derived_file ---------//


//--------- Begin of function FirmHarbor::read_derived_file ---------//
int FirmHarbor::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmHarborGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_harbor);
	return 1;
}
//--------- End of function FirmHarbor::read_derived_file ---------//


//--------- Begin of function FirmInn::write_derived_file ---------//
int FirmInn::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_inn);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmInnGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmInn::write_derived_file ---------//


//--------- Begin of function FirmInn::read_derived_file ---------//
int FirmInn::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmInnGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_inn);
	return 1;
}
//--------- End of function FirmInn::read_derived_file ---------//


//--------- Begin of function FirmMarket::write_derived_file ---------//
int FirmMarket::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_market);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmMarketGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmMarket::write_derived_file ---------//


//--------- Begin of function FirmMarket::read_derived_file ---------//
int FirmMarket::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmMarketGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_market);

	//----- reset market_raw_array[] & market_product_array[] ----//

	int i;
	for( i=0 ; i<MAX_RAW ; i++ )
	{
		market_raw_array[i]     = NULL;
		market_product_array[i] = NULL;
	}

	//------- rebuild market_product_array --------//

	int rawId, productId;

	for( i=0 ; i<MAX_MARKET_GOODS ; i++ )
	{
		rawId     = market_goods_array[i].raw_id;
		productId = market_goods_array[i].product_raw_id;

		if( rawId )
			market_raw_array[rawId-1] = market_goods_array + i;

		if( productId )
			market_product_array[productId-1] = market_goods_array + i;
	}

        //---- force ai to update restocking type and links after load ----//

	if( firm_ai )
		ai_link_checked = 0;

	if( config_adv.game_file_patching &&
		game_file_array.load_file_game_version < 200 &&
		firm_id == FIRM_MARKET )
	{
		// Below game version 200, the restock type was not initialized
		// for human players.
		if( !firm_ai )
			restock_type = 0;
	}

	return 1;
}
//--------- End of function FirmMarket::read_derived_file ---------//


//--------- Begin of function FirmMine::write_derived_file ---------//
int FirmMine::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_mine);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmMineGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmMine::write_derived_file ---------//


//--------- Begin of function FirmMine::read_derived_file ---------//
int FirmMine::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmMineGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_mine);
	return 1;
}
//--------- End of function FirmMine::read_derived_file ---------//


//--------- Begin of function FirmMonster::write_derived_file ---------//
int FirmMonster::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_monster);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmMonsterGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmMonster::write_derived_file ---------//


//--------- Begin of function FirmMonster::read_derived_file ---------//
int FirmMonster::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmMonsterGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_monster);
	return 1;
}
//--------- End of function FirmMonster::read_derived_file ---------//


//--------- Begin of function FirmResearch::write_derived_file ---------//
int FirmResearch::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_research);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmResearchGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmResearch::write_derived_file ---------//


//--------- Begin of function FirmResearch::read_derived_file ---------//
int FirmResearch::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmResearchGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_research);
	return 1;
}
//--------- End of function FirmResearch::read_derived_file ---------//


//--------- Begin of function FirmWar::write_derived_file ---------//
int FirmWar::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.firm_war);
	if( !filePtr->file_write(&gf_rec, sizeof(FirmWarGF)) )
		return 0;
	return 1;
}
//--------- End of function FirmWar::write_derived_file ---------//


//--------- Begin of function FirmWar::read_derived_file ---------//
int FirmWar::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(FirmWarGF)) )
		return 0;
	read_derived_record(&gf_rec.firm_war);
	return 1;
}
//--------- End of function FirmWar::read_derived_file ---------//


//*****//


//-------- Start of function SiteArray::write_file -------------//
//
int SiteArray::write_file(File* filePtr)
{
	filePtr->file_put_short(selected_recno);
	filePtr->file_put_short(untapped_raw_count);
	filePtr->file_put_short(scroll_count);
	filePtr->file_put_short(gold_coin_count);
	filePtr->file_put_short(std_raw_site_count);

	write_record(&gf_rec.dyn_array);
	if( !filePtr->file_write(&gf_rec, sizeof(DynArrayGF)) )
		return 0;

	//---------- write body_buf ---------//

	if( last_ele > 0 )
	{
		SiteGF *site_record_array = (SiteGF*) mem_add(sizeof(SiteGF)*last_ele);
		for( int i=1; i<=last_ele; i++ )
		{
			Site *sitePtr = (Site*) get(i);
			sitePtr->write_record(site_record_array+i-1);
		}
		if( !filePtr->file_write(site_record_array, sizeof(SiteGF)*last_ele) )
		{
			mem_del(site_record_array);
			return 0;
		}
		mem_del(site_record_array);
	}

	//---------- write empty_room_array ---------//

	write_empty_room(filePtr);

	return 1;
}
//--------- End of function SiteArray::write_file ---------------//


//-------- Start of function SiteArray::read_file -------------//
//
int SiteArray::read_file(File* filePtr)
{
	selected_recno		 = filePtr->file_get_short();
	untapped_raw_count =	filePtr->file_get_short();
	scroll_count		 = filePtr->file_get_short();
	gold_coin_count	 =	filePtr->file_get_short();
	std_raw_site_count =	filePtr->file_get_short();

	if( !filePtr->file_read(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	read_record(&gf_rec.dyn_array);

	//---------- read body_buf ---------//

	body_buf = mem_resize(body_buf, ele_num*ele_size);

	if( last_ele > 0 )
	{
		SiteGF *site_record_array = (SiteGF*) mem_add(sizeof(SiteGF)*last_ele);
		if( !filePtr->file_read(site_record_array, sizeof(SiteGF)*last_ele) )
		{
			mem_del(site_record_array);
			return 0;
		}
		for( int i=1; i<=last_ele; i++ )
		{
			Site *sitePtr = (Site*) get(i);
			sitePtr->read_record(site_record_array+i-1);
		}
		mem_del(site_record_array);
	}

	//---------- read empty_room_array ---------//

	read_empty_room(filePtr);

	//------------------------------------------//

	start();    // go top

	return 1;
}
//--------- End of function SiteArray::read_file ---------------//


//*****//


//-------- Start of function TownArray::write_file -------------//
//
int TownArray::write_file(File* filePtr)
{
   int  i;
   Town *townPtr;

	filePtr->file_put_short( size()  );  // no. of towns in town_array
	filePtr->file_put_short( selected_recno );
	write_record(&gf_rec.town_array);
	filePtr->file_write(&gf_rec, sizeof(TownArrayGF));

	filePtr->file_put_short( Town::if_town_recno );

	//-----------------------------------------//

	for( i=1; i<=size() ; i++ )
	{
		townPtr = (Town*) get_ptr(i);

      //----- write townId or 0 if the town is deleted -----//

      if( !townPtr )    // the town is deleted
      {
         filePtr->file_put_short(0);
      }
      else
		{
			#ifdef DEBUG
				townPtr->verify_slot_object_id_array();		// for debugging only
			#endif

			filePtr->file_put_short(1);      // the town exists

			townPtr->write_record(&gf_rec.town);
			if( !filePtr->file_write(&gf_rec, sizeof(TownGF)) )
				return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function TownArray::write_file ---------------//


//-------- Start of function TownArray::read_file -------------//
//
int TownArray::read_file(File* filePtr)
{
   Town*   townPtr;
   int     i;

	int townCount = filePtr->file_get_short();  // get no. of towns from file
	selected_recno = filePtr->file_get_short();

	if(!game_file_array.same_version)
	{
		filePtr->file_read(&gf_rec, sizeof(Version_1_TownArrayGF));
		read_record_v1(&gf_rec.town_array_v1);
	}
	else
	{
		filePtr->file_read(&gf_rec, sizeof(TownArrayGF));
		read_record(&gf_rec.town_array);
	}

	Town::if_town_recno = filePtr->file_get_short();

	//------------------------------------------//

	for( i=1 ; i<=townCount ; i++ )
	{
		if( filePtr->file_get_short()==0 )  // the town has been deleted
		{
			add_blank(1);     // it's a DynArrayB function
		}
		else
		{
			townPtr = town_array.create_town();

			if(!game_file_array.same_version)
			{
				Version_1_Town *oldTown = (Version_1_Town*) mem_add(sizeof(Version_1_Town));
				if( !filePtr->file_read(&gf_rec, sizeof(Version_1_TownGF)) )
				{
					mem_del(oldTown);
					return 0;
				}

				oldTown->read_record(&gf_rec.town_v1);
				oldTown->convert_to_version_2(townPtr);
				mem_del(oldTown);
			}
			else
			{
				if( !filePtr->file_read(&gf_rec, sizeof(TownGF)) )
					return 0;
				townPtr->read_record(&gf_rec.town);
			}

			#ifdef DEBUG
				townPtr->verify_slot_object_id_array();		// for debugging only
			#endif
		}
	}

	//-------- linkout() those record added by add_blank() ----------//
	//-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since TownArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

	//------- read empty room array --------//

	read_empty_room(filePtr);

	return 1;
}
//--------- End of function TownArray::read_file ---------------//


//*****//


//-------- Start of function NationArray::write_file -------------//
//
int NationArray::write_file(File* filePtr)
{
	//------ write info in NationArray ------//

	write_record(&gf_rec.nation_array);
	if( !filePtr->file_write(&gf_rec, sizeof(NationArrayGF)) )
		return 0;

   //---------- write Nations --------------//

   int    i;
   Nation *nationPtr;

   filePtr->file_put_short( size() );  // no. of nations in nation_array

   for( i=1; i<=size() ; i++ )
   {
      nationPtr = (Nation*) get_ptr(i);

      //----- write nationId or 0 if the nation is deleted -----//

      if( !nationPtr )    // the nation is deleted
      {
         filePtr->file_put_short(0);
      }
      else
      {
         filePtr->file_put_short(1);      // there is a nation in this record

         //------ write data in the base class ------//

         if( !nationPtr->write_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function NationArray::write_file -------------//


//-------- Start of function NationArray::read_file -------------//
//
int NationArray::read_file(File* filePtr)
{
   //------ read info in NationArray ------//
	if(!game_file_array.same_version)
	{
		Version_1_NationArray *oldNationArrayPtr = (Version_1_NationArray*) mem_add(sizeof(Version_1_NationArray));
		if( !filePtr->file_read(&gf_rec, sizeof(Version_1_NationArrayGF)) )
		{
			mem_del(oldNationArrayPtr);
			return 0;
		}
		oldNationArrayPtr->read_record(&gf_rec.nation_array_v1);
		oldNationArrayPtr->convert_to_version_2(this);
		mem_del(oldNationArrayPtr);
	}
	else
	{
		if( !filePtr->file_read(&gf_rec, sizeof(NationArrayGF)) )
			return 0;
		read_record(&gf_rec.nation_array);
	}

   //---------- read Nations --------------//

   int     i, nationRecno, nationCount;
   Nation* nationPtr;

   nationCount = filePtr->file_get_short();  // get no. of nations from file

   for( i=1 ; i<=nationCount ; i++ )
   {
      if( filePtr->file_get_short() == 0 )
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create nation object -----------//

         nationRecno = create_nation();
         nationPtr   = nation_array[nationRecno];

         //----- read data in base class --------//

         if( !nationPtr->read_file( filePtr ) )
            return 0;
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since NationArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

	//-------- set NationArray::player_ptr -----------//

   player_ptr = nation_array[player_recno];

	//------- read empty room array --------//

	read_empty_room(filePtr);

	return 1;
}
//--------- End of function NationArray::read_file ---------------//


//--------- Begin of function Nation::write_file ---------//
//
int Nation::write_file(File* filePtr)
{
	write_record(&gf_rec.nation);
	if( !filePtr->file_write(&gf_rec, sizeof(NationGF)) )
		return 0;

	//----------- write AI Action Array ------------//

	action_array.write_record(&gf_rec.dyn_array);
	if( !filePtr->file_write(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	if( action_array.last_ele )
	{
		ActionNodeGF *action_node_array = (ActionNodeGF*)mem_add(sizeof(ActionNodeGF)*action_array.last_ele);
		for( int i=1; i<=action_array.last_ele; i++ )
		{
			ActionNode *actionNodePtr = (ActionNode*)action_array.get(i);
			actionNodePtr->write_record(action_node_array+i-1);
		}
		if( !filePtr->file_write(action_node_array, sizeof(ActionNodeGF)*action_array.last_ele) )
		{
			mem_del(action_node_array);
			return 0;
		}
		mem_del(action_node_array);
	}

	//------ write AI info array ---------//

	write_ai_info(filePtr, ai_town_array, ai_town_count, ai_town_size);

	write_ai_info(filePtr, ai_base_array, ai_base_count, ai_base_size);
	write_ai_info(filePtr, ai_mine_array, ai_mine_count, ai_mine_size);
	write_ai_info(filePtr, ai_factory_array, ai_factory_count, ai_factory_size);
	write_ai_info(filePtr, ai_market_array, ai_market_count, ai_market_size);
	write_ai_info(filePtr, ai_inn_array, ai_inn_count, ai_inn_size);
	write_ai_info(filePtr, ai_camp_array, ai_camp_count, ai_camp_size);
	write_ai_info(filePtr, ai_research_array, ai_research_count, ai_research_size);
	write_ai_info(filePtr, ai_war_array, ai_war_count, ai_war_size);
	write_ai_info(filePtr, ai_harbor_array, ai_harbor_count, ai_harbor_size);

	write_ai_info(filePtr, ai_caravan_array, ai_caravan_count, ai_caravan_size);
	write_ai_info(filePtr, ai_ship_array, ai_ship_count, ai_ship_size);
	write_ai_info(filePtr, ai_general_array, ai_general_count, ai_general_size);

	return 1;
}
//----------- End of function Nation::write_file ---------//


//--------- Begin of static function write_ai_info ---------//
//
static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize)
{
	filePtr->file_put_short( aiInfoCount );
	filePtr->file_put_short( aiInfoSize  );
	filePtr->file_put_short_array( aiInfoArray, aiInfoCount );
}
//----------- End of static function write_ai_info ---------//


//--------- Begin of function Nation::read_file ---------//
//
int Nation::read_file(File* filePtr)
{
	if(!game_file_array.same_version)
	{
		Version_1_Nation *oldNationPtr = (Version_1_Nation*) mem_add(sizeof(Version_1_Nation));
		if( !filePtr->file_read(&gf_rec, sizeof(Version_1_NationGF)) )
		{
			mem_del(oldNationPtr);
			return 0;
		}
		oldNationPtr->read_record(&gf_rec.nation_v1);
		oldNationPtr->convert_to_version_2(this);
		mem_del(oldNationPtr);
	}
	else
	{
		if( !filePtr->file_read(&gf_rec, sizeof(NationGF)) )
			return 0;
		read_record(&gf_rec.nation);
	}

	//-------------- read AI Action Array --------------//

	if( !filePtr->file_read(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	action_array.read_record(&gf_rec.dyn_array);
	action_array.resize(action_array.ele_num); //alloc body_buf again
	if( action_array.last_ele )
	{
		ActionNodeGF *action_node_array = (ActionNodeGF*)mem_add(sizeof(ActionNodeGF)*action_array.last_ele);
		if( !filePtr->file_read(action_node_array, sizeof(ActionNodeGF)*action_array.last_ele) )
		{
			mem_del(action_node_array);
			return 0;
		}
		for( int i=1; i<=action_array.last_ele; i++ )
		{
			ActionNode *actionNodePtr = (ActionNode*)action_array.get(i);
			actionNodePtr->read_record(action_node_array+i-1);
		}
		mem_del(action_node_array);
	}

	//------ write AI info array ---------//

	read_ai_info(filePtr, &ai_town_array, ai_town_count, ai_town_size);

	read_ai_info(filePtr, &ai_base_array, ai_base_count, ai_base_size);
	read_ai_info(filePtr, &ai_mine_array, ai_mine_count, ai_mine_size);
	read_ai_info(filePtr, &ai_factory_array, ai_factory_count, ai_factory_size);
	read_ai_info(filePtr, &ai_market_array, ai_market_count, ai_market_size);
	read_ai_info(filePtr, &ai_inn_array, ai_inn_count, ai_inn_size);
	read_ai_info(filePtr, &ai_camp_array, ai_camp_count, ai_camp_size);
	read_ai_info(filePtr, &ai_research_array, ai_research_count, ai_research_size);
	read_ai_info(filePtr, &ai_war_array, ai_war_count, ai_war_size);
	read_ai_info(filePtr, &ai_harbor_array, ai_harbor_count, ai_harbor_size);

	read_ai_info(filePtr, &ai_caravan_array, ai_caravan_count, ai_caravan_size);
	read_ai_info(filePtr, &ai_ship_array, ai_ship_count, ai_ship_size);
	read_ai_info(filePtr, &ai_general_array, ai_general_count, ai_general_size);

	return 1;
}
//----------- End of function Nation::read_file ---------//


//--------- Begin of static function read_ai_info ---------//
//
static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize)
{
	aiInfoCount = filePtr->file_get_short();
	aiInfoSize  = filePtr->file_get_short();

	// Validate sizes
	if (aiInfoCount < 0 || aiInfoSize < 0 || aiInfoCount > 10000 || aiInfoSize > 10000) {
		// Invalid/corrupt data, do not allocate
		if (*aiInfoArrayPtr) {
			mem_del(*aiInfoArrayPtr);
			*aiInfoArrayPtr = nullptr;
		}
		aiInfoCount = 0;
		aiInfoSize = 0;
		return;
	}

	short actualSize = (aiInfoCount < aiInfoSize) ? aiInfoCount : aiInfoSize;

	if (*aiInfoArrayPtr) {
		mem_del(*aiInfoArrayPtr);
		*aiInfoArrayPtr = nullptr;
	}

	if (actualSize > 0) {
		*aiInfoArrayPtr = (short*) mem_add(actualSize * sizeof(short));
		if (*aiInfoArrayPtr) {
			filePtr->file_get_short_array(*aiInfoArrayPtr, actualSize);
		} else {
			aiInfoCount = 0;
			aiInfoSize = 0;
			return;
		}
	} else {
		*aiInfoArrayPtr = nullptr;
	}

	aiInfoCount = actualSize;
	aiInfoSize = actualSize;
}
//----------- End of static function read_ai_info ---------//

//*****//

//-------- Start of function TornadoArray::write_file -------------//
//
int TornadoArray::write_file(File* filePtr)
{
	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	int    i;
   Tornado *tornadoPtr;

   filePtr->file_put_short( size() );  // no. of tornados in tornado_array

   for( i=1; i<=size() ; i++ )
   {
      tornadoPtr = (Tornado*) get_ptr(i);

      //----- write tornadoId or 0 if the tornado is deleted -----//

      if( !tornadoPtr )    // the tornado is deleted
      {
         filePtr->file_put_short(0);
      }
      else
      {
         filePtr->file_put_short(1);      // there is a tornado in this record

         //------ write data in the base class ------//

         if( !tornadoPtr->write_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function TornadoArray::write_file -------------//


//-------- Start of function TornadoArray::read_file -------------//
//
int TornadoArray::read_file(File* filePtr)
{
	restart_recno    = filePtr->file_get_short();

   int     i, tornadoRecno, tornadoCount;
   Tornado* tornadoPtr;

   tornadoCount = filePtr->file_get_short();  // get no. of tornados from file

   for( i=1 ; i<=tornadoCount ; i++ )
   {
      if( filePtr->file_get_short() == 0 )
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create tornado object -----------//

         tornadoRecno = tornado_array.create_tornado();
         tornadoPtr   = tornado_array[tornadoRecno];

         //----- read data in base class --------//

         if( !tornadoPtr->read_file( filePtr ) )
            return 0;
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since TornadoArray has its own go() which will call GroupArray::go()
      void* ptr = get_ptr();
      if( ptr == NULL )       // add_blank() record
         linkout();
#ifdef DEBUG
      else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
         err_here(); // Suspicious pointer detected
      }
#endif
   }

   //------- read empty room array --------//

   read_empty_room(filePtr);

   return 1;
}
//--------- End of function TornadoArray::read_file ---------------//


//--------- Begin of function Tornado::write_file ---------//
//
int Tornado::write_file(File* filePtr)
{
	write_record(&gf_rec.tornado);
	if( !filePtr->file_write(&gf_rec, sizeof(TornadoGF)) )
		return 0;
	return 1;
}
//----------- End of function Tornado::write_file ---------//


//--------- Begin of function Tornado::read_file ---------//
//
int Tornado::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(TornadoGF)) )
		return 0;
	read_record(&gf_rec.tornado);

	//------------ post-process the data read ----------//

	sprite_info = sprite_res[sprite_id];

	sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Tornado::read_file ---------//


//*****//


//-------- Start of function RebelArray::write_file -------------//
//
int RebelArray::write_file(File* filePtr)
{
	int   i;

	filePtr->file_put_short(size());

	for( i=1; i<=size(); i++ )
	{
		Rebel *rebelPtr = (Rebel*) get_ptr(i);

		//----- write 0 if the object is deleted -----//

		if( !rebelPtr )    // the object is deleted
		{
			filePtr->file_put_short(0);
		}
		else    // the object exists
		{
			filePtr->file_put_short(1);

			rebelPtr->write_record(&gf_rec.rebel);
			if( !filePtr->file_write(&gf_rec, sizeof(RebelGF)) )
				return 0;
		}
	}

	//------- write empty room array --------//

	write_empty_room(filePtr);

	return 1;
}
//--------- End of function RebelArray::write_file ---------------//


//-------- Start of function RebelArray::read_file -------------//
//
int RebelArray::read_file(File* filePtr)
{
	int   i;

	int eleCount = filePtr->file_get_short();

	for( i=1; i<=eleCount; i++ )
	{
		if( filePtr->file_get_short()==0 )    // the object is deleted
		{
			add_blank(1);    // it's a DynArrayB function
		}
		else    // the object exists
		{
			if( !filePtr->file_read(&gf_rec, sizeof(RebelGF)) )
				return 0;

			Rebel *rebelPtr = new Rebel;
			rebelPtr->read_record(&gf_rec.rebel);

			rebel_array.linkin(&rebelPtr);
		}
	}

	//-------- linkout() those record added by add_blank() ----------//
	//-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

	for( i=size(); i>0; i-- )
	{
		DynArrayB::go(i);             // since DynArrayB has its own go() which will call GroupArray::go()
		void* ptr = get_ptr();
		if( ptr == NULL )       // add_blank() record
			linkout();
#ifdef DEBUG
		else if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff) {
			err_here(); // Suspicious pointer detected
		}
#endif
	}

	//------- read empty room array --------//

	read_empty_room(filePtr);

	return 1;
}
//--------- End of function RebelArray::read_file ---------------//


//*****//


//-------- Start of function SpyArray::write_file -------------//
//
int SpyArray::write_file(File* filePtr)
{
	filePtr->file_put_unsigned_short(29); // sizeof(DynArray)
	// write DynArray -- 29 bytes
	filePtr->file_put_long(ele_num);
	filePtr->file_put_long(block_num);
	filePtr->file_put_long(cur_pos);
	filePtr->file_put_long(last_ele);
	filePtr->file_put_long(15); // sizeof(Spy)
	filePtr->file_put_long(sort_offset);
	filePtr->file_put_char(sort_type);
	filePtr->file_put_long(0);

        //---------- write body_buf ---------//

	if( last_ele > 0 )
	{
		Spy spyBlank;
		memset(&spyBlank, 0, sizeof(spyBlank));

		filePtr->file_put_unsigned_short(15*last_ele); // sizeof(Spy)*last_ele

		for( int i=1; i<=last_ele; i++ )
		{
			Spy* spyPtr = (Spy*) get_ptr(i);
			if( !spyPtr )
				spyPtr = &spyBlank;

			// write Spy -- 15 bytes
			filePtr->file_put_short(spyPtr->spy_recno);
			filePtr->file_put_char(spyPtr->spy_place);
			filePtr->file_put_short(spyPtr->spy_place_para);
			filePtr->file_put_char(spyPtr->spy_skill);
			filePtr->file_put_char(spyPtr->spy_loyalty);
			filePtr->file_put_char(spyPtr->true_nation_recno);
			filePtr->file_put_char(spyPtr->cloaked_nation_recno);
			filePtr->file_put_char(spyPtr->notify_cloaked_nation_flag);
			filePtr->file_put_char(spyPtr->exposed_flag);
			filePtr->file_put_char(spyPtr->race_id);
			filePtr->file_put_unsigned_short(spyPtr->name_id);
			filePtr->file_put_char(spyPtr->action_mode);
		}
	}

	//---------- write empty_room_array ---------//

	write_empty_room(filePtr);

	return 1;
}
//--------- End of function SpyArray::write_file ---------------//


//-------- Start of function SpyArray::read_file -------------//
//
int SpyArray::read_file(File* filePtr)
{
	unsigned short recSize = filePtr->file_get_unsigned_short();
	if( recSize != 29 )
		return 0;

	int32_t fileEleNum = filePtr->file_get_long(); // skip overwriting ele_num
	resize(fileEleNum);
	filePtr->file_get_long(); // skip overwriting block_num
	filePtr->file_get_long(); // skip overwriting cur_pos
	int32_t readNum = filePtr->file_get_long(); // skip overwriting last_ele
	filePtr->file_get_long(); // skip overwriting ele_size
	filePtr->file_get_long(); // skip overwriting sort_offset
	filePtr->file_get_char(); // skip overwriting sort_type
	filePtr->file_get_long();

	//---------- read body_buf ---------//

	if( readNum > 0 )
	{
		filePtr->file_get_unsigned_short(); // skip body_buf record len

		for( int i=0; i<readNum; i++ )
		{
			int spy_recno = filePtr->file_get_short();

			if( spy_recno )
			{
				spy_recno = spy_array.add_spy();

				Spy* spyPtr = spy_array[spy_recno];
				//spyPtr->spy_recno = spy_recno;
				spyPtr->spy_place = filePtr->file_get_char();
				spyPtr->spy_place_para = filePtr->file_get_short();
				spyPtr->spy_skill = filePtr->file_get_char();
				spyPtr->spy_loyalty = filePtr->file_get_char();
				spyPtr->true_nation_recno = filePtr->file_get_char();
				spyPtr->cloaked_nation_recno = filePtr->file_get_char();
				spyPtr->notify_cloaked_nation_flag = filePtr->file_get_char();
				spyPtr->exposed_flag = filePtr->file_get_char();
				spyPtr->race_id = filePtr->file_get_char();
				spyPtr->name_id = filePtr->file_get_unsigned_short();
				spyPtr->action_mode = filePtr->file_get_char();
			}
			else
			{
				add_blank(1);     // it's a DynArrayB function

				// read 13 zeroed bytes
				filePtr->file_get_char(); //skip spy_place
				filePtr->file_get_short(); //skip spy_place_para
				filePtr->file_get_char(); //skip spy_skill
				filePtr->file_get_char(); // skip spy_loyalty
				filePtr->file_get_char(); // skip true_nation_recno
				filePtr->file_get_char(); // skip cloaked_nation_recno
				filePtr->file_get_char(); // skip notify_cloaked_nation_flag
				filePtr->file_get_char(); // skip exposed_flag
				filePtr->file_get_char(); // skip race_id
				filePtr->file_get_unsigned_short(); // skip name_id
				filePtr->file_get_char(); // skip action_mode
			}
		}
	}

	//---------- read empty_room_array ---------//

	read_empty_room(filePtr);

	//------------------------------------------//

	start();    // go top

	return 1;
}
//--------- End of function SpyArray::read_file ---------------//


//*****//


//-------- Start of function SnowGroundArray::write_file -------------//
//
int SnowGroundArray::write_file(File* filePtr)
{
	write_record(&gf_rec.snow_ground_array);
	if( !filePtr->file_write(&gf_rec, sizeof(SnowGroundArrayGF)) )
		return 0;

	return 1;
}
//--------- End of function SnowGroundArray::write_file ---------------//


//-------- Start of function SnowGroundArray::read_file -------------//
//
int SnowGroundArray::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(SnowGroundArrayGF)) )
		return 0;
	read_record(&gf_rec.snow_ground_array);

	return 1;
}
//--------- End of function SnowGroundArray::read_file ---------------//

//*****//

//-------- Start of function RegionArray::write_file -------------//
//
int RegionArray::write_file(File* filePtr)
{
	write_record(&gf_rec.region_array);
	if( !filePtr->file_write(&gf_rec, sizeof(RegionArrayGF)) )
		return 0;

	RegionInfoGF* region_info_record_array = (RegionInfoGF*) mem_add(sizeof(RegionInfoGF)*region_info_count);
	for( int i=0; i<region_info_count; i++ )
	{
		RegionInfo* region = region_info_array+i;
		region->write_record(region_info_record_array+i);
	}
	if( !filePtr->file_write(region_info_record_array, sizeof(RegionInfoGF)*region_info_count) )
	{
		mem_del(region_info_record_array);
		return 0;
	}
	mem_del(region_info_record_array);

	//-------- write RegionStat ----------//

	filePtr->file_put_short( region_stat_count );

	RegionStatGF* region_stat_record_array = (RegionStatGF*) mem_add(sizeof(RegionStatGF)*region_stat_count);
	for( int i=0; i<region_stat_count; i++ )
	{
		RegionStat* region = region_stat_array+i;
		region->write_record(region_stat_record_array+i);
	}
	if( !filePtr->file_write(region_stat_record_array, sizeof(RegionStatGF)*region_stat_count) )
	{
		mem_del(region_stat_record_array);
		return 0;
	}
	mem_del(region_stat_record_array);

	//--------- write connection bits ----------//

	int connectBit = (region_info_count -1) * (region_info_count) /2;
	int connectByte = (connectBit +7) /8;

	if( connectByte > 0)
	{
		if( !filePtr->file_write(connect_bits, connectByte) )
			return 0;
	}

	return 1;
}
//--------- End of function RegionArray::write_file ---------------//


//-------- Start of function RegionArray::read_file -------------//
//
int RegionArray::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(RegionArrayGF)) )
		return 0;
	read_record(&gf_rec.region_array);

	if( region_info_count > 0 )
	{
		RegionInfoGF* region_info_record_array = (RegionInfoGF*) mem_add(sizeof(RegionInfoGF)*region_info_count);
		if( !filePtr->file_read(region_info_record_array, sizeof(RegionInfoGF)*region_info_count) )
		{
			mem_del(region_info_record_array);
			return 0;
		}

		// Free existing memory if it exists to prevent memory leaks
		if (region_info_array)
		{
			mem_del(region_info_array);
		}

		region_info_array = (RegionInfo *) mem_add(sizeof(RegionInfo)*region_info_count);

		for( int i=0; i<region_info_count; i++ )
		{
			RegionInfo* region = region_info_array+i;
			region->read_record(region_info_record_array+i);
		}
		mem_del(region_info_record_array);
	}
	else
	{
		// Free existing memory if it exists to prevent memory leaks
		if (region_info_array)
		{
			mem_del(region_info_array);
		}
		region_info_array = NULL;
	}

	//-------- read RegionStat ----------//

	region_stat_count = filePtr->file_get_short();

	RegionStatGF* region_stat_record_array = (RegionStatGF*) mem_add(sizeof(RegionStatGF)*region_stat_count);
	if( !filePtr->file_read(region_stat_record_array, sizeof(RegionStatGF)*region_stat_count) )
	{
		mem_del(region_stat_record_array);
		return 0;
	}

	// Free existing memory if it exists to prevent memory leaks
	if (region_stat_array)
	{
		mem_del(region_stat_array);
	}

	region_stat_array = (RegionStat*) mem_add(region_stat_count*sizeof(RegionStat) );

	for( int i=0; i<region_stat_count; i++ )
	{
		RegionStat* region = region_stat_array+i;
		region->read_record(region_stat_record_array+i);
	}
	mem_del(region_stat_record_array);

	//--------- read connection bits ----------//

	int connectBit = (region_info_count -1) * (region_info_count) /2;
	int connectByte = (connectBit +7) /8;

	if( connectByte > 0)
	{
		// Free existing memory if it exists to prevent memory leaks
		if (connect_bits)
		{
			mem_del(connect_bits);
		}
		
		connect_bits = (unsigned char *)mem_add(connectByte);
		if( !filePtr->file_read(connect_bits, connectByte) )
			return 0;
	}
	else
	{
		// Free existing memory if it exists to prevent memory leaks
		if (connect_bits)
		{
			mem_del(connect_bits);
		}
		connect_bits = NULL;
	}

	return 1;
}
//--------- End of function RegionArray::read_file ---------------//

//*****//

//-------- Start of function NewsArray::write_file -------------//
//
int NewsArray::write_file(File* filePtr)
{
	//----- save news_array parameters -----//

	filePtr->file_write(news_type_option, sizeof(news_type_option));

	filePtr->file_put_short(news_who_option);
	filePtr->file_put_long(last_clear_recno);

	//---------- save news data -----------//

	write_record(&gf_rec.dyn_array);
	if( !filePtr->file_write(&gf_rec, sizeof(DynArrayGF)) )
		return 0;

	if( last_ele > 0 )
	{
		NewsGF *news_record_array = (NewsGF*) mem_add(sizeof(NewsGF)*last_ele);
		for( int i=1; i<=last_ele; i++ )
		{
			News *newsPtr = (News*) get(i);
			newsPtr->write_record(news_record_array+i-1);
		}
		if( !filePtr->file_write(news_record_array, sizeof(NewsGF)*last_ele) )
		{
			mem_del(news_record_array);
			return 0;
		}
		mem_del(news_record_array);
	}

	return 1;
}
//--------- End of function NewsArray::write_file ---------------//


//-------- Start of function NewsArray::read_file -------------//
//
int NewsArray::read_file(File* filePtr)
{
	//----- read news_array parameters -----//

	filePtr->file_read(news_type_option, sizeof(news_type_option));

	news_who_option   = (char) filePtr->file_get_short();
	last_clear_recno  = filePtr->file_get_long();

	//---------- read news data -----------//

	if( !filePtr->file_read(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	read_record(&gf_rec.dyn_array);

	body_buf = mem_resize(body_buf, ele_num*ele_size);

	if( last_ele > 0 )
	{
		NewsGF *news_record_array = (NewsGF*) mem_add(sizeof(NewsGF)*last_ele);
		if( !filePtr->file_read(news_record_array, sizeof(NewsGF)*last_ele) )
		{
			mem_del(news_record_array);
			return 0;
		}
		for( int i=1; i<=last_ele; i++ )
		{
			News *newsPtr = (News*) get(i);
			newsPtr->read_record(news_record_array+i-1);
		}
		mem_del(news_record_array);
	}

	start();    // go top

	return 1;
}
//--------- End of function NewsArray::read_file ---------------//
