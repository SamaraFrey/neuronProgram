//
//  main.cpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//
#include "neuron.hpp"
#include "constants.h"

#include <cmath>
#include <iostream>
#include <fstream>

//make step time

//comment all the constants
//make h/tau constant
//add some asserts
//his update has only steps as arg
//reset buffer
//check of negativ Iext

using namespace std;

struct Interval{
    double start;
    double end;
};

double setExtCurr();
Interval setInterval();

int main() {
    int nbr;

    cout << "How many neurons do you want?" << endl;
    cin >> nbr;
    vector<Neuron> allNeuron(nbr); //vectors with all the neurons
    
    //connect them manually
    allNeuron[0].connectWith();
    
    if(val <=0){
        cout << "your amount of neurons doesn't make sense." << endl;
        return 0;
    }
    
    double extCurr;
    Interval enteredI;
    int time = 0; //starting with 0; global clock
    
    extCurr = setExtCurr();
    enteredI = setInterval();
    
    
    int timeStop;
    double val = enteredI.end - enteredI.start;
    timeStop = val/h; //gives me times of step
    timeStop += time; //bc time line
    
    //open a new file
    ofstream write;
    write.open("memPotValues.txt");

    
    if(write.is_open()){
        cout << "File is open now" << endl;
    } else {
        cout << "File is still closed" << endl;
    }


    //Step 1: calculate newMemPot and store values
    while(time <= timeStop){ //we increment always by h (which is const and defined in constants.h as long as I'm still in my intervall
        for(size_t i = 0; i < allNeuron.size(); ++1){
            if(allNeuron[i].spiked()){
                for(size_t p; p < allNeuron[i].getConnecSize(); ++p){
                    allNeuron[i].connections[p]->addConnec();
                }
            }
        }
        
        //check if file is open
        if (write.fail() == true){
            cout << "Error, I could not open a file for you." << endl;
            break;
        }
        
        bool updated;
        double calcTime = time * h;
        neuron.setConnec(static_cast<int>(neuron.getConnecSize())); //make size_t into int
        updated = neuron.update(time, extCurr); //update the potential ect
        
        //if spiked -> store time in Neuron time vector
        if (updated){
            
            neuron.setTimeSp(calcTime);
            neuron.putInVector(calcTime);
        }
        
        
        cout << neuron.getMemPot() << " , " << neuron.getNbrSp() << ", " << neuron.getTimeSp() << endl;
        
        //store Membran potential in file
        double enter = neuron.getMemPot();
        write << enter;
        cout << endl;

        ++time;
    }
    
    
    
    //Step 2: connect neurons together
    Neuron neuron2(3, 2, 12);
    neuron.addConnect(neuron2);
    
    
    write.close();

    
    return 0;
}

double setExtCurr(){
    double val;
    cout << "Please enter your value for external current:   ";
    cin >> val;
    return val;
}

Interval setInterval(){
    double val;
    cout << "Please enter your value for the Start of the time Interval:   ";
    cin >> val;
    
    double val2;
    cout << "Please enter your value for the End of the time Interval:   ";
    cin >> val2;
    
    Interval interval = {val, val2};
    return interval;
}