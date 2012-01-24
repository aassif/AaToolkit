#ifndef __AA_TIMER
#define __AA_TIMER__

#include <iostream>

namespace Aa
{
  class AA_TOOLKIT_API Timer
  {
   public:
    static double getTime ();

   private:
    bool m_running;
    double m_t0;
    double m_dt;

   public:
    // Constructor.
    Timer (bool autorun);

    // Timer setting.
    void start  ();
    void pause  ();
    void resume ();
    void stop   ();

    bool isRunning () const;

    // Get current time.
    double get () const;
  };
} // namespace Aa

#endif // __AA_TIMER__

