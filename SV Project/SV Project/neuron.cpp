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
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//Constructors
Neuron::Neuron(double p, int sp, double t){
    memPot = p;
    nbrSp = sp;
    timeSp = t;
    connecInput = 0;
    
    connections.push_back(nullptr);
    
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
        double newMemPot = memPot * exp(-h/tau) + extCurr * (R) * (1-exp(-h/tau))+J*connecInput;
        //cout << newMemPot << endl; //check if it is calculated out
        setMemPot(newMemPot);
    
        //break if negative
        if(getMemPot() < 0){
            cout << "Your membrane potential is not possible" << endl;
            return false;
        }
    
        if (spiked()){
            int val = getNbrSp() + 1;
            setNbrSp(val);
            setMemPot(10); //can use 0 or 10mV
            calcTime += refactime; //insensitiv to stimulation after spike
            clock = calcTime/h;
            return true;
        }
    
        clock = calcTime/h;
        return false;
        
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

void Neuron::updateConnecInput(){
    size_t size = getConnecSize(); //bc size can not be negativ
    
    cout << size << endl;
    
    for(size_t i = 0; i < size; ++i){
        Neuron neuronTest;
        neuronTest = connections[i];
        if (clock == neuronTest.spikeVect[i] + D){
            setConnec(getConnec() + 1);
        }
    }
}


