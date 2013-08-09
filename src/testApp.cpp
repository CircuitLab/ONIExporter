#include "testApp.h"

//--------------------------------------------------------------
void testApp::setupGUI()
{
    gui = new ofxUICanvas(0, NI_VIEW_HEIGHT, ofGetWidth(), 100);
    
    loadBtn = new ofxUIImageButton(42, 42, true, "gui/load.png", "LOAD");
    playBtn = new ofxUIImageButton(42, 42, true, "gui/play.png", "PLAY");
    recordBtn = new ofxUIImageButton(42, 42, true, "gui/record.png", "RECORD");
    stopBtn = new ofxUIImageButton(42, 42, true, "gui/stop.png", "STOP");
    rewindBtn = new ofxUIImageButton(42, 42, true, "gui/rewind.png", "REWIND");
    forwardBtn = new ofxUIImageButton(42, 42, true, "gui/forward.png", "FORWARD");
    
    gui->addWidgetRight(loadBtn);
    gui->addWidgetRight(new ofxUISpacer(20, 42));
    gui->addWidgetRight(rewindBtn);
    gui->addWidgetRight(playBtn);
    gui->addWidgetRight(forwardBtn);
    gui->addWidgetRight(recordBtn);
    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->setColorBack(ofColor(0));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(125));
    
}

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(30);
    ofEnableSmoothing();
    ofBackground(0);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    static ofxXmlSettings xml;
    xml.loadFile(ofToDataPath("config.xml"));
    xml.pushTag("config");
    
    if (PROJECT_NAME == xml.getValue("projectName", "")) {
        outputPath = xml.getValue("outputPath", "");
        if (0 < outputPath.length() && "/" != outputPath.substr(outputPath.length() - 1, 1)) {
            outputPath += "/";
        }
    }
    
    ofLogVerbose("outputPath: " + outputPath);
    
    openNIPlayer.setup();
    openNIPlayer.setLogLevel(OF_LOG_ERROR);
    openNIPlayer.setRegister(true);
    openNIPlayer.setLooped(false);
    openNIPlayer.setFrame(30);

	movieExporter.setup(NI_VIEW_WIDTH * 2, NI_VIEW_HEIGHT, movieExporter.BIT_RATE * 2, 30, CODEC_ID_MPEG4, "mp4");
    
    movieExporter.setRecordingArea(0, 0, NI_VIEW_WIDTH * 2, NI_VIEW_HEIGHT);
    
    isRecording = false;
    
    setupGUI();
    
    watcher.watch("");
}

//--------------------------------------------------------------
void testApp::update()
{
    openNIPlayer.update();
    
    if (isRecording) {
        ofLogVerbose("frame rate: " + ofToString(openNIPlayer.getFrameRate()) + ", current frame: " + ofToString(openNIPlayer.getCurrentFrame()) + " / " + ofToString(openNIPlayer.getTotalNumFrames()));
    }
    
    if (isRecording) {
        if (openNIPlayer.getCurrentFrame() == openNIPlayer.getTotalNumFrames()) {
            movieExporter.stop();
            openNIPlayer.setPaused(true);
            isRecording = false;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    if (true) {
        openNIPlayer.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        openNIPlayer.drawImage(NI_VIEW_WIDTH, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
    }
}

//--------------------------------------------------------------
void testApp::exit()
{
    openNIPlayer.stop();
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
        case 'l':
        case 'L': {
            ofFileDialogResult result = ofSystemLoadDialog("Select a .oni file");
            
            string path = result.getPath();
            ofFile oniFile(path);
            
            ofLogVerbose("path : " + path);
            
            if (oniFile.exists() && "ONI" == ofToUpper(oniFile.getExtension())) {
                oniFileName = result.getName();
                
                openNIPlayer.startPlayer(result.getPath());
                openNIPlayer.setPaused(true);
                openNIPlayer.firstFrame();
                
                ofLogVerbose("successfully loaded .oni");
            } else {
                ofLogVerbose("failed to load .oni");
            }
            break;
        }
		case ' ':
			playBtn->setImage("gui/pause.png");
            playBtn->setName("PAUSE");
    
            ofLogVerbose("isPlaying: " + ofToString(openNIPlayer.isPlaying()));
            
            if (openNIPlayer.isPlaying()) {
                if (openNIPlayer.isPaused()) {
                    openNIPlayer.firstFrame();
                    openNIPlayer.setLooped(false);
                    openNIPlayer.setPaused(false);
                } else {
                    openNIPlayer.setPaused(true);
                }
            } else {
                openNIPlayer.startPlayer(oniFileName);
            }
                        
            if (movieExporter.isRecording()) {
                movieExporter.stop();
                isRecording = false;
            } else {
                movieExporter.record(outputPath + oniFileName);
                movieFileName = movieExporter.outFileName();
                ofLogVerbose("out file name is " + movieFileName);
                isRecording = true;
            }
			
			break;
        case '1':
            openNIPlayer.setDepthColoring(COLORING_CYCLIC_RAINBOW);
            break;
        case '2':
            openNIPlayer.setDepthColoring(COLORING_BLUES);
            break;
        case '3':
            openNIPlayer.setDepthColoring(COLORING_COUNT);
            break;
        case '4':
            openNIPlayer.setDepthColoring(COLORING_GREY);
            break;
        case '5':
            openNIPlayer.setDepthColoring(COLORING_PSYCHEDELIC);
            break;
        case '6':
            openNIPlayer.setDepthColoring(COLORING_PSYCHEDELIC_SHADES);
            break;
        case '7':
            openNIPlayer.setDepthColoring(COLORING_RAINBOW);
            break;
        case '8':
            openNIPlayer.setDepthColoring(COLORING_STATUS);
            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    ofLogVerbose("gui event, value: " + ofToString(playBtn->getValue()));
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if (OFX_UI_WIDGET_IMAGEBUTTON) {
        if ("LOAD" == name && 1 == loadBtn->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Select a .oni file");
            
            if (result.bSuccess) {
                string path = result.getPath();
                ofFile oniFile(path);
                                
                if (oniFile.exists() && "ONI" == ofToUpper(oniFile.getExtension())) {
                    oniFileName = result.getName();
                    
                    openNIPlayer.startPlayer(oniFileName);
                    openNIPlayer.setPaused(true);
                    openNIPlayer.firstFrame();
                    
                    ofLogVerbose("successfully loaded .oni");
                } else {
                    ofLogVerbose("failed to load .oni");
                }
            } else {
                ofLogVerbose("User hit cancel");
            }
        } else if ("PLAY" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/pause.png");
            playBtn->setName("PAUSE");
            
            ofLogVerbose("isPlaying: " + ofToString(openNIPlayer.isPlaying()));
            
            if (openNIPlayer.isPlaying() && openNIPlayer.isPaused()) {
                openNIPlayer.setPaused(false);
                isRecording = true;
            } else {
                openNIPlayer.startPlayer(oniFileName);
                isRecording = false;
            }
            
            ofLogVerbose("isPlaying: " + ofToString(openNIPlayer.isPlaying()));
        } else if ("PAUSE" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/play.png");
            playBtn->setName("PLAY");
            
            if (openNIPlayer.isPlaying()) {
                openNIPlayer.setPaused(true);
                isRecording = false;
            }
        } else if ("RECORD" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/stop.png");
            recordBtn->setName("STOP");
        } else if ("STOP" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/record.png");
            recordBtn->setName("RECORD");
        } else if ("REWIND" == name && 1 == rewindBtn->getValue()) {
            openNIPlayer.previousFrame();
        } else if ("FORWARD" == name && 1 == forwardBtn->getValue()) {
            openNIPlayer.nextFrame();
        }
    }
}