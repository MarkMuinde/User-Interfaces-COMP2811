#ifndef VIDEO_PLAY_H
#define VIDEO_PLAY_H

#include <QPushButton>
#include <QMediaPlayer>
#include "the_player.h"

class VideoPlay: public QPushButton  //video control buttons
{
    Q_OBJECT
public:
    ThePlayer *myPlayer;
    QPushButton *myButton;
    int type;   //keep track of what button does
    TheButtonInfo* info; //keep track of next and prev
    vector<TheButtonInfo> vids; //storing list of all videos

    VideoPlay(){
            connect(this, SIGNAL(released()), this, SLOT (clickedPlay() ));
    };

    void init(ThePlayer *player, QPushButton *b);
    void initInfo(vector<TheButtonInfo> v, ThePlayer* p);

    //determine if button clicked was forward skip ,backwards skip, next or previous and call coresponding functions
    void check(){
            if(this->type==2){
                connect(this, SIGNAL(released()), this, SLOT (clickedSkip() ));
            }
            if(this->type==3){
                connect(this, SIGNAL(released()), this, SLOT (clickedBack() ));
            }
            if(this->type==4){
                connect(this, SIGNAL(released()), this, SLOT (clickedNext() ));
            }
            if(this->type==5){
                connect(this, SIGNAL(released()), this, SLOT (clickedPrev() ));
            }
    }
private slots:
    // button actions
    void clickedPlay();
    void clickedSkip();
    void clickedBack();
    void clickedNext();
    void clickedPrev();

signals:
    //button reactions
    void playVid(ThePlayer *player, QPushButton *button);
    void skipVid(ThePlayer *player);
    void back(ThePlayer *player);
    void jumpTo(TheButtonInfo* button);
};

#endif // VIDEO_PLAY_H
