#include "colour.h"

Colour::Colour(){
	this->R = 1;
	this->G = 1;
	this->B = 1;
	this->A = 1;
}

Colour::Colour(float inR, float inG, float inB, float inA){
	this->R = inR;
    this->G = inG;
    this->B = inB;
    this->A = inA;
}

void Colour::setColour(float r, float g, float b, float a) {
    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
}

float* Colour::getColour()
{
    float* colour = new float[4];
    colour[0] = this->R;
    colour[1] = this->G;
    colour[2] = this->B;
    colour[3] = this->A;
    return colour;
}
    
bool Colour::isColour(Colour other){
    return (this->R == other.R) && (this->G == other.G) && (this->B == other.B) && (this->A == other.A);
}