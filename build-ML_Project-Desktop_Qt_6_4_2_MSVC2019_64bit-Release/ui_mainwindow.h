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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton_test4;
    QPushButton *pushButton_linear;
    QPushButton *pushButton_test1;
    QPushButton *pushButton_test2;
    QPushButton *pushButton_RBF;
    QPushButton *pushButton_test3;
    QPushButton *pushButton_PMC;
    QLabel *label_model;
    QLabel *label_result;
    QLabel *label;
    QWidget *tab_2;
    QPushButton *pushButton_train;
    QLabel *label_netPath;
    QPushButton *pushButton_test;
    QPushButton *pushButton_pick;
    QPushButton *pushButton_newLinear;
    QLineEdit *lineEdit_epoch;
    QLabel *label_model_3;
    QPushButton *pushButton_newPmc;
    QPushButton *pushButton_newRbf;
    QLabel *label_mainResult;
    QProgressBar *progressBar;
    QLineEdit *lineEdit_fileName;
    QLabel *label_model_5;
    QLabel *label_image;
    QPushButton *pushButton_pickTraining;
    QLabel *label_model_6;
    QLineEdit *lineEdit_layer;
    QLabel *label_model_7;
    QLineEdit *lineEdit_nodes;
    QPushButton *pushButton_save;
    QLabel *label_model_8;
    QLineEdit *lineEdit_lRate;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(828, 922);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(20, 0, 781, 871));
        tab = new QWidget();
        tab->setObjectName("tab");
        pushButton_test4 = new QPushButton(tab);
        pushButton_test4->setObjectName("pushButton_test4");
        pushButton_test4->setGeometry(QRect(620, 70, 141, 51));
        pushButton_linear = new QPushButton(tab);
        pushButton_linear->setObjectName("pushButton_linear");
        pushButton_linear->setGeometry(QRect(170, 10, 141, 51));
        pushButton_test1 = new QPushButton(tab);
        pushButton_test1->setObjectName("pushButton_test1");
        pushButton_test1->setGeometry(QRect(170, 70, 141, 51));
        pushButton_test2 = new QPushButton(tab);
        pushButton_test2->setObjectName("pushButton_test2");
        pushButton_test2->setGeometry(QRect(320, 70, 141, 51));
        pushButton_RBF = new QPushButton(tab);
        pushButton_RBF->setObjectName("pushButton_RBF");
        pushButton_RBF->setGeometry(QRect(470, 10, 141, 51));
        pushButton_test3 = new QPushButton(tab);
        pushButton_test3->setObjectName("pushButton_test3");
        pushButton_test3->setGeometry(QRect(470, 70, 141, 51));
        pushButton_PMC = new QPushButton(tab);
        pushButton_PMC->setObjectName("pushButton_PMC");
        pushButton_PMC->setGeometry(QRect(320, 10, 141, 51));
        label_model = new QLabel(tab);
        label_model->setObjectName("label_model");
        label_model->setGeometry(QRect(170, 140, 111, 21));
        QFont font;
        font.setPointSize(14);
        label_model->setFont(font);
        label_result = new QLabel(tab);
        label_result->setObjectName("label_result");
        label_result->setGeometry(QRect(390, 140, 251, 20));
        label_result->setFont(font);
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 180, 500, 500));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButton_train = new QPushButton(tab_2);
        pushButton_train->setObjectName("pushButton_train");
        pushButton_train->setGeometry(QRect(130, 80, 141, 51));
        label_netPath = new QLabel(tab_2);
        label_netPath->setObjectName("label_netPath");
        label_netPath->setGeometry(QRect(130, 40, 551, 31));
        QFont font1;
        font1.setPointSize(12);
        label_netPath->setFont(font1);
        pushButton_test = new QPushButton(tab_2);
        pushButton_test->setObjectName("pushButton_test");
        pushButton_test->setGeometry(QRect(280, 80, 141, 51));
        pushButton_pick = new QPushButton(tab_2);
        pushButton_pick->setObjectName("pushButton_pick");
        pushButton_pick->setGeometry(QRect(430, 80, 141, 51));
        pushButton_newLinear = new QPushButton(tab_2);
        pushButton_newLinear->setObjectName("pushButton_newLinear");
        pushButton_newLinear->setGeometry(QRect(130, 140, 141, 51));
        lineEdit_epoch = new QLineEdit(tab_2);
        lineEdit_epoch->setObjectName("lineEdit_epoch");
        lineEdit_epoch->setGeometry(QRect(140, 210, 113, 21));
        label_model_3 = new QLabel(tab_2);
        label_model_3->setObjectName("label_model_3");
        label_model_3->setGeometry(QRect(10, 210, 111, 21));
        label_model_3->setFont(font1);
        pushButton_newPmc = new QPushButton(tab_2);
        pushButton_newPmc->setObjectName("pushButton_newPmc");
        pushButton_newPmc->setGeometry(QRect(280, 140, 141, 51));
        pushButton_newRbf = new QPushButton(tab_2);
        pushButton_newRbf->setObjectName("pushButton_newRbf");
        pushButton_newRbf->setGeometry(QRect(430, 140, 141, 51));
        label_mainResult = new QLabel(tab_2);
        label_mainResult->setObjectName("label_mainResult");
        label_mainResult->setGeometry(QRect(310, 210, 331, 31));
        label_mainResult->setFont(font);
        progressBar = new QProgressBar(tab_2);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(30, 240, 221, 23));
        progressBar->setValue(0);
        lineEdit_fileName = new QLineEdit(tab_2);
        lineEdit_fileName->setObjectName("lineEdit_fileName");
        lineEdit_fileName->setGeometry(QRect(130, 270, 113, 21));
        label_model_5 = new QLabel(tab_2);
        label_model_5->setObjectName("label_model_5");
        label_model_5->setGeometry(QRect(10, 270, 81, 21));
        label_model_5->setFont(font1);
        label_image = new QLabel(tab_2);
        label_image->setObjectName("label_image");
        label_image->setGeometry(QRect(150, 340, 500, 500));
        pushButton_pickTraining = new QPushButton(tab_2);
        pushButton_pickTraining->setObjectName("pushButton_pickTraining");
        pushButton_pickTraining->setGeometry(QRect(10, 380, 101, 51));
        label_model_6 = new QLabel(tab_2);
        label_model_6->setObjectName("label_model_6");
        label_model_6->setGeometry(QRect(290, 270, 121, 21));
        label_model_6->setFont(font1);
        lineEdit_layer = new QLineEdit(tab_2);
        lineEdit_layer->setObjectName("lineEdit_layer");
        lineEdit_layer->setGeometry(QRect(420, 270, 51, 21));
        label_model_7 = new QLabel(tab_2);
        label_model_7->setObjectName("label_model_7");
        label_model_7->setGeometry(QRect(480, 270, 131, 21));
        label_model_7->setFont(font1);
        lineEdit_nodes = new QLineEdit(tab_2);
        lineEdit_nodes->setObjectName("lineEdit_nodes");
        lineEdit_nodes->setGeometry(QRect(610, 270, 51, 21));
        pushButton_save = new QPushButton(tab_2);
        pushButton_save->setObjectName("pushButton_save");
        pushButton_save->setGeometry(QRect(10, 310, 101, 51));
        label_model_8 = new QLabel(tab_2);
        label_model_8->setObjectName("label_model_8");
        label_model_8->setGeometry(QRect(290, 300, 121, 21));
        label_model_8->setFont(font1);
        lineEdit_lRate = new QLineEdit(tab_2);
        lineEdit_lRate->setObjectName("lineEdit_lRate");
        lineEdit_lRate->setGeometry(QRect(420, 300, 51, 21));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 828, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_test4->setText(QCoreApplication::translate("MainWindow", "Test 4", nullptr));
        pushButton_linear->setText(QCoreApplication::translate("MainWindow", "Lineaire", nullptr));
        pushButton_test1->setText(QCoreApplication::translate("MainWindow", "Test 1", nullptr));
        pushButton_test2->setText(QCoreApplication::translate("MainWindow", "Test 2", nullptr));
        pushButton_RBF->setText(QCoreApplication::translate("MainWindow", "RBF", nullptr));
        pushButton_test3->setText(QCoreApplication::translate("MainWindow", "Test 3", nullptr));
        pushButton_PMC->setText(QCoreApplication::translate("MainWindow", "PMC", nullptr));
        label_model->setText(QCoreApplication::translate("MainWindow", "Linear", nullptr));
        label_result->setText(QCoreApplication::translate("MainWindow", "Result:", nullptr));
        label->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Basic Test", nullptr));
        pushButton_train->setText(QCoreApplication::translate("MainWindow", "Train", nullptr));
        label_netPath->setText(QCoreApplication::translate("MainWindow", "Pick a network", nullptr));
        pushButton_test->setText(QCoreApplication::translate("MainWindow", "Test", nullptr));
        pushButton_pick->setText(QCoreApplication::translate("MainWindow", "Pick network", nullptr));
        pushButton_newLinear->setText(QCoreApplication::translate("MainWindow", "New linear", nullptr));
        lineEdit_epoch->setText(QCoreApplication::translate("MainWindow", "15", nullptr));
        label_model_3->setText(QCoreApplication::translate("MainWindow", "Training epoch", nullptr));
        pushButton_newPmc->setText(QCoreApplication::translate("MainWindow", "New PMC", nullptr));
        pushButton_newRbf->setText(QCoreApplication::translate("MainWindow", "New RBF", nullptr));
        label_mainResult->setText(QCoreApplication::translate("MainWindow", "Result:", nullptr));
        lineEdit_fileName->setText(QCoreApplication::translate("MainWindow", "linear", nullptr));
        label_model_5->setText(QCoreApplication::translate("MainWindow", "File name", nullptr));
        label_image->setText(QString());
        pushButton_pickTraining->setText(QCoreApplication::translate("MainWindow", "Pick data", nullptr));
        label_model_6->setText(QCoreApplication::translate("MainWindow", "Number of layer", nullptr));
        lineEdit_layer->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_model_7->setText(QCoreApplication::translate("MainWindow", "Number of nodes", nullptr));
        lineEdit_nodes->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        pushButton_save->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        label_model_8->setText(QCoreApplication::translate("MainWindow", "Learning rate", nullptr));
        lineEdit_lRate->setText(QCoreApplication::translate("MainWindow", "0.01", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Main subject", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
