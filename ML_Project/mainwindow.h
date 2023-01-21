#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawCube(int x, int y, QPainter& paint);

private slots:
    void on_pushButton_PMC_clicked();

    void on_pushButton_linear_clicked();

    void on_pushButton_RBF_clicked();

    void on_pushButton_test1_clicked();

    void on_pushButton_test2_clicked();

    void on_pushButton_test3_clicked();

    void on_pushButton_test4_clicked();

private:
    Ui::MainWindow *ui;

    QString model = "linear";
    long long seed;
};
#endif // MAINWINDOW_H
