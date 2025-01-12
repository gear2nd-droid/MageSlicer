#ifndef CALC_UTILITY
#define CALC_UTILITY

#include <math.h>
#include <opencascade/gp_Pnt.hxx>
#include <opencascade/Geom_BSplineSurface.hxx>
#include <opencascade/GeomLProp_SLProps.hxx>
#include <opencascade/GeomAPI_ProjectPointOnSurf.hxx>
#include <windows.h>
#include <iostream>
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>

#define EQUAL_POINT_REAL_TH 0.1
#define EQUAL_POINT_REAL_TH_FOR_LOOP 1.0
#define EQUAL_POINT_UV_TH 0.000000000000001

typedef enum
{
	OuterWallMiddle = 11,
	OuterWallStart = 12,
	OuterWallEnd = 13,
	InnerWallMiddle = 21,
	InnerWallStart = 22,
	InnerWallEnd = 23,
	InfillMiddle = 31,
	InfillStart = 32,
	InfillEnd = 33,
	SupportMiddle = 41,
	SupportStart = 42,
	SupportEnd = 43,
	Saving = 51,
	None = 0
}PrintType;

class PointData
{
public:
	PrintType type;
	gp_Pnt pnt;
	gp_Dir norm;
	std::pair<double, double> uv;
	double thick;
	double width;
	int blockIdx;
	int layerIdx;
	int loopIdx;
	int curveIdx;
	bool PointData::operator<(const PointData& other) const;
};


typedef void(*OutputCallback)(const char*);
double calcDistance(gp_Pnt a, gp_Pnt b);
bool equalPoint(gp_Pnt a, gp_Pnt b, double th);

PointData calcPointData_uv_with_under(std::pair<double, double> uv,
	Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under,
	PrintType type, double width, int blockIdx, int layerIdx, int loopIdx, int curveIdx, double defaultThick, gp_Dir preNorm);

PointData calcPointData_uv(std::pair<double, double> uv,
	Handle(Geom_BSplineSurface) surface, double thickness,
	PrintType type, double width, int blockIdx, int layerIdx, int loopIdx, int curveIdx, gp_Dir preNorm);


std::pair<double, double>* findIntersection(
	double x1a, double y1a, double x1b, double y1b, double x2a, double y2a, double x2b, double y2b);
std::vector<std::pair<double, double>> crossLines(
	std::vector<std::pair<double, double>> lines, double x1, double y1, double x2, double y2);

// for CGAL
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 PointCgal;
typedef Kernel::Point_2 PointCgal2d;
typedef Kernel::Plane_3 PlaneCgal;

PointCgal calcMatVec(double m[3 * 3], PointCgal pnt);
gp_Vec calcMatVec(double m[3 * 3], gp_Vec vec);
gp_Pnt calcMatVec(double m[3 * 3], double px, double py, double pz);
bool inTriangle(double x0, double y0, double x1, double y1, double x2, double y2,
	double px, double py, double* s, double* t);
void calcQuat2Mat(const double nx, const double ny, const double nz,
	const double angle, double m[3 * 3]);

#endif