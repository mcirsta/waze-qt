/* roadmap_native_keyboard.cc - native keyboard support
 *
 * LICENSE:
 *
 *   Copyright 2011 Assaf Paz
 *
 *   This file is part of Waze.
 *
 *   Waze is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   Waze is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Waze; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

extern "C" {
    #include "roadmap_native_keyboard.h"
}


/******************************



THIS FILE IS NOT REALLY USED




*****************************/


BOOL roadmap_native_keyboard_enabled( void ) {
    return false;
}

BOOL roadmap_native_keyboard_visible( void ) {
    return false;
}

void roadmap_native_keyboard_show( RMNativeKBParams* params ) {
}

void roadmap_native_keyboard_hide( void ) {
}

void roadmap_native_keyboard_get_params( RMNativeKBParams* params_out ) {
    /* never called */
}
