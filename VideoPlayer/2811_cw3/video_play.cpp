
#include "video_play.h"

void VideoPlay::init(ThePlayer *player, QPushButton *b){
    myPlayer = player;
    myButton = b;
}

void VideoPlay::clickedPlay(){
    emit playVid(myPlayer,myButton);
}

void VideoPlay::clickedSkip(){
    emit skipVid(myPlayer);
}

void VideoPlay::clickedBack(){
    emit back(myPlayer);
}

void VideoPlay::initInfo(vector<TheButtonInfo> v, ThePlayer* p){
    vids = v;
    myPlayer = p;
}

void VideoPlay::clickedNext(){

    for ( int i = 0; i < (int)vids.size(); i++ ) {
        if(myPlayer->initialName == vids.at(i).url->fileName()){
            if(i!=(int)vids.size()-1)
                info = &vids.at(i+1);
            else
                info = &vids.at(0);
        }
    }
    emit jumpTo(info);
}

void VideoPlay::clickedPrev(){
    for ( int i = 0; i < (int)vids.size(); i++ ) {
        if(myPlayer->initialName == vids.at(i).url->fileName()){
            if(i!=0)
                info = &vids.at(i-1);
            else
                info = &vids.at((int)vids.size()-1);
        }
    }
    emit jumpTo(info);
}








