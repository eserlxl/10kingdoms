# Memory Management Audit

Generated: Sun Jan 18 10:06:00 +03 2026

## Manual Memory Management

### malloc/free usage
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OU_GOD.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OREMOTE2.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OAI_CAP2.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OHELP.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OBULLET.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OBULLET.cpp:89:	sprite_info->load_bitmap_res();		// the sprite bitmap will be freed by ~Sprite(), so we don't have to add ~Bullet() to free it. `
  - `/opt/lxl/c++/10kingdoms/src/OFIRMIF.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OU_VEHI.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OSYS3.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OERROR.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OREMOTEQ.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OR_RANK.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OR_TECH.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:1083:				continue;	// a free slot`
  - `/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OTOWNDRW.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:6: * This program is free software: you can redistribute it and/or modify`

### new/delete usage
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:59:		delete[] sprite_info_array;`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:88:	sprite_info_array = new SpriteInfo[sprite_info_count];`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:235:	//----------- delete temp arrays -------------//`
  - `/opt/lxl/c++/10kingdoms/src/OREMOTE2.cpp:70:   else //---- allocate a new RemoteMsg if the requested one is bigger than common_msg_buf ----//`
  - `/opt/lxl/c++/10kingdoms/src/OREMOTE2.cpp:448:				//--- check if this message indicates the start of a new message queue ---//`
  - `/opt/lxl/c++/10kingdoms/src/OAI_CAP2.cpp:41:	if( ai_camp_count==0 )		// this can happen when a new nation has just emerged`
  - `/opt/lxl/c++/10kingdoms/src/OAI_CAP2.cpp:44:	if( ai_capture_enemy_town_recno )		// no new action if we are still trying to capture a town`
  - `/opt/lxl/c++/10kingdoms/src/OU_VEHI.cpp:96:	//-------- delete current unit ----------//`
  - `/opt/lxl/c++/10kingdoms/src/OU_VEHI.cpp:100:	unit_array.del(sprite_recno);		// delete the vehicle (e.g. horse)`
  - `/opt/lxl/c++/10kingdoms/src/OERROR.cpp:24:#include <new>`
  - `/opt/lxl/c++/10kingdoms/src/OERROR.cpp:61:// Set new() operator error handler, new_handler() is called when`
  - `/opt/lxl/c++/10kingdoms/src/OERROR.cpp:62:// new cannot allocate sufficient memory required.`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:100:	reset_unit_home_camp(firmRecno);		// this must be called at last as Firm::deinit() will create new units.`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:790:					kill_worker(i);		// if its hit points is zero, delete it`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:813:	// outside and he is now going to lead a new team, then`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:967:			unitPtr->update_loyalty();							// the unit is just assigned to a new leader, set its target loyalty`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:1121:	// attack new target`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:1222:		//----------------- attack new target now -------------------//`
  - `/opt/lxl/c++/10kingdoms/src/OF_CAMP.cpp:1243:	//------- a button should exist for accept new worker or not ---------//`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:95:// Chain : delete nation --> delete firm --> delete job --> delete item`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:125:// new firm.`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:181:// 1. FirmArray::build_firm()  for setting up a new firm`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:195:			firmPtr = new FirmBase;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:199:			firmPtr = new FirmCamp;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:203:			firmPtr = new FirmFactory;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:207:			firmPtr = new FirmInn;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:211:			firmPtr = new FirmMarket;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:215:			firmPtr = new FirmMine;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:219:			firmPtr = new FirmResearch;`
  - `/opt/lxl/c++/10kingdoms/src/OFIRMA.cpp:223:			firmPtr = new FirmWar;`

## Raw Pointer Usage

  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:2: * Seven Kingdoms: Ancient Adversaries`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:4: * Copyright 2010 Jesse Allen`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:7: * it under the terms of the GNU General Public License as published by`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:8: * the Free Software Foundation, either version 2 of the License, or`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:11: * This program is distributed in the hope that it will be useful,`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:12: * but WITHOUT ANY WARRANTY; without even the implied warranty of`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:13: * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:14: * GNU General Public License for more details.`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:16: * You should have received a copy of the GNU General Public License`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:37:	const char *debug_env_var;`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:80:unsigned char dbglog_get_flags(char *name)`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:103:void dbglog_printf(enum __dbglog_class msg_class, struct __dbglog_channel *c, const char *format, ...)`
  - `/opt/lxl/c++/10kingdoms/src/dbglog.cpp:116:	tm *curTime = localtime(&rawTime);`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:2: * Seven Kingdoms: Ancient Adversaries`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:4: * Copyright 1997,1998 Enlight Software Ltd.`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:6: * This program is free software: you can redistribute it and/or modify`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:7: * it under the terms of the GNU General Public License as published by`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:8: * the Free Software Foundation, either version 2 of the License, or`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:11: * This program is distributed in the hope that it will be useful,`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:12: * but WITHOUT ANY WARRANTY; without even the implied warranty of`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:13: * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:14: * GNU General Public License for more details.`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:16: * You should have received a copy of the GNU General Public License`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:73:	Database   	    *dbSprite 	 = game_set.open_db(SPRITE_DB);`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:74:	SpriteRec  	    *spriteRec;`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:75:	SpriteInfo 	    *spriteInfo;`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:76:	SpriteActionRec *spriteActionRec;`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:77:	SpriteMove      *spriteMove;`
  - `/opt/lxl/c++/10kingdoms/src/OSPRTRES.cpp:78:	SpriteAttack    *spriteAttack;`

