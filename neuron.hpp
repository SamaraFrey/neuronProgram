//
//   neuron.hpp
//   SV Project
//
//   Created by Samara Frey on 02.10.17.
//   Copyright © 2017 Samara Frey. All rights reserved.
//

#ifndef neuron_hpp
#define neuron_hpp

#include "constants.h"
#include <stdio.h>
#include <vector>

using namespace std;

class Neuron
{
private:
    double memPot;  //!< initial value to be 0 //!< unit mV
    int nbrSp;      //!< amount of spike a neuron made
    double timeSp;  //!< when last spike occured //unit ms
    double neuronJ; //!< value J that changes for type of neuron
    
    int clock; //!< local clock
    
    vector <double> spikeVect;      //!< store the time of Spikes
    vector <unsigned int> buffer;   //!< buffer
    vector <Neuron*> connections;   //!< store the neurons that are connected in a vector
    
public:
    //!< Constructors
    Neuron(); //!< default
    Neuron(double Jvalue); //!< for subclasses to override J
    
    Neuron(const Neuron& copy) = default; //!< copy
    
    //!< Destructor
    virtual ~Neuron(){};
    
    //!< functions
    void putInVector(double time);
    bool update(int time, double extCurr);
    bool spiked(); //!< true if V over threshold
    void addConnect(Neuron * other);
    Neuron * getConnectNeuron(int i);
    bool receive(int time);
    bool cleanBuffer(); //!< set all bufferelements to 0
    bool destroyConnection(); //!< free the pointers

    //!< setters
    
    //!< getters
    double getMemPot(){
        return memPot;
    }

    double getTimeSp(){
        return timeSp;
    }
    
    int getNbrSp(){
        return nbrSp;
    }
    
    size_t getConnecSize(){
        return connections.size();
    }
    
    double getSpikeVect(size_t i){
        return spikeVect[i];
    }
    
    size_t getSpikeVectSize(){
        return spikeVect.size();
    }
    
};


/**
 * This class is for the exhibitory neurons, which have a different value for J.
*/

class Exhibitory: public Neuron
{
public:
    //! Constructor
    Exhibitory(): Neuron(Je){}
};


/**
 * This class is for the inhibitory neurons, which have a different value for J.
*/

class Inhibitory: public Neuron
{
public:
    //! Constructor
    Inhibitory(): Neuron(Ji){}
};



#endif /* neuron_hpp */
