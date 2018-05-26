/********************************************************************************
** Form generated from reading UI file 'draganddrop.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAGANDDROP_H
#define UI_DRAGANDDROP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_draganddropClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *draganddropClass)
    {
        if (draganddropClass->objectName().isEmpty())
            draganddropClass->setObjectName(QStringLiteral("draganddropClass"));
        draganddropClass->resize(600, 400);
        menuBar = new QMenuBar(draganddropClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        draganddropClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(draganddropClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        draganddropClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(draganddropClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        draganddropClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(draganddropClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        draganddropClass->setStatusBar(statusBar);

        retranslateUi(draganddropClass);

        QMetaObject::connectSlotsByName(draganddropClass);
    } // setupUi

    void retranslateUi(QMainWindow *draganddropClass)
    {
        draganddropClass->setWindowTitle(QApplication::translate("draganddropClass", "draganddrop", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class draganddropClass: public Ui_draganddropClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAGANDDROP_H
