#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parser = new PascalParser(&logOutput);

    syntaxNodeTypes = {
        { PascalSyntaxNodes::PROGRAM,       "PROGRAM"       },
        { PascalSyntaxNodes::BLOCK,         "BLOCK"         },
        { PascalSyntaxNodes::IDENTIFIER,    "IDENTIFIER"    },
        { PascalSyntaxNodes::ARRAY_DECL,    "ARRAY_DECL"    },
        { PascalSyntaxNodes::VARIABLE,      "VARIABLE"      },
        { PascalSyntaxNodes::CONSTANT,      "CONSTANT"      },
        { PascalSyntaxNodes::VAR_SECTION,   "VAR_SECTION"   },
        { PascalSyntaxNodes::CONST_SECTION, "CONST_SECTION" },
        { PascalSyntaxNodes::NUMBER_LIT,    "NUMBER_LIT"    },
        { PascalSyntaxNodes::STRING_LIT,    "STRING_LIT"    },
        { PascalSyntaxNodes::TYPE,          "TYPE"          },
        { PascalSyntaxNodes::STATEMENT,     "STATEMENT"     },
        { PascalSyntaxNodes::EXPRESSION,    "EXPRESSION"    },
        { PascalSyntaxNodes::FUNC_CALL,     "FUNC_CALL"     },
        { PascalSyntaxNodes::FUNC_ARGUMENT, "FUNC_ARGUMENT" },
        { PascalSyntaxNodes::OPERATOR,      "OPERATOR"      },
        { PascalSyntaxNodes::WHILE_LOOP,    "WHILE_LOOP"    },
        { PascalSyntaxNodes::FOR_LOOP,      "FOR_LOOP"      },
        { PascalSyntaxNodes::IF_STATEMENT,  "IF_STATEMENT"  },
        { PascalSyntaxNodes::CONDITION,     "CONDITION"     },
        { PascalSyntaxNodes::ARRAY_INDEX,   "ARRAY_INDEX"   }
    };

    ui->syntaxTreeWidget->setColumnWidth(0, 400);
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
    refreshOutputLog();
}

void MainWindow::refreshSyntaxTreeView()
{
    ui->syntaxTreeWidget->clear();
    if (parser->SyntaxRoot())
    {
        QTreeWidgetItem *treeRootItem = new QTreeWidgetItem(ui->syntaxTreeWidget);
        addSyntaxTreeNode(treeRootItem, parser->SyntaxRoot());
    }
}

void MainWindow::addSyntaxTreeNode(QTreeWidgetItem* parent, SyntaxNode<PascalTokens>* node)
{
    parent->setText(0, syntaxNodeTypes[node->NodeType()]);
    parent->setText(1, QString::fromStdString(node->Name()));
    for (int i = 0; i < node->ChildrenCount(); ++i)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        parent->addChild(treeItem);
        addSyntaxTreeNode(treeItem, node->GetChild(i));
    }
}

void MainWindow::refreshOutputLog()
{
    ui->outputLogWidget->clear();
    char c[256];
    while (!logOutput.fail())
    {
        logOutput.getline(c, 256);
        ui->outputLogWidget->addItem(QString::fromLocal8Bit(c));
    }
    logOutput.clear();
}
