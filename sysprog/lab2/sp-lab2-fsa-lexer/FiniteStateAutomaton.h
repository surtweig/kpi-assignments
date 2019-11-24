#ifndef FINITESTATEAUTOMATON_H
#define FINITESTATEAUTOMATON_H

#include <map>
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
    void AddTransition(TState sourceState, TSignal signal, TState destState);
    void AddTransition(TState sourceState, map<TSignal, TState> destinations);

private:
    TState state;
    const TState invalidState;
    map<pair<TSignal, TState>, TState> transition;
};

template <typename TState, typename TSignal>
FiniteStateAutomaton<TState, TSignal>::FiniteStateAutomaton(TState initialState, TState invalidState) : invalidState(invalidState)
{
    state = initialState;
}

template <typename TState, typename TSignal>
TState FiniteStateAutomaton<TState, TSignal>::Receive(TSignal signal)
{
    pair<TSignal, TState> ss(signal, state);
    //map<pair<TSignal, TState>, TState>::iterator;
    auto i = transition.find(ss);
    if (i != transition.end())
        state = i->second;
    else
        state = invalidState;

    return state;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::Set(TState state)
{
    this->state = state;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::AddTransition(TState sourceState, TSignal signal, TState destState)
{
    pair<TSignal, TState> ss(signal, sourceState);
    transition[ss] = destState;
}

template <typename TState, typename TSignal>
void FiniteStateAutomaton<TState, TSignal>::AddTransition(TState sourceState, map<TSignal, TState> destinations)
{
    for (auto i = destinations.begin(); i != destinations.end(); ++i)
    {
        pair<TSignal, TState> ss(i->first, sourceState);
        transition[ss] = i->second;
    }
}
#endif // FINITESTATEAUTOMATON_H
