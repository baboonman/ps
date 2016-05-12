#ifndef CONTROL_HPP
# define CONTROL_HPP

# include <iostream>
# include "OpenGLInc.hpp"


class						Control
{
	public:
							Control();
							~Control();
		void				processInput(int key, int action);
		void				processMouse(double xPos, double yPos);
		void				setDimension(int width, int height, float mult);

	public:
		bool				gravOn;
		bool				fixed;
		bool				initCube;
		bool				launch;
		float				gravInverted;
		float				posX;
		float				posY;
		
	private:
		float				_width;
		float				_height;
		float				_wMult;
		float				_hMult;
};

#endif
