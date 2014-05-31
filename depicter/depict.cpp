// Enpict Ver 2007-Dec-27
// by Joseph Larson (c) 2008
// Picture encoding tool
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
using namespace std;

class Picture {
  private:
  string code;
  int width, place, ch;
  char current;

  public:
  Picture ();
  Picture (int w);
  int eop ();
  int get_width ();
  int set_code (string c);
  int depict_next ();
};

Picture::Picture () {code = ""; width = place = ch = 0; current = 15;}

Picture::Picture (int w) {Picture (); width = w;}

int Picture::set_code (string c) {
  width = atoi (c.substr (c.length () - 3).c_str ());
  if (!width) return 0;
  code = c.substr(0, c.length () -3);
  current = code.at (0) - 33;
  return 1;
}

int Picture::depict_next () {
  if (!width || !code.compare("")) return 2;
  if (place == 6) {
    if (++ch >= code.length()) return 2;
    current = code.at(ch) - 33; place = 0;
  }
  if (current < 15) {current++; return 0;}
  if (current > 78) {current--; return 1;}
  return ((current - 15) & (1 << place++));
}

int Picture::eop () {if (ch >= code.length()) return 1; else return 0;}

int Picture::get_width () {return width;}

void usage (void) {
  cout << "Enpict\n------\n"
  << "Cymon's picture decoding tool. Either pass it a text file of a picture\n"
  << "code or type the picture code in one line at a time.\n\n"
  << "Command Line Perameters:\n"
  << "DEPICT [inputfile [outputfile]]\n\n"
  << "inputfile : A text file containing the picure code to be decoded.\n"
  << "outputfile : A file that will contain the decoded picture. If no file\n"
  << " is specified the output will default to DEPICT.TXT\n";
}

int main (int argc, char *argv[]) {
  ofstream output;
  int c;
  char cur;
  string filename, line, in, bunny;

  if ((argc > 1) && (!strcmp (argv[1], "/?"))) {usage (); exit (0);}
  if (argc > 2) { // Do we have an output file?
    filename = argv[2];
  } else {
    if (argc > 1) {
      filename = argv[1];
      int n = filename.find_last_of('\\');
      filename.erase(n + 1, filename.length() - n);
      filename.append("Depict.txt");
    } else filename = "Depict.txt";
  }
  output.open(filename.c_str ());
  if (!output.is_open ()) {
    cout << "Unable to open " << filename << " for writing.\n\n";
    usage (); exit (0);
  }
  Picture pict;
  if (argc > 1) { // Do we have an input file?
    ifstream input;
    input.open(argv[1]);
    if (!input.is_open ()) {
      cout << "Unable to open " << argv[1] << " for reading.\n\n";
      return 0;
    }
    in = "";
    while (!input.eof ()) {
      getline (input, line);
      in += line;
    }
    cout << "Code aquired from file.\n";
    input.close();
  } else {
    usage ();
    cout << "\nInput the picture code (Enter a blank line when done)\n";
    getline (cin, line);
    while (line.compare("")) {in += line; getline (cin, line);}
  }
  if (!pict.set_code (in)) {
    cout << "Invalid Code. Please check your code and try again.\n";
    return 1;
  }
  cout << "\nInput a word to use to draw the picture or press ENTER : ";
  getline (cin, bunny);
  if (!bunny.compare ("")) bunny = "XX";
  cout << "Decoded picture:\n"; c = 0;
  cur = (pict.depict_next ()) ? bunny.at(c % bunny.length()) : ' '; c++;
  while (!pict.eop ()) {
    cout << cur; output << cur;
    if ((c % pict.get_width ()) == 0) {cout << '\n'; output << '\n';}
    cur = (pict.depict_next ()) ? bunny.at(c % bunny.length()) : ' '; c++;
  }
  output.close ();
  cout << "\nPress ENTER to continue...";
  getline (cin, line);
}

