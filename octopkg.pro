#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T20:45:08
#
#-------------------------------------------------

QT += core gui network xml widgets
DEFINES += OCTOPKG_EXTENSIONS
CONFIG += qt warn_on debug
LIBS += -L/usr/local/lib
INCLUDEPATH += /usr/local/include
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
DESTDIR += bin
OBJECTS_DIR += build
MOC_DIR += build
UI_DIR += build

HEADERS += src/QtSolutions/qtsingleapplication.h \
        src/packagetreeview.h \
        src/QtSolutions/qtlocalpeer.h \
        src/constants.h \
        src/mainwindow.h \
        src/strconstants.h \
        src/searchlineedit.h \
        src/argumentlist.h \
        src/settingsmanager.h \
        src/uihelper.h \
        src/package.h \
        src/unixcommand.h \
        src/wmhelper.h \
        src/treeviewpackagesitemdelegate.h \
        src/searchbar.h \
        src/transactiondialog.h \
        src/globals.h \
        src/multiselectiondialog.h \
        src/packagerepository.h \
        src/model/packagemodel.h \
        src/ui/octopkgtabinfo.h \
        src/utils.h \
        src/terminal.h

SOURCES += src/QtSolutions/qtsingleapplication.cpp \
        src/packagetreeview.cpp \
        src/QtSolutions/qtlocalpeer.cpp \
        src/main.cpp\
        src/mainwindow.cpp \
        src/strconstants.cpp \
        src/searchlineedit.cpp \
        src/argumentlist.cpp \
        src/settingsmanager.cpp \
        src/package.cpp \
        src/unixcommand.cpp \
        src/wmhelper.cpp \
        src/treeviewpackagesitemdelegate.cpp \
        src/mainwindow_init.cpp \
        src/mainwindow_transaction.cpp \
        src/mainwindow_events.cpp \
        src/mainwindow_help.cpp \
        src/searchbar.cpp \
        src/mainwindow_searchbar.cpp \
        src/transactiondialog.cpp \
        src/mainwindow_news.cpp \
        src/mainwindow_refresh.cpp \
        src/globals.cpp \
        src/multiselectiondialog.cpp \
        src/packagerepository.cpp \
        src/model/packagemodel.cpp \
        src/ui/octopkgtabinfo.cpp \
        src/utils.cpp \
        src/terminal.cpp

FORMS   += ui/mainwindow.ui \
        ui/transactiondialog.ui \
        ui/multiselectiondialog.ui

RESOURCES += resources.qrc
#TRANSLATIONS += resources/translations/octopkg_pt_BR.ts

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

isEmpty(DATADIR) {
    DATADIR = $$PREFIX/share
}

isEmpty(BINDIR) {
    BINDIR = $$PREFIX/bin
}

target.path = $$BINDIR
bin.path = $$BINDIR

desktop.path = $$DATADIR/applications
desktop.files += octopkg.desktop

icon.path = $$DATADIR/icons
icon.files += resources/images/octopi.png

INSTALLS += target bin desktop icon
