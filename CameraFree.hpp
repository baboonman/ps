#ifndef FREECAMERA_HPP
# define FREECAMERA_HPP

# include "CameraControl.hpp"

# define PI_2_1		1.5533430342749533

class					CameraFree : public CameraControl
{
	public:
						CameraFree(t_vecf eyePos, float pitch, float yaw);
		virtual			~CameraFree() {};
		void			controlKey(int key, int action, int mods);
		void			controlMouse(double xPos, double yPos);

		t_vecf &		getEyePos();
		void			applyMatOnVec(float &x, float &y, float &z, float &w);

	private:
		Matrix			calculateMatrix( void );
		void			calcViewDir( void );
		t_vecf			mergeKeyPress( float sens );
		void			initKeyPress( void );

	protected:
		t_vecf			_eyePos;
		float			_pitch;
		float			_yaw;
		t_vecf			_keyPress[3];
		t_vecf			_viewDir;
		t_vecf			_viewNorm;
		double			_oldX;
		double			_oldY;
};

#endif
