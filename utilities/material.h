#ifndef MATERIAL_H
#define MATERIAL_H

// include dependencies
#include "colour.h"

class Material {
public:
	Colour ambient;
	Colour diffuse;
	Colour specular;
	float shininess;

	Material();
	Material(Colour ambient, Colour diffuse, Colour specular, float shininess);
	Material(char colour);
};

#endif