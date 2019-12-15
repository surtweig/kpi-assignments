#include <iostream>
#include <string>
#include "../sp-lab2-fsa-lexer/FiniteStateAutomaton.h"
#include <map>

using namespace std;

enum TSignal
{
    undef,
    dlm,
    cfr,
    ltr
};

void setupFSA(FiniteStateAutomaton<int, TSignal>& fsa)
{
    for (int i = 0; i <= 9; ++i)
    {
        int inext = i+1;
        if (inext == 10)
            inext = 0;
        fsa.AddTransition(i, TSignal::undef, i);
        fsa.AddTransition(i, TSignal::dlm, inext);
        fsa.AddTransition(i, TSignal::cfr, inext);
        fsa.AddTransition(i, TSignal::ltr, inext);
    }

    fsa.AddTransition(3, TSignal::dlm, 7);
    fsa.AddTransition(5, TSignal::cfr, 8);
    fsa.AddTransition(3, TSignal::ltr, 2);
}

int main(int argc, char *argv[])
{
    FiniteStateAutomaton<int, TSignal> fsa(0, -1);
    setupFSA(fsa);
    map<string, TSignal> strToSignal {
        {"dlm", TSignal::dlm},
        {"cfr", TSignal::cfr},
        {"ltr", TSignal::ltr}
    };
    string inp = "";
    while (inp != "stop" && inp != "exit")
    {
        cout << "Signal: ";
        cin >> inp;
        TSignal signal = TSignal::undef;
        if (strToSignal.find(inp) != strToSignal.end())
            signal = strToSignal[inp];

        int prevState = fsa.Current();
        fsa.Receive(signal);
        cout << prevState << " -> [" << inp << "] -> " << fsa.Current() << "\n";
    }
    return 0;
}

