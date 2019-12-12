#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FiniteStateAutomaton.h"

#include <map>
#include <vector>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <QColor>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    //, fsm(0, -1)
{
    ui->setupUi(this);
    //sm.Set(1);
    //fsm = new FiniteStateAutomaton<int, int>(0, -1);

    map<string, PascalTokens> delimiters =
    {
        { "(", _parenthesisOpen  },
        { ")", _parenthesisClose },
        { ",", _comma            },
        { ";", _semicolon        },
        { ".", _period           },
        { "[", _squareOpen       },
        { "]", _squareClose      }
    };

    map<string, PascalTokens> operators =
    {
        { ":=",  _assign   },
        { "=",  _equals    },
        { "<>", _notEquals },
        { "<",  _lower     },
        { "<=", _lowerEq   },
        { ">",  _greater   },
        { ">=", _greaterEq },
        { "+",  _plus      },
        { "-",  _minus     },
        { "*",  _multiply  },
        { "/",  _divide    },
        { "^",  _caret     }
    };

    map<string, PascalTokens> reservedWords =
    {
        { "and", _and       }, { "array", _array   }, { "begin", _begin       }, { "case", _case           }, { "const", _const     },
        { "div", _div       }, { "do", _do         }, { "downto", _downto     }, { "else", _else           }, { "end", _end         },
        { "file", _file     }, { "for", _for       }, { "function", _function }, { "goto", _goto           }, { "if", _if           },
        { "in", _in	        }, { "label", _label   }, { "mod", _mod           }, { "nil", _nil             }, { "not", _not         },
        { "of", _of	        }, { "or", _or         }, { "packed", _packed     }, { "procedure", _procedure }, { "program", _program },
        { "record", _record }, { "repeat", _repeat }, { "set", _set           }, { "then", _then           }, { "to", _to           },
        { "type", _type     }, { "until", _until   }, { "var", _var           }, { "while", _while         }, { "with", _with       }
    };

    lex = new FSALexer<PascalTokens>(reservedWords, operators, delimiters, '\'', "//", "{", "}");

    /*
    istringstream s("begin aaaa:=bbbb; end.");
    vector<pair<PascalTokens, string>> output;
    bool result = lex->Tokenize(s, output);

    qDebug() << (result ? "Success" : "Fail") << ": Tokens " << output.size();
    for (auto i = output.begin(); i != output.end(); ++i)
    {
        qDebug() << i->first << ":" << QString::fromStdString(i->second);
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTokensList()
{
    QString t = ui->plainTextEdit->toPlainText();
    istringstream s(t.toStdString());
    vector<pair<PascalTokens, string>> output;
    bool result = lex->Tokenize(s, output);

    ui->listWidget->clear();
    if (result)
    {
        ui->listWidget->addItem("Success!");
        QBrush brush = ui->listWidget->item(0)->foreground();
        brush.setColor(QColor(Qt::GlobalColor::green));
        ui->listWidget->item(0)->setForeground(brush);
    }
    else
    {
        ui->listWidget->addItem("Fail!");
        QBrush brush = ui->listWidget->item(0)->foreground();
        brush.setColor(QColor(Qt::GlobalColor::red));
        ui->listWidget->item(0)->setForeground(brush);
    }

    for (auto i = output.begin(); i != output.end(); ++i)
    {
        //qDebug() << i->first << ":" << QString::fromStdString(i->second);
        QString prefix = "";
        QString postfix = "";
        QColor foreColor = QColor(Qt::GlobalColor::black);
        if (i->first == lex->IdentifierToken())
        {
            prefix = "id \"";
            postfix = "\"";
        }
        else if (i->first == lex->NumberLiteralToken())
        {
            prefix = "num \"";
            postfix = "\"";
        }
        else if (i->first == lex->StringLiteralToken())
        {
            prefix = "str \"";
            postfix = "\"";
        }
        else
        {
            foreColor = QColor(Qt::GlobalColor::blue);
        }
        ui->listWidget->addItem(prefix + QString::fromStdString(i->second) + postfix);
        int itemIndex = ui->listWidget->count()-1;
        QBrush brush = ui->listWidget->item(itemIndex)->foreground();
        brush.setColor(foreColor);
        ui->listWidget->item(itemIndex)->setForeground(brush);
    }
}

void MainWindow::on_plainTextEdit_textChanged()
{
    refreshTokensList();
}
