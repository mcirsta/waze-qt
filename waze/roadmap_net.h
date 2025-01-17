/* roadmap_gps.h - GPS interface for the RoadMap application.
 *
 * LICENSE:
 *
 *   Copyright 2002 Pascal F. Martin
 *   Copyright 2008 Ehud Shabtai
 *
 *   This file is part of RoadMap.
 *
 *   RoadMap is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
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

#ifndef _ROADMAP_NET__H_
#define _ROADMAP_NET__H_

#include <time.h>
#include "roadmap.h"

#define NET_COMPRESS 0x00000001
#define NET_RESOLVED 0x00000002
#define TEST_NET_COMPRESS( flags ) ( roadmap_net_get_compress_enabled() ? (flags & NET_COMPRESS) : 0 )

typedef void *RoadMapSocket;
#define ROADMAP_INVALID_SOCKET ((RoadMapSocket) NULL)


#define ROADMAP_NET_IS_VALID(s) (s != ROADMAP_INVALID_SOCKET)

typedef void (*RoadMapNetConnectCallback) (RoadMapSocket socket, void *context, roadmap_result res);
typedef void (*RoadMapNetSslConnectCallback) (RoadMapSocket s, void *data, void *context, roadmap_result res);

RoadMapSocket roadmap_net_connect(  const char*       protocol,
                                    const char*       name,
                                    time_t            update_time,
                                    int               default_port,
                                    int               flags,
                                    roadmap_result*   res); // Optional, can be NULL

// A-syncronious receive:
void *roadmap_net_connect_async (const char *protocol,
                                const char *name, 
                               const char *resolved_name,
                                time_t update_time,
                                int default_port,
                                int flags,
                                RoadMapNetConnectCallback callback,
                                void *context);

/* If there is any problem detected, the 2 functions below MUST return
 * a negative value (never 0).
 */
int roadmap_net_receive (RoadMapSocket s, void *data, int size);
int roadmap_net_send    (RoadMapSocket s, const void *data, int length,
                         int wait);
int roadmap_net_send_async (RoadMapSocket s, const void *data, int length);

RoadMapSocket roadmap_net_listen(int port);
RoadMapSocket roadmap_net_accept(RoadMapSocket server_socket);

int roadmap_net_unique_id (unsigned char *buffer, unsigned int size);

void roadmap_net_cancel_connect (void * context);

void roadmap_net_close  (RoadMapSocket s);

void roadmap_net_shutdown ( void );

void roadmap_net_initialize( void );

void roadmap_net_set_compress_enabled( BOOL value );
BOOL roadmap_net_get_compress_enabled( void );
int roadmap_net_get_fd(RoadMapSocket s);
#endif // _ROADMAP_NET__H_

