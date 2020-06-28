/**
 * @file genHoughMap.h
 * @author DeWei (curlywei@gmail.com)
 * @brief Generate Hough Map
 * @version 0.1
 * @date 2020-05-15
 * Technical report please refer to
 * https://hackmd.io/@BelleII-CDC-Trig-NTU-DeWei/r1BNYq89I
 * @copyright Copyright (c) 2020
 * 
*/

#ifndef GENHOUGHMAP_H_
# define GENHOUGHMAP_H_

#if defined(_WIN64) && defined(genHoughMap_EXPORTS)
#  if defined(LIB_INTERNAL)
#    define API_ATTRI __declspec (dllexport)
#  else
#    define API_ATTRI __declspec (dllimport)
#  endif
#endif

#if !defined(API_ATTRI)
#  define API_ATTRI
#endif
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cmath>
#include <string>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
static constexpr double PI2 = 2 * M_PI;
static constexpr double PI_div_2 =  M_PI / 2;

namespace CDCTrig {


/**
 * @brief 
 * Call machanical parameters of CDC in Belle-II
 * Regarding all of constant,
 * @see https://hackmd.io/@BelleII-CDC-Trig-NTU-DeWei/r1BNYq89I 
 * chapter 2. Constant
*/
struct SLConst2D {
 public:

/**
 * @brief 
 * Super layer Max count in the Belle II, it's 5
 * 
 */
static constexpr size_t kSLMaXCount2D = 5;

/**
 * @brief 
 * In the 2D-Triacker, only SL-0, SL-2, SL-4, SL-6, SL-8, those 5 layers be used.
 * This helper array could used in: for (auto&& i : kSLRange2D) ...
 * @example
 * for (const auto& i : kSLRange2D)
 *   std::cout <<  "SL_"<< i << ","
 * // result
 * // SL_0,SL_2,SL_4,SL_6,SL_8,
*/
static constexpr std::array<size_t,kSLMaXCount2D> kSLRange2D 
  { 0, 2, 4, 6, 8 };

/**
 * @brief 
 * The count of x scale(phi) in Hough Plane
*/
static constexpr size_t kHoughPlaneNX = 160;

/**
 * @brief 
 * The count of y scale(r) in Hough Plane
 */
static constexpr size_t kHoughPlaneNY = 16;

/**
 * @brief 
 * The minimun value of x in Hough Plane
*/
static constexpr double kHoughPlaneMinX = -M_PI;

/**
 * @brief 
 * The minimun value of y in Hough Plane
 */
static constexpr double kHoughPlaneMinY = 16;

/**
 * @brief 
 * The maximun value of x in Hough Plane
*/
static constexpr double kHoughPlaneMaxX = M_PI;

/**
 * @brief 
 * The maximun value of y in Hough Planee
 */
static constexpr double kHoughPlaneMaxY = 16;

static constexpr size_t kHoughPlaneRowSize = kHoughPlaneNX/4;


  /**
   * @brief 
   * Get the radius of the specific layer (input only even value 0~8)
   * @param SL_number 
   * The specific layer
   * @return 
   * The result of radius on the specific layer
  */
  static constexpr API_ATTRI double getRadiusSL(const size_t SL_number) {
    switch(SL_number) {
      case 0: return kRadiusEachSL2DSet.at(0);  
      case 2: return kRadiusEachSL2DSet.at(1);
      case 4: return kRadiusEachSL2DSet.at(2);
      case 6: return kRadiusEachSL2DSet.at(3);
      case 8: return kRadiusEachSL2DSet.at(4);
      default: return 0;
    }
  };

  /**
   * @brief 
   * Get the max number of TS on specific layer (input only even value 0~8)
   * @param SL_number 
   * The specific layer
   * @return 
   * The max number of TS of specific layer
  */  
  static constexpr API_ATTRI size_t getTSMaxCounSL(const size_t SL_number) {
    switch(SL_number) {
      case 0: return kTSMaxCountsEachSL2DSet.at(0);  
      case 2: return kTSMaxCountsEachSL2DSet.at(1);
      case 4: return kTSMaxCountsEachSL2DSet.at(2);
      case 6: return kTSMaxCountsEachSL2DSet.at(3);
      case 8: return kTSMaxCountsEachSL2DSet.at(4);
      default: return 0;
    }
  };

  SLConst2D() = delete;
  SLConst2D(const SLConst2D& ) = delete;
  SLConst2D operator=(const SLConst2D&) = delete;
  SLConst2D(SLConst2D&& ) = delete;
  SLConst2D& operator=(SLConst2D&&) = delete;

 private:
/**
 * @brief 
 * Radius of the specific layer
 * Members: { r_SL_0, r_SL_2, r_SL_4, r_SL_6, r_SL_8 }
*/
static constexpr std::array<double,kSLMaXCount2D> kRadiusEachSL2DSet
  { 19.8, 40.16, 62.00, 83.84, 105.68 };

/**
 * @brief 
 * Max count of trigger segment at specific layer
 * Members: { n_TS_SL_0, n_TS_SL_2, n_TS_SL_4, n_TS_SL_6, n_TS_SL_8 }
*/
static constexpr std::array<size_t,kSLMaXCount2D> kTSMaxCountsEachSL2DSet
  { 160, 192, 256, 320, 384 };
}; 

/**
 * @brief 
 * files name of HoughMapping result output 
**/
struct OutputFilesName {

  /**
   * @brief 
   * Construct a new Output Files Name object
   * 
   * @param result_subdir_name 
   * initialize subdir_name
   * 
   * @see 
   * subdir_name
   * 
   * @param result_TSIM_minus 
   * initialize TSIM_minus
   * 
   * @see 
   * TSIM_minus
   * 
   * @param result_TSIM_plus 
   * initialize TSIM_plus
   * 
   * @see 
   * TSIM_plus
   * 
   * @param result_minus_total_dat 
   * initialize minus_total_dat
   * 
   * @see 
   * minus_total_dat
   * 
   * @param result_plus_total_dat 
   * initialize plus_total_dat
   * 
   * @see 
   * plus_total_dat
   * 
   * @param result_hdl 
   * initialize hdl
   * @see hdl
   */
  template <
    typename T0,
    typename T1,
    typename T2,
    typename T3,
    typename T4,
    typename T5
  >
  OutputFilesName(
    T0&& result_subdir_name,
    T1&& result_TSIM_minus, 
    T2&& result_TSIM_plus,
    T3&& result_minus_total_dat, 
    T4&& result_plus_total_dat,
    T5&& result_hdl 
    ) : 
      subdir_name( std::forward<T0>(result_subdir_name) ),
      TSIM_minus( std::forward<T1>(result_TSIM_minus) ),
      TSIM_plus( std::forward<T2>(result_TSIM_plus) ),
      minus_total_dat( std::forward<T3>(result_minus_total_dat) ),
      plus_total_dat( std::forward<T4>(result_plus_total_dat) ),
      hdl( std::forward<T5>(result_hdl) ) {     
  }

  /**
   * @brief 
   * All of output files will output this subdirectory
  */
  const std::string subdir_name;

  /**
   * @brief 
   * Output file name of Minus TSIM file (.cc file)
  */
  const std::string TSIM_minus;

  /**
   * @brief 
   * Output file name of Plus TSIM file (.cc file)
  */
  const std::string TSIM_plus;

  /**
   * @brief 
   * Output file name of Minus Hough Map table (.dat file)
  */
  const std::string minus_total_dat;

  /**
   * @brief 
   * Output file name of Plus Hough Map table (.dat file)
  */  
  const std::string plus_total_dat;

  /**
   * @brief 
   * Output files name of HDL files
  */
  const std::string hdl;

  OutputFilesName() = delete;
};

/**
 * @brief 
 * Stored objects of output file.
 * This struct is used by class OutputFilesDAO.
 * All of files name is named and initialized by OutputFilesName
 * 
 * @see
 * OutputFilesName
 */
struct OutputFiles {
  std::ofstream minus_total_dat; 
  std::ofstream plus_total_dat; 
  std::ofstream TSIM_minus_cc; 
  std::ofstream TSIM_plus_cc;
  std::ofstream hdl_param;
  std::ofstream HoughMapping_hdl;
};

/**
 * @brief 
 * Data Access Object(DAO), which
 * create and get of ofstream with struct OutputFilesName. 
 * This object is global and unique(singleton pattern)
 * To invoke member of this object,
 * please use OutputFiles::instance(<OutputFilesName ofn>) 
 * 
 * @see 
 * OutputFilesName
 * 
 * @example
 * OutputFilesName ofn(...);
 * OutputFilesDAO::instance(ofn).chk_files_init_result();
 */
class OutputFilesDAO { 
 public: 
  /**
   * @brief 
   * 
   * @tparam T 
   * Inference type automatically
   * 
   * @param files_name please
   * input this struct: OutputFilesName
   * 
   * @return 
   * OutputFiles&& instance 
   * 
   * @example 
   * please see example OutputFilesDAO
   * 
   * @see 
   * OutputFilesDAO
  */
  template<typename T>
  API_ATTRI static auto&& instance(T&& files_name) {
    static OutputFilesDAO singleton( std::forward<T>(files_name) );
    return singleton;
  }

  /**
   * @brief 
   * Checking if initialization is OK
   * 
   * @return bool
   * true: initialization is OK
   * false: otherwise
   * 
   * @example 
   * if (!chk_files_init_result) return -1;
   */
  API_ATTRI bool chk_files_init_result();

  /**
   * @brief 
   * Write std::string to spceific file, 
   * must assing *which files* you write after writeToFile().
   * Regarding *which files*, please see content of struct OutputFiles.
   * 
   * @return OutputFiles&
   * 
   * @see 
   * OutputFiles
   * 
   * @example 
   * //write "content" to hdl file
   * OutputFilesDAO()::instance(ofn).writeToFile().hdl << "content";
  */
  API_ATTRI OutputFiles& writeToFile();
  
  /**
   * @brief 
   * Get filename of specific file,
   * must assing *name of which files* you write after getFileName().
   * Regarding *name of which files*, please see the content of struct OutputFilesName.
   * 
   * @return const OutputFilesName& 
   * 
   * @see
   * OutputFilesName
   * 
   * @example
   * std::string hdl_filename =
   *    OutputFilesDAO()::instance(ofn).getFileName().hdl;
   */
  API_ATTRI const OutputFilesName& getFileName() const;

  OutputFilesDAO() = delete;
  OutputFilesDAO(const OutputFilesDAO&) = delete;
  OutputFilesDAO& operator=(const OutputFilesDAO&) = delete;
  OutputFilesDAO(OutputFilesDAO&&) = delete;
  OutputFilesDAO& operator=(OutputFilesDAO&&) = delete;
 
 private:
  template <typename T>
  OutputFilesDAO(T&& files_name) : ofn_( std::forward<T>(files_name) ), of_() { 
    fs::create_directory(ofn_.subdir_name);
    of_.minus_total_dat.open(
      ofn_.subdir_name + "/" + ofn_.minus_total_dat
    );
    of_.plus_total_dat.open(
      ofn_.subdir_name + "/" + ofn_.plus_total_dat
    );
    of_.TSIM_minus_cc.open(
      ofn_.subdir_name + "/" + ofn_.TSIM_minus
    );
    of_.TSIM_plus_cc.open(
      ofn_.subdir_name + "/" + ofn_.TSIM_plus
    );
    of_.hdl_param.open(
      ofn_.subdir_name + "/" + ofn_.hdl + "_param.vhd"
    );
    of_.HoughMapping_hdl.open(
      ofn_.subdir_name + "/" + ofn_.hdl + ".vhd"
    );
  }
  OutputFilesName ofn_;
  OutputFiles of_;
};

/**
 * @brief 
 * Generate Hough-Curve-Space(HSC) from CDC-Ploar-Space in compile time
 * 
 * @tparam SL_n 
 *  The number on spceific Super Layer, which in {0, 2, 4, 6, 8}
 */
template <size_t SL_n>
struct OneOverRHCSFromCDCPolar { 
 public: 
  constexpr OneOverRHCSFromCDCPolar() {  
    constexpr double dou_n_TS_SL = static_cast<double>( SLConst2D::getTSMaxCounSL(SL_n) );
    constexpr double peak_to_peak_x = SLConst2D::kHoughPlaneMaxX - SLConst2D::kHoughPlaneMinX;
    constexpr double const_term_r = 2.0 / SLConst2D::getRadiusSL(SL_n);
    constexpr double const_term_phi = peak_to_peak_x / dou_n_TS_SL;
    for (size_t i = 0; i < SLConst2D::getTSMaxCounSL(SL_n) ; ++i) {
      double dou_i = static_cast<double>(i);
      r.at(i) = const_term_r * sin( dou_i * const_term_phi );
      phi.at(i) = const_term_phi * dou_i + SLConst2D::kHoughPlaneMinX;
    }
  }
  std::array<double, SLConst2D::getTSMaxCounSL(SL_n)> r;
  std::array<double, SLConst2D::getTSMaxCounSL(SL_n)> phi;
};



API_ATTRI std::string getHDLParamContent(const std::string& file_name);

}//namespace CDCTrig 
#endif //GENHOUGHMAP_H_
