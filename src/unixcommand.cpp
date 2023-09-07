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

#include "unixcommand.h"
//#include "strconstants.h"
#include "wmhelper.h"
#include "constants.h"
#include <iostream>

#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>
#include <QtNetwork/QNetworkInterface>
#include <QRegularExpression>
#include <QTcpSocket>
#include <QDebug>

/*
 * Collection of methods to execute many Unix commands
 */

QFile *UnixCommand::m_temporaryFile = 0;

/*
 * Executes the CURL command and returns the StandardError Output, if result code <> 0.
 */
QString UnixCommand::runCurlCommand(const QStringList& params){
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);
  proc.start(QStringLiteral("curl"), params);
  proc.waitForStarted();
  proc.waitForFinished(-1);

  QString res("");

  if (proc.exitCode() != 0)
  {
    res = proc.readAllStandardError();
  }

  proc.close();
  return res;
}

/*
 * Performs a pkg query
 */
QByteArray UnixCommand::performQuery(const QStringList args)
{
  QByteArray result("");
  QProcess pkg;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pkg.setProcessEnvironment(env);

  pkg.start(ctn_PKG_BIN, args);
  pkg.waitForFinished();
  result = pkg.readAllStandardOutput();
  pkg.close();

  return result;
}

/*
 * Performs a pkg query
 * Overloaded with QString parameter
 */
QByteArray UnixCommand::performQuery(const QString &args)
{
  QByteArray result("");
  QProcess pkg;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");

  pkg.setProcessEnvironment(env);
  
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList params = args.split(QStringLiteral(" "), QString::SkipEmptyParts);
#else
  QStringList params = args.split(QStringLiteral(" "), Qt::SkipEmptyParts);
#endif  
  pkg.start(ctn_PKG_BIN, params);
  pkg.waitForFinished();
  result = pkg.readAllStandardOutput();

  pkg.close();
  return result;
}

/*
 * Cancels the running process. Returns octopi-sudo exit code.
 */
int UnixCommand::cancelProcess()
{
  QProcess pkg;
  QString pCommand = QLatin1String("killall pkg");

  QStringList sl;
  sl << pCommand;
  pkg.start(WMHelper::getSUCommand(), sl);
  pkg.waitForFinished(-1);
  return pkg.exitCode();
}

/*
 * Returns a string containing all AUR packages given a searchString parameter
 */
QByteArray UnixCommand::getRemotePackageList(const QString &searchString, bool useCommentSearch)
{
  QByteArray result("");

  if (useCommentSearch)
    result = performQuery("search -Q name -Q version -Q categories -Q www -Q comment -Q pkg-size --comment " + searchString);
  else
    result = performQuery("search -Q name -Q version -Q categories -Q www -Q comment -Q pkg-size " + searchString);

  return result;
}

/*
 * Returns a string containing all locked packages
 */
QByteArray UnixCommand::getLockedPackageList()
{
  QByteArray result = performQuery("lock --show-locked");
  return result;
}

/*
 * Returns a string containing all packages no one depends on
 */
QByteArray UnixCommand::getUnrequiredPackageList()
{
  //pkg query -e '%a = 0' '%n'
  QStringList params;
  params << QStringLiteral("query");
  params << QStringLiteral("-e");
  params << QStringLiteral("%a = 0");
  params << QStringLiteral("%n");

  QByteArray result = performQuery(params);
  return result;
}

/*
 * Returns a string containing all packages that are outdated since last DB sync
 */
QByteArray UnixCommand::getOutdatedPackageList()
{
  QByteArray result = performQuery("upgrade -n");
  return result;
}

/*
 * Returns a string containing all packages that are not contained in any repository
 * (probably the ones installed by a tool such as yaourt)
 */
QByteArray UnixCommand::getForeignPackageList()
{
  QByteArray result = performQuery(QStringList("-Qm"));
  return result;
}

/*
 * Retrieves the dependencies pkg list with 'pkg query %dn' command
 */
QByteArray UnixCommand::getDependenciesList(const QString &pkgName)
{
  QByteArray result = performQuery("query \"%dn\" " + pkgName);
  return result;
}

/*
 * Returns a string with the list of all packages available in all repositories
 * (installed + not installed)
 *
 * @param pkgName Used while the user is searching for the pkg that provides a certain file
 */
QByteArray UnixCommand::getPackageList(const QString &pkgName)
{
  QByteArray result;

  if (pkgName.isEmpty())
  {
    QStringList params;
    params << QStringLiteral("query");
    params << QStringLiteral("%n %v %o %sh %t %c");

    result = performQuery(params);
  }
  else
  {

  }

  return result;
}

/*
 * Given a package name and if it is default to the official repositories,
 * returns a string containing all of its information fields
 * (ex: name, description, version, dependsOn...)
 */
QByteArray UnixCommand::getPackageInformation(const QString &pkgName, bool foreignPackage = false)
{
  QString args;

  if(foreignPackage)
  {

  }
  else
  {
    args = "info " + pkgName;
  }

  QByteArray result = performQuery(args);
  return result;
}

/*
 * Given a package name, returns a string containing all the files inside it
 */
QByteArray UnixCommand::getPackageContentsUsingPkg(const QString& pkgName)
{
  QStringList params;
  params << QStringLiteral("query");
  params << QStringLiteral("%Fp");
  params << pkgName;
  QByteArray res = performQuery(params);

  //if the pkg is more than a MEGABYTE big, let's abort it!
  if (res.size() > 1048576) return "";
  else return res;
}


/*
 * Given a complete file path, returns the package that provides that file
 */
QString UnixCommand::getPackageByFilePath(const QString &filePath)
{
  QString pkgName="";
  QString out = performQuery("which -o " + filePath);

  if (!out.isEmpty() && !out.contains("was not found in the database"))
  {
    int pos = out.indexOf("was installed by package");
    if (pos != -1)
    {
      int pos2 = out.indexOf("/", pos+25);
      pkgName = out.right(out.size()-(pos2+1));
      pkgName.remove('\n');
    }
  }

  return pkgName;
}

/*
 * Based on the given file, we use 'slocate' to suggest complete paths
 */
QStringList UnixCommand::getFilePathSuggestions(const QString &file)
{
  QProcess locate;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  locate.setProcessEnvironment(env);
  
  QStringList params;
  params << QStringLiteral("-l10");
  params << file;
  
  locate.start("locate", params);
  locate.waitForFinished();

  QString ba = locate.readAllStandardOutput();

  QStringList res;
  if (!ba.isEmpty())
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  res=ba.split("\n", QString::SkipEmptyParts);
  if (res.contains("locate: [show only 10 lines]")) res.removeLast();
#else
   res=ba.split("\n", Qt::SkipEmptyParts);
   if (res.contains("locate: [show only 10 lines]")) res.removeLast();
#endif
  }

  return res;
}

/*
 * Retrives the list of package groups
 */
/*QByteArray UnixCommand::getPackageGroups()
{
  QByteArray res = performQuery(QStringList("-Sg"));
  return res;
}*/

/*
 * Given a group name, returns a string containing all packages from it
 */
QByteArray UnixCommand::getPackagesFromGroup(const QString &groupName)
{
  Q_UNUSED(groupName)
  return QByteArray();

  /*QByteArray res =
      performQuery(QString("--print-format \"%r %n\" -Spg " ) + groupName);

  return res;*/
}

/*
 * Retrieves the list of installed packages in a special format for TargetList
 */
QByteArray UnixCommand::getInstalledPackages()
{
  QString args = "query '%n-%v %n#%v";
  QByteArray res = performQuery(args);
  return res;
}

/*
 * Retrieves the list of targets needed to update the entire system or a given package
 */
QByteArray UnixCommand::getTargetUpgradeList(const QString &pkgName)
{
  QString args;
  QByteArray res = "";

  if(!pkgName.isEmpty())
  {
    args = "install -n -f " + pkgName;
    res = performQuery(args);
  }
  else //pkg upgrade
  {
    args = "upgrade -n ";
    res = performQuery(args);
  }

  return res;
}

/*
 * Given a package name, retrieves the list of all targets needed for its removal
 */
QByteArray UnixCommand::getTargetRemovalList(const QString &pkgName)
{
  QString args;
  QByteArray res = "";

  if(!pkgName.isEmpty())
  {
    args = "remove -R -n " + pkgName;
    res = performQuery(args);
  }

  return res;
}

/*
 * Retrieves the system arch
 */
/*QString UnixCommand::getSystemArchitecture()
{
  QStringList slParam;
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);

  slParam << "-m";
  proc.start("uname", slParam);
  proc.waitForFinished();

  QString out = proc.readAllStandardOutput();
  proc.close();

  return out;
}*/

/*
 * Checks if we have internet access!
 */
bool UnixCommand::hasInternetConnection()
{
  QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
  bool result = false;

  for (int i = 0; i < ifaces.count(); i++){
    QNetworkInterface iface = ifaces.at(i);

    if ( iface.flags().testFlag(QNetworkInterface::IsUp)
         && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) ){
      for (int j=0; j<iface.addressEntries().count(); j++){
        /*
         We have an interface that is up, and has an ip address
         therefore the link is present.

         We will only enable this check on first positive,
         all later results are incorrect
        */
        if (result == false)
          result = true;
      }
    }
  }

  //It seems to be alright, but let's make a ping to see the result
  /*if (result == true)
  {
    result = UnixCommand::doInternetPingTest();
  }*/

  return result;
}

/*
 * Pings google site, to make sure internet is OK
 */
bool UnixCommand::doInternetPingTest()
{
  QTcpSocket socket;
  QString hostname = QStringLiteral("www.google.com");

  socket.connectToHost(hostname, 80);
  if (socket.waitForConnected(5000))
    return true;
  else
  {
    hostname = QStringLiteral("www.baidu.com");
    socket.connectToHost(hostname, 80);
    if (socket.waitForConnected(5000))
      return true;
    else
      return false;
  }
}

/*
 * Checks if the given executable is available somewhere in the system
 */
bool UnixCommand::hasTheExecutable( const QString& exeName )
{
  //std::cout << "Searching for the executable: " << exeName.toLatin1().data() << std::endl;

  QProcess proc;
  proc.setProcessChannelMode(QProcess::MergedChannels);
  QString sParam = QStringLiteral("which ") + exeName;
  QStringList params;
  params << QStringLiteral("-c");
  params << sParam;
  
  proc.start("/bin/sh", params);
  proc.waitForFinished();
  QString out = proc.readAllStandardOutput();
  proc.close();

  if (out.isEmpty() || out.count("which") > 0) return false;
  else return true;
}

/*
 * Does some garbage collection, removing uneeded files
 */
void UnixCommand::removeTemporaryFiles()
{
  QDir tempDir(QDir::tempPath());
  QStringList nameFilters;
  nameFilters << "qtsingleapp*" << "gpg*" << ".qt_temp_*";
  QFileInfoList list = tempDir.entryInfoList(nameFilters, QDir::Dirs | QDir::Files | QDir::System | QDir::Hidden);

  for(QFileInfo file: list){
    QFile fileAux(file.filePath());

    if (!file.isDir()){
      fileAux.remove();
    }
    else{
      QDir dir(file.filePath());
      QFileInfoList listd = dir.entryInfoList(QDir::Files | QDir::System);

      for(QFileInfo filed: listd){
        QFile fileAuxd(filed.filePath());
        fileAuxd.remove();
      }

      dir.rmdir(file.filePath());
    }
  }
}

/*
 * Stops the running command
 */
void UnixCommand::terminateCommand()
{
  m_process->terminate();
}

/*
 * Given a filename, checks if it is a text file
 */
bool UnixCommand::isTextFile(const QString& fileName)
{
  QProcess *p = new QProcess();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  p->setProcessEnvironment(env);

  QStringList s(fileName);
  p->start( "file", s );
  p->waitForFinished();

  QByteArray output = p->readAllStandardOutput();
  p->close();
  delete p;

  int from = output.indexOf(":", 0)+1;

  return (((output.indexOf( "ASCII", from ) != -1) ||
          (output.indexOf( "text", from ) != -1) ||
          (output.indexOf( "empty", from ) != -1)) &&
          (output.indexOf( "LSB executable", from) == -1) &&
          (output.indexOf( "LSB relocatable", from) == -1)
          );
}

/*
 * Given a filename, checks if it is an ELF relocatable file
 */
bool UnixCommand::isLSBRelated(const QString& fileName)
{
  QProcess *p = new QProcess();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  p->setProcessEnvironment(env);

  QStringList s(fileName);
  p->start( "file", s );
  p->waitForFinished();

  QByteArray output = p->readAllStandardOutput();
  p->close();
  delete p;
  int from = output.indexOf(":", 0)+1;

  return (output.indexOf( "LSB relocatable", from) != -1 ||
          output.indexOf( "LSB shared object", from) != -1);
}

/*
 * Retrieves pkgNG version.
 */
QString UnixCommand::getPkgNGVersion()
{
  QString v = performQuery("--version");
  return v;
}

/*
 * Executes given commandToRun inside a terminal, so the user can interact
 */
void UnixCommand::runCommandInTerminal(const QStringList& commandList){
  Q_UNUSED(commandList)
  //m_terminal->runCommandInTerminal(commandList);
}

/*
 * Executes the given command using QProcess async technology with ROOT credentials
 */
void UnixCommand::executeCommand(const QString &pCommand, Language lang)
{
  QString command;

  if (lang == ectn_LANG_USER_DEFINED)
  {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("LANG");
    env.remove("LC_MESSAGES");
    env.insert("LANG", QLocale::system().name() + ".UTF-8");
    env.insert("LC_MESSAGES", QLocale::system().name() + ".UTF-8");
    m_process->setProcessEnvironment(env);
  }

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList params = pCommand.split(QStringLiteral(" "), QString::SkipEmptyParts);
#else
  QStringList params = pCommand.split(QStringLiteral(" "), Qt::SkipEmptyParts);
#endif

  params.insert(0, ctn_OCTOPKG_DOAS_PARAMS);
  m_process->start(WMHelper::getSUCommand(), params);
}

/*
 * Executes the given command using QProcess async technology with ROOT credentials
 */
void UnixCommand::executeCommand(QStringList &params)
{
  QString command;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("LANG");
  env.remove("LC_MESSAGES");
  env.insert("LANG", QLocale::system().name() + ".UTF-8");
  env.insert("LC_MESSAGES", QLocale::system().name() + ".UTF-8");
  m_process->setProcessEnvironment(env);

  params.insert(0, ctn_OCTOPKG_DOAS_PARAMS);
  m_process->start(WMHelper::getSUCommand(), params);
}

/*
 * Puts all Standard output of the member process into a member string
 */
void UnixCommand::processReadyReadStandardOutput()
{
  if (m_process->isOpen())
    m_readAllStandardOutput = m_process->readAllStandardOutput();
}

/*
 * Puts all StandardError output of the member process into a member string
 */
void UnixCommand::processReadyReadStandardError()
{
  if (m_process->isOpen())
  {
    m_readAllStandardError = m_process->readAllStandardError();
    m_errorString = m_process->errorString();
  }
}

/*
 * Retrieves Standard output of member process
 */
QString UnixCommand::readAllStandardOutput()
{
  return m_readAllStandardOutput;
}

/*
 * Retrieves StandardError output of member process
 */
QString UnixCommand::readAllStandardError()
{
  return m_readAllStandardError;
}

/*
 * Retrieves ErrorString of member process
 */
QString UnixCommand::errorString()
{
  return m_errorString;
}

/*
 * UnixCommand's constructor: the relevant environment english setting and the connectors
 */
UnixCommand::UnixCommand(QObject *parent): QObject()
{
  m_process = new QProcess(parent);
  //m_terminal = new Terminal(parent, SettingsManager::getTerminal());

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  m_process->setProcessEnvironment(env);

  QObject::connect(m_process, &QProcess::started, this,
                   &UnixCommand::started);
  QObject::connect(this, &UnixCommand::started, this,
                   &UnixCommand::processReadyReadStandardOutput);

  QObject::connect(m_process, &QProcess::readyReadStandardOutput, this,
                   &UnixCommand::readyReadStandardOutput);
  QObject::connect(this, &UnixCommand::readyReadStandardOutput, this,
                   &UnixCommand::processReadyReadStandardOutput);

  QObject::connect(m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,
                   &UnixCommand::finished);
  QObject::connect(this, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished), this,
                   &UnixCommand::processReadyReadStandardOutput);

  QObject::connect(m_process, &QProcess::readyReadStandardError, this,
                   &UnixCommand::readyReadStandardError);
  QObject::connect(this, &UnixCommand::readyReadStandardError, this,
                   &UnixCommand::processReadyReadStandardError);

  //Terminal signals
  /*QObject::connect(m_terminal, SIGNAL( started()), this,
                   SIGNAL( started()));
  QObject::connect(m_terminal, SIGNAL( finished ( int, QProcess::ExitStatus )), this,
                   SIGNAL( finished ( int, QProcess::ExitStatus )) );

  QObject::connect(m_terminal, SIGNAL( startedTerminal()), this,
                   SIGNAL( startedTerminal()));
  QObject::connect(m_terminal, SIGNAL( finishedTerminal(int,QProcess::ExitStatus)), this,
                   SIGNAL( finishedTerminal(int,QProcess::ExitStatus)));*/
}

/*
 * If justOneInstance = false (default), returns TRUE if one instance of the app is ALREADY running
 * Otherwise, it returns TRUE if the given app is running.
 */
bool UnixCommand::isAppRunning(const QString &appName, bool justOneInstance)
{
  QStringList slParam;
  QProcess proc;

  slParam << "-C";
  slParam << appName;
  proc.start("ps", slParam);
  proc.waitForFinished();
  QString out = proc.readAll();
  proc.close();

  if (justOneInstance)
  {
    if (out.count(appName)>0)
      return true;
    else
      return false;
  }
  else
  {
    if (out.count(appName)>1)
      return true;
    else
      return false;
  }
}

/*
 * Returns the SHELL environment variable, if not set defaults to bash.
 */
QString UnixCommand::getShell()
{
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  QString shell = env.value(QStringLiteral("SHELL"), QStringLiteral("/bin/sh"));

  QFileInfo fi(shell);

  if (fi.fileName() == QLatin1String("fish"))
    return QStringLiteral("/bin/sh");
  else
    return fi.fileName();
}

/*
 * Checks if "/" partition is mount using ZFS filesystem
 */
bool UnixCommand::isRootOnZFS()
{
  bool tools = QFile::exists(QLatin1String("/sbin/bectl")) && QFile::exists(QLatin1String("/sbin/zfs"));
  if (!tools) return false;

  QProcess proc;
  QStringList sl;
  sl << QStringLiteral("list");
  sl << QStringLiteral("-S");
  sl << QStringLiteral("mountpoint");
  sl << QStringLiteral("/");

  proc.start(QStringLiteral("zfs"), sl);
  proc.waitForFinished();
  int exitCode=proc.exitCode();
  proc.close();

  return (exitCode == 0);
}

/*
 * Retrieves the BSDFlavour where OctoPkg is running on!
 * Reads file "/etc/os-release" and searchs for compatible OctoPkg BSDs
 */
BSDFlavour UnixCommand::getBSDFlavour()
{
  static BSDFlavour ret;
  static bool firstTime = true;

  if (firstTime)
  {
    if (QFile::exists("/etc/rc.conf.ghostbsd"))
    {
      ret = ectn_GHOSTBSD;
    }
    else
    {
      QProcess p;
      p.start(QStringLiteral("uname"), QStringList() << QStringLiteral("-a"));
      p.waitForFinished();
      QString out = p.readAllStandardOutput();

      if (out.contains("FreeBSD HBSD"))
      {
        ret = ectn_HARDENEDBSD;
      }
      else if (out.contains("FreeBSD"))
      {
        ret = ectn_FREEBSD;
      }
      else if (out.contains("DragonFly"))
      {
        ret = ectn_DRAGONFLYBSD;
      }
      else
      {
        ret = ectn_UNKNOWN;
      }
    }

    firstTime = false;
  }

  return ret;
}
