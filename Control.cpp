#include "Control.hpp"

Control::Control() : gravOn(true), fixed(true), initShape(0), launch(false), gravInverted(1),
	eq(0), posX(0.f), posY(0.f), posZ(0.f), camOn(false), hasResized(false), hasReset(false)
{
	this->_startEyePos = {0.f, 0.f, 0.f};
	this->camera = new CameraFree(this->_startEyePos, 0.f, 0.f);
	this->_tr = {0.f, 0.f, 0.f};
	this->_tmpEyePos = this->_startEyePos;
	this->_mouseX = 0.f;
	this->_mouseY = 0.f;
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
	this->posZ = 0.f;
	this->_tr = {0.f, 0.f, 0.f};
	this->_tmpEyePos = this->_startEyePos;

	delete this->camera;
	this->camera = new CameraFree(this->_startEyePos, 0.f, 0.f);
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
		else if (key == 'R')
			this->reset();
		else if (key == 'X') {
			this->reset();
			this->eq = (this->eq + 1) % 2;
		}
		else if (key > '0' && key < '5') {
			this->reset();
			this->initShape = static_cast<int>(key) - '1';
		}
		else if (key == 'V') {
			if (!this->camOn) {
				this->_tmpEyePos = this->camera->getEyePos();
				this->camera->setMouseFirstPos(this->_mouseX, this->_mouseY);
				this->fixed = true;
			}
			else
				this->_tr = add(sub(this->camera->getEyePos(), this->_tmpEyePos), this->_tr);
			this->camOn = !this->camOn;
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

	this->_mouseX = xPos;
	this->_mouseY = yPos;

	this->posX = ((xPos - halfW) / halfW) * this->_wMult;
	this->posY = ((yPos - halfH) / halfH) * this->_hMult * -1.f;
	this->posZ = 0.f;

	this->posX += this->_tr.x;
	this->posY += this->_tr.y;
	this->posZ += this->_tr.z;

	this->camera->applyViewOnVec(this->posX, this->posY, this->posZ);
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

void					Control::setStartEyePos(t_vecf eyePos)
{
	this->_startEyePos = eyePos;
	delete this->camera;
	this->camera = new CameraFree(this->_startEyePos, 0.f, 0.f);
}
