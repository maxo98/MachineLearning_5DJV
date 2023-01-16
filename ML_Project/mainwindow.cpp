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

    std::vector<int> vecX, vecY, blueX, blueY, redX, redY;

    for(int i = 0; i < 10; i++)
    {
        blueX.push_back(randInt(5, 250));
        blueY.push_back(randInt(5, 250));
        redX.push_back(randInt(250, 500-5));
        redY.push_back(randInt(250, 500-5));
    }

    NeuralNetwork network;

    Activation* tanh = new TanhActivation();

    std::vector<Activation*> arrActiv;
    arrActiv.push_back(tanh);

    std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int> allConn;

    Genome gen(3, 1, arrActiv);

    gen.addHiddenNode(tanh, 1);

    gen.addConnection(0, 3, allConn);
    gen.addConnection(1, 3, allConn);
    gen.addConnection(2, 3, allConn);
    gen.addConnection(3, 4, allConn);
    gen.addConnection(2, 4, allConn);

    Neat::genomeToNetwork(gen, network);

    std::vector<float> input, output;

    input.push_back(-1);
    input.push_back(-1);
    input.push_back(1);
    output.push_back(1);

    for (int i = 0; i < 10000; i++)
    {
        for(int i = 0; i < 10; i++)
        {
            output[0] = -1;
            input[0] = blueX[i];
            input[1] = blueY[i];
            network.backprop(input, output, 0.2);

            output[0] = 1;
            input[0] = redX[i];
            input[1] = redY[i];
            network.backprop(input, output, 0.2);
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
