//#define AA_SMARTPOINTER_DEBUG
//#define AA_TABLE_DEBUG
//#define AA_SIGNAL_DEBUG

#include <AaCmdLine>
#include <AaMatrix>
#include <AaImage>

using namespace std;
using namespace Aa;

int main (int argc, char ** argv)
{
  CmdLine::Parser p ("main");
  p (argc, argv);

#if 1
  dvec3 v1 = vec (1.0, 2.0, 3.0);
  cout << "v1 = " << v1 << endl;

  dvec3 v2 = v1 + 3; //vec (4.0, 5.0, 6.0);
  cout << "v2 = v1 + 3 = " << v2 << endl;

  dvec3 v3 = 6 + v1; //vec (7.0, 8.0, 9.0);
  cout << "v3 = 6 + v2 = " << v3 << endl;

  dmat4 m0;
  cout << "m0 = " << m0 << endl;

  dmat3 m1 = mat (v1, v2, v3);
  cout << "m1 = " << m1 << endl;
  cout << "sizeof (m1) = " << sizeof (m1) << endl;

  cout << "m1 * v1 = " << (m1 * v1) << endl;
  cout << "m1 * v2 = " << (m1 * v2) << endl;
  cout << "m1 * v3 = " << (m1 * v3) << endl;

  dvec2 v5 = v1;
  cout << "v5 = " << v5 << endl;

  dvec2 v6 = vec (0.0, 1.0);
  cout << "v6 = " << v6 << endl;

  dmat3x2 m2 = mat (v1, v2);
  cout << "m2 = " << m2 << endl;
  cout << "sizeof (m2) = " << sizeof (m2) << endl;

  cout << "m2 * v5 = " << (m2 * v5) << endl;
  cout << "m2 * v6 = " << (m2 * v6) << endl;

  cout << "m1 * m1 = " << (m1 * m1) << endl;
  cout << "m1 * m2 = " << (m1 * m2) << endl;
  //cout << "m2 * m1 = " << (m2 * m1) << endl;

  mat3 m3 [3];
  cout << "sizeof (m3) = " << sizeof (m3) << endl;

  cout << "mat4 (m2) = " << mat4 (m2) << endl;

  float data [16] =
  {
     0,  1,  2,  3,
     4,  5,  6,  7,
     8,  9, 10, 11,
    12, 13, 14, 15
  };
  cout << "mat4 (data) = " << mat4 (data) << endl;
#endif

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
#endif

#if 1
  cout << "s1d = Signal<1, int> (4u)\n";
  Signal<1, int> s1d (4u);

  try
  {
    for (AaUInt i = 0; i < 10; ++i) s1d [i] = i;
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
    for (AaUInt i = 0; i < 10; ++i) i2d [vec (i, 0u)] = ivec3 (i);
  }
  catch (exception & e)
  {
    cerr << e.what () << endl;
  }

  try
  {
    for (AaUInt i = 0; i < 10; ++i) i2d [vec (0u, i)] = i;
  }
  catch (exception & e)
  {
    cerr << e.what () << endl;
  }
#endif

  return 0;
}

