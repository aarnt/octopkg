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

#include "constants.h"
#include "wmhelper.h"
#include "unixcommand.h"
#include "strconstants.h"
//#include "settingsmanager.h"
#include <iostream>

#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

/*
 * This class is a helper to abstract some Desktop Environments services for OctoPkg.
 * These include: open and edit a file, open a directory and a terminal.
 *
 * There's also a method to retrieve the available tool to obtain root privileges.
 */

/*
 * Checks if KDE is running
 */
bool WMHelper::isKDERunning(){
  return (qgetenv("XDG_CURRENT_DESKTOP").toLower() == QByteArray("kde"));
}

/*
 * Checks if TDE is running
 */
bool WMHelper::isTDERunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_TDE_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();

  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_TDE_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Checks if XFCE is running
 */
bool WMHelper::isXFCERunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_XFCE_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_XFCE_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Checks if LXDE is running
 */
bool WMHelper::isLXDERunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_LXDE_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_LXDE_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Checks if LXQT is running
 */
bool WMHelper::isLXQTRunning()
{
  return (qgetenv("XDG_CURRENT_DESKTOP").toLower() == QByteArray("lxqt"));
}

/*
 * Checks if OpenBox is running
 */
bool WMHelper::isOPENBOXRunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_OPENBOX_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_OPENBOX_DESKTOP)>0)
    return true;
  else
      return false;
}

/*
 * Checks if MATE is running
 */
bool WMHelper::isMATERunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_MATE_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_MATE_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Checks if Cinnamon is running
 */
bool WMHelper::isCinnamonRunning(){
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_CINNAMON_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_CINNAMON_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Checks if Lumina is running
 */
bool WMHelper::isLuminaRunning()
{
  QStringList slParam;
  QProcess proc;
  slParam << QStringLiteral("-aux");
  //slParam << ctn_LUMINA_DESKTOP;

  proc.start(QStringLiteral("ps"), slParam);
  proc.waitForStarted();
  proc.waitForFinished();
  QString out = QString::fromUtf8(proc.readAll());
  proc.close();

  if (out.count(ctn_LUMINA_DESKTOP)>0)
    return true;
  else
    return false;
}

/*
 * Retrieves the XFCE editor...
 */
QString WMHelper::getXFCEEditor(){
  if (UnixCommand::hasTheExecutable(ctn_XFCE_EDITOR))
    return ctn_XFCE_EDITOR;
  else
    return ctn_XFCE_EDITOR_ALT;
}

/*
 * The generic SU get method. It retrieves the SU you have installed in your system!
 */
QString WMHelper::getSUCommand(){
  return ctn_OCTOPKG_DOAS;
}

/*
 * Chooses whether to use kde-open or kde5-open
 */
QString WMHelper::getKDEOpenHelper(){
  if (UnixCommand::hasTheExecutable(ctn_KDE4_OPEN))
    return ctn_KDE4_OPEN;
  else if (UnixCommand::hasTheExecutable(ctn_KDE5_OPEN))
    return ctn_KDE5_OPEN;
  else
    return "NONE";
}

/*
 * Opens a file using the respective DE file manager
 */
void WMHelper::openFile(const QString& fileName){
  QString fileToOpen(fileName);

  bool isTextFile = UnixCommand::isTextFile(fileToOpen);

  if (!isTextFile){
    int res = QMessageBox::question(qApp->activeWindow(), StrConstants::getConfirmation(),
                                    StrConstants::getThisIsNotATextFile(),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

    if ( res == QMessageBox::No ) return;
  }

  QProcess *p = new QProcess(qApp->activeWindow());
  QStringList s;

  if (isXFCERunning() && UnixCommand::hasTheExecutable(ctn_XFCE_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_XFCE_FILE_MANAGER, s );
  }
  else if (isKDERunning() && UnixCommand::hasTheExecutable(ctn_KDE_FILE_MANAGER)){
    s << "exec";
    s << "file:" + fileToOpen;
    p->start( ctn_KDE_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_KDE4_FILE_MANAGER)){
    s << fileToOpen;
    p->start( getKDEOpenHelper(), s );
  }
  else if (isTDERunning() && UnixCommand::hasTheExecutable(ctn_TDE_FILE_MANAGER)){
    s << "exec";
    s << "file:" + fileToOpen;
    p->start( ctn_TDE_FILE_MANAGER, s );
  }
  else if (isMATERunning() && UnixCommand::hasTheExecutable(ctn_MATE_EDITOR)){
    s << fileToOpen;
    p->start( ctn_MATE_EDITOR, s );
  }
  else if (isCinnamonRunning() && UnixCommand::hasTheExecutable(ctn_CINNAMON_EDITOR)){
    s << fileToOpen;
    p->start( ctn_CINNAMON_EDITOR, s );
  }
  else if (isLXQTRunning() && UnixCommand::hasTheExecutable(ctn_LXQT_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_LXQT_FILE_MANAGER, s );
  }
  else if (isLuminaRunning() && UnixCommand::hasTheExecutable(ctn_LUMINA_OPEN)){
    s << fileToOpen;
    p->start( ctn_LUMINA_OPEN, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_ARCHBANG_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_ARCHBANG_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_LXQT_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_LXQT_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_LXDE_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_LXDE_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_XFCE_FILE_MANAGER)){
    s << fileToOpen;
    p->start( ctn_XFCE_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_GNOME_FILE_MANAGER)){
    s << fileToOpen;
    if (isTextFile)
      p->start( ctn_GNOME_EDITOR, s );
    else
      p->start( ctn_GNOME_FILE_MANAGER, s );
  }
  else if (UnixCommand::hasTheExecutable(ctn_XDG_OPEN)){
    s << fileToOpen;
    p->start( ctn_XDG_OPEN, s );
  }
}

/*
 * Edits a file based on your DE.
 */
void WMHelper::editFile( const QString& fileName, EditOptions opt ){
  QProcess *process = new QProcess(qApp->activeWindow());
  QString p;

  if (isKDERunning() && UnixCommand::hasTheExecutable(ctn_KDE4_EDITOR)){
    p += ctn_KDE4_EDITOR + " " + fileName;
  }
  else if (isMATERunning() && UnixCommand::hasTheExecutable(ctn_MATE_EDITOR)){
    p = ctn_MATE_EDITOR + " " + fileName;
  }
  else if (isLXQTRunning() && UnixCommand::hasTheExecutable(ctn_LXQT_EDITOR)){
    p = ctn_LXQT_EDITOR + " " + fileName;
  }
  else if (isLXQTRunning() && UnixCommand::hasTheExecutable(ctn_LXQT_EDITOR_ALT)){
    p = ctn_LXQT_EDITOR_ALT + " " + fileName;
  }
  else if (isLuminaRunning() && UnixCommand::hasTheExecutable(ctn_LUMINA_EDITOR)){
    p += ctn_LUMINA_EDITOR + " " + fileName;
  }
  else if (UnixCommand::hasTheExecutable(ctn_ARCHBANG_EDITOR))
  {
    p = ctn_ARCHBANG_EDITOR + " " + fileName;
  }
  else if (UnixCommand::hasTheExecutable(ctn_CINNAMON_EDITOR)){
    p = ctn_CINNAMON_EDITOR + " " + fileName;
  }
  else if (UnixCommand::hasTheExecutable(ctn_ARCHBANG_EDITOR))
  {
    p = ctn_ARCHBANG_EDITOR + " " + fileName;
  }
  else if (isXFCERunning() && (UnixCommand::hasTheExecutable(ctn_XFCE_EDITOR) ||
                               UnixCommand::hasTheExecutable(ctn_XFCE_EDITOR_ALT))){

    p = getXFCEEditor() + " " + fileName;
  }
  else if (isTDERunning() && UnixCommand::hasTheExecutable(ctn_TDE_EDITOR)){
    p += ctn_TDE_EDITOR + " " + fileName;
  }
  else if (UnixCommand::hasTheExecutable(ctn_XFCE_EDITOR) || UnixCommand::hasTheExecutable(ctn_XFCE_EDITOR_ALT)){
    p = getXFCEEditor() + " " + fileName;
  }

  if (opt == ectn_EDIT_AS_NORMAL_USER)
  {
    QStringList sl;
    sl << QStringLiteral("-c");

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList params = p.split(QStringLiteral(" "), QString::SkipEmptyParts);
#else
    QStringList params = p.split(QStringLiteral(" "), Qt::SkipEmptyParts);
#endif

    sl << params;
    process->startDetached(UnixCommand::getShell(), sl);
  }
  else
  {

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList params = p.split(QStringLiteral(" "), QString::SkipEmptyParts);
#else
    QStringList params = p.split(QStringLiteral(" "), Qt::SkipEmptyParts);
#endif

    process->startDetached(getSUCommand(), params);
  }
}

/*
 * Opens a directory based on your DE.
 */
void WMHelper::openDirectory( const QString& dirName ){
  QProcess *p = new QProcess(qApp->activeWindow());
  QStringList s;
  QString dir(dirName);

  //Is it really a directory?
  QFileInfo f(dirName);
  if (!f.isDir())
  {
    dir = f.absolutePath();
    f = QFileInfo(dir);
  }

  if (f.exists())
  {
    if (UnixCommand::hasTheExecutable(ctn_ARCHBANG_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_ARCHBANG_FILE_MANAGER, s );
    }
    else if(isXFCERunning() && UnixCommand::hasTheExecutable(ctn_XFCE_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_XFCE_FILE_MANAGER, s );
    }
    else if (isKDERunning())
    {
      if (UnixCommand::hasTheExecutable(ctn_KDE4_FILE_MANAGER))
      {
        s << dir;
        p->startDetached( ctn_KDE4_FILE_MANAGER, s);
      }
      else if (UnixCommand::hasTheExecutable(ctn_KDE_FILE_MANAGER))
      {
        s << "newTab";
        s << dir;
        p->startDetached( ctn_KDE_FILE_MANAGER, s );
      }
    }
    else if (isTDERunning())
    {
      if (UnixCommand::hasTheExecutable(ctn_TDE_FILE_MANAGER))
      {
        s << "newTab";
        s << dir;
        p->startDetached( ctn_TDE_FILE_MANAGER, s );
      }
    }
    else if (isMATERunning() && UnixCommand::hasTheExecutable(ctn_MATE_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_MATE_FILE_MANAGER, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_XFCE_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_XFCE_FILE_MANAGER, s );
    }
    else if (isLXQTRunning() && UnixCommand::hasTheExecutable(ctn_LXQT_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_LXQT_FILE_MANAGER, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_LXDE_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_LXDE_FILE_MANAGER, s );
    }
    else if (isLuminaRunning() && UnixCommand::hasTheExecutable(ctn_LUMINA_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_LUMINA_FILE_MANAGER, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_ANTERGOS_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_ANTERGOS_FILE_MANAGER, s );
    }
    else if (isCinnamonRunning() && UnixCommand::hasTheExecutable(ctn_CINNAMON_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_CINNAMON_FILE_MANAGER, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_GNOME_FILE_MANAGER))
    {
      s << dir;
      p->startDetached( ctn_GNOME_FILE_MANAGER, s );
    }
  }
}

/*
 * Opens a terminal based on your DE.
 */
void WMHelper::openTerminal(const QString& dirName)
{
  QFileInfo f(dirName);
  if (!f.exists()) return;
  
  QStringList s;
  QProcess *m_process = new QProcess(qApp->activeWindow());

    if(WMHelper::isXFCERunning() && UnixCommand::hasTheExecutable(ctn_XFCE_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_XFCE_TERMINAL, s );
    }
    else if (WMHelper::isKDERunning() && UnixCommand::hasTheExecutable(ctn_KDE_TERMINAL)){
      s << "--workdir";
      s << dirName;
      m_process->startDetached( ctn_KDE_TERMINAL, s );
    }
    else if (WMHelper::isTDERunning() && UnixCommand::hasTheExecutable(ctn_TDE_TERMINAL)){
      s << "--workdir";
      s << dirName;
      m_process->startDetached( ctn_TDE_TERMINAL, s );
    }
    else if (WMHelper::isLXDERunning() && UnixCommand::hasTheExecutable(ctn_LXDE_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_LXDE_TERMINAL, s );
    }
    else if (WMHelper::isMATERunning() && UnixCommand::hasTheExecutable(ctn_MATE_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_MATE_TERMINAL, s );
    }
    else if (WMHelper::isCinnamonRunning() && UnixCommand::hasTheExecutable(ctn_CINNAMON_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_CINNAMON_TERMINAL, s );
    }
    else if (WMHelper::isLXQTRunning() && UnixCommand::hasTheExecutable(ctn_LXQT_TERMINAL)){
      s << "--workdir";
      s << dirName;
      m_process->startDetached( ctn_LXQT_TERMINAL, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_PEK_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_PEK_TERMINAL, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_XFCE_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_XFCE_TERMINAL, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_LXDE_TERMINAL)){
      s << "--working-directory=" + dirName;
      m_process->startDetached( ctn_LXDE_TERMINAL, s );
    }
    else if (UnixCommand::hasTheExecutable(ctn_RXVT_TERMINAL)){
      QString cmd;

      if (UnixCommand::isAppRunning("urxvtd"))
      {
        cmd = "urxvtc";

        s << "-name";
        s << "Urxvt";
        s << "-title";
        s << "Urxvt";
        s << "-cd";
        s << dirName;
      }
      else
      {
        cmd = ctn_RXVT_TERMINAL;

        s << "-name";
        s << "Urxvt";
        s << "-title";
        s << "Urxvt";
        s << "-cd";
        s << dirName;
      }

      m_process->startDetached(cmd, s);
    }
    else if (UnixCommand::hasTheExecutable(ctn_XTERM)){
      QString cmd = ctn_XTERM;

      s << "-fn";
      s << "*-fixed-*-*-*-18-*";
      s << "-fg";
      s << "White";
      s << "-bg";
      s << "Black";
      s << "-title";
      s << "xterm";
      s << "-e";
      s << "cd " + dirName + " && /bin/sh";

      m_process->startDetached(cmd, s);
    }
}
