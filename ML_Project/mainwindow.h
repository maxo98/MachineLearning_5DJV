#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "NeuralNetwork.h"
#include "ES_Hyperneat.h"

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
    void createHyperNeatStructure(bool hyperneat);
    void createHyperNeat(bool hyperneat);
    void testHyperThread(int startIndex, int currentWorkload, std::vector<float>& fitness, const std::deque<std::vector<float>>& input,
                                     const std::deque<std::vector<float>>& output, std::atomic<bool>* ticket = nullptr);
    float testHyper(NeuralNetwork* network, const std::deque<std::vector<float>>& input,
                                     const std::deque<std::vector<float>>& output);

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

    void on_pushButton_hyperneat_clicked();

    void on_pushButton_esHyperneat_clicked();

    void on_pushButton_pickHyperneat_clicked();

    void on_pushButton_pickEsHyperneat_clicked();

    void on_pushButton_hybridId_clicked();

private:
    Ui::MainWindow *ui;

    QString model = "linear";//for basic test only
    long long seed;

    //For main subject
    QString dataFolder = "";
    NeuralNetwork mainNetwork;
    Genome mainGen;

    std::mutex lockBaseTest;
    std::mutex lockMainTest;

    Activation* tanh = new TanhActivation();
    Activation* sig = new SigmoidActivation();
    Activation* gauss = new GaussianActivation();
    Activation* lin = new LinearActivation();
    Activation* sin = new SinActivation();
    Activation* threshold = new ThresholdActivation();
    Activation* abs = new AbsActivation();

    Hyperneat* hyper = nullptr;
    bool useHyper = false;
    int popSize = 150;
};
#endif // MAINWINDOW_H
