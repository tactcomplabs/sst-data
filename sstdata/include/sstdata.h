//
// _SSTDATA_H_
//
// Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _SSTDATA_H_
#define _SSTDATA_H_

#include <sst/core/sst_types.h>
#include <sst/core/statapi/statoutput.h>
#include <sst/core/stringize.h>

#ifdef SSTDATA_MPI
#include <mpi.h>
#endif

namespace SST::Statistics{

class SSTDataBase : public StatisticFieldsOutput{
public:
  SST_ELI_DECLARE_BASE(SST::Statistics::SSTDataBase);

  /// SSTDataBase: constructor
  SSTDataBase(Params& outputParameters)
    : StatisticFieldsOutput(outputParameters), isMPI(false),
      rank(0), numRanks(1){
#ifdef SSTDATA_MPI
    int isInit  = -1;
    MPI_Initialized(&isInit);
    if( isInit ){
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_rank(MPI_COMM_WORLD, &numRanks);
    }
#endif
  }

protected:
  /// SSTDataBase: pure virtual checkOutputParameters
  bool checkOutputParameters()  override = 0;

  /// SSTDataBase: pure virtual printUsage
  void printUsage() override = 0;

  /// SSTDataBase: pure virtual startOfSimulation
  void startOfSimulation() override = 0;

  /// SSTDataBase: pure virtual endOfSimulation
  void endOfSimulation() override = 0;

  /// SSTDataBase: pure virtual implStartOutputEntries
  void implStartOutputEntries(StatisticBase* statistic) override = 0;

  /// SSTDataBase: pure virtual implStopOutputEntries
  void implStopOutputEntries() override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, int32_t data) override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, uint32_t data) override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, int64_t data) override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, uint64_t data) override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, float data) override = 0;

  /// SSTDataBase: pure virtual outputField
  void outputField(fieldHandle_t fieldHandle, double data) override = 0;

  /// SSTDataBase: pure virtual acceptsGroups
  virtual bool acceptsGroups() const override = 0;

  /// SSTDataBase: constructor
  SSTDataBase() { ; }

  // Protected data members
  bool isMPI;
  int rank;
  int numRanks;

};  // class SSTDataBase
};  // nameapce SST::Statistics

#endif  // _SSTDATA_H_

// EOF
