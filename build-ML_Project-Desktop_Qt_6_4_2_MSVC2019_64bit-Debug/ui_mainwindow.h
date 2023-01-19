/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *pushButton_test1;
    QPushButton *pushButton_test2;
    QPushButton *pushButton_test3;
    QPushButton *pushButton_test4;
    QPushButton *pushButton_linear;
    QPushButton *pushButton_PMC;
    QPushButton *pushButton_RBF;
    QLabel *label_model;
    QLabel *label_result;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(828, 789);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 210, 500, 500));
        pushButton_test1 = new QPushButton(centralwidget);
        pushButton_test1->setObjectName("pushButton_test1");
        pushButton_test1->setGeometry(QRect(110, 90, 141, 51));
        pushButton_test2 = new QPushButton(centralwidget);
        pushButton_test2->setObjectName("pushButton_test2");
        pushButton_test2->setGeometry(QRect(260, 90, 141, 51));
        pushButton_test3 = new QPushButton(centralwidget);
        pushButton_test3->setObjectName("pushButton_test3");
        pushButton_test3->setGeometry(QRect(410, 90, 141, 51));
        pushButton_test4 = new QPushButton(centralwidget);
        pushButton_test4->setObjectName("pushButton_test4");
        pushButton_test4->setGeometry(QRect(560, 90, 141, 51));
        pushButton_linear = new QPushButton(centralwidget);
        pushButton_linear->setObjectName("pushButton_linear");
        pushButton_linear->setGeometry(QRect(110, 30, 141, 51));
        pushButton_PMC = new QPushButton(centralwidget);
        pushButton_PMC->setObjectName("pushButton_PMC");
        pushButton_PMC->setGeometry(QRect(260, 30, 141, 51));
        pushButton_RBF = new QPushButton(centralwidget);
        pushButton_RBF->setObjectName("pushButton_RBF");
        pushButton_RBF->setGeometry(QRect(410, 30, 141, 51));
        label_model = new QLabel(centralwidget);
        label_model->setObjectName("label_model");
        label_model->setGeometry(QRect(120, 160, 111, 21));
        QFont font;
        font.setPointSize(14);
        label_model->setFont(font);
        label_result = new QLabel(centralwidget);
        label_result->setObjectName("label_result");
        label_result->setGeometry(QRect(290, 160, 251, 20));
        label_result->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 828, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        pushButton_test1->setText(QCoreApplication::translate("MainWindow", "Test 1", nullptr));
        pushButton_test2->setText(QCoreApplication::translate("MainWindow", "Test 2", nullptr));
        pushButton_test3->setText(QCoreApplication::translate("MainWindow", "Test 3", nullptr));
        pushButton_test4->setText(QCoreApplication::translate("MainWindow", "Test 4", nullptr));
        pushButton_linear->setText(QCoreApplication::translate("MainWindow", "Lineaire", nullptr));
        pushButton_PMC->setText(QCoreApplication::translate("MainWindow", "PMC", nullptr));
        pushButton_RBF->setText(QCoreApplication::translate("MainWindow", "RBF", nullptr));
        label_model->setText(QCoreApplication::translate("MainWindow", "Linear", nullptr));
        label_result->setText(QCoreApplication::translate("MainWindow", "Result:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
