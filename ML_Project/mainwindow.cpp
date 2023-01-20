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

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn);
        gen.addConnection(1, 3, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        gen.addConnection(0, 3, allConn);
        gen.addConnection(1, 3, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<float> input, output;

    input.push_back(-1);
    input.push_back(-1);
    input.push_back(1);
    output.push_back(1);

    for (int i = 0; i < 50000; i++)
    {

        output[0] = -1;
        input[0] = blueX[0];
        input[1] = blueY[0];
        network.backprop(input, output, 0.2, false);

        output[0] = 1;
        input[0] = redX[0];
        input[1] = redY[0];
        network.backprop(input, output, 0.2, false);

        output[0] = 1;
        input[0] = redX[1];
        input[1] = redY[1];
        network.backprop(input, output, 0.2, false);
    }
    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);




    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0] = i;
            input[1] = j;
            network.compute(input, output);

            if(output[0] > 0)
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

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn);
        gen.addConnection(1, 3, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        gen.addConnection(0, 3, allConn);
        gen.addConnection(1, 3, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<float> input, output;

    input.push_back(-1);
    input.push_back(-1);
    input.push_back(1);
    output.push_back(1);

    for (int i = 0; i < 100000; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            output[0] = -1;
            input[0] = blueX[j];
            input[1] = blueY[j];
            network.backprop(input, output, 0.2, false);

            output[0] = 1;
            input[0] = redX[j];
            input[1] = redY[j];
            network.backprop(input, output, 0.2, false);
        }
    }
    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);


    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0] = i;
            input[1] = j;
            network.compute(input, output);

            if(output[0] > 0)
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
        if(imgTest.pixel(blueX[i], blueY[i]) != QColor(0, 0, 128).rgb())
        {
            validated = false;
            qDebug() << "blue " << imgTest.pixel(blueX[i], blueY[i]);
        }
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < 10; i++)
    {
        drawCube(redX[i], redY[i], paint);
        if(imgTest.pixel(redX[i], redY[i]) != QColor(128, 0, 0).rgb())
        {
            validated = false;
            qDebug() << "red " << imgTest.pixel(redX[i], redY[i]);
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


void MainWindow::on_pushButton_test3_clicked()
{

    ui->label_result->setText("Result: pending");

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> blueX, blueY, redX, redY;


    blueX.push_back(0);
    blueY.push_back(0);
    blueX.push_back(w);
    blueY.push_back(h);
    redX.push_back(w);
    redY.push_back(0);
    redX.push_back(0);
    redY.push_back(h);

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* lin = new LinearActivation();
    Activation* sig = new SigmoidActivation();

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(sig);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);

    if(model == "linear")
    {
        gen.addConnection(0, 3, allConn);
        gen.addConnection(1, 3, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }else if(model == "PMC")
    {
        qDebug() << "PMC";

        gen.addHiddenNode(sig, 1);
        gen.addHiddenNode(sig, 1);

        gen.addConnection(0, 4, allConn);
        gen.addConnection(1, 4, allConn);
        gen.addConnection(0, 5, allConn);
        gen.addConnection(1, 5, allConn);
        gen.addConnection(4, 3, allConn);
        gen.addConnection(2, 4, allConn);
        gen.addConnection(5, 3, allConn);
        gen.addConnection(2, 5, allConn);
        gen.addConnection(2, 3, allConn);

        Neat::genomeToNetwork(gen, network);
    }

    std::vector<float> input, output;

    input.push_back(-1);
    input.push_back(-1);
    input.push_back(1);
    output.push_back(1);

    for (int i = 0; i < 100000; i++)
    {
        output[0] = 0;
        input[0] = blueX[0]/float(w);
        input[1] = blueY[0]/float(h);
        network.backprop(input, output, 0.1, false);

        output[0] = 1;
        input[0] = redX[0]/float(w);
        input[1] = redY[0]/float(h);
        network.backprop(input, output, 0.1, false);

        output[0] = 0;
        input[0] = blueX[1]/float(w);
        input[1] = blueY[1]/float(h);
        network.backprop(input, output, 0.1, false);

        output[0] = 1;
        input[0] = redX[1]/float(w);
        input[1] = redY[1]/float(h);
        network.backprop(input, output, 0.1, false);
    }
    //network.applyBackprop(gen);

    //Neat::genomeToNetwork(gen, network);




    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0] = i/float(w);
            input[1] = j/float(h);
            network.compute(input, output);

            if(output[0] > 0.5)
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

}

