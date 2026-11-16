// DeckFactory declarations will be added here.

// should this class be abstract??!?!?!

#include <vector>
#include <algorithm>
#include <random>

template <typename C>
class DeckFactory{
    public:
        void shuffle();
        C* getNext();
        bool isEmpty() const;
        virtual ~DeckFactory();

    protected:
        std::vector<C*> deck;
        int currentIndex;
        DeckFactory(); // made it protected so only derived class can call it since its an abstract factory
};

// Template implementation of abstract class above

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
DeckFactory<C>::~DeckFactory() = default;
