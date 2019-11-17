#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centerAndResize();
    init();
    refreshDbView();
}

MainWindow::~MainWindow()
{
    deinit();
    delete ui;
}

void MainWindow::init()
{
    db = new Database();

    /*db->Append("foo", DBRecordData({0.2, 0.4, 0.6}));
    db->Append("Zzz", DBRecordData({9, 5, 3.14}));
    db->Append("Zzz", DBRecordData({9, 5, 3.14}));
    db->Append("bar", DBRecordData({0.5, 0.9, 0.1}));
    db->Append("Zzz", DBRecordData({9, 5, 3.14}));
    db->Append("bar", DBRecordData({2.5, 3.9, 4.1}));
    db->Append("bar", DBRecordData({2.5, 3.9, 4.1}));
    db->Append("qwerty", DBRecordData({2.5, 3.9, 4.1}));*/


    db->Append("Foo", DBRecordData({0.2, 0.4, 0.6}));
        //db->Append("Zzz1", DBRecordData({9, 5, 3.14}));
        //db->Append("Zzz2", DBRecordData({9, 5, 3.14}));
        db->Append("bar", DBRecordData({0.5, 0.9, 0.1}));
        db->Append("Zzz", DBRecordData({91, 5, 3.14}));
    //db->rebuildIndices();
    recordsViewList = QList<DBRecord*>();
    selectedRecord = nullptr;
}

void MainWindow::deinit()
{
    delete db;
}

void MainWindow::refreshDbView()
{
   recordsViewList.clear();
   ui->DatabaseView->clearContents();
   ui->DatabaseView->setColumnWidth(0, 300);
   ui->DatabaseView->setColumnWidth(1, 30);
   ui->DatabaseView->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->setRowCount(db->GetCounter());
   for (size_t i = 0; i < db->GetCounter(); ++i)
   {
       DBRecord* rec = db->GetRecord(i);

       ui->DatabaseView->setItem(int(i), 0, new QTableWidgetItem(rec->Name()));
       if (rec->Nmod() > 0)
           ui->DatabaseView->setItem(int(i), 1, new QTableWidgetItem(QString::number(rec->Nmod())));
       ui->DatabaseView->setItem(int(i), 2, new QTableWidgetItem(QString::number(rec->data.vector[0])));
       ui->DatabaseView->setItem(int(i), 3, new QTableWidgetItem(QString::number(rec->data.vector[1])));
       ui->DatabaseView->setItem(int(i), 4, new QTableWidgetItem(QString::number(rec->data.vector[2])));

       recordsViewList.append(rec);
   }
}

void MainWindow::selectRecord(int listIndex)
{
    if (listIndex >= 0 && listIndex < recordsViewList.count())
    {
        DBRecord* rec = recordsViewList[listIndex];
        selectedRecord = rec;
        ui->RecordNameEdit->setText(rec->Name());
        ui->RecordXEdit->setText(QString::number(rec->data.vector[0]));
        ui->RecordYEdit->setText(QString::number(rec->data.vector[1]));
        ui->RecordZEdit->setText(QString::number(rec->data.vector[2]));
    }
    else
    {
        selectedRecord = nullptr;
        ui->RecordNameEdit->clear();
        ui->RecordXEdit->clear();
        ui->RecordYEdit->clear();
        ui->RecordZEdit->clear();
    }
}

void MainWindow::centerAndResize() {
    // get the dimension available on this screen
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.7; // 90% of the screen size
    height *= 0.8; // 90% of the screen size
    QSize newSize( width, height );

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            newSize,
            qApp->desktop()->availableGeometry()
        )
    );
}

void MainWindow::on_AddRecordButton_clicked()
{
    db->Append(ui->RecordNameEdit->text().toLocal8Bit().data(), DBRecordData({0.2, 0.4, 0.6}));
    refreshDbView();
}

void MainWindow::on_DatabaseView_cellClicked(int row, int column)
{
    selectRecord(row);
}

void MainWindow::on_DeleteRecordButton_clicked()
{
    if (selectedRecord)
    {
        db->Delete(selectedRecord->Key());
        refreshDbView();
        selectRecord(-1);
    }
}

void MainWindow::on_RecordNameEdit_editingFinished()
{

    char* newName = ui->RecordNameEdit->text().toLocal8Bit().data();
    if (selectedRecord)
    {
        if (selectedRecord->Key().CompareName(newName))
        DBRecordData data = selectedRecord->data;

    }

}
