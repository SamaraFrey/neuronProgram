//
//  unittest.cpp
//  SV Project
//
//  Created by Samara Frey on 03.11.17.
//  Copyright © 2017 Samara Frey. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "neuron.hpp"
#include "gtest/gtest.h"
#include "constants.h"
#include <cmath>;

TEST (NeuronTest, MembranePotential){
    Neuron neuron;
    
    neuron.update(1, 0);
    EXPECT_EQ((getMempot(),);
}