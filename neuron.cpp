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

//Idea: overwrite print operator

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
    
    int bufferSize = D/h + 1; //bc D is constant (given in paper)
    vector <unsigned int> buffer (bufferSize); //buffer of size, with all entries to be zero
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
    //since buffer has size D/h+1 the time is gonna exeed it fast
    int timeStep = time;
    do{
        timeStep -= (D/h + 1);
    }while(timeStep > (D/h+1));
    
    //count the received spikes
    int spikeNbr = buffer[timeStep];
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

bool Neuron::receive(int time){
    //if time is smaller then bufferSize we can store +1 at time
    int timeStep = time;

        if(timeStep <= (D/h+1)){
        buffer[timeStep + D/h] += 1;
        return true;
    }
    
    //if time is overexceeding bufferSize, we need to start from the beginning again
    else{
        //set buffer all to 0 bc we "start" from new with the vector
        for(int i; i < buffer.size(); ++i){
            buffer[i] = 0;
        }
        
        //make time where we store +1 smaller till it is okay
        do{
            timeStep -= D/h;
        }while(timeStep > (D/h+1));
        
        //now timeStep is smaller then buffersize, so we store +1
        buffer[timeStep] += 1;
    }
    
    return true;
}




