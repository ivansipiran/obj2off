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

#include "Vertex.h"
#include "util.h"
#include <iostream>
#include <cmath>
#include <map>

void Vertex::getNeighborhood(int rad, vector<Vertex*>& V, Vertex* vertices){
	queue<Vertex*> Q;
	vector<Vertex*> marked;
	
	Q.push(this);
	this->setMark(true);
	this->setDepth(0);
	marked.push_back(this);
	
	while(!Q.empty()){
		Vertex* v0 = Q.front();
		Q.pop();
		V.push_back(new Vertex(v0->getX(), v0->getY(), v0->getZ())); //Indeed, copy vertex information rather than return the same vertex
		
		int dep = v0->getDepth();
		if(dep <= rad){
			set<int> listVertices = v0->getAdjacentVertices();
			set<int> :: iterator it;
			for(it = listVertices.begin(); it!=listVertices.end(); it++){
				Vertex* v1 = &vertices[*it];
					if(!v1->isMarked()){
						Q.push(v1);
						v1->setMark(true);
						v1->setDepth(dep + 1);
						marked.push_back(v1);
					}
				}
			}
	}
	
	vector<Vertex*>::iterator ini = marked.begin();
	
	while(ini<marked.end()){
		(*ini)->setMark(false);
		(*ini)->setDepth(0);
		ini++;
	}
}

int Vertex :: getRadius(Vertex*  vertices, double radius, vector<Vertex*>& V){
	vector<Vertex*> marked; //Almacena los vertices marcados durante el proceso
	map<int, double> distances; //Almacena las distancias relativas al vertice actual
	map<int, Vertex*> markedRing; //Almacena los elementos de un nuevo anillo, el cual al terminar de analizarse, deben ser marcados
	queue<Vertex*> Q; //Cola para recorrer el grafo de la topologia de la malla
	double maxDistance = 0.0; //Maxima distancia encontrada hasta recorrer un determinado anillo
	int rad = -1; //El radio al que se encuentra la maxima distancia
	
	Q.push(this);
	this->setMark(true);
	//this->setDepth(0);
	//marked.push_back(this);
	markedRing.insert(pair<int, Vertex*>(this->index, this));
		
	distances[this->index] = 0.0;
	
	while(!Q.empty()){
		Vertex* v0 = Q.front();
		Q.pop();
		
		int dep = v0->getDepth();
		if(dep != rad){ //Se llego al primer vertice del siguiente anillo, anillo anterior debe ser marcado y rad aumenta en 1
			map<int, Vertex*>::iterator it;
			double max = 0.0;
			
			//Se marca todo el anillo analizado anteriormente y se busca la maxima distancia a algun punto de ese anillo
			for(it = markedRing.begin(); it!=markedRing.end(); it++){
				Vertex* mar = (*it).second;
				mar->setMark(true);
				marked.push_back(mar);
				V.push_back(new Vertex(mar->getX(), mar->getY(), mar->getZ()));
				if(distances[(*it).first] > max)
					max = distances[(*it).first];
			}
			
			rad++;
			markedRing.clear(); //Limpiamos los vertices del anillo que termino de analizarse
			maxDistance = max;
			if(maxDistance > radius)
				break;
		}
		
		set<int> listVertices = v0->getAdjacentVertices();
		set<int> :: iterator it;
		
		//vector<int> vertexFaces = v0->getFaces();
		//for(register int i = 0; i < vertexFaces.size(); i++){
		//	vector<int> faceVertices = faces[vertexFaces[i]].getVertices();
		//	for(register int j = 0; j < faceVertices.size(); j++){
			for(it = listVertices.begin(); it!=listVertices.end(); it++){
				//Vertice* v1 = &vertices[faceVertices[j]];
				Vertex* v1 = &vertices[*it];
				if(!v1->isMarked()){ //Si este punto vecino no esta marcado
					if(distances[v1->getIndex()] == 0.0){ //Solo si su distancia aun no ha sido seteada, se coloca en la cola
						Q.push(v1);   	//Va a la cola de espera
						v1->setDepth(dep + 1);
					}
					markedRing.insert(pair<int, Vertex*>(v1->getIndex(), v1));
					double dist = distanceL2(v0, v1);
					double newDistance = distances[v0->getIndex()] + dist;
					if(distances[v1->getIndex()] == 0.0){ //Es la primera vez que se setea la distancia hacia este punto
						distances[v1->getIndex()] = newDistance;
					}else if(newDistance  < distances[v1->getIndex()]){
						distances[v1->getIndex()] = newDistance;
					}
				}
			}
		//}
	}
	
	if(!markedRing.empty()){
			map<int, Vertex*>::iterator it;
			double max = 0.0;
			
		//Se marca todo el anillo analizado anteriormente y se busca la maxima distancia a algun punto de ese anillo
			for(it = markedRing.begin(); it!=markedRing.end(); it++){
				Vertex* mar = (*it).second;
				mar->setMark(true);
				marked.push_back(mar);
				V.push_back(new Vertex(mar->getX(), mar->getY(), mar->getZ()));
				if(distances[(*it).first] > max)
					max = distances[(*it).first];
			}
			
			rad++;
			markedRing.clear(); //Limpiamos los vertices del anillo que termino de analizarse
			maxDistance = max;
			
	}
	
	//Desmarcar los vertices empleados para analizar este vertice
	vector<Vertex*>::iterator ini = marked.begin();
	while(ini  < marked.end()){
		(*ini)->setMark(false);
		(*ini)->setDepth(0);
		ini++;
	}
	return rad;
}


void Vertex::processMaximum(Vertex* vertices, int numRings){
	//for(register int i = 0; i< faces.size(); i++){
	//	vector<int> faceVertices = caras[faces[i]].getVertices();
	//	for(register int j = 0; j < faceVertices.size(); j++){
		set<int> :: iterator it;
		for(it = adjacentVertices.begin(); it!=adjacentVertices.end(); it++){
		//	Vertice* v1 = &vertices[faceVertices[j]];
			Vertex* v1 = &vertices[*it];
			if(v1!=this){
				if(response < v1->getResponse())
					return;
			}
		}
	//}
	
	isInterest = true;
}

ostream& operator<<(ostream& out, Vertex& point){
	out << "(" << point.x() <<" , "<<point.y()<<" , "<<point.z()<<") -> (" <<
		point.mark << " , "<<point.isInterest<<")";
	
	return out;
}
