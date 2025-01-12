#include "OCCTProxy.hpp"
#include <limits>

void alignmentLayer(OCCTObjects^ myObj, int geomIdLower, int geomidUpper, int nx, int ny, 
  Handle(Geom_BSplineSurface)* surface)
{
  // base surface check
  Handle(Geom_BSplineSurface)* bufLowerSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomIdLower)
    {
      bufLowerSurface = &(myObj->surfaces[i]);
      break;
    }
  }
  double bufLowerUmin, bufLowerUmax, bufLowerVmin, bufLowerVmax;
  bufLowerSurface->get()->Bounds(bufLowerUmin, bufLowerUmax, bufLowerVmin, bufLowerVmax);
  gp_Pnt pl0 = bufLowerSurface->get()->Value(bufLowerUmin, bufLowerVmin);
  gp_Pnt pl1 = bufLowerSurface->get()->Value(bufLowerUmax, bufLowerVmin);
  gp_Pnt pl2 = bufLowerSurface->get()->Value(bufLowerUmax, bufLowerVmax);
  gp_Pnt pl3 = bufLowerSurface->get()->Value(bufLowerUmin, bufLowerVmax);

  // lower surface check
  Handle(Geom_BSplineSurface)* bufUpperSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomidUpper)
    {
      bufUpperSurface = &(myObj->surfaces[i]);
      break;
    }
  }
  double bufUpperUmin, bufUpperUmax, bufUpperVmin, bufUpperVmax;
  bufUpperSurface->get()->Bounds(bufUpperUmin, bufUpperUmax, bufUpperVmin, bufUpperVmax);
  gp_Pnt pu0 = bufUpperSurface->get()->Value(bufUpperUmin, bufUpperVmin);
  gp_Pnt pu1 = bufUpperSurface->get()->Value(bufUpperUmax, bufUpperVmin);
  gp_Pnt pu2 = bufUpperSurface->get()->Value(bufUpperUmax, bufUpperVmax);
  gp_Pnt pu3 = bufUpperSurface->get()->Value(bufUpperUmin, bufUpperVmax);
  double d00, d01, d02, d03, d10, d11, d12, d13, d20, d21, d22, d23, d30, d31, d32, d33;
  double* pat = new double[8];
  int idxMinPat;
  double valMinPat = DBL_MAX;
  d00 = calcDistance(pl0, pu0);
  d01 = calcDistance(pl0, pu1);
  d02 = calcDistance(pl0, pu2);
  d03 = calcDistance(pl0, pu3);
  d10 = calcDistance(pl1, pu0);
  d11 = calcDistance(pl1, pu1);
  d12 = calcDistance(pl1, pu2);
  d13 = calcDistance(pl1, pu3);
  d20 = calcDistance(pl2, pu0);
  d21 = calcDistance(pl2, pu1);
  d22 = calcDistance(pl2, pu2);
  d23 = calcDistance(pl2, pu3);
  d30 = calcDistance(pl3, pu0);
  d31 = calcDistance(pl3, pu1);
  d32 = calcDistance(pl3, pu2);
  d33 = calcDistance(pl3, pu3);
  pat[0] = d00 + d11 + d22 + d33;
  pat[1] = d01 + d12 + d23 + d30;
  pat[2] = d02 + d13 + d20 + d31;
  pat[3] = d03 + d10 + d21 + d32;
  pat[4] = d00 + d13 + d22 + d31;
  pat[5] = d10 + d23 + d32 + d01;
  pat[6] = d20 + d33 + d02 + d11;
  pat[7] = d30 + d03 + d12 + d21;
  for (int i = 0; i < 8; i++)
  {
    if (valMinPat > pat[i])
    {
      valMinPat = pat[i];
      idxMinPat = i;
    }
  }
  double bufU, bufV;
  gp_Pnt bufPnt;
  TColgp_Array2OfPnt bufPnts;
  switch (idxMinPat)
  {
  case 0:
    bufPnts = TColgp_Array2OfPnt(1, nx, 1, ny);
    break;
  case 1:
    bufPnts = TColgp_Array2OfPnt(1, ny, 1, nx);
    break;
  case 2:
    bufPnts = TColgp_Array2OfPnt(1, nx, 1, ny);
    break;
  case 3:
    bufPnts = TColgp_Array2OfPnt(1, ny, 1, nx);
    break;
  case 4:
    bufPnts = TColgp_Array2OfPnt(1, ny, 1, nx);
    break;
  case 5:
    bufPnts = TColgp_Array2OfPnt(1, nx, 1, ny);
    break;
  case 6:
    bufPnts = TColgp_Array2OfPnt(1, ny, 1, nx);
    break;
  case 7:
    bufPnts = TColgp_Array2OfPnt(1, nx, 1, ny);
    break;
  }
  for (int j = 0; j < ny; j++)
  {
    for (int i = 0; i < nx; i++)
    {
      bufU = (bufUpperUmax - bufUpperUmin) / (double)(nx - 1) * i + bufUpperUmin;
      bufV = (bufUpperVmax - bufUpperVmin) / (double)(ny - 1) * j + bufUpperVmin;
      bufPnt = bufUpperSurface->get()->Value(bufU, bufV);
      switch (idxMinPat)
      {
      case 0: // 0t
        bufPnts.SetValue(i + 1, j + 1, bufPnt);
        break;
      case 1: // 1t
        bufPnts.SetValue(ny - j, i + 1, bufPnt);
        break;
      case 2: // 2t
        bufPnts.SetValue(nx - i, ny - j, bufPnt);
        break;
      case 3: // 3t
        bufPnts.SetValue(j + 1, nx - i, bufPnt);
        break;
      case 4: // 0f
        bufPnts.SetValue(j + 1, i + 1, bufPnt);
        break;
      case 5: // 1f
        bufPnts.SetValue(nx - i, j + 1, bufPnt);
        break;
      case 6: // 2f
        bufPnts.SetValue(ny - j, nx - i, bufPnt);
        break;
      case 7: // 3f
        bufPnts.SetValue(i + 1, ny - j, bufPnt);
        break;
      }
    }
  }
  //GeomAPI_PointsToBSplineSurface bufApi = GeomAPI_PointsToBSplineSurface(bufPnts, 3, 8, GeomAbs_G2, 0.001);
  GeomAPI_PointsToBSplineSurface bufApi = GeomAPI_PointsToBSplineSurface(bufPnts, 3, 3, GeomAbs_G2, 0.001);
  *surface = bufApi.Surface();
}

void createSurface(Handle(Geom_BSplineSurface) lowerSurface, Handle(Geom_BSplineSurface) upperSurface,
  int layerCnt, int idx, int nx, int ny, Handle(Geom_BSplineSurface)* surface)
{
  // upper 
  double upperUmin, upperUmax, upperVmin, upperVmax;
  upperSurface.get()->Bounds(upperUmin, upperUmax, upperVmin, upperVmax);

  // lower
  double lowerUmin, lowerUmax, lowerVmin, lowerVmax;
  lowerSurface.get()->Bounds(lowerUmin, lowerUmax, lowerVmin, lowerVmax);

  double upperU, upperV, lowerU, lowerV;
  gp_Pnt upperPnt, lowerPnt;
  TColgp_Array2OfPnt pnts = TColgp_Array2OfPnt(1, nx, 1, ny);
  // point
  for (int j = 0; j < ny; j++)
  {
    for (int i = 0; i < nx; i++)
    {
      upperU = (upperUmax - upperUmin) / (double)(nx - 1) * i + upperUmin;
      upperV = (upperVmax - upperVmin) / (double)(ny - 1) * j + upperVmin;
      lowerU = (lowerUmax - lowerUmin) / (double)(nx - 1) * i + lowerUmin;
      lowerV = (lowerVmax - lowerVmin) / (double)(ny - 1) * j + lowerVmin;
      upperPnt = upperSurface.get()->Value(upperU, upperV);
      lowerPnt = lowerSurface.get()->Value(lowerU, lowerV);
      double x = (upperPnt.X() - lowerPnt.X()) / (double)(layerCnt - 1) * (idx + 1) + lowerPnt.X();
      double y = (upperPnt.Y() - lowerPnt.Y()) / (double)(layerCnt - 1) * (idx + 1) + lowerPnt.Y();
      double z = (upperPnt.Z() - lowerPnt.Z()) / (double)(layerCnt - 1) * (idx + 1) + lowerPnt.Z();
      gp_Pnt pnt = gp_Pnt(x, y, z);
      pnts.SetValue(i + 1, j + 1, pnt);
    }
  }
  // surface
  //GeomAPI_PointsToBSplineSurface api = GeomAPI_PointsToBSplineSurface(pnts, 3, 8, GeomAbs_G2, 0.001);
  GeomAPI_PointsToBSplineSurface api = GeomAPI_PointsToBSplineSurface(pnts, 3, 3, GeomAbs_G2, 0.001);
  *surface = api.Surface();
}

Handle(Geom_BSplineSurface) createCurves(OCCTObjects^ myObj, int surfaceIdx, int wallIdx, int volumeGeomid,
  int* dispEdgeCnt, std::vector<Handle(Geom_Curve)>* curves, 
  std::vector<TopoDS_Face*>* faces, std::vector<Handle(Geom_BSplineSurface)>* volumes)
{
  Handle(Geom_BSplineSurface) surface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == surfaceIdx)
    {
      surface = myObj->surfaces[i];
      break;
    }
  }
  dispEdgeCnt = 0;
  // curve
  int pntCnt = 0;
  double endThickSum = 0.0;
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == volumeGeomid)
    {
      Handle(Geom_BSplineSurface) volume;
      TopoDS_Face* face;
      volume = (myObj->surfaces[i]);
      face = &(myObj->faces[i]);
      GeomInt_IntSS ss = GeomInt_IntSS(surface, volume, 0.001, Standard_False);
      for (int j = 0; j < ss.NbLines(); j++)
      {
        Handle(Geom_Curve) bufCurve = ss.Line(j + 1);
        curves->push_back(bufCurve);
        faces->push_back(face);
        volumes->push_back(volume);
      }
      dispEdgeCnt += ss.NbLines();
    }
  }
  return surface;
}

//[HandleProcessCorruptedStateExceptions]
std::vector<std::vector<std::pair<double, double>>> calcUvMap(
  std::vector<Handle(Geom_Curve)>* curves, std::vector<TopoDS_Face*>* faces, std::vector<Handle(Geom_BSplineSurface)>* volumes, Handle(Geom_BSplineSurface)* surface,
  double pointsDistance, double* real2uv)
{
  std::vector<std::vector<std::pair<double, double>>> curvesUv;
  Paths64 bufCurves;
  double sumDist = 0.0;
  double sumUv = 0.0;
  Standard_Real boundUmin, boundUmax, boundVmin, boundVmax;
  surface->get()->Bounds(boundUmin, boundUmax, boundVmin, boundVmax);
  if (curves->size() > 0)
  {
    for (int k = 0; k < curves->size(); k++)
    {
      // calc length and div count
      double sum = 0.0;
      Geom_Curve* bufCurve = curves->at(k).get();
      TopoDS_Face* bufFace = faces->at(k);
      Geom_BSplineSurface* bufVolume = volumes->at(k).get();
      BRepTopAdaptor_FClass2d checkFace(*bufFace, Precision::Confusion());
      double first = bufCurve->FirstParameter();
      double last = bufCurve->LastParameter();
      gp_Pnt prePnt = bufCurve->Value(first);
      for (int j = 0; j < PRE_CALC_LEN_DIV; j++)
      {
        double prm = (last - first) / (double)PRE_CALC_LEN_DIV * (j + 1) + first;
        gp_Pnt nowPnt = bufCurve->Value(prm);
        double dist = calcDistance(prePnt, nowPnt);
        sum += dist;
        prePnt = nowPnt;
      }
      int cnt = ceil(sum / pointsDistance);
      if (cnt < 2) cnt = 2;
      // listup  uv param
      std::vector<gp_Pnt> listPnt;
      std::vector<std::pair<double, double>> listUv;
      for (int j = 0; j < cnt; j++)
      {
        double prm = (last - first) / (double)(cnt - 1) * (j + 1) + first;
        gp_Pnt pnt = bufCurve->Value(prm);
        // get surface uv
        GeomAPI_ProjectPointOnSurf apiSrf(pnt, *surface);
        GeomAPI_ProjectPointOnSurf apiVolume(pnt, bufVolume);
        Standard_Real srfU, srfV, volumeU, volumeV;
        gp_Pnt2d chk;
        bool nextFlag = false;
        try {
          apiSrf.LowerDistanceParameters(srfU, srfV);
          apiVolume.LowerDistanceParameters(volumeU, volumeV);
          chk = gp_Pnt2d(volumeU, volumeV);
          nextFlag = true;
        }
        catch (System::Runtime::InteropServices::SEHException^ e)
        {
          nextFlag = false;
        }
        if (nextFlag)
        {
          TopAbs_State state = checkFace.Perform(chk);
          if (state == TopAbs_IN || state == TopAbs_ON)
          {
            listPnt.push_back(pnt);
            listUv.push_back(std::pair(srfU, srfV));
          }
        }
      }
      if (listUv.size() > 0)
      {
        curvesUv.push_back(std::vector<std::pair<double, double>>());
        for (int j = 0; j < listUv.size(); j++)
        {
          curvesUv[curvesUv.size() - 1].push_back(listUv[j]);
        }
        for (int j = 1; j < listPnt.size(); j++)
        {
          sumDist += listPnt[j].Distance(listPnt[j - 1]);
          sumUv += sqrt((listUv[j].first - listUv[j - 1].first) * (listUv[j].first - listUv[j - 1].first) +
            (listUv[j].second - listUv[j - 1].second) * (listUv[j].second - listUv[j - 1].second));
        }
      }
    }
    *real2uv = sumUv / sumDist;
  }
  return curvesUv;
}

std::vector<std::vector<std::pair<double, double>>> sortUvMap(
  std::vector<std::vector<std::pair<double, double>>> input, double real2uv, double pointsDistance)
{
  std::vector<std::pair<double, double>> points;
  std::vector<std::vector<std::pair<double, double>>> output;
  for (int k = 0; k < input.size(); k++)
  {
    for (int j = 0; j < input[k].size(); j++)
    {
      points.push_back(input[k][j]);
    }
  }
  int cnt = points.size();
  bool* insertFlag = new bool[cnt];
  for (int i = 0; i < cnt; i++)
  {
    insertFlag[i] = true;
  }
  if (cnt > 0)
  {
    // sort
    std::vector<std::pair<double, double>> bufSort;
    bufSort.push_back(points[0]);
    double puLast = points[0].first;
    double pvLast = points[0].second;
    double puFirst = points[0].first;
    double pvFirst = points[0].second;
    double sumDist = 0.0;
    for (int i = 1; i < cnt; i++)
    {
      double minLastDist = DBL_MAX;
      double minFirstDist = DBL_MAX;
      int minLastIdx = -1;
      int minFirstIdx = -1;
      for (int j = 1; j < cnt; j++)
      {
        if (insertFlag[j])
        {
          double distLast = sqrt((points[j].first - puLast) * (points[j].first - puLast) +
            (points[j].second - pvLast) * (points[j].second - pvLast));
          double distFirst = sqrt((points[j].first - puFirst) * (points[j].first - puFirst) +
            (points[j].second - pvFirst) * (points[j].second - pvFirst));
          if (distFirst > distLast)
          {
            if (distLast <= minLastDist)
            {
              minLastDist = distLast;
              minLastIdx = j;
            }
          }
          else
          {
            if (distFirst <= minFirstDist)
            {
              minFirstDist = distFirst;
              minFirstIdx = j;
            }
          }
        }
      }
      if (minFirstDist > minLastDist)
      {
        bufSort.push_back(points[minLastIdx]);
        insertFlag[minLastIdx] = false;
        sumDist += minLastDist;
        puLast = points[minLastIdx].first;
        pvLast = points[minLastIdx].second;
      }
      else
      {
        bufSort.insert(bufSort.begin(), points[minFirstIdx]);
        insertFlag[minFirstIdx] = false;
        sumDist += minFirstDist;
        puFirst = points[minFirstIdx].first;
        pvFirst = points[minFirstIdx].second;
      }
    }
    // looping
    double pu = bufSort[0].first;
    double pv = bufSort[0].second;
    std::vector<std::pair<double, double>> sub1;
    std::vector<std::vector<std::pair<double, double>>> bufGroup;
    bufGroup.push_back(sub1);
    int lpIdx = 0;
    bufGroup[lpIdx].push_back(bufSort[0]);
    for (int j = 1; j < cnt; j++)
    {
      double nu = bufSort[j].first;
      double nv = bufSort[j].second;
      double dist2 = sqrt((nu - pu) * (nu - pu) + (nv - pv) * (nv - pv));
      // If they are further apart than the average point-to-point, recognize them as separate loops.
      if (dist2 > sumDist / cnt * 10.0)//2.0) // change to parameter?
      {
        std::vector<std::pair<double, double>> sub2;
        bufGroup.push_back(sub2);
        lpIdx++;
      }
      bufGroup[lpIdx].push_back(bufSort[j]);
      pu = nu;
      pv = nv;
    }
    for (int k = bufGroup.size() - 1; k >= 0; k--)
    {
      if (bufGroup[k].size() == 1)
      {
        bufGroup.erase(bufGroup.begin() + k);
      }
    }
    // must loop
    for (int k = 0; k < bufGroup.size(); k++)
    {
      double sumU = 0.0;
      double sumV = 0.0;
      for (int j = 0; j < bufGroup[k].size(); j++)
      {
        sumU += bufGroup[k][j].first;
        sumV += bufGroup[k][j].second;
      }
      double aveU = sumU / bufGroup[k].size();
      double aveV = sumV / bufGroup[k].size();
      int sumTrue = 0;
      int sumFalse = 0;
      double preAngle = atan2(bufGroup[k][0].second - aveV, bufGroup[k][0].first - aveU);
      for (int j = 1; j < bufGroup[k].size(); j++)
      {
        double nowAngle = atan2(bufGroup[k][j].second - aveV, bufGroup[k][j].first - aveU);
        (preAngle < nowAngle) ? sumTrue++ : sumFalse++;
      }
      if (sumTrue < sumFalse)
      {
        std::reverse(bufGroup[k].begin(), bufGroup[k].end());
      }
    }
    // unique
    std::vector<std::vector<std::pair<double, double>>> bufUnique;
    for (int k = 0; k < bufGroup.size(); k++)
    {
      double pu = bufGroup[k][0].first;
      double pv = bufGroup[k][0].second;
      std::vector<std::pair<double, double>> sub3;
      bufUnique.push_back(sub3);
      for (int j = 0; j < bufGroup[k].size(); j++)
      {
        double nu = bufGroup[k][j].first;
        double nv = bufGroup[k][j].second;
        if (pu != nu || pv != nv)
        {
          bufUnique[k].push_back(std::pair(nu, nv));
        }
      }
    }
    //output = bufGroup;
    output = bufUnique;
    return output;
  }
  return output;
}

std::vector<PointData> rotateWallDatas(std::vector<PointData> input, gp_Pnt prePnt)
{
  std::vector<PointData> output;
  // check distance
  int minIdx = 0;
  double minDist = DBL_MAX;
  if (input.size() > 0)
  {
    for (int i = 0; i < input.size(); i++)
    {
      double dist = prePnt.Distance(input[i].pnt);
      if (dist < minDist)
      {
        minIdx = i;
        minDist = dist;
      }
    }
  }
  // rotate
  if (input.size() > 0)
  {
    for (int i = 0; i < input.size(); i++)
    {
      int idx = i + minIdx;
      if (idx >= input.size()) idx -= input.size();
      output.push_back(input[idx]);
      if (i == 0)
      {
        output[i].type = PrintType::OuterWallStart;
      }
      else if (i == input.size() - 1)
      {
        output[i].type = PrintType::OuterWallEnd;
      }
    }
  }

  return output;
}

std::vector<PointData> createWallPointData(std::vector<std::vector<std::pair<double, double>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm)
{
  std::vector<PointData> list;
  if (input.size() > 0)
  {
    for (int k = 0; k < input.size(); k++)
    {
      if (input[k].size() > 0)
      {
        for (int j = 0; j < input[k].size(); j++)
        {
          PrintType type;
          if (j == 0)
          {
            type = PrintType::OuterWallStart;
          }
          else if (j == input[k].size() - 1)
          {
            type = PrintType::OuterWallEnd;
          }
          else
          {
            type = PrintType::OuterWallMiddle;
          }
          PointData data = calcPointData_uv_with_under(input[k][j], surface, under, 
            type, width, blockIdx, layerIdx, k, j, defaultThickness, preNorm);
          list.push_back(data);
        }
      }
    }
  }
  *loopLastIdx = input.size();
  return list;
}

void writeCsv(char* path, std::vector<PointData> list)
{
  std::ofstream outputfile(path, std::ios::app);
  if (list.size() > 0)
  {
    for (int i = 0; i < list.size(); i++)
    {
      outputfile << list[i].type << "," << list[i].pnt.X() << "," << list[i].pnt.Y() << "," << list[i].pnt.Z() << ","
        << list[i].norm.X() << "," << list[i].norm.Y() << "," << list[i].norm.Z() << ","
        << list[i].thick << "," << list[i].width << ","
        << list[i].blockIdx << "," << list[i].layerIdx << "," << list[i].loopIdx << "," << list[i].curveIdx << ","
        << list[i].uv.first << "," << list[i].uv.second
        << std::endl;
    }
  }
  outputfile.close();
}

std::vector<int> checkWallDepth(Paths64 input)
{
  std::vector<int> nestDepth(input.size(), 0);
  if (input.size() > 0)
  {
    // depth check
    for (int k = 0; k < input.size(); k++)
    {
      for (int j = 0; j < input.size(); j++)
      {
        if (k != j)
        {
        int outerCnt = 0;
        int innerCnt = 0;
          for (int i = 0; i < input[k].size(); i++)
          {
            PointInPolygonResult res = PointInPolygon(input[k][i], input[j]);
            if (res == PointInPolygonResult::IsInside)
            {
              innerCnt++;
            }
            else if (res == PointInPolygonResult::IsOutside)
            {
              outerCnt++;
            }
          }
          if (outerCnt < innerCnt)
          {
            nestDepth[k]++;
          }
        }
      }
    }
  }
  return nestDepth;
}

std::vector<std::vector<std::vector<std::pair<double, double>>>> calcInfillByGridUv(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent, 
  Handle(Geom_BSplineSurface) surface)
{
  std::vector<std::vector<std::vector<std::pair<double, double>>>> output;
  if (input.size() > 0)
  {
    // surface check and crosslines
    double umin, umax, vmin, vmax;
    surface.get()->Bounds(umin, umax, vmin, vmax);
    Paths64 lines;
    double linePitch = width * real2uv / percent;
    double pntPitch = pointsDistance * real2uv;
    bool lineFlag = true;
    double u = umin;
    while (u < umax)
    {
      Path64 path;
      if (lineFlag)
      {
        path.push_back(Point64(u * CLIPPER_KVAL, vmin * CLIPPER_KVAL));
        path.push_back(Point64(u * CLIPPER_KVAL, vmax * CLIPPER_KVAL));
        lineFlag = false;
      }
      else
      {
        path.push_back(Point64(u * CLIPPER_KVAL, vmax * CLIPPER_KVAL));
        path.push_back(Point64(u * CLIPPER_KVAL, vmin * CLIPPER_KVAL));
        lineFlag = true;
      }
      lines.push_back(path);
      u += linePitch;
    }
    double v = vmin;
    while (v < vmax)
    {
      Path64 path;
      if (lineFlag)
      {
        path.push_back(Point64(umin * CLIPPER_KVAL, v * CLIPPER_KVAL));
        path.push_back(Point64(umax * CLIPPER_KVAL, v * CLIPPER_KVAL));
        lineFlag = false;
      }
      else
      {
        path.push_back(Point64(umax * CLIPPER_KVAL, v * CLIPPER_KVAL));
        path.push_back(Point64(umin * CLIPPER_KVAL, v * CLIPPER_KVAL));
        lineFlag = true;
      }
      lines.push_back(path);
      v += linePitch;
    }

    // init clipper
    Paths64 bufPaths;
    for (int k = 0; k < input.size(); k++)
    {
      Path64 path;
      if (input[k].size() > 0)
      {
        for (int i = 0; i < input[k].size(); i++)
        {
          path.push_back(Point64(input[k][i].first * CLIPPER_KVAL, input[k][i].second * CLIPPER_KVAL));
        }
        path.push_back(Point64(input[k][0].first * CLIPPER_KVAL, input[k][0].second * CLIPPER_KVAL));
        bufPaths.push_back(path);
      }
    }

    // depth check
    std::vector<int> depths = checkWallDepth(bufPaths);
    int runCnt = 0;
    Paths64 subjects = lines;
    int targetDepth = 0;
    while (runCnt < bufPaths.size())
    {
      Paths64 clips;
      for (int k = 0; k < bufPaths.size(); k++)
      {
        if (depths[k] == targetDepth)
        {
          // create infill for target loop
          ClipperOffset offset;
          offset.AddPath(bufPaths[k], JoinType::Square, EndType::Butt);
          Paths64 offsets;
          if (targetDepth % 2 == 0)
          {
            offset.Execute(-width * real2uv *  CLIPPER_KVAL, offsets);
            for (int j = 0; j < offsets.size(); j++)
            {
              PointInPolygonResult res = PointInPolygon(offsets[j][0], bufPaths[k]);
              if (res == PointInPolygonResult::IsInside)
              {
                clips.push_back(offsets[j]);
              }
            }
          }
          else
          {
            offset.Execute(width * real2uv * CLIPPER_KVAL, offsets);
            for (int j = 0; j < offsets.size(); j++)
            {
              PointInPolygonResult res = PointInPolygon(offsets[j][0], bufPaths[k]);
              if (res == PointInPolygonResult::IsOutside)
              {
                clips.push_back(offsets[j]);
              }
            }
          }
          runCnt++;
        }
      }
      if (targetDepth == 0)
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(subjects);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Intersection, FillRule::NonZero, solution, open_solution);
        subjects = open_solution;
      }
      else if(targetDepth % 2 == 1)
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(subjects);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Difference, FillRule::NonZero, solution, open_solution);
        subjects = open_solution;
      }
      else 
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(lines);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Intersection, FillRule::NonZero, solution, open_solution);
        Clipper64 clipper2;
        clipper2.AddOpenSubject(open_solution);
        clipper2.AddOpenSubject(subjects);
        Paths64 solution2;
        Paths64 open_solution2;
        clipper2.Execute(ClipType::Union, FillRule::NonZero, solution2, open_solution2);
        subjects = open_solution2;
      }
      targetDepth++;
    }
    // output
    if (subjects.size() > 0)
    {
      for (int k = 0; k < subjects.size(); k++)
      {
        std::vector<std::vector<std::pair<double, double>>> sub1;
        output.push_back(sub1);
        for (int j = 0; j < subjects[k].size() / 2; j++)
        {
          std::vector<std::pair<double, double>> sub2;
          output[k].push_back(sub2);
          double stu = subjects[k][j * 2 + 0].x / (double)CLIPPER_KVAL;
          double stv = subjects[k][j * 2 + 0].y / (double)CLIPPER_KVAL;
          double edu = subjects[k][j * 2 + 1].x / (double)CLIPPER_KVAL;
          double edv = subjects[k][j * 2 + 1].y / (double)CLIPPER_KVAL;
          double dist = sqrt((edu - stu) * (edu - stu) + (edv - stv) * (edv - stv));
          int cnt = ceil(dist / real2uv / pointsDistance);
          if (cnt < 2) cnt = 2;
          for (int i = 0; i < cnt; i++)
          {
            double u = (edu - stu) * i / (double)(cnt - 1) + stu;
            double v = (edv - stv) * i / (double)(cnt - 1) + stv;
            output[k][j].push_back(std::pair(u, v));
          }
        }
      }
    }
  }
  return output;
}

std::vector<std::vector<std::vector<std::pair<double, double>>>> calcInfillByGridXyz(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent,
  Handle(Geom_BSplineSurface) surface, int underInfill, int nowInfill, OCCTObjects^ myObj)
{
  std::vector<std::vector<std::vector<std::pair<double, double>>>> output;
  if (input.size() > 0)
  {
    // infill patern on surface
    Paths64 lines;
    if (underInfill == 0)
    {
      int nowIdx = -nowInfill / PRE_CALC_LEN_DIV - 1;
      double pitch = width / percent;
      double x = myObj->bboxXmin;
      bool flag = true;
      while (x < myObj->bboxXmax)
      {
        int cnt = ceil((myObj->bboxYmax - myObj->bboxYmin) / pointsDistance);
        Path64 line;
        for (int i = 0; i < cnt; i++)
        {
          double stY, edY;
          if (flag)
          {
            stY = i / (double)cnt * (myObj->bboxYmax - myObj->bboxYmin) + myObj->bboxYmin;
            edY = (i + 1) / (double)cnt * (myObj->bboxYmax - myObj->bboxYmin) + myObj->bboxYmin;
          }
          else
          {
            stY = (cnt - i - 1) / (double)cnt * (myObj->bboxYmax - myObj->bboxYmin) + myObj->bboxYmin;
            edY = (cnt - i - 2) / (double)cnt * (myObj->bboxYmax - myObj->bboxYmin) + myObj->bboxYmin;
          }
          myObj->infillStPnt[nowIdx][myObj->infillLines + i] = gp_Pnt(x, stY, 0.0);
          myObj->infillEdPnt[nowIdx][myObj->infillLines + i] = gp_Pnt(x, edY, 0.0);
          // calc uv
          Standard_Real stU, stV, edU, edV;
          GeomAPI_ProjectPointOnSurf apiSt(myObj->infillStPnt[nowIdx][myObj->infillLines + i], surface);
          apiSt.LowerDistanceParameters(stU, stV);
          GeomAPI_ProjectPointOnSurf apiEd(myObj->infillEdPnt[nowIdx][myObj->infillLines + i], surface);
          apiEd.LowerDistanceParameters(edU, edV);
          if (i == 0)
          {
            line.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          }
          line.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
        }
        lines.push_back(line);
        myObj->infillLines += cnt;
        x += pitch;
        flag = !flag;
      }
      double y = myObj->bboxYmin;
      while (y < myObj->bboxYmax)
      {
        int cnt = ceil((myObj->bboxXmax - myObj->bboxXmin) / pointsDistance);
        Path64 line;
        for (int i = 0; i < cnt; i++)
        {
          double stX, edX;
          if (flag)
          {
            stX = i / (double)cnt * (myObj->bboxXmax - myObj->bboxXmin) + myObj->bboxXmin;
            edX = (i + 1) / (double)cnt * (myObj->bboxXmax - myObj->bboxXmin) + myObj->bboxXmin;
          }
          else
          {
            stX = (cnt - i - 1) / (double)cnt * (myObj->bboxXmax - myObj->bboxXmin) + myObj->bboxXmin;
            edX = (cnt - i - 2) / (double)cnt * (myObj->bboxXmax - myObj->bboxXmin) + myObj->bboxXmin;
          }
          myObj->infillStPnt[nowIdx][myObj->infillLines + i] = gp_Pnt(stX, y, 0.0);
          myObj->infillEdPnt[nowIdx][myObj->infillLines + i] = gp_Pnt(edX, y, 0.0);
          // calc uv
          Standard_Real stU, stV, edU, edV;
          GeomAPI_ProjectPointOnSurf apiSt(myObj->infillStPnt[nowIdx][myObj->infillLines + i], surface);
          apiSt.LowerDistanceParameters(stU, stV);
          GeomAPI_ProjectPointOnSurf apiEd(myObj->infillEdPnt[nowIdx][myObj->infillLines + i], surface);
          apiEd.LowerDistanceParameters(edU, edV);
          if (i == 0)
          {
            line.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          }
          line.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
        }
        lines.push_back(line);
        myObj->infillLines += cnt;
        y += pitch;
        flag = !flag;
      }
    }
    else
    {
      int underIdx = -underInfill / PRE_CALC_LEN_DIV - 1;
      int nowIdx = -nowInfill / PRE_CALC_LEN_DIV - 1;
      for (int i = 0; i < myObj->infillLines; i++)
      {
        Standard_Real stU, stV, edU, edV;
        GeomAPI_ProjectPointOnSurf apiSt(myObj->infillStPnt[underIdx][i], surface);
        apiSt.LowerDistanceParameters(stU, stV);
        GeomAPI_ProjectPointOnSurf apiEd(myObj->infillEdPnt[underIdx][i], surface);
        apiEd.LowerDistanceParameters(edU, edV);
        myObj->infillStPnt[nowIdx][i] = surface.get()->Value(stU, stV);
        myObj->infillEdPnt[nowIdx][i] = surface.get()->Value(edU, edV);
        Path64 line;
        line.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
        line.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
        lines.push_back(line);
      }
    }

    // init clipper
    Paths64 bufPaths;
    for (int k = 0; k < input.size(); k++)
    {
      Path64 path;
      if (input[k].size() > 0)
      {
        for (int i = 0; i < input[k].size(); i++)
        {
          path.push_back(Point64(input[k][i].first * CLIPPER_KVAL, input[k][i].second * CLIPPER_KVAL));
        }
        path.push_back(Point64(input[k][0].first * CLIPPER_KVAL, input[k][0].second * CLIPPER_KVAL));
        bufPaths.push_back(path);
      }
    }

    // depth check
    std::vector<int> depths = checkWallDepth(bufPaths);
    int runCnt = 0;
    Paths64 subjects = lines;
    int targetDepth = 0;
    while (runCnt < bufPaths.size())
    {
      Paths64 clips;
      for (int k = 0; k < bufPaths.size(); k++)
      {
        if (depths[k] == targetDepth)
        {
          // create infill for target loop
          ClipperOffset offset;
          offset.AddPath(bufPaths[k], JoinType::Square, EndType::Butt);
          Paths64 offsets;
          if (targetDepth % 2 == 0)
          {
            offset.Execute(-width * real2uv * CLIPPER_KVAL, offsets);
            for (int j = 0; j < offsets.size(); j++)
            {
              PointInPolygonResult res = PointInPolygon(offsets[j][0], bufPaths[k]);
              if (res == PointInPolygonResult::IsInside)
              {
                clips.push_back(offsets[j]);
              }
            }
          }
          else
          {
            offset.Execute(width * real2uv * CLIPPER_KVAL, offsets);
            for (int j = 0; j < offsets.size(); j++)
            {
              PointInPolygonResult res = PointInPolygon(offsets[j][0], bufPaths[k]);
              if (res == PointInPolygonResult::IsOutside)
              {
                clips.push_back(offsets[j]);
              }
            }
          }
          runCnt++;
        }
      }
      if (targetDepth == 0)
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(subjects);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Intersection, FillRule::NonZero, solution, open_solution);
        subjects = open_solution;
      }
      else if (targetDepth % 2 == 1)
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(subjects);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Difference, FillRule::NonZero, solution, open_solution);
        subjects = open_solution;
      }
      else
      {
        Clipper64 clipper;
        clipper.AddOpenSubject(lines);
        clipper.AddClip(clips);
        Paths64 solution;
        Paths64 open_solution;
        clipper.Execute(ClipType::Intersection, FillRule::NonZero, solution, open_solution);
        Clipper64 clipper2;
        clipper2.AddOpenSubject(open_solution);
        clipper2.AddOpenSubject(subjects);
        Paths64 solution2;
        Paths64 open_solution2;
        clipper2.Execute(ClipType::Union, FillRule::NonZero, solution2, open_solution2);
        subjects = open_solution2;
      }
      targetDepth++;
    }

    // output
    if (underInfill == 0)
    {
      if (subjects.size() > 0)
      {
        for (int k = 0; k < subjects.size(); k++)
        {
          std::vector<std::vector<std::pair<double, double>>> sub1;
          output.push_back(sub1);
          std::vector<std::pair<double, double>> sub2;
          output[k].push_back(sub2);
          for (int j = 0; j < subjects[k].size() / 2; j++)
          {
            double stu = subjects[k][j * 2 + 0].x / (double)CLIPPER_KVAL;
            double stv = subjects[k][j * 2 + 0].y / (double)CLIPPER_KVAL;
            double edu = subjects[k][j * 2 + 1].x / (double)CLIPPER_KVAL;
            double edv = subjects[k][j * 2 + 1].y / (double)CLIPPER_KVAL;
            double dist = sqrt((edu - stu) * (edu - stu) + (edv - stv) * (edv - stv));
            int cnt = ceil(dist / real2uv / pointsDistance);
            if (cnt < 2) cnt = 2;
            for (int i = 0; i < cnt; i++)
            {
              double u = (edu - stu) * i / (double)(cnt - 1) + stu;
              double v = (edv - stv) * i / (double)(cnt - 1) + stv;
              output[k][0].push_back(std::pair(u, v));
            }
          }
        }
      }
    }
    else 
    {
      // sort
      if (subjects.size() > 1)
      {
        bool* flag = new bool[subjects.size()];
        int flagCnt = 0;
        int lineCnt = 0;
        for (int k = 0; k < subjects.size(); k++) flag[k] = true;
        std::vector<std::vector<std::pair<double, double>>> sub1;
        output.push_back(sub1);
        std::vector<std::pair<double, double>> sub2;
        output[0].push_back(sub2);
        double stu = subjects[0][0].x / (double)CLIPPER_KVAL;
        double stv = subjects[0][0].y / (double)CLIPPER_KVAL;
        double edu = subjects[0][1].x / (double)CLIPPER_KVAL;
        double edv = subjects[0][1].y / (double)CLIPPER_KVAL;
        output[0][lineCnt].push_back(std::pair(stu, stv));
        output[0][lineCnt].push_back(std::pair(edu, edv));
        std::pair<double, double> preEd = std::pair(edu, edv);
        flag[0] = false;
        flagCnt = 1;
        while (flagCnt != subjects.size())
        {
          double minDist = DBL_MAX;
          int minIdx = 0;
          std::pair<double, double> minSt = preEd;
          std::pair<double, double> minEd = preEd;
          for (int k = 1; k < subjects.size(); k++)
          {
            if (flag[k])
            {
              double stu = subjects[k][0].x / (double)CLIPPER_KVAL;
              double stv = subjects[k][0].y / (double)CLIPPER_KVAL;
              double edu = subjects[k][1].x / (double)CLIPPER_KVAL;
              double edv = subjects[k][1].y / (double)CLIPPER_KVAL;
              double distSt = sqrt((preEd.first - stu) * (preEd.first - stu)
                + (preEd.second - stv) * (preEd.second - stv));
              double distEd = sqrt((preEd.first - edu) * (preEd.first - edu)
                + (preEd.second - edv) * (preEd.second - edv));
              if (distSt < distEd)
              {
                if (distSt < minDist)
                {
                  minDist = distSt;
                  minIdx = k;
                  minSt = std::pair(stu, stv);
                  minEd = std::pair(edu, edv);
                }
              }
              else
              {
                if (distEd < minDist)
                {
                  minDist = distEd;
                  minIdx = k;
                  minSt = std::pair(edu, edv);
                  minEd = std::pair(stu, stv);
                }
              }
            }
          }
          flag[minIdx] = false;
          flagCnt++;
          if (minDist > 0.001)
          {
            std::vector<std::pair<double, double>> sub3;
            output[0].push_back(sub3);
            lineCnt++;
            output[0][lineCnt].push_back(minSt);
            output[0][lineCnt].push_back(minEd);
            preEd = minEd;
          }
          else
          {
            output[0][lineCnt].push_back(minSt);
            output[0][lineCnt].push_back(minEd);
            preEd = minEd;
          }
        }
      }
      else if (subjects.size() > 0)
      {
        std::vector<std::vector<std::pair<double, double>>> sub1;
        output.push_back(sub1);
        std::vector<std::pair<double, double>> sub2;
        output[0].push_back(sub2);
        double stu = subjects[0][0].x / (double)CLIPPER_KVAL;
        double stv = subjects[0][0].y / (double)CLIPPER_KVAL;
        double edu = subjects[0][1].x / (double)CLIPPER_KVAL;
        double edv = subjects[0][1].y / (double)CLIPPER_KVAL;
        output[0][0].push_back(std::pair(stu, stv));
        output[0][0].push_back(std::pair(edu, edv));
      }
    }
  }
  return output;
}

std::vector<PointData> rotateInfillDatas(std::vector<PointData> input, gp_Pnt prePnt)
{
  std::vector<PointData> output;
  // check distance
  int minIdx = 0;
  double minDist = DBL_MAX;
  if (input.size() > 0)
  {
    for (int i = 0; i < input.size(); i++)
    {
      double dist = prePnt.Distance(input[i].pnt);
      if (dist < minDist)
      {
        minIdx = i;
        minDist = dist;
      }
    }
  }
  // rotate
  if (input.size() > 0)
  {
    for (int i = 0; i < input.size(); i++)
    {
      int idx = i + minIdx;
      if (idx >= input.size()) idx -= input.size();
      output.push_back(input[idx]);
      if (i == 0)
      {
        output[i].type = PrintType::InfillStart;
      }
      else if (i == input.size() - 1)
      {
        output[i].type = PrintType::InfillEnd;
      }
    }
  }

  return output;
}

std::vector<PointData> createInfillPointData(
  std::vector<std::vector<std::vector<std::pair<double, double>>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx, int loopStartIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm)
{
  std::vector<PointData> list;
  if (input.size() > 0)
  {
    for (int k = 0; k < input.size(); k++)
    {
      if (input[k].size() > 0)
      {
        for (int j = 0; j < input[k].size(); j++)
        {
          if (input[k][j].size() > 0)
          {
            for (int i = 0; i < input[k][j].size(); i++)
            {
              PrintType type;
              if (i == 0)
              {
                type = PrintType::InfillStart;
              }
              else if (i == input[k][j].size() - 1)
              {
                type = PrintType::InfillEnd;
              }
              else
              {
                type = PrintType::InfillMiddle;
              }
              PointData data = calcPointData_uv_with_under(input[k][j][i], surface, under,
                type, width, blockIdx, layerIdx, k + loopStartIdx, j, defaultThickness, preNorm);
              list.push_back(data);
            }
          }
        }
      }
    }
  }
  *loopLastIdx = loopStartIdx + input.size();
  return list;
}

std::vector<std::vector<std::vector<std::pair<double, double>>>> calcBottom(
  std::vector<std::vector<std::pair<double, double>>> input,
  double real2uv, double pointsDistance, double width, double percent,
  Handle(Geom_BSplineSurface) surface, int direction)
{
  std::vector<std::vector<std::vector<std::pair<double, double>>>> output;
  if (input.size() > 0)
  {
    // surface check and crosslines
    double uRectMin, uRectMax, vRectMin, vRectMax;
    Paths64 bufPaths;
    Path64 rect;
    surface.get()->Bounds(uRectMin, uRectMax, vRectMin, vRectMax);
    rect.push_back(Point64(uRectMin * CLIPPER_KVAL, vRectMin * CLIPPER_KVAL));
    rect.push_back(Point64(uRectMax * CLIPPER_KVAL, vRectMin * CLIPPER_KVAL));
    rect.push_back(Point64(uRectMax * CLIPPER_KVAL, vRectMax * CLIPPER_KVAL));
    rect.push_back(Point64(uRectMin * CLIPPER_KVAL, vRectMax * CLIPPER_KVAL));
    rect.push_back(Point64(uRectMin * CLIPPER_KVAL, vRectMin * CLIPPER_KVAL));
    bufPaths.push_back(rect);

    // body size
    double uBodyMin = DBL_MAX;
    double uBodyMax = DBL_MIN;
    double vBodyMin = DBL_MAX;
    double vBodyMax = DBL_MIN;
    for (int i = 0; i < input.size(); i++)
    {
      for (int j = 0; j < input[i].size(); j++)
      {
        uBodyMin = min(input[i][j].first, uBodyMin);
        uBodyMax = max(input[i][j].first, uBodyMax);
        vBodyMin = min(input[i][j].second, vBodyMin);
        vBodyMax = max(input[i][j].second, vBodyMax);
      }
    }


    Paths64 lines;
    double linePitch = width * real2uv;
    double pntPitch = pointsDistance * real2uv;

    if (direction == 0)
    {
      bool lineFlag = true;
      double u = uBodyMin;
      while (u < uBodyMax)
      {
        Path64 path;
        if (lineFlag)
        {
          path.push_back(Point64(u * CLIPPER_KVAL, vBodyMin * CLIPPER_KVAL));
          path.push_back(Point64(u * CLIPPER_KVAL, vBodyMax * CLIPPER_KVAL));
          lineFlag = false;
        }
        else
        {
          path.push_back(Point64(u * CLIPPER_KVAL, vBodyMax * CLIPPER_KVAL));
          path.push_back(Point64(u * CLIPPER_KVAL, vBodyMin * CLIPPER_KVAL));
          lineFlag = true;
        }
        lines.push_back(path);
        u += linePitch;
      }
    }
    else if (direction == 1)
    {
      bool lineFlag = true;
      double v = vBodyMin;
      while (v < vBodyMax)
      {
        Path64 path;
        if (lineFlag)
        {
          path.push_back(Point64(uBodyMin * CLIPPER_KVAL, v * CLIPPER_KVAL));
          path.push_back(Point64(uBodyMax * CLIPPER_KVAL, v * CLIPPER_KVAL));
          lineFlag = false;
        }
        else
        {
          path.push_back(Point64(uBodyMax * CLIPPER_KVAL, v * CLIPPER_KVAL));
          path.push_back(Point64(uBodyMin * CLIPPER_KVAL, v * CLIPPER_KVAL));
          lineFlag = true;
        }
        lines.push_back(path);
        v += linePitch;
      }
    }
    else if (direction == 2)
    {
      bool lineFlag = true;
      double bufLinePitch = linePitch / cos(M_PI / 4.0);
      double stU = uBodyMin + bufLinePitch;
      double stV = vBodyMin;
      double edU = uBodyMin;
      double edV = vBodyMin + bufLinePitch;
      while (stU < uBodyMax || stV < vBodyMax || edU < uBodyMax || edV < vBodyMax)
      {
        Path64 path;
        if (lineFlag)
        {
          path.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          path.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
          lineFlag = false;
        }
        else
        {
          path.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
          path.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          lineFlag = true;
        }
        lines.push_back(path);
        if (stU < uBodyMax)
        {
          stU += bufLinePitch;
        }
        else
        {
          stV += bufLinePitch;
        }
        if (edV < vBodyMax)
        {
          edV += bufLinePitch;
        }
        else
        {
          edU += bufLinePitch;
        }
      }
    }
    else if (direction == 3)
    {
      bool lineFlag = true;
      double bufLinePitch = linePitch / cos(M_PI / 4.0);
      double stU = uBodyMin + bufLinePitch;
      double stV = vBodyMax;
      double edU = uBodyMin;
      double edV = vBodyMax - bufLinePitch;
      while (stU < uBodyMax || stV > vBodyMin || edU < uBodyMax || edV > vBodyMin)
      {
        Path64 path;
        if (lineFlag)
        {
          path.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          path.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
          lineFlag = false;
        }
        else
        {
          path.push_back(Point64(edU * CLIPPER_KVAL, edV * CLIPPER_KVAL));
          path.push_back(Point64(stU * CLIPPER_KVAL, stV * CLIPPER_KVAL));
          lineFlag = true;
        }
        lines.push_back(path);
        if (stU < uBodyMax)
        {
          stU += bufLinePitch;
        }
        else
        {
          stV -= bufLinePitch;
        }
        if (edV > vBodyMin)
        {
          edV -= bufLinePitch;
        }
        else
        {
          edU += bufLinePitch;
        }
      }
    }

    // init clipper
    for (int k = 0; k < input.size(); k++)
    {
      Path64 path;
      for (int i = 0; i < input[k].size(); i++)
      {
        path.push_back(Point64(input[k][i].first * CLIPPER_KVAL, input[k][i].second * CLIPPER_KVAL));
      }
      path.push_back(Point64(input[k][0].first* CLIPPER_KVAL, input[k][0].second* CLIPPER_KVAL));
      bufPaths.push_back(path);
    }

    // depth check
    std::vector<int> depths = checkWallDepth(bufPaths);
    int runCnt = 0;
    Paths64 subjects = lines;
    int targetDepth = 0;
    Clipper64 clipper;
    clipper.AddOpenSubject(subjects);
    clipper.AddClip(bufPaths);
    Paths64 solution;
    Paths64 open_solution;
    clipper.Execute(ClipType::Difference, FillRule::EvenOdd, solution, open_solution);
    subjects = open_solution;
    // output
    //for (int k = 0; k < input.size(); k++)
    //{
    //  std::vector<std::vector<std::pair<double, double>>> sub3;
    //  output.push_back(sub3);
    //  for (int j = 0; j < subjects[k].size(); j++)
    //  {
    //    std::vector<std::pair<double, double>> sub4;
    //    output[k].push_back(sub4);
    //    output[k][0].push_back(input[k][j]);
    //  }
    //}
    if (subjects.size() > 0)
    {
      for (int k = 0; k < subjects.size(); k++)
      {
        std::vector<std::vector<std::pair<double, double>>> sub1;
        output.push_back(sub1);
        for (int j = 0; j < subjects[k].size() / 2; j++)
        {
          std::vector<std::pair<double, double>> sub2;
          output[k].push_back(sub2);
          double stu = subjects[k][j * 2 + 0].x / (double)CLIPPER_KVAL;
          double stv = subjects[k][j * 2 + 0].y / (double)CLIPPER_KVAL;
          double edu = subjects[k][j * 2 + 1].x / (double)CLIPPER_KVAL;
          double edv = subjects[k][j * 2 + 1].y / (double)CLIPPER_KVAL;
          double dist = sqrt((edu - stu) * (edu - stu) + (edv - stv) * (edv - stv));
          int cnt = ceil(dist / real2uv / pointsDistance);
          if (cnt < 2) cnt = 2;
          for (int i = 0; i < cnt; i++)
          {
            double u = (edu - stu) * i / (double)(cnt - 1) + stu;
            double v = (edv - stv) * i / (double)(cnt - 1) + stv;
            output[k][j].push_back(std::pair(u, v));
          }
        }
      }
    }
  }
  return output;
}

std::vector<PointData> createBottomPointData(
  std::vector<std::vector<std::vector<std::pair<double, double>>>> input,
  double width, double defaultThickness, int blockIdx, int layerIdx,
  Handle(Geom_BSplineSurface) surface, Handle(Geom_BSplineSurface) under, int* loopLastIdx, gp_Dir preNorm)
{
  std::vector<PointData> list;
  if (input.size() > 0)
  {
    for (int k = 0; k < input.size(); k++)
    {
      if (input[k].size() > 0)
      {
        for (int j = 0; j < input[k].size(); j++)
        {
          if (input[k][j].size() > 0)
          {
            for (int i = 0; i < input[k][j].size(); i++)
            {
              PrintType type;
              if (i == 0)
              {
                type = PrintType::OuterWallStart;
              }
              else if (i == input[k][j].size() - 1)
              {
                type = PrintType::OuterWallEnd;
              }
              else
              {
                type = PrintType::OuterWallMiddle;
              }
              PointData data = calcPointData_uv_with_under(input[k][j][i], surface, under,
                type, width, blockIdx, layerIdx, k, j, defaultThickness, preNorm);
              list.push_back(data);
            }
          }
        }
      }
    }
  }
  *loopLastIdx = input.size();
  return list;
}

std::vector<std::vector<std::pair<double, double>>> calcGapFill(
  std::vector<std::vector<std::pair<double, double>>> upperUvInner,
  std::vector<std::vector<std::pair<double, double>>> upperUvOuter,
  std::vector<std::vector<std::pair<double, double>>> lowerUvInner,
  std::vector<std::vector<std::pair<double, double>>> lowerUvOuter,
  Handle(Geom_BSplineSurface) upperSurface, Handle(Geom_BSplineSurface) lowerSurface, double upperReal2uv,
  double width, double gapPercent
)
{
  std::vector<std::vector<std::pair<double, double>>> subFillCurveUv;
  // check upper uv
  Paths64 upperPathsInner;
  for (int cu = 0; cu < upperUvInner.size(); cu++)
  {
    Path64 upperPathInner;
    for (int pu = 0; pu < upperUvInner[cu].size(); pu++)
    {
      int x = upperUvInner[cu][pu].first * CLIPPER_KVAL;
      int y = upperUvInner[cu][pu].second * CLIPPER_KVAL;
      upperPathInner.push_back(Point64(x, y));
    }
    upperPathsInner.push_back(upperPathInner);
  }
  Paths64 upperPathsOuter;
  for (int cu = 0; cu < upperUvOuter.size(); cu++)
  {
    Path64 upperPathOuter;
    for (int pu = 0; pu < upperUvOuter[cu].size(); pu++)
    {
      int x = upperUvOuter[cu][pu].first * CLIPPER_KVAL;
      int y = upperUvOuter[cu][pu].second * CLIPPER_KVAL;
      upperPathOuter.push_back(Point64(x, y));
    }
    upperPathsOuter.push_back(upperPathOuter);
  }
  // check lower uv by projection
  Paths64 lowerPathsInner;
  for (int cl = 0; cl < lowerUvInner.size(); cl++)
  {
    Path64 lowerPathInner;
    for (int pl = 0; pl < lowerUvInner[cl].size(); pl++)
    {
      gp_Pnt lowerPnt = lowerSurface.get()->Value(lowerUvInner[cl][pl].first, lowerUvInner[cl][pl].second);
      GeomAPI_ProjectPointOnSurf apiLower(lowerPnt, upperSurface);
      Standard_Real bufU, bufV;
      apiLower.LowerDistanceParameters(bufU, bufV);
      int x = bufU * CLIPPER_KVAL;
      int y = bufV * CLIPPER_KVAL;
      lowerPathInner.push_back(Point64(x, y));
    }
    lowerPathsInner.push_back(lowerPathInner);
  }
  Paths64 lowerPathsOuter;
  for (int cl = 0; cl < lowerUvOuter.size(); cl++)
  {
    Path64 lowerPathOuter;
    for (int pl = 0; pl < lowerUvOuter[cl].size(); pl++)
    {
      gp_Pnt lowerPnt = lowerSurface.get()->Value(lowerUvOuter[cl][pl].first, lowerUvOuter[cl][pl].second);
      GeomAPI_ProjectPointOnSurf apiLower(lowerPnt, upperSurface);
      Standard_Real bufU, bufV;
      apiLower.LowerDistanceParameters(bufU, bufV);
      int x = bufU * CLIPPER_KVAL;
      int y = bufV * CLIPPER_KVAL;
      lowerPathOuter.push_back(Point64(x, y));
    }
    lowerPathsOuter.push_back(lowerPathOuter);
  }

  // check: lower is outer, upper is inner
  int* lowerOutUpperRelate = new int[lowerPathsInner.size()];
  double* lowerOutUpperDist = new double[lowerPathsInner.size()];
  for (int cl = 0; cl < lowerPathsInner.size(); cl++)
  {
    lowerOutUpperRelate[cl] = -1;
    lowerOutUpperDist[cl] = DBL_MAX;
    Paths64 bufLower;
    bufLower.push_back(lowerPathsInner[cl]);
    for (int cu = 0; cu < upperPathsOuter.size(); cu++)
    {
      Paths64 bufUpper;
      bufUpper.push_back(upperPathsOuter[cu]);
      Clipper64 clipper;
      clipper.AddClip(bufLower);
      clipper.AddOpenSubject(bufUpper);
      Paths64 solution, openSolution;
      clipper.Execute(ClipType::Difference, FillRule::NonZero, solution, openSolution);
      if (openSolution.size() > 0)
      {
        for (int pl = 0; pl < lowerPathsInner[cl].size(); pl++)
        {
          for (int i = 0; i < openSolution.size(); i++)
          {
            for (int j = 0; j < openSolution[i].size(); j++)
            {
              double dist = sqrt((lowerPathsInner[cl][pl].x - openSolution[i][j].x) * (lowerPathsInner[cl][pl].x - openSolution[i][j].x)
                + (lowerPathsInner[cl][pl].y - openSolution[i][j].y) * (lowerPathsInner[cl][pl].y - openSolution[i][j].y));
              if (dist < lowerOutUpperDist[cl])
              {
                lowerOutUpperDist[cl] = dist;
                lowerOutUpperRelate[cl] = cu;
              }
            }
          }
        }
      }
    }
  }
  // check: lower is inner, upper is outer
  int* lowerInUpperRelate = new int[lowerPathsOuter.size()];
  double* lowerInUpperDist = new double[lowerPathsOuter.size()];
  for (int cl = 0; cl < lowerPathsOuter.size(); cl++)
  {
    lowerInUpperRelate[cl] = -1;
    lowerInUpperDist[cl] = DBL_MAX;
    Paths64 bufLower;
    bufLower.push_back(lowerPathsOuter[cl]);
    for (int cu = 0; cu < upperPathsInner.size(); cu++)
    {
      Paths64 bufUpper;
      bufUpper.push_back(upperPathsInner[cu]);
      Clipper64 clipper;
      clipper.AddClip(bufUpper);
      clipper.AddOpenSubject(bufLower);
      Paths64 solution, openSolution;
      clipper.Execute(ClipType::Difference, FillRule::NonZero, solution, openSolution);
      if (openSolution.size() > 0)
      {
        for (int pl = 0; pl < lowerPathsOuter[cl].size(); pl++)
        {
          for (int i = 0; i < openSolution.size(); i++)
          {
            for (int j = 0; j < openSolution[i].size(); j++)
            {
              double dist = sqrt((lowerPathsOuter[cl][pl].x - openSolution[i][j].x) * (lowerPathsOuter[cl][pl].x - openSolution[i][j].x)
                + (lowerPathsOuter[cl][pl].y - openSolution[i][j].y) * (lowerPathsOuter[cl][pl].y - openSolution[i][j].y));
              if (dist < lowerInUpperDist[cl])
              {
                lowerInUpperDist[cl] = dist;
                lowerInUpperRelate[cl] = cu;
              }
            }
          }
        }
      }
    }
  }

  // lower offset
  for (int cl = 0; cl < lowerPathsInner.size(); cl++)
  {
    bool flagOuter = false;
    bool flagInner = true;
    //bool flagOuter = true;
    //bool flagInner = false;
    int loopCnt = 1;
    while (flagOuter || flagInner)
    {
      double offset = width * gapPercent * upperReal2uv * CLIPPER_KVAL * loopCnt;
      /*// offset out
      if (lowerInUpperRelate[cl] == -1)
      {
        flagOuter = false;
      }
      else
      {
        Paths64 bufLower;
        bufLower.push_back(lowerPathsInner[cl]);
        Paths64 bufUpper;
        bufUpper.push_back(upperPathsOuter[lowerInUpperRelate[cl]]);
        // offset
        Paths64 offsetSolution = InflatePaths(bufLower, offset, JoinType::Square, EndType::Polygon);
        // rect
        Path64 rect;
        rect.push_back(Point64(LONG_MIN, LONG_MIN));
        rect.push_back(Point64(LONG_MIN, LONG_MAX));
        rect.push_back(Point64(LONG_MAX, LONG_MAX));
        rect.push_back(Point64(LONG_MAX, LONG_MIN));
        rect.push_back(Point64(LONG_MIN, LONG_MIN));
        Paths64 rects;
        rects.push_back(rect);
        Clipper64 clipperArea;
        clipperArea.AddClip(bufUpper);
        clipperArea.AddSubject(rects);
        Paths64 areaSolution, areaOpenSolution;
        clipperArea.Execute(ClipType::Difference, FillRule::NonZero, areaSolution, areaOpenSolution);
        // cut
        Clipper64 cutClipper;
        cutClipper.AddClip(areaSolution);
        cutClipper.AddOpenSubject(offsetSolution);
        Paths64 cutSolution, cutOpenSolution;
        cutClipper.Execute(ClipType::Difference, FillRule::NonZero, cutSolution, cutOpenSolution);
        // add
        if (cutOpenSolution.size() == 0)
        {
          flagOuter = false;
        }
        else
        {
          for (int i = 0; i < cutOpenSolution.size(); i++)
          {
            std::vector<std::pair<double, double>> subsubFillCurveUv;
            if (loopCnt % 2 == 0)
            {
              for (int j = 0; j < cutOpenSolution[i].size(); j++)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            else
            {
              for (int j = cutOpenSolution[i].size() - 1; j >= 0; j--)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            subFillCurveUv.push_back(subsubFillCurveUv);
          }
        }
      }*/
      // offset in
      if (lowerOutUpperRelate[cl] == -1)
      {
        flagInner = false;
      }
      else
      {
        Paths64 bufLower;
        bufLower.push_back(lowerPathsInner[cl]);
        Paths64 bufUpper;
        bufUpper.push_back(upperPathsOuter[lowerOutUpperRelate[cl]]);
        // offset
        Paths64 offsetSolution = InflatePaths(bufLower, -offset, JoinType::Square, EndType::Polygon);
        // cut
        Clipper64 cutClipper;
        cutClipper.AddClip(bufUpper);
        cutClipper.AddOpenSubject(offsetSolution);
        Paths64 cutSolution, cutOpenSolution;
        cutClipper.Execute(ClipType::Difference, FillRule::NonZero, cutSolution, cutOpenSolution);
        // add
        if (cutOpenSolution.size() == 0)
        {
          flagInner = false;
        }
        else
        {
          for (int i = 0; i < cutOpenSolution.size(); i++)
          {
            std::vector<std::pair<double, double>> subsubFillCurveUv;
            if (loopCnt % 2 == 0)
            {
              for (int j = 0; j < cutOpenSolution[i].size(); j++)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            else
            {
              for (int j = cutOpenSolution[i].size() - 1; j >= 0; j--)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            subFillCurveUv.push_back(subsubFillCurveUv);
          }
        }
      }
      loopCnt++;
    }
  }
  for (int cl = 0; cl < lowerPathsOuter.size(); cl++)
  {
    bool flagOuter = true;
    bool flagInner = false;
    //bool flagOuter = false;
    //bool flagInner = true;
    int loopCnt = 1;
    while (flagOuter || flagInner)
    {
      double offset = width * gapPercent * upperReal2uv * CLIPPER_KVAL * loopCnt;
      // offset out
      if (lowerInUpperRelate[cl] == -1)
      {
        flagOuter = false;
      }
      else
      {
        Paths64 bufLower;
        bufLower.push_back(lowerPathsOuter[cl]);
        Paths64 bufUpper;
        bufUpper.push_back(upperPathsInner[lowerInUpperRelate[cl]]);
        // offset
        Paths64 offsetSolution = InflatePaths(bufLower, offset, JoinType::Square, EndType::Polygon);
        // rect
        Path64 rect;
        rect.push_back(Point64(LONG_MIN, LONG_MIN));
        rect.push_back(Point64(LONG_MIN, LONG_MAX));
        rect.push_back(Point64(LONG_MAX, LONG_MAX));
        rect.push_back(Point64(LONG_MAX, LONG_MIN));
        rect.push_back(Point64(LONG_MIN, LONG_MIN));
        Paths64 rects;
        rects.push_back(rect);
        Clipper64 clipperArea;
        clipperArea.AddClip(bufUpper);
        clipperArea.AddSubject(rects);
        Paths64 areaSolution, areaOpenSolution;
        clipperArea.Execute(ClipType::Difference, FillRule::NonZero, areaSolution, areaOpenSolution);
        // cut
        Clipper64 cutClipper;
        cutClipper.AddClip(areaSolution);
        cutClipper.AddOpenSubject(offsetSolution);
        Paths64 cutSolution, cutOpenSolution;
        cutClipper.Execute(ClipType::Difference, FillRule::NonZero, cutSolution, cutOpenSolution);
        // add
        if (cutOpenSolution.size() == 0)
        {
          flagOuter = false;
        }
        else
        {
          for (int i = 0; i < cutOpenSolution.size(); i++)
          {
            std::vector<std::pair<double, double>> subsubFillCurveUv;
            if (loopCnt % 2 == 0)
            {
              for (int j = 0; j < cutOpenSolution[i].size(); j++)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            else
            {
              for (int j = cutOpenSolution[i].size() - 1; j >= 0; j--)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            subFillCurveUv.push_back(subsubFillCurveUv);
          }
        }
      }
      /*// offset in
      if (lowerOutUpperRelate[cl] == -1)
      {
        flagInner = false;
      }
      else
      {
        Paths64 bufLower;
        bufLower.push_back(lowerPathsOuter[cl]);
        Paths64 bufUpper;
        bufUpper.push_back(upperPathsInner[lowerOutUpperRelate[cl]]);
        // offset
        Paths64 offsetSolution = InflatePaths(bufLower, -offset, JoinType::Square, EndType::Polygon);
        // cut
        Clipper64 cutClipper;
        cutClipper.AddClip(bufUpper);
        cutClipper.AddOpenSubject(offsetSolution);
        Paths64 cutSolution, cutOpenSolution;
        cutClipper.Execute(ClipType::Difference, FillRule::NonZero, cutSolution, cutOpenSolution);
        // add
        if (cutOpenSolution.size() == 0)
        {
          flagInner = false;
        }
        else
        {
          for (int i = 0; i < cutOpenSolution.size(); i++)
          {
            std::vector<std::pair<double, double>> subsubFillCurveUv;
            if (loopCnt % 2 == 0)
            {
              for (int j = 0; j < cutOpenSolution[i].size(); j++)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            else
            {
              for (int j = cutOpenSolution[i].size() - 1; j >= 0; j--)
              {
                double u = cutOpenSolution[i][j].x / (double)CLIPPER_KVAL;
                double v = cutOpenSolution[i][j].y / (double)CLIPPER_KVAL;
                subsubFillCurveUv.push_back(std::pair(u, v));
              }
            }
            subFillCurveUv.push_back(subsubFillCurveUv);
          }
        }
      }*/
      loopCnt++;
    }
  }
  return subFillCurveUv;
}

std::vector<std::vector<std::vector<std::pair<double, double>>>> calcOffsetUv(
  std::vector<std::vector<std::pair<double, double>>> input, int numWall,
  double real2uv, double width, bool outerWallFlag)
{
  std::vector<std::vector<std::vector<std::pair<double, double>>>> output
    = std::vector<std::vector<std::vector<std::pair<double, double>>>>();
  
  // check depth
  Paths64 depthCrvs;
  if (input.size() > 0)
  {
    for (int i = 0; i < input.size(); i++)
    {
      Path64 depthCrv;
      if (input[i].size() > 0)
      {
        for (int j = 0; j < input[i].size(); j++)
        {
          depthCrv.push_back(Point64(input[i][j].first * CLIPPER_KVAL, input[i][j].second * CLIPPER_KVAL));
        }
        depthCrvs.push_back(depthCrv);
      }
    }
  }
  std::vector<int> depths = checkWallDepth(depthCrvs);

  // offset
  for (int wall = 0; wall < numWall; wall++)
  {
    double offsetVal;
    if (outerWallFlag)
    {
      offsetVal = -(wall + 0.5) * width * real2uv * CLIPPER_KVAL;
    }
    else
    {
      offsetVal = -wall * width * real2uv * CLIPPER_KVAL;
    }
    if (wall == 0 && outerWallFlag)
    {
      output.push_back(input);
    }
    else
    {
      std::vector<std::vector<std::pair<double, double>>> subOffset;
      if (input.size() > 0)
      {
        for (int i = 0; i < input.size(); i++)
        {
          Paths64 crvs;
          Path64 crv;
          if (input[i].size() > 0)
          {
            for (int j = 0; j < input[i].size(); j++)
            {
              crv.push_back(Point64(input[i][j].first * CLIPPER_KVAL, input[i][j].second * CLIPPER_KVAL));
            }
            crvs.push_back(crv);
          }
          Paths64 offsetSolution;
          if (depths[i] % 2 == 0)
          {
            offsetSolution = InflatePaths(crvs, offsetVal, JoinType::Square, EndType::Polygon);
          }
          else
          {
            offsetSolution = InflatePaths(crvs, -offsetVal, JoinType::Square, EndType::Polygon);
          }
          for (int j = 0; j < offsetSolution.size(); j++)
          {
            std::vector<std::pair<double, double>> subsubOffset;
            for (int k = 0; k < offsetSolution[j].size(); k++)
            {
              int kk = j % 2 == 0 ? k : offsetSolution[j].size() - k - 1;
              subsubOffset.push_back(std::pair(
                offsetSolution[j][kk].x / (double)CLIPPER_KVAL, offsetSolution[j][kk].y / (double)CLIPPER_KVAL));
            }
            subOffset.push_back(subsubOffset);
          }
        }
      }
      output.push_back(subOffset);
    }
  }
  return output;
}

