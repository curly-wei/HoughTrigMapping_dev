#define LIB_INTERNAL

#include "genHoughMap.h"

#include <string>
#include <utility>
#include <sstream>
#include <fstream>

namespace CDCTrig {

API_ATTRI bool OutputFilesDAO::chk_files_init_result() {
  return 
    of_.minus_total_dat.is_open() &&
    of_.plus_total_dat.is_open() &&
    of_.TSIM_minus_cc.is_open() &&
    of_.TSIM_plus_cc.is_open() &&
    of_.hdl_param.is_open() &&
    of_.HoughMapping_hdl.is_open();   
};

API_ATTRI OutputFiles& OutputFilesDAO::writeToFile() {
  return of_;
}

API_ATTRI const OutputFilesName& OutputFilesDAO::getFileName() const {
  return ofn_;
}

static constexpr char kHdlHeader[] =
"library ieee; \n\
use ieee.std_logic_1164.all; \n\
use ieee.std_logic_unsigned.all;\n\
use ieee.std_logic_misc.all; \n\n"
;

API_ATTRI std::string getHDLParamContent(const std::string& file_name) {  
  
  std::stringstream content;

  content <<
  kHdlHeader <<
  "package " << file_name << "_param" << " is \n" <<
  "\tconstant MAX_SL_COUNT_2D_k : integer := " << SLConst2D::kSLMaXCount2D <<"; \n" <<
  "\tconstant MAX_ROW_COUNT_2D_k : integer := " << SLConst2D::kHoughPlaneNY <<";" << "\n";
  
  for(const auto& i : SLConst2D::kSLRange2D) {
    content << 
    "\tconstant N_TS_SL_"<< i <<"_k : integer := " << SLConst2D::getTSMaxCounSL(i) <<"; \n"; 
  }
  content << "\n" <<
  "\tconstant I_dont_know_k : integer := " << SLConst2D::kHoughPlaneRowSize <<"; \n\n" <<  
  "\ttype TS_Data_t is record " << "\n";
  
  for(const auto& i : SLConst2D::kSLRange2D) {
    content << 
    "\t\tSL"<< i <<" : std_logic_vector( ( N_TS_SL_" << i <<" -1) downto 0 ); \n"; 
  }  
  content << 
  "\tend record TS_Data_t;" << "\n";

  content << 
  "\ttype row_data is array ( 0 to (MAX_ROW_COUNT_2D_k-1) ) of \n" <<
  "\t\tstd_logic_vector( (I_dont_know_k -1) downto 0 ); \n" <<
  "\ttype HoughMapping_Result_t is record" << "\n";
  for(const auto& i : SLConst2D::kSLRange2D) {
    content << 
    "\t\tSL"<< i <<" : row_data\n"; 
  }  
  content << 
  "\tend record HoughMapping_Result_t;\n" <<
  "end package " << file_name << "_param" << std::endl;

  return std::move( content.str() );
}

void HCSClusterSlopeRule(double r_present, double r_next) {
  if (r_present < r_next) { // minus
    
  } else if (r_present < r_next) { // plus
    
  }
}



} //namespace CDCTrig 
