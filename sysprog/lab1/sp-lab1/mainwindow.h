#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void selectRecord(int listIndex);

private slots:
    void on_AddRecordButton_clicked();
    void on_DatabaseView_cellClicked(int row, int column);
    void on_DeleteRecordButton_clicked();
    void on_RecordNameEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    Database* db;
    QList<DBRecord*> recordsViewList;
    DBRecord* selectedRecord;
};
#endif // MAINWINDOW_H
