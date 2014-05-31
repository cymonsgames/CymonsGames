// Depicter Ver 2007-Dec-27
// by Joseph Larson (c) 2008
// Picture encoding tool
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
using namespace std;

#define BW (pict.depict_next ()) ? bunny.at(c % bunny.length()) : ' '

string code =
"~~~y,w*7.?\"?`u&10?\"Oht,?LPph`olQu-p`0qh`olQu.rH0ohQqn`s-r`0ohQqn`s.s`0ohQ_q"
"`s.rP3lh3Or`s/sP3lh3?r`s0s07hh7/r`s0r/?`h7Nn?lqPr/?`h7nn?lo2n?0O@l7n_?lo6n?PO"
".p`pn`p<h?0o.q`olQphP70q/q`o<&?(r/o&3(o*uPo&~~~y";
int width = 78;

class Picture {
  private:
  int place, ch;
  char current;

  public:
  Picture ();
  int eop ();
  int depict_next ();
};

Picture::Picture () {place = ch = 0; current = code.at(0) - 33;}

int Picture::depict_next () {
  if (place == 6) {
    if (++ch >= code.length()) return 2;
    current = code.at(ch) - 33; place = 0;
  }
  if (current < 15) {current++; return 0;}
  if (current > 78) {current--; return 1;}
  return ((current - 15) & (1 << place++));
}

int Picture::eop () {if (ch >= code.length()) return 1; else return 0;}

int main () {
  ofstream output;
  int c;
  char cur;
  string line, in, bunny;

  output.open("Depicter.txt");
  Picture pict;
  cout << "\nInput a word to use to draw the picture or press ENTER : ";
  getline (cin, bunny);
  if (!bunny.compare ("")) bunny = "XX";
  cout << "\n"; c = 0;
  cur = BW; c++;
  while (!pict.eop ()) {
    cout << cur; output << cur;
    if ((c % width) == 0) {cout << '\n'; output << '\n';}
    cur = BW; c++;
  }
  output.close ();
  cout << "\nPress ENTER to continue...";
  getline (cin, line);
}

