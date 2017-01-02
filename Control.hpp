#ifndef CONTROL_HPP
# define CONTROL_HPP

# include <iostream>
# include "OpenGLInc.hpp"
# include "CameraFree.hpp"


class						Control
{
	public:
							Control();
							~Control();
		void				processInput(int key, int action, int mods);
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
		CameraFree*			camera;
		bool				camOn;
		bool				hasResized;
		float				width;
		float				height;

		
	private:
		void				_processMouseCoord(double xPos, double yPos);
		float				_width;
		float				_height;
		float				_wMult;
		float				_hMult;
};

#endif
