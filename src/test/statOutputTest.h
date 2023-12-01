#pragma once
#ifndef STAT_OUTPUT_TEST_H_
#define STAT_OUTPUT_TEST_H_

#include <sst/core/sst_types.h>
#include <sst/core/statapi/statoutput.h>

namespace SST::Statistics{

class SSTDataBase : public StatisticFieldsOutput{
public:
  SST_ELI_DECLARE_BASE(SST::Statistics::SSTDataBase);
  SSTDataBase(Params& outputParameters)
    : StatisticFieldsOutput(outputParameters){}
protected:
  bool checkOutputParameters()  = 0;
  void printUsage() = 0;
  void startOfSimulation() = 0;
  void endOfSimulation() = 0;
  void implStartOutputEntries(StatisticBase* statistic) = 0;
  void implStopOutputEntries() = 0;

  void outputField(fieldHandle_t fieldHandle, int32_t data) = 0;
  void outputField(fieldHandle_t fieldHandle, uint32_t data) = 0;
  void outputField(fieldHandle_t fieldHandle, int64_t data) = 0;
  void outputField(fieldHandle_t fieldHandle, uint64_t data) = 0;
  void outputField(fieldHandle_t fieldHandle, float data) = 0;
  void outputField(fieldHandle_t fieldHandle, double data) = 0;

  virtual bool acceptsGroups() const override = 0;

  SSTDataBase() { ; }

};

class StatisticOutputTest : public SSTDataBase{
public:
  SST_ELI_REGISTER_DERIVED(
    SSTDataBase,
    StatisticOutputTest,
    "sstdata",
    "statoutputtest",
    SST_ELI_ELEMENT_VERSION(1,0,0),
    "Output test"
  )

  StatisticOutputTest(Params& outputParameters);

protected:
  bool checkOutputParameters() override;

  void printUsage() override;

  void startOfSimulation() override;

  void endOfSimulation() override;

  void implStartOutputEntries(StatisticBase* statistic) override;

  void implStopOutputEntries() override;

  void outputField(fieldHandle_t fieldHandle, int32_t data) override;
  void outputField(fieldHandle_t fieldHandle, uint32_t data) override;
  void outputField(fieldHandle_t fieldHandle, int64_t data) override;
  void outputField(fieldHandle_t fieldHandle, uint64_t data) override;
  void outputField(fieldHandle_t fieldHandle, float data) override;
  void outputField(fieldHandle_t fieldHandle, double data) override;

  bool acceptsGroups() const override { return true; }

  StatisticOutputTest()  : SSTDataBase() { ; }

private:
};  // end StatOutputTest
};  // end SST::Statistics

#endif
// EOF
