#include "perftestdialog.h"
#include "ui_perftestdialog.h"
#include <QTextStream>
#include "sineseriesgenerator.h"
#include <QDateTime>

PerfTestDialog::PerfTestDialog(quint32 harmonicsNumber, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfTestDialog)
{
    ui->setupUi(this);

    signal1 = nullptr;
    signal2 = nullptr;
    signalSize = 0;

    this->harmonicsNumber = harmonicsNumber;
}

PerfTestDialog::~PerfTestDialog()
{
    delete ui;
}

void PerfTestDialog::on_closeButton_clicked()
{
    hide();
}

void PerfTestDialog::on_runTestButton_clicked()
{
    signalSize = ui->signalSizeSpinBox->value();
    fftTime = 0;
    dftTime = 0;

    runTest();

    ui->testResultsTextEdit->clear();

    QString s;
    QTextStream ts(&s);
    ts << "Performance test for " << signalSize << " samples in miliseconds:" << endl
       << "Generation = " << generationTime << endl
       << "Rxx (Autocorrelation) = " << rxxTime << endl
       << "Rxy (Correlation) = " << rxyTime << endl
       << "DFT = " << dftTime << endl
       << "FFT = " << fftTime << endl;

    ui->testResultsTextEdit->appendPlainText(s);
}

void PerfTestDialog::runTest()
{
    signal1 = new DiscreteSignal(signalSize);
    signal2 = new DiscreteSignal(signalSize);
    DiscreteSignal ac(signalSize);
    DiscreteSignal fphase(signalSize);

    perfTimer.start();

    uint genSeed = uint(QDateTime::currentSecsSinceEpoch());
    SineSeriesGenerator gen = SineSeriesGenerator(genSeed, harmonicsNumber, 1.0);
    gen.fill(*signal1, 0, 0.01);
    generationTime = perfTimer.elapsed();

    genSeed = uint(QDateTime::currentSecsSinceEpoch());
    gen = SineSeriesGenerator(genSeed, harmonicsNumber, 1.0);
    gen.fill(*signal2, 0, 0.01);

    perfTimer.start();
    DiscreteSignal::Autocorrelation(*signal1, ac);
    rxxTime = perfTimer.restart();
    DiscreteSignal::Correlation(*signal1, *signal2, ac);
    rxyTime = perfTimer.elapsed();

    if (signalSize > 0 && (signalSize & (signalSize-1)) == 0)
    {
        perfTimer.start();
        DiscreteSignal::DFT(*signal1, ac, fphase);
        dftTime = perfTimer.elapsed();
        perfTimer.start();
        DiscreteSignal::FFT(*signal1, ac, fphase);
        fftTime = perfTimer.elapsed();
    }

    delete signal1;
    delete signal2;
    signal1 = nullptr;
    signal2 = nullptr;
}
