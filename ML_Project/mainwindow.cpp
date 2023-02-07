#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils.h"
#include <vector>

#include "Neat.h"
#include <random>
#include <QFileDialog>
#include "ThreadPool.h"

#define GREY

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    seed = time(NULL);

    std::cout << "seed " << seed << std::endl;

    srand(seed);//Some random parts don't use this seed

    ui->setupUi(this);

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(lin);

#ifndef GREY
    mainGen = Genome(380*380*3+1, 3, arrActiv);
#else
    mainGen = Genome(380*380+1, 3, arrActiv);
#endif

    ThreadPool* pool = ThreadPool::getInstance();
    pool->start();
}

MainWindow::~MainWindow()
{
    delete tanh;
    delete sig;
    delete gauss;
    delete lin;

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
    if(lockBaseTest.try_lock() == true)
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

        std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(tanh);
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

            network.backprop(input[index], output[index], 0.1, true);
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

        lockBaseTest.unlock();
    }

}


void MainWindow::on_pushButton_test2_clicked()
{
    if(lockBaseTest.try_lock() == true)
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

        std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(tanh);

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

        for (int i = 0; i < 500000; i++)
        {
            int index = randInt(0, 19);

            network.backprop(input[index], output[index], 0.1, true);
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

        lockBaseTest.unlock();
    }
}


void MainWindow::on_pushButton_test3_clicked()
{
    if(lockBaseTest.try_lock() == true)
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

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(tanh);

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

            gen.fullyConnect(1, 2, sig, tanh, allConn, xavierUniformInit, seed);

            Neat::genomeToNetwork(gen, network);
        }else if(model == "RBF")
        {
            qDebug() << "RBF";

            gen.fullyConnect(1, 2, gauss, tanh, allConn, xavierUniformInit, seed);
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


        for (int i = 0; i < 500000; i++)
        {
            int index = randInt(0, 3);

            network.backprop(input[index], output[index], 0.1, true);
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

        lockBaseTest.unlock();
    }
}


void MainWindow::on_pushButton_test4_clicked()
{
    qDebug() << "hello";
    //test4Thread();
    ThreadPool* pool = ThreadPool::getInstance();
    pool->queueJob(&MainWindow::test4Thread, this);
}

void MainWindow::test4Thread()
{
    qDebug() << "hey";
    lockBaseTest.lock();
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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);

    int epoch = 10000000;
    int div = 100000;

    if(model == "linear")
    {
        gen.fullyConnect(0, 0, gauss, tanh, allConn, xavierUniformInit, seed);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        epoch *= 5;
        div *= 5;
        gen.fullyConnect(1, 4, tanh, tanh, allConn, xavierUniformInit, seed);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "RBF")
    {
        qDebug() << "RBF";

        gen.fullyConnect(1, 4, gauss, tanh, allConn, xavierUniformInit, seed);

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

    unsigned int percent = 0;

    for (int i = 0; i < epoch; i++)
    {
        int index = randInt(0, 999);

        network.backprop(input[index], output[index], 0.05 * sig->activate(0.5f - (i/10000000.f)*4), true);

        if(i % div == 0)
        {
            percent++;
            qDebug() << percent;
            ui->label_result->setText("Result: pending " + QString::number(percent) + "%");
        }
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

    lockBaseTest.unlock();
}


void MainWindow::on_pushButton_train_clicked()
{
    ThreadPool* pool = ThreadPool::getInstance();
    pool->queueJob(&MainWindow::train, this);
}

void MainWindow::train()
{
    if(ui->label_netPath->text() == "Pick a network")
    {
        ui->label_mainResult->setText("Pick a network");
        return;
    }

    if(dataFolder == "")
    {
        ui->label_mainResult->setText("No training data set, please pick one");
        return;
    }

    lockMainTest.lock();

    std::deque<std::vector<float>> input, output;

    ui->label_mainResult->setText("Loading data");

    loadData(input, output, dataFolder, true);

    ui->label_mainResult->setText("Data loaded");

    bool validNum;
    double epoch = double(ui->lineEdit_epoch->text().toInt(&validNum));

    if(validNum == false)
    {
        ui->label_mainResult->setText("Error epoch is not a number");
        lockMainTest.unlock();
        return;
    }

    if(useHyper == false)
    {
        float lRate = ui->lineEdit_lRate->text().toFloat(&validNum);

        if(validNum == false)
        {
            ui->label_mainResult->setText("Error learning rate is not a number");
            lockMainTest.unlock();
            return;
        }

        qDebug() << input.size() << " " << input[0].size();
        qDebug() << output.size() << " " << output[0].size();
        qDebug() << epoch;

        unsigned int percent = 0;
        unsigned int div = epoch/100.f;

        if(div == 0) div = 1;

        ui->label_mainResult->setText("Training");

        ui->label_progress->setText("Progress: " + QString::number(percent) + "%");

        for (int i = 0; i < epoch; i++)
        {
            int index = randInt(0, input.size()-1);

            mainNetwork.backprop(input[index], output[index], lRate, true);

            if((i+1) % div == 0)
            {
                percent++;
                qDebug() << percent;

                ui->label_progress->setText("Progress: " + QString::number(percent) + "%");
            }
        }

    }else{
        qDebug() << "hyperneat";

        std::vector<float> fitness;

        fitness.resize(popSize);

        for (int i3 = 0; i3 < epoch; i3++)
        {
            qDebug() << "gen" << i3;
            ui->label_progress->setText("Generation: " + QString::number(i3+1));

            for (int i = 0; i < popSize; i++)
            {
                fitness[i] = 0;
            }

            int threads = 1;
            ThreadPool* pool = ThreadPool::getInstance();
            size_t taskLaunched = pool->getTasksTotal();
            unsigned int cpus = (pool->getThreadPoolSize() >= taskLaunched ? pool->getThreadPoolSize() - taskLaunched : 0);

            float totalWorkload = popSize;
            float workload = (cpus > 1 ? totalWorkload / cpus : totalWorkload);
            float restWorkload = 0;
            int currentWorkload = totalWorkload;
            int startIndex = 0;
            int count = 0;

            if (totalWorkload == 1)
            {
                cpus = 1;
            }

            std::deque<std::atomic<bool>> tickets;

    #ifdef MULTITHREAD
            while (workload < 1 && cpus > 2)
            {
                cpus--;
                workload = totalWorkload / cpus;
            }

            if (workload < 1.f)
            {
                cpus = 0;
            }

            while (cpus > threads)
            {
                currentWorkload = floor(workload);
                float workloadFrac = fmod(workload, 1.0f);
                restWorkload = workloadFrac;

                tickets.emplace_back(false);
                pool->queueJob(&MainWindow::testHyperThread, this, startIndex, currentWorkload + floor(restWorkload), fitness, std::ref(input), std::ref(output), &tickets.back());
                ++threads;

                count += currentWorkload + floor(restWorkload);

                startIndex += currentWorkload + floor(restWorkload);

                restWorkload -= floor(restWorkload);
                restWorkload += workloadFrac;
            }
    #endif //MULTITHREAD

            currentWorkload = totalWorkload - count;

            testHyperThread(startIndex, currentWorkload, fitness, input, output);

            for (std::deque<std::atomic<bool>>::iterator itTicket = tickets.begin(); itTicket != tickets.end(); ++itTicket)
            {
                itTicket->wait(false);
            }

            hyper->setScore(fitness);

            hyper->evolve();
        }

        hyper->genomeToNetwork(*hyper->getGoat(), mainNetwork);
    }

    test();
    lockMainTest.unlock();
}


void MainWindow::on_pushButton_test_clicked()
{
    lockMainTest.lock();
    test();
    lockMainTest.unlock();
}

void MainWindow::testHyperThread(int startIndex, int currentWorkload, std::vector<float>& fitness, const std::deque<std::vector<float>>& input,
                                 const std::deque<std::vector<float>>& output, std::atomic<bool>* ticket)
{
    for (int i = startIndex; i < (startIndex + currentWorkload); i++)
    {
        fitness[i] = testHyper(hyper->getNeuralNetwork(i), input, output);
    }

    if (ticket != nullptr)
    {
        (*ticket) = true;
        ticket->notify_one();
    }
}

float MainWindow::testHyper(NeuralNetwork* network, const std::deque<std::vector<float>>& input,
                                 const std::deque<std::vector<float>>& output)
{
    float score = 0;
    std::vector<float> test;

    //NeuralNetwork net;

    //hyper->genomeToNetwork(*hyper->getGoat(), net);

    for(int i = 0; i < 25; i++)
    {
        int index = randInt(0, input.size()-1);

        network->compute(input[index], test);

        int maxIndex = 0;

        //qDebug() << test[0];

        for(int cpt = 1; cpt < 3; cpt++)
        {
            //qDebug() << test[cpt];

            if(test[maxIndex] < test[cpt])
            {
                maxIndex = cpt;
            }
        }

        for(int cpt = 0; cpt < 3; cpt++)
        {
            if(output[index][cpt] == 1)
            {
                if(maxIndex == cpt)
                {
                    score++;
                }
            }
        }
    }

    return score/2;
}

float MainWindow::test()
{
    if(dataFolder == "")
    {
        ui->label_mainResult->setText("No test data set, please pick one");
        return 0;
    }

    qDebug() << "Start test";
    ui->label_mainResult->setText("Result pending");

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

    ui->label_mainResult->setText("Result: " + QString::number(score) + "%");

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

#ifndef GREY
            itInput->resize(380*380*3+1);
#else
            itInput->resize(380*380+1);
#endif


            for(int x = 0; x < 308; x++)
            {
                for(int y = 0; y < 380; y++)
                {
                    QColor color = image.pixelColor(x, y);

        #ifndef GREY
                    (*itInput)[x * 380 * 3 + y * 3] = color.redF();
                    (*itInput)[x * 380 * 3 + y * 3] = color.greenF();
                    (*itInput)[x * 380 * 3 + y * 3] = color.blueF();
        #else
                    (*itInput)[x * 380 + y] = color.blackF();
        #endif
                }
            }

#ifndef GREY
            (*itInput)[380*380*3] = 0.5f;//Bias
#else
            (*itInput)[380*380] = 0.5f;//Bias
#endif


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
    if(lockMainTest.try_lock() == true)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pick network"), "", tr("*"));

        if (fileName.isEmpty())
        {
            lockMainTest.unlock();
            return;
        }

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(lin);

        mainGen.loadGenome(fileName.toStdString());

        Neat::genomeToNetwork(mainGen, mainNetwork);

        ui->label_netPath->setText(fileName);

        useHyper = false;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_newLinear_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        ui->label_netPath->setText("New linear");

        std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

        mainGen.fullyConnect(0, 0, lin, tanh, allConn, xavierUniformInit, seed);

        Neat::genomeToNetwork(mainGen, mainNetwork);

        useHyper = false;

        lockMainTest.unlock();
    }
}

void MainWindow::on_pushButton_newPmc_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        ui->label_netPath->setText("New PMC");

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

        mainGen.fullyConnect(layer, nodes, tanh, tanh, allConn, heUniformInit, seed);

        Neat::genomeToNetwork(mainGen, mainNetwork);

        useHyper = false;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_newRbf_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        ui->label_netPath->setText("New RBF");

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

        mainGen.fullyConnect(layer, nodes, gauss, tanh, allConn, xavierUniformInit, seed);

        Neat::genomeToNetwork(mainGen, mainNetwork);

        useHyper = false;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_pickTraining_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        QString fileName = QFileDialog::getExistingDirectory(this,"Choose training folder");

        if (fileName.isEmpty())
        {
            lockMainTest.unlock();
            return;
        }

        dataFolder = fileName;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_save_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        if(useHyper == false)
        {
            mainNetwork.applyBackprop(mainGen);
            mainGen.saveCurrentGenome(ui->lineEdit_fileName->text().toStdString());

            ui->label_mainResult->setText("Network saved");
        }else{
            hyper->getGoat()->saveCurrentGenome(ui->lineEdit_fileName->text().toStdString());
            ui->label_mainResult->setText("Network saved");
        }

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_unitTest_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "",
                                                        tr("Images (*.png *.xpm *.jpg)"));

        if(!fileName.isEmpty())
        {
            std::vector<float> input;
            std::vector<float> output;

            QImage image(fileName);

            if(image.width() != 380 && image.height() != 380)
            {
                ui->label_mainResult->setText("Image is of the wrong size");
                lockMainTest.unlock();
                return;
            }

#ifndef GREY
            input.resize(380*380*3+1);
#else
            input.resize(380*380+1);

#endif

            for(int x = 0; x < 308; x++)
            {
                for(int y = 0; y < 380; y++)
                {
                    QColor color = image.pixelColor(x, y);

#ifndef GREY
                    input[x * 380 * 3 + y * 3] = color.redF();
                    input[x * 380 * 3 + y * 3] = color.greenF();
                    input[x * 380 * 3 + y * 3] = color.blueF();
#else
                    input[x * 380 + y] = color.blackF();
#endif

                }
            }

#ifndef GREY
                    input[380*380*3] = 0.5f;//Bias
#else
           input[380*380] = 0.5f;//Bias
#endif

            mainNetwork.compute(input, output);

            int maxIndex = 0;

            qDebug() << output[0];

            for(int cpt = 1; cpt < 3; cpt++)
            {
                qDebug() << output[cpt];

                if(output[maxIndex] < output[cpt])
                {
                    maxIndex = cpt;
                }
            }

            QString strList[3] = {"Hommes", "Femmes", "Autres"};

            ui->label_mainResult->setText("Result " + strList[maxIndex]);

            QPixmap pixmap(fileName);
            ui->label_image->setPixmap(pixmap);
        }
        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_hyperneat_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        ui->label_netPath->setText("New HyperNEAT");
        createHyperNeat(true);

        lockMainTest.unlock();
    }
}

void MainWindow::createHyperNeatClass(bool hyperneat)
{
    NeatParameters neatparam;

    neatparam.activationFunctions.push_back(threshold);
    neatparam.activationFunctions.push_back(abs);
    neatparam.activationFunctions.push_back(sin);
    neatparam.activationFunctions.push_back(gauss);
    neatparam.activationFunctions.push_back(tanh);
    neatparam.activationFunctions.push_back(lin);


    neatparam.pbMutateLink = 0.05;// 0.05;
    neatparam.pbMutateNode = 0.03;//0.03;
    neatparam.pbWeight = 0.9;// 0.9;
    neatparam.pbToggleLink = 0.01;// 0.05;
    neatparam.weightMuteStrength = 1.5;// 2.5;
    neatparam.pbMutateActivation = 0.7;

    neatparam.disjointCoeff = 1.0;
    neatparam.excessCoeff = 1.0;
    neatparam.mutDiffCoeff = 0.4;
    neatparam.activationDiffCoeff = 1.0;
    neatparam.weightCoeff = 0;

    neatparam.killRate = 0.2;

    neatparam.champFileSave = "champ";
    neatparam.avgFileSave = "avg";//Without extension type file
    neatparam.saveChampHistory = true;
    neatparam.saveAvgHistory = true;

    neatparam.pbMateMultipoint = 0.6;
    neatparam.pbMateSinglepoint = 0.0;
    neatparam.interspeciesMateRate = 0.001;
    neatparam.dropOffAge = 15;
    neatparam.ageSignificance = 1.0;
    neatparam.pbMutateOnly = 0.25;
    neatparam.pbMateOnly = 0.2;

    neatparam.speciationDistance = 3;


    neatparam.speciationDistanceMod = 0.5;
    neatparam.minExpectedSpecies = 15;
    neatparam.maxExpectedSpecies = 40;
    neatparam.adaptSpeciation = false;

    neatparam.keepChamp = true;
    neatparam.elistism = true;
    neatparam.rouletteMultiplier = 2.0;

    HyperneatParameters hyperneatParam;

    hyperneatParam.activationFunction = tanh;

    hyperneatParam.cppnOutput = 2;
    hyperneatParam.nDimensions = 2;
    hyperneatParam.thresholdFunction = leoThreshold;
    hyperneatParam.weightModifierFunction = noChangeWeight;

    hyperneatParam.cppnInput = 5;
    hyperneatParam.cppnInputFunction = biasCppnInput;

    float threshold = 0.5;

    float maxDist = 4;

    hyperneatParam.inputVariables.push_back(&maxDist);
    hyperneatParam.thresholdVariables.push_back(&threshold);
    hyperneatParam.weightVariables.push_back(&threshold);

    if(hyper != nullptr)
    {
        delete hyper;
    }


    if(hyperneat == true)
    {
        hyper = new Hyperneat(popSize, neatparam, hyperneatParam);
    }else{

        ES_Parameters esParam;

        esParam.width = 1;

        esParam.initialDepth = 1;
        esParam.maxDepth = 3;
        esParam.bandThreshold = 0.3;
        esParam.iterationLevel = 1;
        esParam.varianceThreshold = 2;
        esParam.allowRecurisvity = false;

        esParam.center.push_back(0);
        esParam.center.push_back(0);

        hyper = new ES_Hyperneat(popSize, neatparam, hyperneatParam, esParam);
    }
}

void MainWindow::createHyperNeat(bool hyperneat)
{
    useHyper = true;

    createHyperNeatClass(hyperneat);

    std::vector<float> pos;
    pos.resize(2);

    for(int x = -190; x < 190; x++)
    {
        for(int y = -190; y < 190; y++)
        {
            pos[0] = x/190.f;
            pos[1] = y/190.f;
            hyper->addInput(pos);
        }
    }

    //Set network output, up, down, left, right

    pos[0] = 1.0;
    pos[1] = 0;
    hyper->addOutput(pos);

    pos[0] = -1.0;
    pos[1] = 0;
    hyper->addOutput(pos);

    pos[0] = 0;
    pos[1] = 1;
    hyper->addOutput(pos);

    qDebug() << "init network";

    hyper->initNetworks();

    qDebug() << "gen network";

    hyper->generateNetworks();
}


void MainWindow::on_pushButton_esHyperneat_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        createHyperNeat(false);

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_pickHyperneat_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pick network"), "", tr("*"));

        if (fileName.isEmpty())
        {
            lockMainTest.unlock();
            return;
        }

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(lin);

        mainGen.loadGenome(fileName.toStdString());

        createHyperNeatClass(true);

        hyper->genomeToNetwork(mainGen, mainNetwork);
        test();

        ui->label_netPath->setText(fileName);

        useHyper = false;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_pickEsHyperneat_clicked()
{
    if(lockMainTest.try_lock() == true)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pick network"), "", tr("*"));

        if (fileName.isEmpty())
        {
            lockMainTest.unlock();
            return;
        }

        std::vector<Activation*> arrActiv;
        arrActiv.push_back(lin);

        mainGen.loadGenome(fileName.toStdString());

        createHyperNeatClass(false);

        hyper->genomeToNetwork(mainGen, mainNetwork);
        test();

        ui->label_netPath->setText(fileName);

        useHyper = false;

        lockMainTest.unlock();
    }
}


void MainWindow::on_pushButton_hybridId_clicked()
{
    useHyper = false;
}

