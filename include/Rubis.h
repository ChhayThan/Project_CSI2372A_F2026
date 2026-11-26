#pragma once

// Rubis declarations will be added here.

#include <ostream>

class Rubis{
    public:
        operator int() const;
        friend std::ostream& operator<<(std::ostream& os, const Rubis& rubie);

    private:
        int rubies;
        Rubis(int);
        friend class RubisDeck;
};