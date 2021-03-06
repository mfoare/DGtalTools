/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/
/**
 * @file meshViewer.cpp
 * @ingroup visualisation
 * @author Bertrand Kerautret (\c kerautre@loria.fr )
 * LORIA (CNRS, UMR 7503), University of Nancy, France
 *
 * @date 2012/07/08
 *
 *
 *
 * This file is part of the DGtalTools.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include "DGtal/base/Common.h"

#include "DGtal/io/Display3D.h"
#include "DGtal/io/viewers/Viewer3D.h"
#include "DGtal/io/readers/MeshReader.h"
#include "DGtal/helpers/StdDefs.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


using namespace std;
using namespace DGtal;


///////////////////////////////////////////////////////////////////////////////
namespace po = boost::program_options;

int main( int argc, char** argv )
{
  // parse command line ----------------------------------------------
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("input,i", po::value<std::vector<string> >()->multitoken(), "off files (.off), or OFS file (.ofs) " )
    ("scaleX,x",  po::value<float>()->default_value(1.0), "set the scale value in the X direction (default 1.0)" )
    ("scaleY,y",  po::value<float>()->default_value(1.0), "set the scale value in the Y direction (default 1.0)" )
    ("scaleZ,z",  po:: value<float>()->default_value(1.0), "set the scale value in the Z direction (default 1.0)")
    ("minLineWidth,w",  po:: value<float>()->default_value(1.5), "set the min line width of the mesh faces (default 1.5)")
    ("customColorMesh",po::value<std::vector<unsigned int> >()->multitoken(), "set the R, G, B, A components of the colors of the mesh faces and eventually the color R, G, B, A of the mesh edge lines (set by default to black). " )
    ("customColorSDP",po::value<std::vector<unsigned int> >()->multitoken(), "set the R, G, B, A components of the colors of the sdp view" )
    ("displaySDP,s", po::value<std::string>(), "Add the display of a set of discrete points as ball of radius 0.5.")
    ("SDPradius", po::value<double>()->default_value(0.5), "change the ball radius to display a set of discrete points (used with displaySDP option)")
    ("invertNormal,n", "threshold min to define binary shape" )
    ("drawVertex,v", "draw the vertex of the mesh" );

  bool parseOK=true;
  po::variables_map vm;
  try{
    po::store(po::parse_command_line(argc, argv, general_opt), vm);
  }catch(const std::exception& ex){
    parseOK=false;
    trace.info()<< "Error checking program options: "<< ex.what()<< endl;
  }
  po::notify(vm);
  if( !parseOK || vm.count("help")||argc<=1)
    {
      std::cout << "Usage: " << argv[0] << " [input]\n"
    << "Display OFF mesh file by using QGLviewer"
    << general_opt << "\n";
      return 0;
    }

  if(! vm.count("input"))
    {
      trace.error() << " The file name was defined" << endl;
      return 0;
    }



  std::vector<std::string> inputFilenameVect = vm["input"].as<std::vector<std::string > >();
  float sx = vm["scaleX"].as<float>();
  float sy = vm["scaleY"].as<float>();
  float sz = vm["scaleZ"].as<float>();

  unsigned int  meshColorR = 240;
  unsigned int  meshColorG = 240;
  unsigned int  meshColorB = 240;
  unsigned int  meshColorA = 255;

  unsigned int  meshColorRLine = 0;
  unsigned int  meshColorGLine = 0;
  unsigned int  meshColorBLine = 0;
  unsigned int  meshColorALine = 255;


  unsigned int  sdpColorR = 240;
  unsigned int  sdpColorG = 240;
  unsigned int  sdpColorB = 240;
  unsigned int  sdpColorA = 255;

  float lineWidth = vm["minLineWidth"].as<float>();


  if(vm.count("customColorMesh")){
    std::vector<unsigned int > vectCol = vm["customColorMesh"].as<std::vector<unsigned int> >();
    if(vectCol.size()!=4 && vectCol.size()!=8 ){
      trace.error() << "colors specification should contain R,G,B and Alpha values"<< std::endl;
    }
    meshColorR = vectCol[0];
    meshColorG = vectCol[1];
    meshColorB = vectCol[2];
    meshColorA = vectCol[3];
    if(vectCol.size() == 8){
      meshColorRLine = vectCol[4];
      meshColorGLine = vectCol[5];
      meshColorBLine = vectCol[6];
      meshColorALine = vectCol[7];

    }

  }
  if(vm.count("customColorSDP")){
    std::vector<unsigned int > vectCol = vm["customColorSDP"].as<std::vector<unsigned int> >();
    if(vectCol.size()!=4){
      trace.error() << "colors specification should contain R,G,B and Alpha values"<< std::endl;
    }
    sdpColorR = vectCol[0];
    sdpColorG = vectCol[1];
    sdpColorB = vectCol[2];
    sdpColorA = vectCol[3];
  }



  QApplication application(argc,argv);
  Viewer3D<> viewer;
  std::stringstream title; 
  title  << "Simple Mesh Viewer: " << inputFilenameVect[0];
  viewer.setWindowTitle(title.str().c_str());
  viewer.show();
  viewer.myGLLineMinWidth = lineWidth;
  viewer.setGLScale(sx, sy, sz);
  bool invertNormal= vm.count("invertNormal");


  double ballRadius = vm["SDPradius"].as<double>();

  trace.info() << "Importing mesh... ";

  std::vector<Mesh<DGtal::Z3i::RealPoint> >  vectMesh;
  for(unsigned int i = 0; i< inputFilenameVect.size(); i++){
    Mesh<DGtal::Z3i::RealPoint> aMesh(!vm.count("customColorMesh"));
    aMesh << inputFilenameVect[i];
    vectMesh.push_back(aMesh);
  }
  
  
  bool import = vectMesh.size()==inputFilenameVect.size();
  if(!import){
    trace.info() << "File import failed. " << std::endl;
    return 0;
  }

  trace.info() << "[done]. "<< std::endl;
  if(vm.count("displaySDP")){
    std::string filenameSDP = vm["displaySDP"].as<std::string>();
    vector<Z3i::RealPoint> vectPoints;
    vectPoints = PointListReader<Z3i::RealPoint>::getPointsFromFile(filenameSDP);
    viewer << CustomColors3D(Color(sdpColorR, sdpColorG, sdpColorB, sdpColorA),
                             Color(sdpColorR, sdpColorG, sdpColorB, sdpColorA));
    for(unsigned int i=0;i< vectPoints.size(); i++){
      viewer.addBall(vectPoints.at(i), ballRadius);
    }
  }
  if(invertNormal){
     for(unsigned int i=0; i<vectMesh.size(); i++){
       vectMesh[i].invertVertexFaceOrder();
     }
  }

  viewer << CustomColors3D(Color(meshColorRLine, meshColorGLine, meshColorBLine, meshColorALine),
                           Color(meshColorR, meshColorG, meshColorB, meshColorA));
  for(unsigned int i=0; i<vectMesh.size(); i++){
    viewer << vectMesh[i];
  }

  if(vm.count("drawVertex")){
    for(unsigned int i=0; i<vectMesh.size(); i++){
      
      for( Mesh<DGtal::Z3i::RealPoint>::VertexStorage::const_iterator it = vectMesh[i].vertexBegin();
           it!=vectMesh[i].vertexEnd(); ++it){
        DGtal::Z3i::Point pt;
        pt[0]=(*it)[0]; pt[1]=(*it)[1]; pt[2]=(*it)[2];
        viewer << pt;
      }
    }
  }


  viewer << Viewer3D<>::updateDisplay;
  return application.exec();
}
