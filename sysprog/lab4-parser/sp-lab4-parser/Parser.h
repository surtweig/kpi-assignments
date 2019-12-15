#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <istream>
#include <sstream>
#include "../../lab2/sp-lab2-fsa-lexer/FSALexer.h"

using namespace std;

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
    _squareOpen, _squareClose, _colon,

    _assign, _notEquals, _equals, _lower, _lowerEq, _greater, _greaterEq,
    _plus, _minus, _multiply, _divide, _caret,

    _stop
};

enum PascalSyntaxNodes
{
    PROGRAM, BLOCK, IDENTIFIER, ARRAY_DECL, VARIABLE, CONSTANT, VAR_SECTION, CONST_SECTION,
    NUMBER_LIT, STRING_LIT, TYPE
};

template <typename TTokenId>
class SyntaxNode
{
private:
    vector<Lexeme<TTokenId>> tokens;
    SyntaxNode<TTokenId>* parent;
    vector<SyntaxNode<TTokenId>*> children;
    PascalSyntaxNodes nType;
    string name;

public:
    SyntaxNode(SyntaxNode<TTokenId>* parent, PascalSyntaxNodes nType, string name)
    {
        this->parent = parent;
        if (parent)
            parent->AddChild(this);
        this->nType = nType;
        this->name = name;
    }

    SyntaxNode<TTokenId>* AddChild(SyntaxNode<TTokenId>* child)
    {
        //child->parent = this;
        children.push_back(child);
        return child;
    }

    ~SyntaxNode()
    {
        for (auto i = children.begin(); i != children.end(); ++i)
            delete *i;
    }

    PascalSyntaxNodes NodeType() {return nType;}
    string Name() {return name;}
    int ChildrenCount() {return children.size();}
    SyntaxNode<TTokenId>* GetChild(int index) {return children[index];}
};

class PascalParser
{
private:
    FSALexer<PascalTokens>* lex;
    vector<Lexeme<PascalTokens>> tokens;
    SyntaxNode<PascalTokens>* tree;
    size_t currentToken;

public:
    PascalParser()
    {
        map<string, PascalTokens> delimiters =
        {
            { "(", _parenthesisOpen  },
            { ")", _parenthesisClose },
            { ",", _comma            },
            { ";", _semicolon        },
            { ".", _period           },
            { "[", _squareOpen       },
            { "]", _squareClose      },
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
            { "^",  _caret     },
            { ":", _colon      }
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
        tree = nullptr;
    }

    ~PascalParser()
    {
        delete lex;
        delete tree;
    }

    bool Parse(string text)
    {
        delete tree;
        tokens.clear();
        istringstream s(text);
        int result = lex->Tokenize(s, tokens);
        if (result == 0)
        {
            currentToken = 0;
            parseProgram();
            return true;
        }
        else
        {
            error("Invalid token", tokens[tokens.size()-1]);
            return false;
        }
    }

    SyntaxNode<PascalTokens>* SyntaxRoot() {return tree;}

    Lexeme<PascalTokens> ConsumeToken()
    {
        if (currentToken < tokens.size())
            return tokens[currentToken++];
        else
        {
            if (tokens.size() > 0)
                return Lexeme<PascalTokens> {PascalTokens::_stop, tokens[tokens.size()-1].str, tokens[tokens.size()-1].lineNumber};
            else
                return Lexeme<PascalTokens> {PascalTokens::_stop, "", 0};
        }
    }

    Lexeme<PascalTokens> PeekToken()
    {
        if (currentToken < tokens.size())
            return tokens[currentToken];
        else
        {
            if (tokens.size() > 0)
                return Lexeme<PascalTokens> {PascalTokens::_stop, tokens[tokens.size()-1].str, tokens[tokens.size()-1].lineNumber};
            else
                return Lexeme<PascalTokens> {PascalTokens::_stop, "", 0};
        }
    }

    void error(string msg, Lexeme<PascalTokens>& t)
    {
        qDebug() << QString::fromStdString(t.str) << "@" << t.lineNumber << ":" << QString::fromStdString(msg);
    }

    void parseStatement(SyntaxNode<PascalTokens>* blockNode)
    {
        Lexeme<PascalTokens> t = ConsumeToken();
        //if (t.tokenId == lex->IdentifierToken())
    }

    void parseStatementsList(SyntaxNode<PascalTokens>* blockNode)
    {
        while (true)
        {
            Lexeme<PascalTokens> t = PeekToken();
            if (t.tokenId == PascalTokens::_end)
                return;

            if (t.tokenId == PascalTokens::_begin)
                parseBlock(blockNode);

            if (t.tokenId == PascalTokens::_stop)
            {
                error("Unexpected end of source file", t);
                return;
            }

            parseStatement(blockNode);
        }
    }

    void parseBlock(SyntaxNode<PascalTokens>* parent)
    {
        Lexeme<PascalTokens> t = ConsumeToken();
        if (t.tokenId == PascalTokens::_begin)
        {
            SyntaxNode<PascalTokens>* blockNode =
                    new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::BLOCK, "");

            parseStatementsList(blockNode);
            t = ConsumeToken();
            if (t.tokenId != PascalTokens::_end)
                error("Expected END keyword at the and of a block", t);
        }
        else
            error("Expected BEGIN keyword at the beginning of a block", t);
    }

    void parseVarDeclaration(SyntaxNode<PascalTokens>* parent)
    {
        Lexeme<PascalTokens> t = ConsumeToken();
        if (t.tokenId == lex->IdentifierToken())
        {
            vector<SyntaxNode<PascalTokens>*> itemNodes;
            itemNodes.push_back(new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::VARIABLE, t.str));
            t = ConsumeToken();
            while (t.tokenId == PascalTokens::_comma)
            {
                t = ConsumeToken();
                if (t.tokenId == lex->IdentifierToken())
                {
                    itemNodes.push_back(new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::VARIABLE, t.str));
                }
                else
                {
                    error("Expected next variable name after a comma", t);
                    return;
                }
                t = ConsumeToken();
            }

            if (t.tokenId == PascalTokens::_colon)
            {
                t = ConsumeToken();
                if (t.tokenId == lex->IdentifierToken())
                {
                    //SyntaxNode<PascalTokens>* itemsType = parent->AddChild(
                    new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::TYPE, t.str);
                }
                else if (t.tokenId == PascalTokens::_array)
                {
                    SyntaxNode<PascalTokens>* itemsArrayDecl =
                                new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::ARRAY_DECL, t.str);

                    t = ConsumeToken();
                    if (t.tokenId == PascalTokens::_squareOpen)
                    {
                        t = ConsumeToken();
                        if (t.tokenId == lex->IdentifierToken())
                        {
                            new SyntaxNode<PascalTokens>(itemsArrayDecl, PascalSyntaxNodes::IDENTIFIER, t.str);
                        }
                        else if (t.tokenId == lex->NumberLiteralToken())
                        {
                            new SyntaxNode<PascalTokens>(itemsArrayDecl, PascalSyntaxNodes::NUMBER_LIT, t.str);
                        }
                        else
                        {
                            error("Array bounds must be either identifiers or number literals", t);
                            return;
                        }

                        t = ConsumeToken();

                        if (t.tokenId == PascalTokens::_period && ConsumeToken().tokenId == PascalTokens::_period)
                        {
                            t = ConsumeToken();
                            if (t.tokenId == lex->IdentifierToken())
                            {
                                new SyntaxNode<PascalTokens>(itemsArrayDecl, PascalSyntaxNodes::IDENTIFIER, t.str);
                            }
                            else if (t.tokenId == lex->NumberLiteralToken())
                            {
                                new SyntaxNode<PascalTokens>(itemsArrayDecl, PascalSyntaxNodes::NUMBER_LIT, t.str);
                            }
                            else
                            {
                                error("Array bounds must be either identifiers or number literals", t);
                                return;
                            }
                        }
                        else
                        {
                            error("Invalid array declaration syntax", t);
                            return;
                        }

                        t = ConsumeToken();

                        if (t.tokenId != PascalTokens::_squareClose)
                        {
                            error("Invalid array declaration syntax", t);
                            return;
                        }

                        t = ConsumeToken();

                        if (t.tokenId != PascalTokens::_of)
                        {
                            error("Expecting OF keyword", t);
                            return;
                        }

                        t = ConsumeToken();
                        if (t.tokenId == lex->IdentifierToken())
                        {
                            new SyntaxNode<PascalTokens>(itemsArrayDecl, PascalSyntaxNodes::TYPE, t.str);
                        }
                        else
                        {
                            error("Expecting array type notation", t);
                            return;
                        }
                    }
                    else
                    {
                        error("Invalid array declaration syntax", t);
                        return;
                    }
                }
                else
                {
                    error("Expected type or array notation", t);
                    return;
                }
            }
            else
            {
                error("Expected colon sign between variable names and type notation", t);
                return;
            }

        }
        else
        {
            error("Expected variable name", t);
            return;
        }

        t = ConsumeToken();
        if (t.tokenId != PascalTokens::_semicolon)
            error("Expecting semicolon after variable declaration", t);
    }

    void parseConstDeclaration(SyntaxNode<PascalTokens>* parent)
    {

    }

    void parseBody(SyntaxNode<PascalTokens>* parent)
    {
        Lexeme<PascalTokens> t = PeekToken();
        if (t.tokenId == PascalTokens::_const)
        {
            ConsumeToken();
            SyntaxNode<PascalTokens>* constsNode =
                    new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::CONST_SECTION, "");

            t = PeekToken();
            while (t.tokenId != PascalTokens::_var || t.tokenId != PascalTokens::_begin || t.tokenId != PascalTokens::_stop)
            {
                parseConstDeclaration(constsNode);
                t = PeekToken();
            }
        }

        if (t.tokenId == PascalTokens::_var)
        {
            ConsumeToken();
            SyntaxNode<PascalTokens>* varNode =
                    new SyntaxNode<PascalTokens>(parent, PascalSyntaxNodes::VAR_SECTION, "");

            t = PeekToken();
            while (t.tokenId != PascalTokens::_var && t.tokenId != PascalTokens::_begin && t.tokenId != PascalTokens::_stop)
            {
                parseVarDeclaration(varNode);
                t = PeekToken();
            }
        }

        if (t.tokenId == PascalTokens::_procedure || t.tokenId == PascalTokens::_function)
        {

        }

        if (t.tokenId == PascalTokens::_begin)
        {
            parseBlock(parent);
        }
        else
            error("Expected BEGIN keyword in a program body", t);
    }

    void parseProgram()
    {
        Lexeme<PascalTokens> t = ConsumeToken();
        if (t.tokenId == PascalTokens::_program)
        {
            t = ConsumeToken();
            if (t.tokenId == lex->IdentifierToken())
            {
                tree = new SyntaxNode<PascalTokens>(nullptr, PascalSyntaxNodes::PROGRAM, t.str);
                t = ConsumeToken();
                if (t.tokenId == PascalTokens::_semicolon)
                {
                    parseBody(tree);
                }
                else
                    error("Expected semicolon after program name", t);
            }
            else
                error("Program name must be a valid identifier", t);
        }
        else
            error("Expected PROGRAM keyword", t);
    }
};

#endif // PARSER_H