#pragma once

// DeckFactory declarations will be added here.

#include <vector>
#include <algorithm>
#include <random>
#include <cstddef>

template <typename C>
class DeckFactory{
    public:
        DeckFactory(const DeckFactory&) = delete;
        DeckFactory& operator=(const DeckFactory&) = delete;

        void shuffle();
        C* getNext();
        bool isEmpty() const;
        virtual ~DeckFactory() = 0;

    protected:
        std::vector<C*> deck;
        std::size_t currentIndex;
        DeckFactory(); // made it protected so only derived class can call it since its an abstract factory
};

template <typename C>
DeckFactory<C>::DeckFactory() : currentIndex(0){}

template <typename C>
void DeckFactory<C>::shuffle(){
    std::random_shuffle(deck.begin(), deck.end());
    currentIndex = 0;
}

template <typename C>
C* DeckFactory<C>::getNext(){
    if(isEmpty()){
        return nullptr;
    }
    
    return deck[currentIndex++];
}

template <typename C>
bool DeckFactory<C>::isEmpty() const {
    return currentIndex >= deck.size();
}

template <typename C>
DeckFactory<C>::~DeckFactory() {
    // Pure virtual destructors still need a body; derived destructors call this automatically to free deck items.
    for (C* item : deck) {
        delete item;
    }
}
