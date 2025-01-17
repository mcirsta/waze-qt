/* roadmap_qtbrowser.c - Qt Browser functionality
 *
 * LICENSE:
 *
  *   Copyright 2012 Assaf Paz, Waze Ltd
 *
 *   This file is part of RoadMap.
 *
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
 */

#ifdef TODO
#include "roadmap_qtbrowser.h"
#include "qt_main.h"
#include <QWebView>
#include <QWebFrame>

extern "C" {
#include "roadmap_browser.h"
#include "ssd/ssd_dialog.h"
}

extern QDeclarativeView* mainWindow;

static QObject *webview = NULL;

static void roadmap_qtbrowser_launcher( RMBrowserContext* context );
static void roadmap_qtbrowser_close();
static void _resize( const RoadMapGuiRect* rect );

/***********************************************************/
/*  Name        : void roadmap_qtbrowser_init()
 *  Purpose     : Initializes the browser
 *  Params      : void
 */
void roadmap_qtbrowser_init( void )
{
   /*
    * Launcher and closer registration.
    */
    roadmap_browser_register_launcher( (RMBrowserLauncherCb) roadmap_qtbrowser_launcher );
    roadmap_browser_register_close( roadmap_qtbrowser_close );
    roadmap_browser_register_resize( _resize );
}

/*************************************************************************************************
 * void roadmap_main_browser_launcher( RMBrowserContext* context )
 * Shows the android browser view
 *
 */
static void roadmap_qtbrowser_launcher( RMBrowserContext* context )
{
    if (webview == NULL)
    {
        webview = mainWindow->rootObject()->findChild<QObject*>("wazeBrowser");
    }

    _resize(&(context->rect));

    QString url = QString::fromAscii(context->url);

    if ( url.indexOf("groups?") == -1)
    {
        // only groups require deviceid
        url.remove("&deviceid=90");
    }

    webview->setProperty("url", url);
    webview->setProperty("visible", true);
}

void roadmap_qtbrowser_close()
{
    webview->setProperty("visible", false);
    webview->setProperty("url", "");
}

/*************************************************************************************************
 * void roadmap_groups_browser_btn_home_cb( void )
 * Custom button callback - Groups browser
 *
 */
void roadmap_groups_browser_btn_home_cb( void )
{
    // TODO
}
/*************************************************************************************************
 * void roadmap_groups_browser_btn_back_cb( void )
 * Custom button callback - Groups browser
 *
 */
void roadmap_groups_browser_btn_back_cb( void )
{
   // TODO
}
/*************************************************************************************************
 * void _resize
 * Browser resize wrapper
 *
 */
static void _resize( const RoadMapGuiRect* rect )
{
    webview->setProperty("x", rect->minx);
    webview->setProperty("y", rect->miny);
    webview->setProperty("width", rect->maxx - rect->minx);
    webview->setProperty("height", rect->maxy - rect->miny);
}

#endif
