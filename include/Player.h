#pragma once

// Player declarations will be added here.

// Update the class accordingly "Student 2 work just did bare bone for now"

#include "Types.h"
#include "Rubis.h"
#include <string>
#include <ostream>

class Player{
    public:
        Player(const std::string&, Side);
        
        std::string getName() const;

        void setActive(bool);
        bool isActive() const;

        int getNRubies() const;
        void addRubis(const Rubis&);

        void setDisplayMode(bool);

        Side getSide() const;
        void setSide(Side);
        
        friend std::ostream& operator<<(std::ostream& os, const Player& player);

    private:
        std::string name;
        bool status;
        int rubies;
        Side playerSide;
        bool endOfGame;
};
