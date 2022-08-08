//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include <iostream>
#include <QTime>
#include <QList>
using namespace std;

// all buttons have been setup, store pointers here

void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    //change video currently playing
    initialName = button->url->fileName();
    emit nameChanged(button->url->fileName());
    setMedia( * button -> url);
    play();
}

void ThePlayer::setDuration(){
   qint64 duration = this->duration();
   //convert duration into Qtime format
   int s = (duration/1000) % 60;
   int m = (duration/60000) % 60;
   int h = (duration/3600000) % 24;
   QTime time(h, m,s);
    timeTotal = time;
}

void ThePlayer::setElapsed(){
    qint64 elapsed = this->position();
    //convert position into Qtime format
    int s = (elapsed/1000) % 60;
    int m = (elapsed/60000) % 60;
    int h = (elapsed/3600000) % 24;
    QTime time(h, m,s);
    timeElapsed = time;

}

void ThePlayer::playVid(ThePlayer *player, QPushButton *button){
    //change state and update play/pause button icon
    if(player->state()==PausedState){
        player->play();
        button->setIcon(QIcon(":/new/icons/resources/pause 2.svg"));
    }
    else{
        button->setIcon(QIcon(":/new/icons/resources/play 2.png"));
        player->pause();
    }
}

QString ThePlayer::getTimeTotal(){
    //returns duration in appropriate format
    if (timeTotal.hour()>0)
        return timeTotal.toString();
    else
        return timeTotal.toString("mm:ss");
}

QString ThePlayer::getTimeElapsed(){
    //returns time elapsed (position) in appropriate format
    if (timeElapsed.hour()>0)
        return timeElapsed.toString();
    else
        return timeElapsed.toString("mm:ss");
}

void ThePlayer::autoPlay(){
    setContent(buttons, infos);
}

void ThePlayer::skipVid(ThePlayer *player){
    //skip forward 2 seconds
    player->setPosition(player->position()+2000);
}

void ThePlayer::back(ThePlayer *player){
    //skip backwards 2 seconds
    player->setPosition(player->position()-2000);
}

void ThePlayer::seek(int time){
    //set position to time received from slider
    this->setPosition(time);
}

void ThePlayer::setSlider(){
    //set slider range and current position
    slider->setRange(0, this->duration() );
    slider->setValue(this->position() );
}

void ThePlayer::setRate(int index){
    //set the players playback rate to new value from choices
    QList<qreal> speeds = {0.5, 1, 1.5, 2};
    setPlaybackRate(speeds[index]);
}

void ThePlayer::setNewVolume(int value){
    //set the players volume to new value
    setVolume(value);
}
