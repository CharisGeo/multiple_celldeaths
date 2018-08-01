// -----------------------------------------------------------------------------
//
// Copyright (C) The BioDynaMo Project.
// All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------
#ifndef CELLDEATHTESTS_H_
#define CELLDEATHTESTS_H_

#include "biodynamo.h"

namespace bdm {

struct RemoveFromSimulationBM : public BaseBiologyModule {
  RemoveFromSimulationBM() : BaseBiologyModule(gAllBmEvents) {}

  template <typename T>
  void Run(T* cell) {
    if (cell->GetDiameter() < 50) {
      cell->ChangeVolume(40000);
    } else {
      cell->RemoveFromSimulation();
    }
  }

  ClassDefNV(RemoveFromSimulationBM, 1);
};

// Define compile time parameter
template <typename Backend>
struct CompileTimeParam : public DefaultCompileTimeParam<Backend> {
  using BiologyModules = Variant<RemoveFromSimulationBM>;
};

inline int Simulate(int argc, const char** argv) {
  Simulation<> simulation(argc, argv); 
  auto* param = simulation.GetParam();

  param->bound_space_ = true;
  param->min_bound_ = 0;
  param->max_bound_ = 500;

   auto construct = [&](const std::array<double, 3>& position) {
    Cell cell(position);
    cell.SetDiameter(40);
    cell.SetMass(1.0);
    return cell;
  };
 ModelInitializer::CreateCellsRandom(param->min_bound_, param->max_bound_, 5, construct);

 auto create = [&](const std::array<double, 3>& place){
   Cell cell(place);
   cell.SetDiameter(30);
   cell.SetMass(1.0);
   cell.AddBiologyModule(RemoveFromSimulationBM());
   return cell;
   };
 ModelInitializer::CreateCellsRandom(param->min_bound_, param->max_bound_, 5, create);

 auto develop = [&](const std::array<double, 3>& wall){
   Cell cell(wall);
   cell.SetDiameter(25);
   cell.SetMass(1.0);
   cell.AddBiologyModule(RemoveFromSimulationBM());
   return cell;
   };
 ModelInitializer::CreateCellsRandom(param->min_bound_, param->max_bound_, 5, develop);
 
  // Run simulation for one timestep
  simulation.GetScheduler()->Simulate(2000);


  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // CELLDEATHTESTS_H_
