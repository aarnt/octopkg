/********************************************************************************
** Form generated from reading UI file 'passworddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDDIALOG_H
#define UI_PASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PasswordDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *commandLayout;
    QLabel *commandL;
    QFrame *line_2;
    QToolButton *commandCopyBtn;
    QLabel *errorL;
    QLabel *descriptionL;
    QLabel *iconL;
    QLabel *backendL;
    QLabel *descCommandL;
    QLabel *descPasswordL;
    QLineEdit *passwordLE;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PasswordDialog)
    {
        if (PasswordDialog->objectName().isEmpty())
            PasswordDialog->setObjectName(QString::fromUtf8("PasswordDialog"));
        PasswordDialog->resize(328, 202);
        PasswordDialog->setWindowTitle(QString::fromUtf8("OctoPkg doas"));
        verticalLayout = new QVBoxLayout(PasswordDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        commandLayout = new QHBoxLayout();
        commandLayout->setSpacing(0);
        commandLayout->setObjectName(QString::fromUtf8("commandLayout"));
        commandL = new QLabel(PasswordDialog);
        commandL->setObjectName(QString::fromUtf8("commandL"));
        commandL->setMinimumSize(QSize(1, 0));
        commandL->setFrameShape(QFrame::Panel);
        commandL->setFrameShadow(QFrame::Sunken);

        commandLayout->addWidget(commandL);

        line_2 = new QFrame(PasswordDialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        commandLayout->addWidget(line_2);

        commandCopyBtn = new QToolButton(PasswordDialog);
        commandCopyBtn->setObjectName(QString::fromUtf8("commandCopyBtn"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("edit-copy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        commandCopyBtn->setIcon(icon);
        commandCopyBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

        commandLayout->addWidget(commandCopyBtn);


        gridLayout->addLayout(commandLayout, 2, 1, 1, 1);

        errorL = new QLabel(PasswordDialog);
        errorL->setObjectName(QString::fromUtf8("errorL"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(errorL->sizePolicy().hasHeightForWidth());
        errorL->setSizePolicy(sizePolicy);

        gridLayout->addWidget(errorL, 1, 1, 1, 1);

        descriptionL = new QLabel(PasswordDialog);
        descriptionL->setObjectName(QString::fromUtf8("descriptionL"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(descriptionL->sizePolicy().hasHeightForWidth());
        descriptionL->setSizePolicy(sizePolicy1);
        descriptionL->setMinimumSize(QSize(230, 0));
        descriptionL->setWordWrap(true);

        gridLayout->addWidget(descriptionL, 0, 1, 1, 1, Qt::AlignVCenter);

        iconL = new QLabel(PasswordDialog);
        iconL->setObjectName(QString::fromUtf8("iconL"));
        iconL->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(iconL, 0, 0, 1, 1);

        backendL = new QLabel(PasswordDialog);
        backendL->setObjectName(QString::fromUtf8("backendL"));
        sizePolicy.setHeightForWidth(backendL->sizePolicy().hasHeightForWidth());
        backendL->setSizePolicy(sizePolicy);
        backendL->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(backendL, 1, 0, 1, 1);

        descCommandL = new QLabel(PasswordDialog);
        descCommandL->setObjectName(QString::fromUtf8("descCommandL"));

        gridLayout->addWidget(descCommandL, 2, 0, 1, 1);

        descPasswordL = new QLabel(PasswordDialog);
        descPasswordL->setObjectName(QString::fromUtf8("descPasswordL"));

        gridLayout->addWidget(descPasswordL, 3, 0, 2, 1);

        passwordLE = new QLineEdit(PasswordDialog);
        passwordLE->setObjectName(QString::fromUtf8("passwordLE"));
        passwordLE->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLE, 3, 1, 2, 1);


        verticalLayout->addLayout(gridLayout);

        line = new QFrame(PasswordDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(PasswordDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setFocusPolicy(Qt::TabFocus);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(passwordLE, buttonBox);
        QWidget::setTabOrder(buttonBox, commandCopyBtn);

        retranslateUi(PasswordDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PasswordDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PasswordDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *PasswordDialog)
    {
#if QT_CONFIG(tooltip)
        commandCopyBtn->setToolTip(QCoreApplication::translate("PasswordDialog", "Copy command to clipboard", nullptr));
#endif // QT_CONFIG(tooltip)
        commandCopyBtn->setText(QCoreApplication::translate("PasswordDialog", "&Copy", nullptr));
        descriptionL->setText(QCoreApplication::translate("PasswordDialog", "The requested action needs administrative privileges.<br>Please enter your password.", nullptr));
#if QT_CONFIG(tooltip)
        backendL->setToolTip(QCoreApplication::translate("PasswordDialog", "LXQt sudo backend", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        backendL->setWhatsThis(QCoreApplication::translate("PasswordDialog", "A program LXQt sudo calls in background to elevate privileges.", nullptr));
#endif // QT_CONFIG(whatsthis)
        descCommandL->setText(QCoreApplication::translate("PasswordDialog", "Command:", nullptr));
        descPasswordL->setText(QCoreApplication::translate("PasswordDialog", "Password:", nullptr));
#if QT_CONFIG(tooltip)
        passwordLE->setToolTip(QCoreApplication::translate("PasswordDialog", "Enter password", nullptr));
#endif // QT_CONFIG(tooltip)
        (void)PasswordDialog;
    } // retranslateUi

};

namespace Ui {
    class PasswordDialog: public Ui_PasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDDIALOG_H
