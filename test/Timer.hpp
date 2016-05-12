#ifndef TIMER_HPP
# define TIMER_HPP

# include <sys/time.h>
# include <sstream>

# define ul_int			unsigned long int

class						Timer
{
	public:
							Timer();
							Timer(bool startRunning);
		void				start();
		void				stop();
		std::string			getTime();

	private:
		struct timeval		_start;
		struct timeval		_end;
		ul_int				_time;

};


#endif
