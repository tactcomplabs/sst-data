#include <cstring>
#include <sst/core/sst_config.h>
#include <sst/core/stringize.h>
#include "statOutputTest.h"
//#include <sst/core/simulation_impl.h>

namespace SST::Statistics{

StatisticOutputTest::StatisticOutputTest(Params& outputParameters)
  : SSTDataBase(outputParameters), batches(), builders(), schemas() {
  //m_useCompression = outputParameters.find<bool>("compressed");
  //Announce this output object's name
  //Output& out      = Simulation_impl::getSimulationOutput();
  //std::cout << "BOGUS" << std::endl;
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

static bool fieldTypeToArrowField(std::vector<StatisticOutputTest::ArrowBuilders> & builders, std::vector<std::shared_ptr<arrow::Field>> & fields, std::string const& field_str, const char * ft_cstr) {
    if(std::strncmp(ft_cstr, "int32_t", 7)) {
        fields.push_back(arrow::field(field_str, arrow::int32()));
        builders.push_back(arrow::Int32Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "int64_t", 7)) {
        fields.push_back(arrow::field(field_str, arrow::int64()));
        builders.push_back(arrow::Int64Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "uint32_t", 8)) {
        fields.push_back(arrow::field(field_str, arrow::uint32()));
        builders.push_back(arrow::UInt32Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "uint64_t", 8)) {
        fields.push_back(arrow::field(field_str, arrow::uint64()));
        builders.push_back(arrow::UInt64Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "float", 5)) {
        fields.push_back(arrow::field(field_str, arrow::float32()));
        builders.push_back(arrow::FloatBuilder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "double", 6)) {
        fields.push_back(arrow::field(field_str, arrow::float64()));
        builders.push_back(arrow::DoubleBuilder{});
        return true;
    }

    fields.push_back(arrow::field(field_str, arrow::null()));
    builders.push_back(arrow::NullBuilder{});
    return false;
};

// https://arrow.apache.org/docs/python/api/datatypes.html
//
// https://arrow.apache.org/cookbook/cpp/create.html#create-arrays-from-standard-c
//
void StatisticOutputTest::implStartOutputEntries(StatisticBase* statistic){

  const std::string m_currentComponentName  = statistic->getCompName();
  const std::string m_currentStatisticName  = statistic->getStatName();
  const std::string m_currentStatisticSubId = statistic->getStatSubId();
  const std::string m_currentStatisticType  = statistic->getStatTypeName();

  std::cout << "implStartOutputEntries" << std::endl
            << 1 << m_currentComponentName << '\t'
            << 2 << m_currentStatisticName << '\t'
            << 3 << m_currentStatisticSubId << '\t'
            << 4 << m_currentStatisticType << std::endl;

  auto arr = getFieldInfoArray();
  SST::Statistics::StatisticFieldInfo * val = nullptr;
  fieldType_t ft;
  bool rv = false;

  std::vector<std::shared_ptr<arrow::Field>> fields;
  fields.reserve(arr.size());
  builders.reserve(arr.size());

  for(FieldInfoArray_t::iterator itr = arr.begin(); itr != arr.end(); itr++) {
    val = *itr;
    ft = val->getFieldType();
    StatisticFieldTypeBase const* ptr = StatisticFieldTypeBase::getField(ft);
    rv = fieldTypeToArrowField(builders, fields, val->getFieldName(), ptr->fieldName());
    //std::cout << "***\t" << val->getStatName() << ' ' << val->getFieldName() << ' ' << val->getFieldUniqueName() << ' ' << ptr->fieldName() << ' ' << ptr->shortName() << std::endl;
  }
  
  schemas.emplace_back(arrow::schema(fields));
}

void StatisticOutputTest::implStopOutputEntries() {
  std::cout << "implStopOutputEntries" << std::endl;

  std::vector<std::shared_ptr<arrow::Array>> arrays;
  arrays.reserve(builders.size());

  int64_t max_length = 0;
  for(auto i = 0; i < builders.size(); ++i) {
     max_length = std::max(max_length, arrays[i]->length());

     if(std::holds_alternative<arrow::Int32Builder>(builders[i])) {
       arrays.push_back(std::get<arrow::Int32Builder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::Int64Builder>(builders[i])) {
       arrays.push_back(std::get<arrow::Int64Builder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::UInt32Builder>(builders[i])) {
       arrays.push_back(std::get<arrow::UInt32Builder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::UInt64Builder>(builders[i])) {
       arrays.push_back(std::get<arrow::UInt64Builder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::FloatBuilder>(builders[i])) {
       arrays.push_back(std::get<arrow::FloatBuilder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::DoubleBuilder>(builders[i])) {
       arrays.push_back(std::get<arrow::DoubleBuilder>(builders[i]).Finish().ValueOrDie());
     }
     else if(std::holds_alternative<arrow::NullBuilder>(builders[i])) {
       arrays.push_back(std::get<arrow::NullBuilder>(builders[i]).Finish().ValueOrDie());
     }
  };

  auto res = arrow::RecordBatch::Make(schemas.back(), max_length, arrays);
  batches.emplace_back(res);
  builders.clear();
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, int32_t data){
  std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(data);
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, uint32_t data){
  std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(data);
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, int64_t data){
  std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(data);
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, uint64_t data){
  std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(data);
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, float data){
  std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(data);
}

void StatisticOutputTest::outputField(fieldHandle_t fieldHandle, double data){
  std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(data);
}

};  // end SST::Statistics

// EOF
