#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QtMath>
#include <QDateTime>
#include "sineseriesgenerator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    plotDraw = new PlotDraw(ui->DrawContainerWidget);
    plotDraw->SetBackground(QColor(255, 255, 255));
    /*
    plotDraw->SetBuffer(100, 1);
    plotDraw->SetSignalStyle(0, PlotDraw::SignalDrawType::Line, QColor(255, 0, 0), 1);

    for (quint32 i = 0; i < 100; ++i)
    {
        plotDraw->SetSample(sin((double)i * 0.1), 0);
        plotDraw->NextFrame();
    }
    plotDraw->SetView(0, 99, -1.2, 1.2);
    */
    QVBoxLayout* layout = new QVBoxLayout(ui->DrawContainerWidget);
    layout->addWidget(plotDraw);
    ui->DrawContainerWidget->setLayout(layout);

    mainSignal = nullptr;
    mainAutocorrelation = nullptr;
    mainDFT = nullptr;
    mainDFTPhase = nullptr;
    mainFFT = nullptr;
    mainFFTPhase = nullptr;

    colorSignal = QColor(0, 0, 0);
    colorAutocorrelation = QColor(255, 0, 0);
    colorDFT = QColor(0, 0, 255);
    colorFFT = QColor(255, 100, 0);
}

void MainWindow::addSignal(DiscreteSignal* &sig, quint32 samplesCount)
{
    if (sig == nullptr)
    {
        sig = new DiscreteSignal(samplesCount);
        plotDraw->AddSignal(sig);
    }
}

void MainWindow::removeSignal(DiscreteSignal* &sig)
{
    if (sig)
    {
        plotDraw->RemoveSignal(sig);
        delete sig;
    }
    sig = nullptr;
}

bool MainWindow::isFourierView()
{
    return ui->FFTCheckbox->isChecked() || ui->FFTPhaseCheckbox->isChecked() || ui->DFTCheckbox->isChecked() || ui->DFTPhaseCheckbox->isChecked();
}

void MainWindow::generate(quint32 harmonicsNumber, quint32 samplesCount, double deltaTime, double maxAmp)
{
    uint genSeed = uint(QDateTime::currentSecsSinceEpoch());
    SineSeriesGenerator gen = SineSeriesGenerator(genSeed, harmonicsNumber, maxAmp);
    if (mainSignal)
    {
        if (mainSignal->Size() != samplesCount)
        {
            removeSignal(mainSignal);
            removeSignal(mainAutocorrelation);
            removeSignal(mainDFT);
            removeSignal(mainDFTPhase);
            removeSignal(mainFFT);
            removeSignal(mainFFTPhase);
            /*
            plotDraw->RemoveSignal(mainSignal);
            delete mainSignal;
            mainSignal = nullptr;

            plotDraw->RemoveSignal(mainAutocorrelation);
            delete mainAutocorrelation;
            mainAutocorrelation = nullptr;

            plotDraw->RemoveSignal(mainDFT);
            delete mainDFT;
            mainDFT = nullptr;
            */
        }
    }

    if (mainSignal == nullptr)
    {
        /*
        mainSignal = new DiscreteSignal(samplesCount);
        mainAutocorrelation = new DiscreteSignal(samplesCount);
        plotDraw->AddSignal(mainSignal);
        plotDraw->AddSignal(mainAutocorrelation);
        */
        addSignal(mainSignal, samplesCount);
        addSignal(mainAutocorrelation, samplesCount);
        addSignal(mainDFT, samplesCount);
        addSignal(mainDFTPhase, samplesCount);
        addSignal(mainFFT, samplesCount);
        addSignal(mainFFTPhase, samplesCount);
    }

    gen.fill(*mainSignal, 0, deltaTime);
    DiscreteSignal::Autocorrelation(*mainSignal, *mainAutocorrelation);
    DiscreteSignal::DFT(*mainSignal, *mainDFT, *mainDFTPhase);
    DiscreteSignal::FFT(*mainSignal, *mainFFT, *mainFFTPhase);

    plotDraw->SetSignalStyle(0,
                             isFourierView() ? PlotDraw::SignalDrawType::Invisible :
                                               PlotDraw::SignalDrawType::Line,
                             colorSignal, 1);

    plotDraw->SetSignalStyle(1,
        ui->AutocorrelationCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorAutocorrelation, 1);

    plotDraw->SetSignalStyle(2,
        ui->DFTCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorDFT, 1);

    plotDraw->SetSignalStyle(3,
        ui->DFTPhaseCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorDFT, 1);

    plotDraw->SetSignalStyle(4,
        ui->FFTCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorFFT, 1);

    plotDraw->SetSignalStyle(5,
        ui->FFTPhaseCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorFFT, 1);

    double mainMax = mainSignal->Max();
    double mainMin = mainSignal->Min();
    double acMax = mainAutocorrelation->Max();
    double acMin = mainAutocorrelation->Min();
    double dftMax = mainDFT->Max();
    double dftMin = mainDFT->Min();
    double fftMax = mainFFT->Max();
    double fftMin = mainFFT->Min();

    plotDraw->SetSignalTransform(4, 0, mainMax / fftMax);
    plotDraw->SetSignalTransform(2, 0, mainMax / dftMax);
    plotDraw->SetSignalTransform(1, 0, mainMax / acMax);
    plotDraw->SetView(0, samplesCount-1, mainMin - 0.1, mainMax + 0.1);

    plotDraw->repaint();
}

MainWindow::~MainWindow()
{
    delete plotDraw;
    delete ui;
}


void MainWindow::on_generateButton_clicked()
{
    generate(
        ui->harmonicsNumberSpinBox->value(),
        ui->samplesNumberEdit->text().toInt(),
        ui->deltaTimeEdit->text().toDouble(),
        1.0);

    double avg = mainSignal->Average();
    ui->avgEdit->setText(QString::number(avg));
    ui->varEdit->setText(QString::number(mainSignal->Variance(avg)));
}

void MainWindow::on_AutocorrelationCheckbox_stateChanged(int arg1)
{
    plotDraw->SetSignalStyle(1,
        ui->AutocorrelationCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorAutocorrelation, 1);
    plotDraw->repaint();
}

void MainWindow::on_DFTCheckbox_stateChanged(int arg1)
{
    plotDraw->SetSignalStyle(2,
        ui->DFTCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorDFT, 1);

    plotDraw->SetSignalStyle(0,
                             isFourierView() ? PlotDraw::SignalDrawType::Invisible :
                                               PlotDraw::SignalDrawType::Line,
                             colorSignal, 1);

    plotDraw->repaint();
}

void MainWindow::on_FFTCheckbox_stateChanged(int arg1)
{
    plotDraw->SetSignalStyle(4,
        ui->FFTCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorFFT, 1);

    plotDraw->SetSignalStyle(0,
                             isFourierView() ? PlotDraw::SignalDrawType::Invisible :
                                               PlotDraw::SignalDrawType::Line,
                             colorSignal, 1);

    plotDraw->repaint();
}

void MainWindow::on_DFTPhaseCheckbox_stateChanged(int arg1)
{
    plotDraw->SetSignalStyle(3,
        ui->DFTPhaseCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorDFT, 1);

    plotDraw->SetSignalStyle(0,
                             isFourierView() ? PlotDraw::SignalDrawType::Invisible :
                                               PlotDraw::SignalDrawType::Line,
                             colorSignal, 1);

    plotDraw->repaint();
}

void MainWindow::on_FFTPhaseCheckbox_stateChanged(int arg1)
{
    plotDraw->SetSignalStyle(5,
        ui->FFTPhaseCheckbox->isChecked() ?
        PlotDraw::SignalDrawType::Line : PlotDraw::SignalDrawType::Invisible,
        colorFFT, 1);

    plotDraw->SetSignalStyle(0,
                             isFourierView() ? PlotDraw::SignalDrawType::Invisible :
                                               PlotDraw::SignalDrawType::Line,
                             colorSignal, 1);

    plotDraw->repaint();
}
