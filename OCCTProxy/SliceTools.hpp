#ifndef SLICE_TOOLS
#define SLICE_TOOLS

//#include <geos_c.h> 
#include <opencascade/TColgp_Array1OfPnt.hxx>
#include <opencascade/GeomAPI_Interpolate.hxx>
#include <opencascade/BRepBuilderAPI_MakeEdge.hxx>
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/GeomAPI_ProjectPointOnSurf.hxx>
#include <opencascade/Geom_OffsetSurface.hxx>
#include <opencascade/GeomConvert_ApproxSurface.hxx>

#include <vector>
#include <iostream>
#include <fstream>

#include <clipper2/clipper.h>

using namespace Clipper2Lib;

#define CLIPPER_KVAL 1000000.0  // double only
#define UV_DISABLE_AREA 0.01


// add infill
//void createInfill(std::vector<std::vector<PointData>>* infill, Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under,
//	std::vector<PointData> points, double width, double defaultThickness);

// pre slice
//void offsetLayer(OCCTObjects^ myObj, int geomIdLower, double dist, Handle(Geom_BSplineSurface) surface);
void offsetVolume(OCCTObjects^ myObj, int geomidVolume, int geomidOffset, int cntOffset, double* offsetValue);
void alignmentLayer(OCCTObjects^ myObj, int geomIdBase, int geomidLower, int nx, int ny, 
  Handle(Geom_BSplineSurface)* lowerSurface);
void createSurface(Handle(Geom_BSplineSurface) lowerSurface, Handle(Geom_BSplineSurface) upperSurface,
  int layerCnt, int idx, int nx, int ny, Handle(Geom_BSplineSurface)* surface);

// csv
void writeCsv(char* path, std::vector<PointData> list);

// wall slice
Handle(Geom_BSplineSurface) createCurves(OCCTObjects^ myObj, int surfaceIdx, int wallIdx, int volumeGeomid,
  int* dispEdgeCnt, std::vector<Handle(Geom_Curve)>* curves, 
  std::vector<TopoDS_Face*>* faces, std::vector<Handle(Geom_BSplineSurface)>* volumes);
std::vector<std::vector<std::pair<double, double>>> calcUvMap(
  std::vector<Handle(Geom_Curve)>* curves, std::vector<TopoDS_Face*>* faces, std::vector<Handle(Geom_BSplineSurface)>* volumes,
  Handle(Geom_BSplineSurface)* surface, double pointsDistance, double* real2uv);
std::vector<std::vector<std::pair<double, double>>> sortUvMap(
  std::vector<std::vector<std::pair<double, double>>> input, double real2uv, double pointsDistance);
std::vector<PointData> createWallPointData(std::vector<std::vector<std::pair<double, double>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm);
std::vector<PointData> rotateWallDatas(std::vector<PointData> input, gp_Pnt prePnt);

// infill
std::vector<int> checkWallDepth(Paths64 input);
std::vector<std::vector<std::vector<std::pair<double, double>>>> calcInfillByGridUv(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent,
  Handle(Geom_BSplineSurface) surface);
std::vector<PointData> createInfillPointData(
  std::vector<std::vector<std::vector<std::pair<double, double>>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx, int loopStartIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm);
std::vector<std::vector<std::vector<std::pair<double, double>>>> calcInfillByGridXyz(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent,
  Handle(Geom_BSplineSurface) surface, int underInfill, int nowInfill, OCCTObjects^ myObj);
std::vector<PointData> rotateInfillDatas(std::vector<PointData> input, gp_Pnt prePnt);

// bottom
std::vector<std::vector<std::vector<std::pair<double, double>>>> calcBottom(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent,
  Handle(Geom_BSplineSurface) surface, int direction);
std::vector<PointData> createBottomPointData(
  std::vector<std::vector<std::vector<std::pair<double, double>>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm);

// gap fill
std::vector<std::vector<std::pair<double, double>>> calcGapFill(
  std::vector<std::vector<std::pair<double, double>>> upperUvInner,
  std::vector<std::vector<std::pair<double, double>>> upperUvOuter,
  std::vector<std::vector<std::pair<double, double>>> lowerUvInner,
  std::vector<std::vector<std::pair<double, double>>> lowerUvOuter,
  Handle(Geom_BSplineSurface) upperSurface, Handle(Geom_BSplineSurface) lowerSurface, double upperReal2uv,
  double width, double gapPercent);

// offset (inner wall)
std::vector<std::vector<std::vector<std::pair<double, double>>>> calcOffsetUv(
  std::vector<std::vector<std::pair<double, double>>> input, int numWall,
  double real2uv, double width, bool outerWallFlag);


// infill
/*void checkWall(std::vector<PointData> points,
  Paths64* outerUv, Paths64* innerUv, double* real2uv);
void createWallOffset(Paths64 outerWallUv, Paths64 innerWallUv, double width, double real2uv,
  Paths64* outerOffsetUv, Paths64* innerOffsetUv);
void createInfill(Paths64 outerOffsetUv, Paths64 innerOffsetUv, double width, double pointsDistance,
  double percent, double real2uv, int blockIdx, int layerIdx, int loopStartIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, double defaultThickness,
  std::vector<PointData>* infill);

std::vector<PointData> checkBottom(std::vector<PointData> wall, double width);
std::vector<PointData> createBottom(std::vector<PointData> arr, double pointsDistance);
*/
#endif