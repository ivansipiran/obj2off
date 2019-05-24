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
#ifndef __CLOCK_H_
#define __CLOCK_H_
#include <iostream>
using namespace std;

//namespace Base{

class Clock{
      private:
              clock_t current;
              clock_t previous;    
              
      public:
             Clock(){previous = current = clock();}
             virtual ~Clock(){}
             
             void tick(){
                  previous = current;     
                  current = clock();
             }
             
             double getTime(){
                    return (((double)(current - previous))/CLOCKS_PER_SEC);
             }
};
//}
#endif
