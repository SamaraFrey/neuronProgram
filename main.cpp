//
//  main.cpp
//  SV Project
//
//  Created by Samara Frey on 02.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//
#include "inhibitory.hpp"
#include "exhibitory.hpp"
#include "neuron.hpp"
#include "constants.h"

#include <cmath>
#include <cassert>
#include <iostream>
#include <random>
#include <fstream>


/*
 Here we simulate the neuron network. We connect the neurons and give them random values.
 We have a global clock with ticks in time steps rather then h. In addition to not always redefine the constants, there is a constant.h file to make the constants more available and easier to change.
*/

using namespace std;

struct Interval{
    double start;
    double end;
};

void checkValue(double value);
//double setExtCurr();
Interval setInterval();

vector<int> getRandom(int nbrChoose, int i);

int main() {
    vector<Neuron> allNeuron; //!< vectors with all the neurons ex and in
    
    //!< add first 10'000 exhibitory neurons
    for(int i = 0; i < Ne; ++i){
        Exhibitory neuron;
        allNeuron.push_back(neuron);
    }
    
    //!< add from the 10'001th on 2'500 inhibitory neurons
    for(int i = 0; i < Ni; ++i){
        Inhibitory neuron;
        allNeuron.push_back(neuron);
    }
    
    cout << "size of allNeurons is: " << allNeuron.size() << endl;
    
    //!< connect randomly neurons with each other
    //!< each neuron has connections to 250 in (Ci), 1000 ex (Ce)
    
    //!< simplicitywise i first store all ex (10'000) and then all in (250) in the allNeuron vector
    
    //!< connect the exhibitory neurons
    //!< which are the first neurons in the allNeuron vector
    for(int j = 0; j < Ne; ++j){

        //!< generate vector (size 1000) with random nbrs from 1 to 10000 for exhib connections
        vector<int> randomVector1(getRandom(Ne, j));
        for(int p = 0; p < randomVector1.size() ; ++p){
            int neuronNbr1 = randomVector1[p];
            allNeuron[j].addConnect(& allNeuron[neuronNbr1]);
        }

        
        //!< generate vector (size 250) with random nbrs from 1 to 2500 for inhib connections
        vector<int> randomVector2(getRandom(Ni, j));
        for(int k = 0; k < randomVector2.size(); ++k){
            int neuronNbr2 = randomVector2[k];
            allNeuron[j].addConnect(& allNeuron[Ne + neuronNbr2]); //!< bc we start having inhibitory neurons from 10'001th on
        }

    }
    
    //!< connect the inhibitory neurons
    //!< my inhibitory neurons start from the Ne's value on and i want to go till the end which is at Ne+Ni (attention: vector starts with 0)
    for(int a = Ne; a < Ni+Ne; ++a){
       
        //!< generate vector (size 1000) with random nbrs from 1 to 10000 for exhib connections
        vector<int> randomVector1(getRandom(Ne, a));
        for(int p = 0; p < randomVector1.size(); ++p){
            int neuronNbr1 = randomVector1[p];
            allNeuron[a].addConnect(& allNeuron[neuronNbr1]);
        }
        
        //!< generate vector (size 250) with random nbrs from 1 to 2500 for inhib connections
        vector<int> randomVector2(getRandom(Ni, a));
        for(int k = 0; k < randomVector2.size(); ++k){
            int neuronNbr2 = randomVector2[k];
            allNeuron[a].addConnect(& allNeuron[Ne + neuronNbr2]); //!< bc we start having inhibitory neurons from 10'001th on
        }
    }


    //!< get the time interval
    int time = 0; //!< starting with 0; global clock
    
    double extCurr = 0.0;
    Interval enteredI = setInterval();
    
    
    int timeStop; //!< steps
    double val = enteredI.end - enteredI.start;
    timeStop = val/h; //!< gives me times of step
    timeStop += time; //!< bc time line
    
    //!< open a new file
    ofstream write;
    write.open("memPotValues.txt");
    
    ofstream spikes;
    spikes.open("spikes.txt");

    
    if(write.is_open()){
        cout << "File is open now" << endl;
    } else {
        cout << "File is still closed" << endl;
    }
    
    //!< check if file is open
    if (write.fail() == true){
        cout << "Error, I could not open a file for you." << endl;
        return 0;
    }
    
    if(spikes.is_open()){
        cout << "File is open now" << endl;
    } else {
        cout << "File is still closed" << endl;
    }
    
    //!< check if file is open
    if (spikes.fail() == true){
        cout << "Error, I could not open a file for you." << endl;
        return 0;
    }


    //!< calculate newMemPot and store values
    while(time <= timeStop){ //!< we increment always by h (which is const and defined in constants.h as long as I'm still in the intervall
        for(size_t i = 0; i < allNeuron.size(); ++i){
            cout << "the time is: " << time << endl;
            bool spiked;
            cout << "THIS IS NEURON: " << i+1 << endl;
            
            spiked = allNeuron[i].update(time, extCurr);
            
            if(spiked){
                cout << "YES I SPIKED! ------------------------------------------------" << endl;

                //!< if spiked also tell to buffer in connected neurons
                //!< p starts from 1 because nullptr is first element
                for(int p = 1; p < allNeuron[i].getConnecSize(); ++p){
                    bool worked = allNeuron[i].getConnectNeuron(p)->receive(time+(D/h)); //!< Delay added
                    
                    assert(worked == true);
                }
            }
            
            //!< if it doesn't spike we continue here
            //!< to check if values calculated
            cout << allNeuron[i].getMemPot() << " , " << allNeuron[i].getNbrSp() << ", " << allNeuron[i].getTimeSp() << endl;
            
            
            //!< store Membran potential in file
            double enter = allNeuron[i].getMemPot();
            write << enter;
            write << endl;
            cout << endl;
        }

        ++time;
    }
    
    for(size_t i = 0; i < allNeuron.size(); ++i){
        for(int j = 0; j < allNeuron[i].getSpikeVectSize(); ++j){
            double val = allNeuron[i].getSpikeVect(j);
            spikes << val;
            spikes << " ";
        }
        
        spikes << endl;
    }
    
    
    write.close();
    spikes.close();

    
    return 0;
}

//!< to check value
void checkValue(double value){
    if (value<=0){
        cout << "Your value is impossible." << endl;
        EXIT_FAILURE;
    }
}

/*
double setExtCurr(){
    double val;
    cout << "Please enter your value for external current:   ";
    cin >> val;
    
    assert(!cin.fail());
    checkValue(val);
    
    return val;
}
*/

Interval setInterval(){
    double val;
    cout << "Please enter the time (double) for the Start of the time Interval:   ";
    cin >> val;
    
    assert(!cin.fail());
    checkValue(val);
    
    double val2;
    cout << "Please enter the time (double) for the End of the time Interval:   ";
    cin >> val2;
    
    assert(!cin.fail());
    checkValue(val2);
    
    Interval interval = {val, val2};
    return interval;
}


/*
 This function is responsible to to create xxxx random numbers and store then in a vector.
 We always need to create nbrChoose/10. Which would be Ce or Ci depending on which neuron I want to connect.
 ATTENTION: the random number can't be i
 @param nbrChoose is the "pot" of numbers where we can choose from
 i is the only number we should not generate
 */

vector<int> getRandom(int nbrChoose, int i){
    vector<int> randomNbrs (nbrChoose/10); //!< size is 10th of nbrs to choose from (10'000 -> 1'000; 2'500 -> 250)
    random_device rd;
    mt19937 gen(rd());
    
    for(size_t k = 0; k < randomNbrs.size(); ++k){
        uniform_int_distribution<> d(1, nbrChoose-1); //!< whatever value -> event occurs 4 times a minute in average (now)
        randomNbrs[k] = d(gen);
        assert(randomNbrs[k] >= 1);
        
        //!< exclude the option to be connected with itself!
        while(d(gen) == i){
            randomNbrs[k] = d(gen); //!< give it another nbr if its the same as neuron itself
        }
    }
    return randomNbrs;
}








