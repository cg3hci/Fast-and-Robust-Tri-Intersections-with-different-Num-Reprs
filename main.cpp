#include <iostream>
#include "tri_tri_intersect.h"
#include "printIntersections.h"
#include "cinolib/gl/glcanvas.h"
#include "cinolib/drawable_triangle_soup.h"

using namespace std;

int main(int argc, char **argv) {
    initFPU();
    typedef enum{
      DOUBLE = 0,
      GMPQ = 1,
      IMPLICIT = 2
  }pointType;

    pointType pType = DOUBLE;

    string arg1 = "";

    if (argc == 2) {

      cout << "Point type: " << argv[1] << endl;

      arg1 = argv[1];
      std::transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
      if (arg1 == "FP") {
        pType = DOUBLE;
      }
      else if (arg1 == "RATIONAL") {
        pType = GMPQ;
      }
      else if (arg1 == "IMPLICIT") {
        pType = IMPLICIT;
      }
      else {
        cout << "Unknown point type: " << argv[1] << endl;
        cout << "Usage: " << argv[0] << "<point type>" << endl;
        cout << "FP" << endl;
        cout << "RATIONAL" << endl;
        cout << "IMPLICIT" << endl;
        return 1;
      }


    }
    else {
      cout << "Usage: " << argv[0] << "<point type>" << endl;
      cout << "FP" << endl;
      cout << "RATIONAL" << endl;
      cout << "IMPLICIT" << endl;
      return 1;
    }

  //Defionition of the triangles
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //Float point definition
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  double fp00[3] = {0.0, 0.0, 0.0};
  double fp01[3] = {1.0, 0.0, 1};
  double fp02[3] = {0.0, 1.0, -1};

  double fp10[3] = {0.25, 0.25, 0.0};
  double fp11[3] = {0.75, 0.25, 1};
  double fp12[3] = {0.25, 0.75, -1};



  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //Rational point definition
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  CGAL::Gmpq r00[3] = {
    CGAL::Gmpq(0, 1), CGAL::Gmpq(0, 1), CGAL::Gmpq(0, 1)};
  CGAL::Gmpq r01[3] = {
    CGAL::Gmpq(1, 1), CGAL::Gmpq(0, 1), CGAL::Gmpq(1, 1)};
  CGAL::Gmpq r02[3] = {
    CGAL::Gmpq(0, 1), CGAL::Gmpq(1, 1), CGAL::Gmpq(-1, 1)};

  CGAL::Gmpq r10[3] = {
    CGAL::Gmpq(25, 100), CGAL::Gmpq(25, 100), CGAL::Gmpq(0, 1)};
  CGAL::Gmpq r11[3] = {
    CGAL::Gmpq(75, 100), CGAL::Gmpq(25, 100), CGAL::Gmpq(1, 1)};
  CGAL::Gmpq r12[3] = {
    CGAL::Gmpq(25, 100), CGAL::Gmpq(75, 100), CGAL::Gmpq(-1, 1)};


  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //Implicit point definition
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  const genericPoint * i00 = new explicitPoint3D(0.0, 0.0, 0.0);
  const genericPoint * i01 = new explicitPoint3D(1.0, 0.0, 1);
  const genericPoint * i02 = new explicitPoint3D(0.0, 1.0, -1);

  const genericPoint * i10 = new explicitPoint3D(0.25, 0.25, 0.0);
  const genericPoint * i11 = new explicitPoint3D(0.75, 0.25, 1);
  const genericPoint * i12 = new explicitPoint3D(0.25, 0.75, -1);



  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //tri_tri_intersect
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

  intersectionResult result;
  switch(pType){
    case DOUBLE:
      result = tri_intersect_tri(fp00, fp01, fp02,
                                 fp10, fp11, fp12);
    break;
    case GMPQ:
      result = tri_intersect_tri(r00, r01, r02,
                                 r10, r11, r12);
    break;
    case IMPLICIT:
      result = tri_intersect_tri(i00, i01, i02,
                                 i10, i11, i12);
    break;
  }

  result.printIntersectionPoints();
  result.printIntersectionEdges();


  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //Display the triangles in the GUI
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

  std::vector<double> t0;
  std::vector<double> t1;

  switch (pType) {
    case DOUBLE:
      t0 = {fp00[0], fp00[1], fp00[2], fp01[0], fp01[1], fp01[2], fp02[0], fp02[1], fp02[2]};
      t1 = {fp10[0], fp10[1], fp10[2], fp11[0], fp11[1], fp11[2], fp12[0], fp12[1], fp12[2]};

      break;
    case GMPQ:
      t0 = tri_to_double(r00, r01, r02);
      t1 = tri_to_double(r10, r11, r12);
      break;
    case IMPLICIT:
      t0 = tri_to_double(*i00, *i01, *i02);
      t1 = tri_to_double(*i10, *i11, *i12);
      break;
  }

  cinolib::GLcanvas gui;
  std::vector<double> coords = {t0[0], t0[1], t0[2],
                               t0[3], t0[4], t0[5],
                               t0[6], t0[7], t0[8],
                               t1[0], t1[1], t1[2],
                               t1[3], t1[4], t1[5],
                               t1[6], t1[7], t1[8]};

  std::vector<uint> tris = {0, 1, 2, 3, 4, 5};

  std::vector<cinolib::Color> colors = {cinolib::Color::PASTEL_CYAN(), cinolib::Color::PASTEL_ORANGE()};

  cinolib::DrawableSegmentSoup s;
  s.thickness = 4;
  cinolib::DrawableTriangleSoup t(coords,tris,colors);
  std::vector<cinolib::Marker> markers;

  printableIntersections tri_gui = {&markers, &gui, &t, &s};

  float size = 0.1f;

  gui.callback_app_controls = [&]() {
    if(ImGui::Checkbox("View triangles", &tri_gui.viewTriangles)){
      if(tri_gui.viewTriangles) gui.push(tri_gui.triangles);
      else gui.pop(tri_gui.triangles);
    }

    if(ImGui::Checkbox("View segments", &tri_gui.viewSegments)){
      if(tri_gui.viewSegments) gui.push(tri_gui.segments);
      else gui.pop(tri_gui.segments);
    }

    if(ImGui::Checkbox("View markers", &tri_gui.viewMarkers)){
      if(tri_gui.viewMarkers) gui.markers = *tri_gui.markers;
      else gui.markers.clear();
    }

    if (ImGui::InputFloat("Marker size", &size)) {}

    if (ImGui::Button("Aggiorna dimensione Marker")) {
      for (auto &m : tri_gui.gui->markers) {
        m.disk_radius = size;
        m.font_size = size;
      }
    }

    ImGui::Text("Intersect: %d", result.intersect);

    if (result.coplanar) ImGui::Text("Coplanar: true"); else ImGui::Text("Coplanar: false");

    ImGui::Text("Point type: %s", argv[1]);
    if (arg1 != "FP") ImGui::TextColored(ImVec4(255,0,0,255),"Attention: Warning! In order to be displayed, the points have been converted to floating points.");
    ImGui::Text("Intersection points: %d", result.getIntersectionPointsSize());
    if (result.getIntersectionPointsSize() > 0) {
      ImGui::Text("Intersection edges: %d", result.getIntersectionEdgesSize());
    } else {
      ImGui::Text("No intersection edges");
    }

    std::streambuf* coutBufferOriginale = std::cout.rdbuf();
    std::ostringstream bufferOutput;
    std::cout.rdbuf(bufferOutput.rdbuf());
    result.printIntersectionPoints();
    result.printIntersectionEdges();

    std::cout.rdbuf(coutBufferOriginale);
    std::string testo = bufferOutput.str();
    ImGui::TextUnformatted(testo.c_str());


  };
  printTrianglesIntersections(t0,t1,result,tri_gui);

  return gui.launch();
}