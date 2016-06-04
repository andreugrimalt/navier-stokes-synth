/* Copyright 2016 Andreu Grimalt */

#include "ofApp.h"

// Code adapted from https://github.com/memo/ofxMSAFluid example

//--------------------------------------------------------------
void ofApp::setup(){
  
    
    // setup fluid stuff
    fluidSolver.setup(100, 100);

    fluidSolver.enableRGB(true).setFadeSpeed(0).setDeltaT(0.32).setVisc(0.00015).setColorDiffusion(0).setFadeSpeed(0);
    
    fluidDrawer.setup(&fluidSolver);
    
    fluidCellsX			= 400;
    
    drawFluid			= true;
    drawParticles		= true;
    
    ofSetFrameRate(30);
    
    ofSetVerticalSync(true);

    windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
    pMouse = msa::getWindowCenter();
    resizeFluid			= true;
    
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    fluidCellsX = 155;
    colorMult=1;
    velocityMult=10.5;
    fluidSolver.colorDiffusion=0;
    fluidSolver.setFadeSpeed(0.001);
    fluidSolver.setVisc(0.00009);
    fluidSolver.solverIterations=50;
    fluidSolver.deltaT=1.8;
    fluidSolver.doRGB=true;
    fluidSolver.doVorticityConfinement=true;
    drawFluid=true;
    drawParticles=false;
    fluidSolver.wrap_x=true;
    fluidSolver.wrap_y=true;
   
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 256;
    // make 50 oscillators
    osc=new ofxMaxiOsc[50];
    
    ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);

}


// add force and dye to fluid, and create particles
void ofApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
        pos.x = ofClamp(pos.x, 0.0f, 1.0f);
        pos.y = ofClamp(pos.y, 0.0f, 1.0f);
        
        int index = fluidSolver.getIndexForPos(pos);
        float t = ofGetElapsedTimef();
        float t1 = ofGetElapsedTimef()/2.0;
        float t2 = ofGetElapsedTimef()/4.0;
        if(addColor) {
            ofColor drawColor;
            drawColor.r=int(255*(ofSignedNoise(50,t)+1)/2);
            drawColor.g=int(255*(ofSignedNoise(50,t1)+1)/2);
            drawColor.b=int(255*(ofSignedNoise(50,t2)+1)/2);
            fluidSolver.addColorAtIndex(index, drawColor);
            if(drawParticles)
                particleSystem.addParticles(pos * ofVec2f(ofGetWindowSize()), 10);
        }

        if(addForce)
            fluidSolver.addForceAtIndex(index, vel * velocityMult);
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    if(resizeFluid) 	{
        fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
        fluidDrawer.setup(&fluidSolver);
        resizeFluid = false;
    }
    fluidSolver.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(255,255,255);
    if(drawFluid) {
        //ofClear(0);
        //glColor3f(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
        fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
    } else {
        // nothing here
    }
    if(drawParticles){
        particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid);
    }
}

//--------------------------------------------------------------
void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        // perform additive synthesis.
        for(int j=0; j<50;j++){
            // The amplitude of each sine wave is determined by the density (color) of a given point in the screen. white=louder, black=lower,
            volNew=fluidSolver.getVelocityAtIndex(j*(fluidSolver.getNumCells()/50)).length()*(fluidSolver.getColorAtIndex(j*(fluidSolver.getNumCells()/50)).r+fluidSolver.getColorAtIndex(j*(fluidSolver.getNumCells()/50)).g+fluidSolver.getColorAtIndex(j*(fluidSolver.getNumCells()/50)).b)*100;
            // amplitude can not be higher than 1
            if(volNew>1){
                volNew=1;
            }
            /* Interpolate volNew (not enough power!). This avoids cracking sounds when the amplitudes change fast
            if(abs(volNew-volOld)>0.1){
                for(float k=volOld; k<volNew; k+=0.01){
                    wave+=k*osc[j].sinebuf(10*j);
                }
            }*/
            // Add all the sine waves together. I use sinbuf for efficiency
            wave+=volNew*osc[j].sinebuf((5000.0/50.0)*j);

        }
        // normalise
        wave/=50;
        output[i*nChannels] = wave;
        output[i*nChannels+1] = wave;
    }
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    
    /* You can just grab this input and stick it in a double, then use it above to create output*/
        for (int i = 0; i < bufferSize; i++){
        //wave=input[i];
        
        /* you can also grab the data out of the arrays*/
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // add some fluid on mouseDragged
    // position of the mouse
    ofVec2f eventPos = ofVec2f(x, y);
    // position of the mouse between 0 and 1
    ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
    // speed of the mouse between 0 and 1
    ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
    // add fluid
    addToFluid(mouseNorm, mouseVel, true, true);
    // store the last position to calculate the speed
    pMouse = eventPos;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
