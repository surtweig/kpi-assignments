#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "perftestdialog.h"
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
    void on_AutocorrelationCheckbox_stateChanged(int arg1);
    void on_DFTCheckbox_stateChanged(int arg1);
    void on_FFTCheckbox_stateChanged(int arg1);
    void on_DFTPhaseCheckbox_stateChanged(int arg1);
    void on_FFTPhaseCheckbox_stateChanged(int arg1);

    void on_perfTestDialogShowButton_clicked();

private:
    Ui::MainWindow *ui;
    PerfTestDialog* perfTestDialog;
    PlotDraw* plotDraw;
    DiscreteSignal* mainSignal;
    DiscreteSignal* mainAutocorrelation;
    DiscreteSignal* mainDFT;
    DiscreteSignal* mainDFTPhase;
    DiscreteSignal* mainFFT;
    DiscreteSignal* mainFFTPhase;
    QColor colorSignal;
    QColor colorAutocorrelation;
    QColor colorDFT;
    QColor colorFFT;

    void generate(quint32 harmonicsNumber, quint32 samplesCount, double deltaTime, double maxAmp);

    void addSignal(DiscreteSignal* &sig, quint32 samplesCount);
    void removeSignal(DiscreteSignal* &sig);

    bool isFourierView();
};
#endif // MAINWINDOW_H
