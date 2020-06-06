#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotdraw.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generateButton_clicked();

private:
    Ui::MainWindow *ui;
    PlotDraw* plotDraw;
    DiscreteSignal* mainSignal;
    DiscreteSignal* mainAutocorrelation;

    void generate(quint32 harmonicsNumber, quint32 samplesCount, double deltaTime, double maxAmp);
};
#endif // MAINWINDOW_H
