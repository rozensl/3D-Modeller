#ifndef COLOUR_H
#define COLOUR_H

class Colour {
public:
	Colour();
	Colour(float inR, float inG, float inB, float inA);

	float R;
	float G;
	float B;
	float A;

	void setColour(float inR, float inG, float inB, float inA);

	float* getColour();

	bool isColour(Colour other);
};

#endif