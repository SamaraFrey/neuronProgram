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

//!< Idea: overwrite print operator

#include "neuron.hpp"
#include "constants.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

//!< Constructors
Neuron::Neuron(double p, int sp, double t){
    memPot = p;
    nbrSp = sp;
    timeSp = t;
    neuronJ = J;
    
    connections.push_back(nullptr); //!< for debugging purposes
    
    //!< buffer of size, with all entries to be zero
    for(int i = 0; i < bufferSize; ++i){
        buffer.push_back(0);
    }
}

Neuron::Neuron(){
    neuronJ = J;
    connections.push_back(nullptr); //!< for debugging purposes
    
    //!< buffer of size, with all entries to be zero
    for(int i = 0; i < bufferSize; ++i){
        buffer.push_back(0);
    }
}

Neuron::Neuron(double Jvalue){
    neuronJ = Jvalue;
    connections.push_back(nullptr); //!< for debugging purposes
    
    //!< buffer of size, with all entries to be zero
    for(int i = 0; i < bufferSize; ++i){
        buffer.push_back(0);
    }
}

/*
//!< operators
void Neuron::operator<<(Neuron write){
    cout << "my nbr of spikes: " << getNbrSp() << ", the time of spikes: " << getTimeSp() << ", the membrane pot: " << getMemPot() << endl;
} */

//!< functions
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
    //!< since buffer has size D/h+1 the time is gonna exceed it fast
    
    //!< since we interate through the buffervector and start again everytime when we exceed it, we substract the size till it is smaller and then can be read at correct place
    int timeStep = time;
    if(time >= bufferSize){
        timeStep = time % bufferSize; //!< e.g. 13%4 = 1 (bc 3*4 + 1 = 13)
    }
    
    cout << "updating a neuron:" << endl;
    
    //!< count the received spikes
    unsigned int spikeNbr;
    if(timeStep == 0){ //!< bc first time step is at the zero element of vector and we start with time 0
        spikeNbr = 0;
    } else {
        spikeNbr = buffer[timeStep];
    }
    
    cout << " the amount of spikes is: " << spikeNbr << endl;
    
    bool spiking = false;
    
    //!< since the whole clocksystem is in steps, also the refractorytime has to be used in steps
    int refractStep = refactime/h;
    assert(refractStep > 0); //!< refractorytime is for sure never gonna be 0; so neither is the steps
    
    //!< neuron is insensitiv for a refacttime
    //!< check when neuron has last spiked if my spikeVect is not empty
    if(!spikeVect.empty()){
        cout << "********** I've had some spikes already **********" << endl;
        //!< if last spike as occured less then refracttime ago
        if((clock-(timeSp/h)) <= refractStep){
            cout << " setting mem to 0 because i just spiked recent" << endl;
            setMemPot(0); //!< can use 0 or 10mV
            clock += 1; //!< increase clock
            return false;
        }
        cout << " Spikes have been longer then refracttime ago!" << endl;
    }

    //!< if spike has been longer ago then refac we just continue

    //!< poisson distribution of background input
    static random_device rd;
    static mt19937 gen(rd());
    static poisson_distribution<> p(lambda); //!< gives me back an int as default
    
    
    double background = static_cast<double> (p(gen)); //!< change type
    cout << "background noise is: " << background << endl;
    cout << "old mempot is: " << memPot << endl;
    while (background < 0){
        background = p(gen);
    }
    
    assert(background >= 0);
    
    double newMemPot = memPot * exp(-h/tau) + extCurr * (R) * (1-exp(-h/tau))+(spikeNbr+background) * neuronJ;
    
    //!< set it as new potential
    setMemPot(newMemPot);
    
    //!< break if negative
    assert(getMemPot() >= 0);
    
    if(spiked()){
        //!< increase the nbr of spikes the neuron itself has
        setNbrSp(getNbrSp() + 1);
        setMemPot(0);
        
        //!< store the time in the spike vector and set spike time
        setTimeSp(time*h);
        putInVector(time*h);
        spiking = true;

        
        cout << "! OUH I spiked! I set mempot to 0!" << endl;
    }

    clock += 1; //!< increase local clock
    
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

void Neuron::addConnect(Neuron * other){
    //!< cout << "I'm connecting the neurons" << endl;
    connections.push_back(other);
}


/*
 This function is responsible to show to which neuron it is connected to. It is mainly done to make the private vector connection to be accessible to be able to be read.
 @param     i is the place in the vector that we wanna look at
 @return    it returns the neuron at that certain place of the connection vector
*/

Neuron * Neuron::getConnectNeuron(int i){
    //!< i added a nullptr as first element of the connection
    cout << endl << endl;
    cout << "                I CONNECT TO A NEURON" << endl;
    return connections[i];
}


/*
 This function is responsible receive all the spikes it gets from its connected neurons and also to reset the buffer if necessary.
 @param the time is the global time we need to check where we are at in the buffer
*/

bool Neuron::receive(int time){
    //!< if time is smaller then bufferSize we can store +1 at time
    int timeStep = time;    //!< delay D taken in account when called
    assert(timeStep >= 15); //!< stop if time is negative -> at least 15! bc delay has been added

    //!< modulo will not work for timestep (only 15 bc of delay) smaller then bufferSize
    
    if(timeStep < bufferSize){
        buffer[timeStep] += 1;
        return true;
    }
    
    cout << "time step is: " << timeStep << endl;
    //!< if time is overexceeding bufferSize, we need to start from the beginning again to store it at correct place

    //!< calculate where we store the +1
    int storeTime = timeStep % bufferSize;
    
    assert(storeTime <= bufferSize); //!< will always be smaller then 16
    
    //!< set buffer all to 0 bc we "start" from new with the vector
    bool cleaned = cleanBuffer();
    
    cout << "the store time: " << storeTime << endl;
    assert(cleaned == true); //!< buffer has to be cleaned
    
    assert(storeTime < static_cast<int> (buffer.size())); //!< assert if storeTime is bigger
    
    //!< now timeStep is smaller then buffersize, so we store +1
    buffer[storeTime] += 1;
    
    return true;
}


/*
 This function has been created for debugging porpuses only. Also it makes the code more readable.
 @return true if the buffer elements have been set to 0
*/

bool Neuron::cleanBuffer(){
    for(int i = 0; i < bufferSize; ++i){
        buffer[i] = 0;
    }s
    return true;
}


/*
 This function will be called in the very end to free and delete the pointers of the connection vector.
 @return true if the pointers have been freed
*/

bool Neuron::destroyConnection(){
    for(size_t i = 0; i < connections.size(); ++i){
        connections[i] = nullptr;
        delete connections[i];
    }
    assert(connections.empty()); //!< if connections is not empty assert
    return true;
}

