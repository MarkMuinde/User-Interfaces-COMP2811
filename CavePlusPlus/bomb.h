#ifndef BOMB_H
#define BOMB_H

#include "thing.h"
#include "placeable.h"

class Bomb : public Thing, public Placeable
{
    string getName()
    {
        return "bomb";
    }
};

#endif // BOMB_H
