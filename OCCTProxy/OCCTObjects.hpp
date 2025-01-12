#ifndef OCCT_OBJECTS
#define OCCT_OBJECTS

#include <opencascade/AIS_InteractiveObject.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/STEPControl_Reader.hxx>
#include <opencascade/IFSelect_ReturnStatus.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/BRepAdaptor_Surface.hxx>
#include <opencascade/TopoDS.hxx>
#include <limits>
#include <opencascade/gp_Trsf.hxx>
#include <opencascade/BRepBuilderAPI_Transform.hxx>
#include <fstream>
#include <string>
#include <vector>
#include <opencascade/GeomAPI_PointsToBSplineSurface.hxx>
#include <opencascade/BRepBuilderAPI_MakeFace.hxx>
#include <opencascade/Geom_BSplineSurface.hxx>
#include <opencascade/GeomConvert.hxx>
#include <opencascade/BRepBuilderAPI_MakeEdge.hxx>
#include <opencascade/Bnd_Box.hxx>
#include <opencascade/BRepBndLib.hxx>
#include <map>

#define CALC_LAYER_MAX 1000
#define CALC_POINT_PER_LAYER_MAX 100000

typedef enum
{
  E_TOPODS,
  E_TOPODS_SHAPE,
  E_TOPODS_FACE,
  E_TOPODS_EDGE
}E_TYPE;

std::vector<std::string> split(const std::string& s, char delim);

private ref class OCCTObjects
{
public:
  //void** objects;
  Handle(AIS_InteractiveObject)* objects;
  TopoDS_Shape* shapes;
  TopoDS_Face* faces;
  TopoDS_Edge* edges;
  Handle(Geom_BSplineSurface)* surfaces;
  Handle(Geom_Curve)* curves;
  int* geomids;
  int* types;
  int length;
  OCCTObjects(int len);
  ~OCCTObjects();
  int getLength();
  int setObject_ShapeWithSurface(Handle(AIS_InteractiveObject) object, TopoDS_Shape shape, 
    Handle(Geom_BSplineSurface) surface, int geomid);
  int setObject_Face(Handle(AIS_InteractiveObject) object, TopoDS_Face face, 
    Handle(Geom_BSplineSurface) surface, int geomid);
  int setObject_Edge(Handle(AIS_InteractiveObject) object, TopoDS_Edge edge, 
    Handle(Geom_Curve) curve, int geomid);
  void setObject(int idx, Handle(AIS_InteractiveObject) object);
  Handle(AIS_InteractiveObject) getObject(int idx);
  void setGeomId(int idx, int geomid);
  int getGeomId(int idx);
  void setType(int idx, int type);
  int getType(int idx);
  // infill
  double bboxXmin = DBL_MAX;
  double bboxXmax = DBL_MIN;
  double bboxYmin = DBL_MAX;
  double bboxYmax = DBL_MIN;
  double bboxZmin = DBL_MAX;
  double bboxZmax = DBL_MIN;
  gp_Pnt** infillStPnt;
  gp_Pnt** infillEdPnt;
  int infillLines;
};


#endif