#include "Control.hpp"

Control::Control() : gravOn(true), fixed(true), initShape(0), launch(false), gravInverted(1),
	eq(0), posX(0.f), posY(0.f), camOn(false), hasResized(false), hasReset(false)
{
	this->camera = new CameraFree({0.f, 0.f, 0.f}, 0.f, 0.f);
}

Control::~Control()
{
	delete (this->camera);
}

void					Control::reset()
{
	this->gravOn = true;
	this->fixed = true;
	this->launch = false;
	this->gravInverted = 1;
	this->camOn = false;
	this->hasResized = false;
	this->hasReset = true;
	this->posX = 0.f;
	this->posY = 0.f;

	delete this->camera;
	this->camera = new CameraFree({0.f, 0.f, 0.f}, 0.f, 0.f);
}

void					Control::setDimension(int width, int height, float mult)
{
	this->_mult = mult;
	this->_width = width;
	this->_height = height;
	this->_setHWmult();
}

void					Control::processInput(int key, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == 'G') {
			this->gravOn = !this->gravOn;
			if (this->gravOn)
				this->gravInverted = 1;
			else
				this->gravInverted = 0;
		}
		else if (key == 'F')
			this->fixed = !this->fixed;
		else if (key == 32)
			this->launch = !this->launch;
		else if (key == 'I')
			this->gravInverted = this->gravInverted * -1;
		else if (key == 'V')
			this->camOn = !this->camOn;
		else if (key == 'R')
			this->reset();
		else if (key == 'E') {
			this->reset();
			this->eq = (this->eq + 1) % 2;
		}
		else if (key > '0' && key < '5') {
			this->reset();
			this->initShape = static_cast<int>(key) - '1';
		}
	}
	if (camOn)
	{
		this->camera->controlKey(key, action, mods);
	}
}

void					Control::processMouse(double xPos, double yPos)
{
	if (!camOn)
		this->_processMouseCoord(xPos, yPos);
	else
		this->camera->controlMouse(xPos, yPos);
}

void					Control::_processMouseCoord(double xPos, double yPos)
{
	float				halfW = this->_width / 2;
	float				halfH = this->_height / 2;

	this->posX = ((xPos - halfW) / halfW) * this->_wMult;
	this->posY = ((yPos - halfH) / halfH) * this->_hMult * -1.f;
}

void					Control::_setHWmult()
{
	this->_wMult = this->_mult * (this->_width / this->_height);
	this->_hMult = this->_mult;
}

float					Control::getWidth() const
{
	return (this->_width);
}

float					Control::getHeight() const
{
	return (this->_height);
}

void					Control::setNewDim(float width, float height)
{
	this->_width = width;
	this->_height = height;
	this->_setHWmult();
}

void					Control::setNewMult(float mult)
{
	this->_mult = mult;
	this->_setHWmult();
}
