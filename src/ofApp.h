#pragma once

#include "MSAFluid.h"
//#include "MSATimer.h"
#include "ParticleSystem.h"
#include "ofxMaxim.h"
#include "ofMain.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
    
        float                   colorMult;
        float                   velocityMult;
        int                     fluidCellsX;
        bool                    resizeFluid;
        bool                    drawFluid;
        bool                    drawParticles;
    
        msa::fluid::Solver      fluidSolver;
        msa::fluid::DrawerGl	fluidDrawer;
    
        ParticleSystem          particleSystem;
    
        ofVec2f                 pMouse;
    
    // Audio stuff
    void audioRequested (float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived (float * input, int bufferSize, int nChannels); /* input method */
    
    int		initialBufferSize; /* buffer size */
    int		sampleRate;
    double wave,outputs[2];
    ofxMaxiOsc* osc;
    float volOld,volNew;
		
};
