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

    cout << "How many neurons do you want? //enter 3 for the moment!" << endl;
    cin >> nbr;
    vector<Neuron> allNeuron(nbr); //vectors with all the neurons
    
    //connect them manually (for now)
    allNeuron[0].addConnect(allNeuron[1]);
    allNeuron[0].addConnect(allNeuron[2]);
    allNeuron[1].addConnect(allNeuron[2]);
    
    if(nbr <=0){
        cout << "your amount of neurons doesn't make sense." << endl;
        return 0;
    }
    
    //get the time interval
    int time = 0; //starting with 0; global clock
    
    double extCurr = setExtCurr();
    Interval enteredI = setInterval();
    
    
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
    
    //check if file is open
    if (write.fail() == true){
        cout << "Error, I could not open a file for you." << endl;
        return 0;
    }

    //Step 1: calculate newMemPot and store values
    while(time <= timeStop){ //we increment always by h (which is const and defined in constants.h as long as I'm still in my intervall
        for(size_t i = 0; i < allNeuron.size(); ++i){
            bool spiked = allNeuron[i].update(time, extCurr);
            
            cout << "bobibob" << endl;
            if(spiked){
                //if spiked -> store time in Neuron time vector
                double calcTime = time * h;
                allNeuron[i].setTimeSp(calcTime);
                allNeuron[i].putInVector(calcTime);

                //if spiked also tell to buffer in connected neurons
                for(size_t p; p < allNeuron[i].getConnecSize(); ++p){
                    allNeuron[i].getConnectNeuron(p+1).receive(time); //bc first one is nullptr; i add D in function receive
                }
               
            }
            
            //if it doesn't spike we continue here
            //to check if values calculated
            cout << allNeuron[i].getMemPot() << " , " << allNeuron[i].getNbrSp() << ", " << allNeuron[i].getTimeSp() << endl;
            
            //store Membran potential in file
            double enter = allNeuron[i].getMemPot();
            write << enter;
            cout << endl;
        
        }

        ++time;
    }
    
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