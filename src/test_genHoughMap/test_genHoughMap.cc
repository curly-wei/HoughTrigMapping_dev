#include "../genHoughMap/genHoughMap.h"

#include <iostream>
#include <string>

int main(){    
  // 1. Specific name of output files 
  constexpr char default_subdir_name[] = "result";
  constexpr char default_M_CC[] = "HoughMappingMinus.cc";
  constexpr char default_P_CC[] = "HoughMappingPlus.cc";
  constexpr char default_data_M[] = "minus_total.dat";
  constexpr char default_data_P[] = "plus_total.dat";
  constexpr char default_HDL_prefix[] = "UT4_HoughMapping";
  
  CDCTrig::OutputFilesName ofn( 
    default_subdir_name,
    default_M_CC,
    default_P_CC,
    default_data_M,
    default_data_P,
    default_HDL_prefix
  );

  // 2. Check if files initialization is successful
  if ( !CDCTrig::OutputFilesDAO::instance(ofn).chk_files_init_result() ) {
    std::cerr << "Init fail, \
                  directory of result files \
                  or result files\
                  hasn't been create" << std::endl;
    return -1;
  }

  // 3. Write (singleton) files with alogrithm
  const std::string& hdl_filename = 
    CDCTrig::OutputFilesDAO::instance(ofn).getFileName().hdl;
  
  CDCTrig::
  OutputFilesDAO::instance(ofn).writeToFile().hdl_param <<
    CDCTrig::getHDLParamContent(hdl_filename) <<
  std::endl;      


}