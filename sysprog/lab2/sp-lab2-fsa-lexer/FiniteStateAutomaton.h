#ifndef FINITESTATEAUTOMATON_H
#define FINITESTATEAUTOMATON_H

#include <QDebug>

#include <map>
#include <vector>
using namespace std;

template <typename TState, typename TSignal>
class FiniteStateAutomaton
{
public:
    FiniteStateAutomaton(TState initialState, TState invalidState);
    TState Receive(TSignal signal);
    TState Current() { return state; }
    TState Invalid() { return invalidState; }
    void Set(TState state);
    void AddTransition(TState sourceState, TSignal signal, TState destState, bool overwrite = true);
    void AddTransition(TState sourceState, const map<TSignal, TState>& destinations, bool overwrite = true);
    void AddTransition(TState sourceState, const vector<TSignal>& signalList, TState destState, bool overwrite = true);
    TState GetTransition(TState sourceState, TSignal signal);
    map<TSignal, TState> GetTransitions(TState sourceState);

private:
    TState state;
    const TState invalidState;
    //map<pair<TSignal, TState>, TState> transition;
    map<TState, map<TSignal, TState>> transition;
};

template <typename TState, typename TSignal>
FiniteStateAutomaton<TState, TSignal>::FiniteStateAutomaton(TState initialState, TState invalidState) : invalidState(invalidState)
{
    state = initialState;
}

template <typename TState, typename TSignal>
TState FiniteStateAutomaton<TState, TSignal>::Receive(TSignal signal)
{
    //pair<TSignal, TState> ss(signal, state);
    //map<pair<TSignal, TState>, TState>::iterator;
    //auto i = transition.find(ss);

    if (transition.find(state) == transition.end())
        return state;

    auto i = transition[state].find(signal);
    if (i != transition[state].end())
    {
        //qDebug() << state << "->(" << signal << ")->" << i->second;
        state = i->second;
    }
    else
    {
        //qDebug() << state << "->(" << signal << ")-> INVALID";
        state = invalidState;
    }

    return state;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::Set(TState state)
{
    //qDebug() << "->" << state;
    this->state = state;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::AddTransition(TState sourceState, TSignal signal, TState destState, bool overwrite)
{
    //pair<TSignal, TState> ss(signal, sourceState);
    //transition[ss] = destState;

    if (overwrite || transition[sourceState].find(signal) == transition[sourceState].end())
        transition[sourceState][signal] = destState;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::AddTransition(TState sourceState, const map<TSignal, TState>& destinations, bool overwrite)
{
    for (auto i = destinations.begin(); i != destinations.end(); ++i)
    {
        //pair<TSignal, TState> ss(i->first, sourceState);
        if (overwrite || transition[sourceState].find(i->first) == transition[sourceState].end())
            transition[sourceState][i->first] = i->second;
    }
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::AddTransition(TState sourceState, const vector<TSignal>& signalList, TState destState, bool overwrite)
{
    for (auto i = signalList.begin(); i != signalList.end(); ++i)
    {
        //pair<TSignal, TState> ss(*i, sourceState);
        if (overwrite || transition[sourceState].find(*i) == transition[sourceState].end())
            transition[sourceState][*i] = destState;
    }
}

template <typename TState, typename TSignal>
TState FiniteStateAutomaton<TState, TSignal>::GetTransition(TState sourceState, TSignal signal)
{
    //pair<TSignal, TState> ss(signal, sourceState);
    auto i = transition[sourceState].find(signal);
    if (i != transition[sourceState].end())
        return i->second;
    else
        return invalidState;
}

template <typename TState, typename TSignal>
map<TSignal, TState> FiniteStateAutomaton<TState, TSignal>::GetTransitions(TState sourceState)
{
    return transition[sourceState];
}

#endif // FINITESTATEAUTOMATON_H
