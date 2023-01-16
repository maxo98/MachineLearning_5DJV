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
    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h);
    QPainter paint(&pix);
    pix.fill( Qt::white );

    std::vector<int> greenX, greenY, blueX, blueY, redX, redY;

    for(int i = 0; i < 5; i++)
    {
        blueX.push_back(randInt(5, 200));
        blueY.push_back(randInt(5, 500-5));
        redX.push_back(randInt(205, 300-5));
        redY.push_back(randInt(5, 500-5));
        greenX.push_back(randInt(300, 500-5));
        greenY.push_back(randInt(300, 500-5));
    }

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();
    Activation* sin = new SinActivation();
    Activation* sig = new SigmoidActivation();
    Activation* lin = new LinearActivation();
    Activation* relu = new ReluActivation();


    std::vector<Activation*> arrActiv;
    arrActiv.push_back(sig);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 3, arrActiv);

    gen.addHiddenNode(sig, 1);
    gen.addHiddenNode(sig, 1);
    gen.addHiddenNode(sig, 1);
    gen.addHiddenNode(sig, 1);
    gen.addHiddenNode(sig, 1);

    gen.addHiddenNode(sig, 2);
    gen.addHiddenNode(sig, 2);
    gen.addHiddenNode(sig, 2);
    gen.addHiddenNode(sig, 2);
    gen.addHiddenNode(sig, 2);

    /*gen.addHiddenNode(sig, 3);
    gen.addHiddenNode(sig, 3);
    gen.addHiddenNode(sig, 3);
    gen.addHiddenNode(sig, 3);
    gen.addHiddenNode(sig, 3);

    gen.addHiddenNode(sig, 4);
    gen.addHiddenNode(sig, 4);
    gen.addHiddenNode(sig, 4);
    gen.addHiddenNode(sig, 4);
    gen.addHiddenNode(sig, 4);*/

    gen.addConnection(0, 6, allConn);
    gen.addConnection(1, 6, allConn);
    gen.addConnection(2, 6, allConn);
    gen.addConnection(0, 7, allConn);
    gen.addConnection(1, 7, allConn);
    gen.addConnection(2, 7, allConn);
    gen.addConnection(0, 8, allConn);
    gen.addConnection(1, 8, allConn);
    gen.addConnection(2, 8, allConn);
    gen.addConnection(0, 9, allConn);
    gen.addConnection(1, 9, allConn);
    gen.addConnection(2, 9, allConn);
    gen.addConnection(0, 10, allConn);
    gen.addConnection(1, 10, allConn);
    gen.addConnection(2, 10, allConn);

    for(int i = 6; i <= 10; i++)
    {
        for(int cpt = 11; cpt <= 15; cpt++)
        {
            gen.addConnection(i, cpt, allConn);
        }
    }
    for(int cpt = 11; cpt <= 15; cpt++)
    {
        gen.addConnection(2, cpt, allConn);
    }

    /*for(int i = 11; i <= 15; i++)
    {
        for(int cpt = 16; cpt <= 20; cpt++)
        {
            gen.addConnection(i, cpt, allConn);
        }
    }
    for(int cpt = 16; cpt <= 20; cpt++)
    {
        gen.addConnection(2, cpt, allConn);
    }*/

    /*for(int i = 16; i <= 20; i++)
    {
        for(int cpt = 21; cpt <= 25; cpt++)
        {
            gen.addConnection(i, cpt, allConn);
        }
    }
    for(int cpt = 21; cpt <= 25; cpt++)
    {
        gen.addConnection(2, cpt, allConn);
    }*/

    for(int i = 11; i <= 15; i++)
    {
        for(int cpt = 3; cpt <= 5; cpt++)
        {
            gen.addConnection(i, cpt, allConn);
        }
    }
    for(int cpt = 3; cpt <= 5; cpt++)
    {
        gen.addConnection(2, cpt, allConn);
    }

    Neat::genomeToNetwork(gen, network);


    std::vector<float> input, output;

    input.push_back(-1);
    input.push_back(-1);
    input.push_back(1);
    output.push_back(-1);
    output.push_back(-1);
    output.push_back(-1);

    for (int i = 0; i < 1000000; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            output[0] = 0;
            output[1] = 1;
            output[2] = 0;
            input[0] = blueX[j]/500.f;
            input[1] = blueY[j]/500.f;
            network.backprop(input, output, 0.4);

            output[0] = 1;
            output[1] = 0;
            output[2] = 0;
            input[0] = redX[j]/500;
            input[1] = redY[j]/500;
            //network.backprop(input, output, 1);

            output[0] = 0;
            output[1] = 0;
            output[2] = 1;
            input[0] = greenX[j]/500.f;
            input[1] = greenY[j]/500.f;
            network.backprop(input, output, 0.4);
        }


    }
    network.applyBackprop(gen);

    Neat::genomeToNetwork(gen, network);

    gen.saveCurrentGenome();


    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            input[0] = i/500.f;
            input[1] = j/500.f;
            network.compute(input, output);

            //std::cout << output << std::endl;

            if(output[0] > output[1])
            {
                if(output[0] > output[2])
                {
                    paint.setPen(QColor(128, 0, 0, 255));
                }else{
                    paint.setPen(QColor(0, 128, 0, 255));
                }

            }else{
                if(output[1] > output[2])
                {
                    paint.setPen(QColor(0, 0, 128, 255));
                }else
                {
                    paint.setPen(QColor(0, 128, 0, 255));
                }
            }

            paint.drawRect(QRect(i,j,1,1));
        }
    }

    paint.setPen(QColor(0, 0, 255, 255));

    for(int i = 0; i < 5; i++)
    {
        drawCube(blueX[i], blueY[i], paint);
    }

    paint.setPen(QColor(255, 0, 0, 255));

    for(int i = 0; i < 5; i++)
    {
        //drawCube(redX[i], redY[i], paint);
    }

    paint.setPen(QColor(0, 255, 0, 255));

    for(int i = 0; i < 5; i++)
    {
        drawCube(greenX[i], greenY[i], paint);
    }

    ui->label->setPixmap(pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawCube(int x, int y, QPainter& paint)
{
    paint.drawRect(QRect(x-5,y-5,10,10));
}
