//
//  constants.h
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//

#ifndef constants_h
#define constants_h

//this file contains all the constant we use for the equations

//my time measurment will be in ms
//I will use mV as the unit of the potentials


const double tau = 20; //ms constant
const double h = 0.1; //ms constant
const double R = 20; //constant
const double treshold = 20; //mV potential when it spikes
const double refactime = 2; //ms time it takes to be able to take another spike
const double J = 0.1; //mv potential given by a neuron to another one
const double D = 1.5; //ms delay because of distance of neurons

#endif /* constants_h */

/*

 include cassert
 assert(condition) -> tells you if it fails, where it fails
 
*/