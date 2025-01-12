#ifndef DELAUNAY_TRIANGULATION
#define DELAUNAY_TRIANGULATION

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <utility>
#include <random>
#include <cmath>
#include <limits>
#include <chrono>
#include <cassert>
#include "CalcUtility.hpp"

struct PointDT {
  /*** Point on the 2D-plane ***/
  double x, y;
  PointDT operator + (const PointDT& p) const { return PointDT{ x + p.x, y + p.y }; }
  PointDT operator - (const PointDT& p) const { return PointDT{ x - p.x, y - p.y }; }
  PointDT operator * (double d) const { return PointDT{ x * d, y * d }; }
  PointDT operator / (double d) const { return PointDT{ x / d, y / d }; }
  bool operator < (const PointDT& p) const { return x == p.x ? y < p.y : x < p.x; }
  PointDT normal() const { return PointDT{ y, -x }; }
  double norm() const { return x * x + y * y; }
};

double dot(PointDT a, PointDT b);
double cross(PointDT a, PointDT b);

/*** Edge connecting two points ***/
using Edge = std::pair<size_t, size_t>;
Edge make_edge(size_t a, size_t b);

/*** Core Implementation ***/
class DelaunayTriangulation {
  bool is_ccw(size_t a, size_t b, size_t c) const { return cross(P[b] - P[a], P[c] - P[a]) > 0; }

  struct Triangle {
    /*** Triangle on the 2D-plane ***/
    size_t a, b, c;
    size_t opposite(Edge e) const {
      if (e.first != a && e.second != a) return a;
      if (e.first != b && e.second != b) return b;
      return c;
    }
  };

  Triangle make_triangle(size_t a, size_t b, size_t c) const {
    /*** Make triangle where the direction 'a->b->c' is counter-clockwise ***/
    if (!is_ccw(a, b, c)) std::swap(b, c);
    return Triangle{ a, b, c };
  }

  bool point_in_triangle(size_t tar, const Triangle& t) const {
    /*** Check wheter a point lies in an triangle or not ***/
    if (!is_ccw(t.a, t.b, tar)) return false;
    if (!is_ccw(t.b, t.c, tar)) return false;
    if (!is_ccw(t.c, t.a, tar)) return false;
    return true;
  }

private:
  size_t n;
  std::vector<PointDT> P;
  std::vector<Triangle> T;
  std::vector<std::list<size_t>> CH;
  std::vector<Edge> edge;

  size_t add_child_triangle(size_t k, const Triangle& t) {
    /*** Add a child triangle for T[k] ***/
    size_t new_k = T.size();
    T.push_back(t);
    CH.push_back(std::list<size_t>());
    CH[k].push_back(new_k);
    return new_k;
  }

  size_t add_child_triangle(size_t k1, size_t k2, const Triangle& t) {
    /*** Add a child triangle for T[k1] and T[k2] ***/
    size_t new_k = T.size();
    T.push_back(t);
    CH.push_back(std::list<size_t>());
    CH[k1].push_back(new_k);
    CH[k2].push_back(new_k);
    return new_k;
  }

  size_t find_child(size_t k, size_t tar) const {
    /*** Find child triangle of T[k] where P[tar] lies in ***/
    for (size_t i : CH[k]) if (point_in_triangle(tar, T[i])) return i;
    return std::numeric_limits<size_t>::max();
  }

private:
  struct EdgeHash {
    /*** Hash function for edges ***/
    size_t operator () (const Edge& e) const {
      static const size_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
      size_t key = ((e.first << 16) ^ e.second) ^ FIXED_RANDOM;
      return key ^ (key >> 16);
    }
  };

  // Maps an edge to its incident triangles
  std::unordered_map<Edge, std::set<size_t>, EdgeHash> e2t;

  void register_triangle(size_t k, const Triangle& t) {
    /*** Register a new triangle ***/
    e2t[make_edge(t.a, t.b)].insert(k);
    e2t[make_edge(t.b, t.c)].insert(k);
    e2t[make_edge(t.c, t.a)].insert(k);
  }

  void unregister_triangle(size_t k, const Triangle& t) {
    /*** Unregister an old triangle ***/
    e2t[make_edge(t.a, t.b)].erase(k);
    e2t[make_edge(t.b, t.c)].erase(k);
    e2t[make_edge(t.c, t.a)].erase(k);
  }

private:
  bool is_ilegal(Edge e, size_t c, size_t tar) {
    /*** Check wheter an edge is ilegal or not ***/
    size_t a = e.first, b = e.second;

    PointDT s1 = (P[a] + P[b]) / 2.;
    PointDT s2 = (P[b] + P[c]) / 2.;
    PointDT t1 = s1 + (P[b] - P[a]).normal();
    PointDT t2 = s2 + (P[c] - P[b]).normal();

    double d1 = cross(t2 - s2, s2 - s1);
    double d2 = cross(t2 - s2, t1 - s1);
    PointDT ct = s1 + (t1 - s1) * d1 / d2;

    double dx1 = P[a].x - ct.x;
    double dy1 = P[a].y - ct.y;
    double dx2 = P[tar].x - ct.x;
    double dy2 = P[tar].y - ct.y;
    return (dx1 * dx1 + dy1 * dy1) > (dx2 * dx2 + dy2 * dy2);
  }

  void legalize_edge(size_t piv, Edge e) {
    /*** Legalize an edge with pivot P[piv] ***/
    if (e2t.count(e) == 0) return;
    if (e2t[e].size() != 2) return;

    size_t k1 = *e2t[e].begin();
    size_t k2 = *e2t[e].rbegin();
    size_t a = T[k1].opposite(e);
    size_t b = T[k2].opposite(e);

    if (is_ilegal(e, a, b)) {
      unregister_triangle(k1, T[k1]);
      unregister_triangle(k2, T[k2]);
      e2t.erase(e);

      Triangle t1 = make_triangle(e.first, a, b);
      Triangle t2 = make_triangle(e.second, a, b);

      size_t k1_ = add_child_triangle(k1, k2, t1);
      size_t k2_ = add_child_triangle(k1, k2, t2);

      register_triangle(k1_, t1);
      register_triangle(k2_, t2);

      size_t c = (piv != a ? a : b);
      legalize_edge(piv, make_edge(e.first, c));
      legalize_edge(piv, make_edge(e.second, c));
    }
  }

  void sub_division(size_t k, size_t tar) {
    /*** Divide triangle T[k] by P[tar] ***/
    unregister_triangle(k, T[k]);

    Triangle t1 = make_triangle(T[k].a, T[k].b, tar);
    Triangle t2 = make_triangle(T[k].b, T[k].c, tar);
    Triangle t3 = make_triangle(T[k].c, T[k].a, tar);

    size_t k1 = add_child_triangle(k, t1);
    size_t k2 = add_child_triangle(k, t2);
    size_t k3 = add_child_triangle(k, t3);

    register_triangle(k1, t1);
    register_triangle(k2, t2);
    register_triangle(k3, t3);

    legalize_edge(tar, make_edge(T[k].a, T[k].b));
    legalize_edge(tar, make_edge(T[k].b, T[k].c));
    legalize_edge(tar, make_edge(T[k].c, T[k].a));
  }

  void modify_convexity() {
    /*** Modify edges of the bounding convex hull ***/
    for (size_t a = n; a <= n + 2; ++a) {
      for (size_t b = 0; b < n; ++b) {
        Edge e(b, a);
        if (e2t.count(e) == 0) continue;

        size_t k1 = *e2t[e].begin();
        size_t k2 = *e2t[e].rbegin();

        size_t c = T[k1].opposite(e);
        size_t d = T[k2].opposite(e);

        if (!is_ccw(a, b, c)) std::swap(c, d);
        if (!is_ccw(c, d, b)) continue;

        unregister_triangle(k1, T[k1]);
        unregister_triangle(k2, T[k2]);

        Triangle t1 = make_triangle(a, c, d);
        Triangle t2 = make_triangle(b, c, d);

        size_t k1_ = add_child_triangle(k1, k2, t1);
        size_t k2_ = add_child_triangle(k1, k2, t2);

        register_triangle(k1_, t1);
        register_triangle(k2_, t2);
      }
    }
  }

private:
  std::uint32_t seed;

  std::uint32_t xor128() {
    static std::uint32_t x = seed, y = 362436069, z = 521288629, w = 88675123;
    std::uint32_t t = (x ^ (x << 11));
    x = y; y = z; z = w;
    return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
  }

public:
  DelaunayTriangulation(const std::vector<PointCgal> xyz, uint32_t seed_ = 123456789) {
    n = xyz.size();
    P = std::vector<PointDT>(n);
    for (size_t i = 0; i < n; ++i) {
      P[i].x = xyz[i].x();
      P[i].y = xyz[i].y();
    }

    double r = std::numeric_limits<double>::min();
    for (size_t i = 0; i < n; ++i) {
      r = std::max(r, std::abs(P[i].x));
      r = std::max(r, std::abs(P[i].y));
    }

    // Add three vertices of the initial bounding triangle
    P.push_back(PointDT{ 3.1 * r, 0 });
    P.push_back(PointDT{ 0, 3.1 * r });
    P.push_back(PointDT{ -3.1 * r, -3.1 * r });

    seed = seed_;
  }

  void execute(double min_delta = 1e-6, double max_delta = 1e-5, int max_miss_count = 30) {
    /*** Execute the algorithm ***/

    // Random reordering
    std::vector<size_t> id(n);
    std::iota(id.begin(), id.end(), size_t(0));
    for (size_t i = 0; i < n; ++i) {
      size_t j = xor128() % (n - i);
      std::swap(id[j], id[n - i - 1]);
    }

    // Initialize each data structure
    T = std::vector<Triangle>();
    CH = std::vector<std::list<size_t>>();
    T.push_back(make_triangle(n, n + 1, n + 2));
    CH.push_back(std::list<size_t>());
    register_triangle(0, T[0]);
    e2t = std::unordered_map<Edge, std::set<size_t>, EdgeHash>();

    // Main iteration
    for (size_t tar : id) {
      int miss_count = 0;
      double px = P[tar].x, py = P[tar].y;
      size_t k = 0;
      while (!CH[k].empty()) {
        size_t nxt = find_child(k, tar);
        if (nxt == std::numeric_limits<size_t>::max()) {
          ++miss_count;
          if (miss_count >= max_miss_count) break;
          // P[tar] perturbates when it falls on an edge
          double dx = min_delta + (max_delta - min_delta) * xor128() / 0xFFFFFFFFu;
          double dy = min_delta + (max_delta - min_delta) * xor128() / 0xFFFFFFFFu;
          dx *= (xor128() % 2 == 0 ? 1 : -1);
          dy *= (xor128() % 2 == 0 ? 1 : -1);
          P[tar].x = px + dx;
          P[tar].y = py + dy;
        }
        else {
          k = nxt;
        }
      }
      if (CH[k].empty()) sub_division(k, tar);
      P[tar].x = px;
      P[tar].y = py;
    }

    // Modify edges of the bounding convex hull
    modify_convexity();

    // Save the solution
    edge = std::vector<Edge>();
    for (auto it : e2t) {
      Edge e = it.first;
      if (e.first < n && e.second < n) edge.push_back(e);
    }
  }

  const std::vector<Edge>& get_edges() { return edge; }

  const std::vector<size_t*> get_triangles() {
    std::vector<size_t*> ret;
    for (size_t i = 0; i < T.size(); i++)
    {
      if (T[i].a < n && T[i].b < n && T[i].c < n)
      {
        size_t* buf = new size_t[3];
        buf[0] = T[i].a;
        buf[1] = T[i].b;
        buf[2] = T[i].c;
        ret.push_back(buf);
      }
    }
    return ret;
  }

  const std::vector<double*> get_points() {
    std::vector<double*> ret;
    for (size_t i = 0; i < n; i++)
    {
      double* buf = new double[2];
      buf[0] = P[i].x;
      buf[1] = P[i].y;
      ret.push_back(buf);
    }
    return ret;
  }
};

#endif
