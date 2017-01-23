#ifndef QTIME_INCLUDED
#define QTIME_INCLUDED

#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <string>

inline double unix_time()
{
	return static_cast<double>(std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now()));
}

inline std::string date_time_str()
{
	time_t t = std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now());
	return std::string(ctime(&t));
}

class Timer
{
public:
	Timer(bool startTimer = false);

	/// status: Returns true if timer is running
	bool status() const;

	/// start: Begin tracking time; returns false if already started
	bool start();

	/// stop: Stop tracking time; returns false if already stopped
	bool stop(bool resetTimer = false);

	/// reset: Reset elapsed time to zero; returns false if already reset
	bool reset();

	/// Elapsed time, in their respective units
	double seconds() const;
	double minutes() const;
	double hours() const;
	double days() const;
private:
	bool m_status;
	std::chrono::time_point<std::chrono::system_clock> m_startpoint;
	double m_total;
};

Timer::Timer(bool startTimer)
: m_status(false), m_total(0.00)
{
	if (startTimer) start();
}

bool Timer::status() const
{
	return m_status;
}

bool Timer::start()
{
	if (m_status) return false;
	m_startpoint = std::chrono::system_clock::now();
	m_status = true;
	return true;
}

bool Timer::stop(bool resetTimer)
{
	if (resetTimer) reset();
	if (!m_status) return false;
	std::chrono::duration<double> d =
		std::chrono::system_clock::now() - m_startpoint;
	m_total += d.count();
	m_status = false;
	return true;
}

bool Timer::reset()
{
	if (!m_status && m_total == 0.00) return false;
	m_startpoint = std::chrono::system_clock::now();
	m_total = 0.00;
	return true;
}

double Timer::seconds() const
{
	if (m_status) {
		std::chrono::duration<double> d =
			std::chrono::system_clock::now() - m_startpoint;
		return m_total + d.count();
	} else return m_total;
}

double Timer::minutes() const
{
	return seconds() / 60.00;
}

double Timer::hours() const
{
	return seconds() / 3600.00;
}

double Timer::days() const
{
	return seconds() / 86400.00;
}

#endif // QTIME_INCLUDED
