#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include "Mesh.h"
#include "Clock.h"

using namespace std;

string getOutputPath(string filename){
  string outputPath = filename;
  size_t pos;
  string directory = "";

  if((pos = filename.find_last_of('/')) != string::npos){
    directory = filename.substr(0, pos + 1);
    outputPath = filename.substr(pos + 1);
  }

  pos = outputPath.find_last_of('.');
  outputPath = outputPath.substr(0, pos);
  
  outputPath = directory + outputPath + ".off";

  return outputPath;
}

int main(int argc, char *argv[])
{
        
	string offFilename = argv[1];
	
	cout<<"File:"<<offFilename;
    
    string outfilename = getOutputPath(offFilename);
    
	Clock r;
	r.tick();
	Mesh mesh(argv[1]);
	r.tick();
	
	cout<<"->Loading time:"<<r.getTime()<<"s."<<endl;
	
	r.tick();
	ofstream out(outfilename.c_str());
	out << mesh;
	out.close();
	r.tick();
	
	cout << "Conversion time" << r.getTime() << "s." << endl;
    
	return EXIT_SUCCESS;
}
