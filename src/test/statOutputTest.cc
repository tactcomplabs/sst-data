#include <sst/core/sst_config.h>
#include <sst/core/stringize.h>
#include "statOutputTest.h"
//#include <sst/core/simulation_impl.h>

namespace SST::Statistics{

StatisticOutputTest::StatisticOutputTest(Params& outputParameters)
  : SSTDataBase(outputParameters){
//    m_useCompression = outputParameters.find<bool>("compressed");
    // Announce this output object's name
  //Output& out      = Simulation_impl::getSimulationOutput();
  std::cout << "BOGUS" << std::endl;
  //Output out("", 0, 0, Output::STDOUT);
  //out.verbose(CALL_INFO, 1, 0, " : StatisticOutputTest enabled...\n");
  setStatisticOutputName("StatisticOutputTest");
}

bool StatisticOutputTest::checkOutputParameters(){
  return true;
}

void StatisticOutputTest::printUsage(){
  Output out("", 0, 0, Output::STDOUT);
  out.output(" : Usage - Sends all statistic output compilation test.\n");
}

void StatisticOutputTest::startOfSimulation(){
  std::cout << "startOfSimulation" << std::endl;
}

void StatisticOutputTest::endOfSimulation(){
  std::cout << "endOfSimulation" << std::endl;
}

void StatisticOutputTest::implStartOutputEntries(StatisticBase* statistic){
  std::cout << "implStartOutputEntries" << std::endl;
}

void StatisticOutputTest::implStopOutputEntries(){
  std::cout << "implStopOutputEntries" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, int32_t data){
  std::cout << "outputField-int" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, uint32_t data){
  std::cout << "outputField-uint" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, int64_t data){
  std::cout << "outputField-long" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, uint64_t data){
  std::cout << "outputField-ulong" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, float data){
  std::cout << "outputField-float" << std::endl;
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, double data){
  std::cout << "outputField-double" << std::endl;
}

};  // end SST::Statistics

// EOF
