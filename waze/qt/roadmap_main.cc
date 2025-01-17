/*
 * LICENSE:
 *
 *   (c) Copyright 2002 Pascal F. Martin
 *   (c) Copyright 2003 Latchesar Ionkov
 *   (c) Copyright 2008 Ehud Shabtai
 *   (c) Copyright 2011 Assaf Paz
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>

// Crash handling
#include <signal.h>

#include <sstream>
#include <QApplication>
#include <QtDeclarative>
#include <QList>
#include <QDeclarativeItem>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include "qmlapplicationviewer.h"
#include "qt_main.h"
#include "roadmap_qtbrowser.h"
#include "qt_network.h"
#include "qt_contacts.h"
#include "qt_datamodels.h"

#ifdef Q_WS_MAEMO_5
#include <QtDBus/QtDBus>
#endif

extern "C" {
#include "roadmap.h"
#include "roadmap_start.h"
#include "roadmap_config.h"
#include "roadmap_history.h"
#include "roadmap_main.h"
#include "roadmap_time.h"
#include "editor/editor_main.h"
#include "roadmap_qtmain.h"
#include "tts_was_provider.h"
}

int USING_PHONE_KEYPAD = 0;

unsigned char APP_SHUTDOWN_FLAG = 0;	// The indicator if the application is in the exiting process

#define CRASH_DUMP_ADDR_NUM		200		/* The number of addresses to dump when crash is occured */
#define CRASH_DUMP_ADDR_NUM_ON_SHUTDOWN      20      /* The number of addresses to dump when crash is occured on shutdown
                                                      * to not blow up the log on each exit
                                                     */

static void roadmap_start_event (int event);

//===============================================================

static QApplication* app;

QGraphicsScene* scene = NULL;
QDeclarativeView* mainWindow;
#if defined(QTMOBILITY)
RContactsView* contactsView;
#endif
RMapTimers* timers;
QList<RoadMapIO*> ioList;
QMainWindow* appWindow;

static int RoadMapMainStatus;

typedef struct roadmap_main_timer {
   int id;
   RoadMapCallback callback;
}  roadmap_main_timer;
static roadmap_main_timer RoadMapMainPeriodicTimer[ROADMAP_MAX_TIMER];

////////////////////////////////////////

/*************************************************************************************************
 * void roadmap_main_show_contacts( void )
 * Requests the system to show the external browser with the requested url
 *
 */
void roadmap_main_open_url( const char* url )
{
   /* TODO */
}

void roadmap_main_set_input    (RoadMapIO *io, RoadMapInput callback)
{
    if (io != NULL &&
        io->subsystem == ROADMAP_IO_NET &&
        io->os.socket != NULL)
    {
        ioList.append(io);
        RNetworkSocket* socket = (RNetworkSocket*) io->os.socket;
        socket->set_io(io);
        socket->setCallback(callback, ReadDirection);
    }
}

void roadmap_main_set_output   (RoadMapIO *io, RoadMapInput callback, BOOL is_connect)
{
    if (io != NULL &&
        io->subsystem == ROADMAP_IO_NET &&
        io->os.socket != NULL)
    {
        ioList.append(io);
        RNetworkSocket* socket = (RNetworkSocket*) io->os.socket;
        socket->set_io(io);
        socket->setCallback(callback, WriteDirection);
    }
}

void roadmap_main_remove_input (RoadMapIO *io)
{
    ioList.removeOne(io);
}

RoadMapIO *roadmap_main_output_timedout(time_t timeout)
{
    QDateTime timeoutTime = QDateTime::fromTime_t(timeout);
    QList<RoadMapIO*>::iterator it = ioList.begin();
    for (; it != ioList.end(); it++)
    {
        RoadMapIO* io = *it;
        if (((RNetworkSocket*)io->os.socket)->isTimedOut(timeoutTime))
        {
            return io;
        }
    }

    return NULL;
}

void roadmap_main_new(const char* title, int width, int height) {

    editor_main_set(1);
}


void roadmap_main_set_keyboard(struct RoadMapFactoryKeyMap *bindings, RoadMapKeyInput callback) {
    /* TODO */
}

RoadMapMenu roadmap_main_new_menu () {
    return NULL;
}

void roadmap_main_free_menu (RoadMapMenu menu) {

}

void roadmap_main_popup_menu (RoadMapMenu menu,
                              int x, int y) {

}

void roadmap_main_add_menu(RoadMapMenu menu, const char* label) {

}

void roadmap_main_add_menu_item(RoadMapMenu menu,
                                const char* label,
                                const char* tip,
                                RoadMapCallback callback) {

}

void roadmap_main_add_separator(RoadMapMenu menu) {

}

void roadmap_main_add_toolbar (const char *orientation) {

}

void roadmap_main_add_tool(const char* label,
                           const char *icon,
                           const char* tip,
                           RoadMapCallback callback) {

}

void roadmap_main_add_tool_space(void) {

}

void roadmap_main_set_cursor (int newcursor) {

   if (mainWindow) {

      switch (newcursor) {

      case ROADMAP_CURSOR_NORMAL:
         mainWindow->unsetCursor ();
         break;

      case ROADMAP_CURSOR_WAIT:
         mainWindow->setCursor (QCursor(Qt::BusyCursor));
         break;
      }
   }
}


void roadmap_main_add_canvas(void) {
// The canvas is implicitely added to the main window.
//      if (mainWindow) {
//              mainWindow->addCanvas();
//      }
}

void roadmap_main_add_status(void) {
      RoadMapMainStatus = 1;
}

void roadmap_main_show(void) {
   if (mainWindow) {
       if (mainWindow->isFullScreen()) {
          mainWindow->showNormal();
       } else {
         mainWindow->showFullScreen();
       }
   }
}


void roadmap_main_set_periodic (int interval, RoadMapCallback callback) {

   if (timers) {
      timers->addTimer(interval, callback);
   }
}


void roadmap_main_remove_periodic (RoadMapCallback callback) {

   if (timers) {
      timers->removeTimer(callback);
   }
}

RoadMapCallback idle_callback;

void roadmap_main_set_idle_function (RoadMapCallback callback) {

   /* under QT, a timeout of 0 means the routine will run "right after"
    * the event queue is emptied.
    */
   if (timers) {
      idle_callback = callback;
      timers->addTimer(0, callback);
   }
}

void roadmap_main_remove_idle_function (void) {
   if (timers) {
      timers->removeTimer(idle_callback);
   }
}

void roadmap_main_toggle_full_screen (void) {
  mainWindow->showFullScreen();
}


void roadmap_main_flush (void) {

   if (app && app->hasPendingEvents ()) {
        app->processEvents ();
   }
}


int roadmap_main_flush_synchronous (int deadline) {

   if (app != NULL) {

      long start_time, duration;

      start_time = roadmap_time_get_millis();

      app->processEvents ();
      app->syncX();

      duration = roadmap_time_get_millis() - start_time;

      if (duration > deadline) {

         roadmap_log (ROADMAP_DEBUG, "processing flush took %d", duration);

         return 0; /* Busy. */
      }
   }
   return 1;
}


void roadmap_main_exit(void) {

    if ( !APP_SHUTDOWN_FLAG )
    {
       roadmap_log( ROADMAP_WARNING, "Exiting the application\n" );

       APP_SHUTDOWN_FLAG = 1;
    }

   roadmap_start_exit();

   delete timers;
#if defined(QTMOBILITY)
   delete contactsView;
#endif
   delete mainWindow;

   app->quit();
   exit(0);
}

static int roadmap_main_signals_init()
{
#ifndef __WIN32
  struct sigaction signala;

  memset(&signala, 0, sizeof (struct sigaction));

  signala.sa_handler = RCommonApp::signalHandler;
  sigemptyset(&signala.sa_mask);
  signala.sa_flags |= SA_RESTART;

  if (sigaction(SIGHUP, &signala, 0) > 0)
    return 1;
  if (sigaction(SIGTERM, &signala, 0) > 0)
    return 2;
  if (sigaction(SIGINT, &signala, 0) > 0)
    return 3;
  if (sigaction(SIGQUIT, &signala, 0) > 0)
    return 4;
  return 0;
#else
  signal(SIGTERM, RMapMainWindow::signalHandler);
  signal(SIGINT, RMapMainWindow::signalHandler);
#endif
}

void roadmap_gui_minimize() {
    /* TODO */
}

void roadmap_gui_maximize() {
    /* TODO */
}

void roadmap_main_minimize (void) {
#ifdef Q_WS_MAEMO_5
    QDBusConnection connection = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/","com.nokia.hildon_desktop","exit_app_view");
    connection.send(message);
#else
    appWindow->showMinimized();
#endif
}

/*************************************************************************************************
 * void roadmap_start_event (int event)
 * Start event hanler
 *
 */
static void roadmap_start_event (int event) {
    /* TODO */
    switch (event) {
           case ROADMAP_START_INIT:
           {
        #ifdef FREEMAP_IL
                  editor_main_check_map ();
        #endif      /*
                    roadmap_device_events_register( on_device_event, NULL);
                    roadmap_main_set_bottom_bar( TRUE );
                    roadmap_androidrecommend_init();
                    roadmap_androideditbox_init();
                    roadmap_androidspeechtt_init();*/
#ifdef TODO
                    roadmap_qtbrowser_init();
#endif
                    tts_was_provider_init();
                  break;
           }
   }
}

double atof_locale_safe(const char *str) {
    double result;
    std::istringstream s(str);
    std::locale l("C");
    s.imbue(l);
    s >> result;
    return result;

}

time_s roadmap_time_get_current()
{
    QDateTime qTime = QDateTime::currentDateTime();
    time_s current_time;

    current_time.year = qTime.date().year();
    current_time.month = qTime.date().month();
    current_time.day = qTime.date().day();
    current_time.hour = qTime.time().hour();
    current_time.min = qTime.time().minute();
    current_time.sec = qTime.time().second();
    current_time.msec = qTime.time().msec();

    return current_time;
}

time_t roadmap_time_translate(const char *hhmmss, const char *ddmmyy) {
    return QDateTime::fromString(QString(ddmmyy).append(QString(hhmmss)), QString("HHmmssddMMyy")).toTime_t();
}

#if defined(QTMOBILITY)
void roadmap_main_show_contacts() {
    if (contactsView == NULL)
    {
        contactsView = new RContactsView(mainWindow);
        contactsView->setGeometry(0, 0, mainWindow->width(), mainWindow->height());
        contactsView->initialize();
    }

    contactsView->show();
    RCommonApp::instance()->mouseAreaPressed();
}
#endif

void roadmap_main_set_qml_context_property(const char* name, QObject* value)
{
    mainWindow->rootContext()->setContextProperty(QString::fromAscii(name), value);
}

int main(int argc, char* argv[]) {

   app = new QApplication(argc, argv);
   RCommonApp* appUtil = RCommonApp::instance();

   QObject::connect(app, SIGNAL(aboutToQuit()), appUtil, SLOT(quit()));

   QCoreApplication::setOrganizationName("Waze");
   QCoreApplication::setApplicationName("Waze");

   qmlRegisterType<RMapCanvas>("org.waze", 1, 0, "WazeMap");

   QMainWindow w(app->desktop());
   w.setAttribute(Qt::WA_AcceptTouchEvents);
   w.grabGesture(Qt::PinchGesture);

   QmlApplicationViewer* appView = new QmlApplicationViewer;
   mainWindow = appView;

   appView->setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);

   appView->setMainQmlFile(QApplication::applicationDirPath() + QString(APP_PATH_PREFIX "/qml/MainView.qml"));
   appView->engine()->setProperty("width", app->desktop()->width());
   appView->engine()->setProperty("height", app->desktop()->height());

   qt_datamodels_register();

   appView->setAttribute(Qt::WA_TranslucentBackground);
   w.setCentralWidget(appView);
#if defined(Q_WS_MAEMO_5) || defined(MEEGO_VERSION_MAJOR) || defined(Q_WS_SIMULATOR)
   w.showFullScreen();
#else
   w.showNormal();
#endif
   appWindow = &w;

   QObject *item = dynamic_cast<QObject*>(mainWindow->rootObject());
   QObject::connect(item, SIGNAL(invokeAction(QString)), appUtil, SLOT(invokeAction(QString)));
   QObject::connect(item, SIGNAL(buttonClicked()), appUtil, SLOT(mouseAreaPressed()));

#if defined(QTMOBILITY)
   contactsView = NULL;
#endif

   roadmap_option (app->argc(), app->argv(), NULL);

   roadmap_start_subscribe ( roadmap_start_event );

   timers = new RMapTimers(app);

   roadmap_main_signals_init();

   roadmap_start(app->argc(), app->argv());

   return app->exec();
}
