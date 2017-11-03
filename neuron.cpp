//
//  neuron.cpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//


/**
* This is the main class for neurons, afterwards we will have subclasses which inherit from it.
*  -> inhibitory and excitatory
* Each neuron has certain amount of connections, some spike it gives and receives and its  own local clock.
*
* We store how many spikes it does and when they occur. The membrane potentials of the neuron are going to be printed into a seperate file -> this happens in main.
*/

#include "neuron.hpp"
#include "constants.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

//! Constructors
//!< This constructor is called it no arguments are passed.
Neuron::Neuron(){
    neuronJ = J;
    connections.push_back(nullptr); //!< For debugging purposes our first element is going to
    
    //!< Buffer of size (D/h+1), with all its entries to be zero
    for(int i = 0; i < bufferSize; ++i){
        buffer.push_back(0);
    }
}

//!< This constructor is called if the value J has to be different. Ergo will be called by its subclasses
Neuron::Neuron(double Jvalue){
    neuronJ = Jvalue;
    connections.push_back(nullptr); //!< For debugging purposes our first element is going to
    
    //!< Buffer of size (D/h+1), with all its entries to be zero
    for(int i = 0; i < bufferSize; ++i){
        buffer.push_back(0);
    }
}

//! Functions of neurons

/**
 * Pushing back given times into the spikeVect vector, which we later on need to write the values into the spike.txt file.
 * @param time: the time the spike occured and has to be saved.
 */

void Neuron::putInVector(double time){
    spikeVect.push_back(time);
}


/**
 * This function is responsible to update the neuron, meaning it reset the membrane potential, taking account how many spike it receives and what randomly generated background noise it gets.
 * @param time: is the global time, to see where we are at
 * @param the extCurr: is the external current which has been before entered in the main and now is set to 0
 * @return the update fn returns a boolean (true if updated), this helps to assure that the neuron has been updated.
 */

bool Neuron::update(int time, double extCurr){
    /*! 
     since buffer has size D/h+1 the time is gonna exceed it fast
     since we interate through the buffervector and start again everytime when we exceed it, we substract the size till it is smaller and then can be read at correct place
    */
    
    int timeStep = time;
    if(time >= bufferSize){
        timeStep = time % bufferSize;
    }
    
    //!< Count the spikes received at time
    unsigned int spikeNbr;
    
    //!< first time step is at the zero element of vector and we start with time 0
    if(timeStep == 0){
        spikeNbr = 0;
    } else {
        spikeNbr = buffer[timeStep];
    }
    
    bool spiking = false;
    
    //!< The whole clocksystem is in steps, therefore the refractorytime has to be used in steps as well
    int refractStep = refactime/h;
    
    //!< Refractorytime is for sure never gonna be 0; so neither is the steps
    assert(refractStep > 0);
    
    //! The neuron is insensitiv for a refacttime
    //!< If my spikeVect is not empty, then check when neuron has had last spike
    if(!spikeVect.empty()){
        
        //!< If last spike as occured less then refracttime ago
        if((clock-(timeSp/h)) <= refractStep){
            memPot = 0;
            clock += 1;
            return false;
        }
    }

    //!< If spike has been longer ago then refracttime, we just continue

    //!< Generating background noise of the brain by possion distribution
    static random_device rd;
    static mt19937 gen(rd());
    static poisson_distribution<> p(lambda); //!< Pois() gives me back an int as default
    
    double background = static_cast<double> (p(gen)); //!< change type
    assert(background >= 0); //!< background noise can't be negativ (Pois() is never negative)
    
    //! Calculate membrane potential
    double newMemPot = (memPot * exp(-h/tau)) + (extCurr * (R) * (1-exp(-h/tau)))+spikeNbr * neuronJ + background * J;
    
    //!< Set it as new potential
    memPot = newMemPot;
   
    if(spiked()){
        
        //!< Increase the nbr of spikes the neuron itself has
        nbrSp += 1;
        memPot = 0;

        //!< Store the time in the spike vector and set spike time
        timeSp = time*h;
        putInVector(time*h);
        spiking = true;
    }

    clock += 1; //!< Increase local clock
    
    return spiking;
}


/**
* This function is responsible to tell if the neuron spikes.
*
* @return if the neuron spikes -> true
*         if the neuron doesn't spike -> false
*/

bool Neuron::spiked(){
    if(memPot >= treshold){
        return true;
    }
    else {
        return false;
    }
}


/**
* This function is responsible to connect neurons together, it pushes the new connected neuron in the existing connection vector.
* @param the neuron that it is gonna be connected to
*/

void Neuron::addConnect(Neuron * other){
    //!< cout << "I'm connecting the neurons" << endl;
    connections.push_back(other);
}


/**
* This function is responsible to show to which neuron it is connected to. It is mainly done to make the private vector connection to be accessible to be able to be read.
* @param     i is the place in the vector that we wanna look at
* @return    it returns the neuron at that certain place of the connection vector
*/

Neuron * Neuron::getConnectNeuron(int i){
    //!< i added a nullptr as first element of the connection
    return connections[i];
}


/**
* This function is responsible receive all the spikes it gets from its connected neurons and also to reset the buffer if necessary.
* @param time: is the global time we need to check where we are at in the buffer
* @return true if neuron received spike -> debug purposes
*/

bool Neuron::receive(int time){
    //!< If time is smaller then bufferSize we can store +1 at time
    int timeStep = time;    //!< delay D taken in account when called
    assert(timeStep >= D/h); //!< stop if time is negative -> at least 15! bc delay has been added

    //!< Modulo will not work for timesteps that are  smaller then bufferSize (only 15 bc of delay)
    
    if(timeStep < bufferSize){
        buffer[timeStep] += 1;
        return true;
    }
    
    //!< If time is overexceeding bufferSize, we need to start from the beginning again to store it at correct place

    //!< Calculate where we store the +1
    int storeTime = timeStep % bufferSize;

    //!< Set buffer all to 0 because we overexceeded it and we "start" from new with the vector
    bool cleaned = cleanBuffer();
    assert(cleaned == true); //!< Check that buffer has been cleaned
    
    assert(storeTime < static_cast<int> (buffer.size())); //!< assert if storeTime is bigger
    
    //!< timeStep is now smaller then buffersize, so we store +1
    buffer[storeTime] += 1;
    
    return true;
}


/**
* This function has been created for debugging porpuses only. Also it makes the code more readable.
* @return true if the buffer elements have been set to 0
*/

bool Neuron::cleanBuffer(){
    for(int i = 0; i < bufferSize; ++i){
        buffer[i] = 0;
    }
    return true;
}


/**
* This function will be called in the very end to free and delete the pointers of the connection vector.
* @return true if the pointers have been freed
*/

bool Neuron::destroyConnection(){
    for(size_t i = 0; i < connections.size(); ++i){
        connections[i] = NULL;
        delete connections[i];
    }
    connections.erase(connections.begin(), connections.end());
    
    assert(connections.empty()); //!< if connections is not empty assert
    return true;
}

