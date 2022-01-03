#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
#include <random>
#include <QVector>
#include <QDebug>
#include <algorithm>
#include <QtMath>
#include <QStringListModel>
#include <QStringList>
#include <QListView>
#include <QSlider>
#include <QLabel>
#include <QPoint>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbCreateGeneration_clicked();

    void on_pbProcess_clicked();

    void on_pbShowGraph_clicked();

    void on_hsIterationAmount_valueChanged(int value);

    void on_pb5BestGraphShow_clicked();

private:
    Ui::MainWindow *ui;
//    double maxX;
//    double minX;
    int chromosomeAmount;
    int geneAmount;
    int Function(QVector<float> x);
    QVector<float> FromIntToFloatArray(int num);
    //int IntFromBoolVector(QVector<float> vec);
    //double IntNormalize(int val);
    QVector<QVector<float>> OnePointMethod(QVector<QVector<float>> parentGeneration);
    //QVector<QVector<float>> MultyPointMethod(QVector<QVector<float>> parentGeneration);
    QVector<QVector<float>> UniformMethod(QVector<QVector<float>> parentGeneration);
    QVector<QVector<float>> GetNextGeneration(QVector<QVector<float>> parentGeneration, QVector<QVector<float>> childGeneration);
    QVector<QVector<float>> Mutation(QVector<QVector<float>> generation);
    void ShowInListView(QVector<QVector<float>> vec, QListView *lv);
    QVector<QVector<float>> firstGeneration;
    QVector<QVector<float>> minGeneration;
    QVector<QVector<float>> middleGeneration;
    QVector<float> maxFitnessSequence;
    QVector<float> aveFitnessSequence;
    QVector<QVector<float>> inputs;
    QVector<float> middle;
    QVector<bool> outputs;
    QVector<bool> etalon;
    void AddGraph(QVector<QVector<float>> thisGeneration, QCustomPlot *Graph);
};

#endif // MAINWINDOW_H
