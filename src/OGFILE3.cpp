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
#include <dbglog.h>

DBGLOG_DEFAULT_CHANNEL(GameFile);

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
	// MSG("UnitArray::read_file: Starting\n");
	long filePosStart = filePtr->file_pos();
	// MSG("UnitArray::read_file: Starting file position: %ld\n", filePosStart);
	
	Unit*   unitPtr;
	int     i, unitId, emptyRoomCount=0;

	// MSG("UnitArray::read_file: Reading restart_recno\n");
	restart_recno    = filePtr->file_get_short();

	// MSG("UnitArray::read_file: Reading unitCount\n");
	int unitCount    = filePtr->file_get_short();  // get no. of units from file
	// MSG("UnitArray::read_file: unitCount=%d\n", unitCount);

	// MSG("UnitArray::read_file: Reading selected_recno, selected_count\n");
	selected_recno   = filePtr->file_get_short();
	selected_count   = filePtr->file_get_short();
	// MSG("UnitArray::read_file: Reading cur_group_id, cur_team_id\n");
	cur_group_id     = filePtr->file_get_long();
	cur_team_id      = filePtr->file_get_long();
	// MSG("UnitArray::read_file: Reading idle_blocked_unit_reset_count, unit_search_tries\n");
	idle_blocked_unit_reset_count = filePtr->file_get_short();
	unit_search_tries	= filePtr->file_get_long ();
	unit_search_tries_flag = (char) filePtr->file_get_short();

	// MSG("UnitArray::read_file: Reading visible_unit_count and unused shorts\n");
   visible_unit_count					= filePtr->file_get_short();
	// unused short*4
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();

   long filePosBeforeUnits = filePtr->file_pos();
   // MSG("UnitArray::read_file: File position before reading units: %ld\n", filePosBeforeUnits);
   
   // MSG("UnitArray::read_file: Starting to read %d units\n", unitCount);
   for( i=1 ; i<=unitCount ; i++ )
   {
      long filePosBeforeUnitId = filePtr->file_pos();
      unitId = filePtr->file_get_short();
      // MSG("UnitArray::read_file: Unit %d/%d: unitId=%d at position %ld\n", i, unitCount, unitId, filePosBeforeUnitId);

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
			ERR("UnitArray::read_file: Invalid unitId=%d (0x%04x), unit_info_count=%d, i=%d/%d - this suggests file corruption or misalignment\n", 
				unitId, unitId, unit_res.unit_info_count, i, unitCount);
			
			// Try to recover: if unitId looks like it might be part of unit data (e.g., sprite_id = 256),
			// we might be misaligned due to old format writing unit data for deleted units.
			// Seek back 2 bytes (we just read the short) and try to read UnitGF to get the real unit_id
			if( unitId >= 256 && unitId < 1000 && i > 1 ) {
				// MSG("UnitArray::read_file: Attempting recovery: unitId=%d might be sprite_id from misaligned read\n", unitId);
				long currentPos = filePtr->file_pos();
				// MSG("UnitArray::read_file: Current file position: %ld, seeking back 2 bytes\n", currentPos);
				if (filePtr->file_seek(-2, SEEK_CUR) < 0) // Seek back to before we read the invalid unitId
					return 0;
				
				// Try to read UnitGF structure to get the actual unit_id
				UnitGF tempGF;
				long recoveryPos = filePtr->file_pos();
				// MSG("UnitArray::read_file: Recovery: Attempting to read UnitGF at position %ld\n", recoveryPos);
				if( filePtr->file_read(&tempGF, sizeof(UnitGF)) ) {
					// Extract unit_id from the structure (it's at offset after sprite fields)
					int8_t actualUnitId = tempGF.unit_id;
					// MSG("UnitArray::read_file: Recovery: Read UnitGF, actual unit_id=%d\n", actualUnitId);
					
					// Check if actualUnitId is valid
					// MSG("UnitArray::read_file: Recovery: actual unit_id=%d, unit_info_count=%d\n", actualUnitId, unit_res.unit_info_count);
					if( actualUnitId >= 1 && actualUnitId <= unit_res.unit_info_count ) {
						// MSG("UnitArray::read_file: Recovery successful! Actual unitId=%d, file was misaligned\n", actualUnitId);
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
								ERR("UnitArray::read_file: Recovery: Failed to read result_node_array for unitId=%d\n", unitId);
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
								ERR("UnitArray::read_file: Recovery: Failed to read way_point_array for unitId=%d\n", unitId);
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
							// Read into the team_info member of the union for consistency with write
							if( filePtr->file_read(&gf_rec.team_info, sizeof(TeamInfoGF)) )
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
							ERR("UnitArray::read_file: Recovery: read_derived_file failed for unitId=%d\n", unitId);
							return 0;
						}
						
						unitPtr->fix_attack_info();
						// fprintf(stderr, "[DEBUG] Recovery: Successfully recovered and loaded unit %d\n", unitId);
						// Continue to next iteration
						continue;
					} else if( actualUnitId == 0 ) {
						// If actualUnitId is 0, this might be a deleted unit that had its data written
						// We need to skip the unit data to maintain alignment
						ERR("UnitArray::read_file: Recovery: Detected deleted unit (unit_id=0) with unit data written - this is the misalignment cause\n");
						ERR("UnitArray::read_file: Recovery: Need to skip unit data for this deleted unit to maintain alignment\n");
						// Skip the rest of UnitGF (we've already read it into tempGF)
						// Then skip result_node_array, way_point_array, team_info, and derived data
						// This is complex without knowing unit_id - for now, fail with clear message
						ERR("UnitArray::read_file: Recovery: File format mismatch - deleted units have unit data written.\n");
						ERR("UnitArray::read_file: Recovery: Please resave the game with the current version.\n");
						return 0;
					} else {
						ERR("UnitArray::read_file: Recovery failed: actual unit_id=%d is also invalid (out of range 1-%d)\n", 
							actualUnitId, unit_res.unit_info_count);
						ERR("UnitArray::read_file: Recovery: This suggests severe file corruption at unit %d/%d (position %ld)\n", 
							i, unitCount, filePosBeforeUnitId);
						ERR("UnitArray::read_file: Recovery: File may be corrupted or from an incompatible version\n");
						// Try to continue by skipping this corrupted unit
						// We've already read UnitGF, so we need to skip the rest of the unit data
						// This is risky, but better than failing completely
						// MSG("UnitArray::read_file: Attempting to skip corrupted unit %d and continue loading\n", i);
						add_blank(1);  // Add a blank slot for this corrupted unit
						// We've already read UnitGF (sizeof(UnitGF) bytes), so we're at the right position
						// But we need to skip result_node_array, way_point_array, team_info, and derived data
						// Without knowing the unit_id, we can't know the sizes. This is complex.
						// For now, fail but with a clear message
						ERR("UnitArray::read_file: Cannot safely skip corrupted unit - file is too corrupted to recover\n");
						return 0;
					}
				} else {
					ERR("UnitArray::read_file: Recovery failed: could not read UnitGF structure at position %ld\n", recoveryPos);
					ERR("UnitArray::read_file: File appears corrupted at unit %d/%d\n", i, unitCount);
					return 0;
				}
			} else {
				ERR("UnitArray::read_file: Invalid unitId=%d at unit %d/%d, but recovery condition not met (not in range 256-999 or i=%d <= 1)\n", 
					unitId, i, unitCount, i);
			}
			
			ERR("UnitArray::read_file: The save file appears to be from an incompatible version or corrupted at unit %d/%d (position %ld).\n", 
				i, unitCount, filePosBeforeUnitId);
			ERR("UnitArray::read_file: Please resave the game with the current version, or use a save file created with this version.\n");
            return 0;
         }
         //----- create unit object -----------//
         unitPtr = create_unit( unitId );
         unitPtr->unit_id = unitId;

         long filePosBeforeUnitData = filePtr->file_pos();
         // MSG("UnitArray::read_file: Unit %d (unitId=%d): Reading base class data at position %ld\n", i, unitId, filePosBeforeUnitData);
         
         //---- read data in base class -----//

         if( !unitPtr->read_file( filePtr ) )
         {
			ERR("UnitArray::read_file: Unit::read_file failed for unitId=%d, unit index %d, position %ld\n", unitId, i, filePosBeforeUnitData);
            return 0;
		}
		long filePosAfterUnitData = filePtr->file_pos();
		// MSG("UnitArray::read_file: Unit %d (unitId=%d): Base class read succeeded (read %ld bytes)\n", i, unitId, filePosAfterUnitData - filePosBeforeUnitData);
		// long filePosAfterUnitData = filePtr->file_pos();
		// fprintf(stderr, "[DEBUG] Unit %d (unitId=%d): read_file consumed %ld bytes\n", i, unitId, filePosAfterUnitData - filePosBeforeUnitData);

         //----- read data in derived class -----//
         long filePosBeforeDerived = filePtr->file_pos();
         // MSG("UnitArray::read_file: Unit %d (unitId=%d): Reading derived class data at position %ld\n", i, unitId, filePosBeforeDerived);

         if( !unitPtr->read_derived_file( filePtr ) )
         {
			ERR("UnitArray::read_file: Unit::read_derived_file failed for unitId=%d, unit index %d, position %ld\n", unitId, i, filePosBeforeDerived);
            return 0;
		}
		long filePosAfterDerived = filePtr->file_pos();
		// MSG("UnitArray::read_file: Unit %d (unitId=%d): Derived class read succeeded (read %ld bytes)\n", i, unitId, filePosAfterDerived - filePosBeforeDerived);
		// long filePosAfterDerived = filePtr->file_pos();
		// fprintf(stderr, "[DEBUG] Unit %d (unitId=%d): read_derived_file consumed %ld bytes\n", i, unitId, filePosAfterDerived - filePosAfterUnitData);

			unitPtr->fix_attack_info();
      }
   }
   long filePosAfterAllUnits = filePtr->file_pos();
   // MSG("UnitArray::read_file: Finished reading all %d units, total bytes read: %ld\n", unitCount, filePosAfterAllUnits - filePosBeforeUnits);

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
   long filePosBeforeEmptyRoom = filePtr->file_pos();
   // MSG("UnitArray::read_file: About to read empty_room_array at position %ld, expected emptyRoomCount=%d\n", filePosBeforeEmptyRoom, emptyRoomCount);
   if( !read_empty_room(filePtr) )
   {
      ERR("UnitArray::read_file: read_empty_room failed at position %ld\n", filePosBeforeEmptyRoom);
      return 0;
   }
   // MSG("UnitArray::read_file: Finished reading empty_room_array, empty_room_count=%d (expected %d)\n", empty_room_count, emptyRoomCount);
   
   // Log file position after reading empty_room_array
   long filePosAfterEmptyRoom = filePtr->file_pos();
   // MSG("UnitArray::read_file: File position after empty_room_array: %ld (read %ld bytes)\n", filePosAfterEmptyRoom, filePosAfterEmptyRoom - filePosBeforeEmptyRoom);
   
   // Defensive: Validate empty_room_count matches what we expect
   // If there's a significant mismatch (> 10 units difference), the file is likely corrupted or from incompatible version
   if( empty_room_count != emptyRoomCount )
   {
      int diff = abs(empty_room_count - emptyRoomCount);
      if( diff > 10 || empty_room_count > 100 )
      {
         ERR("UnitArray::read_file: ERROR - empty_room_count mismatch! Read %d but expected %d (diff=%d) - file may be corrupted or from incompatible version\n", 
            empty_room_count, emptyRoomCount, diff);
         ERR("UnitArray::read_file: This suggests the save file format is incompatible. Please resave the game with the current version.\n");
         return 0; // Fail the load
      }
      else
      {
         // MSG("UnitArray::read_file: WARNING - empty_room_count mismatch! Read %d but expected %d (diff=%d) - minor difference, continuing\n", 
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

   long filePosEnd = filePtr->file_pos();
   // MSG("UnitArray::read_file: Completed successfully, final position: %ld (total read: %ld bytes)\n", filePosEnd, filePosEnd - filePosStart);
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
	long filePosBeforeUnitGF = filePtr->file_pos();
	// MSG("Unit::write_file: Writing UnitGF at position %ld (writing %zu bytes)\n", filePosBeforeUnitGF, sizeof(UnitGF));
	
	// CRITICAL: Validate result_node_array BEFORE writing UnitGF
	// If validation fails, we must set result_node_count to 0 before write_record
	// to ensure UnitGF is written with count=0, preventing a mismatch when loading
	bool result_node_array_valid = false;
	if( result_node_array )
	{
		// Defensive: Validate result_node_count before writing
		// Reasonable bounds: 0 to 10000 (paths shouldn't be longer than this)
		if( result_node_count < 0 || result_node_count > 10000 )
		{
			ERR("Unit::write_file: CORRUPTION DETECTED! Invalid result_node_count=%d (unit_id=%d) - should be 0-10000\n", 
				result_node_count, unit_id);
			ERR("Unit::write_file: This indicates memory corruption during gameplay. Setting count to 0 in UnitGF to prevent file corruption.\n");
			result_node_count = 0;
			result_node_array = nullptr;
		}
		else
		{
			// Defensive: Validate result_node_array pointer is reasonable
			if( (uintptr_t)result_node_array < 0x1000 || (uintptr_t)result_node_array == 0xdeadbeef )
			{
				ERR("Unit::write_file: CORRUPTION DETECTED! Invalid result_node_array pointer=%p (unit_id=%d, count=%d)\n", 
					result_node_array, unit_id, result_node_count);
				ERR("Unit::write_file: This indicates memory corruption during gameplay. Setting count to 0 in UnitGF to prevent file corruption.\n");
				result_node_count = 0;
				result_node_array = nullptr;
			}
			else
			{
				result_node_array_valid = true;
			}
		}
	}
	else
	{
		// CRITICAL: If result_node_array is nullptr but result_node_count > 0, this is inconsistent
		// Set count to 0 to prevent writing UnitGF with a non-zero count but no array data
		// This can happen if the array was freed (e.g., path completed) but count wasn't reset
		if( result_node_count > 0 )
		{
			// Note: This should be rare now that next_move() resets the count, but keep as defensive check
			// MSG("Unit::write_file: Fixing inconsistency: result_node_array is nullptr but result_node_count=%d (unit_id=%d), resetting count\n",
			// 	result_node_count, unit_id);
			result_node_count = 0;
		}
	}
	
	// CRITICAL: Validate way_point_array BEFORE writing UnitGF
	// If validation fails, we must set way_point_array_size to 0 before write_record
	// to ensure UnitGF is written with size=0, preventing a mismatch when loading
	bool way_point_array_valid = false;
	if( way_point_array )
	{
		// Defensive: Validate way_point_array_size before writing
		// Reasonable bounds: 0 to 10000 (paths shouldn't be longer than this)
		if( way_point_array_size < 0 || way_point_array_size > 10000 )
		{
			ERR("Unit::write_file: CORRUPTION DETECTED! Invalid way_point_array_size=%d (unit_id=%d) - should be 0-10000\n", 
				way_point_array_size, unit_id);
			ERR("Unit::write_file: This indicates memory corruption during gameplay. Setting size to 0 in UnitGF to prevent file corruption.\n");
			way_point_array_size = 0;
			way_point_array = nullptr;
		}
		else
		{
			// Defensive: Validate way_point_array pointer is reasonable
			if( (uintptr_t)way_point_array < 0x1000 || (uintptr_t)way_point_array == 0xdeadbeef )
			{
				ERR("Unit::write_file: CORRUPTION DETECTED! Invalid way_point_array pointer=%p (unit_id=%d, size=%d)\n", 
					way_point_array, unit_id, way_point_array_size);
				ERR("Unit::write_file: This indicates memory corruption during gameplay. Setting size to 0 in UnitGF to prevent file corruption.\n");
				way_point_array_size = 0;
				way_point_array = nullptr;
			}
			else
			{
				way_point_array_valid = true;
			}
		}
	}
	else
	{
		// CRITICAL: If way_point_array is nullptr but way_point_array_size > 0, this is inconsistent
		// Set size to 0 to prevent writing UnitGF with a non-zero size but no array data
		// This can happen if the array was freed but size wasn't reset
		if( way_point_array_size > 0 )
		{
			// Note: This should be rare since reset_way_point_array() resets the size, but keep as defensive check
			// MSG("Unit::write_file: Fixing inconsistency: way_point_array is nullptr but way_point_array_size=%d (unit_id=%d), resetting size\n",
			// 	way_point_array_size, unit_id);
			way_point_array_size = 0;
		}
	}
	
	// CRITICAL: Capture values BEFORE write_record, as write_record copies them to UnitGF
	// Also preserve the validation state to ensure consistency
	int saved_result_node_count = result_node_count;
	int saved_way_point_array_size = way_point_array_size;
	bool saved_result_node_array_valid = result_node_array_valid;
	bool saved_way_point_array_valid = way_point_array_valid;
	void* saved_result_node_array_ptr = result_node_array;  // Save pointer for later validation
	void* saved_way_point_array_ptr = way_point_array;     // Save pointer for later validation
	
	write_record(&gf_rec.unit);
	
	// Verify that write_record correctly copied result_node_count to UnitGF
	if( gf_rec.unit.result_node_count != saved_result_node_count )
	{
		ERR("Unit::write_file: CORRUPTION DETECTED! result_node_count changed during write_record: was %d, now UnitGF has %d (unit_id=%d)\n",
			saved_result_node_count, gf_rec.unit.result_node_count, unit_id);
		ERR("Unit::write_file: This should never happen - write_record should copy the value, not modify it.\n");
		// Use the value from UnitGF to maintain consistency
		result_node_count = gf_rec.unit.result_node_count;
		// If UnitGF has a non-zero count but we determined the array is invalid, this is a problem
		if( !result_node_array_valid && gf_rec.unit.result_node_count > 0 )
		{
			ERR("Unit::write_file: CRITICAL! UnitGF was written with count=%d but result_node_array is invalid - this will cause load failure!\n",
				gf_rec.unit.result_node_count);
			ERR("Unit::write_file: Attempting to correct by setting count to 0 in memory (but UnitGF already written with wrong count).\n");
			result_node_count = 0;
		}
	}
	
	// Verify that write_record correctly copied way_point_array_size to UnitGF
	if( gf_rec.unit.way_point_array_size != saved_way_point_array_size )
	{
		ERR("Unit::write_file: CORRUPTION DETECTED! way_point_array_size changed during write_record: was %d, now UnitGF has %d (unit_id=%d)\n",
			saved_way_point_array_size, gf_rec.unit.way_point_array_size, unit_id);
		ERR("Unit::write_file: This should never happen - write_record should copy the value, not modify it.\n");
		// Use the value from UnitGF to maintain consistency
		way_point_array_size = gf_rec.unit.way_point_array_size;
		// If UnitGF has a non-zero size but we determined the array is invalid, this is a problem
		if( !way_point_array_valid && gf_rec.unit.way_point_array_size > 0 )
		{
			ERR("Unit::write_file: CRITICAL! UnitGF was written with size=%d but way_point_array is invalid - this will cause load failure!\n",
				gf_rec.unit.way_point_array_size);
			ERR("Unit::write_file: Attempting to correct by setting size to 0 in memory (but UnitGF already written with wrong size).\n");
			way_point_array_size = 0;
		}
	}
	
	// Write from the unit member of the union for consistency and clarity
	// (though &gf_rec and &gf_rec.unit are the same address in a union)
	if( !filePtr->file_write(&gf_rec.unit, sizeof(UnitGF)) )
	{
		ERR("Unit::write_file: Failed to write UnitGF at position %ld\n", filePosBeforeUnitGF);
		return 0;
	}
	long filePosAfterUnitGF = filePtr->file_pos();
	// MSG("Unit::write_file: UnitGF written successfully (wrote %ld bytes, expected %zu)\n", filePosAfterUnitGF - filePosBeforeUnitGF, sizeof(UnitGF));

	//--------------- write memory data ----------------//

	// Use saved validation state to ensure consistency - the array might have been freed/invalidated
	// between validation and writing, but if UnitGF promises data, we must write it
	bool should_write_result_node_array = (saved_result_node_array_valid && saved_result_node_count > 0 && 
	                                       saved_result_node_array_ptr != nullptr);
	
	if( should_write_result_node_array && result_node_array && result_node_array_valid && result_node_count > 0 )
	{
		// CRITICAL: Verify that result_node_count in UnitGF matches the actual count
		// This prevents writing incorrect record sizes if memory was corrupted between writing UnitGF and writing result_node_array
		if( gf_rec.unit.result_node_count != result_node_count )
		{
			ERR("Unit::write_file: CORRUPTION DETECTED! result_node_count mismatch: UnitGF has %d but Unit object has %d (unit_id=%d)\n",
				gf_rec.unit.result_node_count, result_node_count, unit_id);
			ERR("Unit::write_file: This indicates memory corruption occurred between writing UnitGF and writing result_node_array.\n");
			ERR("Unit::write_file: The count in UnitGF was written as %d, but we're trying to write %d nodes.\n",
				gf_rec.unit.result_node_count, result_node_count);
			ERR("Unit::write_file: Using the count from UnitGF (%d) to maintain file consistency, but this may indicate data loss.\n",
				gf_rec.unit.result_node_count);
			// Use the count from UnitGF to maintain consistency with what was already written
			result_node_count = gf_rec.unit.result_node_count;
			// If the count is now 0 or invalid, skip writing
			if( result_node_count <= 0 || result_node_count > 10000 )
			{
				ERR("Unit::write_file: UnitGF count is also invalid (%d), skipping result_node_array write\n", result_node_count);
				result_node_count = 0;
				result_node_array = nullptr;
				// Continue without writing result_node_array - but this should not happen since we validated before
			}
		}
		
		// Only write if we still have valid data after the check above
		if( result_node_array && result_node_count > 0 )
		{
			size_t writeSize = sizeof(ResultNodeGF)*result_node_count;
					// MSG("Unit::write_file: Verified result_node_count=%d matches UnitGF.count=%d before writing result_node_array\n",
					// 	result_node_count, gf_rec.unit.result_node_count);
					long filePosBeforeResultNodes = filePtr->file_pos();
					// MSG("Unit::write_file: Writing result_node_array, count=%d, size=%zu bytes at position %ld\n", 
					// 	result_node_count, writeSize, filePosBeforeResultNodes);
					// MSG("Unit::write_file: sizeof(ResultNodeGF)=%zu, calculated writeSize=%zu (count * sizeof)\n",
					// 	sizeof(ResultNodeGF), writeSize);
					// Allocate using ResultNodeGF size, not ResultNode size - they may differ
					ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*result_node_count);
					for( int i=0; i<result_node_count; i++ )
					{
						ResultNode *node = result_node_array+i;
						node->write_record(node_record_array+i);
					}
						if( !filePtr->file_write(node_record_array, writeSize) )
					{
						ERR("Unit::write_file: Failed to write result_node_array at position %ld\n", filePosBeforeResultNodes);
						mem_del(node_record_array);
						return 0;
					}
					long filePosAfterResultNodes = filePtr->file_pos();
					long bytesWritten = filePosAfterResultNodes - filePosBeforeResultNodes;
					// MSG("Unit::write_file: result_node_array written successfully (wrote %ld bytes, expected %zu)\n", 
					// 	bytesWritten, writeSize);
					
					// Defensive: Validate that we actually wrote the expected amount
					// In STRUCTURED mode, file position should move by: 2 (header) + writeSize (data)
					long expectedBytesWritten = 2 + (long)writeSize;
					if( bytesWritten < expectedBytesWritten )
					{
						ERR("Unit::write_file: CORRUPTION DETECTED! Wrote only %ld bytes but expected at least %ld bytes (2-byte header + %zu data) for unit_id=%d, count=%d\n",
							bytesWritten, expectedBytesWritten, writeSize, unit_id, result_node_count);
						ERR("Unit::write_file: This suggests the file write failed partway through or the file system is corrupted.\n");
						mem_del(node_record_array);
						return 0; // Fail the save to prevent corrupted file
					}
					
					// Additional validation: Check if bytesWritten is significantly larger than expected
					// This would indicate the record size header was written incorrectly
					if( bytesWritten > expectedBytesWritten + 2 )
					{
						ERR("Unit::write_file: CORRUPTION DETECTED! Wrote %ld bytes but expected %ld bytes (2-byte header + %zu data) for unit_id=%d, count=%d\n",
							bytesWritten, expectedBytesWritten, writeSize, unit_id, result_node_count);
						ERR("Unit::write_file: File position moved %ld bytes more than expected - record size header may have been written incorrectly.\n",
							bytesWritten - expectedBytesWritten);
						ERR("Unit::write_file: This suggests memory corruption or incorrect dataSize passed to file_write.\n");
						mem_del(node_record_array);
						return 0; // Fail the save to prevent corrupted file
					}
					
			mem_del(node_record_array);
		}
	}
	
	// CRITICAL: Verify that if UnitGF has result_node_count > 0, we actually wrote the array
	// Use saved validation state to check if we SHOULD have written it
	if( gf_rec.unit.result_node_count > 0 && !should_write_result_node_array )
	{
		ERR("Unit::write_file: CRITICAL INCONSISTENCY! UnitGF has result_node_count=%d but result_node_array was not written (unit_id=%d)\n",
			gf_rec.unit.result_node_count, unit_id);
		ERR("Unit::write_file: This will cause load failure - UnitGF promises %d nodes but no array data exists.\n",
			gf_rec.unit.result_node_count);
		ERR("Unit::write_file: saved_result_node_array_valid=%d, saved_result_node_count=%d, saved_result_node_array_ptr=%p\n",
			saved_result_node_array_valid, saved_result_node_count, saved_result_node_array_ptr);
		ERR("Unit::write_file: This suggests the array was invalid during validation but UnitGF was written with count > 0.\n");
		ERR("Unit::write_file: Failing save to prevent corrupted file.\n");
		return 0; // Fail the save to prevent corruption
	}

	//### begin alex 15/10 ###//
	// Use saved validation state to ensure consistency
	bool should_write_way_point_array = (saved_way_point_array_valid && saved_way_point_array_size > 0 && 
	                                     saved_way_point_array_ptr != nullptr);
	
	if( should_write_way_point_array && way_point_array && way_point_array_valid && way_point_array_size > 0 )
	{
		// Validation already done before UnitGF write - just verify consistency
		if( gf_rec.unit.way_point_array_size != way_point_array_size )
		{
			ERR("Unit::write_file: CORRUPTION DETECTED! way_point_array_size mismatch: UnitGF has %d but Unit object has %d (unit_id=%d)\n",
				gf_rec.unit.way_point_array_size, way_point_array_size, unit_id);
			ERR("Unit::write_file: This indicates memory corruption occurred between writing UnitGF and writing way_point_array.\n");
			ERR("Unit::write_file: Using the size from UnitGF (%d) to maintain file consistency, but this may indicate data loss.\n",
				gf_rec.unit.way_point_array_size);
			// Use the size from UnitGF to maintain consistency with what was already written
			way_point_array_size = gf_rec.unit.way_point_array_size;
			// If the size is now 0 or invalid, skip writing
			if( way_point_array_size <= 0 || way_point_array_size > 10000 )
			{
				ERR("Unit::write_file: UnitGF size is also invalid (%d), skipping way_point_array write\n", way_point_array_size);
				way_point_array_size = 0;
				way_point_array = nullptr;
				// Continue without writing way_point_array - but this should not happen since we validated before
			}
		}
		
		// Only write if we still have valid data after the check above
		if( way_point_array && way_point_array_size > 0 )
		{
			err_when(way_point_array_size==0 || way_point_array_size<way_point_count);
			size_t wayPointWriteSize = sizeof(ResultNodeGF)*way_point_array_size;
			long filePosBeforeWayPoints = filePtr->file_pos();
			// MSG("Unit::write_file: Writing way_point_array, size=%d, writeSize=%zu bytes at position %ld\n", 
			// 	way_point_array_size, wayPointWriteSize, filePosBeforeWayPoints);
			ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*way_point_array_size);
			for( int i=0; i<way_point_array_size; i++ )
			{
				ResultNode *node = way_point_array+i;
				node->write_record(node_record_array+i);
			}
			if( !filePtr->file_write(node_record_array, wayPointWriteSize) )
			{
				ERR("Unit::write_file: Failed to write way_point_array at position %ld\n", filePosBeforeWayPoints);
				mem_del(node_record_array);
				return 0;
			}
			long filePosAfterWayPoints = filePtr->file_pos();
			long bytesWritten = filePosAfterWayPoints - filePosBeforeWayPoints;
			// MSG("Unit::write_file: way_point_array written successfully (wrote %ld bytes, expected %zu)\n", 
			// 	bytesWritten, wayPointWriteSize);
			
			// Defensive: Validate that we actually wrote the expected amount
			long expectedBytesWritten = 2 + (long)wayPointWriteSize;
			if( bytesWritten < expectedBytesWritten )
			{
				ERR("Unit::write_file: CORRUPTION DETECTED! Wrote only %ld bytes but expected at least %ld bytes (2-byte header + %zu data) for unit_id=%d, size=%d\n",
					bytesWritten, expectedBytesWritten, wayPointWriteSize, unit_id, way_point_array_size);
				ERR("Unit::write_file: This suggests the file write failed partway through or the file system is corrupted.\n");
				mem_del(node_record_array);
				return 0; // Fail the save to prevent corrupted file
			}
			
			mem_del(node_record_array);
		}
	}
	
	// CRITICAL: Verify that if UnitGF has way_point_array_size > 0, we actually wrote the array
	// Use saved validation state to check if we SHOULD have written it
	if( gf_rec.unit.way_point_array_size > 0 && !should_write_way_point_array )
	{
		ERR("Unit::write_file: CRITICAL INCONSISTENCY! UnitGF has way_point_array_size=%d but way_point_array was not written (unit_id=%d)\n",
			gf_rec.unit.way_point_array_size, unit_id);
		ERR("Unit::write_file: This will cause load failure - UnitGF promises %d way points but no array data exists.\n",
			gf_rec.unit.way_point_array_size);
		ERR("Unit::write_file: saved_way_point_array_valid=%d, saved_way_point_array_size=%d, saved_way_point_array_ptr=%p\n",
			saved_way_point_array_valid, saved_way_point_array_size, saved_way_point_array_ptr);
		ERR("Unit::write_file: This suggests the array was invalid during validation but UnitGF was written with size > 0.\n");
		ERR("Unit::write_file: Failing save to prevent corrupted file.\n");
		return 0; // Fail the save to prevent corruption
	}
	//#### end alex 15/10 ####//

	if( team_info )
	{
		team_info->write_record(&gf_rec.team_info);
		// Write from the team_info member of the union, not from the start of the union
		// This ensures we only write TeamInfoGF bytes, not leftover data from previous writes
		if( !filePtr->file_write(&gf_rec.team_info, sizeof(TeamInfoGF)) )
			return 0;
	}

	return 1;
}
//----------- End of function Unit::write_file ---------//


//--------- Begin of function Unit::read_file ---------//
//
int Unit::read_file(File* filePtr)
{
	long filePosBeforeUnitGF = filePtr->file_pos();
	// MSG("Unit::read_file: Reading UnitGF at position %ld (expecting %zu bytes)\n", filePosBeforeUnitGF, sizeof(UnitGF));
	// Read into the unit member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit, sizeof(UnitGF)) )
	{
		ERR("Unit::read_file: Failed to read UnitGF at position %ld\n", filePosBeforeUnitGF);
		return 0;
	}
	long filePosAfterUnitGF = filePtr->file_pos();
	long bytesMoved = filePosAfterUnitGF - filePosBeforeUnitGF;
	// MSG("Unit::read_file: UnitGF read successfully (file position moved %ld bytes, expected %zu), unit_id=%d\n", bytesMoved, sizeof(UnitGF), gf_rec.unit.unit_id);
	
	// In STRUCTURED mode, file_read() writes a 2-byte record size header, then the data.
	// So we expect: sizeof(UnitGF) (unstructured) or sizeof(UnitGF) + 2 (structured with header)
	// Anything significantly smaller indicates corruption/truncation.
	// Anything significantly larger (>10 bytes more) also indicates corruption.
	long minExpected = (long)sizeof(UnitGF);      // unstructured: just the data
	long maxExpected = (long)sizeof(UnitGF) + 2;   // structured: 2-byte header + data
	
	if( bytesMoved < minExpected || bytesMoved > maxExpected + 10 )
	{
		ERR("Unit::read_file: Corrupted UnitGF record at position %ld: moved %ld bytes, expected between %ld and %ld bytes - file is corrupted!\n", 
			filePosBeforeUnitGF, bytesMoved, minExpected, maxExpected);
		return 0;
	}
	// else if( bytesMoved == maxExpected )
	// {
	// 	MSG("Unit::read_file: File position moved %ld bytes (structured record with 2-byte header - this is OK)\n", bytesMoved);
	// }
	// else if( bytesMoved != minExpected )
	// {
	// 	MSG("Unit::read_file: File position moved %ld bytes instead of %zu (within acceptable range)\n", 
	// 		bytesMoved, sizeof(UnitGF));
	// }
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
		long filePosBeforeResultNodes = filePtr->file_pos();
		size_t expectedSize = sizeof(ResultNodeGF)*result_node_count;
		// MSG("Unit::read_file: Reading result_node_array, count=%d, expected size=%zu bytes at position %ld\n", 
		// 	result_node_count, expectedSize, filePosBeforeResultNodes);
		// Allocate using ResultNodeGF size, not ResultNode size - they may differ
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*result_node_count);

		if( !filePtr->file_read(node_record_array, expectedSize) )
		{
			ERR("Unit::read_file: Failed to read result_node_array at position %ld (expected %zu bytes)\n", 
				filePosBeforeResultNodes, expectedSize);
			mem_del(node_record_array);
			// Ensure pointers are nullptr so deinit() doesn't try to free invalid pointers
			result_node_array = nullptr;
			result_node_count = 0;
			return 0;
		}
		
		long filePosAfterRead = filePtr->file_pos();
		long bytesMoved = filePosAfterRead - filePosBeforeResultNodes;
		// MSG("Unit::read_file: result_node_array file position moved %ld bytes (expected ~%zu, count=%d, sizeof(ResultNodeGF)=%zu)\n", 
		// 	bytesMoved, expectedSize, result_node_count, sizeof(ResultNodeGF));
		
		// Validate that we read the correct amount of data
		// In STRUCTURED mode, file position moves by: 2 (header) + recordSize (data)
		// The record size header should match expectedSize exactly (or be 0 if > 0xFFFF)
		// We need exactly expectedSize bytes of actual data, so file position should move by exactly 2 + expectedSize
		long expectedMove = 2 + (long)expectedSize;  // 2-byte header + data
		
		// Allow small tolerance (up to 2 bytes) for potential rounding or minor format differences
		// But if it's significantly different, it's corruption
		if( bytesMoved < expectedMove - 2 )
		{
			ERR("Unit::read_file: result_node_array read too little data: moved %ld bytes, expected at least %ld bytes (2-byte header + %zu data) - file may be corrupted!\n", 
				bytesMoved, expectedMove, expectedSize);
			ERR("Unit::read_file: This suggests the save file was corrupted during writing or the file format is incompatible.\n");
			ERR("Unit::read_file: unit_id=%d, result_node_count=%d, sizeof(ResultNodeGF)=%zu\n", 
				gf_rec.unit.unit_id, result_node_count, sizeof(ResultNodeGF));
			mem_del(node_record_array);
			// Ensure pointers are nullptr so deinit() doesn't try to free invalid pointers
			result_node_array = nullptr;
			result_node_count = 0;
			return 0;
		}
		
		// Check if record size is significantly larger than expected (indicates corruption or misalignment)
		// If bytesMoved is more than 2 bytes larger than expected, the record size header was wrong
		if( bytesMoved > expectedMove + 2 )
		{
			ERR("Unit::read_file: result_node_array record size mismatch: moved %ld bytes, expected %ld bytes (2-byte header + %zu data) - file may be corrupted!\n", 
				bytesMoved, expectedMove, expectedSize);
			ERR("Unit::read_file: The record size header in the file says %ld bytes of data, but we expected %zu bytes.\n",
				bytesMoved - 2, expectedSize);
			ERR("Unit::read_file: This suggests file corruption, misalignment, or the file was written with incorrect data sizes.\n");
			ERR("Unit::read_file: unit_id=%d, result_node_count=%d, sizeof(ResultNodeGF)=%zu\n", 
				gf_rec.unit.unit_id, result_node_count, sizeof(ResultNodeGF));
			mem_del(node_record_array);
			// Ensure pointers are nullptr so deinit() doesn't try to free invalid pointers
			result_node_array = nullptr;
			result_node_count = 0;
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
		long filePosBeforeWayPoints = filePtr->file_pos();
		// MSG("Unit::read_file: Reading way_point_array, size=%d at position %ld\n", way_point_array_size, filePosBeforeWayPoints);
		ResultNodeGF *node_record_array = (ResultNodeGF*) mem_add(sizeof(ResultNodeGF)*way_point_array_size);
		memset(node_record_array, 0, sizeof(ResultNodeGF)*way_point_array_size); // Patch: zero-initialize
		if( !filePtr->file_read(node_record_array, sizeof(ResultNodeGF)*way_point_array_size) )
		{
			ERR("Unit::read_file: Failed to read way_point_array at position %ld\n", filePosBeforeWayPoints);
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
		long filePosAfterWayPoints = filePtr->file_pos();
		// MSG("Unit::read_file: way_point_array read successfully (read %ld bytes)\n", filePosAfterWayPoints - filePosBeforeWayPoints);
	}
	//#### end alex 15/10 ####//

	safe_free((void*&)team_info);
	// Check has_team_info flag from UnitGF before reading TeamInfoGF
	// This matches the write logic which only writes TeamInfoGF if team_info exists
	if( gf_rec.unit.has_team_info )
	{
		long filePosBeforeTeamInfo = filePtr->file_pos();
		// MSG("Unit::read_file: Reading team_info at position %ld\n", filePosBeforeTeamInfo);
		// Read into the team_info member of the union for consistency with write
		if( !filePtr->file_read(&gf_rec.team_info, sizeof(TeamInfoGF)) )
		{
			ERR("Unit::read_file: Failed to read team_info at position %ld\n", filePosBeforeTeamInfo);
			return 0;
		}
		team_info = (TeamInfo*) mem_add(sizeof(TeamInfo));
		memset(team_info, 0, sizeof(TeamInfo)); // Patch: zero-initialize
		team_info->read_record(&gf_rec.team_info);
		long filePosAfterTeamInfo = filePtr->file_pos();
		// MSG("Unit::read_file: team_info read successfully (read %ld bytes)\n", filePosAfterTeamInfo - filePosBeforeTeamInfo);
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
	
	long filePosBeforeDerived = filePtr->file_pos();
	// MSG("Unit::write_derived_file: Writing derived class data for unit_id=%d, writeSize=%d at position %ld\n", unit_id, writeSize, filePosBeforeDerived);

   if( writeSize > 0 )
   {
      if( !filePtr->file_write( (char*) this + sizeof(Unit), writeSize ) )
      {
         ERR("Unit::write_derived_file: Failed to write %d bytes of derived class data for unit_id=%d at position %ld\n", writeSize, unit_id, filePosBeforeDerived);
         return 0;
      }
      long filePosAfterDerived = filePtr->file_pos();
      // MSG("Unit::write_derived_file: Successfully wrote %ld bytes of derived class data for unit_id=%d\n", filePosAfterDerived - filePosBeforeDerived, unit_id);
   }
   else
   {
      // MSG("Unit::write_derived_file: No derived class data to write for unit_id=%d (writeSize=%d)\n", unit_id, writeSize);
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
   
   long filePosBeforeDerived = filePtr->file_pos();
   // MSG("Unit::read_derived_file: Reading derived class data for unit_id=%d, readSize=%d at position %ld\n", unit_id, readSize, filePosBeforeDerived);

   if( readSize > 0 )
   {
      if( !filePtr->file_read( (char*) this + sizeof(Unit), readSize ) )
      {
         ERR("Unit::read_derived_file: Failed to read %d bytes of derived class data for unit_id=%d at position %ld\n", readSize, unit_id, filePosBeforeDerived);
         return 0;
      }
      long filePosAfterDerived = filePtr->file_pos();
      // MSG("Unit::read_derived_file: Successfully read %ld bytes of derived class data for unit_id=%d\n", filePosAfterDerived - filePosBeforeDerived, unit_id);
   }
   else
   {
      // MSG("Unit::read_derived_file: No derived class data to read for unit_id=%d (readSize=%d)\n", unit_id, readSize);
   }

   return 1;
}
//----------- End of function Unit::read_derived_file ---------//


//--------- Begin of function UnitCaravan::write_derived_file ---------//
int UnitCaravan::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_caravan);
	// Write from the unit_caravan member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_caravan, sizeof(UnitCaravanGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitCaravan::write_derived_file ---------//


//--------- Begin of function UnitCaravan::read_derived_file ---------//
int UnitCaravan::read_derived_file(File* filePtr)
{
	// Read into the unit_caravan member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_caravan, sizeof(UnitCaravanGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_caravan);
	return 1;
}
//--------- End of function UnitCaravan::read_derived_file ---------//


//--------- Begin of function UnitExpCart::write_derived_file ---------//
int UnitExpCart::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_exp_cart);
	// Write from the unit_exp_cart member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_exp_cart, sizeof(UnitExpCartGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitExpCart::write_derived_file ---------//


//--------- Begin of function UnitExpCart::read_derived_file ---------//
int UnitExpCart::read_derived_file(File* filePtr)
{
	// Read into the unit_exp_cart member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_exp_cart, sizeof(UnitExpCartGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_exp_cart);
	return 1;
}
//--------- End of function UnitExpCart::read_derived_file ---------//


//--------- Begin of function UnitGod::write_derived_file ---------//
int UnitGod::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_god);
	// Write from the unit_god member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_god, sizeof(UnitGodGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitGod::write_derived_file ---------//


//--------- Begin of function UnitGod::read_derived_file ---------//
int UnitGod::read_derived_file(File* filePtr)
{
	// Read into the unit_god member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_god, sizeof(UnitGodGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_god);
	return 1;
}
//--------- End of function UnitGod::read_derived_file ---------//


//--------- Begin of function UnitMarine::write_derived_file ---------//
int UnitMarine::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_marine);
	// Write from the unit_marine member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_marine, sizeof(UnitMarineGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMarine::write_derived_file ---------//


//--------- Begin of function UnitMarine::read_derived_file ---------//
int UnitMarine::read_derived_file(File* filePtr)
{
	// Read into the unit_marine member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_marine, sizeof(UnitMarineGF)) )
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
	// Write from the unit_monster member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_monster, sizeof(UnitMonsterGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMonster::write_derived_file ---------//


//--------- Begin of function UnitMonster::read_derived_file ---------//
int UnitMonster::read_derived_file(File* filePtr)
{
	// Read into the unit_monster member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_monster, sizeof(UnitMonsterGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_monster);
	return 1;
}
//--------- End of function UnitMonster::read_derived_file ---------//


//--------- Begin of function UnitVehicle::write_derived_file ---------//
int UnitVehicle::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_vehicle);
	// Write from the unit_vehicle member of the union, not from the start of the union
	if( !filePtr->file_write(&gf_rec.unit_vehicle, sizeof(UnitVehicleGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitVehicle::write_derived_file ---------//


//--------- Begin of function UnitVehicle::read_derived_file ---------//
int UnitVehicle::read_derived_file(File* filePtr)
{
	// Read into the unit_vehicle member of the union for consistency with write
	if( !filePtr->file_read(&gf_rec.unit_vehicle, sizeof(UnitVehicleGF)) )
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
	if (!filePtr->file_write(&gf_rec, sizeof(TownArrayGF)))
		return 0;

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
		if (!filePtr->file_read(&gf_rec, sizeof(Version_1_TownArrayGF)))
			return 0;
		read_record_v1(&gf_rec.town_array_v1);
	}
	else
	{
		if (!filePtr->file_read(&gf_rec, sizeof(TownArrayGF)))
			return 0;
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
	if( aiInfoCount > 0 && aiInfoArray )
	{
		filePtr->file_put_short_array( aiInfoArray, aiInfoCount );
	}
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
			if( !filePtr->file_get_short_array((int16_t*)*aiInfoArrayPtr, actualSize) )
			{
				// If read fails, clean up
				mem_del(*aiInfoArrayPtr);
				*aiInfoArrayPtr = nullptr;
				aiInfoCount = 0;
				aiInfoSize = 0;
				return;
			}
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

	if (!filePtr->file_write(news_type_option, sizeof(news_type_option)))
		return 0;

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

	if (!filePtr->file_read(news_type_option, sizeof(news_type_option)))
		return 0;

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
