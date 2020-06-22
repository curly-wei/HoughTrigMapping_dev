#define TRG_SHORT_NAMES

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <math.h>
#include <iomanip>
#include <filesystem>
namespace fs = std::filesystem; 
#include "Utilities.h"

using namespace std;
using namespace Belle2;

#define NAME          "HoughMapping"
#define VERSION       "version 0.02"

void printHeader(ofstream& out, const string function);
void superLayer(const unsigned id);
void printMapping(ofstream& out, vector<vector<vector<int>>>& HPcell);

//Hough Plane Parameter
// free parameters
//pt range
const double minPt = 0.3;
//number of cells
const unsigned nX = 160;
const unsigned nY = 34;
//vertical shift (up / down / off: +1 / -1 / 0)
const int shiftSign = 0;
//extra cells on each side for quarter plane in FPGA
const int extraXleft = 2;
const int extraXright = 4;
//starting index for quarter plane in FPGA
const int x0 = 100;

// derived values
//Phi Range (X axis of Hough plane)
const double minX = -M_PI;
const double maxX = M_PI;
const double dx = (maxX - minX) / nX;
//limited range for UT3 (quarter detector with overlap, in units of cells)
const unsigned xL = x0 - extraXleft;
const unsigned x1 = x0 + nX / 4;
const unsigned xR = x1 + extraXright;
//1/(r/cm) Range (Y axis of Hough plane)
const double maxYsym = 29.9792458 * 1.5e-4 / minPt;
//shift 1/r Range by one quarter cell
const double minY = -maxYsym + shiftSign * maxYsym / nY / 2;
const double maxY = maxYsym + shiftSign * maxYsym / nY / 2;
const double dy = (maxY - minY) / nY;

const std::string resut_subdir_name("result");

// table for fast simulation
vector<vector<vector<int>>> HPcell1(nY, vector<vector<int>>(nX, vector<int>()));
ofstream output1( resut_subdir_name + "/total_priority1.dat");

vector<vector<vector<int>>> HPcell2(nY, vector<vector<int>>(nX, vector<int>()));
ofstream output2( resut_subdir_name + "/total_priority2.dat");

//...C++ for TSIM...
ofstream outcc( resut_subdir_name + "/HoughMapping.cc");

int
main(int, char**)
{
  fs::create_directories(resut_subdir_name);
  cout << NAME << " ... " << VERSION << endl;
  cout << "input parameters:" << endl;
  cout << "pt > " << minPt << ", " << nX << " x " << nY << " cells" << endl;
  cout << "limits: x in [" << minX << ", " << maxX << "], y in [" << minY << ", " << maxY << "]" << endl;
  cout << "x index range for FPGA: " << xL << " to " << xR << endl;

  printHeader(outcc, "HoughMapping");

  for (unsigned isl = 0; isl < 9; isl += 2)
    superLayer(isl);

  printMapping(output1, HPcell1);
  printMapping(output2, HPcell2);

  outcc << endl << "}" << endl;
  outcc.close();
}

void
printHeader(ofstream& out, const string function)
{
  //...Date...
  string ts = TRGUtil::dateString();

  out << "// This file is generated by " << NAME << "(" << VERSION << ")" << endl;
  out << "// " << ts << endl << endl;
  out << "#define TRGCDC_SHORT_NAMES" << endl;
  out << "#include \"trg/trg/State.h\"" << endl;
  out << "#include \"trg/cdc/Tracker2D.h\"" << endl;
  out << "using namespace std;" << endl;
  out << "using namespace Belle2;" << endl;
  out << "void" << endl;
  out << "TCTracker2D::" << function << "(void) {" << endl;

  out << "    //...TS hit map..." << endl;
  out << "    TRGState SL0_TS = _ts.subset(0, 160);" << endl;
  out << "    TRGState SL2_TS = _ts.subset(160, 192);" << endl;
  out << "    TRGState SL4_TS = _ts.subset(160 + 192, 256);" << endl;
  out << "    TRGState SL6_TS = _ts.subset(160 + 192 + 256, 320);" << endl;
  out << "    TRGState SL8_TS = _ts.subset(160 + 192 + 256 + 320, 384);" << endl;

  out << "    //...Hough cells..." << endl;

  for (unsigned isl = 0; isl < 9; isl += 2) {
    for (unsigned iy = 1; iy < nY + 1; iy++) {
      out << "    TRGState SL" << to_string(isl) << "_row"
          << to_string(iy) << "(" << to_string(nX) << ");" << endl;
    }
  }
}

void
superLayer(const unsigned SL)
{
  //Radius of SL_center cell and 2nd priority
  double r_SL = 0;
  double r_SL2 = 0;
  //Number of TS each SL
  int N_TS_SL = 0;
  //Number of TS that cross the selected quarter of the Hough plane
  // (commented at the moment to stay compatible with TSF)
  //int N_TS_lim = 0;

  if (SL == 0) {
    r_SL = 19.8;
    r_SL2 = 20.8;
    N_TS_SL = 160;
    //N_TS_lim = 55;
  } else if (SL == 2) {
    r_SL = 40.16;
    r_SL2 = 41.98;
    N_TS_SL = 192;
    //N_TS_lim = 76;
  } else if (SL == 4) {
    r_SL = 62.0;
    r_SL2 = 63.82;
    N_TS_SL = 256;
    //N_TS_lim = 116;
  } else if (SL == 6) {
    r_SL = 83.84;
    r_SL2 = 85.66;
    N_TS_SL = 320;
    //N_TS_lim = 165;
  } else if (SL == 8) {
    r_SL = 105.68;
    r_SL2 = 107.5;
    N_TS_SL = 384;
    //N_TS_lim = 208;
  } else {
    cout << NAME << " !!! bad super layer ID " << SL << endl;
    return;
  }

  double R1;
  double R2;
  double R1_2;
  double R2_2;
  double phi1;
  double phi2;
  double cross1;
  double cross2;

  // monitor TS IDs that cross the selected quarter of the Hough plane
  int TS_min = N_TS_SL;
  int TS_max = 0;

  const string vhd = resut_subdir_name + "/UT3_0_SL" + to_string(SL) + ".vhd";
  ofstream outVHD(vhd);

  //generate firmware code
  outVHD << "library IEEE;" << endl;
  outVHD << "use IEEE.STD_LOGIC_1164.ALL;" << endl;
  outVHD << " " << endl;
  outVHD << " " << endl;
  outVHD << "entity UT3_0_SL" << SL << " is" << endl;
  outVHD << " " << endl;
  outVHD << "Port (" << endl;
  outVHD << "           SL" << SL << "_map : out SL_map;" << endl;
  outVHD << "           SL" << SL << "_TS  : in  SL_HIT (NumTSF" << SL << " downto 0)" << endl;
  outVHD << "           );" << endl;
  outVHD << "end UT3_0_SL" << SL << ";" << endl;
  outVHD << " " << endl;
  outVHD << " " << endl;
  outVHD << "architecture Behavioral of UT3_0_SL" << SL << " is" << endl;
  outVHD << " " << endl;
  outVHD << "begin" << endl;
  outVHD << " " << endl;

  //vertical
  for (unsigned k = 0 ; k < nY ; ++k) {
    //horizontal
    for (unsigned j = 0 ; j < nX ; ++j) {

      if (xL <= j && j < xR)
        outVHD << "SL" << SL << "_map(" << k << ")(" << j - xL << ") <= ";
      outcc << "    SL" << SL << "_row" << k + 1 << ".set(" << j << ", ";

      bool first = true;

      // get phi at cell borders (+ small margin to account for rounding errors)
      phi1 = minX + j * dx - 1e-10;
      phi2 = minX + (j + 1) * dx + 1e-10;

      //TS
      for (int i = 0 ; i < N_TS_SL ; i++) {
        // calculate 1/r(phi) at Hough cell border phi1 and phi2 for 1st and 2nd priority
        R1 = 2 / r_SL * sin(phi1 - (2 * M_PI / N_TS_SL) * i);
        R2 = 2 / r_SL * sin(phi2 - (2 * M_PI / N_TS_SL) * i);
        R1_2 = 2 / r_SL2 * sin(phi1 - (2 * M_PI / N_TS_SL) * (i + 0.5));
        R2_2 = 2 / r_SL2 * sin(phi2 - (2 * M_PI / N_TS_SL) * (i + 0.5));

        /* Check whether R(phi) = 1/r(phi) crosses the Hough cell
         * defined by (phi1, phi2, R1, R2)
         * with a positive slope.
         */

        //...1st priority...
        if (R1 < R2) {
          // crossing if R1 below top cell edge and R2 above bottom cell edge
          cross1 = minY + (k + 1) * dy - R1;
          cross2 = minY + k * dy - R2;
          if (cross1 * cross2 <= 0.0) {
            HPcell1[k][j].push_back(SL);
            HPcell1[k][j].push_back(i);
            if (!first) {
              if (xL <= j && j < xR)
                outVHD << " or ";
              outcc << " or ";
            }
            if (xL <= j && j < xR) {
              outVHD << "SL" << SL << "_TSHit(" << i << ")(3)";
              TS_min = (i < TS_min) ? i : TS_min;
              TS_max = (i > TS_max) ? i : TS_max;
            }
            outcc << "SL" << SL << "_TS[" << i << "]";
            first = false;
          }
        }

        //...2nd priority...
        if (R1_2 < R2_2) {
          // crossing if R1_2 below top cell edge and R2_2 above bottom cell edge
          cross1 = minY + (k + 1) * dy - R1_2;
          cross2 = minY + k * dy - R2_2;
          if (cross1 * cross2 <= 0.0) {
            HPcell2[k][j].push_back(SL);
            HPcell2[k][j].push_back(i);
            if (!first) {
              if (xL <= j && j < xR)
                outVHD << " or ";
            }
            if (xL <= j && j < xR) {
              outVHD << "SL" << SL << "_TSHit(" << i << ")(2) or "
                     << "SL" << SL << "_TSHit(" << i + 1 << ")(1)";
              TS_min = (i < TS_min) ? i : TS_min;
              TS_max = (i > TS_max) ? i : TS_max;
            }
            // missing: 2nd priority for firmware simulation
            first = false;
          }
        }
      }

      if (xL <= j && j < xR)
        outVHD << ";" << endl;
      outcc << ");" << endl;
    }
  }
  cout << "SL " << SL << ": TS range for FPGA is [" << TS_min << ", " << TS_max << "]" << endl;

  outVHD << " " << endl;
  outVHD << "end Behavioral;" << endl;
  return;
}

void
printMapping(ofstream& out, vector<vector<vector<int>>>& HPcell)
{
  //...Date...
  string ts = TRGUtil::dateString();
  out << "// This file is generated by " << NAME << "(" << VERSION << ")" << endl;
  out << "// " << ts << endl << endl;
  //...Hough plane parameters...
  out << nX << " " << nY << " " << minY << " " << maxY << endl;
  //...TS patterns...
  for (unsigned iy = 0; iy < nY; iy++) {
    for (unsigned ix = 0; ix < nX; ix++) {
      out << ix << " " << iy << " ";
      for (unsigned its = 0; its < HPcell[iy][ix].size(); its++) {
        out << HPcell[iy][ix][its] << " ";
      }
      out << " " << endl;
    }
  }
}
