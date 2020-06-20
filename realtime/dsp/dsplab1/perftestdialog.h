#ifndef PERFTESTDIALOG_H
#define PERFTESTDIALOG_H

#include <QDialog>
#include <QElapsedTimer>
#include "discretesignal.h"

using namespace std;

namespace Ui {
class PerfTestDialog;
}

class PerfTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerfTestDialog(quint32 harmonicsNumber, QWidget *parent = nullptr);
    ~PerfTestDialog();

private slots:
    void on_closeButton_clicked();
    void on_runTestButton_clicked();

private:
    Ui::PerfTestDialog *ui;

    DiscreteSignal* signal1;
    DiscreteSignal* signal2;
    QElapsedTimer perfTimer;
    quint64 generationTime;
    quint64 rxxTime;
    quint64 rxyTime;
    quint64 fftTime;
    quint64 dftTime;
    size_t signalSize;
    quint32 harmonicsNumber;

    void runTest();
};

#endif // PERFTESTDIALOG_H
