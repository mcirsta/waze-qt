/*
 * LICENSE:
 *
 *   Copyright 2010 Avi Ben-Shoshan
 *
 *   RoadMap is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License V2 as published by
 *   the Free Software Foundation.
 *
 *   RoadMap is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with RoadMap; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SINGLE_SEARCH_DLG_H_
#define SINGLE_SEARCH_DLG_H_

#include "../ssd/ssd_dialog.h"
void single_search_dlg_show( PFN_ON_DIALOG_CLOSED cbOnClosed, void*                context);

BOOL single_search_auto_search( const char* address);

void single_search_dlg_show_favorite( const char *favorite_name,
                                      PFN_ON_DIALOG_CLOSED cbOnClosed,
                                      void*                context);
BOOL single_search_add_favorite( const char* address, const char *favorite_name);
#endif /* SINGLE_SEARCH_DLG_H_ */
