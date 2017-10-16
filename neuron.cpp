//
//  neuron.cpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//


//this is the main class for neurons, afterwards we will have subclasses which inherit from it
//-> inhibitory and excitatory
//with certain amount of connections

//has own local clock

//change inputconnec
//print overwrite operator

#include "neuron.hpp"
#include "constants.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//Constructors
Neuron::Neuron(double p, int sp, double t){
    memPot = p;
    nbrSp = sp;
    timeSp = t;
    
    connections.push_back(nullptr); //for debugging purposes
    
    int bufferSize = D/h + 1;
    vector <unsigned int> buffer (bufferSize);
}

Neuron::Neuron(){} //default

//operators
void Neuron::operator=(Neuron* other){
    memPot = other->getMemPot();
    nbrSp  = other->getNbrSp();
    timeSp = other->getTimeSp();
    clock  = other->getClock();
}

//functions
void Neuron::putInVector(double time){
    spikeVect.push_back(time);
}

bool Neuron::update(int time, double extCurr){
    //count the received spikes
    int spikeNbr = buffer[time];
    bool spiking = false;
    
    if(spiked()){
        //increase the nbr of spikes the neuron itself has
        int val = getNbrSp() + 1;
        setNbrSp(val);
        spiking = true;
    }
    
    //bc neuron is insensitiv for a refacttime
    if(clock - time < refactime){
        setMemPot(0); //can use 0 or 10mV
        return true;
    } else {
        double newMemPot = memPot * exp(-h/tau) + extCurr * (R) * (1-exp(-h/tau))+J*spikeNbr;
        
        //break if negative
        assert(getMemPot() < 0);
        
        //set it as new potential
        setMemPot(newMemPot);

    }
    
    clock = clock+1;
    return spiking;
}

bool Neuron::spiked(){
    if(getMemPot() >= treshold){
        return true;
    }
    else {
        return false;
    }
}

void Neuron::addConnect(Neuron other){
    cout << "I'm connecting the neurons" << endl;
    Neuron* pointerNeuron = &other;
    connections.push_back(pointerNeuron);
}

Neuron Neuron::getConnectNeuron(int i){
    Neuron neuron = *connections[i];
    return neuron;
}

void Neuron::receive(int time){
    buffer[time + D/h] += 1;
}




