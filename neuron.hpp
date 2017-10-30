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
    Neuron(double p, int sp, double t); //!< not needed, only for test porpuses
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

    //!< setters
    void setMemPot(double a){
        memPot = a;
    }
    
    void setNbrSp(int a){
        nbrSp = a;
    }
    
    void setTimeSp(double a){
        timeSp = a;
    }
    
    
    //!< getters
    double getMemPot(){
        return memPot;
    }
    
    int getNbrSp(){
        return nbrSp;
    }
    
    double getTimeSp(){
        return timeSp;
    }
    
    size_t getConnecSize(){
        return connections.size();
    }
    
    double getSpikeVect(int i){
        return spikeVect[i];
    }
    
    size_t getSpikeVectSize(){
        return spikeVect.size();
    }
    
    int getClock(){
        return clock;
    }
    
    double getJ(){
        return neuronJ;
    }
};


#endif /* neuron_hpp */
