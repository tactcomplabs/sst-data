//
// _STATARROW_CC_
//
// Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "statArrow.h"

namespace SST::Statistics{

StatisticOutputArrow::StatisticOutputArrow(Params& outputParameters)
  : SSTDataBase(outputParameters), batches(), builders(), schemas() {
  const int Verbosity = outputParameters.find<int>("verbose", 0);
  out.init("statarrow[:@p:@t]: ",
              Verbosity, 0, SST::Output::STDOUT);
  setStatisticOutputName("StatisticOutputArrow");
}

bool StatisticOutputArrow::checkOutputParameters(){
  return true;
}

void StatisticOutputArrow::printUsage(){
  out.output(" : Usage - Sends all statistic output to Apache Arrow\n");
  out.output(" : help = Force Statistic Output to display usage\n");
}

void StatisticOutputArrow::startOfSimulation(){
}

void StatisticOutputArrow::endOfSimulation(){
}

static bool fieldTypeToArrowField(std::vector<StatisticOutputArrow::ArrowBuilders> & builders,
                                  std::vector<std::shared_ptr<arrow::Field>> & fields,
                                  std::string const& field_str,
                                  const char * ft_cstr) {
    if(std::strncmp(ft_cstr, "int32_t", 7) == 0) {
        fields.push_back(arrow::field(field_str, arrow::int32()));
        builders.push_back(arrow::Int32Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "int64_t", 7) == 0) {
        fields.push_back(arrow::field(field_str, arrow::int64()));
        builders.push_back(arrow::Int64Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "uint32_t", 8) == 0) {
        fields.push_back(arrow::field(field_str, arrow::uint32()));
        builders.push_back(arrow::UInt32Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "uint64_t", 8) == 0) {
        fields.push_back(arrow::field(field_str, arrow::uint64()));
        builders.push_back(arrow::UInt64Builder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "float", 5) == 0) {
        fields.push_back(arrow::field(field_str, arrow::float32()));
        builders.push_back(arrow::FloatBuilder{});
        return true;
    }
    else if(std::strncmp(ft_cstr, "double", 6) == 0) {
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
void StatisticOutputArrow::implStartOutputEntries(StatisticBase* statistic){

  const std::string m_currentComponentName  = statistic->getCompName();
  const std::string m_currentStatisticName  = statistic->getStatName();
  const std::string m_currentStatisticSubId = statistic->getStatSubId();
  const std::string m_currentStatisticType  = statistic->getStatTypeName();

#if 0
  std::cout << "implStartOutputEntries" << std::endl
            << 1 << m_currentComponentName << '\t'
            << 2 << m_currentStatisticName << '\t'
            << 3 << m_currentStatisticSubId << '\t'
            << 4 << m_currentStatisticType << std::endl;
#endif

  auto arr = getFieldInfoArray();
  SST::Statistics::StatisticFieldInfo * val = nullptr;
  fieldType_t ft;
  bool rv = false;

  std::vector<std::shared_ptr<arrow::Field>> fields;
  fields.reserve(arr.size()-1);
  builders.reserve(arr.size()-1);
  for(FieldInfoArray_t::iterator itr = arr.begin(); itr != arr.end(); itr++) {
    val = *itr;
    ft = val->getFieldType();
    StatisticFieldTypeBase const* ptr = StatisticFieldTypeBase::getField(ft);
    rv = fieldTypeToArrowField(builders,
                               fields,
                               val->getFieldName(),
                               ptr->fieldName());
  }

  schemas.emplace_back(arrow::schema(fields));
}

void StatisticOutputArrow::implStopOutputEntries() {
  std::vector<std::shared_ptr<arrow::Array>> arrays;
  arrays.reserve(builders.size());

  int64_t max_length = 0;
  for(std::size_t i = 0; i < builders.size(); ++i) {
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

     max_length = std::max(max_length, arrays[i]->length());
  };

  auto res = arrow::RecordBatch::Make(schemas.back(), max_length, arrays);
  batches.emplace_back(res);
  builders.clear();
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, int32_t data){
  if(std::holds_alternative<arrow::Int32Builder>(builders[fieldHandle])) {
    std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, uint32_t data){
  if(std::holds_alternative<arrow::UInt32Builder>(builders[fieldHandle])) {
    std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, int64_t data){
  if(std::holds_alternative<arrow::Int64Builder>(builders[fieldHandle])) {
    std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, uint64_t data){
  if(std::holds_alternative<arrow::UInt64Builder>(builders[fieldHandle])) {
    std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, float data){
  if(std::holds_alternative<arrow::FloatBuilder>(builders[fieldHandle])) {
    std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

void StatisticOutputArrow::outputField(fieldHandle_t fieldHandle, double data){
  if(std::holds_alternative<arrow::DoubleBuilder>(builders[fieldHandle])) {
    std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(data);
  }else {
    switch(builders[fieldHandle].index()) {
        case 1:
           std::get<arrow::Int32Builder>(builders[fieldHandle]).Append(static_cast<std::int32_t>(data));
        break;
        case 2:
           std::get<arrow::Int64Builder>(builders[fieldHandle]).Append(static_cast<std::int64_t>(data));
        break;
        case 3:
           std::get<arrow::UInt32Builder>(builders[fieldHandle]).Append(static_cast<std::uint32_t>(data));
        break;
        case 4:
           std::get<arrow::UInt64Builder>(builders[fieldHandle]).Append(static_cast<std::uint64_t>(data));
        break;
        case 5:
           std::get<arrow::FloatBuilder>(builders[fieldHandle]).Append(static_cast<float>(data));
        break;
        case 6:
           std::get<arrow::DoubleBuilder>(builders[fieldHandle]).Append(static_cast<double>(data));
        break;
        default:
           std::cerr << "error\t" << builders[fieldHandle].index() << ' ' << fieldHandle << std::endl;
        break;
    }
  }
}

};  // end SST::Statistics

// EOF
