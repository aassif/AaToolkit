#ifndef AA_TIMER__H
#define AA_TIMER__H

#ifndef _WIN32
  #include <sys/time.h>
#endif

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::Timer ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  class Timer
  {
   public:
    inline static double Time ();

   private:
    bool m_running;
    double m_t0;
    double m_dt;

   public:
    // Constructor.
    inline Timer (bool autorun);

    // Timer setting.
    inline void start  ();
    inline void pause  ();
    inline void resume ();
    inline void stop   ();

    inline bool isRunning () const;

    // Get current time.
    inline double get () const;
  };

////////////////////////////////////////////////////////////////////////////////

  double Timer::Time ()
  {
#ifdef _WIN32
    return 0.0;
#else
    struct timeval time;
    gettimeofday (&time, 0);
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
    m_t0 = Timer::Time ();
    m_dt = 0.0;
    m_running = true;
  }

  void Timer::pause ()
  {
    m_dt = (Timer::Time () - m_t0);
    m_running = false;
  }

  void Timer::resume ()
  {
    m_t0 = (Timer::Time () - m_dt);
    m_dt = 0.0; 
    m_running = true;
  }

  void Timer::stop ()
  {
    m_t0 = Timer::Time ();
    m_dt = 0.0;
    m_running = false;
  }

  bool Timer::isRunning () const
  {
    return m_running;
  }

  double Timer::get () const
  {
    //return (m_running ? m_dt = (Timer::Time () - m_t0) : m_dt);
    return (m_running ? Timer::Time () - m_t0 : m_dt);
  }

} // namespace Aa

#endif // AA_TIMER__H

