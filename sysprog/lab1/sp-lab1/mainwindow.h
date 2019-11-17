#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//template class Database<DBRecordData>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void deinit();

    void centerAndResize();
    void refreshDbView();
    void showDb();
    void selectRecord(int listIndex);

private slots:
    void on_AddRecordButton_clicked();
    void on_DatabaseView_cellClicked(int row, int column);
    void on_DeleteRecordButton_clicked();
    void on_RecordNameEdit_editingFinished();

    void on_RecordNameEdit_returnPressed();

    void on_SearchRecordNameEdit_textChanged(const QString &arg1);

    void on_CancelSearchButton_clicked();

private:
    Ui::MainWindow *ui;
    Database<DBRecordData>* db;
    QList<DBRecord<DBRecordData>*> recordsViewList;
    DBRecord<DBRecordData>* selectedRecord;
};
#endif // MAINWINDOW_H
