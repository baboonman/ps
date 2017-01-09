#include "Timer.hpp"

Timer::Timer() {}

Timer::Timer(Timer const & rhs)
{
	*this = rhs;
}

Timer::~Timer() {}

Timer&			Timer::operator=(Timer const & rhs)
{
	this->_start = rhs.getStart();
	this->_end = rhs.getEnd();
	return *this;
}

Timer::Timer(bool startRunning)
{
	if (startRunning)
		this->start();
}

void			Timer::start()
{
	gettimeofday(&(this->_start), NULL);
}

void			Timer::stop()
{
	gettimeofday(&(this->_end), NULL);
	this->_time = (this->_end.tv_sec * 1000000 + this->_end.tv_usec)
					- (this->_start.tv_sec * 1000000 + this->_start.tv_usec);
}

std::string		Timer::getTime()
{
	std::stringstream	ss;
	int             m, s, ms, us;

	m = (this->_time / 60000000);
	s = (this->_time / 1000000) % 60;
	ms = (this->_time / 1000) % 1000;
	us = this->_time % 1000;
	ss << m << "m" << s << "s" << ms << "ms" << us << "us";
	return ss.str();
}

float			Timer::getFps()
{
	float		fTime = this->_time / 1000000.0f;
	return (1.f / fTime);
}

struct timeval	Timer::getStart() const
{
	return (this->_start);
}

struct timeval	Timer::getEnd() const
{
	return (this->_end);
}
