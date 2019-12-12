#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "FiniteStateAutomaton.h"
#include "FSALexer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum PascalTokens
{
    _and    ,_array  ,_begin    ,_case      ,_const,
    _div    ,_do     ,_downto   ,_else      ,_end,
    _file   ,_for    ,_function ,_goto      ,_if,
    _in	    ,_label  ,_mod      ,_nil       ,_not,
    _of	    ,_or     ,_packed   ,_procedure ,_program,
    _record ,_repeat ,_set      ,_then      ,_to,
    _type   ,_until  ,_var      ,_while     ,_with,

    _parenthesisOpen, _parenthesisClose, _comma, _semicolon, _period,
    _squareOpen, _squareClose,

    _assign, _notEquals, _equals, _lower, _lowerEq, _greater, _greaterEq,
    _plus, _minus, _multiply, _divide, _caret
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;
    //FiniteStateAutomaton<int, int>* fsm;
    FSALexer<PascalTokens>* lex;

    void refreshTokensList();
};
#endif // MAINWINDOW_H
