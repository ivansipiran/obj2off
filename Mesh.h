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
#ifndef __MESH_H_
#define __MESH_H_

#include "Vertex.h"
#include "Face.h"
#include <vector>

using namespace std;

class Mesh{

      //Topological information
      Vertex* vertices;
      Face* faces;
	  
	  int numVertices;
      int numFaces;
	  
	  float** D;
      
      //BBox Information
      double xmin, xmax, ymin, ymax, zmin, zmax;
      double diag;


	  public:
	  		void cleanMesh();
      
             Mesh();
             Mesh(const char* nombreArchivo);
             ~Mesh();

             void loadFromOffFile(const char* filename);
			 void loadFromObjFile(const char* filename);
             
             friend ostream& operator<<(ostream& os, Mesh &obj);
			 Vertex* getVertices(){return vertices;}
			 Face* getFaces(){return faces;}
			 
			 int getNumberVertices(){return numVertices;}
			 int getNumFaces(){return numFaces;}
			 
			 double getDiagonal(){return diag;}
			 			 
};
#endif
