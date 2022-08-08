#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include <string>

#include "cave.h"
#include "explode.h"
#include "rock.h"
#include "thing.h"
#include "location.h"
#include "move.h"
#include "place.h"
#include "throw.h"

using namespace std;
int biggerSize;

Cave::Cave(int w, int h) : width(w), height(h) // width and height of the cave
{
    if ( width < 5 || height < 5)
        throw new logic_error("cave too small for tom.");

    if (width > height)
        biggerSize = width;
    else
        biggerSize = height;

    map = new Location**[biggerSize];

    for (int x = 0; x < width; x++) {
        Location** column = new Location*[biggerSize];
        map[x] = column;
        for (int y = 0; y < height; y++)
            column[y] = new Location();
    }

    // create some rocks
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++)
            if (
                    (x == 0 || y == 0 || x == (width-1) || y == (height-1)) )
                 map[x][y] -> add( new Rock() );
    }

    tom = new Tom();
    // add tom to the middle of the map
    tom -> setLocation( this, width/2,height/2);
}

Cave::~Cave()
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++)
    delete(map[x][y]); // fixme: I don't think this deletes all Locations and arrays declared in the constructor
}
}

Cave::Cave(const Cave & deepCopyOfCave){
    width = deepCopyOfCave.width;
    height = deepCopyOfCave.height;
    map = new Location**[biggerSize];
    for (int x=0; x<width;x++){
        Location** columns = new Location*[biggerSize];
        map[x] = columns;
        for(int y=0;y<height;y++){
            columns[y] = new Location();
            map[x][y]=columns[y];
        }
    }
    for (int x=0; x<width;x++){
        for(int y=0;y<height;y++){
            if (
                    (x == 0 || y == 0 || x == (width-1) || y == (height-1)) )
                 map[x][y] -> add( new Rock() );
        }
    }
    }

void Cave::command (string userCommand)
{
    if (Move().triggersOn(userCommand))
        Move().fire(*this, userCommand);
    else if (Place().triggersOn(userCommand))
        Place().fire(*this, userCommand);
    else if (Throw().triggersOn(userCommand))
        Throw().fire(*this, userCommand);
    else if (Explode().triggersOn(userCommand))
        Explode().fire(*this, userCommand);
    else
        cerr << "tom doesn't know how to "+userCommand << endl;;
}

void Cave::show()
{
    vector<Thing*>* thingsWithTom = map[tom -> getX()][tom -> getY()] -> getThings();

    for (int y = 0; y < height; y++) { // for all rows
            for (int x = 0; x < width; x++) // for all columns
                cout << map[x][y] -> show(); // output whatever we find there

            cout << "  "; // list the things at this location
            if (y <  (int) thingsWithTom -> size())
                cout << (*thingsWithTom)[y] -> getName();

            cout << endl;
        }

        cout << endl;
    }
