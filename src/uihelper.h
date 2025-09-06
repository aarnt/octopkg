/*
* This file is part of OctoPkg, an open-source GUI for pkgng.
* Copyright (C) 2015 Alexandre Albuquerque Arnt
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#ifndef ICONHELPER_H
#define ICONHELPER_H

#include "wmhelper.h"
#include "unixcommand.h"

#include <QDir>
#include <QIcon>
#include <QApplication>
#include <QWidget>

/*
 * IconHelper provides some very used icons to the interface
 */

class IconHelper{
public:
  static QIcon getIconOctopiTransparent(){ return QIcon(QStringLiteral(":/resources/images/octopi_transparent.png")); }
  static QIcon getIconOctopiRed(){ return QIcon(QStringLiteral(":/resources/images/octopi_red.png")); }
  static QIcon getIconOctopiYellow(){ return QIcon(QStringLiteral(":/resources/images/octopi_yellow.png")); }
  static QIcon getIconOctopiGreen(){ return QIcon(QStringLiteral(":/resources/images/octopi_green.png")); }
  static QIcon getIconInstalled(){ return QIcon(QStringLiteral(":/resources/images/installed.png")); }
  static QIcon getIconNonInstalled(){ return QIcon(QStringLiteral(":/resources/images/noninstalled.png")); }
  static QIcon getIconOutdated(){ return QIcon(QStringLiteral(":/resources/images/outdated.png")); }
  static QIcon getIconNewer(){ return QIcon(QStringLiteral(":/resources/images/newer.png")); }
  static QIcon getIconUnrequired(){ return QIcon(QStringLiteral(":/resources/images/unrequired.png")); }
  static QIcon getIconLocked(){ return QIcon(QStringLiteral(":/resources/images/locker-key.png")); }
  static QIcon getIconUnlock(){ return QIcon(QStringLiteral(":/resources/images/unlock.png")); }
  static QIcon getIconForeignGreen(){ return QIcon(QStringLiteral(":/resources/images/foreign_green.png")); }
  static QIcon getIconForeignRed(){ return QIcon(QStringLiteral(":/resources/images/foreign_red.png")); }
  static QIcon getIconStop(){ return QIcon(QStringLiteral(":/resources/images/stop_small_red.png")); }

  static QIcon getIconHardDrive(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("drive-harddisk", QIcon(QStringLiteral(":/resources/images/drive-harddisk.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/drive-harddisk.png"));
  }

  static QIcon getIconBinary(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("application-x-object",QIcon(QStringLiteral(":/resources/images/binary.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/binary.png"));
  }

  static QIcon getIconToRemove(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("dialog-cancel", QIcon(QStringLiteral(":/resources/images/toremove.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/toremove.png"));
  }

  static QIcon getIconToInstall(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("download", QIcon(QStringLiteral(":/resources/images/toinstall.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/toinstall.png"));
  }

  static QIcon getIconTerminal(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("utilities-terminal", QIcon(QStringLiteral(":/resources/images/terminal.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/terminal.png"));
  }

  static QIcon getIconRemoveItem() {
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("list-remove", QIcon(QStringLiteral(":/resources/images/remove_item.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/remove_item.png"));
  }

  static QIcon getIconInstallItem() {
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("list-add", QIcon(QStringLiteral(":/resources/images/install_item.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/install_item.png"));
  }

  static QIcon getIconExit(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("application-exit", QIcon(QStringLiteral(":/resources/images/exit.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/exit.png"));
  }

  // Icons for QActions
  static QIcon getIconSyncDatabase(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("view-refresh", QIcon(QStringLiteral(":/resources/images/refresh.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/refresh.png"));
  }

  static QIcon getIconCommit(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("dialog-ok-apply", QIcon(QStringLiteral(":/resources/images/commit.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/commit.png"));
  }

  static QIcon getIconRollback(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-undo", QIcon(QStringLiteral(":/resources/images/rollback.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/rollback.png"));
  }

  static QIcon getIconSystemUpgrade(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("go-up", QIcon(QStringLiteral(":/resources/images/fast_forward.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/fast_forward.png"));
  }

  static QIcon getIconGetNews(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("application-rss+xml", QIcon(QStringLiteral(":/resources/images/rss.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/rss.png"));
  }

  static QIcon getIconCollapse(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("zoom-out", QIcon(QStringLiteral(":/resources/images/collapse.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/collapse.png"));
  }

  static QIcon getIconExpand(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("zoom-in", QIcon(QStringLiteral(":/resources/images/expand.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/expand.png"));
  }

  static QIcon getIconEditFile(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("document-edit", QIcon(QStringLiteral(":/resources/images/editfile.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/editfile.png"));
  }

  static QIcon getIconEditCopy(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-copy", QIcon(QStringLiteral(":/resources/images/edit-copy.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/edit-copy.png"));
  }

  static QIcon getIconFolder(){
      if (WMHelper::isKDERunning())
          return QIcon::fromTheme(QStringLiteral("document-open-folder"), QIcon(QStringLiteral(":/resources/images/folder.png")));
      else
          return QIcon::fromTheme(QStringLiteral("folder"), QIcon(QStringLiteral(":/resources/images/folder.png")));
  }

  static QIcon getIconFindFileInPackage(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-find", QIcon(QStringLiteral(":/resources/images/find.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/find.png"));
  }

  /*static QIcon getIconMirrorCheck(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("svn-update", QIcon(QStringLiteral(":/resources/images/mirror-check.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/mirror-check.png"));
  }*/

  static QIcon getIconShowGroups(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("view-list-tree", QIcon(QStringLiteral(":/resources/images/show_groups.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/show_groups.png"));
  }

  static QIcon getIconClose(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-delete", QIcon(QStringLiteral(":/resources/images/window_close.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/window_close.png"));
  }

  static QIcon getIconSearch(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-find", QIcon(QStringLiteral(":/resources/images/esf-search.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/esf-search.png"));
  }

  static QIcon getIconClear(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("edit-clear-locationbar-ltr", QIcon(QStringLiteral(":/resources/images/esf-clear.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/esf-clear.png"));
  }

  static QIcon getIconInternet(){
    if (WMHelper::isKDERunning())
      return QIcon::fromTheme("applications-internet", QIcon(QStringLiteral(":/resources/images/cloud.png")));
    else
      return QIcon(QStringLiteral(":/resources/images/cloud.png"));
  }

  //QActions without icons in OctoPkg does this works for gtk, too?
  static QIcon getIconHelpAbout(){ return QIcon::fromTheme("help-about"); }
  static QIcon getIconHelpUsage(){ return QIcon::fromTheme("help-contents"); }
  static QIcon getIconInstallLocalPackage(){ return QIcon::fromTheme("utilities-file-archiver"); }
};

//This is a RAII class used when the GUI is going to face a very CPU intensive action
class CPUIntensiveComputing: public QObject{
private:
  QWidget *m_parent;

public:
  CPUIntensiveComputing(){
    m_parent = 0;
    QApplication::setOverrideCursor(Qt::WaitCursor);
  }

  CPUIntensiveComputing(QWidget *parent){
    if (parent) m_parent = parent;
    if (m_parent) m_parent->setCursor(Qt::WaitCursor);
  }

  virtual ~CPUIntensiveComputing()
  {
    if (m_parent)
      m_parent->setCursor(Qt::ArrowCursor);
    else
      QApplication::restoreOverrideCursor();
  }

  void restoreDefaultCursor(){
    QApplication::restoreOverrideCursor();
  }
};

#endif // ICONHELPER_H
