#ifndef EXPLODE_H
#define EXPLODE_H

#include <string>
#include <iostream>
#include <sstream>
#include "cave.h"
#include "command.h"
#include "coin.h"
#include "mushroom.h"

using namespace std;

class Explode : public Command
{
public:
    Explode() : Command("explode"){};

    void fire(Cave &c, string userCommand)
    {
        int explodeAtX = c.getTom()->getX(), explodeAtY = c.getTom()->getY();

        explode(c, explodeAtX, explodeAtY);
        explode(c, explodeAtX + 1, explodeAtY);
        explode(c, explodeAtX - 1, explodeAtY);
        explode(c, explodeAtX, explodeAtY + 1);
        explode(c, explodeAtX, explodeAtY - 1);
    }

private:
    void chainReaction(Cave &c, int bombX, int bombY, Location* loc, vector<Thing *> things)
        {
            for (auto t : things)
                if (t->getName() != "Tom")
                    loc->remove(t);
                if (bombX != c.getWidth() - 1)
                    explode(c, bombX + 1, bombY);
                if (bombX != 0)
                    explode(c, bombX - 1, bombY);
                if (bombY != c.getHeight() - 1)
                    explode(c, bombX, bombY + 1);
                if (bombY != 0)
                    explode(c, bombX, bombY - 1);
        }
    void explode(Cave &c, int x, int y)
    {
        Location *loc = c.getMap()[x][y];
        auto things = *loc->getThings();
        for (auto t : things)
            if (t->getName() == "bomb")
                return chainReaction(c, x, y, loc, things);
    }
};

#endif // EXPLODE_H
