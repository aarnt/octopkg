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

#include "package.h"
#include "unixcommand.h"
#include "stdlib.h"
#include "strconstants.h"
#include "constants.h"
#include <iostream>

#include <QTextStream>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>

/*
 * This class abstracts all the relevant package information and services
 */

/*
 * Retrieves the basic package name, without version numbers
 */
QString Package::getBaseName( const QString& p )
{
	QString packageBaseName="";
	QString aux(p);
	int numberOfSegments = p.count('-')+1;
	int nameSegment = numberOfSegments-3;
	int a;

	for (int i=1; i<= nameSegment; i++){
		a=aux.indexOf("-");
		packageBaseName += aux.left(a);
		if (i<nameSegment) packageBaseName += "-";
		aux = aux.mid(a+1);
	}

	if (packageBaseName == "") packageBaseName += p.left(p.indexOf("-"));
	return packageBaseName;
}

/*
 * Given a QString, this method searches for a link pattern and inserts an URL html/ftp link tag
 * Returns the modified (or not) QString
 */
QString Package::makeURLClickable( const QString &s )
{
  if (s.trimmed().isEmpty() || s == "UNKNOWN" || s == "unknown") return s;

  QString sb = s;
  QRegExp rx("((ht|f)tp(s?))://(\\S)+[^\"|)|(|.|\\s|\\n]");

  rx.setCaseSensitivity( Qt::CaseInsensitive );
  int search = 0;
  int ini = 0;

  //First we search for the 1st pattern: rx
  while ( (ini = rx.indexIn( sb, search )) != -1 ){
    QString s1 = rx.cap();
    QString ns;

    ns = "<a href=\"" + s1 + "\">" + s1 + "</a>";
    sb.replace( ini, s1.length(), ns);
    search = ini + (2*s1.length()) + 15;
  }

  return sb;
}

/*
 * This function was copied from ArchLinux Pacman project
 * A pow() implementation that is specialized for an integer base and small,
 * positive-only integer exponents.
 */
double Package::simplePow(int base, int exp)
{
  double result = 1.0;
  for(; exp > 0; exp--) {
    result *= base;
  }

  return result;
}

/*
 * Converts a size in kbytes to a readable QString representation
 */
QString Package::kbytesToSize( float Bytes )
{
  float tb = 1073741824;
  float gb = 1048576;
  float mb = 1024;
  float kb = 1;
  QString res;

  if( Bytes >= tb )
    res = res.asprintf("%.2f TiB", (float)Bytes/tb);
  else if( Bytes >= gb && Bytes < tb )
    res = res.asprintf("%.2f GiB", (float)Bytes/gb);
  else if( Bytes >= mb && Bytes < gb )
    res = res.asprintf("%.2f MiB", (float)Bytes/mb);
  else if( Bytes >= kb && Bytes < mb )
    res = res.asprintf("%.2f KiB", (float)Bytes/kb);
  else if ( Bytes < kb)
    res = res.asprintf("%.2f Bytes", Bytes * 1024);
  else
    res = res.asprintf("%.2f Bytes", Bytes);

  return res;
}

/*
 * Converts the size in String type to double
 */
double Package::strToKBytes(QString size)
{
  double res=0;
  if (size == "0.00B") res = 0;
  else if (size.contains("kB", Qt::CaseInsensitive))
  {
    bool ok;
    int p = size.indexOf("kB", Qt::CaseInsensitive);
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = value / 1.024;
    }
  }
  else if (size.contains("MB"))
  {
    bool ok;
    int p = size.indexOf("MB");
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = (value * 1024) / 1.048576;
    }
  }
  else if (size.contains("B"))
  {
    bool ok;
    int p = size.indexOf("B");
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = value;
    }
  }

  return res;
}

/*
 * Converts the size in String type to double
 */
double Package::strToKBytes2(QString size)
{
  double res=0;
  if (size == "0.00B") res = 0;
  else if (size.contains("KiB", Qt::CaseInsensitive))
  {
    bool ok;
    int p = size.indexOf("KiB", Qt::CaseInsensitive);
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = value;
    }
  }
  else if (size.contains("MiB"))
  {
    bool ok;
    int p = size.indexOf("MiB");
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = value * 1024;
    }
  }
  else if (size.contains("B"))
  {
    bool ok;
    int p = size.indexOf("B");
    double value = size.left(p).toDouble(&ok);
    if (ok)
    {
      res = value / 1024;
    }
  }

  return res;
}

/*
 * Retrieves the list of locked packages
 */
QSet<QString> *Package::getLockedPackageList()
{
  QString lockedPkgList = UnixCommand::getLockedPackageList();
  
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuples = lockedPkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuples = lockedPkgList.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif
  
  packageTuples.removeFirst(); //Removes string -> "Currently locked packages:"
  QSet<QString>* res = new QSet<QString>();

  for(QString packageTuple: packageTuples)
  {
    int lastsep = packageTuple.lastIndexOf("-");
    res->insert(packageTuple.left(lastsep));
  }

  return res;
}

/*
 * Retrieves the list of unrequired packages (those no other packages depends on)
 */
QSet<QString>* Package::getUnrequiredPackageList()
{
  QString unrequiredPkgList = UnixCommand::getUnrequiredPackageList();

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuples = unrequiredPkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuples = unrequiredPkgList.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif
  
  QSet<QString>* res = new QSet<QString>();

  for(QString packageTuple: packageTuples)
  {
    res->insert(packageTuple);
    //qDebug() << packageTuple;
  }

  return res;
}

/*
 * Retrieves the list of outdated packages (those which have newer versions available to download)
 */
QMap<QString, OutdatedPackageInfo> *Package::getOutdatedStringList()
{
  QString outPkgList = UnixCommand::getOutdatedPackageList();

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuples = outPkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuples = outPkgList.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif
  
  QMap<QString, OutdatedPackageInfo>* res = new QMap<QString, OutdatedPackageInfo>();
  bool pkgsToUpgrade = false;

  if (packageTuples.contains("Installed packages to be UPGRADED:", Qt::CaseInsensitive))
  {
    for(QString packageTuple: packageTuples)
    {
      if (packageTuple.contains("packages to be") &&
          !packageTuple.contains("Installed packages to be UPGRADED:"))
      {
        pkgsToUpgrade = false;
        continue;
      }
      else if (packageTuple.contains("packages to be") &&
          packageTuple.contains("Installed packages to be UPGRADED:"))
      {
        pkgsToUpgrade = true;
      }

      if (pkgsToUpgrade && packageTuple.contains("\t"))
      {
        packageTuple.remove("\t");

        QStringList parts = packageTuple.split(' ');
        {
          OutdatedPackageInfo opi;
          QString pkgName;

          pkgName = parts[0];
          pkgName.remove(pkgName.size()-1, 1);

          opi.oldVersion = parts[1];
          opi.newVersion = parts[3];

          res->insert(pkgName, opi);
        }
      }
    }
  }

  return res;
}

/*
 * Retrieves the list of all package groups available
 */
/*QStringList *Package::getPackageGroups()
{
  QString packagesFromGroup = UnixCommand::getPackageGroups();
  QStringList packageTuples = packagesFromGroup.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
  QStringList * res = new QStringList();

  for(QString packageTuple, packageTuples)
  {
    packageTuple = packageTuple.trimmed();
    if (!packageTuple.isEmpty())
    {
      res->append(packageTuple); //We only need the package name!
    }
  }

  res->removeDuplicates();
  res->sort();
  return res;
}*/

/*
 * Retrieves the list of packages from a given group name
 */
QStringList *Package::getPackagesOfGroup(const QString &groupName)
{
  Q_UNUSED(groupName)
  return nullptr;
  
  /*QString packagesFromGroup = UnixCommand::getPackagesFromGroup(groupName);
  QStringList packageTuples = packagesFromGroup.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
  QStringList * res = new QStringList();

  for(QString packageTuple, packageTuples)
  {
    QStringList parts = packageTuple.split(' ');
    res->append(parts[1]); //We only need the package name!
  }

  return res;*/
}

/*
 * Retrieves the list of targets needed to upgrade the entire system OR
 * to install/reinstall/upgrade a given package
 */
TransactionInfo Package::getTargetUpgradeList(const QString &pkgName)
{
  QString targets = UnixCommand::getTargetUpgradeList(pkgName);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList infoTuples = targets.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList infoTuples = targets.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif
  
  TransactionInfo res;
  res.packages = new QStringList();

  for(QString infoTuple: infoTuples)
  {
    int pos = infoTuple.indexOf("to be downloaded");
    int tab = infoTuple.indexOf("\t");
    if (pos != -1)
    {
      res.sizeToDownload = infoTuple.left(pos).trimmed();
    }
    if (tab != -1) //We are dealing with packages HERE!
    {
      res.packages->append(infoTuple.remove(QRegularExpression("\t")).trimmed());
    }
  }

  res.packages->sort();
  return res;
}

/*
 * Retrieves the list of targets needed to be removed with the given package
 */
QStringList *Package::getTargetRemovalList(const QString &pkgName)
{
  QString targets = UnixCommand::getTargetRemovalList(pkgName);
  
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList infoTuples = targets.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList infoTuples = targets.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif
  
  QStringList *res = new QStringList();

  for(QString infoTuple: infoTuples)
  {
    int tab = infoTuple.indexOf("\t");
    if (tab != -1) //We are dealing with packages HERE!
    {
      int colon = infoTuple.indexOf(":");
      if (colon != -1)
      {
        res->append(infoTuple.mid(0, colon).trimmed());
      }
      else
      {
        res->append(infoTuple.remove(QRegularExpression("\t")).trimmed());
      }
    }
  }

  res->sort();
  return res;
}

/*
 *Retrieves the list of foreign packages (those installed from unknown repositories like AUR)
 */
/*QList<PackageListData> *Package::getForeignPackageList()
{
  QString foreignPkgList = UnixCommand::getForeignPackageList();
  QStringList packageTuples = foreignPkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
  QList<PackageListData> * res = new QList<PackageListData>();

  for(QString packageTuple, packageTuples)
  {
    QStringList parts = packageTuple.split(' ');
    if (parts.size() == 2)
    {
      res->append(PackageListData(parts[0], "", parts[1], ectn_FOREIGN));
    }
  }

  return res;
}*/

/*
 * Retrieves the list of all available packages in the database (installed + non-installed)
 */
QList<PackageListData> * Package::getPackageList(const QString &packageName)
{
  QString pkgName, pkgOrigin, pkgVersion, pkgComment, pkgDescription, pkgInstallDate;
  double pkgInstalledSize, pkgDownloadedSize;
  PackageStatus pkgStatus;
  QString pkgList = UnixCommand::getPackageList(packageName);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuples = pkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuples = pkgList.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif

  QList<PackageListData> * res = new QList<PackageListData>();

  if(!pkgList.isEmpty())
  {
    pkgDescription = "";

    for(QString packageTuple: packageTuples)
    {
      pkgComment = "";
      QStringList parts = packageTuple.split(' ');
      pkgName = parts[0];
      pkgVersion = parts[1];

      int bar = parts[2].indexOf("/");
      pkgOrigin = parts[2].left(bar);
      pkgStatus = ectn_INSTALLED;
      pkgDownloadedSize = 0;
      pkgInstalledSize = strToKBytes(parts[3]);

      //If we are using pkgng >= 1.6
      if (pkgInstalledSize == 0)
        pkgInstalledSize = strToKBytes2(parts[3]);

      pkgInstallDate = parts[4];
      if (pkgInstallDate.isEmpty()) pkgInstallDate = QStringLiteral("0");

      for(int c=5; c<parts.count(); c++)
      {
        pkgComment += " " + parts[c];
      }

      if (!pkgComment.isEmpty()) pkgComment = pkgName + " " + pkgComment;
      pkgComment = pkgComment.trimmed();
      pkgDescription = pkgComment;

      PackageListData pld =
          PackageListData(pkgName, pkgOrigin, pkgVersion, pkgComment, pkgStatus, pkgInstalledSize, pkgDownloadedSize, pkgInstallDate);

      res->append(pld);
    }
  }

  return res;
}

/*
 * Parses the list of packages obtained by the "pkg" tool
 */
QList<PackageListData> * Package::parsePackageTuple(const QStringList &packageTuples, QStringList &packageCache)
{
  QString pkgName, pkgVersion, pkgCategories, pkgWWW, pkgComment;
  double pkgPkgSize;
  int indName, indVersion, indCategories, indWWW, indComment, indPkgSize;
  const int cSpaces = 16;
  QList<PackageListData> * res = new QList<PackageListData>();

  for(QString packageTuple: packageTuples)
  {
    if (packageTuple.at(0).isLower())
    {
      //TODO We have to search for the pkg to discover its status
      PackageListData pld;

      if (!pkgName.isEmpty())
      {
        pld.name = pkgName;

        if (packageCache.contains(pkgName)) continue;

        pld.version = pkgVersion;
        pld.categories = pkgCategories;
        pld.www = pkgWWW;
        pld.comment = pkgName + " " + pkgComment;
        pld.downloadSize = pkgPkgSize;
        pld.status = ectn_NON_INSTALLED;
        pld.repository = ctn_PKGNG_FAKE_REPOSITORY;

        packageCache.append(pkgName);

        pkgName="";
        pkgVersion="";
        pkgCategories="";
        pkgWWW="";
        pkgComment="";
        pkgPkgSize=0;

        res->append(pld);
      }

      continue;
    }

    indName = packageTuple.indexOf("Name           :");
    indVersion = packageTuple.indexOf("Version        :");
    indCategories = packageTuple.indexOf("Categories     :");
    indWWW = packageTuple.indexOf("WWW            :");
    indComment = packageTuple.indexOf("Comment        :");
    indPkgSize = packageTuple.indexOf("Pkg size       :");

    if (indName > -1)
    {
      indName += cSpaces;
      pkgName = packageTuple.right(packageTuple.size()-(indName+1));
    }
    else if (indVersion > -1)
    {
      indVersion += cSpaces;
      pkgVersion = packageTuple.right(packageTuple.size()-(indVersion+1));
    }
    else if (indCategories > -1)
    {
      indCategories += cSpaces;
      pkgCategories = packageTuple.right(packageTuple.size()-(indCategories+1));
    }
    else if (indWWW > -1)
    {
      indWWW += cSpaces;
      pkgWWW = packageTuple.right(packageTuple.size()-(indWWW+1));
    }
    else if (indComment > -1)
    {
      indComment += cSpaces;
      pkgComment = packageTuple.right(packageTuple.size()-(indComment+1));
    }
    else if (indPkgSize > -1)
    {
      indPkgSize += cSpaces;
      pkgPkgSize = strToKBytes2(packageTuple.right(packageTuple.size()-(indPkgSize+1)));
    }
  }

  //Adds the last one...
  if (packageTuples.count() > 0 && (!packageCache.contains(pkgName)))
  {
    PackageListData pld;
    pld.name = pkgName;
    pld.version = pkgVersion;
    pld.categories = pkgCategories;
    pld.www = pkgWWW;
    pld.comment = pkgName + " " + pkgComment;
    pld.downloadSize = pkgPkgSize;
    pld.status = ectn_NON_INSTALLED;
    pld.repository = ctn_PKGNG_FAKE_REPOSITORY;

    packageCache.append(pkgName);

    pkgName="";
    pkgVersion="";
    pkgCategories="";
    pkgWWW="";
    pkgComment="";
    pkgPkgSize=0;

    res->append(pld);
  }

  return res;
}

/*
 * Retrieves the list of all AUR packages in the database (installed + non-installed)
 * given the search parameter
 */
QList<PackageListData> * Package::getRemotePackageList(const QString& searchString)
{
  QList<PackageListData> * res = new QList<PackageListData>();
  QList<PackageListData> * resName = new QList<PackageListData>();
  QList<PackageListData> * resComment = new QList<PackageListData>();
  QStringList packageCache;

  if (searchString.isEmpty())
    return res;

  QString auxSearchString=searchString;
  auxSearchString.replace(QLatin1String("+"), QLatin1String("\\+"));
  auxSearchString.remove(QLatin1Char('^'));
  auxSearchString.remove(QLatin1Char('$'));
  QString pkgList = UnixCommand::getRemotePackageList(auxSearchString, false);
  
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuples = pkgList.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuples = pkgList.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif

  QRegularExpression re(searchString);
  QRegularExpressionMatch match;
  resName = parsePackageTuple(packageTuples, packageCache);

  for(PackageListData pld: *resName)
  {
    if (searchString.at(0) == QLatin1Char('^') || searchString.at(searchString.length()-1) == QLatin1Char('$'))
    {
      match = re.match(pld.name);
      if (match.hasMatch()) res->append(pld);
    }
    else
      res->append(pld);
  }

  QString pkgListComment = UnixCommand::getRemotePackageList(auxSearchString, true);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList packageTuplesComment = pkgListComment.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList packageTuplesComment = pkgListComment.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif

  resComment = parsePackageTuple(packageTuplesComment, packageCache);

  for(PackageListData pld: *resComment)
  {
    if (searchString.at(0) == QLatin1Char('^') || searchString.at(searchString.length()-1) == QLatin1Char('$'))
    {
      match = re.match(pld.name);
      if (match.hasMatch()) res->append(pld);
    }
    else
      res->append(pld);
  }

  return res;
}

/*
 * Given a QString containing the output of pacman -Si/Qi (pkgInfo),
 * this method returns the contents of the given field (ex: description)
 */
QString Package::extractFieldFromInfo(const QString &field, const QString &pkgInfo)
{
  int fieldPos = pkgInfo.indexOf(field);
  int fieldEnd, fieldEnd2;
  QString aux;

  if (fieldPos > 0)
  {
    if(field == "Options")
    {
      fieldPos = pkgInfo.indexOf(":", fieldPos+1);
      fieldPos+=2;
      aux = pkgInfo.mid(fieldPos);

      fieldEnd = aux.indexOf("Shared Libs required");
      fieldEnd2 = aux.indexOf("Shared Libs provided");
      if (fieldEnd2 == -1)
        fieldEnd2 = aux.indexOf("Annotations");

      if((fieldEnd > fieldEnd2 && fieldEnd2 != -1) ||
         (fieldEnd == -1 && fieldEnd2 != -1)) fieldEnd = fieldEnd2;

      aux = aux.left(fieldEnd).trimmed();
      aux = aux.replace("\n", "<br>");
    }
    else
    {
      fieldPos = pkgInfo.indexOf(":", fieldPos+1);
      fieldPos+=2;
      aux = pkgInfo.mid(fieldPos);
      fieldEnd = aux.indexOf('\n');
      aux = aux.left(fieldEnd).trimmed();
    }
  }

  return aux;
}

/*
 * Retrieves "Name" field of the given package information string represented by pkgInfo
 */
QString Package::getName(const QString& pkgInfo)
{
  return extractFieldFromInfo("Name", pkgInfo);
}

/*
 * Retrieves "Version" field of the given package information string represented by pkgInfo
 */
QString Package::getVersion(const QString &pkgInfo)
{
  return extractFieldFromInfo("Version", pkgInfo);
}

/*
 * Retrieves "Repository" field of the given package information string represented by pkgInfo
 */
QString Package::getRepository(const QString &pkgInfo)
{
  return extractFieldFromInfo("Repository", pkgInfo);
}

/*
 * Retrieves "URL" field of the given package information string represented by pkgInfo
 */
QString Package::getURL(const QString &pkgInfo)
{
  QString URL = extractFieldFromInfo("WWW", pkgInfo);
  if (!URL.isEmpty())
    return makeURLClickable(URL);
  else
    return URL;
}

/*
 * Retrieves "Licenses" field of the given package information string represented by pkgInfo
 */
QString Package::getLicense(const QString &pkgInfo)
{
  return extractFieldFromInfo("Licenses", pkgInfo);
}

/*
 * Retrieves "Groups" field of the given package information string represented by pkgInfo
 */
QString Package::getGroup(const QString &pkgInfo)
{
  return extractFieldFromInfo("Categories", pkgInfo);
}

/*
 * Retrieves "Provides" field of the given package information string represented by pkgInfo
 */
QString Package::getProvides(const QString &pkgInfo)
{
  return extractFieldFromInfo("Provides", pkgInfo);
}

/*
 * Retrieves "Depends On" field of the given package information string represented by pkgInfo
 */
QString Package::getDependsOn(const QString &pkgInfo)
{
  return extractFieldFromInfo("Depends On", pkgInfo);
}

/*
 * Retrieves "Optional Deps" field of the given package information string represented by pkgInfo
 */
QString Package::getOptDepends(const QString &pkgInfo)
{
  return extractFieldFromInfo("Optional Deps", pkgInfo);
}

/*
 * Retrieves "Conflicts With" field of the given package information string represented by pkgInfo
 */
QString Package::getConflictsWith(const QString &pkgInfo)
{
  return extractFieldFromInfo("Conflicts With", pkgInfo);
}

/*
 * Retrieves "Replaces" field of the given package information string represented by pkgInfo
 */
QString Package::getReplaces(const QString &pkgInfo)
{
  return extractFieldFromInfo("Replaces", pkgInfo);
}

/*
 * Retrieves "RequiredBy" field of the given package information string represented by pkgInfo
 */
QString Package::getRequiredBy(const QString &pkgInfo)
{
  return extractFieldFromInfo("Required By", pkgInfo);
}

/*
 * Retrieves "OptionalFor" field of the given package information string represented by pkgInfo
 */
QString Package::getOptionalFor(const QString &pkgInfo)
{
  return extractFieldFromInfo("Optional For", pkgInfo);
}

/*
 * Retrieves "Packager" field of the given package information string represented by pkgInfo
 */
QString Package::getPackager(const QString &pkgInfo)
{
  return extractFieldFromInfo("Packager", pkgInfo);
}

/*
 * Retrieves "Maintainer" field of the given package information string represented by pkgInfo
 */
QString Package::getMaintainer(const QString &pkgInfo)
{
  return extractFieldFromInfo("Maintainer", pkgInfo);
}

/*
 * Retrieves "Architecture" field of the given package information string represented by pkgInfo
 */
QString Package::getArch(const QString &pkgInfo)
{
  return extractFieldFromInfo("Architecture", pkgInfo);
}

/*
 * Retrieves "Build Date" field of the given package information string represented by pkgInfo
 */
QString Package::getInstalledOn(const QString &pkgInfo)
{
  return extractFieldFromInfo("Installed on", pkgInfo);
  //qDebug() << aux;
  //return QDateTime::fromString(aux); //"ddd MMM d hh:mm:ss yyyy");
}

/*
 * Retrieves "Download Size" field of the given package information string represented by pkgInfo
 */
double Package::getDownloadSize(const QString &pkgInfo)
{
  QString aux = extractFieldFromInfo("Download Size", pkgInfo);
  bool isMega = (aux.indexOf("MiB", Qt::CaseInsensitive) != -1);
  aux = aux.section(QRegularExpression("\\s"), 0, 0);

  bool ok;
  double res = aux.toDouble(&ok);

  if (ok)
  {
    if (isMega) res *= 1024;
    return res;
  }
  else
    return 0;
}

/*
 * Retrieves "Download Size" field of the given package information string represented by pkgInfo
 */
QString Package::getDownloadSizeAsString(const QString &pkgInfo)
{
  QString aux = extractFieldFromInfo("Download Size", pkgInfo);
  return aux;
}

/*
 * Retrieves "Installed Size" field of the given package information string represented by pkgInfo
 */
double Package::getInstalledSize(const QString &pkgInfo)
{
  QString aux = extractFieldFromInfo("Flat size", pkgInfo);
  bool isMega = (aux.indexOf("MiB", Qt::CaseInsensitive) != -1);
  aux = aux.section(QRegularExpression("\\s"), 0, 0);
  bool ok;
  double res = aux.toDouble(&ok);

  if (ok)
  {
    if (isMega) res *= 1024;
    return res;
  }
  else
    return 0;
}

/*
 * Retrieves "Installed Size" field of the given package information string represented by pkgInfo
 */
QString Package::getInstalledSizeAsString(const QString &pkgInfo)
{
  QString aux = extractFieldFromInfo("Flat size", pkgInfo);
  return aux;
}

/*
 * Retrieves "Options" field of the given package information string represented by pkgInfo
 */
QString Package::getOptions(const QString &pkgInfo)
{
  QString aux = extractFieldFromInfo("Options", pkgInfo);
  return aux;
}

/**
 * This function was copied from ArchLinux Pacman project
 *
 * Compare alpha and numeric segments of two versions.
 * return 1: a is newer than b
 *        0: a and b are the same version
 *       -1: b is newer than a
 */
int Package::rpmvercmp(const char *a, const char *b){
  char oldch1, oldch2;
  char *str1, *str2;
  char *ptr1, *ptr2;
  char *one, *two;
  int rc;
  int isnum;
  int ret = 0;

  /* easy comparison to see if versions are identical */
  if(strcmp(a, b) == 0) return 0;

  str1 = strdup(a);
  str2 = strdup(b);

  one = ptr1 = str1;
  two = ptr2 = str2;

  /* loop through each version segment of str1 and str2 and compare them */
  while (*one && *two) {
    while (*one && !isalnum((int)*one)) one++;
    while (*two && !isalnum((int)*two)) two++;

    /* If we ran to the end of either, we are finished with the loop */
    if (!(*one && *two)) break;

    /* If the separator lengths were different, we are also finished */
    if ((one - ptr1) != (two - ptr2)) {
      return (one - ptr1) < (two - ptr2) ? -1 : 1;
    }

    ptr1 = one;
    ptr2 = two;

    /* grab first completely alpha or completely numeric segment */
    /* leave one and two pointing to the start of the alpha or numeric */
    /* segment and walk ptr1 and ptr2 to end of segment */
    if (isdigit((int)*ptr1)) {
      while (*ptr1 && isdigit((int)*ptr1)) ptr1++;
      while (*ptr2 && isdigit((int)*ptr2)) ptr2++;
      isnum = 1;
    } else {
      while (*ptr1 && isalpha((int)*ptr1)) ptr1++;
      while (*ptr2 && isalpha((int)*ptr2)) ptr2++;
      isnum = 0;
    }

    /* save character at the end of the alpha or numeric segment */
    /* so that they can be restored after the comparison */
    oldch1 = *ptr1;
    *ptr1 = '\0';
    oldch2 = *ptr2;
    *ptr2 = '\0';

    /* this cannot happen, as we previously tested to make sure that */
    /* the first string has a non-null segment */
    if (one == ptr1) {
      ret = -1;       /* arbitrary */
      goto cleanup;
    }

    /* take care of the case where the two version segments are */
    /* different types: one numeric, the other alpha (i.e. empty) */
    /* numeric segments are always newer than alpha segments */
    /* XXX See patch #60884 (and details) from bugzilla #50977. */
    if (two == ptr2) {
      ret = isnum ? 1 : -1;
      goto cleanup;
    }

    if (isnum) {
      /* this used to be done by converting the digit segments */
      /* to ints using atoi() - it's changed because long  */
      /* digit segments can overflow an int - this should fix that. */

      /* throw away any leading zeros - it's a number, right? */
      while (*one == '0') one++;
      while (*two == '0') two++;

      /* whichever number has more digits wins */
      if (strlen(one) > strlen(two)) {
        ret = 1;
        goto cleanup;
      }
      if (strlen(two) > strlen(one)) {
        ret = -1;
        goto cleanup;
      }
    }

    /* strcmp will return which one is greater - even if the two */
    /* segments are alpha or if they are numeric.  don't return  */
    /* if they are equal because there might be more segments to */
    /* compare */
    rc = strcmp(one, two);
    if (rc) {
      ret = rc < 1 ? -1 : 1;
      goto cleanup;
    }

    /* restore character that was replaced by null above */
    *ptr1 = oldch1;
    one = ptr1;
    *ptr2 = oldch2;
    two = ptr2;
  }

  /* this catches the case where all numeric and alpha segments have */
  /* compared identically but the segment separating characters were */
  /* different */
  if ((!*one) && (!*two)) {
    ret = 0;
    goto cleanup;
  }

  /* the final showdown. we never want a remaining alpha string to
         * beat an empty string. the logic is a bit weird, but:
         * - if one is empty and two is not an alpha, two is newer.
         * - if one is an alpha, two is newer.
         * - otherwise one is newer.
         * */
  if ( (!*one && !isalpha((int)*two))
       || isalpha((int)*one) ) {
    ret = -1;
  } else {
    ret = 1;
  }

cleanup:
  free(str1);
  free(str2);
  return ret;
}

/*
 * Retrieves "Description" field of the given package information string represented by pkgInfo
 */
QString Package::getDescription(const QString &pkgInfo)
{
  return extractFieldFromInfo("Description", pkgInfo);
}

/*
 * Retrieves "Comment" field of the given package information string represented by pkgInfo
 */
QString Package::getComment(const QString &pkgInfo)
{
  return extractFieldFromInfo("Comment", pkgInfo);
}

/*
 * Retrieves all information for a given package name
 */
PackageInfoData Package::getInformation(const QString &pkgName, bool foreignPackage)
{
  PackageInfoData res;
  QString pkgInfo = UnixCommand::getPackageInformation(pkgName, foreignPackage);

  res.name = pkgName;
  res.version = getVersion(pkgInfo);
  res.url = getURL(pkgInfo);
  res.license = getLicense(pkgInfo);
  res.group = getGroup(pkgInfo);
  res.maintainer = getMaintainer(pkgInfo);
  res.arch = getArch(pkgInfo);
  res.installedOn = getInstalledOn(pkgInfo);
  res.description = getDescription(pkgInfo);
  res.comment = getComment(pkgInfo);
  res.downloadSize = getDownloadSize(pkgInfo);
  res.installedSize = getInstalledSize(pkgInfo);  
  res.downloadSizeAsString = getDownloadSizeAsString(pkgInfo);
  res.installedSizeAsString = getInstalledSizeAsString(pkgInfo);
  res.options = getOptions(pkgInfo);

  return res;
}

/*
 * Helper to get only the Download Size field of package information
 */
double Package::getDownloadSizeDescription(const QString &pkgName)
{
  QString pkgInfo = UnixCommand::getPackageInformation(pkgName, false);
  return getDownloadSize(pkgInfo);
}

/*
 * Helper to get only the Description field of package information, for use in tooltips
 */
QString Package::getInformationDescription(const QString &pkgName, bool foreignPackage)
{
  QString pkgInfo = UnixCommand::getPackageInformation(pkgName, foreignPackage);
  return getDescription(pkgInfo);
}

/*
 * Helper to get only the Installed Size field of package information, for use in tooltips
 */
QString Package::getInformationInstalledSize(const QString &pkgName, bool foreignPackage)
{
  QString pkgInfo = UnixCommand::getPackageInformation(pkgName, foreignPackage);
  return kbytesToSize(getInstalledSize(pkgInfo));
}

/*
 * Retrieves the dependencies list of packages for the given pkgName
 */
QString Package::getDependencies(const QString &pkgName)
{
  QStringList pkgList;
  QString res;
  QString aux = UnixCommand::getDependenciesList(pkgName);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  pkgList = aux.split("\n", QString::SkipEmptyParts);
#else
  pkgList = aux.split("\n", Qt::SkipEmptyParts);
#endif

  pkgList.sort();

  for(QString dependency: pkgList)
  {
    if (dependency.at(0) == "\"")
    {
      dependency.remove(dependency.size()-1, 1);
      dependency.remove(0, 1);
    }

    res += "<a href=\"goto:" + dependency + "\">" + dependency + "</a> ";
  }

  return res.trimmed();
}

/*
 * Retrieves the file list content of the given package
 */
QStringList Package::getContents(const QString& pkgName, bool isInstalled)
{
  QByteArray result;

  if (isInstalled)
  {
    result = UnixCommand::getPackageContentsUsingPkg(pkgName);
  }

  QString aux(result);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList fileList = aux.split("\n", QString::SkipEmptyParts);
#else
  QStringList fileList = aux.split("\n", Qt::SkipEmptyParts);
#endif

  //Let's change that listing a bit...
  QStringList auxList;
  for(QString file: fileList)
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList parts = file.split("/", QString::SkipEmptyParts);
#else
    QStringList parts = file.split("/", Qt::SkipEmptyParts);
#endif

    navigateThroughDirs(parts, auxList, 0);
  }

  fileList = fileList + auxList;
  fileList.sort();

  return fileList;
}

/*
 * Retrieves the list of optional dependencies of the given package
 */
QStringList Package::getOptionalDeps(const QString &pkgName)
{
  QString pkgInfo = UnixCommand::getPackageInformation(pkgName, false);
  QString aux = Package::getOptDepends(pkgInfo);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList result = aux.split("<br>", QString::SkipEmptyParts);
#else
  QStringList result = aux.split("<br>", Qt::SkipEmptyParts);
#endif

  result.removeAll("None");

  return result;
}

/*
 * Navigate thru directories to build a hierarquic directory list
 */
void Package::navigateThroughDirs(QStringList parts, QStringList& auxList, int ind)
{
  static QString dir="";
  dir = dir + "/" + parts[ind];

  if (!auxList.contains(dir+"/"))
  {
    auxList.append(dir+"/");
  }

  if (ind < parts.count()-2)
  {
    ind++;
    navigateThroughDirs(parts, auxList, ind);
  }
  else
  {
    dir="";
    return;
  }
}

/*
 * Returns a modified RegExp-based string given the string entered by the user
 */
QString Package::parseSearchString(QString searchStr, bool exactMatch)
{
  if (searchStr.indexOf("*.") == 0){
    searchStr = searchStr.remove(0, 2);
    searchStr.insert(0, "\\S+\\.");
  }

  if (searchStr.indexOf("*") == 0){
    searchStr = searchStr.remove(0, 1);
    searchStr.insert(0, "\\S+");
  }

  if (searchStr.endsWith("*")){
    searchStr.remove(searchStr.length()-1, 1);
    searchStr.append("\\S*");
  }

  if (searchStr.indexOf("^") == -1 && searchStr.indexOf("\\S") != 0){
    if (!exactMatch) searchStr.insert(0, "\\S*");
    else searchStr.insert(0, "^");
  }

  if (searchStr.indexOf("$") == -1){
    if (!exactMatch && !searchStr.endsWith("\\S*")) searchStr.append("\\S*");
    else searchStr.append("$");
  }

  searchStr.replace(QLatin1String("+"), QLatin1String("\\+"));
  searchStr.replace("?", ".");
  return searchStr;
}

/*
 * Checks if this system has the core.db
 */
bool Package::hasPkgNGDatabase()
{
  static bool done = false;
  static bool answer = false;

  if (!done)
  {
    //If we're running inside a GhostBSD OS, let's check which repo file we get
    if (UnixCommand::getBSDFlavour() == ectn_GHOSTBSD)
    {
      QFile f(ctn_PKGNG_GHOSTBSD_CORE_DB_FILE);
      answer = f.exists();

      //Well, maybe it's an old GhostBSD release...
      if (!answer)
      {
        QFile f(ctn_PKGNG_FREEBSD_CORE_DB_FILE);
        answer = f.exists();
      }
    }
    else if (UnixCommand::getBSDFlavour() == ectn_DRAGONFLYBSD)
    {
      QFile f(ctn_PKGNG_DRAGONFLYBSD_CORE_DB_FILE);
      answer = f.exists();
    }
    else if (UnixCommand::getBSDFlavour() == ectn_HARDENEDBSD)
    {
      QFile f(ctn_PKGNG_HARDENEDBSD_CORE_DB_FILE);
      answer = f.exists();
    }
    else
    {
      QFile f(ctn_PKGNG_FREEBSD_CORE_DB_FILE);
      answer = f.exists();
    }

    done = true;
  }

  return answer;
}

/*
 * Returns true if the package is included in the forbidden removal list
 */
bool Package::isForbidden(const QString pkgName)
{
  QStringList forbiddenPkgs = { "pkg", "xorg" };
  return forbiddenPkgs.contains(pkgName);
}
