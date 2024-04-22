//
// _STATSQLLITE_H_
//
// Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STAT_SQLLITE_H_
#define _STAT_SQLLITE_H_

#include <variant>

// -- SST-DATA Headers
#include "sstdata.h"

// -- SQLLITE3 Headers
#include "sqlite3.h"

namespace SST::Statistics{

class StatisticOutputSQLLite : public SSTDataBase{
public:
  SST_ELI_REGISTER_DERIVED(
    SSTDataBase,
    StatisticOutputSQLLite,
    "sstdata",
    "statsqllite",
    SST_ELI_ELEMENT_VERSION(1,0,0),
    "SQLLite3 Statistics Output"
  )

  SST_ELI_DOCUMENT_PARAMS(
    { "dbfile",     "database output file",     "Statistics.sql"},
  )

  StatisticOutputSQLLite(Params& outputParameters);

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

  StatisticOutputSQLLite()  : SSTDataBase() { ; }

private:

  // private methods

  /// StatisticOutputSQLLite: converts an SST FieldType to an SQL type
  std::string fieldToSQLType(std::string Type);

  // parameters
  std::string dbfile;                   ///< database file

  // private data members
  Output out;                           ///< sst output handler
  sqlite3 *ppDb;                        ///< database handler

  std::vector<std::string> outBuf;      ///< output buffer
  std::string curComponentName;         ///< component name
  std::string curStatisticName;         ///< statistic name
  std::string curStatisticSubId;        ///< statistic subid
  std::string curStatisticType;         ///< statistic type

};  // end StatOutputTest
};  // end SST::Statistics

#endif
// EOF
