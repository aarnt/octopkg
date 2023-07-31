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

#ifndef UNIXCOMMAND_H
#define UNIXCOMMAND_H

#include <QObject>
#include <QProcess>
#include <QRandomGenerator>
#include <QTime>
#include <unistd.h>

#include "package.h"
#include "utils.h"

const QString ctn_MIRROR_CHECK_APP("mirror-check");

enum CommandExecuting { ectn_NONE, ectn_CLEAN_CACHE, ectn_MIRROR_CHECK,
                        ectn_CHECK_UPDATES, ectn_SYSTEM_UPGRADE, ectn_INSTALL,
                        ectn_REMOVE, ectn_REMOVE_INSTALL,
                        ectn_LOCK, ectn_UNLOCK,
                        ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL,
                        ectn_RUN_IN_TERMINAL, ectn_LOCAL_PKG_REFRESH };

enum BSDFlavour { ectn_DRAGONFLYBSD, ectn_FREEBSD, ectn_GHOSTBSD, ectn_HARDENEDBSD, ectn_UNKNOWN };

enum Language { ectn_LANG_ENGLISH, ectn_LANG_USER_DEFINED };

//Forward class declarations.
class QString;
//class QStringList;
class Terminal;

class UnixCommand : public QObject{
  Q_OBJECT

private:
  QString m_readAllStandardOutput;
  QString m_readAllStandardError;
  QString m_errorString;
  Terminal *m_terminal;
  QProcess *m_process;
  static QFile *m_temporaryFile;

public:
  UnixCommand(QObject *parent);

  inline QProcess * getProcess(){ return m_process; }

  //Returns the BSD Flavour where OctoPkg is running on
  static BSDFlavour getBSDFlavour();

  //Delegations from Package class (due to QProcess use)
  static QString runCurlCommand(const QStringList &params);

  static QByteArray performQuery(const QStringList args);
  static QByteArray performQuery(const QString &args);
  static QByteArray getRemotePackageList(const QString &searchString, bool useCommentSearch = true);
  static QByteArray getLockedPackageList();
  static QByteArray getUnrequiredPackageList();
  static QByteArray getOutdatedPackageList();
  static QByteArray getForeignPackageList();
  static QByteArray getDependenciesList(const QString &pkgName);
  static QByteArray getPackageList(const QString &pkgName = "");
  static QByteArray getPackageInformation(const QString &pkgName, bool foreignPackage);
  static QByteArray getPackageContentsUsingPkg(const QString &pkgName);
  static bool isPkgfileInstalled();

  static QString getPackageByFilePath(const QString &filePath);
  static QStringList getFilePathSuggestions(const QString &file);
  static QByteArray getPackageGroups();
  static QByteArray getPackagesFromGroup(const QString &groupName);
  static QByteArray getInstalledPackages();
  static QByteArray getTargetUpgradeList(const QString &pkgName = "");
  static QByteArray getTargetRemovalList(const QString &pkgName);

  //static QString getSystemArchitecture();
  static bool hasInternetConnection();
  static bool doInternetPingTest();
  static bool isTextFile( const QString& fileName ); //fileName is Path + Name
  static bool isLSBRelated(const QString& fileName);
  static QString getPkgNGVersion();
  static bool hasTheExecutable( const QString& exeName );
  static bool isAppRunning(const QString &appName, bool justOneInstance = false);

  static bool isRootRunning(){
    int uid = geteuid();
    return (uid == 0); //Returns TRUE if root is running OctoPkg
  }

  static QFile* getTemporaryFile(){
    quint32 gen = QRandomGenerator::global()->generate();
    m_temporaryFile = new QFile(ctn_TEMP_ACTIONS_FILE + QString::number(gen));
    m_temporaryFile->open(QIODevice::ReadWrite|QIODevice::Text);
    m_temporaryFile->setPermissions(QFile::Permissions(QFile::ExeOwner|QFile::ReadOwner));

    return m_temporaryFile;
  }

  static void removeTemporaryFile(){
    if (m_temporaryFile != 0){
      m_temporaryFile->close();
      m_temporaryFile->remove();
      delete m_temporaryFile;
      m_temporaryFile = 0;
    }
  }

  static QString getShell();
  static bool isRootOnZFS();

  static void execCommandAsNormalUser(const QString &pCommand);
  static void execCommand(const QString &pCommand);
  static QByteArray getCommandOutput(const QString &pCommand);
  static void removeTemporaryFiles();

  void runCommandInTerminal(const QStringList& commandList);
  void runCommandInTerminalAsNormalUser(const QStringList& commandList);
  void terminateCommand();
  void executeCommand(const QString &pCommand, Language lang=ectn_LANG_ENGLISH);
  void executeCommand(QStringList &params);
  void executeCommandAsNormalUser(const QString &pCommand);

  QString readAllStandardOutput();
  QString readAllStandardError();
  QString errorString();

public slots:
  void processReadyReadStandardOutput();
  void processReadyReadStandardError();

signals:
  void started();
  void readyReadStandardOutput();
  void finished ( int, QProcess::ExitStatus );
  void readyReadStandardError();

  //ProcessWrapper signals
  void startedTerminal();
  void finishedTerminal(int, QProcess::ExitStatus);
};

#endif // UNIXCOMMAND_H
