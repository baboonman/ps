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
		void				setEyePos(t_vecf eyePos);
		float				getWidth() const;
		float				getHeight() const;
		void				setNewDim(float width, float height);
		void				setNewMult(float mult);	
		void				reset();

	public:
		bool				gravOn;
		bool				fixed;
		int					initShape;
		bool				launch;
		float				gravInverted;
		char				eq;
		float				posX;
		float				posY;
		CameraFree*			camera;
		bool				camOn;
		bool				hasResized;
		bool				hasReset;
		
	private:
		void				_processMouseCoord(double xPos, double yPos);
		void				_setHWmult();
		float				_width;
		float				_height;
		float				_mult;
		float				_wMult;
		float				_hMult;
		t_vecf				_startEyePos;
};

#endif
