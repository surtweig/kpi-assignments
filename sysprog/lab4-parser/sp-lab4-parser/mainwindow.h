#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Parser.h>

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
    void on_CompileBtn_clicked();

private:
    Ui::MainWindow *ui;
    PascalParser* parser;

    void refreshSyntaxTreeView();
};
#endif // MAINWINDOW_H
