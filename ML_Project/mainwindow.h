#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "NeuralNetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class NewNetwork{LINEAR, PMC, RBF};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawCube(int x, int y, QPainter& paint);

    float test();

    void loadData(std::deque<std::vector<float>>& input, std::deque<std::vector<float>>& output, const QString& folder, bool training);

    void test4Thread();
    void train();

private slots:
    void on_pushButton_PMC_clicked();

    void on_pushButton_linear_clicked();

    void on_pushButton_RBF_clicked();

    void on_pushButton_test1_clicked();

    void on_pushButton_test2_clicked();

    void on_pushButton_test3_clicked();

    void on_pushButton_test4_clicked();

    void on_pushButton_train_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_pick_clicked();

    void on_pushButton_newLinear_clicked();

    void on_pushButton_newPmc_clicked();

    void on_pushButton_newRbf_clicked();

    void on_pushButton_pickTraining_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_unitTest_clicked();

private:
    Ui::MainWindow *ui;

    QString model = "linear";//for basic test only
    long long seed;

    NewNetwork newNetType = NewNetwork::LINEAR;//For main subject
    QString dataFolder = "";
    NeuralNetwork mainNetwork;
    Genome mainGen;

    std::mutex lockBaseTest;
    std::mutex lockMainTest;

    Activation* tanh = new TanhActivation();
    Activation* sig = new SigmoidActivation();
    Activation* gauss = new GaussianActivation();
    Activation* lin = new LinearActivation();
};
#endif // MAINWINDOW_H
