//
//  Simon.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Simon.h"

Simon::Simon(){
    width = 800*1.625;
    height = 600*1.625;

    trackMode = TRACK_JUST_OBJECT;
    nUsers = 2;
}

void Simon::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center,
                        _space.height* height * 0.85,
                        _space.height* height * 0.85);

    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0,255);
    fbo.end();

    gameOverSound.loadSound("simon/gameover.mp3");
    mask.loadImage("simon/mask.png");
    led.loadImage("simon/LED.png");
    digitFont.loadFont("simon/digit.ttf", space.height * 0.04, true, true);

    //  Green button
    //
    buttons[0].color.set(11, 185, 32);
    buttons[1].color.set(255,35,30);
    buttons[2].color.set(2,174,247);
    buttons[3].color.set(252,252,1);

    for(int i = 0; i < 4; i++){
        loadPoints(buttons[i],"simon/"+ofToString(i)+".xml");
        buttons[i].sound.loadSound("simon/"+ofToString(i)+".mp3");
    }

    reset();
}

bool Simon::loadPoints(ofPolyline &_poly, string _file){
    bool loaded = false;

    ofxXmlSettings XML;

    if (XML.loadFile(_file)){
        _poly.clear();

        // Load the mask path
        if ( XML.pushTag("mask") ){
            int totalMaskCorners = XML.getNumTags("point");
            if (totalMaskCorners > 0){
                _poly.clear();
            }

            for(int i = 0; i < totalMaskCorners; i++){
                XML.pushTag("point",i);
                _poly.addVertex( space.x + XML.getValue("x", 0.0) * space.width, space.y + XML.getValue("y", 0.0)*space.height);
                XML.popTag();
            }
            XML.popTag(); // Pop "mask"
        }
    } else
        ofLog(OF_LOG_ERROR,"Simon: loading file " + _file );

    return loaded;
}

bool Simon::someonePlaying(){
    bool anyPlaying = false;
    for(int i = 0; i < 4;i++){
        if ( buttons[i].sound.getIsPlaying() ){
            anyPlaying = true;
        }
    }
    return anyPlaying;
}

void Simon::reset(){

    secuence.clear();
    countDown = ofRandom(ofGetFrameRate()*0.5,

                         ofGetFrameRate()*0.8);
    nextButton = 0;
    score = 0;
    turn = 0;
}

void Simon::objectAdded(ofxBlob &_blob){

    ofPoint pos;

    pos.x = _blob.centroid.x * width;
    pos.y = _blob.centroid.y * height;

    if (turn != 0){
        if ( !someonePlaying() ){

            for(int i = 0; i < 4;i++){
                if (buttons[i].inside(pos)){
                    buttons[i].sound.play();

                    if ( secuence[nextButton] == i ){
                        nextButton++;
                        if ( nextButton == secuence.size()){
                            score++;
                            turn = (turn+1)%nUsers;
                        }
                    } else {
                        for(int j = 0; j < 4;j++){
                            buttons[j].sound.play();
                        }

                        reset();
                        gameOverSound.play();
                        countDown = ofRandom(ofGetFrameRate()*0.5,ofGetFrameRate()*0.8);
                    }
                }
            }
        }
    }
}

void Simon::update(){

    //  If it´s computer turns
    //
    if (turn == 0){

        //  Proced with a timer in order to seems like a human
        //
        if (countDown > 0){
            countDown--;
        } else if (countDown == 0){

            //  If it´s at the end of the road (or the first turn of the game)
            //  choose a number, add it and start repeting what it got.
            //
            if ( nextButton == secuence.size() ){
                int newButtom = ((int)ofRandom(1034))%4;
                secuence.push_back(newButtom);
                countDown = ofRandom(ofGetFrameRate()*0.5,ofGetFrameRate()*0.8);
                nextButton = 0;
            } else if ( nextButton < secuence.size() ) {
                if ( !someonePlaying() ){
                    buttons[ secuence[nextButton] ].sound.play();

                    if ( nextButton == secuence.size()-1 ){
                        nextButton = 0;
                        turn = (turn+1)%nUsers;
                    } else {
                        nextButton++;
                    }

                    countDown = ofRandom(ofGetFrameRate()*0.5,ofGetFrameRate()*0.8);
                } else {
                    countDown = ofRandom(ofGetFrameRate()*0.5,ofGetFrameRate()*0.8);

                }


            }
        }
    }
}

void Simon::render(){
    fbo.begin();
    ofClear(0,255);
    ofPushStyle();
    ofSetColor(255,255);

    //  Draw 4 buttons
    //
    for(int i = 0; i < 4;i++){

        //  Calculate the alpha color from it´s position of the sound (0 start - 1 end)
        int alpha = 100;
        if ( buttons[i].sound.getIsPlaying() ){
            alpha = 100 + abs(sin(buttons[i].sound.getPosition() * PI ))*155;
        }

        //  Draw button shape
        ofFill();
        ofSetColor(buttons[i].color.r, buttons[i].color.g, buttons[i].color.b, alpha);
        ofBeginShape();
        for (int j = 0; j < buttons[i].getVertices().size(); j++){
            ofVertex( buttons[i].getVertices()[j] );
        }
        ofEndShape(true);
    }

    //  Draw the nice mask image
    //
    ofSetColor(255,255);
    mask.draw(space);

    //  Draw point counter
    //
    //      draw "88" on background for digit simulation
    ofSetColor(105,11,23,200);
    digitFont.drawString(ofToString(88, 2), space.x + space.width*0.5 , space.y+space.height*0.62);
    ofSetColor(243, 28, 36,240);
    //      estimate the score and draw it
    string scoreString = "0"+ ofToString(score);
    if (score > 9){
        scoreString = ofToString(score);
    }
    digitFont.drawString(scoreString, space.x + space.width*0.5 , space.y+space.height*0.62);

    //  Draw the LEDS that sign the turns
    //
    if (turn == 0){
        ofSetColor(243, 28, 36, 240);
        led.draw(space.x + space.width*0.44 , space.y+space.height*0.585, led.getWidth()*0.2, led.getHeight()*0.2);
    } else if (!someonePlaying()){
        ofSetColor(28, 243, 36, 240);
        led.draw(space.x + space.width*0.44 , space.y+space.height*0.605, led.getWidth()*0.2, led.getHeight()*0.2);
    }

    ofPopStyle();
    fbo.end();
}
