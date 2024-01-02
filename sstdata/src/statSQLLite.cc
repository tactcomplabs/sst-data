//
// _STATSQLLITE_CC_
//
// Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "statSQLLite.h"

namespace SST::Statistics{

StatisticOutputSQLLite::StatisticOutputSQLLite(Params& outputParameters)
  : SSTDataBase(outputParameters) {
  setStatisticOutputName("StatisticOutputSQLLite");
}

bool StatisticOutputSQLLite::checkOutputParameters(){
  return true;
}

void StatisticOutputSQLLite::printUsage(){
  Output out("", 0, 0, Output::STDOUT);
  out.output(" : Usage - Sends all statistic output compilation test.\n");
}

void StatisticOutputSQLLite::startOfSimulation(){
  std::cout << "startOfSimulation" << std::endl;
}

void StatisticOutputSQLLite::endOfSimulation(){
  std::cout << "endOfSimulation" << std::endl;
}

void StatisticOutputSQLLite::implStartOutputEntries(StatisticBase* statistic){
}

void StatisticOutputSQLLite::implStopOutputEntries(){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, int32_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, uint32_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, int64_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, uint64_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, float data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle, double data){
}

};  // end SST::Statistics

// EOF
