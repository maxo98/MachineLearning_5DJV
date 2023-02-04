#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils.h"
#include <vector>

#include "Neat.h"
#include <random>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    seed = time(NULL);

    std::cout << "seed " << seed << std::endl;

    srand(seed);//Some random parts don't use this seed

    ui->setupUi(this);

    Activation* lin = new LinearActivation();

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);
    mainGen = Genome(380*380*3+1, 3, arrActiv);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawCube(int x, int y, QPainter& paint)
{
    paint.drawRect(QRect(x-1,y-1,3,3));
}

void MainWindow::on_pushButton_linear_clicked()
{
    model = "linear";
    ui->label_model->setText("Linear");
}

void MainWindow::on_pushButton_PMC_clicked()
{
    model = "PMC";
    ui->label_model->setText("PMC");
}

void MainWindow::on_pushButton_RBF_clicked()
{
    model = "RBF";
    ui->label_model->setText("RBF");
}


void MainWindow::on_pushButton_test1_clicked()
{
    ui->label_result->setText("Result: pending");

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> blueX, blueY, redX, redY;


    blueX.push_back(w/3.5f);
    blueY.push_back(h/3.5f);
    redX.push_back(w/3.5f * 2);
    redY.push_back(h/3.5f * 3);
    redX.push_back(w/3.5f * 3);
    redY.push_back(h/3.5f * 3);

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* sig = new SigmoidActivation();
    Activation* gauss = new GaussianActivation();
    Activation* lin = new LinearActivation();

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);
    Genome gen(3, 1, arrActiv);

    float normalizedXavier = 1/sqrt(3.f);

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "RBF")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<std::vector<float>> input, output;

    input.resize(3);
    output.resize(3);

    input[0].push_back(blueX[0]/float(w));
    input[0].push_back(blueY[0]/float(h));
    input[0].push_back(1);
    output[0].push_back(-1);

    input[1].push_back(redX[0]/float(w));
    input[1].push_back(redY[0]/float(h));
    input[1].push_back(1);
    output[1].push_back(1);

    input[2].push_back(redX[1]/float(w));
    input[2].push_back(redY[1]/float(h));
    input[2].push_back(1);
    output[2].push_back(1);


    for (int i = 0; i < 100000; i++)
    {
        int index = randInt(0, 2);

        network.backprop(input[index], output[index], 0.1, false);
    }

    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0][0] = i/float(w);
            input[0][1] = j/float(h);
            network.compute(input[0], output[0]);

            if(output[0][0] > 0.f)
            {
                paint.setPen(QColor(128, 0, 0, 255));
            }else{
                paint.setPen(QColor(0, 0, 128, 255));
            }

            paint.drawRect(QRect(i,j,1,1));
        }
    }

    bool validated = true;
    QImage imgTest = pix.toImage();

    for(int i = 0; i < blueX.size(); i++)
    {
        if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
        {
            validated = false;
            qDebug() << "blue " << imgTest.pixelColor(blueX[i], blueY[i]).red() << " " << imgTest.pixelColor(blueX[i], blueY[i]).green() << " " << imgTest.pixelColor(blueX[i], blueY[i]).blue();
        }
    }

    for(int i = 0; i < redX.size(); i++)
    {
        if(imgTest.pixel(redX[i], redY[i]) != QColor(128, 0, 0).rgb())
        {
            validated = false;
            qDebug() << "red " << imgTest.pixelColor(redX[i], redY[i]).red() << " " << imgTest.pixelColor(redX[i], redY[i]).green() << " " << imgTest.pixelColor(redX[i], redY[i]).blue();
        }
    }

    paint.setPen(QColor(0, 0, 255, 255));

    for(int i = 0; i < blueX.size(); i++)
    {
        drawCube(blueX[i], blueY[i], paint);

        if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
        {
            validated = false;
            qDebug() << "blue " << imgTest.pixelColor(blueX[i], blueY[i]).red() << " " << imgTest.pixelColor(blueX[i], blueY[i]).green() << " " << imgTest.pixelColor(blueX[i], blueY[i]).blue();
        }
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < redX.size(); i++)
    {
        drawCube(redX[i], redY[i], paint);

        if(imgTest.pixel(redX[i], redY[i]) != QColor(128, 0, 0).rgb())
        {
            validated = false;
            qDebug() << "red " << imgTest.pixelColor(redX[i], redY[i]).red() << " " << imgTest.pixelColor(redX[i], redY[i]).green() << " " << imgTest.pixelColor(redX[i], redY[i]).blue();
        }
    }

    if(validated == true)
    {
        ui->label_result->setText("Result: OK");
    }else{
        ui->label_result->setText("Result: KO");
    }

    ui->label->setPixmap(pix);
}


void MainWindow::on_pushButton_test2_clicked()
{
    ui->label_result->setText("Result: pending");

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> blueX, blueY, redX, redY;

    for(int i = 0; i < 10; i++)
    {
        blueX.push_back(randInt(0, 250));
        blueY.push_back(randInt(0, 250));
        redX.push_back(randInt(251, 499));
        redY.push_back(randInt(251, 499));
    }

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* gauss = new GaussianActivation();
    Activation* lin = new LinearActivation();

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);

    Genome gen(3, 1, arrActiv);

    float normalizedXavier = 1/sqrt(3.f);

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "RBF")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<std::vector<float>> input, output;

    input.resize(20);
    output.resize(20);

    for(int i = 0; i < 10; i++)
    {
        input[i].push_back(blueX[i]/float(w));
        input[i].push_back(blueY[i]/float(h));
        input[i].push_back(1);
        output[i].push_back(-1);
    }

    for(int i = 0; i < 10; i++)
    {
        input[i+10].push_back(redX[i]/float(w));
        input[i+10].push_back(redY[i]/float(h));
        input[i+10].push_back(1);
        output[i+10].push_back(1);
    }

    for (int i = 0; i < 100000; i++)
    {
        int index = randInt(0, 19);

        network.backprop(input[index], output[index], 0.1, false);
    }
    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);


    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0][0] = i/float(w);
            input[0][1] = j/float(h);
            network.compute(input[0], output[0]);

            if(output[0][0] > 0)
            {
                paint.setPen(QColor(128, 0, 0, 255));
            }else{
                paint.setPen(QColor(0, 0, 128, 255));
            }

            paint.drawRect(QRect(i,j,1,1));
        }
    }

    bool validated = true;
    QImage imgTest = pix.toImage();

    for(int i = 0; i < 10; i++)
    {
        if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
        {
            validated = false;
            qDebug() << "blue " << imgTest.pixel(blueX[i], blueY[i]);
        }
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < 10; i++)
    {
        if(imgTest.pixel(redX[i], redY[i]) != QColor(128, 0, 0).rgb())
        {
            validated = false;
            qDebug() << "red " << imgTest.pixel(redX[i], redY[i]);
        }
    }


    paint.setPen(QColor(0, 0, 255, 255));

    for(int i = 0; i < 10; i++)
    {
        drawCube(blueX[i], blueY[i], paint);
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < 10; i++)
    {
        drawCube(redX[i], redY[i], paint);
    }

    if(validated == true)
    {
        ui->label_result->setText("Result: OK");
    }else{
        ui->label_result->setText("Result: KO");
    }

    ui->label->setPixmap(pix);
}


void MainWindow::on_pushButton_test3_clicked()
{

    ui->label_result->setText("Result: pending");

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> blueX, blueY, redX, redY;


    blueX.push_back(1);
    blueY.push_back(1);
    blueX.push_back(w-1);
    blueY.push_back(h-1);
    redX.push_back(w-1);
    redY.push_back(1);
    redX.push_back(1);
    redY.push_back(h-1);

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* lin = new LinearActivation();
    Activation* sig = new SigmoidActivation();
    Activation* gauss = new GaussianActivation();

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);
    float normalizedXavier = 1/sqrt(3.f);

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, 0);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        qDebug() << "PMC";

        gen.fullyConnect(1, 2, sig, lin, allConn, xavierNormalInit, seed);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "RBF")
    {
        qDebug() << "RBF";

        gen.fullyConnect(1, 2, gauss, lin, allConn, xavierNormalInit, seed);
        Neat::genomeToNetwork(gen, network);
    }

    std::vector<std::vector<float>> input, output;

    input.resize(4);
    output.resize(4);

    input[0].push_back(blueX[0]/float(w));
    input[0].push_back(blueY[0]/float(h));
    input[0].push_back(1);
    output[0].push_back(0);

    input[1].push_back(redX[0]/float(w));
    input[1].push_back(redY[0]/float(h));
    input[1].push_back(1);
    output[1].push_back(1);

    input[2].push_back(blueX[1]/float(w));
    input[2].push_back(blueY[1]/float(h));
    input[2].push_back(1);
    output[2].push_back(0);


    input[3].push_back(redX[1]/float(w));
    input[3].push_back(redY[1]/float(h));
    input[3].push_back(1);
    output[3].push_back(1);


    for (int i = 0; i < 100000; i++)
    {
        int index = randInt(0, 3);

        network.backprop(input[index], output[index], 0.1, false);
    }
    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0][0] = i/float(w);
            input[0][1] = j/float(h);
            network.compute(input[0], output[0]);

            if(output[0][0] > 0.5)
            {
                paint.setPen(QColor(128, 0, 0, 255));
            }else{
                paint.setPen(QColor(0, 0, 128, 255));
            }

            paint.drawRect(QRect(i,j,1,1));
        }
    }

    bool validated = true;
    QImage imgTest = pix.toImage();

    for(int i = 0; i < blueX.size(); i++)
    {
        if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
        {
            /*input[0][0] = blueX[i]/float(w);
            input[0][1] = blueY[i]/float(h);
            network.compute(input[0], output[0]);
            qDebug() << output[0];*/
            validated = false;
            qDebug() << "blue " << imgTest.pixelColor(blueX[i], blueY[i]).red() << " " << imgTest.pixelColor(blueX[i], blueY[i]).green() << " " << imgTest.pixelColor(blueX[i], blueY[i]).blue();
        }
    }

    for(int i = 0; i < redX.size(); i++)
    {
        if(imgTest.pixel(redX[i], redY[i]) != QColor(128, 0, 0).rgb())
        {
            /*input[0][0] = redX[i]/float(w);
            input[0][1] = redY[i]/float(h);
            network.compute(input[0], output[0]);
            qDebug() << output[0];*/
            validated = false;
            qDebug() << "red " << imgTest.pixelColor(redX[i], redY[i]).red() << " " << imgTest.pixelColor(redX[i], redY[i]).green() << " " << imgTest.pixelColor(redX[i], redY[i]).blue();
        }
    }

    paint.setPen(QColor(0, 0, 255, 255));

    for(int i = 0; i < blueX.size(); i++)
    {
        drawCube(blueX[i], blueY[i], paint);
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < redX.size(); i++)
    {
        drawCube(redX[i], redY[i], paint);
    }

    if(validated == true)
    {
        ui->label_result->setText("Result: OK");
    }else{
        ui->label_result->setText("Result: KO");
    }

    ui->label->setPixmap(pix);

}


void MainWindow::on_pushButton_test4_clicked()
{
    std::default_random_engine generator (seed);
    ui->label_result->setText("Result: pending");

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> blueX, blueY, redX, redY;

    for(int i = 0; i < 10; i++)
    {
        blueX.push_back(randInt(0, 250));
        blueY.push_back(randInt(0, 250));
        redX.push_back(randInt(251, 499));
        redY.push_back(randInt(251, 499));
    }

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* sig = new SigmoidActivation();
    Activation* lin = new LinearActivation();
    Activation* gauss = new GaussianActivation();

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);

    if(model == "linear")
    {
        gen.fullyConnect(0, 0, gauss, lin, allConn, xavierNormalInit, seed);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        gen.fullyConnect(1, 4, tanh, lin, allConn, xavierNormalInit, seed);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "RBF")
    {
        qDebug() << "RBF";

        gen.fullyConnect(1, 4, gauss, lin, allConn, xavierNormalInit, seed);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<std::vector<float>> input, output;

    input.resize(1000);
    output.resize(1000);

    for(int i = 0; i < 1000; i++)
    {
        blueX.push_back(randInt(0, 499));
        blueY.push_back(randInt(0, 499));

        input[i].push_back(blueX[i]/float(w));
        input[i].push_back(blueY[i]/float(h));
        input[i].push_back(0.5);

        if((blueX[i] <= 150 || blueX[i] >= (500 - 150)) && (blueY[i] <= 150 || blueY[i] >= (500 - 150)))
        {
            output[i].push_back(1);
        }else{
            output[i].push_back(-1);
        }
    }

    for (int i = 0; i < 50000000; i++)
    {
        int index = randInt(0, 999);

        network.backprop(input[index], output[index], 0.05 * sig->activate(0.5f - (i/10000000.f)*4), false);
    }



    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);


    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0][0] = i/float(w);
            input[0][1] = j/float(h);
            network.compute(input[0], output[0]);

            if(output[0][0] > 0)
            {
                paint.setPen(QColor(128, 0, 0, 255));
            }else{
                paint.setPen(QColor(0, 0, 128, 255));
            }

            paint.drawRect(QRect(i,j,1,1));
        }
    }

    bool validated = true;
    QImage imgTest = pix.toImage();

    for(int i = 0; i < 1000; i++)
    {
        if((blueX[i] <= 150 || blueX[i] >= (500 - 150)) && (blueY[i] <= 150 || blueY[i] >= (500 - 150)))
        {
            if(imgTest.pixel(blueX[i], blueY[i]) != QColor(128, 0, 0).rgb())
            {
                validated = false;
                qDebug() << "red " << imgTest.pixel(blueX[i], blueY[i]);
            }

        }else{
            if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
            {
                validated = false;
                qDebug() << "blue " << imgTest.pixel(blueX[i], blueY[i]);
            }
        }
    }

    for(int i = 0; i < 1000; i++)
    {
        if((blueX[i] <= 150 || blueX[i] >= (500 - 150)) && (blueY[i] <= 150 || blueY[i] >= (500 - 150)))
        {
            paint.setPen(QColor(255, 0, 0, 255));
        }else{
            paint.setPen(QColor(0, 0, 255, 255));
        }

        drawCube(blueX[i], blueY[i], paint);
    }

    if(validated == true)
    {
        ui->label_result->setText("Result: OK");
    }else{
        ui->label_result->setText("Result: KO");
    }

    ui->label->setPixmap(pix);
}


void MainWindow::on_pushButton_train_clicked()
{
    if(dataFolder == "")
    {
        ui->label_mainResult->setText("No training data set, please pick one");
        return;
    }

    Activation* sig = new SigmoidActivation();

    std::deque<std::vector<float>> input, output;

    loadData(input, output, dataFolder, true);

    bool validNum;
    double epoch = double(ui->lineEdit_epoch->text().toInt(&validNum));

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error epoch is not a number");
        return;
    }

    float lRate = ui->lineEdit_lRate->text().toFloat(&validNum);

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error learning rate is not a number");
        return;
    }

    qDebug() << input.size() << " " << input[0].size();
    qDebug() << output.size() << " " << output[0].size();
    qDebug() << epoch;

    for (int i = 0; i < epoch; i++)
    {
        int index = randInt(0, input.size()-1);

        qDebug() << i << " " << input[index].size() << " " << output[index].size();

        mainNetwork.backprop(input[index], output[index], lRate, false);
    }

    test();
}


void MainWindow::on_pushButton_test_clicked()
{
    test();
}

float MainWindow::test()
{
    if(dataFolder == "")
    {
        ui->label_mainResult->setText("No test data set, please pick one");
        return 0;
    }

    qDebug() << "Start test";

    std::deque<std::vector<float>> input, output;

    std::vector<float> test;

    loadData(input, output, dataFolder, false);

    float score = 0;

    for(int i = 0; i < input.size(); i++)
    {
        mainNetwork.compute(input[i], test);

        int maxIndex = 0;

        qDebug() << test[0];

        for(int cpt = 1; cpt < 3; cpt++)
        {
            qDebug() << test[cpt];

            if(test[maxIndex] < test[cpt])
            {
                maxIndex = cpt;
            }
        }

        for(int cpt = 0; cpt < 3; cpt++)
        {
            if(output[i][cpt] == 1)
            {
                if(maxIndex == cpt)
                {
                    score++;
                }
            }
        }
    }

    score = score / output.size() * 100;

    qDebug() << score;

    qDebug() << QString::number(score);

    ui->label_mainResult->setText("Result: " + QString::number(score));

    return score;
}

void MainWindow::loadData(std::deque<std::vector<float>>& input, std::deque<std::vector<float>>& output, const QString& folder, bool training)
{
    QString strList[3] = {"/Hommes", "/Femmes", "/Autres"};

    int count = 0;

    for(int i2 = 0; i2 < 3; i2++)
    {
        QString path = folder + strList[i2];

        if(training == true)
        {
            path += "/80%";
        }else{
            path += "/20%";
        }

        QDir dir(path);
        QStringList filter;

        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpeg");
        filter << QLatin1String("*.jpg");

        dir.setNameFilters(filter);

        QFileInfoList filelistinfo = dir.entryInfoList();
        QStringList fileList;

        input.resize(filelistinfo.size() + input.size());
        output.resize(filelistinfo.size() + output.size());

        std::deque<std::vector<float>>::iterator itInput = input.begin() + count;
        std::deque<std::vector<float>>::iterator itOutput = output.begin() + count;

        foreach (const QFileInfo &fileinfo, filelistinfo)
        {
            QImage image(fileinfo.absoluteFilePath());

            itInput->resize(380*380*3+1);

            for(int x = 0; x < 308; x++)
            {
                for(int y = 0; y < 380; y++)
                {
                    QColor color = image.pixelColor(x, y);

                    (*itInput)[x * 380 * 3 + y * 3] = color.redF();
                    (*itInput)[x * 380 * 3 + y * 3] = color.greenF();
                    (*itInput)[x * 380 * 3 + y * 3] = color.blueF();
                }
            }

            (*itInput)[380*380*3] = 0.5f;//Bias

            itOutput->resize(3);

            for(int i = 0; i < 3; i++)
            {
                (*itOutput)[i] = (i == i2 ? 1 : -1);
            }

            ++itInput;
            ++itOutput;
        }

        count = input.size();
    }
}

void MainWindow::on_pushButton_pick_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Pick network"), "", tr("*"));

    if (fileName.isEmpty())
        return;

    Activation* lin = new LinearActivation();
    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);

    mainGen.loadGenome(fileName.toStdString());

    Neat::genomeToNetwork(mainGen, mainNetwork);

    ui->label_netPath->setText(fileName);
}


void MainWindow::on_pushButton_newLinear_clicked()
{
    newNetType = NewNetwork::LINEAR;
    ui->label_netPath->setText("New linear");

    Activation* lin = new LinearActivation();

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    mainGen.fullyConnect(0, 0, lin, lin, allConn, xavierNormalInit, seed);

    Neat::genomeToNetwork(mainGen, mainNetwork);
}

void MainWindow::on_pushButton_newPmc_clicked()
{
    newNetType = NewNetwork::PMC;
    ui->label_netPath->setText("New PMC");

    Activation* tanh = new TanhActivation();
    Activation* lin = new LinearActivation();

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    bool validNum;

    int layer = ui->lineEdit_layer->text().toInt(&validNum);

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error layer is not a number");
    }

    int nodes = ui->lineEdit_nodes->text().toInt(&validNum);

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error nodes is not a number");
    }

    mainGen.fullyConnect(layer, nodes, tanh, lin, allConn, xavierNormalInit, seed);

    Neat::genomeToNetwork(mainGen, mainNetwork);
}


void MainWindow::on_pushButton_newRbf_clicked()
{
    newNetType = NewNetwork::RBF;
    ui->label_netPath->setText("New RBF");

    Activation* lin = new LinearActivation();
    Activation* gauss = new GaussianActivation();

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    bool validNum;

    int layer = ui->lineEdit_layer->text().toInt(&validNum);

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error layer is not a number");
    }

    int nodes = ui->lineEdit_nodes->text().toInt(&validNum);

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error nodes is not a number");
    }

    mainGen.fullyConnect(layer, nodes, gauss, lin, allConn, xavierNormalInit, seed);

    Neat::genomeToNetwork(mainGen, mainNetwork);
}


void MainWindow::on_pushButton_pickTraining_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this,"Choose training folder");

    if (fileName.isEmpty())
        return;

    dataFolder = fileName;
}


void MainWindow::on_pushButton_save_clicked()
{
    mainNetwork.applyBackprop(mainGen);
    mainGen.saveCurrentGenome(ui->lineEdit_fileName->text().toStdString());
}

