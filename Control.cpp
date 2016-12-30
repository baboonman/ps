#include "Control.hpp"

Control::Control() : gravOn(true), fixed(true), initCube(true), launch(false), gravInverted(1), camOn(false)
{
	this->camera = new CameraFree({0.f, 0.f, 0.f}, 0.f, 0.f);
}

Control::~Control()
{
	delete (this->camera);
}

void					Control::processInput(int key, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == 'G')
			this->gravOn = !this->gravOn;
		if (key == 'F')
			this->fixed = !this->fixed;
		if (key == 'C')
			this->initCube = !this->initCube;
		if (key == 32)
			this->launch = !this->launch;
		if (key == 'I')
			this->gravInverted = this->gravInverted * -1;
		if (key == 'V') {
			this->camOn = !this->camOn;
		}
	}
	if (camOn)
	{
		this->camera->controlKey(key, action, mods);
	}
}

void					Control::_processMouseCoord(double xPos, double yPos)
{
	float				halfW = this->_width / 2;
	float				halfH = this->_height / 2;

	this->posX = ((xPos - halfW) / halfW) * this->_wMult;
	this->posY = ((yPos - halfH) / halfH) * this->_hMult * -1.f;
}

void					Control::processMouse(double xPos, double yPos)
{
	if (!camOn)
		this->_processMouseCoord(xPos, yPos);
	else
		this->camera->controlMouse(xPos, yPos);
}

void					Control::setDimension(int width, int height, float mult)
{
	float				wMult, hMult;

	wMult = mult;
	hMult = mult * height / width;
	this->_width = width;
	this->_height = height;
	this->_wMult = wMult;
	this->_hMult = hMult;
}
