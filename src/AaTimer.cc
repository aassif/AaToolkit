#include "AaTimer"
#ifndef _WIN32
  #include <sys/time.h>
#endif

namespace Aa
{
  double Timer::getTime ()
  {
#ifdef _WIN32
    return 0.0;
#else
    struct timeval time;
    gettimeofday (&time, NULL);
    return (double) (time.tv_sec) + (double) (time.tv_usec) / 1000000.0;
#endif
  }

  Timer::Timer (bool autorun) :
    m_running (false),
    m_t0 (), m_dt (0.0)
  {
    if (autorun)
      this->start ();
  }

  void Timer::start ()
  {
    m_t0 = Timer::getTime ();
    m_dt = 0.0;
    m_running = true;
  }

  void Timer::pause ()
  {
    m_dt = (Timer::getTime () - m_t0);
    m_running = false;
  }

  void Timer::resume ()
  {
    m_t0 = (Timer::getTime () - m_dt);
    m_dt = 0.0; 
    m_running = true;
  }

  void Timer::stop ()
  {
    m_t0 = Timer::getTime ();
    m_dt = 0.0;
    m_running = false;
  }

  bool Timer::isRunning () const
  {
    return m_running;
  }

  double Timer::get () const
  {
    //return (m_running ? m_dt = (Timer::getTime () - m_t0) : m_dt);
    return (m_running ? Timer::getTime () - m_t0 : m_dt);
  }
} // namespace Aa

