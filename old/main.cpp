#include <iostream>
#include "darformula.h"

using namespace std;
using namespace ElaroDARFormula;

int main()
{
  darformula testFormula;
  string x="x";
  string y="y";
  string z="z";
  string acceptedVars[3] = {x,y,z};
  try
  {
    testFormula.setFormula("11+x+y+z", acceptedVars, 3);
  }catch(runtime_error e)
  {
     cout<< &(e.what) << endl;
  }

  cout << "All done" << endl;
  return 0;
}
