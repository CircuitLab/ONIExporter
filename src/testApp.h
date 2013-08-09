#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxUI.h"
#include "ofxMovieExporter.h"

#define PROJECT_NAME "ONI Exporter"
#define NI_VIEW_WIDTH 640
#define NI_VIEW_HEIGHT 480


class testApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
    ofxUICanvas *gui;
    ofxUIImageButton *loadBtn, *playBtn, *pauseBtn, *recordBtn, *stopBtn, *rewindBtn, *forwardBtn;
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxOpenNI openNIPlayer;
    string oniFileName, movieFileName;
    bool isRecording;
    
	Apex::ofxMovieExporter movieExporter;
	ofImage testImage;
    string outputPath;
    
    DataFolderWatcher watcher;
};
