#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Object.h"



class Circle : public RenderadbleObject
{
public:
	//Circle();
	void RenderObject();	
};

class Sphere : public RenderadbleObject
{
public:
	void RenderObject();

};

class Box : public RenderadbleObject
{
public:
	void RenderObject();

};

class S : public RenderadbleObject
{
public:
	void RenderObject();

};



#endif