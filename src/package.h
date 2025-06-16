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

#ifndef PACKAGE_H
#define PACKAGE_H

#include "settingsmanager.h"

#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QFileSystemWatcher>
#include <QDateTime>
#include <QMap>
#include <QHash>
#include <QSet>

const QString ctn_TEMP_ACTIONS_FILE ( QDir::homePath() + QDir::separator() + ".config/octopkg" + QDir::separator() + ".qt_temp_" );
const QString ctn_PACMAN_DATABASE_DIR = "/var/lib/pacman";
const QString ctn_PKGNG_DRAGONFLYBSD_CORE_DB_FILE = "/var/db/pkg/repo-Avalon.sqlite";
const QString ctn_PKGNG_FREEBSD_CORE_DB_FILE = "/var/db/pkg/repo-FreeBSD.sqlite";
const QString ctn_PKGNG_FREEBSD_DEFAULT_CORE_DB_FILE = "/var/db/pkg/local.sqlite";
const QString ctn_PKGNG_HARDENEDBSD_CORE_DB_FILE = "/var/db/pkg/repo-HardenedBSD.sqlite";
const QString ctn_PKGNG_GHOSTBSD_CORE_DB_FILE = "/var/db/pkg/repo-GhostBSD.sqlite";
const QString ctn_PKGNG_PCBSD_CORE_DB_FILE = "/var/db/pkg/repo-pcbsd-major.sqlite";
const QString ctn_PKGNG_NOMADBSD_CORE_DB_FILE = "/var/db/pkg/local.sqlite";
const QString ctn_PKGNG_FAKE_REPOSITORY = "_WWW";

enum PackageStatus { ectn_LOCKED, ectn_INSTALLED, ectn_NON_INSTALLED, ectn_OUTDATED, ectn_NEWER };

enum ViewOptions { ectn_ALL_PKGS, ectn_INSTALLED_PKGS, ectn_NON_INSTALLED_PKGS };

struct PackageListData{
  QString name;
  QString repository;
  QString origin;
  QString version;
  QString categories;
  QString www;
  QString comment;
  QString description;
  QString outatedVersion;
  double  installedSize;
  double  downloadSize;
  QString installedOn;
  QString license;
  int     popularity; //votes
  PackageStatus status;

  PackageListData() : name(""),
                    downloadSize(0.0),
                    popularity(0),
                    status(ectn_NON_INSTALLED){
  }

  PackageListData(QString n, QString v)
    : name(n),
      version(v),
      downloadSize(0.0),
      popularity(0),
      status(ectn_NON_INSTALLED){
  }

  PackageListData(QString n, QString v, QString dSize)
                                    : name(n), 
                                    version(v), 
                                    downloadSize(QString(dSize).toDouble()),
                                    popularity(0),
                                    status(ectn_NON_INSTALLED){
  }

  PackageListData(QString n, QString r, QString v, PackageStatus pkgStatus, QString outVersion="")
                                    : name(n),
                                    repository(r),
                                    version(v),
                                    outatedVersion(outVersion.trimmed()),
                                    downloadSize(0.0),
                                    popularity(0),
                                    status(pkgStatus){
  }

  PackageListData(QString n, QString r, QString v, QString d, PackageStatus pkgStatus, QString outVersion="")
                                    : name(n),
                                    repository(r),
                                    version(v),
                                    description(d),
                                    outatedVersion(outVersion.trimmed()),
                                    downloadSize(0.0),
                                    popularity(0),
                                    status(pkgStatus){
  }

  PackageListData(QString n, QString o, QString v, QString c, PackageStatus pkgStatus, double iSize, double dSize, QString iDate)
                                    : name(n),
                                    repository(""),
                                    origin(o),
                                    version(v),
                                    comment(c),
                                    installedSize(iSize),
                                    downloadSize(dSize),
                                    installedOn(iDate),
                                    status(pkgStatus){
  }
};

struct TransactionInfo{
  QStringList *packages;
  QString sizeToInstall;
  QString sizeToDownload;
};

struct OutdatedPackageInfo{
  QString oldVersion;
  QString newVersion;
};

struct PackageInfoData{
  QString name;
  QString repository;
  QString version;
  QString url;
  QString license;
  QString group;
  QString provides;
  QString requiredBy;
  QString optionalFor;
  QString dependsOn;
  QString optDepends;
  QString conflictsWith;
  QString replaces;
  QString packager;
  QString maintainer;
  QString arch;
  QString description;
  QString comment;
  QString installedOn;
  double downloadSize;
  double installedSize;
  QString downloadSizeAsString;
  QString installedSizeAsString;
  QString options;
};

class Result;

class Package{  
  private:
    static Result verifyPreReleasePackage(const QStringList &versao1,
                                          const QStringList &versao2, const QString &pacote);

    static QString extractFieldFromInfo(const QString &field, const QString &pkgInfo);
    static double simplePow(int base, int exp);
    static void navigateThroughDirs(QStringList parts, QStringList& auxList, int ind);

	public:
    static int rpmvercmp(const char *a, const char *b);
    static QSet<QString>* getLockedPackageList();
    static QSet<QString>* getUnrequiredPackageList();
    static QMap<QString, OutdatedPackageInfo> *getOutdatedStringList();
    static QStringList * getPackageGroups();
    static QStringList * getPackagesOfGroup(const QString &groupName);
    static TransactionInfo getTargetUpgradeList(const QString &pkgName = "");
    static QStringList * getTargetRemovalList(const QString &pkgName);
    static QList<PackageListData> * parsePackageTuple(const QStringList &packageTuples, QStringList &packageCache);
    static QList<PackageListData> *getPackageList(const QString &packageName = "");

    //Remote package methods
    static QList<PackageListData> * getRemotePackageList(const QString& searchString);
    static PackageInfoData getInformation(const QString &pkgName, bool foreignPackage = false);
    static double getDownloadSizeDescription(const QString &pkgName);
    static QString getInformationDescription(const QString &pkgName, bool foreignPackage = false);
    static QString getInformationInstalledSize(const QString &pkgName, bool foreignPackage = false);
    static QString getDependencies(const QString &pkgName);
    static QStringList getContents(const QString &pkgName, bool isInstalled);
    static QStringList getOptionalDeps(const QString &pkgName);
    static QString getName(const QString &pkgInfo);
    static QString getVersion(const QString &pkgInfo);
    static QString getRepository(const QString &pkgInfo);
    static QString getURL(const QString &pkgInfo);
    static QString getLicense(const QString &pkgInfo);
    static QString getGroup(const QString &pkgInfo);
    static QString getProvides(const QString &pkgInfo);
    static QString getDependsOn(const QString &pkgInfo);
    static QString getOptDepends(const QString &pkgInfo);
    static QString getConflictsWith(const QString &pkgInfo);
    static QString getReplaces(const QString &pkgInfo);
    static QString getRequiredBy(const QString &pkgInfo);
    static QString getOptionalFor(const QString &pkgInfo);
    static QString getPackager(const QString &pkgInfo);
    static QString getMaintainer(const QString &pkgInfo);
    static QString getArch(const QString &pkgInfo);
    static QString getDescription(const QString &pkgInfo);
    static QString getComment(const QString &pkgInfo);
    static QString getInstalledOn(const QString &pkgInfo);
    static double getDownloadSize(const QString &pkgInfo);
    static QString getDownloadSizeAsString(const QString &pkgInfo);
    static double getInstalledSize(const QString &pkgInfo);
    static QString getInstalledSizeAsString(const QString &pkgInfo);
    static QString getOptions(const QString &pkgInfo);

    static QString kbytesToSize(float Bytes );
    static double strToKBytes(QString size);
    static double strToKBytes2(QString size);
    static QString makeAnchorOfDependencies(const QString &deps);
    static QString makeURLClickable(const QString &information);
    static QString getBaseName( const QString& pkgName );
    static QString parseSearchString( QString searchStr, bool exactMatch = false );
    static bool hasPkgNGDatabase();
    static bool isForbidden(const QString pkgName);
};

#endif
