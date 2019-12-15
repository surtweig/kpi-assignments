#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parser = new PascalParser();
}

MainWindow::~MainWindow()
{
    delete parser;
    delete ui;
}


void MainWindow::on_CompileBtn_clicked()
{
    parser->Parse(ui->plainTextEdit->toPlainText().toStdString());
    refreshSyntaxTreeView();
}

void MainWindow::refreshSyntaxTreeView()
{
    ui->syntaxTreeWidget->clear();

}
