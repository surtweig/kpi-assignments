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
    showDb();
}

MainWindow::~MainWindow()
{
    deinit();
    delete ui;
}

void MainWindow::init()
{
    db = new Database<DBRecordData>();

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
    recordsViewList = QList<DBRecord<DBRecordData>*>();
    selectedRecord = nullptr;
}

void MainWindow::deinit()
{
    delete db;
}

void MainWindow::showDb()
{
    recordsViewList.clear();
    for (size_t i = 0; i < db->GetCounter(); ++i)
    {
        DBRecord<DBRecordData>* rec = db->GetRecord(i);
        recordsViewList.append(rec);
    }
    refreshDbView();
}

void MainWindow::refreshDbView()
{
   ui->DatabaseView->clearContents();
   ui->DatabaseView->setColumnWidth(0, 300);
   ui->DatabaseView->setColumnWidth(1, 30);
   ui->DatabaseView->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignLeft);
   ui->DatabaseView->setRowCount(recordsViewList.count());
   for (int i = 0; i < recordsViewList.count(); ++i)
   {
       DBRecord<DBRecordData>* rec = recordsViewList[i];

       ui->DatabaseView->setItem(int(i), 0, new QTableWidgetItem(rec->Name()));
       if (rec->Nmod() > 0)
           ui->DatabaseView->setItem(int(i), 1, new QTableWidgetItem(QString::number(rec->Nmod())));
       ui->DatabaseView->setItem(int(i), 2, new QTableWidgetItem(QString::number(rec->data.vector[0])));
       ui->DatabaseView->setItem(int(i), 3, new QTableWidgetItem(QString::number(rec->data.vector[1])));
       ui->DatabaseView->setItem(int(i), 4, new QTableWidgetItem(QString::number(rec->data.vector[2])));

   }
}

void MainWindow::selectRecord(int listIndex)
{
    if (listIndex >= 0 && listIndex < recordsViewList.count())
    {
        DBRecord<DBRecordData>* rec = recordsViewList[listIndex];
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

void MainWindow::centerAndResize()
{
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.7;
    height *= 0.8;
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
    db->Append(
        ui->RecordNameEdit->text().toLocal8Bit().data(),
        DBRecordData({ui->RecordXEdit->text().toFloat(),
                      ui->RecordYEdit->text().toFloat(),
                      ui->RecordZEdit->text().toFloat()}));
    showDb();
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
        showDb();
        selectRecord(-1);
    }
}

void MainWindow::on_RecordNameEdit_editingFinished()
{

}

void MainWindow::on_RecordNameEdit_returnPressed()
{
    if (selectedRecord)
    {
        DBRecordKey selectedKey = selectedRecord->Key();
        if (selectedKey.CompareName(ui->RecordNameEdit->text().toLocal8Bit().constData()) != 0)
        {
            DBRecordData data = selectedRecord->data;
            db->Delete(selectedKey);
            size_t newIndex = db->Append(ui->RecordNameEdit->text().toLocal8Bit().constData(), data);
            //selectRecord(-1);
            refreshDbView();
            selectedRecord = db->Select(newIndex);
        }
    }
}

void MainWindow::on_SearchRecordNameEdit_textChanged(const QString &arg1)
{
    if (ui->SearchRecordNameEdit->text().count() == 0)
        showDb();
    else
    {
        DBRecord<DBRecordData>* rec = db->Search(ui->SearchRecordNameEdit->text().toLocal8Bit().constData());
        DBRecord<DBRecordData>* recprev = rec;
        DBRecord<DBRecordData>* recnext = rec;
        recordsViewList.clear();
        if (rec)
            recordsViewList.append(rec);
        while (recprev != nullptr || recnext != nullptr)
        {
            recprev = db->SearchNext(recprev, -1);
            recnext = db->SearchNext(recnext, 1);
            if (recprev)
                recordsViewList.prepend(recprev);
            if (recnext)
                recordsViewList.append(recnext);
        }
        refreshDbView();
    }
}

void MainWindow::on_CancelSearchButton_clicked()
{
    ui->SearchRecordNameEdit->setText("");
}
