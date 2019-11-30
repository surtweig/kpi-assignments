#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FiniteStateAutomaton.h"

#include <map>
#include <vector>
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
        { "and", _and     }, { "array", _array   }, { "begin", _begin     }, { "case", _case       }, { "const", _const },
        { "div", _div     }, { "do", _do      }, { "downto", _downto    }, { "else", _else       }, { "end", _end },
        { "file", _file    }, { "for", _for     }, { "function", _function  }, { "goto", _goto       }, { "if", _if },
        { "in", _in	     }, { "label", _label   }, { "mod", _mod       }, { "nil", _nil        }, { "not", _not },
        { "of", _of	     }, { "or", _or      }, { "packed", _packed    }, { "procedure", _procedure  }, { "program", _program },
        { "record", _record  }, { "repeat", _repeat  }, { "set", _set       }, { "then", _then       }, { "to", _to },
        { "type", _type    }, { "until", _until   }, { "var", _var       }, { "while", _while      }, { "with", _with }
    };

    lex = new FSALexer<PascalTokens>(reservedWords, operators, delimiters, '\'', "//", "{", "}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

