//
// Created by twak on 14/10/2019.
//

#ifndef RESPONSIVELABEL_H
#define RESPONSIVELABEL_H

#include <QtWidgets>
#include <QLabel>
#include <QString>
#include <map>

using namespace std;

// Please add to, but DO NOT modify the existing constants in this file.
// label names that are displayed
const QString
        kHomeLink       = "home",
        kShoppingBasket = "basket",
        kSignIn         = "sign-in",
        kNavTabs        = "navigation tabs",
        kAdvert         = "advert",
        kSearchButton   = "search",
        kSearchResult   = "result",
        kSearchText     = "search query",
        kSearchForward  = ">",
        kSearchBackward = "<",
        kSearchOptions  = "search options",
        kBackToTop      = " ^ ",
        kAdvert2         = "advert",
        kLegal          = "About Us,Copyrights,Privacy & Legal";

// every label name has a colour
const map<QString, QString> kColours = {
        make_pair(kHomeLink,       "#ffaaff"),
        make_pair(kShoppingBasket, "#ffaaaa"),
        make_pair(kSignIn,         "#aaffaa"),
        make_pair(kNavTabs,        "#ffa9d3"),
        make_pair(kAdvert,         "#ffaa66"),
        make_pair(kSearchButton,   "#aaffff"),
        make_pair(kSearchResult,   "#99ddff"),
        make_pair(kSearchText,     "#aaaaff"),
        make_pair(kSearchForward,  "#5caeff"),
        make_pair(kSearchBackward, "#5caeff"),
        make_pair(kLegal,          "#C0C0C0"),
        make_pair(kBackToTop,      "#5caeff"),
        make_pair(kAdvert2,        "#ffaa66"),
        make_pair(kSearchOptions,  "#9dceff")
    };

class ResponsiveLabel : public QLabel {
public:
    ResponsiveLabel(QString name) ;
    ResponsiveLabel(QString name, QString colour) ;
};


#endif // RESPONSIVELABEL_H
