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
  tablename = getOutputParameters().find<std::string>("tablename", "STATS_");
  tablename += std::to_string(rank);

  int err = sqlite3_open( dbfile.c_str(), &ppDb );
  if( err != SQLITE_OK ){
    out.output( "Failed to open database file at %s with error code=%d\n",
                dbfile.c_str(), err );
  }

  return true;
}

void StatisticOutputSQLLite::printUsage(){
  out.output(" : Usage - Sends all statistic output to SQLLite3.\n");
  out.output(" : help = Force Statistic Output to display usage\n");
  out.output(" : dbfile = </path/to/Statistics.sql> - Database output file; default = ./db.sql\n");
  out.output(" : tablename = TableNameX - Database table name w/o rank info; default = STATS_\n");
}

void StatisticOutputSQLLite::startOfSimulation(){
  int err = sqlite3_open( dbfile.c_str(), &ppDb );
  if( err != SQLITE_OK ){
    out.output( "Failed to open database file at %s with error code=%d\n",
                dbfile.c_str(), err );
  }

  // init the output buffer as string objects
  for( auto it = getFieldInfoArray().begin();
       it != getFieldInfoArray().end(); it++ ){
    outBuf.push_back(std::string(""));
  }

  // drop the previous table
  std::string sql = "DROP TABLE IF EXISTS " + tablename + ";";
  err = sqlite3_exec(ppDb, sql.c_str(), NULL, NULL, NULL);
  if( err != SQLITE_OK ){
    out.output( "Failed to drop table=%s in database=%s with error code=%d\nSQL=%s\n",
                tablename.c_str(), dbfile.c_str(), err, sql.c_str() );
  }

  // build the table creation sql
  sql =
    "CREATE TABLE IF NOT EXISTS " + tablename +
    " ( ComponentName TEXT NOT NULL, StatisticName TEXT NOT NULL, StatisticSubId TEXT, StatisticType TEXT NOT NULL,";

  StatisticFieldInfo* statField = nullptr;
  auto it = getFieldInfoArray().begin();
  while( it != getFieldInfoArray().end() ){
    statField = *it;
    sql += statField->getFieldName();
    sql += "_";
    sql += getFieldTypeShortName(statField->getFieldType());
    sql += " ";
    sql += fieldToSQLType(getFieldTypeShortName(statField->getFieldType()));

    it++;
    if( it != getFieldInfoArray().end() ){
      sql += ",";
    }else{
      sql += ");";
    }
  }
  err = sqlite3_exec(ppDb, sql.c_str(), NULL, NULL, NULL);
  if( err != SQLITE_OK ){
    out.output( "Failed to create table=%s in database=%s with error code=%d\nSQL=%s\n",
                tablename.c_str(), dbfile.c_str(), err, sql.c_str() );
  }
}

std::string StatisticOutputSQLLite::fieldToSQLType(std::string Type){
  if( (Type == "u64") ||
      (Type == "u32") ||
      (Type == "s64") ||
      (Type == "s32")){
    return std::string("INTEGER");
  }else if( (Type == "float") ||
            (Type == "double") ){
    return std::string("REAL");
  }
  return std::string("TEXT");
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

  if( curStatisticSubId.length() == 0 ){
    curStatisticSubId = curStatisticName;
  }

  for( size_t i = 0; i < getFieldInfoArray().size(); i++ ) {
    outBuf[i] = "0";
  }
}

void StatisticOutputSQLLite::implStopOutputEntries(){
  std::string sql =
    "INSERT INTO " + tablename +
    " ( ComponentName, StatisticName, StatisticSubId, StatisticType,";

  StatisticFieldInfo* statField = nullptr;
  auto it = getFieldInfoArray().begin();
  while( it != getFieldInfoArray().end() ){
    statField = *it;
    sql += statField->getFieldName();
    sql += "_";
    sql += getFieldTypeShortName(statField->getFieldType());

    it++;
    if( it != getFieldInfoArray().end() ){
      sql += ",";
    }else{
      sql += ") ";
    }
  }

  const std::string quote("'");

  sql+= " VALUES(" +
    quote + curComponentName + "'," +
    quote + curStatisticName + "'," +
    quote + curStatisticSubId + "'," +
    quote + curStatisticType + "',";

  it = getFieldInfoArray().begin();
  unsigned idx = 0;
  while( it != getFieldInfoArray().end() ){
    sql += outBuf[idx];
    idx++;

    it++;
    if( it != getFieldInfoArray().end() ){
      sql += ",";
    }else{
      sql += ");";
    }
  }
  int err = sqlite3_exec(ppDb, sql.c_str(), NULL, NULL, NULL);
  if( err != SQLITE_OK ){
    out.output( "Failed to commit data to database=%s with error code=%d;\nSQL=%s\n",
                dbfile.c_str(), err, sql.c_str() );
  }
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         int32_t data){
  outBuf[fieldHandle] = format_string("%" PRId32, data);
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         uint32_t data){
  outBuf[fieldHandle] = format_string("%" PRIu32, data);
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         int64_t data){
  outBuf[fieldHandle] = format_string("%" PRId64, data);
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         uint64_t data){
  outBuf[fieldHandle] = format_string("%" PRIu64, data);
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         float data){
  outBuf[fieldHandle] = format_string("%f", (double)(data));
}

void StatisticOutputSQLLite::outputField(fieldHandle_t fieldHandle,
                                         double data){
  outBuf[fieldHandle] = format_string("%f", data);
}

};  // end SST::Statistics

// EOF
