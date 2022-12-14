//
// Created by twak on 14/10/2019.
// DO NOT EDIT THIS FILE
//

#include "folder_compressor.h"
#include "screenshot.h"
#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <QString>
#include<ctime>

using namespace std;


inline bool exists(const std::string& name) {  // https://stackoverflow.com/questions/12774207
    ifstream f(name.c_str());
    return f.good();
}

void pack(const string& srcLocation) {
    string username;

    if (!exists(srcLocation+"/report/index.html")) {
        cout << "no report file generated. Run with 'test' before 'pack'." << endl;
        exit(-3);
    }

    string patchFile = srcLocation+"/../submit.patch";

    FolderCompressor fc(NULL);
    bool success = fc.compressFolder( QString(srcLocation.c_str()),
                                      QString ( patchFile.c_str() ) );

    if (!success)
        cout << "failed to write " << patchFile << endl;
    else
        cout << "please submit: " << patchFile << endl;
}

void unpack (const string& srcLocation) {
    string patchFile = srcLocation+"/../submit.patch";
    string tmpFile = srcLocation+"/../tmp_"+ to_string( std::time(0) )+"/";

    FolderCompressor fc(NULL);
    bool success = fc.decompressFolder(QString ( patchFile.c_str() ), QString (tmpFile.c_str()) );

    if (!success)
        cout << "failed to unpack " << patchFile << endl;
    else
        cout << "files unpacked to: " << tmpFile << endl;
}

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    cout << "Qt version: " << QT_VERSION_STR << endl;

    // have you read the basic instructions?
    string hints(" e.g \"/usr/home/twak/cw1\" or \"C:\\Users\\tom kelly\\Desktop\\responsive\". ");
    hints += "Include the quotes. Exclude final slash(\\ or /).\n\n\nquitting :(";

    if (argc < 2) {
        cout << "set command line argument to include location of this source." << hints << endl;
        exit(-1);
    }

    // have you given a valid source folder (cw1 culprits: you know who you are...)
    string srcLocation(argv[1]);
    if (!exists(srcLocation+"/responsive_window.cpp")) {
        cout << "couldn't find source code in "+srcLocation+"." << hints << endl;
        exit(-1);
    }

    if (argc == 3)  {
        string arg = string(argv[2]);

        if (arg.compare("test") == 0) {
            // create the Qt Application
            QApplication app(argc, argv);

            // grading-mode - run the screenshot subclass
            Screenshot screenshot (srcLocation);
            screenshot.show();

            return app.exec();
        } else if (arg.compare("pack")==0)
            pack(srcLocation);
        else if (arg.compare("unpack")==0)
            unpack(srcLocation);
        else
            cout <<  "unknown command line argument: "<< arg <<endl;
    } else { // run interactively
        // create the Qt Application
        QApplication app(argc, argv);
        // interactive mode - run the regular superclass
        ResponsiveWindow window;
        window.show();
        // wait for the app to terminate
        return app.exec();
    }

    return 0;
}
