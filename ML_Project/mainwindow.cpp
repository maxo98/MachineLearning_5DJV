#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils.h"
#include <vector>
#include "NeuralNetwork.h"
#include "Neat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    auto seed = time(NULL);

    std::cout << "seed " << seed << std::endl;

    srand(seed);//Some random parts don't use this seed

    ui->setupUi(this);

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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(sig);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

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
        qDebug() << "PMC";

        gen.addHiddenNode(sig, 1);
        gen.addHiddenNode(sig, 1);

        gen.addConnection(0, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(0, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(4, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(5, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

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

        gen.addHiddenNode(tanh, 1);
        gen.addHiddenNode(tanh, 1);
        gen.addHiddenNode(tanh, 1);
        gen.addHiddenNode(tanh, 1);

        gen.addConnection(0, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(0, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 6, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(0, 6, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(1, 7, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(0, 7, allConn, randPosNeg() * randFloat() * normalizedXavier);

        gen.addConnection(4, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(5, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(6, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(7, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);

        gen.addConnection(2, 4, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 5, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 3, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 6, allConn, randPosNeg() * randFloat() * normalizedXavier);
        gen.addConnection(2, 7, allConn, randPosNeg() * randFloat() * normalizedXavier);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<std::vector<float>> input, output;

    input.resize(500);
    output.resize(500);

    for(int i = 0; i < 500; i++)
    {
        blueX.push_back(randInt(0, 499));
        blueY.push_back(randInt(0, 499));

        input[i].push_back(blueX[i]/float(w));
        input[i].push_back(blueY[i]/float(h));
        input[i].push_back(1);

        if((blueX[i] <= 150 || blueX[i] >= (500 - 150)) && (blueY[i] <= 150 || blueY[i] >= (500 - 150)))
        {
            output[i].push_back(1);
        }else{
            output[i].push_back(-1);
        }

    }

    for (int i = 0; i < 10000000; i++)
    {
        int index = randInt(0, 499);

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

    for(int i = 0; i < 500; i++)
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

    for(int i = 0; i < 500; i++)
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

