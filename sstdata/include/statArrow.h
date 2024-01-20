//
// _STATARROW_H_
//
// Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STAT_ARROW_H_
#define _STAT_ARROW_H_

// -- CXX Headers
#include <variant>
#include <cstring>

// -- SST-DATA Headers
#include "sstdata.h"

// -- Arrow Headers
#include <arrow/type.h>
#include <arrow/api.h>

namespace SST::Statistics{

class StatisticOutputArrow : public SSTDataBase{
public:
  SST_ELI_REGISTER_DERIVED(
    SSTDataBase,
    StatisticOutputArrow,
    "sstdata",
    "statarrow",
    SST_ELI_ELEMENT_VERSION(1,0,0),
    "Apache Arrow Statistics Output"
  )

  StatisticOutputArrow(Params& outputParameters);

  using ArrowBuilders = std::variant<
    std::monostate,
    arrow::Int32Builder,
    arrow::Int64Builder,
    arrow::UInt32Builder,
    arrow::UInt64Builder,
    arrow::FloatBuilder,
    arrow::DoubleBuilder,
    arrow::NullBuilder
  >;

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

  StatisticOutputArrow()  : SSTDataBase() { ; }

private:
  std::vector<std::shared_ptr<arrow::RecordBatch>> batches;
  std::vector<ArrowBuilders> builders;
  std::vector<std::shared_ptr<arrow::Schema>> schemas;

};  // end StatOutputTest
};  // end SST::Statistics

#endif
// EOF
