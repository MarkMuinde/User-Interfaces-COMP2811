//
// Created by twak on 07/10/2019.
//

#include "responsive_layout.h"
#include "responsive_label.h"
#include <iostream>

using namespace std;

// you should probably make extensive changes to this function
void ResponsiveLayout::setGeometry(const QRect &r /* our layout should always fit inside r */ ) {

    QLayout::setGeometry(r);
    int x=1;

    // for all the Widgets added in ResponsiveWindow.cpp
    for (int i = 0; i < list_.size(); i++) {

        QLayoutItem *o = list_.at(i);

        try {
            // cast the widget to one of our responsive labels
            ResponsiveLabel *label = static_cast<ResponsiveLabel *>(o->widget());

            if (label == NULL) // null: cast failed on pointer
                cout << "warning, unknown widget class in layout" << endl;

            //navigation tab
            else if (label -> text() == kNavTabs ){ // headers go at the top
                label -> setFont (QFont("SansSerif",18, QFont::Bold));
                label -> setGeometry(5+r.x(),0+r.y(),
                                     r.width()-10, 40);}

            // show a search button on any, at the second tab from the left of the window
            else if (label -> text() == kSearchButton ){
                label -> setFont (QFont("SansSerif",13, QFont::Bold));
                label -> setGeometry(50+r.x(),45+r.y(),40, 30);}

            //search bar
            else if (label -> text() == kSearchText ){
                label -> setFont (QFont("SansSerif",13, QFont::Bold));
                label -> setGeometry(95+r.x(),45+r.y(),
                                     r.width() - 200,30);}

            //previous page
            else if (label -> text() == kSearchBackward ){
                label -> setGeometry(95+r.x(),80+r.y(),20,20);}

            //next page
            else if (label -> text() == kSearchForward ){
                label -> setGeometry(r.width() - 125+r.x(),
                                     80+r.y(),20, 20);}

            //space for any and all filters to refine search options
            else if (label -> text() == kSearchOptions ){
                label -> setGeometry(120+r.x(),80+r.y(),r.width()-250,20);}

            //shopping basket for keeping items to be bought later
            else if (label -> text() == kShoppingBasket ){
                label -> setFont (QFont("SansSerif",13, QFont::Bold));
                label -> setGeometry(r.width() -96+r.x(),
                                     45+r.y(),45, 30);}

            //sign in for users
            else if (label -> text() == kSignIn ){
                label -> setFont (QFont("SansSerif",12, QFont::Bold));
                label -> setGeometry(r.width() - 45+r.x(),
                                     45+r.y(),40, 30);}

            //home link
            else if (label -> text() == kHomeLink ){
                label -> setFont (QFont("SansSerif",13, QFont::Bold));
                label -> setGeometry(5+r.x(),45+r.y(),40, 30);}

            //back to top for refining search parameters or changing pages
            else if (label -> text() == kBackToTop
                     && r.height() > 600){
                label -> setGeometry(95+r.x(),
                                     r.height() - 95+r.y(),
                                     r.width()-200,20);}

            //space for any and all adverts to be tiled left of the screen
            else if (label -> text() == kAdvert
                     && r.width() > 500){
                label -> setGeometry(5+r.x(),105+r.y(),85,
                                     r.height()-180);}

            //space for any and all adverts to be tiled right of the screen
            else if (label -> text() == kAdvert2
                     && r.width() > 500){
                label -> setGeometry(r.width()-300+r.x(),105+r.y(),85,
                                     180);}

            // fixme: focus group did not like this behaviour for the search result element.
            // fixed: now search results appear at the centre of the screen inbetween adverts
            // and a large gap between them
            else if (label -> text() == kSearchResult ){
                label -> setGeometry(95+r.x(),
                                     x*105+r.y(),r.width()-200,80);
                x++;}

            //space for copyrights, legal info,about,etc.
            else if (label -> text() == kLegal
                     && r.width() > 400
                     && r.height() > 500 ){
                label -> setFont (QFont("SansSerif",18, QFont::Bold));
                label -> setGeometry(0+r.x(),r.height()-70+r.y(),
                                     r.width(), 70);}

            else // otherwise: disappear label by moving out of bounds
                label -> setGeometry (-1,-1,0,0);
        }
        catch (bad_cast) {
            // bad_case: cast failed on reference...
            cout << "warning, unknown widget class in layout" << endl;
        }
    }
}

// following methods provide a trivial list-based implementation of the QLayout class
int ResponsiveLayout::count() const {
    return list_.size();
}

QLayoutItem *ResponsiveLayout::itemAt(int idx) const {
    return list_.value(idx);
}

QLayoutItem *ResponsiveLayout::takeAt(int idx) {
    return idx >= 0 && idx < list_.size() ? list_.takeAt(idx) : 0;
}

void ResponsiveLayout::addItem(QLayoutItem *item) {
    list_.append(item);
}

QSize ResponsiveLayout::sizeHint() const {
    return minimumSize();
}

QSize ResponsiveLayout::minimumSize() const {
    return QSize(320,320);
}

ResponsiveLayout::~ResponsiveLayout() {
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}
