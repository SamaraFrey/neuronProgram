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


const double tau = 20;      //ms constant
const double h = 0.1;       //ms constant
const double R = 20;        //constant
const double treshold = 20; //mV potential when it spikes
const double refactime = 2; //ms time it takes to be able to take another spike
const double J = 0.2;       //mV potential given by background
const double D = 1.5;       //ms delay because of distance of neurons
const int bufferSize = D/h + 1; //size of buffer vector -> instead of calculating it each time

//specific values for in-/exhibitory neurons
const double Ji = 0.1;      //mv potential given by a inhibitory neuron
const double Je = 0.1;      //mV potential given by a exhibitory neuron

const int Ce = 10000;       //amount of exhib neurons
const int Ci = 2500;        //amount of inhib neurons

//variables for distributions
const double frequencyTh = treshold/(Ce*J*tau);
const double frequencyExt = 2 * frequencyTh;
const double lambda = frequencyExt * Ce * h * J; //constant to be used in possiondistribution

#endif /* constants_h */
