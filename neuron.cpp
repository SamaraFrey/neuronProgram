//
//  neuron.cpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//


/*
This is the main class for neurons, afterwards we will have subclasses which inherit from it.
 -> inhibitory and excitatory
Each neuron has certain amount of connections, some spike it gives and receives and its  own local clock.

We store how many spikes it does and when they occur. The membrane potentials of the neuron are going to be printed into a seperate file -> this happens in main.
*/

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
    
    vector <unsigned int> buffer (bufferSize); //buffer of size, with all entries to be zero
}

Neuron::Neuron(){
    connections.push_back(nullptr); //for debugging purposes
    
    vector <unsigned int> buffer (bufferSize); //buffer of size, with all entries to be zero
} //default

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


/*
 This function is responsible to update the neuron, meaning it reset the membrane potential, taking account how many spike it receives and what external current it gets.
 @param time is the global time, to see where we are at
        the extCurr is the external current which has been entered in the main
 @return the update fn returns a boolean, this helps to assure that the neuron has been    updated
*/

bool Neuron::update(int time, double extCurr){
    //since buffer has size D/h+1 the time is gonna exceed it fast
    
    //since we interate through the buffervector and start again everytime when we exceed it, we substract the size till it is smaller and then can be read at correct place
    assert(time == 0);
    
    int timeStep = time;
    if(time > bufferSize){
        timeStep = time % bufferSize; //e.g. 13%4 = 1 (bc 3*4 + 1 = 13)
    }
    
    //count the received spikes
    int spikeNbr = buffer[timeStep-1];
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
        double newMemPot = memPot * exp(-h/tau) + extCurr * (R) * (1-exp(-h/tau))+neuronJ*spikeNbr;
        
        //break if negative
        assert(getMemPot() < 0);
        
        //set it as new potential
        setMemPot(newMemPot);
    }
    
    clock = clock+1;
    return spiking;
}


/*
 This function is responsible to tell if the neuron spikes.
 
 @return if the neuron spikes -> true
         if the neuron doesn't spike -> false
 */

bool Neuron::spiked(){
    if(getMemPot() >= treshold){
        return true;
    }
    else {
        return false;
    }
}


/*
 This function is responsible to connect neurons together, it pushes the new connected neuron in the existing connection vector.
 @param the neuron that it is gonna be connected to
*/

void Neuron::addConnect(Neuron other){
    //cout << "I'm connecting the neurons" << endl;
    Neuron* pointerNeuron = &other;
    connections.push_back(pointerNeuron);
}


/*
 This function is responsible to show to which neuron it is connected to. It is mainly done to make the private vector connection to be accessible to be able to be read.
 @param i is the place in the vector that we wanna look at
 @return    logically this function returns the neuron at that certain place of the connection vector
*/

Neuron Neuron::getConnectNeuron(int i){
    Neuron neuron = *connections[i];
    return neuron;
}


/*
 This function is responsible receive all the spikes it gets from its connected neurons and also to reset the buffer if necessary.
 @param the time is the global time we need to check where we are at in the buffer
 */

bool Neuron::receive(int time){
    //if time is smaller then bufferSize we can store +1 at time
    int timeStep = time;
    assert(timeStep<0); //stop if time is negative

    if(timeStep <= bufferSize){
        buffer[timeStep] += 1;
        return true;
    }
    
    //if time is overexceeding bufferSize, we need to start from the beginning again to store it at correct place
    else{
        //set buffer all to 0 bc we "start" from new with the vector
        for(int i; i < bufferSize; ++i){
            buffer[i] = 0;
        }
        
        //we calculate the time till it is smaller then the bufferSize (bc we start vector again) and store it accordingly
        do{
            timeStep -= bufferSize;
        }while(timeStep > bufferSize);
        
        //now timeStep is smaller then buffersize, so we store +1
        buffer[timeStep] += 1;
    }
    
    return true;
}




