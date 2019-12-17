#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QString>
#include <Parser.h>
#include <sstream>
#include <map>

using namespace std;

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
    map<PascalSyntaxNodes, QString> syntaxNodeTypes;
    stringstream logOutput;

    void refreshSyntaxTreeView();
    void addSyntaxTreeNode(QTreeWidgetItem* parent, SyntaxNode<PascalTokens>* node);
    void refreshOutputLog();
};
#endif // MAINWINDOW_H
