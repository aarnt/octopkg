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

#ifndef STRCONSTANTS_H
#define STRCONSTANTS_H

class QString;

class StrConstants{
public:

  static QString getApplicationName();
  static QString getApplicationVersion();
  static QString getQtVersion();
  static QString getApplicationCliHelp();
  static QString getAll();
  static QString getDragonFlyBSDNews();
  static QString getFreeBSDNews();
  static QString getGhostBSDNews();
  static QString getHardenedBSDNews();
  static QString getNewsErrorMessage();
  static QString getIncompatibleBSDFlavourError();
  static QString getInternetUnavailableError();
  static QString getDisplayAllCategories();
  static QString getHelpUsage();
  static QString getHelpAbout();
  static QString getName();
  static QString getVersion();
  static QString getOutdatedVersion();
  static QString getAvailableVersion();
  static QString getNoDescriptionAvailabe();
  static QString getURL();
  static QString getOrigin();
  static QString getLicenses();
  static QString getCategory();
  static QString getCategories();
  static QString getProvides();
  static QString getDependencies();
  static QString getDependsOn();
  static QString getRequiredBy();
  static QString getOptionalFor();
  static QString getOptionalDeps();
  static QString getConflictsWith();
  static QString getReplaces();
  static QString getOptions();
  static QString getSize();
  static QString getDownloadSize();
  static QString getInstalledSize();
  static QString getPackager();
  static QString getMaintainer();
  static QString getPackage();
  static QString getPackages();
  static QString getArchitecture();
  static QString getInstalledOn();
  static QString getDescription();
  static QString getAttention();
  static QString getAutomaticSuCommand();
  static QString getPassword();
  static QString getTabInfoName();
  static QString getTabFilesName();
  static QString getTabTransactionName();
  static QString getTabOutputName();
  static QString getTabNewsName();
  static QString getContentsOf();
  static QString getFind();
  static QString getClear();
  static QString getOutdatedInstalledVersion();
  static QString getNewerInstalledVersion();
  static QString getBuildingPackageList();
  static QString getSearchingForBSDNews();
  static QString getOneOutdatedPackage();
  static QString getOutdatedPackages(int outdatedPackagesCount);
  static QString getNewVersionAvailable();
  static QString getTotalPackages(int packageCount);
  static QString getSelectedPackages(int selectedCount);
  static QString getNumberInstalledPackages(int installedPackagesCount);
  static QString getNumberOutdatedPackages(int outdatedPackagesCount);
  static QString getNumberAvailablePackages(int availablePackagesCount);
  static QString getCleaningPackageCache();
  static QString getRemovingPacmanTransactionLockFile();
  static QString getLineEditTextLocal();
  static QString getLineEditTextRemote();
  static QString getRemotePackageSearchTip();
  static QString getSyncing();
  static QString getPressAnyKey();
  static QString getSyncDatabase();
  static QString getCheckUpdates();
  static QString getIsUpToDate();
  static QString getLockingPackage();
  static QString getUnlockingPackage();
  static QString getSystemUpgrade();
  static QString getInstallingPackages();
  static QString getRemovingPackages();
  static QString getRemovingAndInstallingPackages();
  static QString getChooseATerminal();
  static QString getCollectingTransactionData();
  static QString getRunningCommandInTerminal();
  static QString getCommandFinishedOK();
  static QString getCommandFinishedWithErrors();
  static QString geRetrievingPackage();
  static QString getTotalDownloadSize();
  static QString getRetrievePackage();
  static QString getRemovePackage();
  static QString getRetrievePackages(int retrievePackagesCount);
  static QString getRemovePackages(int removePackagesCount);
  static QString getWarnHoldPkgFound();
  static QString getNoNewUpdatesAvailable();
  static QString getOneNewUpdate();
  static QString getNewUpdates(int newUpdatesCount);
  static QString getConfirmationQuestion();
  static QString getWarning();
  static QString getConfirmation();
  static QString getThereHasBeenATransactionError();
  static QString getConfirmExecuteTransactionInTerminal();
  static QString getCleanCacheConfirmation();
  static QString getRemovePacmanTransactionLockFileConfirmation();
  static QString getCancelTransactionConfirmation();
  static QString getPkgNotAvailable();
  static QString getEnterAdministratorsPassword();
  static QString getErrorNoSuCommand();
  static QString getYoullNeedSuFrontend();
  static QString getOctoPKGDoasNotFound();
  static QString getErrorNoPkgFound();
  static QString getErrorNoDoasSudoFound();
  static QString getErrorNoSHFound();
  static QString getErrorRunningWithRoot();
  static QString getThereIsARunningTransaction();
  static QString getThereArePendingActions();
  static QString getDoYouReallyWantToQuit();
  static QString getExecutingCommand();
  static QString getRunInTerminal();
  static QString getNeedsAppRestart();
  static QString getWarnNeedsAppRestart();
  static QString getFileChooserTitle();
  static QString getThisIsNotATextFile();
  static QString getTransactionInstallText();
  static QString getTransactionRemoveText();
  static QString getRemove();
  static QString getInstall();
  static QString getInstallPkg();
  static QString getReinstall();
  static QString getUpdate();
  static QString getLock();
  static QString getUnlock();
  static QString getRemoveItem();
  static QString getRemoveItems();
  static QString getPressCtrlAToSelectAll();
  static QString getFilterLocalPackages();
  static QString getSearchForPackages();
  static QString getSearchStringIsShort();
  static QString getCopyFullPath();
  static QString getNotifierSetupDialogTitle();
  static QString getNotiferSetupDialogGroupBoxTitle();
  static QString getOnceADay();
  static QString getOnceADayAt();
  static QString getOnceADayAtDesc();
  static QString getOnceEvery();
  static QString getOnceEveryDesc();
  static QString getSetInterval();
  static QString getForeignToolGroup();
  static QString getStop();

  //Style Sheets ---------------------------------

  static QString getToolBarCSS();
  static QString getTreeViewCSS();
};

#endif // STRCONSTANTS_H
