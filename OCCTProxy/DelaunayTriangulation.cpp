#include "DelaunayTriangulation.hpp"
#include "CalcUtility.hpp"

double dot(PointDT a, PointDT b) 
{ 
  return a.x * b.x + a.y * b.y; 
}
double cross(PointDT a, PointDT b) 
{ 
  return a.x * b.y - a.y * b.x; 
}

Edge make_edge(size_t a, size_t b) 
{ 
  return Edge(std::min(a, b), std::max(a, b)); 
}