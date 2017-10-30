//
//  inhibitory.hpp
//  SV Project
//
//  Created by Samara Frey on 23.10.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//

#ifndef inhibitory_hpp
#define inhibitory_hpp

#include <stdio.h>
#include "neuron.hpp"


class Inhibitory: public Neuron
{
public:
    //!< Constructor
    Inhibitory(): Neuron(Ji){}
};


#endif /* inhibitory_hpp */
