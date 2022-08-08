//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QSlider>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    QTime timeElapsed;

public:
    QTime timeTotal;
    QSlider *slider;
    QString initialName;
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
        connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(setDuration())); //keep duration value up to date
        connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(setElapsed())); //keep time elapsed value up to date

    }


    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    //allow access to player timings in string form
    QString getTimeTotal();
    QString getTimeElapsed();

signals:
    void nameChanged(QString);
    void durationChanged();
    void positionChanged();

private slots:

    // change the image and video for one button every one second <- Not currently in use
    void shuffle();

    //change player position in video
    void seek(int);

    //change player volume
    void setNewVolume(int);

    //video play/pause action
    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);

    //update duration and elapsed time variables
    void setDuration();
    void setElapsed();

    //set progress slider position
    void setSlider();

    //set playback rate
    void setRate(int);

    //video functions
    void playVid(ThePlayer* player, QPushButton* button);
    void skipVid(ThePlayer *player);
    void back(ThePlayer *player);
    void autoPlay();


};

#endif //CW2_THE_PLAYER_H
