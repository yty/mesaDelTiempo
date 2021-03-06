//
//  Simon.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef SIMONGAME
#define SIMONGAME

#include "ofMain.h"
#include "Game.h"
#include "ofxXmlSettings.h"

class LightButton : public ofPolyline {
public:    
    LightButton(){
    };
    
    ofSoundPlayer   sound;
    ofColor         color;
};

class Simon: public Game{
public:
    Simon();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    objectAdded(ofxBlob &_blob);
    
    void    update();
    void    render();
    
private:
    bool    loadPoints(ofPolyline &poly, string file);
    bool    someonePlaying();
    
    ofSoundPlayer   gameOverSound;
    ofTrueTypeFont  digitFont;
    ofImage         mask;
    ofImage         led;
    
    LightButton     buttons[4];
    
    vector<int>     secuence;
    int             nUsers, turn, countDown, nextButton, score;
};

#endif
