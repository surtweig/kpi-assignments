#ifndef FSALEXER_H
#define FSALEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "FiniteStateAutomaton.h"

using namespace std;

template <typename TTokenId>
class FSALexer
{    

public:
    FSALexer(map<string, TTokenId> tokens,
             TTokenId stringDelimiterTokenId, TTokenId numberTokenId, TTokenId invalidTokenId,
             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish);

    vector<TTokenId> Tokenize(istream input);
    string GetToken(TTokenId tid);

private:
    FiniteStateAutomaton<TTokenId, char> fsa;
    map<string, TTokenId> tokens;
    TTokenId stringTk;
    TTokenId numberTk;
    TTokenId invalidTk;
    string commentLine;
    string commentStart;
    string commentFinish;
};

template <typename TTokenId>
FSALexer<TTokenId>::FSALexer(map<string, TTokenId> tokens,
                             TTokenId stringDelimiterTokenId, TTokenId numberTokenId, TTokenId invalidTokenId,
                             string commentLineDelimiter, string commentBlockStart, string commentBlockFinish) :
    fsa(invalidTokenId, invalidTokenId)

{

}


#endif // FSALEXER_H
