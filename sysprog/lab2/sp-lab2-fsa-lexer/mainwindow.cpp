#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FiniteStateAutomaton.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    //, fsm(0, -1)
{
    ui->setupUi(this);
    //sm.Set(1);
    fsm = new FiniteStateAutomaton<int, int>(0, -1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

