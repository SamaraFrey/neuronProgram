//
//  exhibitory.hpp
//  SV Project
//
//  Created by Samara Frey on 23.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//

#ifndef exhibitory_hpp
#define exhibitory_hpp

#include <stdio.h>
#include "neuron.hpp"
//#include "constants.h"



class Exhibitory: public Neuron
{
public:
    //Constructor
    Exhibitory(): Neuron(Je){}
};


#endif /* exhibitory_hpp */