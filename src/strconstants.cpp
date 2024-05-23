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

#include "strconstants.h"
#include <QObject>
#include <QString>

QString StrConstants::getApplicationName(){
  return "OctoPkg";
}

QString StrConstants::getApplicationVersion(){
  return "0.4.0";
}

QString StrConstants::getQtVersion(){
  return "Qt " + QString(QT_VERSION_STR);
}

QString StrConstants::getApplicationCliHelp(){
  QString str =
      "\n" + QObject::tr("OctoPkg usage help:") + "\n\n" +
      "-version: " + QObject::tr("show application version.") + "\n"; /*+
      "-sysupgrade: " + QObject::tr("force a system upgrade at startup.") + "\n";*/
  return str;
}

QString StrConstants::getAll(){
  return QObject::tr("All");
}

QString StrConstants::getDragonFlyBSDNews(){
  return QObject::tr("DragonFlyBSD news");
}

QString StrConstants::getFreeBSDNews(){
  return QObject::tr("FreeBSD news");
}

QString StrConstants::getGhostBSDNews(){
  return QObject::tr("GhostBSD news");
}

QString StrConstants::getHardenedBSDNews(){
  return QObject::tr("HardenedBSD news");
}

QString StrConstants::getNewsErrorMessage(){
  return QObject::tr("No news could be found! Press Ctrl+G to download the latest news.");
}

QString StrConstants::getIncompatibleBSDFlavourError(){
  return QObject::tr("This BSD flavour seems to be incompatible with OctoPkg!");
}

QString StrConstants::getInternetUnavailableError(){
  return QObject::tr("Internet seems unavailable!");
}

QString StrConstants::getDisplayAllCategories(){
  return QObject::tr("Display all categories");
}

QString StrConstants::getHelpUsage(){
  return QObject::tr("Usage");
}

QString StrConstants::getHelpAbout(){
  return QObject::tr("About");
}

QString StrConstants::getName(){
  return QObject::tr("Name");
}

QString StrConstants::getVersion(){
  return QObject::tr("Version");
}

QString StrConstants::getOutdatedVersion(){
  return QObject::tr("Outdated version");
}

QString StrConstants::getAvailableVersion(){
  return QObject::tr("Available version");
}

QString StrConstants::getNoDescriptionAvailabe(){
  return QObject::tr("No description available.");
}

QString StrConstants::getURL(){
  return QObject::tr("URL");
}

QString StrConstants::getOrigin(){
  return QObject::tr("Origin");
}

QString StrConstants::getLicenses(){
  return QObject::tr("Licenses");
}

QString StrConstants::getCategory(){
  return QObject::tr("Category");
}

QString StrConstants::getCategories(){
  return QObject::tr("Categories");
}

QString StrConstants::getProvides(){
  return QObject::tr("Provides");
}

QString StrConstants::getDependencies(){
  return QObject::tr("Dependencies");
}

QString StrConstants::getDependsOn(){
  return QObject::tr("Depends On");
}

QString StrConstants::getRequiredBy(){
  return QObject::tr("Required By");
}

QString StrConstants::getOptionalFor(){
  return QObject::tr("Optional For");
}

QString StrConstants::getOptionalDeps(){
  return QObject::tr("Optional Deps");
}

QString StrConstants::getConflictsWith(){
  return QObject::tr("Conflicts With");
}

QString StrConstants::getReplaces(){
  return QObject::tr("Replaces");
}

QString StrConstants::getOptions(){
  return QObject::tr("Options");
}

QString StrConstants::getSize(){
  return QObject::tr("Size");
}

QString StrConstants::getDownloadSize(){
  return QObject::tr("Download Size");
}

QString StrConstants::getInstalledSize(){
  return QObject::tr("Installed Size");
}

QString StrConstants::getPackager(){
  return QObject::tr("Packager");
}

QString StrConstants::getMaintainer(){
  return QObject::tr("Maintainer");
}

QString StrConstants::getPackage(){
  return QObject::tr("Package");
}

QString StrConstants::getPackages(){
  return QObject::tr("Packages");
}

QString StrConstants::getArchitecture(){
  return QObject::tr("Architecture");
}

QString StrConstants::getInstalledOn(){
  return QObject::tr("Installed on");
}

QString StrConstants::getDescription(){
  return QObject::tr("Description");
}

QString StrConstants::getAttention(){
  return QObject::tr("Attention");
}

QString StrConstants::getAutomaticSuCommand(){
  return QObject::tr("automatic");
}

QString StrConstants::getPassword(){
  return QObject::tr("Password");
}

QString StrConstants::getTabInfoName(){
  return QObject::tr("Info");
}

QString StrConstants::getTabFilesName(){
  return QObject::tr("Files");
}

QString StrConstants::getTabTransactionName(){
  return QObject::tr("Actions");
}

QString StrConstants::getTabOutputName(){
  return QObject::tr("Output");
}

QString StrConstants::getTabNewsName(){
  return QObject::tr("News");
}

QString StrConstants::getContentsOf(){
  return QObject::tr("Contents of \"%1\"");
}

QString StrConstants::getFind(){
  return QObject::tr("Find");
}

QString StrConstants::getClear(){
  return QObject::tr("Clear");
}

QString StrConstants::getOutdatedInstalledVersion(){
  return QObject::tr(" (outdated installed version is %1)");
}

QString StrConstants::getNewerInstalledVersion(){
  return QObject::tr(" (newer installed version is %1)");
}

QString StrConstants::getBuildingPackageList(){
  return QObject::tr("Building package list...");
}

QString StrConstants::getSearchingForBSDNews(){
  return QObject::tr("Searching for %1 latest news...");
}

QString StrConstants::getOneOutdatedPackage(){
  return QObject::tr("There is one outdated package in your system:");
}

QString StrConstants::getOutdatedPackages(int outdatedPackagesCount){
  return QObject::tr("There are %n outdated packages in your system:", 0, outdatedPackagesCount);
}

QString StrConstants::getNewVersionAvailable(){
  return QObject::tr("(version %1 is available)");
}

QString StrConstants::getTotalPackages(int packageCount){
  return QObject::tr("%n packages", 0, packageCount);
}

QString StrConstants::getSelectedPackages(int selectedCount){
  return QObject::tr("%n selected", 0, selectedCount);
}

QString StrConstants::getNumberInstalledPackages(int installedPackagesCount){
  return QObject::tr("%n installed", 0, installedPackagesCount);
}

QString StrConstants::getNumberOutdatedPackages(int outdatedPackagesCount){
  return QObject::tr("%n outdated", 0, outdatedPackagesCount);
}

QString StrConstants::getNumberAvailablePackages(int availablePackagesCount){
  return QObject::tr("%n available", 0, availablePackagesCount);
}

QString StrConstants::getCleaningPackageCache(){
  return QObject::tr("Cleaning the local cache...");
}

QString StrConstants::getRemovingPacmanTransactionLockFile(){
  return QObject::tr("Removing Pacman's transaction lock file...");
}

QString StrConstants::getLineEditTextLocal(){
  return QObject::tr("Search for installed packages");
}

QString StrConstants::getLineEditTextRemote(){
  return QObject::tr("Type the package name/description and press ENTER");
}

QString StrConstants::getRemotePackageSearchTip(){
  return QObject::tr("Press ENTER to start your search");
}

QString StrConstants::getSyncing(){
  return QObject::tr("Syncing");
}

QString StrConstants::getPressAnyKey(){
  return QObject::tr("Press any key to continue...");
}

QString StrConstants::getSyncDatabase(){
  return QObject::tr("Sync database");
}

QString StrConstants::getCheckUpdates(){
  return QObject::tr("Checking for updates...");
}

QString StrConstants::getIsUpToDate(){
  return QObject::tr("is up to date");
}

QString StrConstants::getLockingPackage()
{
  return QObject::tr("Locking package...");
}

QString StrConstants::getUnlockingPackage()
{
  return QObject::tr("Unlocking package...");
}

QString StrConstants::getSystemUpgrade(){
  return QObject::tr("Installing updates...");
}

QString StrConstants::getInstallingPackages(){
  return QObject::tr("Installing selected packages...");
}

QString StrConstants::getRemovingPackages(){
  return QObject::tr("Removing selected packages...");
}

QString StrConstants::getRemovingAndInstallingPackages(){
  return QObject::tr("Removing/installing selected packages...");
}

QString StrConstants::getChooseATerminal(){
  return QObject::tr("Choose a terminal");
}

QString StrConstants::getRunningCommandInTerminal(){
  return QObject::tr("Running command in terminal...");
}

QString StrConstants::getCommandFinishedOK(){
  return QObject::tr("Command finished OK!");
}

QString StrConstants::getCommandFinishedWithErrors(){
  return QObject::tr("Command finished with errors!");
}

QString StrConstants::geRetrievingPackage(){
  return QObject::tr("Retrieving %1");
}

QString StrConstants::getTotalDownloadSize(){
  return QObject::tr("Total download size: %1 KB");
}

QString StrConstants::getRetrievePackage(){
  return QObject::tr("The following package needs to be retrieved");
}

QString StrConstants::getRemovePackage(){
  return QObject::tr("The following package will be removed");
}

QString StrConstants::getRetrievePackages(int retrievePackagesCount){
  return QObject::tr("The following %n packages need to be retrieved", 0, retrievePackagesCount);
}

QString StrConstants::getRemovePackages(int removePackagesCount){
  return QObject::tr("The following %n packages will be removed", 0, removePackagesCount);
}

QString StrConstants::getWarnHoldPkgFound() {
  return QObject::tr("There are forbidden packages in the removal list!");
}

QString StrConstants::getNoNewUpdatesAvailable(){
  return QObject::tr("There are no new updates available!");
}

QString StrConstants::getOneNewUpdate(){
  return QObject::tr("There is an update available!");
}

QString StrConstants::getNewUpdates(int newUpdatesCount){
  return QObject::tr("There are %n updates available!", 0, newUpdatesCount);
}

QString StrConstants::getConfirmationQuestion(){
  return QObject::tr("Confirm?");
}

QString StrConstants::getWarning(){
  return QObject::tr("Warning");
}

QString StrConstants::getConfirmation(){
  return QObject::tr("Confirmation");
}

QString StrConstants::getThereHasBeenATransactionError(){
  return QObject::tr("There has been an action error!");
}

QString StrConstants::getConfirmExecuteTransactionInTerminal(){
  return QObject::tr("Do you want to execute these actions in a Terminal?");
}

QString StrConstants::getCleanCacheConfirmation(){
  return QObject::tr("Do you really want to clean the local package cache?");
}

QString StrConstants::getRemovePacmanTransactionLockFileConfirmation(){
  return QObject::tr("Do you really want to remove Pacman's transaction lock file?");
}

QString StrConstants::getCancelTransactionConfirmation(){
  return QObject::tr("Do you really want to cancel the action?");
}

QString StrConstants::getPkgNotAvailable(){
  return QObject::tr("This package is not available in the database!");
}

QString StrConstants::getEnterAdministratorsPassword(){
  return QObject::tr(
        "Please, enter the administrator's password");
}

QString StrConstants::getErrorNoSuCommand(){
  return
      QObject::tr("There are no means to get administrator's credentials.");
}

QString StrConstants::getYoullNeedSuFrontend(){
  return QObject::tr("You'll need to install qt-sudo.");
}

QString StrConstants::getOctoPKGDoasNotFound(){
  return QObject::tr("\"/usr/local/bin/qt-sudo\" was not found!");
}

QString StrConstants::getErrorNoPkgFound(){
  return QObject::tr("'pkg' binary was not found.");
}

QString StrConstants::getErrorNoDoasSudoFound(){
  return QObject::tr("Neither 'doas' or 'sudo' were found or configured in your system.");
}

QString StrConstants::getErrorNoSHFound(){
  return QObject::tr("'sh' binary was not found.");
}

QString StrConstants::getErrorRunningWithRoot(){
  return QObject::tr("You can not run OctoPkg with administrator's credentials.");
}

QString StrConstants::getThereIsARunningTransaction(){
  return QObject::tr("Canceling the running transaction may damage your system!");
}

QString StrConstants::getThereArePendingActions(){
  return QObject::tr("There are pending actions");
}

QString StrConstants::getDoYouReallyWantToQuit(){
  return QObject::tr("Do you really want to quit?");
}

QString StrConstants::getExecutingCommand(){
  return QObject::tr("Executing command");
}

QString StrConstants::getRunInTerminal(){
  return QObject::tr("Run in terminal");
}

QString StrConstants::getNeedsAppRestart(){
  return QObject::tr("Needs application restart to take effect");
}

QString StrConstants::getWarnNeedsAppRestart(){
  return QObject::tr("These changes need application restart to take effect!");
}

QString StrConstants::getFileChooserTitle(){
  return QObject::tr("Select the packages you want to install");
}

QString StrConstants::getThisIsNotATextFile(){
  return QObject::tr("This file does not appear to be a simple text.\n"
                     "Are you sure you want to open it?");
}

QString StrConstants::getTransactionInstallText(){
  return QObject::tr("To be installed");
}

QString StrConstants::getTransactionRemoveText(){
  return QObject::tr("To be removed");
}

QString StrConstants::getRemove(){
  return QObject::tr("remove");
}

QString StrConstants::getInstall(){
  return QObject::tr("install");
}

QString StrConstants::getInstallPkg(){
  return QObject::tr("Install");
}

QString StrConstants::getReinstall()
{
  return QObject::tr("Reinstall");
}

QString StrConstants::getUpdate()
{
  return QObject::tr("Update");
}

QString StrConstants::getLock()
{
  return QObject::tr("Lock");
}

QString StrConstants::getUnlock()
{
  return QObject::tr("Unlock");
}

QString StrConstants::getRemoveItem(){
  return QObject::tr("Remove item");
}

QString StrConstants::getRemoveItems(){
  return QObject::tr("Remove items");
}

QString StrConstants::getPressCtrlAToSelectAll(){
  return QObject::tr("Press Ctrl+A to select/deselect all");
}

QString StrConstants::getFilterLocalPackages(){
  return QObject::tr("View installed packages");
}

QString StrConstants::getSearchForPackages(){
  return QObject::tr("Install a new package");
}

QString StrConstants::getSearchStringIsShort(){
  return QObject::tr("Search string must have at least 2 characters.");
}

QString StrConstants::getCopyFullPath(){
  return QObject::tr("Copy path to clipboard");
}

QString StrConstants::getNotifierSetupDialogTitle(){
  return QObject::tr("Setup");
}

QString StrConstants::getNotiferSetupDialogGroupBoxTitle(){
  return QObject::tr("Sync database interval");
}

QString StrConstants::getOnceADay(){
  return QObject::tr("Once a day");
}

QString StrConstants::getOnceADayAt(){
  return QObject::tr("Once a day, at");
}

QString StrConstants::getOnceADayAtDesc(){
  return QObject::tr("(value in 24-hour format: 0 to 23)");
}

QString StrConstants::getOnceEvery(){
  return QObject::tr("Once every");
}

QString StrConstants::getOnceEveryDesc(){
  return QObject::tr("(value in minutes: 5 to 1380)");
}

QString StrConstants::getSetInterval(){
  return QObject::tr("Set interval...");
}

QString StrConstants::getForeignToolGroup(){
  /*QString tool = getForeignRepositoryToolName();
    tool[0] = tool[0].toUpper();
    tool = "<" + tool + ">";
    return tool.toLatin1();*/

  return "NONE";
}

QString StrConstants::getStop()
{
  return QObject::tr("Stop");
}

//Style Sheets ---------------------------------

QString StrConstants::getToolBarCSS(){
  return QString("QToolBar { border: 5px; } "
                 "QToolTip {}");
}

QString StrConstants::getTreeViewCSS(){
  return QString("QTreeView::branch:has-siblings:!adjoins-item {"
                 "   border-image: url(:/resources/styles/vline.png) 0;}"
                 "QTreeView::branch:has-siblings:adjoins-item {"
                 "    border-image: url(:/resources/styles/branch-more.png) 0;}"
                 "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
                 "   border-image: url(:/resources/styles/branch-end.png) 0;}"
                 "QTreeView::branch:has-children:!has-siblings:closed,"
                 "QTreeView::branch:closed:has-children:has-siblings {"
                 "       border-image: none;"
                 "        image: url(:/resources/styles/branch-closed_BW.png);}"
                 "QTreeView::branch:open:has-children:!has-siblings,"
                 "QTreeView::branch:open:has-children:has-siblings  {"
                 "       border-image: none;"
                 "       image: url(:/resources/styles/branch-open_BW.png);}");
}
