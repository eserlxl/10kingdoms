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

// Filename    : OEXPMASK.CPP
// Description : explored area mask
// Owner       : Gilbert


#include <OEXPMASK.h>
#include <OFILE.h>
#include <OWORLDMT.h>
#include <OVGABUF.h>
#include <OSTR.h>
#include <OCOLTBL.h>
#include <OCONFIG.h>

// --------- Define Constant ------//
#define MASK_FILENAME "EXPLMASK.BIN"
#define REMAP_FILENAME "EXPREMAP.BIN"

// no. of darkness scale defined as MAX_BRIGHTNESS_ADJUST_DEGREE in OVGA.H

// ------- Begin of function ExploredMask::init ---------//
void ExploredMask::init(ColorTable *colorTable)
{
	brightness_table = colorTable;

	// ------- read into exploration mask bitmap --------//
	String str;
	str = DIR_RES;
	str += MASK_FILENAME;
	File maskFile;
	if (!maskFile.file_open(str))
		return;
	int fileSize = maskFile.file_size();
	mask_bitmap = (char *) mem_add( fileSize );
	if (!maskFile.file_read(mask_bitmap, fileSize))
	{
		mem_del(mask_bitmap);
		mask_bitmap = NULL;
		maskFile.file_close();
		return;
	}
	maskFile.file_close();

	// ------- read into exploration remap bitmap ------//
	str = DIR_RES;
	str += REMAP_FILENAME;
	File remapFile;
	if (!remapFile.file_open(str))
	{
		mem_del(mask_bitmap);
		mask_bitmap = NULL;
		return;
	}
	fileSize = remapFile.file_size();
	remap_bitmap = (char *) mem_add(fileSize);
	if (!remapFile.file_read(remap_bitmap, fileSize))
	{
		mem_del(remap_bitmap);
		remap_bitmap = NULL;
		remapFile.file_close();
		return;
	}
	remapFile.file_close();
}
// ------- End of function ExploredMask::init ---------//


// ------- Begin of function ExploredMask::deinit ---------//
void ExploredMask::deinit()
{
	mem_del(mask_bitmap);
	mem_del(remap_bitmap);
}
// ------- End of function ExploredMask::deinit ---------//


// ------- Begin of function ExploredMask::draw ---------//
//
// Draw a mask for explored area
// short xLoc, yLoc            location of the mask
// int northRow                explored flags, bit 0 for northeast square
//                             bit 1 for north square, bit 2 for northwest square
// int thisRow                 explored flags, bit 0 for east square
//                             bit 1 for this square, bit 2 for west square
// int southRow                explored flags, bit 0 for southeast square
//                             bit 1 for south square, bit 2 for southwest square
//
void ExploredMask::draw(short x, short y, int northRow, int thisRow, int southRow)
{
	switch(config.explore_mask_method)
	{
	case 0:
		break;
	case 1:		// use bit masking
		vga_back.explore_mask(x, y, mask_bitmap, northRow, thisRow, southRow);
		break;
	case 2:		// use remapping
		vga_back.explore_remap(x, y, remap_bitmap, (char **)brightness_table->get_table_array(),
			northRow, thisRow, southRow);
		break;
	default:
		err_here();
	}
}
// ------- End of function ExploredMask::draw ---------//


