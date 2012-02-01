//#define AA_SMARTPOINTER_DEBUG
//#define AA_TABLE_DEBUG
//#define AA_SIGNAL_DEBUG

#include <AaImage>

using namespace std;
using namespace Aa;

int main (int argc, char ** argv)
{
#if 1
  cout << "s1 = NULL\n";
  SmartPointer<int> s1; // = NULL;

  cout << "s2 (23)\n";
  SmartPointer<int> s2 (23);

  cout << "s3 = SmartPointer (11)\n";
  SmartPointer<int> s3 = SmartPointer<int> (11);

  cout << "s1 = s1\n";
  s1 = s1;

  cout << "s2 = s1\n";
  s2 = s1;

  cout << "s3 = s1\n";
  s3 = s1;

  cout << "s2 = SmartPointer () = SmartPointer (78)\n";
  s2 = SmartPointer<int> () = SmartPointer<int> (78);

  cout << "s3 = s2\n";
  s3 = s2;

  cout << "s4 = s3\n";
  SmartPointer<int> s4 (s3);

  //cout << "s2 = NULL\n";
  //s2 = NULL;
#endif

#if 0
  cout << Index<1> (0) << endl;
  cout << Index<2> (1, 2) << endl;
  cout << Index<2> (Index<1> (1), 2) << endl;
  cout << Index<3> (Index<2> (3, 4), 5) << endl;
#endif

#if 1
  cout << "s1d = Signal<1, int> (4)\n";
  Signal<1, int> s1d (4);

  try
  {
    for (int i = 0; i < 10; ++i) s1d [i] = i;
  }
  catch (exception & e)
  {
    cerr << e.what () << endl;
  }
#endif

#if 1
  cout << "i2d = Im<2, RGB<int>> (4, 6)\n";
  Im<2, RGB<int> > i2d (vec (4u, 6u));

  try
  {
    for (int i = 0; i < 10; ++i) i2d (i, 0) = i;
  }
  catch (exception & e)
  {
    cerr << e.what () << endl;
  }

  //cout << "i2d.alloc (6, 4)\n";
  //i2d.alloc (vec (6u, 4u));

  try
  {
    for (int i = 0; i < 10; ++i) i2d (0, i) = i;
  }
  catch (exception & e)
  {
    cerr << e.what () << endl;
  }
#endif

  return 0;
}
