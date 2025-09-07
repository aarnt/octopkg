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

/*
 * This is MainWindow's Transaction related code
 */

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "uihelper.h"
#include "wmhelper.h"
#include "strconstants.h"
#include "transactiondialog.h"
#include "globals.h"
#include "searchlineedit.h"
#include "constants.h"
#include <iostream>
#include <cassert>

#include <QComboBox>
#include <QProgressBar>
#include <QMessageBox>
#include <QStandardItem>
#include <QTextBrowser>
#include <QDebug>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

/*
 * Disables transaction buttons
 */
void MainWindow::disableTransactionButtons()
{
  ui->actionCommit->setEnabled(false);
  ui->actionCancel->setEnabled(false);
}

/*
 * Watches the state of tvTransaction treeview to see if Commit/Cancel actions must be activated/deactivated
 */
void MainWindow::changeTransactionActionsState()
{
  bool state = isThereAPendingTransaction();
  ui->actionCommit->setEnabled(state);
  ui->actionCancel->setEnabled(state);
  ui->actionCheckUpdates->setEnabled(!state);

  if (state == false && m_outdatedStringList->count() > 0)
    ui->actionSystemUpgrade->setEnabled(true);
  else if (state == true)
    ui->actionSystemUpgrade->setEnabled(false);
}

/*
 * Removes all packages from the current transaction
 */
void MainWindow::clearTransactionTreeView()
{
  removePackagesFromRemoveTransaction();
  removePackagesFromInstallTransaction();
}

/*
 * Looks if the Transaction's Remove or Install parent items have any package inside them
 */
bool MainWindow::isThereAPendingTransaction()
{
  return (getRemoveTransactionParentItem()->hasChildren() ||
          getInstallTransactionParentItem()->hasChildren());
}

/*
 * Retrieves the Remove parent item of the Transaction treeview
 */
QStandardItem * MainWindow::getRemoveTransactionParentItem()
{
  QTreeView *tvTransaction =
      ui->twProperties->widget(ctn_TABINDEX_TRANSACTION)->findChild<QTreeView*>("tvTransaction");
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(tvTransaction->model());
  QStandardItem *si = 0;

  if(sim)
  {
    si = sim->item(0, 0);
  }

  return si;
}

/*
 * Retrieves the Install parent item of the Transaction treeview
 */
QStandardItem * MainWindow::getInstallTransactionParentItem()
{
  QTreeView *tvTransaction =
      ui->twProperties->widget(ctn_TABINDEX_TRANSACTION)->findChild<QTreeView*>("tvTransaction");
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(tvTransaction->model());
  QStandardItem *si = 0;

  if(sim)
  {
    si = sim->item(1, 0);
  }

  return si;
}

/*
 * Inserts the given package into the Remove parent item of the Transaction treeview
 */
void MainWindow::insertRemovePackageIntoTransaction(const QString &pkgName)
{
  QTreeView *tvTransaction =
      ui->twProperties->widget(ctn_TABINDEX_TRANSACTION)->findChild<QTreeView*>("tvTransaction");
  QStandardItem * siRemoveParent = getRemoveTransactionParentItem();
  QStandardItem * siInstallParent = getInstallTransactionParentItem();
  QStandardItem * siPackageToRemove = new QStandardItem(IconHelper::getIconToRemove(), pkgName);
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(siRemoveParent->model());
  QList<QStandardItem *> foundItems = sim->findItems(pkgName, Qt::MatchRecursive | Qt::MatchExactly);
  int slash = pkgName.indexOf("/");
  QString pkg = pkgName.mid(slash+1);
  siPackageToRemove->setText(pkg);

  if (foundItems.size() == 0)
  {
    int slash = pkgName.indexOf("/");
    QString pkg = pkgName.mid(slash+1);
    QList<QStandardItem *> aux = sim->findItems(pkg, Qt::MatchRecursive | Qt::MatchExactly);

    if (aux.count() > 0) siInstallParent->removeRow(aux.at(0)->row());
    siRemoveParent->appendRow(siPackageToRemove);
  }
  else if (foundItems.size() == 1 && foundItems.at(0)->parent() == siInstallParent)
  {
    siInstallParent->removeRow(foundItems.at(0)->row());
    siRemoveParent->appendRow(siPackageToRemove);
  }

  //ui->twProperties->setCurrentIndex(ctn_TABINDEX_TRANSACTION);
  tvTransaction->expandAll();
  changeTransactionActionsState();
}

/*
 * Inserts the given package into the Install parent item of the Transaction treeview
 */
void MainWindow::insertInstallPackageIntoTransaction(const QString &pkgName)
{
  QTreeView *tvTransaction =
      ui->twProperties->widget(ctn_TABINDEX_TRANSACTION)->findChild<QTreeView*>("tvTransaction");
  QStandardItem * siInstallParent = getInstallTransactionParentItem();
  QStandardItem * siPackageToInstall = new QStandardItem(IconHelper::getIconToInstall(), pkgName);
  QStandardItem * siRemoveParent = getRemoveTransactionParentItem();
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(siInstallParent->model());
  QList<QStandardItem *> foundItems = sim->findItems(pkgName, Qt::MatchRecursive | Qt::MatchExactly);

  if (foundItems.size() == 0)
  {
    int slash = pkgName.indexOf("/");
    QString pkg = pkgName.mid(slash+1);
    QList<QStandardItem *> aux = sim->findItems(pkg, Qt::MatchRecursive | Qt::MatchExactly);

    if (aux.count() > 0) siRemoveParent->removeRow(aux.at(0)->row());
    siInstallParent->appendRow(siPackageToInstall);
  }
  else if (foundItems.size() == 1 && foundItems.at(0)->parent() == siRemoveParent)
  {
    siRemoveParent->removeRow(foundItems.at(0)->row());
    siInstallParent->appendRow(siPackageToInstall);
  }

  //ui->twProperties->setCurrentIndex(ctn_TABINDEX_TRANSACTION);
  tvTransaction->expandAll();
  changeTransactionActionsState();
}

/*
 * Removes all packages from the Remove parent item of the Transaction treeview
 */
void MainWindow::removePackagesFromRemoveTransaction()
{
  QStandardItem * siRemove = getRemoveTransactionParentItem();
  siRemove->removeRows(0, siRemove->rowCount());
  changeTransactionActionsState();
}

/*
 * Removes all packages from the Install parent item of the Transaction treeview
 */
void MainWindow::removePackagesFromInstallTransaction()
{
  QStandardItem * siInstall = getInstallTransactionParentItem();
  siInstall->removeRows(0, siInstall->rowCount());
  changeTransactionActionsState();
}

/*
 * Retrieves the number of "to be removed" packages
 */
int MainWindow::getNumberOfTobeRemovedPackages()
{
  QStandardItem * siRemoval = getRemoveTransactionParentItem();
  return siRemoval->rowCount();
}

/*
 * Retrieves the list of all packages scheduled to be removed
 */
QString MainWindow::getTobeRemovedPackages()
{
  QStandardItem * siRemoval = getRemoveTransactionParentItem();
  QString res;

  for(int c=0; c < siRemoval->rowCount(); c++)
  {
    res += siRemoval->child(c)->text() + " ";
  }

  res = res.trimmed();
  return res;
}

/*
 * Retrieves the list of all packages scheduled to be installed
 */
QString MainWindow::getTobeInstalledPackages()
{
  QStandardItem * siInstall = getInstallTransactionParentItem();
  QString res;

  for(int c=0; c < siInstall->rowCount(); c++)
  {
    res += siInstall->child(c)->text() + " ";
  }

  res = res.trimmed();
  return res;
}

/*
 * Inserts the current selected packages for removal into the Transaction Treeview
 * This is the SLOT, it needs to call insertRemovePackageIntoTransaction(PackageName) to work!
 */
void MainWindow::insertIntoRemovePackage()
{
  qApp->processEvents();
  //ensureTabVisible(ctn_TABINDEX_TRANSACTION);
  QModelIndexList selectedRows = ui->tvPackages->selectionModel()->selectedRows();

  //First, let's see if we are dealing with a package group
  if(!isAllCategoriesSelected())
  {
    //If we are trying to remove all the group's packages, why not remove the entire group?
    if(selectedRows.count() == m_packageModel->getPackageCount())
    {
      insertRemovePackageIntoTransaction(getSelectedCategory());
      return;
    }
  }

  for(QModelIndex item: selectedRows)
  {
    const PackageRepository::PackageData*const package = m_packageModel->getData(item);
    if (package == NULL) {
      assert(false);
      continue;
    }

    insertRemovePackageIntoTransaction(/*package->repository + "/" + */package->name);
  }
}

/*
 * Inserts the current selected group for removal into the Transaction Treeview
 */
void MainWindow::insertGroupIntoRemovePackage()
{
  //ensureTabVisible(ctn_TABINDEX_TRANSACTION);
  insertRemovePackageIntoTransaction(getSelectedCategory());
}

/*
 * Inserts the current selected packages for installation into the Transaction Treeview
 * This is the SLOT, it needs to call insertInstallPackageIntoTransaction(PackageName) to work!
 */
void MainWindow::insertIntoInstallPackage()
{
  qApp->processEvents();
  //ensureTabVisible(ctn_TABINDEX_TRANSACTION);
  QModelIndexList selectedRows = ui->tvPackages->selectionModel()->selectedRows();

  for(QModelIndex item: selectedRows)
  {
    const PackageRepository::PackageData*const package = m_packageModel->getData(item);
    if (package == NULL) {
      assert(false);
      continue;
    }

    insertInstallPackageIntoTransaction(package->name);
  }
}

/*
 * Searches in Install Transaction queue for the given package
 */
bool MainWindow::isPackageInInstallTransaction(const QString &pkgName)
{
  QStandardItem * siInstallParent = getInstallTransactionParentItem();
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(siInstallParent->model());
  const PackageRepository::PackageData*const package = m_packageRepo.getFirstPackageByName(pkgName);
  QString repo;

  if (package != NULL) repo = package->repository;

  QList<QStandardItem *> foundItems = sim->findItems(repo + "/" + pkgName, Qt::MatchRecursive | Qt::MatchExactly);

  return (foundItems.size() > 0);
}

/*
 * Searches in Remove Transaction queue for the given package
 */
bool MainWindow::isPackageInRemoveTransaction(const QString &pkgName)
{
  QStandardItem * siRemoveParent = getRemoveTransactionParentItem();
  QStandardItemModel *sim = qobject_cast<QStandardItemModel *>(siRemoveParent->model());
  QList<QStandardItem *> foundItems = sim->findItems(pkgName, Qt::MatchRecursive | Qt::MatchExactly);

  return (foundItems.size() > 0);
}

/*
 * Inserts all optional deps of the current select package into the Transaction Treeview
 */
/*void MainWindow::insertIntoInstallPackageOptDeps(const QString &packageName)
{
  CPUIntensiveComputing *cic = new CPUIntensiveComputing;

  //Does this package have non installed optional dependencies?
  QStringList optDeps = Package::getOptionalDeps(packageName); //si->text());
  QList<const PackageRepository::PackageData*> optionalPackages;

  for(QString optDep: optDeps)
  {
    QString candidate = optDep;
    int points = candidate.indexOf(":");
    candidate = candidate.mid(0, points).trimmed();

    const PackageRepository::PackageData*const package = m_packageRepo.getFirstPackageByName(candidate);
    if(!isPackageInInstallTransaction(candidate) &&
       !isPackageInstalled(candidate) && package != 0)
    {
      optionalPackages.append(package);
    }
  }

  if(optionalPackages.count() > 0)
  {
    MultiSelectionDialog *msd = new MultiSelectionDialog(this);
    msd->setWindowTitle(packageName + ": " + StrConstants::getOptionalDeps());
    msd->setWindowIcon(windowIcon());
    QStringList selectedPackages;

    for(const PackageRepository::PackageData* candidate: optionalPackages)
    {
      QString desc = candidate->description;
      int space = desc.indexOf(" ");
      desc = desc.mid(space+1);

      msd->addPackageItem(candidate->name, candidate->description, candidate->repository);
    }

    delete cic;
    if (msd->exec() == QMessageBox::Ok)
    {
      selectedPackages = msd->getSelectedPackages();
      for(QString pkg: selectedPackages)
      {
        insertInstallPackageIntoTransaction(pkg);
      }
    }

    delete msd;
  }
  else
  {
    delete cic;
  }
}*/

/*
 * Inserts all remove dependencies of the current select package into the Transaction Treeview
 * Returns TRUE if the user click OK or ENTER and number of selected packages > 0.
 * Returns FALSE otherwise.
 */
/*bool MainWindow::insertIntoRemovePackageDeps(const QStringList &dependencies)
{
  QList<const PackageRepository::PackageData*> newDeps;
  for(QString dep: dependencies)
  {
    const PackageRepository::PackageData*const package = m_packageRepo.getFirstPackageByName(dep);
    if (package != NULL && package->installed() && !isPackageInRemoveTransaction(dep))
    {
      newDeps.append(package);
    }
  }

  if (newDeps.count() > 0)
  {
    CPUIntensiveComputing *cic = new CPUIntensiveComputing;

    MultiSelectionDialog *msd = new MultiSelectionDialog(this);
    msd->setWindowTitle(StrConstants::getRemovePackages(newDeps.count()));
    msd->setWindowIcon(windowIcon());
    QStringList selectedPackages;

    for(const PackageRepository::PackageData* dep: newDeps)
    {
      QString desc = dep->description;
      int space = desc.indexOf(" ");
      desc = desc.mid(space+1);
      msd->addPackageItem(dep->name, desc, dep->repository);
    }

    msd->setAllSelected();
    delete cic;
    int res = msd->exec();

    if (res == QMessageBox::Ok)
    {
      selectedPackages = msd->getSelectedPackages();
      for(QString pkg: selectedPackages)
      {
        insertRemovePackageIntoTransaction(pkg);
      }
    }

    delete msd;

    return (res == QMessageBox::Ok && selectedPackages.count() >= 0);
  }
  else return true;
}*/

/*
 * Inserts the current selected group for removal into the Transaction Treeview
 */
void MainWindow::insertGroupIntoInstallPackage()
{
  //ensureTabVisible(ctn_TABINDEX_TRANSACTION);
  insertInstallPackageIntoTransaction(getSelectedCategory());
}

/*
 * Adjust the count and selection count status of the selected tvTransaction item (Remove or Insert parents)
 */
void MainWindow::tvTransactionAdjustItemText(QStandardItem *item)
{
  int countSelected=0;

  QTreeView *tvTransaction =
      ui->twProperties->currentWidget()->findChild<QTreeView*>("tvTransaction");
  if (!tvTransaction) return;

  for(int c=0; c < item->rowCount(); c++)
  {
    if(tvTransaction->selectionModel()->isSelected(item->child(c)->index()))
    {
      countSelected++;
    }
  }

  QString itemText = item->text();
  int slash = itemText.indexOf("/");
  int pos = itemText.indexOf(")");

  if (slash > 0){
    itemText.remove(slash, pos-slash);
  }

  pos = itemText.indexOf(")");
  itemText.insert(pos, "/" + QString::number(countSelected));
  item->setText(itemText);
}

/*
 * SLOT called each time the selection of items in tvTransaction is changed
 */
void MainWindow::tvTransactionSelectionChanged(const QItemSelection&, const QItemSelection&)
{
  tvTransactionAdjustItemText(getRemoveTransactionParentItem());
  tvTransactionAdjustItemText(getInstallTransactionParentItem());
}

/*
 * Method called every time some item is inserted or removed in tvTransaction treeview
 */
void MainWindow::tvTransactionRowsChanged(const QModelIndex& parent)
{
  if (m_commandExecuting == ectn_NONE)
    ensureTabVisible(ctn_TABINDEX_TRANSACTION);

  QStandardItem *item = m_modelTransaction->itemFromIndex(parent);
  QString count = QString::number(item->rowCount());

  QStandardItem * itemRemove = getRemoveTransactionParentItem();
  QStandardItem * itemInstall = getInstallTransactionParentItem();

  if (item == itemRemove)
  {
    if (item->rowCount() > 0)
    {
      itemRemove->setText(StrConstants::getTransactionRemoveText() + " (" + count + ")");
      tvTransactionAdjustItemText(itemRemove);
    }
    else itemRemove->setText(StrConstants::getTransactionRemoveText());
  }
  else if (item == itemInstall)
  {
    if (item->rowCount() > 0)
    {
      itemInstall->setText(StrConstants::getTransactionInstallText() + " (" + count + ")");
      tvTransactionAdjustItemText(itemInstall);
    }
    else itemInstall->setText(StrConstants::getTransactionInstallText());
  }

  int lToInstall=itemInstall->rowCount();
  int lToRemove=itemRemove->rowCount();

  if (lToInstall > 0 || lToRemove > 0)
  {
    QString newText=StrConstants::getTabTransactionName() + QLatin1String(" (");

    if (lToInstall > 0)
      newText += QLatin1String("+") + QString::number(lToInstall);
    if (lToRemove > 0)
      newText += QLatin1String("-") + QString::number(lToRemove);

    newText += QLatin1String(")");
    ui->twProperties->setTabText(ctn_TABINDEX_TRANSACTION, newText);
  }
  else
  {
    ui->twProperties->setTabText(ctn_TABINDEX_TRANSACTION, StrConstants::getTabTransactionName());
  }
}

/*
 * SLOT called each time some item is inserted into tvTransaction
 */
void MainWindow::tvTransactionRowsInserted(const QModelIndex& parent, int, int)
{
  tvTransactionRowsChanged(parent);
}

/*
 * SLOT called each time some item is removed from tvTransaction
 */
void MainWindow::tvTransactionRowsRemoved(const QModelIndex& parent, int, int)
{
  tvTransactionRowsChanged(parent);
}

/*
 * Whenever the user presses DEL over the Transaction TreeView, we:
 * - Delete the package if it's bellow of "To be removed" or "To be installed" parent;
 * - Delete all the parent's packages if the user clicked in "To be removed" or "To be installed" items.
 */
void MainWindow::onPressDelete()
{
  QTreeView *tvTransaction =
      ui->twProperties->widget(ctn_TABINDEX_TRANSACTION)->findChild<QTreeView*>("tvTransaction");

  if (tvTransaction->hasFocus())
  {
    if(tvTransaction->currentIndex() == getRemoveTransactionParentItem()->index()){
      removePackagesFromRemoveTransaction();
    }
    else if(tvTransaction->currentIndex() == getInstallTransactionParentItem()->index()){
      removePackagesFromInstallTransaction();
    }
    else
    {
      for(int c=tvTransaction->selectionModel()->selectedIndexes().count()-1; c>=0; c--)
      {
        const QModelIndex mi = tvTransaction->selectionModel()->selectedIndexes().at(c);
        if (m_modelTransaction->itemFromIndex(mi)->parent() != 0)
        {
          m_modelTransaction->removeRow(mi.row(), mi.parent());
        }
      }
    }

    changeTransactionActionsState();
  }
}

/*
 * Checks if some SU utility is available...
 * Returns false if not!
 */
bool MainWindow::isSUAvailable()
{
  if (WMHelper::getSUCommand() == ctn_NO_SU_COMMAND){
    QMessageBox::critical( 0, StrConstants::getApplicationName(),
                           StrConstants::getErrorNoSuCommand() +
                           "\n" + StrConstants::getYoullNeedSuFrontend());
    return false;
  }
  else
    return true;
}

/*
 * Checks if Internet connection is up/down
 */
bool MainWindow::isInternetAvailable()
{
  bool res=true;

  //Test if Internet access exists
  if (!UnixCommand::hasInternetConnection())
  {
    QMessageBox::critical(this, StrConstants::getError(), StrConstants::getInternetUnavailableError());
    res=false;
  }

  return res;
}

/*
 * Does a repository sync with "pkg update -f" !
 */
void MainWindow::doCheckUpdates()
{
  if (!isInternetAvailable()) return;

  m_commandExecuting = ectn_CHECK_UPDATES;
  disableTransactionActions();
  m_unixCommand = new UnixCommand(this);

  QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
  QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                   this, &MainWindow::actionsProcessReadOutput);
  QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                   this, &MainWindow::actionsProcessFinished);
  QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                   this, &MainWindow::actionsProcessRaisedError);

  QString command = QStringLiteral("env ASSUME_ALWAYS_YES=yes ") + ctn_PKG_BIN + QStringLiteral(" update -f");
  m_unixCommand->executeCommand(command);
}

/*
 * Does a pkg lock on the selected packages
 */
void MainWindow::doLock()
{
  QModelIndexList selectedRows = ui->tvPackages->selectionModel()->selectedRows();

  if (selectedRows.count() == 1)
  {
    const PackageRepository::PackageData*const package = m_packageModel->getData(selectedRows.at(0));
    if (package)
    {
      m_commandExecuting = ectn_LOCK;
      disableTransactionActions();
      m_unixCommand = new UnixCommand(this);

      QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
      QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                       this, &MainWindow::actionsProcessReadOutput);
      QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                       this, &MainWindow::actionsProcessFinished);
      QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                       this, &MainWindow::actionsProcessRaisedError);

      QString command = ctn_PKG_BIN + " lock -y " + package->name;
      m_unixCommand->executeCommand(command);
    }
  }
}

/*
 * Does a pkg unlock on the selected packages
 */
void MainWindow::doUnlock()
{
  QModelIndexList selectedRows = ui->tvPackages->selectionModel()->selectedRows();

  if (selectedRows.count() == 1)
  {
    const PackageRepository::PackageData*const package = m_packageModel->getData(selectedRows.at(0));
    if (package)
    {
      m_commandExecuting = ectn_UNLOCK;
      disableTransactionActions();
      m_unixCommand = new UnixCommand(this);

      QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
      QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                       this, &MainWindow::actionsProcessReadOutput);
      QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                       this, &MainWindow::actionsProcessFinished);
      QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                       this, &MainWindow::actionsProcessRaisedError);

      QString command = ctn_PKG_BIN + " unlock -y " + package->name;
      m_unixCommand->executeCommand(command);
    }
  }
}

/*
 * doSystemUpgrade shared code ...
 */
void MainWindow::prepareSystemUpgrade()
{
  m_systemUpgradeDialog = false;

  m_lastCommandList.clear();
  m_lastCommandList.append(ctn_PKG_BIN + " upgrade;");
  m_lastCommandList.append("echo -e;");
  m_lastCommandList.append("read -n1 -p \"" + StrConstants::getPressAnyKey() + "\"");

  m_unixCommand = new UnixCommand(this);

  QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
  QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                   this, &MainWindow::actionsProcessReadOutput);
  QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                   this, &MainWindow::actionsProcessFinished);
  QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                   this, &MainWindow::actionsProcessRaisedError);

  disableTransactionActions();
}

/*
 * Prepares the Package::getTargetUpgradeList() transaction info in a separate thread
 */
void MainWindow::prepareTargetUpgradeList(const QString& pkgName, CommandExecuting type)
{
  clearTabOutput();
  writeToTabOutput(QLatin1String("<b>") + StrConstants::getCollectingTransactionData() +
                       QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);
  qApp->processEvents();

  QFuture<TransactionInfo> f;
  f = QtConcurrent::run(getTargetUpgradeList, pkgName);

  disconnect(&g_fwTargetUpgradeList, &QFutureWatcherBase::finished, this, &MainWindow::doInstall);
  disconnect(&g_fwTargetUpgradeList, &QFutureWatcherBase::finished, this, &MainWindow::doRemoveAndInstall);
  disconnect(&g_fwTargetUpgradeList, SIGNAL(finished()), this, SLOT(doSystemUpgrade()));

  if (type == ectn_INSTALL)
  {
    toggleSystemActions(false);
    ui->actionCommit->setEnabled(false);
    ui->actionCancel->setEnabled(false);

    connect(&g_fwTargetUpgradeList, &QFutureWatcherBase::finished, this, &MainWindow::doInstall);
  }
  else if (type == ectn_REMOVE_INSTALL)
  {
    toggleSystemActions(false);
    ui->actionCommit->setEnabled(false);
    ui->actionCancel->setEnabled(false);

    connect(&g_fwTargetUpgradeList, &QFutureWatcherBase::finished, this, &MainWindow::doRemoveAndInstall);
  }
  else if (type == ectn_SYSTEM_UPGRADE)
  {
    toggleSystemActions(false);

    connect(&g_fwTargetUpgradeList, SIGNAL(finished()), this, SLOT(doSystemUpgrade()));
  }

  g_fwTargetUpgradeList.setFuture(f);
}

/*
 * Prepares the 'pkg upgrade' transaction information
 */
void MainWindow::doPreSystemUpgrade()
{
  prepareTargetUpgradeList();
}

/*
 * Issues a killall pkg command to stop the running pkg process
 */
void MainWindow::stopTransaction()
{
  m_unixCommand->cancelProcess();
}

/*
 * Does a system upgrade with "pacman -Su" !
 */
void MainWindow::doSystemUpgrade(SystemUpgradeOptions systemUpgradeOptions)
{
  if (!isInternetAvailable()) return;

  if (m_systemUpgradeDialog) return;

  if(m_callSystemUpgrade && m_numberOfOutdatedPackages == 0)
  {
    m_callSystemUpgrade = false;
    return;
  }
  else if (m_callSystemUpgradeNoConfirm && m_numberOfOutdatedPackages == 0)
  {
    m_callSystemUpgrade = false;
    return;
  }

  if (!isSUAvailable()) return;

  //Shows a dialog indicating the targets needed to be retrieved and asks for the user's permission.
  TransactionInfo ti = g_fwTargetUpgradeList.result(); //Package::getTargetUpgradeList();
  QStringList *targets = ti.packages;

  //There are no new updates to install!
  if (targets->count() == 0 && m_outdatedStringList->count() == 0)
  {
    clearTabOutput();
    writeToTabOutputExt(QLatin1String("<b>") + StrConstants::getNoNewUpdatesAvailable() + QLatin1String("</b>"));
    return;
  }
  else if (targets->count() == 0 && m_outdatedStringList->count() > 0)
  {
    //This is a bug and should be shown to the user!
    clearTabOutput();
    //writeToTabOutputExt(UnixCommand::getTargetUpgradeList());
    QString out = UnixCommand::getTargetUpgradeList();
    splitOutputStrings(out);
    return;
  }

  QString list;

  for(QString target: *targets)
  {
    list = list + target + "\n";
  }

  //User already confirmed all updates in the notifier window!
  if (systemUpgradeOptions == ectn_NOCONFIRM_OPT)
  {
    prepareSystemUpgrade();

    m_commandExecuting = ectn_SYSTEM_UPGRADE;

    QString command;
    command = ctn_PKG_BIN + QLatin1String(" upgrade -y");

    m_unixCommand->executeCommand(command);
    m_commandQueued = ectn_NONE;
  }
  else
  {
    //Let's build the system upgrade transaction dialog...
    /*totalDownloadSize = totalDownloadSize / 1024;
      QString ds = Package::kbytesToSize(totalDownloadSize);*/
    QString ds = ti.sizeToDownload;

    TransactionDialog question(this);

    if(targets->count()==1)
      question.setText(StrConstants::getRetrievePackage() +
                       QLatin1String("\n\n") + StrConstants::getTotalDownloadSize().arg(ds).remove(QLatin1String(" KB")));
    else
      question.setText(StrConstants::getRetrievePackages(targets->count()) +
                       QLatin1String("\n\n") + StrConstants::getTotalDownloadSize().arg(ds).remove(QLatin1String(" KB")));

    question.setWindowTitle(StrConstants::getConfirmation());
    question.setInformativeText(StrConstants::getConfirmationQuestion());
    question.setDetailedText(list);

    m_systemUpgradeDialog = true;

    writeToTabOutput(QLatin1String("<b>") + StrConstants::getCommandFinishedOK() +
                         QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);

    int result = question.exec();

    if(result == QDialogButtonBox::Yes || result == QDialogButtonBox::AcceptRole)
    {
      prepareSystemUpgrade();

      if (result == QDialogButtonBox::Yes)
      {
        m_commandExecuting = ectn_SYSTEM_UPGRADE;
        QStringList params;

        if (question.isBootEnvChecked())
        {
          QString beName = QDateTime::currentDateTime().toString(QLatin1String("yyMMdd-hhmmss"));
          params << UnixCommand::getShell();
          params << "-c";
          params << "bectl create " + beName + "; " + ctn_PKG_BIN + " upgrade -y";
        }
        else
        {
          params << ctn_PKG_BIN;
          params << "upgrade";
          params << "-y";
        }

        m_unixCommand->executeCommand(params);
        m_commandQueued = ectn_NONE;
      }
      else if (result == QDialogButtonBox::AcceptRole)
      {
        m_commandExecuting = ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL;
        m_unixCommand->runCommandInTerminal(m_lastCommandList);
        m_commandQueued = ectn_NONE;
      }
    }
    else if (result == QDialogButtonBox::No)
    {
      m_systemUpgradeDialog = false;
      enableTransactionActions();
      toggleSystemActions(true);
    }
  }
}

/*
 * Prepares the Remove and Install transaction information
 */
void MainWindow::doPreRemoveAndInstall()
{
  QString listOfInstallTargets = getTobeInstalledPackages();
  prepareTargetUpgradeList(listOfInstallTargets, ectn_REMOVE_INSTALL);
}

/*
 * Removes and Installs all selected packages
 */
void MainWindow::doRemoveAndInstall()
{
  QString listOfRemoveTargets = getTobeRemovedPackages();
  QString removeList;
  QString allLists;
  TransactionDialog question(this);
  QString dialogText;

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList removeTargets = listOfRemoveTargets.split(" ", QString::SkipEmptyParts);
#else
  QStringList removeTargets = listOfRemoveTargets.split(" ", Qt::SkipEmptyParts);
#endif 
  
  for(QString target: removeTargets)
  {
    removeList = removeList + StrConstants::getRemove() + " "  + target + "\n";
  }

  QString listOfInstallTargets = getTobeInstalledPackages();
  TransactionInfo ti = g_fwTargetUpgradeList.result();
  QStringList *installTargets = ti.packages;
  QString ds = ti.sizeToDownload;

  if (ti.sizeToDownload == "0") ds = "0.00 Bytes";

  QString installList;

  for(QString target: *installTargets)
  {
    installList = installList + StrConstants::getInstall() + " " + target + "\n";
  }

  allLists.append(removeList);
  allLists.append(installList);

  if (removeTargets.count() == 1)
  {
    dialogText = StrConstants::getRemovePackage() + "\n";
  }
  else if (removeTargets.count() > 1)
  {
    dialogText = StrConstants::getRemovePackages(removeTargets.count()) + "\n";
  }
  if (installTargets->count() == 1)
  {
    dialogText += StrConstants::getRetrievePackage() +
      "\n\n" + StrConstants::getTotalDownloadSize().arg(ds).remove(" KB");
  }
  else if (installTargets->count() > 1)
  {
    dialogText += StrConstants::getRetrievePackages(installTargets->count()) +
      "\n\n" + StrConstants::getTotalDownloadSize().arg(ds).remove(" KB");
  }

  question.setText(dialogText);
  question.setWindowTitle(StrConstants::getConfirmation());
  question.setInformativeText(StrConstants::getConfirmationQuestion());
  question.setDetailedText(allLists);
  question.uncheckBootEnv();

  writeToTabOutput(QLatin1String("<b>") + StrConstants::getCommandFinishedOK() +
                       QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);

  int result = question.exec();

  if(result == QDialogButtonBox::Yes || result == QDialogButtonBox::AcceptRole)
  {
    disableTransactionButtons();

    QStringList params;

    if (question.isBootEnvChecked())
    {
      QString beName = QDateTime::currentDateTime().toString(QLatin1String("yyMMdd-hhmmss"));
      params << UnixCommand::getShell();
      params << "-c";
      params << "bectl create " + beName + "; " +
        ctn_PKG_BIN + " remove -y " + listOfRemoveTargets + "; " +
        ctn_PKG_BIN + " install -f -y " + listOfInstallTargets;
    }
    else
    {
      params << UnixCommand::getShell();
      params << "-c";
      params << ctn_PKG_BIN + " remove -y " + listOfRemoveTargets + "; " +
        ctn_PKG_BIN + " install -f -y " + listOfInstallTargets;
    }

    m_lastCommandList.clear();
    m_lastCommandList.append(ctn_PKG_BIN + " remove " + listOfRemoveTargets + ";");
    m_lastCommandList.append(ctn_PKG_BIN + " install -f " + listOfInstallTargets + ";");
    m_lastCommandList.append("echo -e;");
    m_lastCommandList.append("read -n1 -p \"" + StrConstants::getPressAnyKey() + "\"");

    m_unixCommand = new UnixCommand(this);

    QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                     this, &MainWindow::actionsProcessReadOutput);
    QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                     this, &MainWindow::actionsProcessFinished);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                     this, &MainWindow::actionsProcessRaisedError);

    disableTransactionActions();

    if (result == QDialogButtonBox::Yes)
    {
      m_commandExecuting = ectn_REMOVE_INSTALL;
      m_unixCommand->executeCommand(params);
    }
    else if (result == QDialogButtonBox::AcceptRole)
    {
      m_commandExecuting = ectn_RUN_IN_TERMINAL;
      m_unixCommand->runCommandInTerminal(m_lastCommandList);
    }
  }
  else
  {
    m_commandExecuting = ectn_NONE;
    enableTransactionActions();
  }
}

/*
 * Removes ALL the packages selected by the user with "pacman -Rcs (CASCADE)" !
 */
void MainWindow::doRemove()
{
  clearTabOutput();
  writeToTabOutput(QLatin1String("<b>") + StrConstants::getCollectingTransactionData() +
                       QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);
  qApp->processEvents();

  QString listOfTargets = getTobeRemovedPackages();
  QStringList *_targets = Package::getTargetRemovalList(listOfTargets);
  listOfTargets = "";
  QString list;

  for(QString target: *_targets)
  {
    list = list + target + "\n";
    listOfTargets += target + " ";
  }

  TransactionDialog question(this);

  //Shows a dialog indicating the targets which will be removed and asks for the user's permission.
  if(_targets->count()==1)
  {
    question.setText(StrConstants::getRemovePackage());
  }
  else
    question.setText(StrConstants::getRemovePackages(_targets->count()));

  if (getNumberOfTobeRemovedPackages() < _targets->count())
    question.setWindowTitle(StrConstants::getWarning());
  else
    question.setWindowTitle(StrConstants::getConfirmation());

  question.setInformativeText(StrConstants::getConfirmationQuestion());
  question.setDetailedText(list);
  question.uncheckBootEnv();

  writeToTabOutput(QLatin1String("<b>") + StrConstants::getCommandFinishedOK() +
                       QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);

  int result = question.exec();

  if(result == QDialogButtonBox::Yes || result == QDialogButtonBox::AcceptRole)
  {
    QString command;
    QStringList params;

    if (question.isBootEnvChecked())
    {
      QString beName = QDateTime::currentDateTime().toString(QLatin1String("yyMMdd-hhmmss"));
      params << UnixCommand::getShell();
      params << "-c";
      params << "bectl create " + beName + "; " + ctn_PKG_BIN + " remove -R -y " + listOfTargets;
    }
    else
    {
      command = ctn_PKG_BIN + " remove -R -y " + listOfTargets;
    }

    m_lastCommandList.clear();
    m_lastCommandList.append(ctn_PKG_BIN + " remove -R " + listOfTargets + ";");
    m_lastCommandList.append("echo -e;");
    m_lastCommandList.append("read -n1 -p \"" + StrConstants::getPressAnyKey() + "\"");

    m_unixCommand = new UnixCommand(this);

    QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                     this, &MainWindow::actionsProcessReadOutput);
    QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                     this, &MainWindow::actionsProcessFinished);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                     this, &MainWindow::actionsProcessRaisedError);

    disableTransactionActions();

    if (result == QDialogButtonBox::Yes)
    {
      m_commandExecuting = ectn_REMOVE;
      if (params.count() > 0)
        m_unixCommand->executeCommand(params);
      else
        m_unixCommand->executeCommand(command);
    }

    if (result == QDialogButtonBox::AcceptRole)
    {
      m_commandExecuting = ectn_RUN_IN_TERMINAL;
      m_unixCommand->runCommandInTerminal(m_lastCommandList);
    }
  }
  else
  {
    m_commandExecuting = ectn_NONE;
    enableTransactionActions();
  }
}

/*
 * Prepares the Install transaction information
 */
void MainWindow::doPreInstall()
{
  QString listOfTargets = getTobeInstalledPackages();
  prepareTargetUpgradeList(listOfTargets, ectn_INSTALL);
}

/*
 * Installs ALL the packages selected by the user with "pacman -S (INCLUDING DEPENDENCIES)" !
 */
void MainWindow::doInstall()
{
  QString listOfTargets = getTobeInstalledPackages();

  TransactionInfo ti = g_fwTargetUpgradeList.result(); //Package::getTargetUpgradeList(listOfTargets);
  QStringList *targets = ti.packages;

  if (targets->count() == 0)
  {
    QMessageBox::critical( 0, StrConstants::getApplicationName(),
                           StrConstants::getPkgNotAvailable());
    enableTransactionActions();
    return;
  }

  QString list;
  QString ds = ti.sizeToDownload;

  if (ti.sizeToDownload == "0") ds = "0.00 Bytes";

  TransactionDialog question(this);

  for(QString target: *targets)
  {
    list = list + target + "\n";
  }

  if(targets->count()==1)
  {
    question.setText(StrConstants::getRetrievePackage() +
                          "\n\n" + StrConstants::getTotalDownloadSize().arg(ds).remove(" KB"));
  }
  else if (targets->count() > 1)
    question.setText(StrConstants::getRetrievePackages(targets->count()) +
                     "\n\n" + StrConstants::getTotalDownloadSize().arg(ds).remove(" KB"));

  question.setWindowTitle(StrConstants::getConfirmation());
  question.setInformativeText(StrConstants::getConfirmationQuestion());
  question.setDetailedText(list);
  question.uncheckBootEnv();

  writeToTabOutput(QLatin1String("<b>") + StrConstants::getCommandFinishedOK() +
                       QLatin1String("</b><br>"), ectn_DONT_TREAT_URL_LINK);

  int result = question.exec();

  if(result == QDialogButtonBox::Yes || result == QDialogButtonBox::AcceptRole)
  {
    disableTransactionButtons();

    QStringList params;
    QString command;

    if (question.isBootEnvChecked())
    {
      QString beName = QDateTime::currentDateTime().toString(QLatin1String("yyMMdd-hhmmss"));
      params << UnixCommand::getShell();
      params << "-c";
      params << "bectl create " + beName + "; " + ctn_PKG_BIN + " install -f -y " + listOfTargets;
    }
    else
    {
      command = ctn_PKG_BIN + " install -f -y " + listOfTargets;
    }

    m_lastCommandList.clear();
    m_lastCommandList.append(ctn_PKG_BIN + " install -f " + listOfTargets + ";");
    m_lastCommandList.append("echo -e;");
    m_lastCommandList.append("read -n1 -p \"" + StrConstants::getPressAnyKey() + "\"");

    disableTransactionActions();
    m_unixCommand = new UnixCommand(this);

    QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
    QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                     this, &MainWindow::actionsProcessFinished);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                     this, &MainWindow::actionsProcessReadOutput);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                     this, &MainWindow::actionsProcessRaisedError);

    if (result == QDialogButtonBox::Yes)
    {
      m_commandExecuting = ectn_INSTALL;
      if (params.count() > 0)
        m_unixCommand->executeCommand(params);
      else
        m_unixCommand->executeCommand(command);
    }
    else if (result == QDialogButtonBox::AcceptRole)
    {
      m_commandExecuting = ectn_RUN_IN_TERMINAL;
      m_unixCommand->runCommandInTerminal(m_lastCommandList);
    }
  }
  else
  {
    m_commandExecuting = ectn_NONE;
    enableTransactionActions();
  }
}

/*
 * Installs ALL the packages manually selected by the user with "pacman -U (INCLUDING DEPENDENCIES)" !
 */
void MainWindow::doInstallLocalPackages()
{
  QString listOfTargets;
  QString list;
  QFileInfo fi;

  for(QString target: m_packagesToInstallList)
  {
    fi.setFile(target);
    list = list + fi.fileName() + "\n";
  }

  for(QString pkgToInstall: m_packagesToInstallList)
  {
    listOfTargets += pkgToInstall + " ";
  }

  TransactionDialog question(this);
  question.setWindowTitle(StrConstants::getConfirmation());
  question.setInformativeText(StrConstants::getConfirmationQuestion());
  question.setDetailedText(list);

  if(m_packagesToInstallList.count()==1)
  {
    if (m_packagesToInstallList.at(0).indexOf("HoldPkg was found in") != -1)
    {
      QMessageBox::warning(
            this, StrConstants::getAttention(), StrConstants::getWarnHoldPkgFound(), QMessageBox::Ok);
      return;
    }
    else question.setText(StrConstants::getRetrievePackage());
  }
  else
    question.setText(StrConstants::getRetrievePackages(m_packagesToInstallList.count()));

  int result = question.exec();

  if(result == QDialogButtonBox::Yes || result == QDialogButtonBox::AcceptRole)
  {
    QString command;
    command = "pacman -U --force --noconfirm " + listOfTargets;

    m_lastCommandList.clear();
    m_lastCommandList.append("pacman -U --force " + listOfTargets + ";");
    m_lastCommandList.append("echo -e;");
    m_lastCommandList.append("read -n1 -p \"" + StrConstants::getPressAnyKey() + "\"");

    m_commandExecuting = ectn_INSTALL;
    disableTransactionActions();
    m_unixCommand = new UnixCommand(this);

    QObject::connect(m_unixCommand, &UnixCommand::started, this, &MainWindow::actionsProcessStarted);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardOutput,
                     this, &MainWindow::actionsProcessReadOutput);
    QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                     this, &MainWindow::actionsProcessFinished);
    QObject::connect(m_unixCommand, &UnixCommand::readyReadStandardError,
                     this, &MainWindow::actionsProcessRaisedError);

    if (result == QDialogButtonBox::Yes)
    {
      m_commandExecuting = ectn_INSTALL;
      m_unixCommand->executeCommand(command);
    }
    else if (result == QDialogButtonBox::AcceptRole)
    {
      m_commandExecuting = ectn_RUN_IN_TERMINAL;
      m_unixCommand->runCommandInTerminal(m_lastCommandList);
    }
  }
}

/*
 * Clears the local package cache using "pacman -Sc"
 */
void MainWindow::doCleanCache()
{
  int res = QMessageBox::question(this, StrConstants::getConfirmation(),
                                  StrConstants::getCleanCacheConfirmation(),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

  if (res == QMessageBox::Yes)
  {
    disableTransactionActions();
    m_commandExecuting = ectn_CLEAN_CACHE;
    m_unixCommand = new UnixCommand(this);

    QObject::connect(m_unixCommand, qOverload<int, QProcess::ExitStatus>(&UnixCommand::finished),
                     this, &MainWindow::actionsProcessFinished);

    clearTabOutput();
    writeToTabOutputExt("<b>" + StrConstants::getCleaningPackageCache() + "</b>");
    QString command = ctn_PKG_BIN + " clean -a -y";
    m_unixCommand->executeCommand(command);
  }
}

/*
 * Disables all Transaction related actions
 */
void MainWindow::disableTransactionActions()
{
  toggleSystemActions(false);
  toggleTransactionActions(false);
}

/*
 * Enables all Transaction related actions
 */
void MainWindow::enableTransactionActions()
{
  toggleTransactionActions(true);
}

/*
 * Sets with the given boolean the state of all Transaction related actions
 */
void MainWindow::toggleTransactionActions(const bool value)
{
  bool state = isThereAPendingTransaction();
  if (value == true && state == true)
  {
    ui->actionCommit->setEnabled(true);
    ui->actionCancel->setEnabled(true);
    m_actionSwitchToRemoteSearch->setEnabled(true);
    ui->actionCheckUpdates->setEnabled(false);
    ui->actionSystemUpgrade->setEnabled(false);
  }
  else if (value == true && state == false)
  {
    ui->actionCommit->setEnabled(false);
    ui->actionCancel->setEnabled(false);
    m_actionSwitchToRemoteSearch->setEnabled(true);
    ui->actionCheckUpdates->setEnabled(true);

    if (value == true && m_outdatedStringList->count() > 0)
      ui->actionSystemUpgrade->setEnabled(true);
  }
  else if (value == false && state == false)
  {
    m_actionSwitchToRemoteSearch->setEnabled(false);
    ui->actionCheckUpdates->setEnabled(false);
    ui->actionSystemUpgrade->setEnabled(false);
  }

  ui->actionInstall->setEnabled(value);
  m_actionInstallPkgUpdates->setEnabled(value);
  ui->actionRemoveTransactionItem->setEnabled(value);
  ui->actionRemoveTransactionItems->setEnabled(value);
  ui->actionRemove->setEnabled(value);
  ui->actionPacmanLogViewer->setEnabled(value);
  ui->actionRepositoryEditor->setEnabled(value);  
  m_actionSysInfo->setEnabled(value);
  ui->actionGetNews->setEnabled(value);
  ui->actionCleanLocalCache->setEnabled(value);
  ui->actionOpenRootTerminal->setEnabled(value);
  ui->actionHelpUsage->setEnabled(value);
  ui->actionDonate->setEnabled(value);
  ui->actionHelpAbout->setEnabled(value);
  m_actionPackageInfo->setEnabled(value);
  ui->actionFindFileInPackage->setEnabled(value);
  m_actionLockPackage->setEnabled(value);
  m_actionUnlockPackage->setEnabled(value);

  //Search menu
  ui->actionSearchByFile->setEnabled(value);
  ui->actionSearchByName->setEnabled(value);
  ui->actionSearchByDescription->setEnabled(value);

  m_leFilterPackage->setEnabled(value);
  disconnect(ui->twProperties, &QTabWidget::currentChanged, this, &MainWindow::changedTabIndex);
  ui->twProperties->setTabEnabled(ctn_TABINDEX_INFORMATION, value);
  ui->twProperties->setTabEnabled(ctn_TABINDEX_FILES, value);
  connect(ui->twProperties, &QTabWidget::currentChanged, this, &MainWindow::changedTabIndex);

  //We have to toggle the combobox groups as well
  if (m_initializationCompleted) ui->twGroups->setEnabled(value);
}

/*
 * Enables / Disables important interface actions
 */
void MainWindow::toggleSystemActions(const bool value)
{
  if (value == true && m_commandExecuting != ectn_NONE) return;

  bool state = isThereAPendingTransaction();

  if (isRemoteSearchSelected() && !state)
  {
    ui->actionCheckUpdates->setEnabled(true);
  }
  else if ((value == true && !state) || value == false)
  {
    ui->actionCheckUpdates->setEnabled(value);
  }

  ui->actionInstallLocalPackage->setEnabled(value);
  ui->actionGetNews->setEnabled(value);
  ui->actionCleanLocalCache->setEnabled(value);
  m_actionSwitchToRemoteSearch->setEnabled(value);

  if (value == true && !state && m_outdatedStringList->count() > 0)
    ui->actionSystemUpgrade->setEnabled(true);
  else
    ui->actionSystemUpgrade->setEnabled(false);
}

/*
 * Triggers the especific methods that need to be called given the packages in the transaction
 */
void MainWindow::commitTransaction()
{
  if (!isInternetAvailable()) return;

  //Are there any remove actions to be commited?
  if(getRemoveTransactionParentItem()->rowCount() > 0 && getInstallTransactionParentItem()->rowCount() > 0)
  {
    doPreRemoveAndInstall();
  }
  else if(getRemoveTransactionParentItem()->rowCount() > 0)
  {
    doRemove();
  }
  else if(getInstallTransactionParentItem()->rowCount() > 0)
  {
    doPreInstall();
  }
}

/*
 * Clears the transaction treeview
 */
void MainWindow::cancelTransaction()
{
  int res = QMessageBox::question(this,
                        StrConstants::getConfirmation(),
                        StrConstants::getCancelTransactionConfirmation(),
                        QMessageBox::Yes|QMessageBox::No,
                        QMessageBox::No);

  if(res == QMessageBox::Yes)
  {
    clearTransactionTreeView();
  }
}

/*
 * This SLOT is called whenever Pacman's process has just started execution
 */
void MainWindow::actionsProcessStarted()
{
  m_progressWidget->setValue(0);
  m_progressWidget->setMaximum(100);

  clearTabOutput();

  //First we output the name of action we are starting to execute!

  if (m_commandExecuting == ectn_CHECK_UPDATES)
  {
    writeToTabOutput("<b>" + StrConstants::getCheckUpdates() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_SYSTEM_UPGRADE || m_commandExecuting == ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL)
  {
    writeToTabOutput("<b>" + StrConstants::getSystemUpgrade() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_LOCK)
  {
    writeToTabOutput("<b>" + StrConstants::getLockingPackage() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_UNLOCK)
  {
    writeToTabOutput("<b>" + StrConstants::getUnlockingPackage() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_REMOVE)
  {
    writeToTabOutput("<b>" + StrConstants::getRemovingPackages() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_INSTALL)
  {
    writeToTabOutput("<b>" + StrConstants::getInstallingPackages() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_REMOVE_INSTALL)
  {
    writeToTabOutput("<b>" + StrConstants::getRemovingAndInstallingPackages() + "</b><br><br>");
  }
  else if (m_commandExecuting == ectn_RUN_IN_TERMINAL)
  {
    writeToTabOutput("<b>" + StrConstants::getRunningCommandInTerminal() + "</b><br><br>");
  }

  QString msg = m_unixCommand->readAllStandardOutput();
  msg = msg.trimmed();

  if (!msg.isEmpty())
  {
    writeToTabOutputExt(msg);
  }
}

/*
 * This SLOT is called when Pacman's process has finished execution
 *
 */
void MainWindow::actionsProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  m_toolButtonStopTransaction->setVisible(false);

  //bool bRefreshGroups = true;
  m_progressWidget->close();
  ui->twProperties->setTabText(ctn_TABINDEX_OUTPUT, StrConstants::getTabOutputName());

  //mate-terminal is returning code 255 sometimes...
  if ((exitCode == 0 || exitCode == 255) && exitStatus == QProcess::NormalExit)
  {
    //First, we empty the tabs cache!
    m_cachedPackageInInfo = "";
    m_cachedPackageInFiles = "";

    writeToTabOutputExt("<br><b>" +
                     StrConstants::getCommandFinishedOK() + "</b><br>");
  }
  else
  {
    writeToTabOutputExt("<br><b>" +
                     StrConstants::getCommandFinishedWithErrors() + "</b><br>");
  }

  if (m_commandExecuting == ectn_CLEAN_CACHE)
  {
    m_unixCommand->removeTemporaryFile();
    resetTransaction();
    return;
  }

  if (m_commandQueued == ectn_NONE)
  {
    if(exitCode == 0 || exitCode == 255) //mate-terminal is returning code 255 sometimes...
    {
      clearTransactionTreeView();

      //After the command, we can refresh the package list, so any change can be seem.
      if (m_commandExecuting == ectn_CHECK_UPDATES)
      {
        //Retrieves the RSS News from respective Distro site...
        if (isRemoteSearchSelected())
        {
          m_leFilterPackage->clear();
          refreshDistroNews(true, false);
          m_commandExecuting = ectn_LOCAL_PKG_REFRESH;
          remoteSearchClicked();
        }
        else
        {
          m_leFilterPackage->clear();
          refreshDistroNews(true, false);
          metaBuildPackageList();
          connect(this, &MainWindow::buildPackageListDone, this, &MainWindow::resetTransaction);
        }
      }
      else if (m_commandExecuting == ectn_SYSTEM_UPGRADE ||
               m_commandExecuting == ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL)
      {
        if (isRemoteSearchSelected())
        {
          m_leFilterPackage->clear();
          m_commandExecuting = ectn_LOCAL_PKG_REFRESH;
          remoteSearchClicked();
        }
        else
        {
          m_leFilterPackage->clear();
          metaBuildPackageList();
          connect(this, &MainWindow::buildPackageListDone, this, &MainWindow::resetTransaction);
        }
      }
      else
      {
        if (isRemoteSearchSelected())
        {
          m_leFilterPackage->clear();
          m_commandExecuting = ectn_LOCAL_PKG_REFRESH;
          remoteSearchClicked();
        }
        else
        {
          m_leFilterPackage->clear();
          metaBuildPackageList();
          connect(this, &MainWindow::buildPackageListDone, this, &MainWindow::resetTransaction);
        }
      }
    }
  }

  if (exitCode != 0)
  {
    resetTransaction();
  }
}

/*
 * Garbage collects some transaction objects and reset others
 */
void MainWindow::resetTransaction()
{
  enableTransactionActions();

  if (m_unixCommand != nullptr)
  {
    m_unixCommand->removeTemporaryFile();
    delete m_unixCommand;
    m_unixCommand = nullptr;
  }

  m_commandExecuting = ectn_NONE;
  disconnect(this, &MainWindow::buildPackageListDone, this, &MainWindow::resetTransaction);
}

/*
 * This SLOT is called whenever Pacman's process has something to output to Standard out
 */
void MainWindow::actionsProcessReadOutput()
{
  QString msg = m_unixCommand->readAllStandardOutput();
  splitOutputStrings(msg);
}

/*
 * Searches the given msg for a series of verbs that a Pacman transaction may produce
 */
bool MainWindow::searchForKeyVerbs(const QString &msg)
{
  return (msg.contains(QRegularExpression("Fetching ")) ||
          msg.contains(QRegularExpression("Updating ")) ||
          msg.contains(QRegularExpression("Processing ")));
}

/*
 * Processes the output of the 'pkg process' so we can update percentages and messages at real time
 */
void MainWindow::parsePkgProcessOutput(const QString &pMsg)
{  
  if (m_commandExecuting == ectn_RUN_IN_TERMINAL ||
      m_commandExecuting == ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL) return;

  bool continueTesting = false;
  QString perc;
  QString msg = pMsg;
  QString progressRun;
  QString progressEnd;

  msg.remove(QRegularExpression(".+\\[Y/n\\].+"));

  //Let's remove color codes from strings...
  msg.remove("\033[0;1m");
  msg.remove("\033[0m");
  msg.remove("[1;33m");
  msg.remove("[00;31m");
  msg.remove("\033[1;34m");
  msg.remove("\033[0;1m");
  msg.remove("c");
  msg.remove("C");
  msg.remove("");
  msg.remove("[m[0;37m");
  msg.remove("o");
  msg.remove("[m");
  msg.remove("[m");
  msg.remove("[32m");
  msg.remove("[33m");
  msg.remove(";37m");
  msg.remove("[c");
  msg.remove("[mo");

  //qDebug() << "_treat: " << msg;

  progressRun = "%";
  progressEnd = "100%";

  //If it is a percentage, we are talking about curl output...
  if(msg.indexOf(progressEnd) != -1)
  {
    perc = "100%";
    if (!m_progressWidget->isVisible())
    {
      m_progressWidget->show();
      if (m_commandExecuting != ectn_CHECK_UPDATES)
        m_toolButtonStopTransaction->setVisible(true);
    }

    m_progressWidget->setValue(100);
    continueTesting = true;
  }

  if (msg.indexOf(progressRun) != -1 || continueTesting)
  {
    int p = msg.indexOf("%");
    if (p == -1 || (p-3 < 0) || (p-2 < 0)) return; //Guard!

    if (msg.at(p-3).isSpace())
      perc = msg.mid(p-2, 3).trimmed();

    //qDebug() << "percentage is: " << perc;

    QString target, pName;
    QRegularExpression regex;
    QRegularExpressionMatch match;

    if (msg.contains("Fetching") && !msg.contains(QRegularExpression("B/s")))
    {
      int p = msg.indexOf(":");
      if (p == -1) return; //Guard!

      target = msg.left(p).remove("Fetching").trimmed();
      pName = target;
      regex.setPattern("\\[\\d+/\\d+\\]\\s\\s(.*)");
      match = regex.match(target);
      if (match.hasMatch())
      {
        pName = match.captured(1);
      }

      if(!textInTabOutput(pName))
      {
        if (!msg.contains("["))
          writeToTabOutputExt("<b><font color=\"#FF8040\">Fetching " + target + "</font></b>");
        else
          writeToTabOutputExt("<b><font color=\"#B4AB58\">Fetching " + target + "</font></b>");
      }
    }
    else if (msg.contains("Processing"))
    {
      int p = msg.indexOf(":");
      if (p == -1) return; //Guard!

      target = msg.left(p).remove("Processing").trimmed();

      if(!textInTabOutput(target))
        writeToTabOutputExt("Processing " + target + "...");
        //writeToTabOutputExt("<b><font color=\"#4BC413\">Processing " + target + "</font></b>"); //GREEN
    }
    else if (msg.contains("Extracting") && perc == "100%")
    {
      writeToTabOutputExt(msg);
    }

    //Here we print the transaction percentage updating
    if(!perc.isEmpty() && perc.indexOf("%") > 0)
    {
      int percentage = perc.left(perc.size()-1).toInt();
      if (!m_progressWidget->isVisible()){
        m_progressWidget->show();

        if (m_commandExecuting != ectn_CHECK_UPDATES)
            m_toolButtonStopTransaction->setVisible(true);
      }

      m_progressWidget->setValue(percentage);
    }
  }
  //It's another error, so we have to output it
  else
  {      
    if (msg.contains(QRegularExpression("ETA")) ||
      msg.contains(QRegularExpression("KiB")) ||
      msg.contains(QRegularExpression("MiB")) ||
      msg.contains(QRegularExpression("kB/s")) ||
      msg.contains(QRegularExpression("MB/s")) ||
      msg.contains(QRegularExpression("[0-9]+ B")) ||
      msg.contains(QRegularExpression("[0-9]{2}:[0-9]{2}"))) return;

    //Let's supress some annoying string bugs...
    msg.remove(QRegularExpression("\\(process.+"));
    msg.remove(QRegularExpression("Using the fallback.+"));
    msg.remove(QRegularExpression("Gkr-Message:.+"));
    msg.remove(QRegularExpression("kbuildsycoca.+"));
    msg.remove(QRegularExpression("Connecting to deprecated signal.+"));
    msg.remove(QRegularExpression("QVariant.+"));
    msg.remove(QRegularExpression("libGL.+"));
    msg.remove(QRegularExpression("Password.+"));
    msg.remove(QRegularExpression("gksu-run.+"));
    msg.remove(QRegularExpression("GConf Error:.+"));
    msg.remove(QRegularExpression(":: Do you want.+"));
    msg.remove(QRegularExpression("org\\.kde\\."));
    msg.remove(QRegularExpression("QCommandLineParser"));
    msg.remove(QRegularExpression("QCoreApplication.+"));
    msg.remove(QRegularExpression("Detected locale.+"));
    msg.remove(QRegularExpression("Qt depends on a UTF-8 locale.+"));
    msg.remove(QRegularExpression("^for more information\\."));
    msg.remove(QRegularExpression("Fontconfig warning.+"));
    msg.remove(QRegularExpression("reading configurations from.+"));
    msg.remove(QRegularExpression(".+annot load library.+"));
    msg.remove(QRegularExpression("QXcbConnection: XCB error.+"));
    msg.remove(QRegularExpression("qt.qpa.xcb:.+"));
    msg.remove(QRegularExpression("qt5ct: using qt5ct plugin"));

    //Gksu buggy strings
    /*msg.remove(QRegularExpression("couldn't lock.+"));
    msg.remove(QRegularExpression("you should recompile libgtop and dependent applications.+"));
    msg.remove(QRegularExpression("This libgtop was compiled on.+"));
    msg.remove(QRegularExpression("If you see strange problems caused by it.+"));
    msg.remove(QRegularExpression("LibGTop-Server.+"));
    msg.remove(QRegularExpression("received eof.+"));
    msg.remove(QRegularExpression("pid [0-9]+"));*/
    msg = msg.trimmed();

    //std::cout << "debug: " << msg.toLatin1().data() << std::endl;
    QString order;
    int ini = msg.indexOf(QRegularExpression("\\(\\s{0,3}[0-9]{1,4}/[0-9]{1,4}\\) "));
    if (ini == 0)
    {
      int rp = msg.indexOf(")");
      if (rp == -1) return; //Guard!

      order = msg.left(rp+2);
      msg = msg.remove(0, rp+2);
    }

    if (!msg.isEmpty())
    {
      if (msg.contains(QRegularExpression("removing ")) && !textInTabOutput(msg + " "))
      {
        //Does this package exist or is it a proccessOutput buggy string???
        QString pkgName = msg.mid(9).trimmed();

        const PackageRepository::PackageData*const package = m_packageRepo.getFirstPackageByName(pkgName);
        if (pkgName.indexOf("...") != -1 &&
            (package != NULL && package->installed()))
        {
          writeToTabOutputExt("<b><font color=\"#E55451\">" + msg + "</font></b>"); //RED
        }
      }
      else
      {
        QString altMsg = msg;
        writeToTabOutputExt(altMsg); //BLACK
      }
    }
  }

  if(m_commandExecuting == ectn_NONE)
    ui->twProperties->setTabText(ctn_TABINDEX_OUTPUT, StrConstants::getTabOutputName());
}

/*
 * Breaks the output generated by QProcess so we can parse the strings
 * and give a better feedback to our users (including showing percentages)
 *
 * Returns true if the given output was split
 */
bool MainWindow::splitOutputStrings(const QString &output)
{
  bool res = true;
  QString msg = output.trimmed();
  
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
  QStringList msgs = msg.split(QRegularExpression("\\n"), QString::SkipEmptyParts);
#else
  QStringList msgs = msg.split(QRegularExpression("\\n"), Qt::SkipEmptyParts);
#endif

  for (QString m: msgs)
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList m2 = m.split(QRegularExpression("\\(\\s{0,3}[0-9]{1,4}/[0-9]{1,4}\\) "), QString::SkipEmptyParts);
#else
    QStringList m2 = m.split(QRegularExpression("\\(\\s{0,3}[0-9]{1,4}/[0-9]{1,4}\\) "), Qt::SkipEmptyParts);
#endif

    if (m2.count() == 1)
    {
      //Let's try another test... if it doesn't work, we give up.
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
      QStringList maux = m.split(QRegularExpression("%"), QString::SkipEmptyParts);
#else
      QStringList maux = m.split(QRegularExpression("%"), Qt::SkipEmptyParts);
#endif

      if (maux.count() > 1)
      {
        for (QString aux: maux)
        {
          aux = aux.trimmed();
          if (!aux.isEmpty())
          {
            if (aux.at(aux.length()-1).isDigit())
            {
              aux += "%";
            }

            //std::cout << "Error1: " << aux.toLatin1().data() << std::endl;
            parsePkgProcessOutput(aux);
          }
        }
      }
      else if (maux.count() == 1)
      {
        if (!m.isEmpty())
        {
          //std::cout << "Error2: " << m.toLatin1().data() << std::endl;
          parsePkgProcessOutput(m);
        }
      }
    }
    else if (m2.count() > 1)
    {
      for (QString m3: m2)
      {
        if (!m3.isEmpty())
        {
          //std::cout << "Error3: " << m3.toLatin1().data() << std::endl;
          parsePkgProcessOutput(m3);
        }
      }
    }
    else res = false;
  }

  return res;
}

/*
 * This SLOT is called whenever Pacman's process has something to output to Standard error
 */
void MainWindow::actionsProcessRaisedError()
{
  QString msg = m_unixCommand->readAllStandardError();
  splitOutputStrings(msg);
}

/*
 * A helper method which writes the given string to OutputTab's textbrowser
 */
void MainWindow::writeToTabOutput(const QString &msg, TreatURLLinks treatURLLinks)
{
  QTextBrowser *text = ui->twProperties->widget(ctn_TABINDEX_OUTPUT)->findChild<QTextBrowser*>("textBrowser");
  if (text)
  {
    ensureTabVisible(ctn_TABINDEX_OUTPUT);
    positionTextEditCursorAtEnd();

    if(treatURLLinks == ectn_TREAT_URL_LINK)
    {
      text->insertHtml(Package::makeURLClickable(msg));
    }
    else
    {
      text->insertHtml(msg);
    }

    text->ensureCursorVisible();
  }
}

/*
 * A helper method which writes the given string to OutputTab's textbrowser
 * This is the EXTENDED version, it checks lots of things before writing msg
 */
void MainWindow::writeToTabOutputExt(const QString &msg, TreatURLLinks treatURLLinks)
{
  //std::cout << "To print: " << msg.toLatin1().data() << std::endl;
  QTextBrowser *text = ui->twProperties->widget(ctn_TABINDEX_OUTPUT)->findChild<QTextBrowser*>("textBrowser");
  if (text)
  {    
    //If the msg waiting to being print is from curl status OR any other unwanted string...
    if ((msg.contains(QRegularExpression("\\(\\d")) &&
         (!msg.contains("target", Qt::CaseInsensitive)) &&
         (!msg.contains("package", Qt::CaseInsensitive)) &&
         (!msg.contains("conflicting", Qt::CaseInsensitive))) ||

        (msg.contains(QRegularExpression("\\d\\)")) &&
        (!msg.contains("target", Qt::CaseInsensitive)) &&
        (!msg.contains("package", Qt::CaseInsensitive))) ||

        msg.indexOf("Enter a selection", Qt::CaseInsensitive) == 0 ||
        msg.indexOf("Proceed with", Qt::CaseInsensitive) == 0 ||
        (msg.indexOf("%") != -1 && (!msg.contains("xtracting"))) ||
        msg.indexOf("---") != -1 ||
        msg == "[1/")
    {
      return;
    }

    //If the msg waiting to being print has not yet been printed...
    //std::cout << "Searching for duplicates: " << msg.toLatin1().data() << std::endl;
    if(textInTabOutput(msg))
    {
      return;
    }

    QString newMsg = msg;
    ensureTabVisible(ctn_TABINDEX_OUTPUT);
    positionTextEditCursorAtEnd();

    if(newMsg.contains(QRegularExpression("<font color")))
    {
      newMsg += "<br>";
    }
    else
    {
      if(newMsg.contains(QRegularExpression("REMOVED")) ||
         newMsg.contains(QRegularExpression("removing ")) ||
         newMsg.contains(QRegularExpression("could not ")) ||
         newMsg.contains(QRegularExpression("error:")) ||
         newMsg.contains(QRegularExpression("failed")) ||
         newMsg.contains(QRegularExpression("is not synced")) ||
         newMsg.contains(QRegularExpression("[Rr]emoving")) ||
         newMsg.contains(QRegularExpression("[Dd]einstalling")) ||
         newMsg.contains(QRegularExpression("[Dd]eleting")) ||
         newMsg.contains(StrConstants::getCommandFinishedWithErrors()) ||
         newMsg.contains(QRegularExpression("could not be found")))
      {
        newMsg = "<b><font color=\"#E55451\">" + newMsg + "&nbsp;</font></b>"; //RED
      }
      else if (newMsg.contains(QRegularExpression("warning")) ||
               newMsg.contains(QRegularExpression("downgrading")) ||
               newMsg.contains(QRegularExpression("options changed")))
      {
        newMsg = "<b><font color=\"#FF8040\">" + newMsg + "</font></b>"; //ORANGE
      }
      else if(newMsg.contains(QRegularExpression("[Rr]einstalling")) ||
              newMsg.contains(QRegularExpression("[Ii]nstalling")) ||
              newMsg.contains(QRegularExpression("[Uu]pgrading")) ||
              newMsg.contains(QRegularExpression("[Ll]oading")) ||
              newMsg.contains(QRegularExpression("[Rr]esolving")) ||
              newMsg.contains(QRegularExpression("[Ee]xtracting")) ||
              newMsg.contains(QRegularExpression("[Ll]ooking")))
      {
         newMsg = "<b><font color=\"#4BC413\">" + newMsg + "</font></b>"; //GREEN
      }
      else if (newMsg.contains("-") &&
               (!newMsg.contains(QRegularExpression("->"))) &&
               (!newMsg.contains(QRegularExpression("(is|are) up-to-date"))) &&
               (!newMsg.contains(QRegularExpression("\\s"))))
      {
        newMsg = "<b><font color=\"#B4AB58\">" + newMsg + "</font></b>"; //IT'S A PKGNAME!
      }
      else if (newMsg.contains(":") &&
               (!newMsg.contains(QRegularExpression("->"))) &&
               (!newMsg.contains(QRegularExpression("Number of packages to be"))) &&
               (!newMsg.contains(QRegularExpression("\\):"))) &&
               (!newMsg.contains(QRegularExpression(":$"))))
      {
        newMsg = "<b><font color=\"#B4AB58\">" + newMsg + "</font></b>"; //IT'S A PKGNAME!
      }
    }

    if (newMsg.contains("::"))
    {
      newMsg = "<br><B>" + newMsg + "</B><br><br>";
    }

    if (!newMsg.contains(QRegularExpression("<br")) && !newMsg.contains("hecking integrity"))
    {
      newMsg += "<br>";
    }

    if (newMsg.contains(QRegularExpression("done \\(\\d+ conflicting\\)")))
    {
      newMsg = "&nbsp;" + newMsg;
    }

    if (newMsg.contains("xtracting"))
    {
      int f=newMsg.indexOf(":");
      newMsg = newMsg.mid(0, f).trimmed() + "...<br>";
    }

    if (treatURLLinks == ectn_TREAT_URL_LINK)
      text->insertHtml(Package::makeURLClickable(newMsg));
    else
      text->insertHtml(newMsg);

    //std::cout << "Printed: " << msg.toLatin1().data() << std::endl;
    text->ensureCursorVisible();
  }
}
