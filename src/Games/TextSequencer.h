//
//  ofxTextSequencer.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef TEXTSEQUENCER
#define TEXTSEQUENCER

#include "ofMain.h"

#include "ofxTextShape.h"

#include "ofxTextArc.h"
#include "ofxTextBlock.h"
#include "ofxTextSpiral.h"

#include "ofxXmlSettings.h"

typedef struct {
    string                  text, fontFile;
    float                   seconds, speed, scale, fontSize, fontDpi;
    int                     spin;
    textShape               shape;
    horizontalAlignment     hAlign;
    verticalAlignment       vAlign;
} textPhrase;

class TextSequencer: public ofRectangle {
public:
    TextSequencer();
    
    bool    loadSequence(string _xmlFile);
    
    virtual void    setSpeed(float _speed){ speed = _speed;};
    virtual void    setSecForChar( float _secForChar){ secForChar = _secForChar; };
    virtual void    setSecBetweenPhrases( float _secBetweenPhrase){ secBetweenPhrase = _secBetweenPhrase;};
    virtual void    setNextPhrase(textPhrase &_phrase );
    
    virtual string  getText(){return rawText;};
    virtual float   getNormTransitionValue();
    
    virtual void    update();
    virtual void    draw();
    
protected:
    virtual string      spinningString(string _orginalText, int _nChars, int _offset);
    
    string              rawText;
    ofxTextShape        *text;
    
    textShape           defaultShape;
    string              defaultFontFile;
    float               defaultFontSize, defaultFontDpi, defaultSpeed;
    int                 defaultSpin;
    verticalAlignment   defaultVertAlign;
    horizontalAlignment defaultHoriAlign;
    
    float   secBetweenPhrase, secForChar;
    float   speed, seconds, countDown;
    int     spin;
    
    bool    bWaiting;
    
private:
    vector<textPhrase>  script;
    int                 currentLine;
};

#endif
