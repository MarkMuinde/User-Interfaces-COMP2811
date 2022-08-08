#ifndef THROW_H
#define THROW_H

#include "coin.h"
#include "command.h"
#include "mushroom.h"
#include "rock.h"
#include <iostream>

using namespace std;

class Throw : public Command {
public:
    Throw() : Command("throw") {};

    void fire(Cave& c, string userCommand ) {

            string s = tail(userCommand);
            int x = c.getTom()->getX() , y = c.getTom()->getY();
            int X = c.getWidth(), Y = c.getHeight();

            if ((s[0] == 'c' && s[5]=='n')){
               y--;
               if (y<(Y-1)){
               Location* loc =c.getMap()[c.getTom()->getX()][c.getTom()->getY()-1];
               loc -> add(new Coin());}}

            else if ((s[0] == 'c' && s[5]=='e')){
               x++;
               if (x<(X-1)){
               Location* loc =c.getMap()[c.getTom()->getX()+1][c.getTom()->getY()];
               loc -> add(new Coin());}}

            else if ((s[0] == 'c' && s[5]=='s')){
               y++;
               if (y<(Y-1)){
               Location* loc = c.getMap()[c.getTom()->getX()][c.getTom()->getY()+1];
               loc -> add(new Coin());}}

            else if ((s[0] == 'c' && s[5]=='w')){
               x--;
               if (x<(X-1)){
               Location* loc = c.getMap()[c.getTom()->getX()-1][c.getTom()->getY()];
               loc -> add(new Coin());}}

            else if ((s[0] == 'm' && s[9]=='n')){
               y--;
               if (y<(Y-1)){
               Location* loc = c.getMap()[c.getTom()->getX()][c.getTom()->getY()-1];
               loc -> add(new Mushroom());}}

            else if ((s[0] == 'm' && s[9]=='e')){
               x++;
               if (x<(X-1)){
               Location* loc = c.getMap()[c.getTom()->getX()+1][c.getTom()->getY()];
               loc -> add(new Mushroom());}}

            else if ((s[0] == 'm' && s[9]=='s')){
               y++;
               if (y<(Y-1)){
               Location* loc = c.getMap()[c.getTom()->getX()][c.getTom()->getY()+1];
               loc -> add(new Mushroom());}}

            else if ((s[0] == 'm' && s[9]=='w')){
               x--;
               if (x<(X-1)){
               Location* loc = c.getMap()[c.getTom()->getX()-1][c.getTom()->getY()];
               loc -> add(new Mushroom());}}

            else
                cerr << "I don't know how to place a " << userCommand << endl;
        }
    };

#endif // THROW_H
