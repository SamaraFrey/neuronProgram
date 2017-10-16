//
//  neuron.hpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//

#ifndef neuron_hpp
#define neuron_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class Neuron
{
private:
    double memPot; //initial value to be 0 //unit mV
    int nbrSp;
    double timeSp; //#when spikes occured //unit ms
    
    int clock; //local clock
    
    vector <double> spikeVect; //store the time of Spikes
    vector <Neuron*> connections; //store the neurons that are connected in a vector
    vector <unsigned int> buffer; //buffer
    
public:
    //Constructors
    Neuron(double p, int sp, double t);
    Neuron(); //default
    Neuron(const Neuron& copy) = default; //copy
    
    //Destructor
    ~Neuron(){};
    
    //operators
    void operator=(Neuron* other);
    
    //functions
    void putInVector(double time);
    bool update(int time, double extCurr);
    bool spiked(); //true if V over threshold
    void addConnect(Neuron other);
    Neuron getConnectNeuron(int i);
    void receive(int time);

    //setters&getters might be useful for later
    //setters
    void setMemPot(double a){
        memPot = a;
    }
    
    void setNbrSp(int a){
        nbrSp = a;
    }
    
    void setTimeSp(double a){
        timeSp = a;
    }
    
    
    //getters
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
    
    int getSpikeVect(int i){
        return spikeVect[i];
    }
    
    int getClock(){
        return clock;
    }
};


#endif /* neuron_hpp */
