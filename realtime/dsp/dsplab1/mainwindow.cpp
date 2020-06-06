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
}

void MainWindow::generate(quint32 harmonicsNumber, quint32 samplesCount, double deltaTime, double maxAmp)
{
    SineSeriesGenerator gen = SineSeriesGenerator(uint(QDateTime::currentSecsSinceEpoch()), harmonicsNumber, maxAmp);
    if (mainSignal)
    {
        if (mainSignal->Size() != samplesCount)
        {
            plotDraw->RemoveSignal(mainSignal);
            delete mainSignal;
            mainSignal = nullptr;

            plotDraw->RemoveSignal(mainAutocorrelation);
            delete mainAutocorrelation;
            mainAutocorrelation = nullptr;
        }
    }

    if (mainSignal == nullptr)
    {
        mainSignal = new DiscreteSignal(samplesCount);
        mainAutocorrelation = new DiscreteSignal(samplesCount);
        plotDraw->AddSignal(mainSignal);
        plotDraw->AddSignal(mainAutocorrelation);
    }

    gen.fill(*mainSignal, 0, deltaTime);
    DiscreteSignal::Autocorrelation(*mainSignal, *mainAutocorrelation);

    plotDraw->SetSignalStyle(0, PlotDraw::SignalDrawType::Line, QColor(255, 0, 0), 1);
    plotDraw->SetSignalStyle(1, PlotDraw::SignalDrawType::Line, QColor(0, 0, 255), 1);

    double mainMax = mainSignal->Max();
    double mainMin = mainSignal->Min();
    double acMax = mainAutocorrelation->Max();
    double acMin = mainAutocorrelation->Min();

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
