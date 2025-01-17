/* roadmap_httpcopy_async.cc - Download RoadMap maps using the HTTP protocol asynchornously.
 *
 * LICENSE:
 *
 *   Copyright 2003 Pascal Martin.
 *   Copyright 2008 Ehud Shabtai
 *   Copyright 2012 Assaf Paz
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
 *
 *
 */

#include "qt_webaccessor.h"
#include <QFile>

extern "C" {
#include "roadmap.h"
#include "roadmap_io.h"
#include "roadmap_net.h"
#include "roadmap_file.h"
#include "roadmap_path.h"
#include "roadmap_main.h"
#include "roadmap_base64.h"

#include "editor/editor_main.h"
#include "../roadmap_httpcopy_async.h"
}

#define ROADMAP_HTTP_MP_BOUNDARY   "---------------------------10424402741337131014341297293"

bool roadmap_http_async_post_file( RoadMapHttpAsyncCallbacks *callbacks, void *context,
                                           const char *source, const char* header, const char *full_name, int size )
{
    QFile file(QString::fromAscii(full_name));
    QByteArray data = file.readAll();
    int data_length = data.length();

    return WazeWebAccessor::getInstance().postRequestProgress(QString::fromAscii(source), HTTPCOPY_FLAG_NONE, callbacks, context, header, data, data_length);
}

static char* get_encoded_auth (const char *user, const char *pw) {
   char auth_string[255];
   char *base64Text;
   int text_length;
   int size;

   snprintf (auth_string, sizeof(auth_string), "%s:%s", user, pw);

   size = strlen(auth_string);
   text_length = roadmap_base64_get_buffer_size(size);
   base64Text = new char[text_length];

   if (!roadmap_base64_encode(auth_string, size, &base64Text, text_length))
      return NULL;

   return base64Text;
}

const char* roadmap_http_async_get_upload_header( const char* content_type, const char *full_name, int size,
                                                 const char *user, const char *pw)
{
   static char s_header[512];
   char *encoded_auth;

   const char *filename = roadmap_path_skip_directories (full_name);

   s_header[0] = '\0';

   if (user && user[0] && pw && pw[0]) {
      encoded_auth = get_encoded_auth(user, pw);
      if (!encoded_auth) {
         return s_header;
      }
      snprintf( s_header, sizeof( s_header ),
               "Authorization: Basic %s\r\n",
               encoded_auth);

      delete(encoded_auth);

   }


   snprintf( s_header + strlen(s_header), sizeof( s_header ) - strlen(s_header),
            "Content-Type: multipart/form-data; boundary=%s\r\n"
            "Content-Length: %d\r\n\r\n"
            "--%s\r\n"
            "Content-disposition: form-data; name=\"file_0\"; filename=\"%s\"\r\n"
            "Content-type: %s\r\n"
            "Content-Transfer-Encoding: binary\r\n",
            ROADMAP_HTTP_MP_BOUNDARY,
            (int)(size + 237 + strlen(filename) + strlen(content_type)),
            ROADMAP_HTTP_MP_BOUNDARY,
            filename,
            content_type);

   return s_header;
}

void roadmap_http_async_post( RoadMapHttpAsyncCallbacks *callbacks, void *context,
                                            const char *source, const char* header, const void* data, int data_length, int flags )
{
    WazeWebAccessor::getInstance().postRequestProgress(QString::fromAscii(source), flags, callbacks, context, header, data, data_length);
}

const char* roadmap_http_async_get_simple_header( const char* content_type, int content_len )
{
   static char s_header[256];

   snprintf( s_header, sizeof( s_header ),
               "Content-type: %s\r\n"
               "Content-Length: %d\r\n",
               content_type, content_len );

   return s_header;
}


HttpAsyncContext * roadmap_http_async_copy (RoadMapHttpAsyncCallbacks *callbacks,
                                      void *context,
                             const char *source,
                             time_t update_time) {
    return WazeWebAccessor::getInstance().getRequest(QString::fromAscii(source), 0, callbacks, update_time, context);
}

void roadmap_http_async_copy_abort (HttpAsyncContext *context) {
    roadmap_log(ROADMAP_INFO, "Aborting url request: %s", context->url.toString().toAscii().constData());
    context->http->abort();
}

