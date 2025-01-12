#ifndef SLICE_OBJECTS
#define SLICE_OBJECTS

#include <float.h>
#include <opencascade/AIS_InteractiveObject.hxx>
#include <opencascade/AIS_Shape.hxx>
#include <opencascade/TopoDS.hxx>
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/BRepAdaptor_Surface.hxx>
#include <opencascade/GeomInt_IntSS.hxx>
#include <opencascade/GeomAPI_ProjectPointOnSurf.hxx>
//#include "CalcUtility.hpp"
#include <cstdint>
#include <opencascade/Geom_Line.hxx>
#include <opencascade/Geom_TrimmedCurve.hxx>
#include <clipper2/clipper.h>
#include <opencascade/TColgp_Array1OfPnt.hxx>
#include <opencascade/GeomAPI_Interpolate.hxx>
#include <opencascade/BRepBuilderAPI_MakeEdge.hxx>
#include <opencascade/BRepTopAdaptor_FClass2d.hxx>
#include <map>
#include <CGAL/Delaunay_triangulation_2.h>
#include "DelaunayTriangulation.hpp"

using namespace Clipper2Lib;

#define SAMPLE_PARAM 0.01
#define IDX_PATERN 10
#define PRE_CALC_LEN_DIV 10
#define POINTS_FACTOR 2

private ref class SliceObjects
{
public:
  //void** objects;
  Handle(AIS_InteractiveObject)* layers;
  int layer_length;
  SliceObjects(int len);
  ~SliceObjects();
};
/*
struct LayerIndex {
  int surfaceId;
  int wallId;
  int infillId;
  int supportId;
};*/

//typedef CGAL::Delaunay_triangulation_2<Kernel> Delaunay;

#endif