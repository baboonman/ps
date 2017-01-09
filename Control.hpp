#ifndef CONTROL_HPP
# define CONTROL_HPP

# include <iostream>
# include "OpenGLInc.hpp"
# include "CameraFree.hpp"


class						Control
{
	public:
							Control();
							Control(Control const & rhs);
							~Control();
		Control&			operator=(Control const & rhs);
		void				processInput(int key, int action, int mods);
		void				processMouse(double xPos, double yPos);
		void				setDimension(int width, int height, float mult);
		void				setEyePos(t_vecf eyePos);
		float				getWidth() const;
		float				getHeight() const;
		void				setNewDim(float width, float height);
		void				setNewMult(float mult);	
		void				reset();
		void				setStartEyePos(t_vecf eyePos);

	public:
		bool				gravOn;
		bool				fixed;
		int					initShape;
		bool				launch;
		float				gravInverted;
		char				eq;
		float				posX;
		float				posY;
		float				posZ;
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
		float				_mouseX;
		float				_mouseY;
		t_vecf				_startEyePos;

		t_vecf				_tr;
		t_vecf				_tmpEyePos;
		
};

#endif
