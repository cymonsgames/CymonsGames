// Enpict ver 2007-Dec-28
// by Joseph Larson with file processing by Paul Kitchen (c) 2008
// joealarson@gmail.com Paul_Kitchin@yahoo.com
// Picture encoding tool
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

class Picture {
  private:
  string code;
  int width, place;
  char current;

  public:
  Picture ();
  Picture (int w);
  void set_width (int w);
  int enpict (char block);
  string get_code ();
  int get_width ();
};

Picture::Picture () {code = ""; width = place = 0; current = 15;}

Picture::Picture (int w) {Picture (); set_width (w);}

void Picture::set_width (int w) {width = w;}

int Picture::enpict (char block) {
  if (!width) return 1;
  if (place == 6) {                        // process runs or start a new line.
    if (current <= 15 && current > 0 && block == ' ') current--;
    else if (current >= 78 && current < 93 && block != ' ') current++;
    else {current += 33; code += current; place = 0; current = 15;}
  }
  if (place < 6) current += (block != ' ') << place++;       // Encode non-run.
  return 0;
}

string Picture::get_code () {
//  ostringstream oss;

//  if (width < 100) oss << '0';
//  if (width < 10) oss << '0';
//  oss << width;
  return code + (char)(current + 33);// + oss.str();
}

int Picture::get_width () {return width;}

void usage (void) {
  cout << "Enpict\n------\n"
  << "Cymon's picture encoding tool. Either pass it a text file of a picture\n"
  << "or type the picture in one line at a time.\n\n"
  << "Command Line Perameters:\n"
  << "ENPICT [inputfile [outputfile]]\n\n"
  << "inputfile : A text file containing the picure to be encoded. (All\n"
  << " non-whitespace characters treated the same.)\n"
  << "outputfile : A file that will contain the encoded and compressed\n"
  << " picture data. If no file is specified the output will default to\n"
  << " ENPICT.TXT\n";
}

int main (int argc, char *argv[]) {
  ofstream output;
  char in;
  int width, c, d;
  string filename, line;

  if ((argc > 1) && (!strcmp (argv[1], "/?"))) {usage (); exit (0);}
  if (argc > 2) { // Do we have an output file?
    filename = argv[2];
  } else {
    if (argc > 1) {
      filename = argv[1];
      int n = filename.find_last_of('\\');
      filename.erase(n + 1, filename.length() - n);
      filename.append("Enpict.txt");
    } else filename = "Enpict.txt";
  }
  output.open(filename.c_str ());
  if (!output.is_open ()) {
    cout << "Unable to open " << filename << " for writing.\n\n";
    usage (); exit (0);
  }
  Picture pict;
  if (argc > 1) { // Do we have an input file?
    ifstream input (argv[1]);
    if (!input.is_open ()) {
      cout << "Unable to open " << argv[1] << " for reading.\n\n";
      return 0;
    }
    streamsize longest = 0;
    while (input.ignore(numeric_limits< streamsize >::max(), '\n'))
      longest = max(longest, input.gcount());
    pict.set_width (--longest);
    input.clear();
    input.seekg (0, ios::beg);
    while (getline(input, line)) {
      for (string::const_iterator d = line.begin(), end = line.end();
        d != end; ++d) pict.enpict(*d);
      for (int d = line.size(); d != longest; ++d)
        pict.enpict (' ');
    }
    input.close ();
  } else {
    usage ();
    cout << "\nInput the width of your picture : ";
    cin >> width;
    pict.set_width (width);
    cout << "\nStart inputing the picture. All non-space characters will be\n"
      << "treated the same. Input a blank line to quit\n";
    cout << ":"; for (c = 0; c < width; c++) cout << '-';
    cout << ":\n:";
    cin >> ws;
    c = 0; in = cin.get ();
    while (in != '\n') {
      c++; pict.enpict (in);
      in = cin.get ();
      if (c == width) while (in != '\n') in = cin.get ();
      if (in == '\n') {
        while (c++ < width) pict.enpict (' ');
         cout << ':';
         c = 0; in = cin.get ();
      }
    }
  }
  cout << "Encoded picture data for Depicter.c:\n\nstring code =\n\"";
  output << "string code =\n\"";
  string temp = pict.get_code();
  d = 0;
  for (c = 0; c < temp.length ();c++) {
    if ((temp.at(c) == '"') || (temp.at(c) == '\\'))
      {cout << '\\'; output << '\\'; d++;}
    cout << temp.at(c); output << temp.at(c);
    if (!(++d % 77)) {cout << "\"\n\""; output << "\"\n\"";}
  }
  cout   << "\";\nint width = " << pict.get_width() << ";\n";
  output << "\";\nint width = " << pict.get_width() << ";\n";
  output.close ();
  cout << "\nPress ENTER to continue...";
  getline (cin, line);
}
