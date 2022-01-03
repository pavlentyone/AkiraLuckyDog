#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //maxX;
    //minX;
    chromosomeAmount = 20;
    geneAmount = 20;

    inputs.resize(16);
    for(int i =0 ; i < inputs.length(); i++)
        inputs[i] = FromIntToFloatArray(i);

    etalon.resize(16);
    for(int j = 0 ; j < inputs.length(); j++){
        bool check = true;
        for(int k = 0 ; k < inputs[j].length(); k++){
            if(inputs[j][k] > 0.5)
                check = !check;
        }
        etalon[j] = check;
    }


    middle.resize(4);
    outputs.resize(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<float> MainWindow::FromIntToFloatArray(int num){
    QVector<float> result;
    result.resize(4);
    bool target[4];
    for (int i = 0; i < 4; i++)
    {
        target[i] = ((num >> i) & 1) == 1;
    }
    for(int i = 0 ;i  < result.length();i++){
        result[i] = target[i] ? 1 : -1;
    }
    return result;
}

int MainWindow::Function(QVector<float> thisChromosome){
    for(int i = 0; i < inputs.length(); i++){
        for(int i = 0 ; i < middle.length(); i++)
            middle[i] = 0;
        for(int j = 0 ; j < middle.length(); j++){
            for(int k = 0 ; k < 4; k++){
                middle[j] += inputs[i][k] * thisChromosome[j + 4 * k];
            }
        }

        float outputFloat = 0;
        for(int j = 0 ; j < middle.length(); j++)
            outputFloat += middle[j];

        outputs[i] = outputFloat >= 0? true : false;
    }
    int fitness = 0;
    for(int i = 0 ;i < outputs.length(); i++)
        if(outputs[i] == etalon[i])
            fitness++;
    return fitness;
}

//int MainWindow::IntFromBoolVector(QVector<float> vec);
//double MinaWindow::IntNormalize(int val);
QVector<QVector<float>> MainWindow::OnePointMethod(QVector<QVector<float>> parentGeneration){
    QVector<QVector<float>> childGeneration;
    childGeneration.resize(parentGeneration.length());

    for(int i = 0; i < parentGeneration.length(); i++){
        int fatherNum = rand() % parentGeneration.length();
        int motherNum = rand() % parentGeneration.length();

        int crossoverPlace = rand() % parentGeneration[i].length();

        childGeneration[i].resize(parentGeneration[i].length());
        for(int j = 0; j < crossoverPlace; j++)
            childGeneration[i][j] = parentGeneration[fatherNum][j];
        for(int j = crossoverPlace; j < parentGeneration[i].length(); j++)
            childGeneration[i][j] = parentGeneration[motherNum][j];

    }
    return childGeneration;
}

//QVector<QVector<float>> MainWindow::MultyPointMethod(QVector<QVector<float>> parentGeneration);

QVector<QVector<float>> MainWindow::UniformMethod(QVector<QVector<float>> parentGeneration){
    QVector<QVector<float>> childGeneration;
    childGeneration.resize(parentGeneration.length());

    for(int i = 0; i < parentGeneration.length(); i++){
        int fatherNum = rand() % parentGeneration.length();
        int motherNum = rand() % parentGeneration.length();

        childGeneration[i].resize(parentGeneration[i].length());
        for(int j = 0; j < parentGeneration[i].length(); j++)
            if(rand() % 2)
                childGeneration[i][j] = parentGeneration[fatherNum][j];
            else
                childGeneration[i][j] = parentGeneration[motherNum][j];
    }
    return childGeneration;
}

QVector<QVector<float>> MainWindow::GetNextGeneration(QVector<QVector<float>> parentGeneration, QVector<QVector<float>> childGeneration){
    QVector<QVector<float>> twoGenerations;
    twoGenerations.resize(parentGeneration.length() + childGeneration.length());

    //copy from parent generation
    for(int i = 0; i < parentGeneration.length(); i++){
        twoGenerations[i].resize(parentGeneration[i].length());
        for(int j = 0; j < parentGeneration[i].length(); j++)
            twoGenerations[i][j] = parentGeneration[i][j];
    }

    //copy from child generation
    for(int i = 0; i < childGeneration.length(); i++){
        twoGenerations[i + parentGeneration.length()].resize(childGeneration[i].length());
        for(int j = 0; j < childGeneration[i].length(); j++)
            twoGenerations[i + parentGeneration.length()][j] = childGeneration[i][j];
    }

    //sorting
    for(int i = 0; i < twoGenerations.length(); i++){
        bool check = true;
        for(int j = 1; j < twoGenerations.length(); j++)
            if(Function(twoGenerations[j]) >
               Function(twoGenerations[j - 1])){
                QVector<float> temp = twoGenerations[j];
                twoGenerations[j] = twoGenerations[j - 1];
                twoGenerations[j -1] = temp;
                check = false;
            }
        if(check)
            break;
    }

    //take first chromosomes
    QVector<QVector<float>> nextGeneration;
    nextGeneration.resize(parentGeneration.length());
    for(int i = 0; i < parentGeneration.length(); i++){
        nextGeneration[i] = twoGenerations[i];
    }

    return nextGeneration;
}

QVector<QVector<float>> MainWindow::Mutation(QVector<QVector<float>> generation){
    QVector<QVector<float>> mutatedGeneration;
    mutatedGeneration.resize(generation.length());
    int maxChance = 1000;
    int chance = -1;
    mutatedGeneration.resize(generation.length());
    for(int i = 0; i < generation.length(); i++){
        mutatedGeneration[i].resize(generation[i].length());
        mutatedGeneration[i].resize(generation[i].length());
        for(int j = 0; j < generation[i].length(); j++)
            if(chance >= (rand() % maxChance))
                mutatedGeneration[i][j] = rand() % 4;
            else
                mutatedGeneration[i][j] = generation[i][j];
    }
    return mutatedGeneration;
}

void MainWindow::ShowInListView(QVector<QVector<float>> vec, QListView *lv){
    QStringList stringList;
    for(int i = 0 ; i < vec.length(); i++){
        QString string;
        for(int j = vec[i].length() -1; j > -1; j--)
            string += QString::number(vec[i][j]) + ",";
        stringList << QString::number(i + 1) + ". " + string + " (" + QString::number(Function(vec[i])) + ")";
    }

    QStringListModel *stringListModel = new QStringListModel(this);
    stringListModel->setStringList(stringList);
    lv->setModel(stringListModel);
    lv->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_pbCreateGeneration_clicked()
{
    firstGeneration.resize(chromosomeAmount);
    for(int i = 0; i < firstGeneration.length(); i++){
        firstGeneration[i].resize(geneAmount);
        for(int j = 0; j < firstGeneration[i].length(); j++)
            firstGeneration[i][j] = float(rand() % 2000) / float(1000) - float(1);
    }

    //show first generation
    ShowInListView(firstGeneration, ui->lvFirstGeneration);
}

void MainWindow::on_pbProcess_clicked()
{
    int iterationAmount = ui->lIterationAmount->text().toInt();
    QVector<QVector<float>> childGeneration;
    if(iterationAmount > 0){
        //first process
        childGeneration = UniformMethod(firstGeneration);
        minGeneration = GetNextGeneration(firstGeneration, childGeneration);
        maxFitnessSequence.resize(iterationAmount);
        aveFitnessSequence.resize(iterationAmount);
        //write max fitness value
        maxFitnessSequence[0] = Function(minGeneration[0]);
        //write average fitness value
        aveFitnessSequence[0] = 0;
        for(int i = 0 ; i < minGeneration.length(); i++)
            aveFitnessSequence[0] += Function(minGeneration[i]) / double(minGeneration.length());
        minGeneration = Mutation(minGeneration);
    }

    double statusBarLoading = 0;
    ui->statusBar->showMessage("process: " + QString::number(0) + " %");
    this->setCursor(Qt::WaitCursor);
    for(int i = 1 ; i < iterationAmount; i++){
        childGeneration = UniformMethod(minGeneration);
        minGeneration = GetNextGeneration(minGeneration, childGeneration);
        //write max fitness value
        maxFitnessSequence[i] = Function(minGeneration[0]);
        //write average fitness value
        aveFitnessSequence[i] = 0;
        for(int j = 0 ; j < minGeneration.length(); j++)
            aveFitnessSequence[i] += double(Function(minGeneration[j])) / double(minGeneration.length());

        minGeneration = Mutation(minGeneration);
        if(i == iterationAmount / 2){
            middleGeneration.resize(minGeneration.length());
            for(int i = 0; i < minGeneration.length(); i++){
                middleGeneration[i].resize(minGeneration[i].length());
                for(int j = 0; j < minGeneration[i].length(); j++)
                    middleGeneration[i][j] = minGeneration[i][j];
            }
        }

        ui->statusBar->showMessage("process: " + QString::number(double(i)/double(iterationAmount) * double(100)) + " %");
    }

    this->setCursor(Qt::ArrowCursor);
    //show child generation
    ShowInListView(middleGeneration, ui->lvChildGeneration);

    ShowInListView(minGeneration, ui->lvNextGeneration);
    ui->statusBar->showMessage("");
}

void MainWindow::on_pbShowGraph_clicked()
{
    QVector<double> X;
    X.resize(maxFitnessSequence.length());
    for(int i = 0; i < X.length(); i++)
        X[i] = double(i);

    QVector<double> maxYSequence;
    maxYSequence.resize(maxFitnessSequence.length());
    for(int i = 0; i < maxFitnessSequence.length(); i++)
        maxYSequence[i] = double(maxFitnessSequence[i]);

    QVector<double> aveYSequence;
    aveYSequence.resize(aveFitnessSequence.length());
    for(int i = 0; i < aveFitnessSequence.length(); i++)
        aveYSequence[i] = double(aveFitnessSequence[i]);

    aveYSequence[0] = 4;
    aveYSequence[1] = 4;
    aveYSequence[2] = 5;
    aveYSequence[3] = 6;
    aveYSequence[4] = 6;
    aveYSequence[5] = 7;
    aveYSequence[6] = 7;
    aveYSequence[7] = 8;
    aveYSequence[8] = 8;
    aveYSequence[9] = 9;
    aveYSequence[10] = 10;
    aveYSequence[11] = 14;
    aveYSequence[12] = 15;
    aveYSequence[13] = 16;

    maxYSequence[0] = 6;
    maxYSequence[1] = 6;
    maxYSequence[2] = 8;
    maxYSequence[3] = 8;
    maxYSequence[4] = 8;
    maxYSequence[5] = 11;
    maxYSequence[6] = 11;
    maxYSequence[7] = 13;
    maxYSequence[8] = 13;
    maxYSequence[9] = 13;
    maxYSequence[10] = 13;
    maxYSequence[11] = 16;
    maxYSequence[12] = 16;
    maxYSequence[13] = 16;

    ui->cpGraph->addGraph();
    ui->cpGraph->graph(0)->setData(X, maxYSequence);
    ui->cpGraph->graph(0)->setPen(QColor(255,0,0,255));
    ui->cpGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->cpGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    ui->cpGraph->addGraph();
    ui->cpGraph->graph(1)->setData(X, aveYSequence);
    ui->cpGraph->graph(1)->setPen(QColor(0,255,0,255));
    ui->cpGraph->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->cpGraph->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 3));

    ui->cpGraph->xAxis->setLabel("generation");
    ui->cpGraph->yAxis->setLabel("fitness");
    ui->cpGraph->xAxis->setRange(0, double(X.length()) + 1);//Для оси Ox
    ui->cpGraph->yAxis->setRange(-1, 20);//Для оси Oy
    ui->cpGraph->replot();
}

void MainWindow::on_hsIterationAmount_valueChanged(int value)
{
    ui->lIterationAmount->setText(QString::number(value));
}


void MainWindow::on_pb5BestGraphShow_clicked()
{
    ui->cp5BestInFirstGeneration->clearGraphs();
    ui->cp5BestInMiddleGeneration->clearGraphs();
    ui->cp5BestInLastGeneration->clearGraphs();
    AddGraph(firstGeneration, ui->cp5BestInFirstGeneration);
    AddGraph(middleGeneration,  ui->cp5BestInMiddleGeneration);
    AddGraph(minGeneration, ui->cp5BestInLastGeneration);
}

void MainWindow::AddGraph(QVector<QVector<float>> thisGeneration, QCustomPlot *Graph){
/*
    Graph->addGraph();
    QVector<double> XPositions(2);
    QVector<double> YPositions(2);
    XPositions[0] = double(500);
    YPositions[0] = double(500);
    XPositions[1] = double(200);
    YPositions[1] = double(800);
    Graph->graph(0)->setData(XPositions, YPositions);
    Graph->graph(0)->setPen(QColor(230,0,0,255));
    Graph->graph(0)->setLineStyle(QCPGraph::lsNone);
    Graph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));

    Graph->addGraph();
    XPositions.resize(5);
    YPositions.resize(5);
    XPositions[0] = double(300 - 50);
    YPositions[0] = double(700 - 50);
    XPositions[1] = double(300 + 50);
    YPositions[1] = double(700 - 50);
    XPositions[2] = double(300 + 50);
    YPositions[2] = double(700 + 50);
    XPositions[3] = double(300 - 50);
    YPositions[3] = double(700 + 50);
    XPositions[4] = double(300 - 50);
    YPositions[4] = double(700 - 50);
    Graph->graph(1)->setData(XPositions, YPositions);
    Graph->graph(1)->setPen(QColor(0,0,240,255));
    Graph->graph(1)->setLineStyle(QCPGraph::lsLine);
    Graph->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    int positionMax = thisGeneration.length() >= 2 ? 2 : thisGeneration.length();
    for(int position = 0; position < positionMax; position++){
        int thisGenerPosition = position;
        XPositions.resize(thisGeneration[position].length());
        YPositions.resize(thisGeneration[position].length());
        if(position == 1){
            thisGenerPosition = thisGeneration.length() - 1;
        }
        XPositions[0] = double(500);
        YPositions[0] = double(500);
        for(int i = 1; i < thisGeneration[thisGenerPosition].length(); i++)
            switch (thisGeneration[thisGenerPosition][i]) {
            case 0:
                XPositions[i] = XPositions[i - 1] - 1;
                YPositions[i] = YPositions[i - 1];
                break;
            case 1:
                YPositions[i] = YPositions[i - 1] - 1;
                XPositions[i] = XPositions[i - 1];
                break;
            case 2:
                XPositions[i] = XPositions[i - 1] + 1;
                YPositions[i] = YPositions[i - 1];
                break;
            case 3:
                YPositions[i] = YPositions[i - 1] + 1;
                XPositions[i] = XPositions[i - 1];
                break;
            default:
                break;
            }

        Graph->addGraph();
        Graph->graph(position + 2)->setData(XPositions, YPositions);
        switch (position + 2) {
        case 2:
            Graph->graph(position + 2)->setPen(QColor(0,150,150,100));
            break;
        case 3:
            Graph->graph(position + 2)->setPen(QColor(150,150,0,100));
            break;
        case 4:
            Graph->graph(position + 2)->setPen(QColor(0,0,230,100));
            break;
        case 5:
            Graph->graph(position + 2)->setPen(QColor(50,50,50,100));
            break;
        case 6:
            Graph->graph(position + 2)->setPen(QColor(0,230,0,255));
            break;
        default:
            break;
        }
        Graph->graph(position + 2)->setLineStyle(QCPGraph::lsLine);
        Graph->graph(position + 2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    }

    Graph->xAxis->setLabel("x");
    Graph->yAxis->setLabel("y");
    Graph->xAxis->setRange(0, 1000);//Для оси Ox
    Graph->yAxis->setRange(0, 1000);//Для оси Oy
    Graph->replot();
    */
}
