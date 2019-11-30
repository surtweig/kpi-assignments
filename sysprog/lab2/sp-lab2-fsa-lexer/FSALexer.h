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
    // Assuming TTokenId is a complete enumeration type
    //typedef typename underlying_type<TTokenId>::type TTokenIdSuperset;
    typedef int TTokenIdSuperset;

    FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators,
             TTokenId nullTokenId, TTokenId invalidTokenId,
             TTokenId stringDelimiterTokenId, TTokenId numberTokenId,
             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish);

    bool Tokenize(istream& input, vector< pair<TTokenId, string> >& output);
    string GetToken(TTokenId tid);

private:
    FiniteStateAutomaton<TTokenIdSuperset, char> fsa;
    map<string, TTokenId> tokens;
    TTokenId nullTk;
    TTokenId invalidTk;
    TTokenId stringTk;
    TTokenId numberTk;
    string commentLine;
    string commentStart;
    string commentFinish;

    string readTokenStr(istream& input, uint32_t start, uint32_t end = 0);
    TTokenIdSuperset getMaxToken();
};

template <typename TTokenId>
FSALexer<TTokenId>::FSALexer(map<string, TTokenId> reservedWords, map<string, TTokenId> operators,
                             TTokenId nullTokenId, TTokenId invalidTokenId,
                             TTokenId stringDelimiterTokenId, TTokenId numberTokenId,
                             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish) :
    fsa(TTokenIdSuperset(nullTokenId), TTokenIdSuperset(invalidTokenId)),
    tokens(reservedWords),
    nullTk(nullTokenId),
    invalidTk(invalidTokenId),
    numberTk(numberTokenId),
    stringTk(stringDelimiterTokenId),
    commentLine(commentLineDelimiter),
    commentStart(commentBlockStart),
    commentFinish(commentBlockFinish)
{
    tokens.insert(operators.begin(), operators.end());

    TTokenIdSuperset maxTokenId = getMaxToken();
    TTokenIdSuperset stateIndexer = maxTokenId+1;

    for (auto i = tokens.begin(); i != tokens.end(); ++i)
    {

    }
    /*
    TTokenIdSuperset firstCharState = stateIndexer;
    for (char c = 0; c < char(0xff); ++c)
    {
        if (isprint(c))
        {
            fsa.AddTransition(nullTokenId, c, firstCharState + c);
        }
    }
    stateIndexer += 0xff;
    */


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
string FSALexer<TTokenId>::readTokenStr(istream& input, uint32_t start, uint32_t end)
{
    // storing current stream position
    istream::pos_type pos = input.tellg();

    if (end == 0)
        end = pos;

    // returning to beginning of the token
    input.seekg(start);

    // reading current token's characters from the stream
    uint32_t tksize = uint32_t(end-start);
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
        fsa.Set(nullTk);
        //TTokenId currentTk = nullTk;
        istream::pos_type currentStart = input.tellg();
        if (currentStart == istream::pos_type(-1))
            return false;

        while (input.peek() != char_traits<char>::eof())
        {
            istream::pos_type currentStreamPos = input.tellg();
            TTokenIdSuperset fsaState = fsa.Receive(input.get());

            // If new FSA state corresponds to a token
            if (tokens.find(fsaState) != tokens.end())
            {
                TTokenId nextTk = TTokenId(fsaState);
                if (nextTk != invalidTk)
                {
                    istream::pos_type nextStart = input.tellg();
                    //if (currentTk != nextTk)
                    {
                        //if (currentTk != nullTk)
                        {
                            /*
                            // storing current stream position
                            istream::pos_type pos = input.tellg();

                            // returning to beginning of the current token
                            input.seekg(currentStart);

                            // sanity check
                            assert(currentStart < pos);

                            // reading current token's characters from the stream
                            uint32_t tksize = uint32_t(pos-currentStart);
                            string tkstr(tksize, ' ');
                            input.read(&tkstr[0], tksize);

                            // adding detected token
                            output.push_back(pair<TTokenId, string>(currentTk, tkstr));

                            // proceeding to next token
                            input.seekg(pos);
                            */
                            output.push_back(pair<TTokenId, string>(nextTk, readTokenStr(input, currentStart)));
                        }
                        //currentTk = nextTk;
                        currentStart = nextStart;
                    }
                }
                else
                {
                    output.push_back(pair<TTokenId, string>(invalidTk, readTokenStr(input, currentStart)));
                    return false;
                }
            }
        }
    }
    else
        return false;
}


#endif // FSALEXER_H
