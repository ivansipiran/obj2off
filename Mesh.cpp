	/***************************************************************************
 *   Copyright (C) 2008 by    *
 *      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <map>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <cassert>
#include <cfloat>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include "Mesh.h"
#include "Clock.h"
#include "util.h"


using namespace std;
using namespace boost;

Mesh::Mesh(){
       vertices = NULL;
       faces = NULL;
       numVertices = 0;
       numFaces = 0;
}

Mesh::Mesh(const char* filename)
{
	   vertices = NULL;
	   faces = NULL;
	   numVertices = 0;
	   numFaces = 0;
	   
	   xmin = xmax = ymin = ymax = zmin = zmax = 0.0;

	   loadFromObjFile(filename);
       
       //Finding bounding box...
       for(register int i = 0; i < getNumberVertices(); i++){
       		if(vertices[i].getX() < xmin)
       			xmin = vertices[i].getX();
       		else if(vertices[i].getX() > xmax)
       			xmax = vertices[i].getX();
       		
       		if(vertices[i].getY() < ymin)
       			ymin = vertices[i].getY();
       		else if(vertices[i].getY() > ymax)
       			ymax = vertices[i].getY();
       		
       		if(vertices[i].getZ() < zmin)
       			zmin = vertices[i].getZ();
       		else if(vertices[i].getZ() > zmax)
       			zmax = vertices[i].getZ();
       }
       
       diag = sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin) + (zmax - zmin)*(zmax - zmin));
}

Mesh::~Mesh(){
    	cleanMesh();
}


//Clean up the object
void Mesh::cleanMesh(){
	if(faces){
	           delete[] faces;
			   faces = NULL;
	     }
	if(vertices){
	           delete[] vertices;
			   vertices = NULL;
	     }
}


void Mesh::loadFromOffFile(const char* filename){

	int numEdges;
		
	ifstream in(filename);
	
	string format;
	getline(in, format);
	assert(format.find("OFF") != string::npos);
	
	in>>numVertices>>numFaces>>numEdges;
	skipline(in);
	
	vertices = new Vertex[numVertices];
	faces = new Face[numFaces];
	
	
	for(register int i = 0; i < numVertices; i++){
		double x, y, z;
		in>>x>>y>>z;
		skipline(in);
		
		vertices[i].setX(x);	vertices[i].setY(y);	vertices[i].setZ(z);
		vertices[i].setIndex(i);
	}
	
	
	for(register int i = 0;  i < numFaces; i++){
		int numVert;
		in >> numVert;
		
		assert(numVert == 3);
		
		int p1, p2, p3;
			
		in>>p1>>p2>>p3;
		skipline(in);
		
		faces[i].addVertex(p1);				faces[i].addVertex(p2);				faces[i].addVertex(p3);
		
		vertices[p1].addFace(i);			vertices[p2].addFace(i);			vertices[p3].addFace(i);
		//vertices[p1].setNumFace(i);			vertices[p2].setNumFace(i);			vertices[p3].setNumFace(i);
		//vertices[p1].setPosBarycentric(0);	vertices[p2].setPosBarycentric(1);	vertices[p3].setPosBarycentric(2);
		
		vertices[p1].addVertex(p2);	vertices[p1].addVertex(p3);
		vertices[p2].addVertex(p1);	vertices[p2].addVertex(p3);
		vertices[p3].addVertex(p1);	vertices[p3].addVertex(p2);
	}
	
	in.close();
	
}

/*void Mesh::loadFromObjFile(const char* filename){

	ifstream in(filename);
	
	string str;
	
	vector<Vertex> vert;
	vector<Face> fac;
	
	while(!in.eof()){
		getline(in, str);
		if(str.size()==0 || str[0]=='#')
			continue;
		size_t found = str.find(' ');
		string command = str.substr(0, found);
		str = str.substr(found);
		found = str.find_first_not_of(' ');
		str = str.substr(found);
		
		
		if(command.compare("vt")==0 || command.compare("vn")==0)
			continue;
		if(command.compare("v")==0){ //Read vertex
			Vertex v;
			string val;
			
			found = str.find(' ');
			val = str.substr(0, found + 1);
			v.setX(atof(val.c_str()));
			
			str = str.substr(found + 1);
			found = str.find_first_not_of(' ');
			str = str.substr(found);
			found = str.find(' ');
			val = str.substr(0, found + 1);
			v.setY(atof(val.c_str()));
			
			str = str.substr(found + 1);
			v.setZ(atof(str.c_str()));
			vert.push_back(v);
		}
		
		if(command.compare("f")==0){ //Read face
			Face f;
			string val;
			
			//str = str.substr(0, found + 1);
			found = str.find_first_of(" /");
			val = str.substr(0, found + 1);
			f.addVertex(atoi(val.c_str())-1);
			
			if(str[found]=='/')
				found = str.find(' ');
			
			str = str.substr(found + 1);
			found = str.find_first_not_of(' ');
			str = str.substr(found);
			found = str.find_first_of(" /");
			val = str.substr(0, found + 1);
			f.addVertex(atoi(val.c_str())-1);
			
			if(str[found]=='/')
				found = str.find(' ');
			
			str = str.substr(found + 1);
			found = str.find_first_not_of(' ');
			str = str.substr(found);
			
			found = str.find_first_of(" /");
			
			if(found==string::npos)
				val = str;
			else
				val = str.substr(0, found + 1);
			f.addVertex(atoi(val.c_str())-1);
			
			fac.push_back(f);
		}
	}
	
	numVertices = (int)vert.size();
	numFaces = (int)fac.size();
	
	vertices = new Vertex[numVertices];
	faces = new Face[numFaces];
	
	
	for(int i = 0; i < vert.size(); i++){
		vertices[i].setX(vert[i].x());
		vertices[i].setY(vert[i].y());
		vertices[i].setZ(vert[i].z());
		
		vertices[i].setIndex(i);
	}
	
	for(int i = 0; i < fac.size(); i++){
		int p1,p2,p3;
		p1 = fac[i].getVertex(0);
		p2 = fac[i].getVertex(1);
		p3 = fac[i].getVertex(2);
		
		faces[i].addVertex(p1);		faces[i].addVertex(p2);		faces[i].addVertex(p3);
		
		vertices[p1].addFace(i);	vertices[p2].addFace(i);	vertices[p3].addFace(i);
		vertices[p1].addVertex(p2);	vertices[p1].addVertex(p3);
		vertices[p2].addVertex(p1);	vertices[p2].addVertex(p3);
		vertices[p3].addVertex(p1);	vertices[p3].addVertex(p2);
	}
	
	in.close();
}*/

void Mesh::loadFromObjFile(const char* filename){
	ifstream in(filename);
	
	string str;
	
	vector<Vertex> vert;
	vector<Face> fac;
	
	int lineNumber = 1;
	while(!in.eof()){
		getline(in, str);
		//cout << lineNumber << ": " << str << endl;
		if(str.size()==0 || str[0]=='#')
			continue;
		
		//Extraer los tokens y guardarlos en un vector
		vector<string> tok;
		char_separator<char> sep(" ");
		tokenizer< char_separator<char> > tokens(str, sep);
		BOOST_FOREACH (const string& t, tokens){
			tok.push_back(t);
		}
		
		if(tok[tok.size()-1].find('\\')!=string::npos){
			string str1;
			lineNumber++;
			getline(in, str1);
			tokenizer< char_separator<char> > tokens3(str1, sep);
			BOOST_FOREACH (const string& t, tokens3){
				tok.push_back(t);
			}
		}
		
		if(tok[0].compare("v")==0){
			//if(tok.size()==4){
				Vertex v;
				v.setX(atof(tok[1].c_str()));
				v.setY(atof(tok[2].c_str()));
				v.setZ(atof(tok[3].c_str()));
				vert.push_back(v);
			//}else{
			//	cout << "Error while reading vertex in line:" << lineNumber << endl;
			//	exit(EXIT_FAILURE);
			//}
		}else if(tok[0].compare("f") == 0){
			//cout << tok.size() << endl;
			int numVertex = tok.size() - 1;
			if(numVertex < 3){
				cout << "Error while reading face on line:" << lineNumber << endl;
				exit(EXIT_FAILURE);
			}
			vector<int> indices;
			char_separator<char> sep2("/");
			for(int i = 1; i < tok.size(); i++){
				tokenizer< char_separator<char> > tokens2(tok[i], sep2);
				tokenizer< char_separator<char> >::iterator beg = tokens2.begin();
				int index = atoi(beg->c_str());
				//cout << index << " ";
				vector<int>::iterator it = find(indices.begin(), indices.end(), index);
				if(it == indices.end())
					indices.push_back(index);
				else{
					cout << "Repeating vertices in face list on line:" << lineNumber << "(" << str << ")"<<endl;
					//exit(EXIT_FAILURE);
				}
			}
			//cout << endl;
			
			//int ant = 1;
			//for(int i = 2; i < indices.size(); i++){
				//Face f;
				//f.addVertex(indices[0]-1);
				//f.addVertex(indices[ant]-1);
				//f.addVertex(indices[i]-1);
				//fac.push_back(f);
			//	ant = i;
			//}
			Face f;
			f.addVertex(indices[0]-1);
			f.addVertex(indices[1]-1);
			f.addVertex(indices[2]-1);
			fac.push_back(f);
		}
		lineNumber++;
	}
	
	//cout << "File loaded" << endl;
	numVertices = (int)vert.size();
	numFaces = (int)fac.size();
	
	vertices = new Vertex[numVertices];
	faces = new Face[numFaces];
	
	//cout << "Created structures" << endl;
	
	for(int i = 0; i < vert.size(); i++){
		vertices[i].setX(vert[i].x());
		vertices[i].setY(vert[i].y());
		vertices[i].setZ(vert[i].z());
		
		vertices[i].setIndex(i);
	}
	
	//cout << "Vertex loaded" << endl;
	
	for(int i = 0; i < fac.size(); i++){
		int p1,p2,p3;
		p1 = fac[i].getVertex(0);
		p2 = fac[i].getVertex(1);
		p3 = fac[i].getVertex(2);
		
		faces[i].addVertex(p1);		faces[i].addVertex(p2);		faces[i].addVertex(p3);
		
	//	vertices[p1].addFace(i);	vertices[p2].addFace(i);	vertices[p3].addFace(i);
	//	vertices[p1].addVertex(p2);	vertices[p1].addVertex(p3);
	//	vertices[p2].addVertex(p1);	vertices[p2].addVertex(p3);
	//	vertices[p3].addVertex(p1);	vertices[p3].addVertex(p2);
	}
	
	//cout << "Faces loaded" << endl;
	
	in.close();
	
}

ostream& operator<<(ostream& os, Mesh& obj){
         os<<"OFF"<<endl;
         os<<obj.getNumberVertices()<<" "<<obj.getNumFaces()<<" "<<0<<endl;
         for(int i = 0; i<obj.getNumberVertices(); i++){
             os<<obj.vertices[i].getX()<<" "<<obj.vertices[i].getY()<<" "<<obj.vertices[i].getZ()<<endl;
         }
		 for(int i = 0; i < obj.getNumFaces(); i++){
				os<<3<<" ";
				vector<int> vert = obj.faces[i].getVertices();
				for(int j = 0; j < (vert.size() - 1); j++){
					os<<vert[j]<<" ";
				}
				os<<vert[vert.size()-1]<<endl;
		 }
		 return os;
 }


