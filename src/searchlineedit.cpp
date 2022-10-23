/*
  Source code extracted from:
  http://www.jakepetroules.com/2011/07/10/creating-a-windows-explorer-style-search-box-in-qt

  Written by Jake Petroules
  Adapted to suit QTGZManager
*/

#include "searchlineedit.h"
#include "strconstants.h"
#include "wmhelper.h"
#include "uihelper.h"
#include "iostream"

#include <QApplication>
#include <QToolButton>
#include <QStyle>
#include <QRegularExpressionValidator>
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>
//#include <QDebug>

SearchLineEdit::SearchLineEdit(QWidget *parent, bool hasSLocate) :
  QLineEdit(parent){

  m_hasLocate = hasSLocate;
  m_completerModel = new QStringListModel(this);
  m_completer = new QCompleter(m_completerModel, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCompletionColumn(0);
  m_completer->setMaxVisibleItems(10);

  setCompleter(m_completer);

  // Create the search button and set its icon, cursor, and stylesheet
  this->m_SearchButton = new QToolButton(this);
  this->m_SearchButton->setFocusPolicy(Qt::NoFocus);

  // Increase button size a bit for kde
  if (WMHelper::isKDERunning())
    this->m_SearchButton->setFixedSize(18, 18);
  else
    this->m_SearchButton->setFixedSize(16, 16);

  this->m_SearchButton->setCursor(Qt::ArrowCursor);
  this->m_SearchButton->setStyleSheet(this->buttonStyleSheetForCurrentState());

  m_defaultValidator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\-\\$\\^\\*\\+\\(\\)\\[\\]\\.\\s]+"), this);
  m_aurValidator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\-\\s\\+\\$\\^]+"), this);
  m_fileValidator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\-\\/\\.]+"), this);

  setValidator(m_defaultValidator);

  // Update the search button when the text changes
  QObject::connect(this, SIGNAL(textChanged(QString)), SLOT(updateSearchButton(QString)));

  // Some stylesheet and size corrections for the text box
  this->setPlaceholderText(StrConstants::getFind());
  this->setStyleSheet(this->styleSheetForCurrentState());
  this->setFocusPolicy(Qt::StrongFocus);
}

/*
 * Refreshes the validator used in QLineEdit depending on the options choosed by the user
 */
void SearchLineEdit::setRefreshValidator(ValidatorType validatorType)
{
  if (validatorType == ectn_AUR_VALIDATOR)
  {
    m_completerModel->setStringList(QStringList());
    setValidator(m_aurValidator);
  }
  else if (validatorType == ectn_FILE_VALIDATOR)
    setValidator(m_fileValidator);
  else if (validatorType == ectn_DEFAULT_VALIDATOR)
  {
    m_completerModel->setStringList(QStringList());
    setValidator(m_defaultValidator);
  }

  //If the current string is not valid anymore, let's erase it!
  int pos = 0;
  QString search = text();
  if (this->validator()->validate(search, pos) == QValidator::Invalid)
    setText("");
}

/*
 * Refreshes completer data used in QLineEdit if slocate is installed
 */
void SearchLineEdit::refreshCompleterData()
{
  if (m_hasLocate)
  {
    QStringList sl = UnixCommand::getFilePathSuggestions(text());

    if (sl.count() > 0)
    {
      m_completerModel->setStringList(sl);
    }
  }
}

void SearchLineEdit::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
    this->m_SearchButton->move(5, (this->rect().height() - this->m_SearchButton->height()) / 2);
}

void SearchLineEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_U && event->modifiers() == Qt::ControlModifier)
  {
    event->ignore();
  }
  else
  {
    return QLineEdit::keyPressEvent(event);
  }
}

void SearchLineEdit::updateSearchButton(const QString &text)
{
  if (!text.isEmpty()){
    // We have some text in the box - set the button to clear the text
    QObject::connect(this->m_SearchButton, SIGNAL(clicked()), SLOT(clear()));
  }
  else{
    // The text box is empty - make the icon do nothing when clicked
    QObject::disconnect(this->m_SearchButton, SIGNAL(clicked()), this, SLOT(clear()));
  }

  this->m_SearchButton->setStyleSheet(this->buttonStyleSheetForCurrentState());
}

QString SearchLineEdit::styleSheetForCurrentState()
{
  int frameWidth = 1;
  QString style;
  style += "QLineEdit {";

  if (this->text().isEmpty())
  {
    style += "font-family: 'MS Sans Serif';";
    style += "font-style: italic;";
  }
  else
  {
    QFont font(QApplication::font());
    font.setItalic(true);
    setFont(font);
  }

  style += "padding-left: 20px;";
  style += QString("padding-right: %1px;").arg(this->m_SearchButton->sizeHint().width() + frameWidth + 1);
  style += "border-width: 3px;}";
  //style += "border-image: url(:/resources/images/esf-border.png) 3 3 3 3 stretch;";
  //style += "background-color: rgba(255, 255, 255, 255);"; //204);";
  //style += "color: black;}";

  return style;
}

void SearchLineEdit::setFoundStyle(){
  QString style;
  style += "QLineEdit {";

  style += "font-family: 'MS Sans Serif';";
  style += "font-style: italic;";
  style += "padding-left: 20px;";
  style += QString("padding-right: %1px;").arg(this->m_SearchButton->sizeHint().width() + 2);
  style += "border-width: 3px;}";
  //style += "border-image: url(:/resources/images/esf-border.png) 3 3 3 3 stretch;";
  //style += "color: black; ";
  //style += "background-color: rgb(255, 255, 200);";
  //style += "border-color: rgb(206, 204, 197);}";
  setStyleSheet(style);

  /*else
  // setPalette() must be called after setStyleSheet()
  {
    style += "padding-left: 20px;}";
    setStyleSheet(style);

    QPalette palette(QApplication::palette());
    palette.setColor(QPalette::Base, QColor(255, 255, 200));
    palette.setColor(QPalette::Text, Qt::darkGray); // give more contrast to text
    setPalette(palette);
  }*/
}

void SearchLineEdit::setNotFoundStyle(){
  QString style;
  style += "QLineEdit {";
  style += "font-family: 'MS Sans Serif';";
  style += "font-style: italic;";
  style += "padding-left: 20px;";
  style += QString("padding-right: %1px;").arg(this->m_SearchButton->sizeHint().width() + 2);
  style += "border-width: 3px;}";
  //style += "border-image: url(:/resources/images/esf-border.png) 3 3 3 3 stretch;";
  //style += "color: white; ";
  //style += "background-color: lightgray;"; //rgb(255, 108, 108); //palette(mid);"; //rgb(207, 135, 142);";
  //style += "border-color: rgb(206, 204, 197);}";
  setStyleSheet(style);
}

QString SearchLineEdit::buttonStyleSheetForCurrentState() const
{
  // When using KDE avoid stylesheet customization
  if (WMHelper::isKDERunning()) {
    this->text().isEmpty() ? this->m_SearchButton->setIcon(IconHelper::getIconSearch())
                           : this->m_SearchButton->setIcon(IconHelper::getIconClear());

    if (!this->text().isEmpty())
      this->m_SearchButton->setToolTip(StrConstants::getClear());
    else
      this->m_SearchButton->setToolTip("");

    this->m_SearchButton->setAutoRaise(true);
    return QString();
  }

  QString style;
  style += "QToolButton {";
  style += "border: none; margin: 0; padding: 0;";
  style += QString("background-image: url(:/resources/images/esf-%1.png);").arg(this->text().isEmpty() ? "search" : "clear");
  style += "}";

  if (!this->text().isEmpty())
  {
    style += "QToolButton:pressed { background-image: url(:/resources/images/esf-clear.png); }";
    this->m_SearchButton->setToolTip(StrConstants::getClear());
  }
  else this->m_SearchButton->setToolTip("");

  return style;
}
