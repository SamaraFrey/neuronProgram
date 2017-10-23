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
#include <cassert>
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

void checkValue(double value);
double setExtCurr();
Interval setInterval();

int main() {
    int nbr;

    cout << "How many neurons do you want? //enter 3 for the moment!" << endl;
    cin >> nbr;
    checkValue(nbr); //check so it so not <=0
    
    vector<Neuron> allNeuron(nbr); //vectors with all the neurons
    
    //give neurons values manually
    allNeuron[0] = Neuron(3,0,0);
    allNeuron[1] = Neuron(2,0,0);
    allNeuron[2] = Neuron(1,0,0);
    
    //connect them manually (for now)
    allNeuron[0].addConnect(allNeuron[1]);
    allNeuron[0].addConnect(allNeuron[2]);
    allNeuron[1].addConnect(allNeuron[2]);
    
    //get the time interval
    int time = 0; //starting with 0; global clock
    
    double extCurr = setExtCurr();
    Interval enteredI = setInterval();
    
    
    int timeStop; //steps
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
            bool spiked;
            spiked = allNeuron[i].update(time, extCurr);
            
            cout << "bobibob" << endl;
            if(spiked){
                //if spiked -> store time in Neuron time vector
                double calcTime = time * h;
                allNeuron[i].setTimeSp(calcTime);
                allNeuron[i].putInVector(calcTime);

                //if spiked also tell to buffer in connected neurons
                for(size_t p; p < allNeuron[i].getConnecSize(); ++p){
                    bool worked = allNeuron[i].getConnectNeuron(p+1).receive(time); //bc first one is nullptr; i add D in function receive
                    assert(worked == false); //if receiving didn't work we are going to stop the programm
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

//to check value
void checkValue(double value){
    if (value<=0){
        cout << "Your value is impossible." << endl;
        EXIT_FAILURE;
    }
}

double setExtCurr(){
    double val;
    cout << "Please enter your value for external current:   ";
    cin >> val;
    
    checkValue(val);
    
    return val;
}

Interval setInterval(){
    double val;
    cout << "Please enter your value for the Start of the time Interval:   ";
    cin >> val;
    
    checkValue(val);
    
    double val2;
    cout << "Please enter your value for the End of the time Interval:   ";
    cin >> val2;
    
    checkValue(val2);
    
    Interval interval = {val, val2};
    return interval;
}