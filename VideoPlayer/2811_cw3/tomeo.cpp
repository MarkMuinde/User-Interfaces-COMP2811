/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QScrollArea>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QSlider>
#include <QComboBox>
#include "the_player.h"
#include "the_button.h"
#include "timer.h"
#include "video_play.h"

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;
        }
    }

    return out;
}


void setupSliders(QSlider* progress, QSlider* vSlider, ThePlayer *player){

    player->slider = progress;

    //ensures slider updates with position changes and can be used to change position
    player->connect(progress, SIGNAL(sliderMoved(int)), player, SLOT(seek(int)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), player, SLOT(setSlider()));

    //connect volume slider with function to change the players volume
    player->connect(vSlider, SIGNAL(sliderMoved(int)), player, SLOT(setNewVolume(int)));
    vSlider->setRange(0,100);
    vSlider->setMaximumWidth(100);

}

void setupNextPrevButtons(VideoPlay *next, VideoPlay *prev, ThePlayer *player){
    next->setIcon((QIcon(":/new/icons/resources/skip forward.png")));
    next->type = 4;
    next->check();
    next->connect(next,SIGNAL(jumpTo(TheButtonInfo*)),player,SLOT(jumpTo(TheButtonInfo*)));

    prev->setIcon((QIcon(":/new/icons/resources/skip back.png")));
    prev->type = 5;
    prev->check();
    next->connect(prev,SIGNAL(jumpTo(TheButtonInfo*)),player,SLOT(jumpTo(TheButtonInfo*)));
}

void setupBackForwardButtons(VideoPlay *skip, VideoPlay *rewind, ThePlayer *player){
    skip->setIcon((QIcon(":/new/icons/resources/fast forward.png")));
    skip->setMaximumWidth(60);
    skip->setFixedHeight(24);
    skip->type = 2;
    skip->check();
    skip->connect(skip,SIGNAL(skipVid(ThePlayer*)),player,SLOT(skipVid(ThePlayer*)));
    skip->init(player,skip);
    rewind->setIcon((QIcon(":/new/icons/resources/fast backwards.png")));
    rewind->setMaximumWidth(60);
    rewind->setFixedHeight(24);
    rewind->type = 3;
    rewind->check();
    rewind->connect(rewind,SIGNAL(back(ThePlayer*)),player,SLOT(back(ThePlayer*)));
    rewind->init(player,rewind);

}

VideoPlay * makePlayPauseButton(ThePlayer *player){
    //Adding a play/pause button
    VideoPlay *playButton = new VideoPlay();
    playButton->setIcon(QIcon(":/new/icons/resources/pause 2.svg"));
    playButton->setMaximumWidth(60);
    playButton->setFixedHeight(24);
    playButton->connect(playButton,SIGNAL(playVid(ThePlayer*,QPushButton*)),player,SLOT(playVid(ThePlayer*,QPushButton*)));
    playButton->init(player,playButton);
    return playButton;
}

void setupTitle(QLabel *main, QLabel *title, ThePlayer *player){
    title->connect(player, SIGNAL(nameChanged(QString)), title, SLOT(setText(QString)));
    title->setFixedHeight(35);
    title->setFont(QFont ("Arial", 24));
    main->setFixedHeight(35);
    main->setFont(QFont ("Arial", 24));
}
Timer * makeTimer(ThePlayer *player){
    //create label to display elapsed time and duration
    Timer *time = new Timer(player);
    QObject::connect(player, SIGNAL(durationChanged(qint64)), time, SLOT(setTime(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), time, SLOT(setTime(qint64)));
    return time;
}

QHBoxLayout * makeVolumeControls(QSlider *vSlider){
    //volume icon
    QLabel *vIcon = new QLabel();
    vIcon->setFixedSize(20, 20);
    vIcon->setPixmap(QPixmap(":/new/icons/resources/volume.png"));

    //layout for volume controls
    QHBoxLayout *volume = new QHBoxLayout();
    volume->addWidget(vIcon);
    volume->addWidget(vSlider);

    return volume;
}

QComboBox * makePlaybackRateControls(ThePlayer *player){
    //playback rate combo box
    QComboBox *rate = new QComboBox();
    rate->addItem("x0.5");
    rate->addItem("x1.0");
    rate->addItem("x1.5");
    rate->addItem("x2.0");
    rate->setCurrentIndex(1);
    rate->setFixedHeight(24);
    QObject::connect(rate, SIGNAL(currentIndexChanged(int)), player, SLOT(setRate(int)));
    return rate;
}

QHBoxLayout * makeTitleLayout(ThePlayer *player){
    //Implementation of video title as a QLabel
    //Video title should equal the file name of the currently playing file.
    QLabel *main = new QLabel("Currently Playing: ");
    QLabel *title = new QLabel(player->initialName);
    setupTitle(main, title, player);

    //Title layout in the main window.
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(main);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    return titleLayout;
}

QHBoxLayout * makeContorlsLayout(ThePlayer *player, VideoPlay *prev, VideoPlay *next, VideoPlay *rewind, VideoPlay *skip, QSlider *vSlider, QSlider *progress){
    //Play/Pause control layout in the main window.
    QHBoxLayout *playPauseLayout = new QHBoxLayout();
    playPauseLayout->addWidget(prev);
    playPauseLayout->addWidget(rewind);
    playPauseLayout->addWidget(makePlayPauseButton(player));
    playPauseLayout->addWidget(skip);
    playPauseLayout->addWidget(next);
    playPauseLayout->addWidget(makePlaybackRateControls(player));
    playPauseLayout->addItem(makeVolumeControls(vSlider));
    playPauseLayout->addWidget(progress);
    playPauseLayout->addWidget(makeTimer(player));
    return playPauseLayout;
}

QScrollArea * makeScrollArea(QGridLayout *grid){
    //create scroll area filled with previewing videos
    QWidget *bottom_videos = new QWidget();
    bottom_videos->setLayout(grid);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(bottom_videos);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFixedHeight(scrollArea->sizeHint().height() +10);
    return scrollArea;
}

void makePreview(QWidget *window, TheButton *button, QGridLayout *grid, int i){
    //setup video player/widget with initial video set as the buttons video
    QVideoWidget *miniVideoWidget = new QVideoWidget();
    ThePlayer *miniPlayer = new ThePlayer();
    miniPlayer->setVideoOutput(miniVideoWidget);
    miniPlayer->setMedia(*button->info->url);
    miniPlayer->play();
    miniPlayer->setVolume(0);
    miniVideoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    miniVideoWidget->setFixedHeight(0.2*window->geometry().height());
    miniVideoWidget->setFixedWidth(float((0.2*16/9))*window->geometry().height());

    //setup label to read the videos name
    button->setFont(QFont ("Arial", 16));
    button->setFixedWidth(float((0.15*16/9))*window->geometry().height());
    button->setText(button->info->url->fileName());

    //label to display video duration
    Timer *duration = new Timer(miniPlayer);
    QObject::connect(miniPlayer, SIGNAL(durationChanged(qint64)), duration, SLOT(setDuration(qint64)));

    //add players, buttons and duartion labels to grid
    grid->addWidget(miniVideoWidget, 0, i, Qt::AlignCenter);
    grid->addWidget(button, 1, i, Qt::AlignCenter);
    grid->addWidget(duration, 2, i, Qt::AlignCenter);
}

void initalNoVideos(){
    const int result = QMessageBox::question(
                NULL,
                QString("Tomeo"),
                QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                QMessageBox::Yes |
                QMessageBox::No );

    switch( result )
    {
    case QMessageBox::Yes:
      QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
      break;
    default:
        break;
    }
    exit(-1);
}
int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {
        initalNoVideos();
    }

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(930, 680);

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    //the slider to show video progress
    QSlider *progress = new QSlider(Qt::Horizontal);

    //the volume slider
    QSlider *vSlider = new QSlider(Qt::Horizontal);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);
    setupSliders(progress, vSlider, player);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();

    // a list of the buttons
    vector<TheButton*> buttons;

    //the buttons and videos are added in a grid layout
    QGridLayout *grid = new QGridLayout();

    // create the preview video players for the bottom of the page
    for ( int i = 0; i < (int)videos.size(); i++ ) {

        //create buttons for each video in folders
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        button->init(&videos.at(i));

        //create video players and durations comprising preview and add to layout grid
        makePreview(&window, button, grid, i);
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    //Adding next and previous
    VideoPlay *next = new VideoPlay();
    VideoPlay *prev = new VideoPlay();
    setupNextPrevButtons(next, prev, player);
    next->initInfo(videos,player);
    prev->initInfo(videos,player);

    //Adding seek adn rewind buttons
    VideoPlay *skip = new VideoPlay();
    VideoPlay *rewind = new VideoPlay();
    setupBackForwardButtons(skip, rewind, player);

    // add the video and the buttons to the top level widget
    top->addLayout(makeTitleLayout(player));
    top->addWidget(videoWidget);
    top->addSpacing(0);
    top->addLayout(makeContorlsLayout(player, prev, next, rewind, skip, vSlider, progress));
    top->addWidget(makeScrollArea(grid));

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}

