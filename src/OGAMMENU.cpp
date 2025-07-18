/*
 * Ten Kingdoms: Ancient Adversaries
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

//Filename   : OGAMMENU.CPP
//Description: In Game Menu

#include <OVGABUF.h>
#include <OSYS.h>
#include <OIMGRES.h>
#include <OGAME.h>


void Game::disp_gen_map_status( int curStep, int maxStep, int section )
{
	const int POPUP_WINDOW_WIDTH = 266;
	const int POPUP_WINDOW_HEIGHT = 149;
	const int POPUP_WINDOW_X1 = (vga_front.buf_width() - POPUP_WINDOW_WIDTH) / 2;
	const int POPUP_WINDOW_Y1 = (vga_front.buf_height() - POPUP_WINDOW_HEIGHT) / 2;

	const int BAR_X1 = POPUP_WINDOW_X1 + 46;
	// ###### begin Gilbert 29/10 ######//
	const int BAR_Y1 = POPUP_WINDOW_Y1 + 106;
	// ###### end Gilbert 29/10 ######//

	const int MAX_SECTION = 2;		// section 0 for world.genmap, section 1 for battle.run
	static int accSectionWeight[MAX_SECTION+1] = { 0, 60, 100 };

	if( section == 0 && curStep == 0)
	{
		image_menu.put_front(POPUP_WINDOW_X1, POPUP_WINDOW_Y1, "NEWWORLD");
	}

	err_when( section < 0 || section >= MAX_SECTION );
	err_when( curStep < 0 || curStep > maxStep );
	if( curStep >= 0 && curStep <= maxStep)
	{
		float r = float(accSectionWeight[section]) + 
			float((accSectionWeight[section+1]-accSectionWeight[section]) * curStep) / maxStep;
		vga_front.indicator(4, BAR_X1, BAR_Y1, r, (float)accSectionWeight[MAX_SECTION], 0);
	}

	sys.blt_virtual_buf();
}
