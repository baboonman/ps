#ifndef TIMER_HPP
# define TIMER_HPP

# include <sys/time.h>
# include <sstream>

# define ul_int			unsigned long int

class						Timer
{
	public:
							Timer();
							Timer(Timer const & rhs);
		virtual				~Timer();
		Timer&				operator=(Timer const & rhs);
							Timer(bool startRunning);
		void				start();
		void				stop();
		std::string			getTime();
		float				getFps();
		struct timeval		getStart() const;
		struct timeval		getEnd() const;

	private:
		struct timeval		_start;
		struct timeval		_end;
		ul_int				_time;

};


#endif
