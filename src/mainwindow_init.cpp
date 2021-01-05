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
 * This is MainWindow's initialization code
 */

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "strconstants.h"
#include "uihelper.h"
#include "settingsmanager.h"
#include "searchlineedit.h"
#include "treeviewpackagesitemdelegate.h"
#include "searchbar.h"
#include <iostream>
#include <cassert>

#include <QLabel>
#include <QStandardItemModel>
#include <QTextBrowser>
#include <QResource>
#include <QFile>
#include <QComboBox>
#include <QListView>
#include <QTabBar>
#include <QProgressBar>
#include <QSystemTrayIcon>
#include <QToolButton>
#include <QScreen>
#include <QDebug>

/*
 * Loads various application settings configured in ~/.config/octopkg/octopkg.conf
 */
void MainWindow::loadSettings(){
  if (ui->tvPackages->model() != NULL)
  {
    int packageListOrderedCol = SettingsManager::instance()->getPackageListOrderedCol();
    Qt::SortOrder packageListSortOrder = (Qt::SortOrder) SettingsManager::instance()->getPackageListSortOrder();

    ui->tvPackages->header()->setSortIndicator( packageListOrderedCol, packageListSortOrder );
    ui->tvPackages->sortByColumn( packageListOrderedCol, packageListSortOrder );
  }
  else assert(false);
}

/*
 * This method only retrieves the App saved panels settings
 */
void MainWindow::loadPanelSettings(){
  int panelOrganizing = SettingsManager::instance()->getPanelOrganizing();

  switch(panelOrganizing){
    case ectn_MAXIMIZE_PACKAGES:
      maximizePackagesTreeView();
      break;
    case ectn_MAXIMIZE_PROPERTIES:
      maximizePropertiesTabWidget();
      break;
    case ectn_NORMAL:
      ui->splitterHorizontal->restoreState(SettingsManager::instance()->getSplitterHorizontalState());
      break;
  }

  //Do we have to show or hide the Groups panel?
  if (!SettingsManager::getShowGroupsPanel()){
    hideGroupsWidget();
  }
}

/*
 * Saves all application settings to ~/.config/octopkg/octopkg.conf
 */
void MainWindow::saveSettings(SaveSettingsReason saveSettingsReason){
  switch(saveSettingsReason){
    case ectn_CurrentTabIndex:
      SettingsManager::instance()->setCurrentTabIndex(ui->twProperties->currentIndex());
      break;

    case ectn_MAXIMIZE_PACKAGES:
      SettingsManager::instance()->setPanelOrganizing(ectn_MAXIMIZE_PACKAGES);
      break;

    case ectn_MAXIMIZE_PROPERTIES:
      SettingsManager::instance()->setPanelOrganizing(ectn_MAXIMIZE_PROPERTIES);
      break;

    case ectn_NORMAL:
      SettingsManager::instance()->setPanelOrganizing(ectn_NORMAL);
      SettingsManager::instance()->setSplitterHorizontalState(ui->splitterHorizontal->saveState());
      SettingsManager::instance()->setShowGroupsPanel(1); //And also show Groups panel!
      break;

    case ectn_PackageList:
      SettingsManager::instance()->setPackageListOrderedCol(ui->tvPackages->header()->sortIndicatorSection());
      SettingsManager::instance()->setPackageListSortOrder(ui->tvPackages->header()->sortIndicatorOrder());
      break;

    case ectn_GROUPS:
      QList<int> rl;
      rl = ui->splitterVertical->sizes();

      int show=0;
      if ( rl[1] != 0 )
      {
        show = 1;
      }

      SettingsManager::instance()->setShowGroupsPanel(show);
      break;
  }
}

/*
 * Save Package treeview column widths to bring them back in the next execution
 */
void MainWindow::savePackageColumnWidths()
{
  SettingsManager::setPackageIconColumnWidth(
        ui->tvPackages->columnWidth(PackageModel::ctn_PACKAGE_ICON_COLUMN));
  SettingsManager::setPackageNameColumnWidth(
        ui->tvPackages->columnWidth(PackageModel::ctn_PACKAGE_NAME_COLUMN));
  SettingsManager::setPackageVersionColumnWidth(
        ui->tvPackages->columnWidth(PackageModel::ctn_PACKAGE_VERSION_COLUMN));

  if (!m_actionSwitchToRemoteSearch->isChecked())
  {
    SettingsManager::setPackageSizeColumnWidth(
        ui->tvPackages->columnWidth(PackageModel::ctn_PACKAGE_SIZE_COLUMN));
  }
}

/*
 * If we have some outdated packages, let's put octoPkg in a red face/angry state ;-)
 */
void MainWindow::initAppIcon()
{
  m_outdatedStringList->clear();
  m_outdatedList = Package::getOutdatedStringList();
  for(QString k: m_outdatedList->keys())
  {
    m_outdatedStringList->append(k);
  }

  m_numberOfOutdatedPackages = m_outdatedStringList->count();
  refreshAppIcon();
}

/*
 * Inits the Groups combobox, so it can be added in app's toolBar
 */
void MainWindow::initPackageGroups()
{
  //This is the twGroups init code
  ui->twGroups->setVisible(false);

  /*ui->twGroups->setColumnCount(1);
  ui->twGroups->setHeaderLabel(StrConstants::getCategories());
  ui->twGroups->header()->setSortIndicatorShown(false);
  ui->twGroups->header()->setSectionsClickable(false);
  ui->twGroups->header()->setSectionsMovable(false);
  ui->twGroups->header()->setSectionResizeMode(QHeaderView::Fixed);
  ui->twGroups->setFrameShape(QFrame::NoFrame);
  ui->twGroups->setFrameShadow(QFrame::Plain);
  ui->twGroups->setStyleSheet(StrConstants::getTreeViewCSS());
  ui->twGroups->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(ui->twGroups, SIGNAL(itemSelectionChanged()), this, SLOT(onPackageGroupChanged()));*/
}

/*
 * Whenever user changes the Groups combobox, this method will be triggered...
 */
void MainWindow::onPackageGroupChanged()
{
  if (isAllCategoriesSelected())
  {
    ui->actionSearchByName->setChecked(true);
    tvPackagesSearchColumnChanged(ui->actionSearchByName);
  }
}

/*
 * Inits the Repository menuItem in menuBar
 */
void MainWindow::initMenuBar()
{
  /*QActionGroup *actionGroupPackages = new QActionGroup(this);
  QActionGroup *actionGroupRepositories = new QActionGroup(this);

  ui->actionViewAllPackages->setText(StrConstants::getAll());

  actionGroupPackages->addAction(ui->actionViewAllPackages);
  actionGroupPackages->addAction(ui->actionViewInstalledPackages);
  actionGroupPackages->addAction(ui->actionViewNonInstalledPackages);
  actionGroupPackages->setExclusive(true);

  m_actionMenuRepository = ui->menuView->addAction(StrConstants::getOrigin());
  QMenu *subMenu = new QMenu(ui->menuView);
  connect(subMenu, SIGNAL(triggered(QAction*)), this, SLOT(selectedRepositoryMenu(QAction*)));

  m_actionRepositoryAll = subMenu->addAction(StrConstants::getAll());
  m_actionRepositoryAll->setCheckable(true);
  m_actionRepositoryAll->setChecked(true);
  subMenu->addSeparator();

  RepoConf *repoConf = new RepoConf();
  QStringList repos = repoConf->getRepos();

  for(QString repo: repos)
  {
    QAction * createdAction = subMenu->addAction(repo);
    createdAction->setCheckable(true);
    actionGroupRepositories->addAction(createdAction);
  }

  actionGroupRepositories->addAction(m_actionRepositoryAll);
  actionGroupRepositories->setExclusive(true);
  m_actionMenuRepository->setMenu(subMenu);
  */

  ui->menuView->menuAction()->setVisible(false);

  for (QAction * act: ui->menuBar->actions())
  {
    QString text = act->text();
    //text = text.remove("&");
    act->setText(qApp->translate("MainWindow", text.toUtf8(), 0));
  }
}

/*
 * Inits the toolbar, including taking that annoying default view action out of the game
 */
void MainWindow::initToolBar()
{
  initPackageGroups();

  if (QApplication::screens().count() == 1)
  {
    QScreen *sc = QApplication::screens().first();

    //if (sc->size().width() >= 1920)
    if (sc->devicePixelRatio() > 1.0)
      ui->mainToolBar->setIconSize(QSize(32, 32));
    else
      ui->mainToolBar->setIconSize(QSize(22, 22));
  }

  ui->mainToolBar->addAction(ui->actionSyncPackages);
  ui->mainToolBar->addAction(ui->actionSystemUpgrade);

  if (m_outdatedStringList->count() > 0)
    ui->actionSystemUpgrade->setEnabled(true);
  else
    ui->actionSystemUpgrade->setEnabled(false);

  ui->mainToolBar->addAction(ui->actionCommit);
  ui->mainToolBar->addAction(ui->actionCancel);
  m_separatorForActionRemoteSearch = ui->mainToolBar->addSeparator();
  //ui->mainToolBar->addAction(m_actionSwitchToLocalSearch);
  ui->mainToolBar->addAction(m_actionSwitchToRemoteSearch);

  m_dummyAction = new QAction(this);
  m_dummyAction->setVisible(false);
  ui->mainToolBar->addAction(m_dummyAction);
  m_leFilterPackage->setMinimumHeight(24);
  ui->mainToolBar->addWidget(m_leFilterPackage);
  ui->mainToolBar->toggleViewAction()->setEnabled(false);
  ui->mainToolBar->toggleViewAction()->setVisible(false);
  ui->mainToolBar->setStyleSheet(StrConstants::getToolBarCSS());

  m_leFilterPackage->setPlaceholderText(StrConstants::getLineEditTextLocal());
}

/*
 * The only thing needed here is to create a dynamic label which will contain the package counters
 */
void MainWindow::initStatusBar()
{
  m_lblSelCounter = new QLabel(this);
  m_lblTotalCounters = new QLabel(this);
  m_progressWidget = new QProgressBar(this);
  m_progressWidget->close();
  m_progressWidget->setMaximumWidth(250);

  ui->statusBar->addWidget(m_lblSelCounter);
  ui->statusBar->addWidget(m_lblTotalCounters);
  ui->statusBar->addPermanentWidget(m_progressWidget);
}

/*
 * Inits the outdated toolbutton, which warns the user about outdated packages
 */
void MainWindow::initToolButtonPacman()
{
  m_toolButtonPacman = new QToolButton(this);
  m_toolButtonPacman->setIconSize(QSize(16, 16));
  m_toolButtonPacman->setIcon(IconHelper::getIconOutdated());
  m_toolButtonPacman->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_toolButtonPacman->setAutoRaise(true);
  m_toolButtonPacman->hide();
  m_menuToolButtonPacman = new QMenu(this);
  m_menuToolButtonPacman->addAction(m_actionInstallPkgUpdates);
  m_toolButtonPacman->setPopupMode(QToolButton::MenuButtonPopup);
  m_toolButtonPacman->setMenu(m_menuToolButtonPacman);

  connect(m_toolButtonPacman, &QAbstractButton::clicked, this, &MainWindow::outputOutdatedPackageList);
}

/*
 * Sets the TabWidget Properties to the given index/tab and change app focus to its child widget
 */
void MainWindow::changeTabWidgetPropertiesIndex(const int newIndex)
{
  int oldTabIndex = ui->twProperties->currentIndex();
  ensureTabVisible(newIndex);

  if (newIndex == oldTabIndex)
  {
    if (oldTabIndex == ctn_TABINDEX_INFORMATION)
    {
      refreshTabInfo();
    }
    else if (oldTabIndex == ctn_TABINDEX_FILES)
    {
      refreshTabFiles();
    }

    ui->twProperties->currentWidget()->childAt(1,1)->setFocus();
  }
  else
  {
    //For any other tab... just doing the following is enough
    ui->twProperties->currentWidget()->childAt(1,1)->setFocus();
  }
}

/*
 * Sets the current tabWidget index, based on last user session
 */
void MainWindow::initTabWidgetPropertiesIndex()
{
  connect(ui->splitterHorizontal, &QSplitter::splitterMoved, this, &MainWindow::horizontalSplitterMoved);
  ui->twProperties->setCurrentIndex(SettingsManager::getCurrentTabIndex());
}

/*
 * This is the 4th tab (Transaction).
 * It pops up whenever the user selects a remove/install action on a selected package
 */
void MainWindow::initTabTransaction()
{
  m_modelTransaction = new QStandardItemModel(this);
  QWidget *tabTransaction = new QWidget();
  QGridLayout *gridLayoutX = new QGridLayout(tabTransaction);
  gridLayoutX->setSpacing(0);
  gridLayoutX->setMargin(0);

  QTreeView *tvTransaction = new QTreeView(tabTransaction);
  tvTransaction->setObjectName("tvTransaction");
  tvTransaction->setContextMenuPolicy(Qt::CustomContextMenu);
  tvTransaction->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tvTransaction->setDropIndicatorShown(false);
  tvTransaction->setAcceptDrops(false);
  tvTransaction->setSelectionMode(QAbstractItemView::ExtendedSelection);
  tvTransaction->setItemDelegate(new TreeViewPackagesItemDelegate(tvTransaction));
  tvTransaction->header()->setSortIndicatorShown(false);
  tvTransaction->header()->setSectionsClickable(false);
  tvTransaction->header()->setSectionsMovable(false);
  tvTransaction->header()->setSectionResizeMode(QHeaderView::Fixed);
  tvTransaction->setFrameShape(QFrame::NoFrame);
  tvTransaction->setFrameShadow(QFrame::Plain);
  tvTransaction->setStyleSheet(StrConstants::getTreeViewCSS());
  tvTransaction->expandAll();

  m_modelTransaction->setSortRole(0);
  m_modelTransaction->setColumnCount(0);

  QStringList sl;
  m_modelTransaction->setHorizontalHeaderLabels(sl << StrConstants::getPackages());

  QStandardItem *siToBeRemoved = new QStandardItem(IconHelper::getIconToRemove(),
                                                   StrConstants::getTransactionRemoveText());
  QStandardItem *siToBeInstalled = new QStandardItem(IconHelper::getIconToInstall(),
                                                     StrConstants::getTransactionInstallText());

  m_modelTransaction->appendRow(siToBeRemoved);
  m_modelTransaction->appendRow(siToBeInstalled);

  gridLayoutX->addWidget(tvTransaction, 0, 0, 1, 1);
  tvTransaction->setModel(m_modelTransaction);

  QString aux(StrConstants::getTabTransactionName());
  ui->twProperties->removeTab(ctn_TABINDEX_TRANSACTION);
  ui->twProperties->insertTab(ctn_TABINDEX_TRANSACTION, tabTransaction, QApplication::translate (
                                "MainWindow", aux.toUtf8(), 0/*, QApplication::UnicodeUTF8*/ ));

  connect(tvTransaction, &QWidget::customContextMenuRequested, this, &MainWindow::execContextMenuTransaction);
  connect(tvTransaction->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::tvTransactionSelectionChanged);

  connect(tvTransaction->model(), &QAbstractItemModel::rowsInserted,
          this, &MainWindow::tvTransactionRowsInserted);
  connect(tvTransaction->model(), &QAbstractItemModel::rowsRemoved,
          this, &MainWindow::tvTransactionRowsRemoved);
}

/*
 * This is the LineEdit widget used to filter the package list
 */
void MainWindow::initLineEditFilterPackages(){
  connect(m_leFilterPackage, &QLineEdit::textChanged, this, &MainWindow::reapplyPackageFilter);
}

/*
 * This is the package treeview, it lists the installed [and not installed] packages in the system
 */
void MainWindow::initPackageTreeView()
{
  ui->tvPackages->init();
  ui->tvPackages->setModel(m_packageModel.get());
  ui->tvPackages->resizePackageView();

  connect(ui->tvPackages->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::tvPackagesSelectionChanged);
  //connect(ui->tvPackages, SIGNAL(activated(QModelIndex)), this, SLOT(changedTabIndex()));
  //connect(ui->tvPackages, SIGNAL(clicked(QModelIndex)), this, SLOT(changedTabIndex()));
  connect(ui->tvPackages->header(), &QHeaderView::sortIndicatorChanged, this,
          &MainWindow::headerViewPackageListSortIndicatorClicked);
  connect(ui->tvPackages, &QWidget::customContextMenuRequested, this,
          &MainWindow::execContextMenuPackages);
  connect(ui->tvPackages, &QAbstractItemView::doubleClicked, this, &MainWindow::onDoubleClickPackageList);
}

void MainWindow::removePackageTreeViewConnections()
{
  disconnect(ui->tvPackages->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::tvPackagesSelectionChanged);
  //disconnect(ui->tvPackages, SIGNAL(activated(QModelIndex)), this, SLOT(changedTabIndex()));
  //disconnect(ui->tvPackages, SIGNAL(clicked(QModelIndex)), this, SLOT(changedTabIndex()));
  disconnect(ui->tvPackages->header(), &QHeaderView::sortIndicatorChanged, this,
          &MainWindow::headerViewPackageListSortIndicatorClicked);
  disconnect(ui->tvPackages, &QWidget::customContextMenuRequested, this,
          &MainWindow::execContextMenuPackages);
  disconnect(ui->tvPackages, &QAbstractItemView::doubleClicked, this, &MainWindow::onDoubleClickPackageList);
}

/*
 * This tab has a QTextBrowser which shows information about the selected package
 */
void MainWindow::initTabInfo(){
  QWidget *tabInfo = new QWidget();
  QGridLayout *gridLayoutX = new QGridLayout ( tabInfo );
  gridLayoutX->setSpacing ( 0 );
  gridLayoutX->setMargin ( 0 );

  QTextBrowser *text = new QTextBrowser(tabInfo);
  text->setObjectName("textBrowser");
  text->setReadOnly(true);
  text->setFrameShape(QFrame::NoFrame);
  text->setFrameShadow(QFrame::Plain);
  text->setOpenLinks(false);
  connect(text, &QTextBrowser::anchorClicked, this, &MainWindow::outputTextBrowserAnchorClicked);
  connect(text, qOverload<const QUrl&>(&QTextBrowser::highlighted), this, &MainWindow::showAnchorDescription);
  gridLayoutX->addWidget ( text, 0, 0, 1, 1 );

  QString tabName(StrConstants::getTabInfoName());
  ui->twProperties->removeTab(ctn_TABINDEX_INFORMATION);
  ui->twProperties->insertTab(ctn_TABINDEX_INFORMATION, tabInfo, QApplication::translate (
      "MainWindow", tabName.toUtf8(), 0/*, QApplication::UnicodeUTF8*/ ) );
  ui->twProperties->setUsesScrollButtons(false);

  SearchBar *searchBar = new SearchBar(this);
  connect(searchBar, &SearchBar::textChanged, this, &MainWindow::searchBarTextChangedInTextBrowser);
  connect(searchBar, &SearchBar::closed, this, &MainWindow::searchBarClosedInTextBrowser);
  connect(searchBar, &SearchBar::findNext, this, &MainWindow::searchBarFindNextInTextBrowser);
  connect(searchBar, &SearchBar::findPrevious, this, &MainWindow::searchBarFindPreviousInTextBrowser);
  gridLayoutX->addWidget(searchBar, 1, 0, 1, 1);

  ui->twProperties->setCurrentIndex(ctn_TABINDEX_INFORMATION);
  text->show();
  text->setFocus();
}

/*
 * This is the files treeview, which shows the directory structure of ONLY installed packages's files.
 */
void MainWindow::initTabFiles()
{
  QWidget *tabPkgFileList = new QWidget(this);
  QGridLayout *gridLayoutX = new QGridLayout ( tabPkgFileList );
  gridLayoutX->setSpacing ( 0 );
  gridLayoutX->setMargin ( 0 );
  QStandardItemModel *modelPkgFileList = new QStandardItemModel(this);
  QTreeView *tvPkgFileList = new QTreeView(tabPkgFileList);
  tvPkgFileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tvPkgFileList->setDropIndicatorShown(false);
  tvPkgFileList->setAcceptDrops(false);
  tvPkgFileList->header()->setSortIndicatorShown(false);
  tvPkgFileList->header()->setSectionsClickable(false);
  tvPkgFileList->header()->setSectionsMovable(false);
  tvPkgFileList->header()->setSectionResizeMode(QHeaderView::Fixed);
  tvPkgFileList->setFrameShape(QFrame::NoFrame);
  tvPkgFileList->setFrameShadow(QFrame::Plain);
  tvPkgFileList->setObjectName("tvPkgFileList");
  tvPkgFileList->setStyleSheet(StrConstants::getTreeViewCSS());

  modelPkgFileList->setSortRole(0);
  modelPkgFileList->setColumnCount(0);
  gridLayoutX->addWidget(tvPkgFileList, 0, 0, 1, 1);
  tvPkgFileList->setModel(modelPkgFileList);

  QString aux(StrConstants::getTabFilesName());
  ui->twProperties->removeTab(ctn_TABINDEX_FILES);
  ui->twProperties->insertTab(ctn_TABINDEX_FILES, tabPkgFileList, QApplication::translate (
                                                  "MainWindow", aux.toUtf8(), 0/*, QApplication::UnicodeUTF8*/ ) );

  tvPkgFileList->setContextMenuPolicy(Qt::CustomContextMenu);
  SearchBar *searchBar = new SearchBar(this);

  connect(searchBar, &SearchBar::textChanged, this, &MainWindow::searchBarTextChangedInTreeView);
  connect(searchBar, &SearchBar::closed, this, &MainWindow::searchBarClosedInTreeView);
  connect(searchBar, &SearchBar::findNext, this, &MainWindow::searchBarFindNextInTreeView);
  connect(searchBar, &SearchBar::findPrevious, this, &MainWindow::searchBarFindPreviousInTreeView);

  gridLayoutX->addWidget(searchBar, 1, 0, 1, 1);

  connect(tvPkgFileList, &QWidget::customContextMenuRequested,
          this, &MainWindow::execContextMenuPkgFileList);
  connect(tvPkgFileList, &QAbstractItemView::doubleClicked,
          this, &MainWindow::openFile);
}

/*
 * This is the TextEdit output tab, which shows the output of pacman commands.
 */
void MainWindow::initTabOutput()
{
  QWidget *tabOutput = new QWidget();
  QGridLayout *gridLayoutX = new QGridLayout(tabOutput);
  gridLayoutX->setSpacing ( 0 );
  gridLayoutX->setMargin ( 0 );

  QTextBrowser *text = new QTextBrowser(tabOutput);
  text->setObjectName("textBrowser");
  text->setReadOnly(true);
  text->setOpenLinks(false);
  text->setFrameShape(QFrame::NoFrame);
  text->setFrameShadow(QFrame::Plain);

  connect(text, &QTextBrowser::anchorClicked, this, &MainWindow::outputTextBrowserAnchorClicked);
  connect(text, qOverload<const QUrl&>(&QTextBrowser::highlighted), this, &MainWindow::showAnchorDescription);
  gridLayoutX->addWidget (text, 0, 0, 1, 1);

  QString aux(StrConstants::getTabOutputName());
  ui->twProperties->removeTab(ctn_TABINDEX_OUTPUT);
  ui->twProperties->insertTab(ctn_TABINDEX_OUTPUT, tabOutput, QApplication::translate (
      "MainWindow", aux.toUtf8(), 0/*, QApplication::UnicodeUTF8*/ ) );

  SearchBar *searchBar = new SearchBar(this);
  connect(searchBar, &SearchBar::textChanged, this, &MainWindow::searchBarTextChangedInTextBrowser);
  connect(searchBar, &SearchBar::closed, this, &MainWindow::searchBarClosedInTextBrowser);
  connect(searchBar, &SearchBar::findNext, this, &MainWindow::searchBarFindNextInTextBrowser);
  connect(searchBar, &SearchBar::findPrevious, this, &MainWindow::searchBarFindPreviousInTextBrowser);
  gridLayoutX->addWidget(searchBar, 1, 0, 1, 1);

  ui->twProperties->setCurrentIndex(ctn_TABINDEX_OUTPUT);
  text->show();
  text->setFocus();
}

/*
 * Initialize QAction objects
 */
void MainWindow::initActions()
{
  m_hasSLocate = true; //UnixCommand::hasTheExecutable("slocate");
  m_hasMirrorCheck = false;
  m_actionSysInfo = new QAction(this);
  m_actionPackageInfo = new QAction(this);
  m_actionPackageInfo->setText(StrConstants::getTabInfoName());

  m_actionLockPackage = new QAction(this);
  m_actionLockPackage->setText(StrConstants::getLock());
  m_actionLockPackage->setIcon(IconHelper::getIconLocked());

  m_actionUnlockPackage = new QAction(this);
  m_actionUnlockPackage->setText(StrConstants::getUnlock());
  m_actionUnlockPackage->setIcon(IconHelper::getIconUnlock());

  /*m_actionSwitchToLocalSearch = new QAction(this);
  m_actionSwitchToLocalSearch->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_I));
  m_actionSwitchToLocalSearch->setIcon(IconHelper::getIconHardDrive());
  m_actionSwitchToLocalSearch->setText(StrConstants::getFilterLocalPackages());
  m_actionSwitchToLocalSearch->setToolTip(m_actionSwitchToLocalSearch->toolTip() + QLatin1String(" (") +
                                          m_actionSwitchToLocalSearch->shortcut().toString() + QLatin1String(")"));
  m_actionSwitchToLocalSearch->setCheckable(true);
  m_actionSwitchToLocalSearch->setChecked(true);*/

  m_actionSwitchToRemoteSearch = new QAction(this);
  m_actionSwitchToRemoteSearch->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_I));
  m_actionSwitchToRemoteSearch->setIcon(IconHelper::getIconInternet());
  m_actionSwitchToRemoteSearch->setText(StrConstants::getSearchForPackages());
  m_actionSwitchToRemoteSearch->setToolTip(m_actionSwitchToRemoteSearch->toolTip() + QLatin1String(" (") +
                                           m_actionSwitchToRemoteSearch->shortcut().toString() + QLatin1String(")"));
  m_actionSwitchToRemoteSearch->setCheckable(true);
  m_actionSwitchToRemoteSearch->setChecked(false);

  m_actionGroupSearch = new QActionGroup(this);
  //m_actionGroupSearch->addAction(m_actionSwitchToLocalSearch);
  m_actionGroupSearch->addAction(m_actionSwitchToRemoteSearch);
  m_actionGroupSearch->setExclusive(true);
  connect(m_actionGroupSearch, &QActionGroup::triggered, this, &MainWindow::remoteSearchClicked);

  m_actionInstallPkgUpdates = new QAction(this);
  m_actionInstallPkgUpdates->setIcon(IconHelper::getIconToInstall());
  m_actionInstallPkgUpdates->setText(ui->actionInstall->text());
  m_actionInstallPkgUpdates->setIconVisibleInMenu(true);
  connect(m_actionInstallPkgUpdates, &QAction::triggered, this, &MainWindow::doPreSystemUpgrade);

  m_actionShowGroups = new QAction(this);
  m_actionShowGroups->setIcon(IconHelper::getIconShowGroups());
  m_actionShowGroups->setText(StrConstants::getCategories());
  m_actionShowGroups->setCheckable(true);
  m_actionShowGroups->setChecked(true);
  m_actionShowGroups->setShortcut(QKeySequence(Qt::Key_F9));
  connect(m_actionShowGroups, &QAction::triggered, this, &MainWindow::hideGroupsWidget);

  m_actionCopyFullPath = new QAction(this);
  m_actionCopyFullPath->setText(StrConstants::getCopyFullPath());
  m_actionCopyFullPath->setIcon(IconHelper::getIconEditCopy());
  connect(m_actionCopyFullPath, &QAction::triggered, this, &MainWindow::copyFullPathToClipboard);

  QActionGroup *actionGroup = new QActionGroup(this);
  actionGroup->addAction(ui->actionSearchByDescription);
  actionGroup->addAction(ui->actionSearchByName);
  actionGroup->addAction(ui->actionSearchByFile);
  ui->actionSearchByName->setChecked(true);
  actionGroup->setExclusive(true);
  connect(actionGroup, &QActionGroup::triggered, this, &MainWindow::tvPackagesSearchColumnChanged);

  //ui->actionInstallLocalPackage->setIcon(IconHelper::getIconFolder());
  ui->actionOpenDirectory->setIcon(IconHelper::getIconFolder());

  connect(ui->actionCleanLocalCache, &QAction::triggered, this, &MainWindow::doCleanCache);
  connect(ui->tvPackages->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::invalidateTabs);
  //connect(ui->actionInstallLocalPackage, SIGNAL(triggered()), this, SLOT(installLocalPackage()));
  connect(ui->actionRemoveTransactionItem, &QAction::triggered, this, &MainWindow::onPressDelete);
  connect(ui->actionRemoveTransactionItems, &QAction::triggered, this, &MainWindow::onPressDelete);
  connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionSyncPackages, &QAction::triggered, this, &MainWindow::doSyncDatabase);
  connect(ui->actionSystemUpgrade, &QAction::triggered, this, &MainWindow::doPreSystemUpgrade);
  connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::insertIntoRemovePackage);
  connect(ui->actionInstall, &QAction::triggered, this, &MainWindow::insertIntoInstallPackage);
  connect(m_actionLockPackage, &QAction::triggered, this, &MainWindow::doLock);
  connect(m_actionUnlockPackage, &QAction::triggered, this, &MainWindow::doUnlock);
  connect(ui->actionFindFileInPackage, &QAction::triggered, this, &MainWindow::findFileInPackage);
  connect(ui->actionCommit, &QAction::triggered, this, &MainWindow::commitTransaction);
  connect(ui->actionCancel, &QAction::triggered, this, &MainWindow::cancelTransaction);
  connect(ui->actionGetNews, &QAction::triggered, this, [=]() { refreshDistroNews(); });
  connect(ui->twProperties, &QTabWidget::currentChanged, this, &MainWindow::changedTabIndex);
  connect(ui->actionHelpUsage, &QAction::triggered, this, &MainWindow::onHelpUsage);
  connect(ui->actionDonate, &QAction::triggered, this, &MainWindow::onHelpDonate);
  connect(ui->actionHelpAbout, &QAction::triggered, this, &MainWindow::onHelpAbout);
  connect(m_actionPackageInfo, &QAction::triggered, this, &MainWindow::showPackageInfo);

  //Actions from tvPkgFileList context menu
  connect(ui->actionCollapseAllItems, &QAction::triggered, this, &MainWindow::collapseAllContentItems);
  connect(ui->actionExpandAllItems, &QAction::triggered, this, &MainWindow::expandAllContentItems);
  connect(ui->actionCollapseItem, &QAction::triggered, this, &MainWindow::collapseThisContentItems);
  connect(ui->actionExpandItem, &QAction::triggered, this, &MainWindow::expandThisContentItems);
  connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
  connect(ui->actionEditFile, &QAction::triggered, this, &MainWindow::editFile);
  connect(ui->actionOpenDirectory, &QAction::triggered, this, &MainWindow::openDirectory);
  connect(ui->actionOpenTerminal, &QAction::triggered, this, &MainWindow::openTerminal);

  // Use theme icons for QActions
  ui->actionSyncPackages->setIcon(IconHelper::getIconSyncDatabase());
  ui->actionCommit->setIcon(IconHelper::getIconCommit());
  ui->actionCancel->setIcon(IconHelper::getIconRollback());
  ui->actionExit->setIcon(IconHelper::getIconExit());
  ui->actionSystemUpgrade->setIcon(IconHelper::getIconSystemUpgrade());
  ui->actionInstall->setIcon(IconHelper::getIconToInstall());
  ui->actionRemove->setIcon(IconHelper::getIconToRemove());
  ui->actionGetNews->setIcon(IconHelper::getIconGetNews());
  ui->actionCollapseItem->setIcon(IconHelper::getIconCollapse());
  ui->actionExpandItem->setIcon(IconHelper::getIconExpand());
  ui->actionCollapseAllItems->setIcon(IconHelper::getIconCollapse());
  ui->actionExpandAllItems->setIcon(IconHelper::getIconExpand());
  ui->actionOpenFile->setIcon(IconHelper::getIconBinary());
  ui->actionEditFile->setIcon(IconHelper::getIconEditFile());
  ui->actionOpenDirectory->setIcon(IconHelper::getIconFolder());
  ui->actionOpenTerminal->setIcon(IconHelper::getIconTerminal());
  ui->actionRemoveTransactionItem->setIcon(IconHelper::getIconClose());
  ui->actionRemoveTransactionItems->setIcon(IconHelper::getIconClose());
  ui->actionFindFileInPackage->setIcon(IconHelper::getIconFindFileInPackage());
  //ui->actionOpenRootTerminal->setIcon(IconHelper::getIconTerminal());

  //Actions for the View menu
  connect(ui->actionViewAllPackages, &QAction::triggered, this, &MainWindow::selectedAllPackagesMenu);
  connect(ui->actionViewInstalledPackages, &QAction::triggered, this, &MainWindow::selectedInstalledPackagesMenu);
  connect(ui->actionViewNonInstalledPackages, &QAction::triggered, this, &MainWindow::selectedNonInstalledPackagesMenu);

  if (WMHelper::isKDERunning())
  {
    ui->actionHelpAbout->setIcon(IconHelper::getIconHelpAbout());
    ui->actionHelpUsage->setIcon(IconHelper::getIconHelpUsage());
    //ui->actionInstallLocalPackage->setIcon(IconHelper::getIconInstallLocalPackage());
  }

  // Populate Tools menu
  ui->menuTools->menuAction()->setVisible(false);
  //refreshMenuTools();

  if (WMHelper::isXFCERunning())
  {
    //Loop through all actions and set their icons (if any) visible to menus.
    for(QAction* ac: this->findChildren<QAction*>(QRegularExpression("(m_a|a)ction\\S*")))
    {
      if (ac) ac->setIconVisibleInMenu(true);
    }
  }

  QString text;
  for(QAction* ac: this->findChildren<QAction*>(QRegularExpression("(m_a|a)ction\\S*")))
  {
    text = ac->text();
    ac->setText(qApp->translate("MainWindow", text.toUtf8(), 0));

    if (!ac->shortcut().isEmpty())
    {
      ac->setToolTip(ac->toolTip() + " (" + ac->shortcut().toString() + ")");
    }
  }

  ui->actionInstallLocalPackage->setVisible(false);
  ui->actionOpenRootTerminal->setVisible(false);
  toggleTransactionActions(true);
}
