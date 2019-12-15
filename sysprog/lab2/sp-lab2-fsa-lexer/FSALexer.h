#ifndef FSALEXER_H
#define FSALEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <ctype.h>
#include "FiniteStateAutomaton.h"
#include <limits>

using namespace std;

template <typename TTokenId>
struct Lexeme
{
    TTokenId tokenId;
    string str;
    int lineNumber;
};

template <typename TTokenId>
class FSALexer
{
public:

    ///
    /// \brief TTokenIdSuperset is an ordered type which could contain all token ids
    /// as well as all intermediate FSA state ids
    ///
    typedef int TTokenIdSuperset;
    //typedef typename underlying_type<TTokenId>::type TTokenIdSuperset; // Assuming TTokenId is a complete enumeration type

    ///
    /// \brief FSALexer Finite state automaton based lexer
    /// \param reservedWords Identifiers with designated token ids
    /// \param operators Special characters operators (arithmetic, logic, etc.)
    /// \param delimiters Delimiting characters (braces, commas, etc.)
    /// \param commentLineDelimiter
    /// \param commentBlockStart
    /// \param commentBlockFinish
    ///
    FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators, map<string, TTokenId> delimiters,
             char stringDelimiter, string commentLineDelimiter, string commentBlockStart, string commentBlockFinish);

    ///
    /// \brief Tokenize Produces a sequence of tokens from current input position to the end
    /// \param input character stream
    /// \param output sequence of tokens
    /// \return zero on success, number of line with error otherwise
    ///
    int Tokenize(istream& input, vector<Lexeme<TTokenId>>& output);

    ///
    /// \brief GetToken returns token's string representation (if present)
    ///
    string GetToken(TTokenId tid);

    inline TTokenId NullToken() { return nullTk; }
    inline TTokenId InvalidToken() { return invalidTk; }
    inline TTokenId IdentifierToken() { return identifierTk; }
    inline TTokenId StringLiteralToken() { return stringTk; }
    inline TTokenId NumberLiteralToken() { return numberTk; }

private:
    FiniteStateAutomaton<TTokenIdSuperset, char>* fsa;
    map<string, TTokenId> tokens;
    map<TTokenId, string> idToTkStr;
    TTokenId nullTk;
    TTokenId invalidTk;
    TTokenId identifierTk;
    TTokenId stringTk;
    TTokenId numberTk;
    string commentLine;
    string commentStart;
    string commentFinish;

    ///
    /// \brief readTokenStr reads a string from input start (inclusively)
    /// then returns the stream to original position
    ///
    string readTokenStr(istream& input, uint32_t start, uint32_t count);

    ///
    /// \brief getMaxToken returns greatest TokenId from given tokens
    ///
    TTokenIdSuperset getMaxToken();

    bool isTokenId(TTokenIdSuperset state);

    ///
    /// \brief fsaAddString builds a string sequence in fsa
    /// \param stateIndexer external state indexer for creating new state ids
    /// \return state id of the last character
    ///
    TTokenIdSuperset fsaAddString(string tokenStr, TTokenIdSuperset& stateIndexer);

    void fsaIdentifierTokenTraverse(TTokenIdSuperset start,
                                    const map<char, TTokenIdSuperset>& baseIdChars,
                                    const vector<char>& stopChars,
                                    TTokenId tokenId);
};

template <typename TTokenId>
FSALexer<TTokenId>::FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators, map<string, TTokenId> delimiters,
                             char stringDelimiter, string commentLineDelimiter, string commentBlockStart, string commentBlockFinish) :
    tokens(reservedWords),
    commentLine(commentLineDelimiter),
    commentStart(commentBlockStart),
    commentFinish(commentBlockFinish)
{
    tokens.insert(operators.begin(), operators.end());
    tokens.insert(delimiters.begin(), delimiters.end());

    idToTkStr.clear();
    for (auto i = tokens.begin(); i != tokens.end(); ++i)
        idToTkStr[i->second] = i->first;

    TTokenIdSuperset maxTokenId = getMaxToken();
    TTokenIdSuperset stateIndexer = maxTokenId+1;

    nullTk = TTokenId(++stateIndexer);
    identifierTk = TTokenId(++stateIndexer);
    numberTk = TTokenId(++stateIndexer);
    stringTk = TTokenId(++stateIndexer);
    invalidTk = TTokenId(++stateIndexer);

    qDebug() << "nullTk =" << nullTk << "\nidentifierTk =" << identifierTk << "\nnumberTk =" << numberTk
                << "\nstringTk =" << stringTk << "\ninvalidTk =" << invalidTk;

    fsa = new FiniteStateAutomaton<TTokenIdSuperset, char>(nullTk, invalidTk);

    vector<char> whitespace({' ', '\t', '\n', '\r', 0});
    vector<char> delimitersChars;
    vector<char> operatorsFirstChars;
    vector<char> identifierChars({'_'});
    vector<char> numberChars;
    for (char c = 'a'; c <= 'z'; ++c)
        identifierChars.push_back(c);
    for (char c = 'A'; c <= 'Z'; ++c)
        identifierChars.push_back(c);
    for (char c = '0'; c <= '9'; ++c)
    {
        identifierChars.push_back(c);
        numberChars.push_back(c);
    }
    numberChars.push_back('.');

    // Adding whitespace idling
    fsa->AddTransition(nullTk, whitespace, nullTk);

    // Adding single-char delimiter tokens
    for (auto i = delimiters.begin(); i != delimiters.end(); ++i)
    {
        char delimChar = i->first[0];
        fsa->AddTransition(nullTk, delimChar, i->second);
        delimitersChars.push_back(delimChar);
    }

    // Adding operator tokens
    for (auto i = operators.begin(); i != operators.end(); ++i)
    {
        TTokenIdSuperset lastCharState = fsaAddString(i->first, stateIndexer);
        fsa->AddTransition(lastCharState, whitespace, i->second);
        fsa->AddTransition(lastCharState, delimitersChars, i->second);
        fsa->AddTransition(lastCharState, identifierChars, i->second);
        fsa->AddTransition(lastCharState, stringDelimiter, i->second);
        operatorsFirstChars.push_back(i->first[0]);
    }

    // Adding reserved words tokens
    for (auto i = reservedWords.begin(); i != reservedWords.end(); ++i)
    {
        TTokenIdSuperset lastCharState = fsaAddString(i->first, stateIndexer);
        fsa->AddTransition(lastCharState, whitespace, i->second);
        fsa->AddTransition(lastCharState, delimitersChars, i->second);
        fsa->AddTransition(lastCharState, operatorsFirstChars, i->second);
    }

    // Adding identifiers
    vector<char> stopChars(whitespace);
    stopChars.insert(stopChars.end(), delimitersChars.begin(), delimitersChars.end());
    stopChars.insert(stopChars.end(), operatorsFirstChars.begin(), operatorsFirstChars.end());

    map<char, TTokenIdSuperset> baseIdChars;
    vector<TTokenIdSuperset> reservedWordsFirstCharStates;
    for (auto i = identifierChars.begin(); i != identifierChars.end(); ++i)
    {
        TTokenIdSuperset existingChar = fsa->GetTransition(nullTk, *i);
        if (existingChar != invalidTk)
            reservedWordsFirstCharStates.push_back(existingChar);

        baseIdChars[*i] = ++stateIndexer;
        qDebug() << "baseIdChar" << *i << baseIdChars[*i];
        if (*i < '0' || *i > '9')
            fsa->AddTransition(nullTk, *i, baseIdChars[*i], false);
    }
    for (auto i = identifierChars.begin(); i != identifierChars.end(); ++i)
    {
        fsaIdentifierTokenTraverse(baseIdChars[*i], baseIdChars, stopChars, identifierTk);
    }
    for (auto i = reservedWordsFirstCharStates.begin(); i != reservedWordsFirstCharStates.end(); ++i)
    {
        fsaIdentifierTokenTraverse(*i, baseIdChars, stopChars, identifierTk);
    }

    // Adding numbers
    map <char, TTokenIdSuperset> baseNumberChars;
    for (auto i = numberChars.begin(); i != numberChars.end(); ++i)
    {
        baseNumberChars[*i] = ++stateIndexer;
        qDebug() << "baseNumberChar" << *i << baseIdChars[*i];
        if (*i != '.')
            fsa->AddTransition(nullTk, *i, baseNumberChars[*i]);
    }
    for (auto i = numberChars.begin(); i != numberChars.end(); ++i)
    {
        fsaIdentifierTokenTraverse(baseNumberChars[*i], baseNumberChars, stopChars, numberTk);
    }

    // Adding strings
    map <char, TTokenIdSuperset> baseStrChars;
    TTokenIdSuperset strStartState = stateIndexer++;
    TTokenIdSuperset strFinishState = stateIndexer++;
    qDebug() << "strStart" << strStartState;
    qDebug() << "strFinish" << strFinishState;
    fsa->AddTransition(nullTk, stringDelimiter, strStartState);
    fsa->AddTransition(strStartState, stringDelimiter, strFinishState);
    for (int c = 0; c < 0xff; ++c)
    {
        if (isprint(c) && c != stringDelimiter)
        {
            baseStrChars[char(c)] = ++stateIndexer;
            fsa->AddTransition(strStartState, c, baseStrChars[c]);
            qDebug() << "printable '" << char(c) << "'" << baseStrChars[c];
        }
    }
    for (auto i = baseStrChars.begin(); i != baseStrChars.end(); ++i)
    {
        fsa->AddTransition(i->second, stringDelimiter, strFinishState);
        for (int c = 0; c < 0xff; ++c)
        {
            if (isprint(c) && c != stringDelimiter)
            {
                fsa->AddTransition(i->second, c, baseStrChars[c]);
            }
        }
    }
    fsa->AddTransition(strFinishState, stopChars, stringTk);
}

template <typename TTokenId>
typename FSALexer<TTokenId>::TTokenIdSuperset FSALexer<TTokenId>::fsaAddString(string tokenStr, TTokenIdSuperset& stateIndexer)
{
    TTokenIdSuperset state = nullTk;
    for (auto ci = tokenStr.begin(); ci != tokenStr.end(); ++ci)
    {
        TTokenIdSuperset nextState = fsa->GetTransition(state, *ci);
        if (nextState == invalidTk)
        {
            nextState = ++stateIndexer;
            qDebug() << QString::fromStdString(tokenStr) << *ci << nextState;
            fsa->AddTransition(state, *ci, nextState);
        }
        state = nextState;
    }
    return state;
}

template <typename TTokenId>
void FSALexer<TTokenId>::fsaIdentifierTokenTraverse(TTokenIdSuperset start,
                                const map<char, TTokenIdSuperset>& baseIdChars,
                                const vector<char>& stopChars,
                                TTokenId tokenId)
{
    // remembering all transitions from that state
    map<char, TTokenIdSuperset> trans = fsa->GetTransitions(start);

    // adding identifier chars transitions
    fsa->AddTransition(start, baseIdChars, false);

    // adding stop transitions to the Identifier token
    fsa->AddTransition(start, stopChars, tokenId, false);

    // traverse original transitions
    for (auto i = trans.begin(); i != trans.end(); ++i)
        // if a transition could lead to an identifier
        if (baseIdChars.find(i->first) != baseIdChars.end())
            fsaIdentifierTokenTraverse(i->second, baseIdChars, stopChars, tokenId);
}


template <typename TTokenId>
typename FSALexer<TTokenId>::TTokenIdSuperset FSALexer<TTokenId>::getMaxToken()
{
    TTokenIdSuperset maxtk = tokens.begin()->second;
    for (auto i = tokens.begin(); i != tokens.end(); ++i)
    {
        TTokenIdSuperset tk = i->second;
        if (tk > maxtk)
            maxtk = tk;
    }
    return maxtk;
}

template <typename TTokenId>
string FSALexer<TTokenId>::readTokenStr(istream& input, uint32_t start, uint32_t count)
{
    // storing current stream position
    //istream::pos_type pos = input.tellg();

    // returning to beginning of the token
    input.seekg(start);

    //qDebug() << "readTokenStr: pos =" << pos << "start =" << start << "count =" << count;

    // reading current token's characters from the stream
    uint32_t tksize = uint32_t(count);
    string tkstr(tksize, ' ');
    input.read(&tkstr[0], tksize);

    // restoring original position
    //input.seekg(pos);

    return tkstr;
}

template <typename TTokenId>
bool FSALexer<TTokenId>::isTokenId(TTokenIdSuperset state)
{
    return state < invalidTk;
}

template <typename TTokenId>
int FSALexer<TTokenId>::Tokenize(istream& input, vector<Lexeme<TTokenId>>& output)
{
    if (input)
    {
        int lineCounter = 1;
        istream::pos_type lastLineEnd = 0;
        fsa->Set(nullTk);
        istream::pos_type currentStart = 0;
        bool nextTokenExpected = true;
        input.seekg(0, input.end);
        istream::pos_type endg = input.tellg();
        input.seekg(0, input.beg);

        qDebug() << "Tokenize: input.tellg() =" << input.tellg();

        while (input.tellg() <= endg)
        {
            istream::pos_type currentStreamPos = input.tellg();
            int c = 0;
            if (currentStreamPos < endg)
                c = input.get();
            TTokenIdSuperset fsaState = fsa->Receive(c);

            if (fsaState == invalidTk)
            {
                output.push_back(Lexeme<TTokenId> {invalidTk, readTokenStr(input, currentStart, currentStreamPos-currentStart+1), lineCounter});
                return lineCounter;
            }

            if (c == '\n' && lastLineEnd != currentStreamPos)
            {
                lineCounter++;
                lastLineEnd = currentStreamPos;
            }

            if (fsaState != nullTk && nextTokenExpected)
            {
                currentStart = currentStreamPos;
                nextTokenExpected = false;
                qDebug() << "currentStart =" << currentStart;
            }
            // If new FSA state corresponds to a token
            if (isTokenId(fsaState))
            {
                TTokenId nextTk = TTokenId(fsaState);
                if (idToTkStr.find(nextTk) != idToTkStr.end()
                        || nextTk == identifierTk
                        || nextTk == numberTk
                        || nextTk == stringTk)
                {
                    // Any token of more than one char is stopped by the beginning
                    // of another token, so after reading a token, input stream needs
                    // to be returned to the first char of the next one. However,
                    // as an exception from that rule, single-char tokens
                    // such as delimiters are stopped by themselves and the first
                    // char of the next token will not be consumed.
                    uint32_t tkLength = 1;
                    if (currentStreamPos != currentStart)
                        tkLength = currentStreamPos - currentStart;

                    output.push_back(Lexeme<TTokenId> {nextTk, readTokenStr(input, currentStart, tkLength), lineCounter});

                    //input.seekg(currentStreamPos);

                    fsa->Set(nullTk);
                    nextTokenExpected = true;

                    //currentStart = 0;
                }
            }

            if (c == 0 || c == char_traits<char>::eof())
                break;
        }
        return 0;
    }
    else
        return -1;
}


#endif // FSALEXER_H
