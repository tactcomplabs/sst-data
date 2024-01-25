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
  : SSTDataBase(outputParameters), ppDb(nullptr) {

  const int Verbosity = outputParameters.find<int>("verbose", 0);
  out.init("statsqllite[:@p:@t]: ",
              Verbosity, 0, SST::Output::STDOUT);

  setStatisticOutputName("StatisticOutputSQLLite");
}

bool StatisticOutputSQLLite::checkOutputParameters(){

  // check to see if the user wants the help menu
  bool foundKey = false;
  getOutputParameters().find<std::string>("help", "1", foundKey);
  if ( true == foundKey ) { return false; }

  // read all the parameters
  dbfile = getOutputParameters().find<std::string>("dbfile", "Statistics.sql");

  int err = sqlite3_open( dbfile.c_str(), &ppDb );
  if( err != SQLITE_OK ){
    out.output( "Failed to open database file at %s with error code=%d\n",
                dbfile.c_str(), err );
  }

  return true;
}

void StatisticOutputSQLLite::printUsage(){
  out.output(" : Usage - Sends all statistic output compilation test.\n");
  out.output(" : help = Force Statistic Output to display usage\n");
  out.output(" : dfile = </path/to/Statistics.sql> - Database output file; default = ./db.sql\n");
}

void StatisticOutputSQLLite::startOfSimulation(){
  int err = sqlite3_open( dbfile.c_str(), &ppDb );
  if( err != SQLITE_OK ){
    out.output( "Failed to open database file at %s with error code=%d\n",
                dbfile.c_str(), err );
  }

  const std::string sql =
    "CREATE TABLE IF NOT EXISTS STATS( ComponentName TEXT NOT NULL, StatisticName TEXT NOT NULL, StatisticSubId TEXT, StatisticType TEXT NOT NULL, SimTime INTEGER, Rank INTEGER, Sum.u64 INTEGER, SumSQ.u64 INTEGER, Count.u64 INTEGER, Min.u64 INTEGER, Max.u64 INTEGER);";
  err = sqlite3_exec(ppDb, sql.c_str(), NULL, NULL, NULL);
}

void StatisticOutputSQLLite::endOfSimulation(){
  if( ppDb ){
    sqlite3_close(ppDb);
  }
}

void StatisticOutputSQLLite::implStartOutputEntries(StatisticBase* statistic){
  // retrieve the current statistic info
  curComponentName  = statistic->getCompName();
  curStatisticName  = statistic->getStatName();
  curStatisticSubId = statistic->getStatSubId();
  curStatisticType  = statistic->getStatTypeName();
}

void StatisticOutputSQLLite::implStopOutputEntries(){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         int32_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         uint32_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         int64_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         uint64_t data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         float data){
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         double data){
}

};  // end SST::Statistics

// EOF
