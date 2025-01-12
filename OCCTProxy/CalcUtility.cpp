#include "CalcUtility.hpp"
//#include "OCCTProxy.hpp"


bool PointData::operator<(const PointData& other) const
{
  if (pnt.Y() == other.pnt.Y())
  {
    return pnt.X() < other.pnt.X();
  }
  return pnt.Y() < other.pnt.Y();
}

double calcDistance(gp_Pnt a, gp_Pnt b)
{
	double dist = (a.X() - b.X()) * (a.X() - b.X()) + (a.Y() - b.Y()) * (a.Y() - b.Y()) + (a.Z() - b.Z()) * (a.Z() - b.Z());
	return sqrt(dist);
}

bool equalPoint(gp_Pnt a, gp_Pnt b, double th)
{
  double dist = calcDistance(a, b);
  return dist < th;
}

PointData calcPointData_uv_with_under(std::pair<double, double> uv,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under,
  PrintType type, double width, int blockIdx, int layerIdx, int loopIdx, int curveIdx, double defaultThick, gp_Dir preNorm)
{
  PointData data;
  data.type = type;
  data.width = width;
  data.blockIdx = blockIdx;
  data.layerIdx = layerIdx;
  data.loopIdx = loopIdx;
  data.curveIdx = curveIdx;
  data.uv = uv;
  // pnt
  data.pnt = surface.get()->Value(uv.first, uv.second);
  // norm
  GeomLProp_SLProps prop(surface, uv.first, uv.second, 1, 0.01);
  if (prop.IsNormalDefined())
  {
    data.norm = prop.Normal();
  }
  else
  {
    data.norm = preNorm;
  }
  // thickness
  GeomAPI_ProjectPointOnSurf api(data.pnt, under);
  try {
    data.thick = api.LowerDistance();
  }
  catch (std::exception e)
  {
    data.thick = defaultThick;
  }
  return data;
}


PointData calcPointData_uv(std::pair<double, double> uv,
  Handle(Geom_BSplineSurface) surface, double thickness,
  PrintType type, double width, int blockIdx, int layerIdx, int loopIdx, int curveIdx, gp_Dir preNorm)
{
  PointData data;
  data.type = type;
  data.width = width;
  data.blockIdx = blockIdx;
  data.layerIdx = layerIdx;
  data.loopIdx = loopIdx;
  data.curveIdx = curveIdx;
  data.uv = uv;
  // pnt
  data.pnt = surface.get()->Value(uv.first, uv.second);
  // norm
  GeomLProp_SLProps prop(surface, uv.first, uv.second, 1, 0.01);
  if (prop.IsNormalDefined())
  {
    data.norm = prop.Normal();
  }
  else
  {
    data.norm = preNorm;
  }
  // thickness
  data.thick = thickness;
  return data;
}

std::pair<double, double>* findIntersection(
  double x1a, double y1a, double x1b, double y1b, double x2a, double y2a, double x2b, double y2b) 
{

  double denom = (y2b - y2a) * (x1b - x1a) - (x2b - x2a) * (y1b - y1a);

  if (denom == 0.0) {
    return nullptr;
  }

  double ua = ((x2b - x2a) * (y1a - y2a) - (y2b - y2a) * (x1a - x2a)) / denom;
  double ub = ((x1b - x1a) * (y1a - y2b) - (y1b - y1a) * (x1a - x2a)) / denom;

  if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0) {
    double intersectionX = x1a + ua * (x1b - x1a);
    double intersectionY = y1a + ua * (y1b - y1a);
    return new std::pair<double, double>(intersectionX, intersectionY);
  }

  return nullptr;
}

std::vector<std::pair<double, double>> crossLines(
  std::vector<std::pair<double, double>> lines, double x1, double y1, double x2, double y2)
{
  std::vector<std::pair<double, double>> arr;
  for (int i = 0; i < lines.size() - 1; i++)
  {
    std::pair<double, double>* res = findIntersection(
      lines[i].first, lines[i].second, lines[i + 1].first, lines[i + 1].second, x1, y1, x2, y2);
    if (res != nullptr)
    {
      arr.push_back(*res);
    }
  }
  std::sort(arr.begin(), arr.end(), [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
    return a.first < b.first;
    });
  return arr;
}

PointCgal calcMatVec(double m[3 * 3], PointCgal pnt)
{
  double x = m[3 * 0 + 0] * pnt.x() + m[3 * 0 + 1] * pnt.y() + m[3 * 0 + 2] * pnt.z();
  double y = m[3 * 1 + 0] * pnt.x() + m[3 * 1 + 1] * pnt.y() + m[3 * 1 + 2] * pnt.z();
  double z = m[3 * 2 + 0] * pnt.x() + m[3 * 2 + 1] * pnt.y() + m[3 * 2 + 2] * pnt.z();
  return PointCgal(x, y, z);
}

gp_Vec calcMatVec(double m[3 * 3], gp_Vec vec)
{
  double x = m[3 * 0 + 0] * vec.X() + m[3 * 0 + 1] * vec.Y() + m[3 * 0 + 2] * vec.Z();
  double y = m[3 * 1 + 0] * vec.X() + m[3 * 1 + 1] * vec.Y() + m[3 * 1 + 2] * vec.Z();
  double z = m[3 * 2 + 0] * vec.X() + m[3 * 2 + 1] * vec.Y() + m[3 * 2 + 2] * vec.Z();
  return gp_Vec(x, y, z);
}

gp_Pnt calcMatVec(double m[3 * 3], double px, double py, double pz)
{
  double x = m[3 * 0 + 0] * px + m[3 * 0 + 1] * py + m[3 * 0 + 2] * pz;
  double y = m[3 * 1 + 0] * px + m[3 * 1 + 1] * py + m[3 * 1 + 2] * pz;
  double z = m[3 * 2 + 0] * px + m[3 * 2 + 1] * py + m[3 * 2 + 2] * pz;
  return gp_Pnt(x, y, z);
}

bool inTriangle(double x0, double y0, double x1, double y1, double x2, double y2,
  double px, double py, double* s, double* t)
{
  double area = 0.5 * (-y1 * x2 + y0 * (-x1 + x2) + x0 * (y1 - y2) + x1 * y2);
  *s = 1.0 / (2.0 * area) * (y0 * x2 - x0 * y2 + (y2 - y0) * px + (x0 - x2) * py);
  *t = 1.0 / (2.0 * area) * (x0 * y1 - y0 * x1 + (y0 - y1) * px + (x1 - x0) * py);
  if ((0.0 <= *s) && (*s <= 1.0) && (0.0 <= *t) && (*t <= 1.0) && (*s + *t <= 1.0))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void calcQuat2Mat(const double nx, const double ny, const double nz,
  const double angle, double m[3 * 3])
{
  double qx = nx * sin(angle / 2.0);
  double qy = ny * sin(angle / 2.0);
  double qz = nz * sin(angle / 2.0);
  double qw = cos(angle / 2.0);
  m[3 * 0 + 0] = 2.0 * qw * qw + 2.0 * qx * qx - 1.0;
  m[3 * 0 + 1] = 2.0 * qx * qy - 2.0 * qz * qw;
  m[3 * 0 + 2] = 2.0 * qx * qz + 2.0 * qy * qw;
  m[3 * 1 + 0] = 2.0 * qx * qy + 2.0 * qz * qw;
  m[3 * 1 + 1] = 2.0 * qw * qw + 2.0 * qy * qy - 1.0;
  m[3 * 1 + 2] = 2.0 * qy * qz - 2.0 * qx * qw;
  m[3 * 2 + 0] = 2.0 * qx * qz - 2.0 * qy * qw;
  m[3 * 2 + 1] = 2.0 * qy * qz + 2.0 * qx * qw;
  m[3 * 2 + 2] = 2.0 * qw * qw + 2.0 * qz * qz - 1.0;
}

