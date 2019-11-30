#ifndef FSALEXER_H
#define FSALEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <ctype.h>
#include "FiniteStateAutomaton.h"

using namespace std;

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
    /// \param delimiters Delimiting operators (braces, commas, etc.)
    /// \param nullTokenId Empty token
    /// \param invalidTokenId Invalid token used for error signalling
    /// \param stringTokenId String constant token
    /// \param numberTokenId Number constant token
    /// \param commentLineDelimiter
    /// \param commentBlockStart
    /// \param commentBlockFinish
    ///
    FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators, map<string, TTokenId> delimiters,
             //TTokenId nullTokenId, TTokenId invalidTokenId,
             //TTokenId identifierTokenId, TTokenId stringTokenId, TTokenId numberTokenId,
             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish);

    ///
    /// \brief Tokenize Produces a sequence of tokens from current input position to the end
    /// \param input character stream
    /// \param output sequence of tokens
    /// \return
    ///
    bool Tokenize(istream& input, vector< pair<TTokenId, string> >& output);

    ///
    /// \brief GetToken returns token's string representation (if present)
    ///
    string GetToken(TTokenId tid);

private:
    FiniteStateAutomaton<TTokenIdSuperset, char>* fsa;
    map<string, TTokenId> tokens;
    TTokenId nullTk;
    TTokenId invalidTk;
    TTokenId identifierTk;
    TTokenId stringTk;
    TTokenId numberTk;
    string commentLine;
    string commentStart;
    string commentFinish;

    ///
    /// \brief readTokenStr reads a string from input between start and finish positions (inclusively)
    /// then returns the stream to original position
    ///
    string readTokenStr(istream& input, uint32_t start, uint32_t finish = 0);

    ///
    /// \brief getMaxToken returns greatest TokenId from given tokens
    ///
    TTokenIdSuperset getMaxToken();

    ///
    /// \brief fsaAddString builds a string sequence in fsa
    /// \param stateIndexer external state indexer for creating new state ids
    /// \return state id of the last character
    ///
    TTokenIdSuperset fsaAddString(string tokenStr, TTokenIdSuperset& stateIndexer);
};

template <typename TTokenId>
FSALexer<TTokenId>::FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators, map<string, TTokenId> delimiters,
                             //TTokenId nullTokenId, TTokenId invalidTokenId,
                             //TTokenId identifierTokenId, TTokenId stringTokenId, TTokenId numberTokenId,
                             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish) :
    tokens(reservedWords),
    commentLine(commentLineDelimiter),
    commentStart(commentBlockStart),
    commentFinish(commentBlockFinish)
{
    tokens.insert(operators.begin(), operators.end());
    tokens.insert(delimiters.begin(), delimiters.end());

    TTokenIdSuperset maxTokenId = getMaxToken();
    TTokenIdSuperset stateIndexer = maxTokenId+1;

    nullTk = ++stateIndexer;
    invalidTk = ++stateIndexer;

    fsa = new FiniteStateAutomaton<TTokenIdSuperset, char>(nullTk, invalidTk);

    identifierTk = ++stateIndexer;
    numberTk = ++stateIndexer;
    stringTk = ++stateIndexer;

    vector<char> whitespace({' ', '\t', '\n', '\r'});

    for (auto i = delimiters.begin(); i != delimiters.end(); ++i)
    {
        TTokenIdSuperset lastCharState = fsaAddString(i->first, stateIndexer);
        fsa->AddTransition(lastCharState, whitespace, i->second);
    }
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
            fsa->AddTransition(state, *ci, nextState);
        }
        state = nextState;
    }
    return state;
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
string FSALexer<TTokenId>::readTokenStr(istream& input, uint32_t start, uint32_t finish)
{
    // storing current stream position
    istream::pos_type pos = input.tellg();

    if (finish == 0)
        finish = pos;

    // returning to beginning of the token
    input.seekg(start);

    // reading current token's characters from the stream
    uint32_t tksize = uint32_t(finish-start);
    string tkstr(tksize, ' ');
    input.read(&tkstr[0], tksize);

    // restoring original position
    input.seekg(pos);

    return tkstr;
}


template <typename TTokenId>
bool FSALexer<TTokenId>::Tokenize(istream& input, vector< pair<TTokenId, string> >& output)
{
    if (input)
    {
        fsa->Set(nullTk);
        istream::pos_type currentStart = 0;
        //if (currentStart == istream::pos_type(-1))
        //    return false;

        while (input.peek() != char_traits<char>::eof())
        {
            istream::pos_type currentStreamPos = input.tellg();
            TTokenIdSuperset fsaState = fsa->Receive(input.get());

            if (fsaState == invalidTk)
            {
                output.push_back(pair<TTokenId, string>(invalidTk, readTokenStr(input, currentStart, currentStreamPos)));
                return false;
            }

            if (fsaState != nullTk && currentStart == 0)
                currentStart = currentStreamPos;

            // If new FSA state corresponds to a token
            if (tokens.find(fsaState) != tokens.end())
            {
                TTokenId nextTk = TTokenId(fsaState);
                //istream::pos_type nextStart = input.tellg();
                output.push_back(pair<TTokenId, string>(nextTk, readTokenStr(input, currentStart, currentStreamPos)));
                input.seekg(currentStreamPos);
                fsa->Set(nullTk);
                currentStart = 0;
            }
        }
    }
    else
        return false;
}


#endif // FSALEXER_H
