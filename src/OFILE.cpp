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

//Filename    : OFILE.CPP
//Description : Object File

#include <ALL.h>
#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <dbglog.h>
#include <OFILE.h>
#include <errno.h>

DBGLOG_DEFAULT_CHANNEL(File);

//-------- Begin of function File::file_open ----------//
//
// Open an existing file for reading
//
// <char*> fileName      = name of the file
// [int]   handleError   = Treat RW operation failures as fatal or not
//                         (default true, 1).
// [int]   fileType = FLAT (0, default) or STRUCTURED (1). Structured file stores 
//                    its data as a records, where every record has the
//                    following format: [entry size: uint16; entry data: bytes]
//                    However this behaviour is used with read/write methods only;
//                    put/get methods (e.g. put_short) still write raw data.
//
// return : 1-success, 0-fail
//
int File::file_open(const char* fileName, int handleError, int fileType)
{
	// MSG("file_open: Attempting to open file '%s' (handleError=%d, fileType=%d)\n", fileName, handleError, fileType);
	
	if(strlen(fileName) >= FilePath::MAX_FILE_PATH)
	{
		ERR("file_open: File name too long (%zu >= %d)\n", strlen(fileName), FilePath::MAX_FILE_PATH);
		if (handleError)
			err.run("File : file name is too long.");
		return 0;
	}

	if (file_handle != NULL)
	{
		// MSG("file_open: Closing previously open file\n");
		file_close();
	}

	strcpy(file_name, fileName);
	handle_error = handleError;
	file_type = (FileType)fileType;

	file_handle = fopen(fileName, "rb");
	if (!file_handle)
	{
		ERR("file_open: Failed to open file '%s': %s (errno=%d)\n", fileName, strerror(errno), errno);
		if (handleError)
			err.run("[File::file_open] error opening file %s: %s\n", fileName, strerror(errno));
		return 0;
	}

	long fileSize = file_size();
	// MSG("file_open: Successfully opened file '%s', size=%ld bytes\n", fileName, fileSize);
	return 1;
}
//---------- End of function File::file_open ----------//


//-------- Begin of function File::file_create ----------//
//
// Create a new file for writing (reading is also permitted)
//
// <char*> fileName      = name of the file
// [int]   handleError   = Treat RW operation failures as fatal or not
//                         (default true, 1).
// [int]   fileType = FLAT (0, default) or STRUCTURED (1). Structured file stores 
//                    its data as a records, where every record has the
//                    following format: [entry size: uint16; entry data: bytes]
//                    However this behaviour is used with read/write methods only;
//                    put/get methods (e.g. put_short) still write raw data.
//
// return : 1-success, 0-fail
//
int File::file_create(const char* fileName, int handleError, int fileType)
{
	if(strlen(fileName) >= FilePath::MAX_FILE_PATH)
	{
		if (handleError)
			err.run("File : file name is too long.");
		return 0;
	}

	strcpy(file_name, fileName);
	handle_error = handleError;
	file_type = (FileType)fileType;

	file_handle = fopen(fileName, "wb+");
	if (!file_handle)
	{
		if (handleError)
			err.run("[File::file_create] couldn't create %s: %s\n", fileName, strerror(errno));
		return 0;
	}

	return 1;
}
//---------- End of function File::file_create ----------//

//-------- Begin of function File::file_close ----------//
//
void File::file_close()
{
	if (file_handle != NULL)
	{
		file_name[0] = '\0';
		fclose(file_handle);
		file_handle = NULL;
	}
}
//---------- End of function File::file_close ----------//


//-------- Begin of function File::~File ----------//
//
File::~File()
{
	file_close();
}
//---------- End of function File::~File ----------//


//-------- Begin of function File::file_write ----------//
//
// Write a block of data to the file
//
// <void*>    dataBuf = pointer to data buffer to be written to the file
// <unsigned> dataSize = length of the data (must < 64K)
//
// return : 1-success, 0-fail
//
int File::file_write(void* dataBuf, unsigned dataSize)
{
	err_when(!file_handle);

	long filePosBefore = file_pos();
	
	if (file_type == File::STRUCTURED)
	{
		// writing record size
		if (dataSize > 0xFFFF)
		{
			// If 'dataSize' exceed the unsigned short limit, write 0
			// instead of actual record size. Such a record may be
			// correctly read though (if proper data size will be
			// specified when calling successive file_read).
			file_put_unsigned_short(0);
			// MSG("[File::file_write] warning: record size exceeds uint16 MAX value\n");
		}
		else
		{
			// MSG("[File::file_write] Writing record size header: %u bytes at position %ld\n", dataSize, filePosBefore);
			file_put_unsigned_short(dataSize);
			// Verify the header was written correctly by checking file position
			long filePosAfterHeader = file_pos();
			if( filePosAfterHeader != filePosBefore + 2 )
			{
				ERR("[File::file_write] Header write verification failed! Position moved %ld bytes (expected 2) at %ld\n",
					filePosAfterHeader - filePosBefore, filePosBefore);
			}
		}
		
		// Check if header write failed
		if (ferror(file_handle))
		{
			if (handle_error)
				err.run("[File::file_write] error occurred while writing record size header to file: %s\n", file_name);
			else
				ERR("[File::file_write] error occurred while writing record size header to file: %s\n", file_name);
			return 0;
		}
	}

	size_t bytesWritten = fwrite(dataBuf, 1, dataSize, file_handle);

	// Defensive: Check if fwrite wrote all requested bytes
	if (bytesWritten != dataSize)
	{
		ERR("[File::file_write] Partial write detected! Wrote %zu bytes but requested %u bytes at position %ld in file: %s\n",
			bytesWritten, dataSize, filePosBefore, file_name);
		if (handle_error)
			err.run("[File::file_write] partial write: %zu/%u bytes written to file: %s\n", bytesWritten, dataSize, file_name);
		else
			ERR("[File::file_write] partial write: %zu/%u bytes written to file: %s\n", bytesWritten, dataSize, file_name);
		return 0;
	}

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_write] error occurred while writing file: %s\n", file_name);
		else
			ERR("[File::file_write] error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}
//---------- End of function File::file_write ----------//


//-------- Begin of function File::file_read ----------//
//
// Read a block of data from the file
//
// <void*>    dataBuf = pointer to data buffer to be written to the file
// <unsigned> dataSize = length of the data (must < 64K)
//
// return : 1-success, 0-fail
//
int File::file_read(void* dataBuf, unsigned dataSize)
{
	err_when(!file_handle);

	long filePosBefore = file_pos();
	unsigned bytesToRead = dataSize, recordSize = dataSize;
	
	// Maximum reasonable record size: 10MB (should be much smaller for game data)
	// This is used for validation to detect corrupted record sizes
	static const unsigned MAX_REASONABLE_RECORD_SIZE = 10 * 1024 * 1024;

	if (file_type == File::STRUCTURED)
	{
		recordSize = file_get_unsigned_short();
		// Add error checking to prevent uninitialized value usage
		if (ferror(file_handle) || feof(file_handle))
		{
			ERR("file_read: Error reading record size from file '%s' at position %ld\n", file_name, filePosBefore);
			if (handle_error)
				err.run("[File::file_read] error occurred while reading record size from file: %s\n", file_name);
			else
				ERR("[File::file_read] error occurred while reading record size from file: %s\n", file_name);
			return 0;
		}
		
		// Validate recordSize - it should be reasonable
		// If recordSize is 0, it means the actual size > 0xFFFF, so use dataSize
		// If recordSize is corrupted (unreasonably large), reject it
		if (recordSize > 0 && recordSize > MAX_REASONABLE_RECORD_SIZE)
		{
			ERR("file_read: Corrupted record size %u (0x%x) at position %ld - exceeds maximum reasonable size %u (requested %u bytes)\n", 
				recordSize, recordSize, filePosBefore, MAX_REASONABLE_RECORD_SIZE, dataSize);
			if (handle_error)
				err.run("[File::file_read] corrupted record size %u in file: %s\n", recordSize, file_name);
			return 0;
		}
		
		// Also check if recordSize is suspiciously large compared to what we requested
		// If it's more than 100x larger, it's likely corrupted
		if (recordSize > 0 && dataSize > 0 && recordSize > dataSize * 100)
		{
			ERR("file_read: Suspicious record size %u at position %ld - %u times larger than requested %u bytes\n", 
				recordSize, filePosBefore, recordSize / dataSize, dataSize);
			if (handle_error)
				err.run("[File::file_read] suspicious record size %u (requested %u) in file: %s\n", recordSize, dataSize, file_name);
			return 0;
		}
		
		// Defensive: Check if record size significantly differs from expected (indicates corruption/misalignment)
		// If recordSize < dataSize, we can't read enough data - this is a fatal error
		// If recordSize > dataSize, we can read dataSize bytes and skip the rest - this is recoverable
		// Allow small tolerance (up to 2 bytes) for minor format differences
		if (recordSize > 0 && dataSize > 0)
		{
			if (recordSize < dataSize)
			{
				// Not enough data in the record - fatal error
				unsigned diff = dataSize - recordSize;
				ERR("file_read: Record size too small at position %ld - file has %u bytes but we expected %u bytes (missing: %u)\n", 
					filePosBefore, recordSize, dataSize, diff);
				ERR("file_read: This suggests file corruption, truncation, or the file was written with incorrect data sizes.\n");
				if (handle_error)
					err.run("[File::file_read] record size too small %u vs %u in file: %s\n", recordSize, dataSize, file_name);
				return 0;
			}
			else if (recordSize > dataSize)
			{
				// More data than expected - warn but allow recovery by reading dataSize and skipping the rest
				unsigned diff = recordSize - dataSize;
				if (diff > 2 && diff > dataSize * 0.1) // More than 2 bytes and more than 10% difference
				{
					ERR("file_read: Record size larger than expected at position %ld - file has %u bytes but we expected %u bytes (extra: %u)\n", 
						filePosBefore, recordSize, dataSize, diff);
					ERR("file_read: This suggests file corruption, misalignment, or the file was written with incorrect data sizes.\n");
					ERR("file_read: Attempting recovery by reading %u bytes and skipping the remaining %u bytes.\n", dataSize, diff);
					// Don't return 0 - allow recovery by reading dataSize bytes and skipping the rest
				}
			}
		}
		
		if (recordSize && recordSize < dataSize) // recordSize==0, if the size > 0xFFFF
			bytesToRead = recordSize; // the read size is the minimum of the record size and the supposed read size
		// MSG("file_read: Reading structured record (requested=%u, recordSize=%u, bytesToRead=%u) at position %ld\n", 
		// 	dataSize, recordSize, bytesToRead, filePosBefore);
	}
	else
	{
		// MSG("file_read: Reading %u bytes at position %ld\n", dataSize, filePosBefore);
	}

	size_t itemsRead = fread(dataBuf, 1, bytesToRead, file_handle);

	// In the case of file_type == File::STRUCTURED
	// if the record was read partially,
	// skip remaining bytes in record and seek to next one
	if (bytesToRead < recordSize)
	{
		if (file_seek(recordSize - bytesToRead, SEEK_CUR) < 0)
			return 0;
	}

	// In the case of file_type == File::STRUCTURED
	// if the actual record size was smaller than requested data size,
	// fill the remaining bytes in buffer with 0
	if (bytesToRead < dataSize)
		memset((char*)dataBuf + bytesToRead, 0, dataSize - bytesToRead);

	if (ferror(file_handle))
	{
		ERR("file_read: Error occurred while reading file '%s' at position %ld (read %zu of %u bytes)\n", 
			file_name, filePosBefore, itemsRead, bytesToRead);
		// This used to prompt for a retry -- was this necessary?
		if (handle_error)
			err.run("[File::file_read] error occurred while reading file: %s\n", file_name);
		else
			ERR("[File::file_read] error occurred while reading file: %s\n", file_name);
		return 0;
	}

	if (itemsRead != bytesToRead)
	{
		ERR("file_read: Short read from file '%s' at position %ld (read %zu of %u bytes)\n", 
			file_name, filePosBefore, itemsRead, bytesToRead);
		return 0;
	}

	long filePosAfter = file_pos();
	// MSG("file_read: Successfully read %zu bytes (position %ld -> %ld)\n", itemsRead, filePosBefore, filePosAfter);
	return 1;
}
//---------- End of function File::file_read ----------//


int File::file_put_char(int8_t value)
{
	err_when(!file_handle);

	fwrite(&value, 1, sizeof(int8_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_put_short] error occurred while writing file: %s\n", file_name);
		else
			ERR("[File::file_put_short] error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}

int8_t File::file_get_char()
{
	err_when(!file_handle);

	int8_t value = 0;  // Initialize value to prevent uninitialized usage
	fread(&value, 1, sizeof(int8_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_get_char] error occurred while reading file: %s\n", file_name);
		else
			ERR("[File::file_get_char] error occurred while reading file: %s\n", file_name);
		return 0;
	}

	return value;
}

int File::file_put_short(int16_t value)
{
	err_when(!file_handle);

	value = SDL_SwapLE16(value);
	fwrite(&value, 1, sizeof(int16_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_put_char] error occurred while writing file: %s\n", file_name);
		else
			ERR("[File::file_put_char] error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}

int16_t File::file_get_short()
{
    	err_when(!file_handle);

	int16_t value = 0;  // Initialize value to prevent uninitialized usage
	fread(&value, 1, sizeof(int16_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_get_short] error occurred while reading file: %s\n", file_name);
		else
			ERR("[File::file_get_short] error occurred while reading file: %s\n", file_name);
		return 0;
	}

	return SDL_SwapLE16(value);
}

//-------- Begin of function File::file_put_short_array ----------//
//
// Put a short array that has count elements.
//
// return : 1-success, 0-fail
//
int File::file_put_short_array(int16_t *out, int count)
{
	err_when( !file_handle );

	// Validate input parameters
	if (!out || count <= 0)
	{
		if (handle_error)
			err.run("[File::file_put_short_array] invalid parameters: out=%p, count=%d\n", out, count);
		else
			ERR("[File::file_put_short_array] invalid parameters: out=%p, count=%d\n", out, count);
		return 0;
	}

	unsigned dataSize = count*sizeof(int16_t);
	if( file_type == File::STRUCTURED )
	{
		if( dataSize > 0xFFFF )
			file_put_unsigned_short(0);
		else
			file_put_unsigned_short(dataSize);
	}

	for( int i=0; i<count; i++ )
		file_put_short(out[i]);

	if( ferror(file_handle) )
	{
		if( handle_error )
			err.run("error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}
//---------- End of function File::file_put_short_array ----------//


//-------- Begin of function File::file_get_short_array ----------//
//
// Get a short array that has count elements, and returns the data in out. No
// allocation is performed, so the user provides the appropriate space at the
// address provided.
//
// return : 1-success, 0-fail
//
int File::file_get_short_array(int16_t *in, int count)
{
	err_when( !file_handle );
	if (!in) {
		if (handle_error)
			err.run("[File::file_get_short_array] null pointer passed for array\n");
		else
			ERR("[File::file_get_short_array] null pointer passed for array\n");
		return 0;
	}

	unsigned arrayBytes = count*sizeof(int16_t);
	unsigned bytesToRead = arrayBytes;
	unsigned recordBytes = arrayBytes;
	
	// Maximum reasonable record size: 10MB (should be much smaller for game data)
	// This is used for validation to detect corrupted record sizes
	static const unsigned MAX_REASONABLE_RECORD_SIZE = 10 * 1024 * 1024;
	
	if( file_type == File::STRUCTURED )
	{
		recordBytes = file_get_unsigned_short();
		
		// Validate recordBytes - it should be reasonable
		if (recordBytes > 0 && recordBytes > MAX_REASONABLE_RECORD_SIZE)
		{
			ERR("file_get_short_array: Corrupted record size %u (0x%x) - exceeds maximum reasonable size %u\n", 
				recordBytes, recordBytes, MAX_REASONABLE_RECORD_SIZE);
			if (handle_error)
				err.run("[File::file_get_short_array] corrupted record size %u in file: %s\n", recordBytes, file_name);
			return 0;
		}
		
		// Also check if recordBytes is suspiciously large compared to what we requested
		if (recordBytes > 0 && arrayBytes > 0 && recordBytes > arrayBytes * 100)
		{
			ERR("file_get_short_array: Suspicious record size %u - %u times larger than requested %u bytes\n", 
				recordBytes, recordBytes / arrayBytes, arrayBytes);
			if (handle_error)
				err.run("[File::file_get_short_array] suspicious record size %u (requested %u) in file: %s\n", recordBytes, arrayBytes, file_name);
			return 0;
		}
		
		bytesToRead = MIN(arrayBytes, recordBytes);
	}

	for( int i=0; i<bytesToRead/sizeof(int16_t); i++ )
		in[i] = file_get_short();

	//-------- if the data size has been reduced ----------//

	if( bytesToRead < recordBytes )
	{
		if (file_seek(recordBytes - bytesToRead, SEEK_CUR) < 0)
			return 0;
	}

	//---- if the data size has been increased, reset the unread area ---//

	if( recordBytes < arrayBytes )
		memset((char*)in + recordBytes, 0, arrayBytes - recordBytes);

	if( ferror(file_handle) )
	{
		if( handle_error )
			err.run("error occurred while reading file: %s\n", file_name);

		return 0;
	}

	return 1;
}
//---------- End of function File::file_get_short_array ----------//


int File::file_put_unsigned_short(uint16_t value)
{
    	err_when(!file_handle);

	value = SDL_SwapLE16(value);
	fwrite(&value, 1, sizeof(uint16_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_put_unsigned_short] error occurred while writing file: %s\n", file_name);
		else
			ERR("[File::file_put_unsigned_short] error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}

uint16_t File::file_get_unsigned_short()
{
    	err_when(!file_handle);

	uint16_t value = 0;  // Initialize value to prevent uninitialized usage
	fread(&value, 1, sizeof(uint16_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_get_unsigned_short] error occurred while reading file: %s\n", file_name);
		else
			ERR("[File::file_get_unsigned_short] error occurred while reading file: %s\n", file_name);
		return 0;
	}

	return SDL_SwapLE16(value);
}

int File::file_put_long(int32_t value)
{
    	err_when(!file_handle);

	value = SDL_SwapLE32(value);
	fwrite(&value, 1, sizeof(int32_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_put_long] error occurred while writing file: %s\n", file_name);
		else
			ERR("[File::file_put_long] error occurred while writing file: %s\n", file_name);
		return 0;
	}

	return 1;
}

int32_t File::file_get_long()
{
    	err_when(!file_handle);

	int32_t value = 0;  // Initialize value to prevent uninitialized usage
	fread(&value, 1, sizeof(int32_t), file_handle);

	if (ferror(file_handle))
	{
		if (handle_error)
			err.run("[File::file_get_long] error occurred while reading file: %s\n", file_name);
		else
			ERR("[File::file_get_long] error occurred while reading file: %s\n", file_name);
		return 0;
	}

	return SDL_SwapLE32(value);
}

//---------- Start of function File::file_seek ---------//
// whence = SEEK_SET, SEEK_CUR, SEEK_END (default: SEEK_SET)
// return : new offset from the file beginning.
long File::file_seek(long offset, int whence)
{
	fseek(file_handle, offset, whence);
	return ftell(file_handle);
}

long File::file_pos()
{
	return ftell(file_handle);
}

long File::file_size()
{
	long actual = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);

	long size = ftell(file_handle);

	fseek(file_handle, actual, SEEK_SET);
	return size;
}
