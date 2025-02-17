#include "OCCTProxy.hpp"
#include <stdio.h>
#include <limits>

SliceObjects::SliceObjects(int len) 
{
  layers = new Handle(AIS_InteractiveObject)[len];
}
SliceObjects::~SliceObjects()
{
  delete[] layers;
}

int OCCTProxy::calcLayer(int layerCnt, 
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx, 
  int geomidUpper, int geomidLower, int geomIdBase, int nx, int ny, double pitch,
  int surfaceR, int surfaceG, int surfaceB)
{ 
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "pre slice start\n";

  // openmp
  omp_set_num_threads(omp_get_max_threads());
  omp_set_dynamic(0);


  // value set
  defaultThickness = pitch;

  // alignment lower surface
  Handle(Geom_BSplineSurface) lowerSurface;
  alignmentLayer(myObj, geomIdBase, geomidLower, nx, ny, &lowerSurface);
  Handle(Geom_BSplineSurface)* bufLowerSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomidLower)
    {
      myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
      myObj->geomids[i] = 0;
      break;
    }
  }
  BRepBuilderAPI_MakeFace makerLower(lowerSurface, Precision::Confusion());
  TopoDS_Face faceLower = makerLower.Face();
  Handle(AIS_InteractiveObject) targetLower = new AIS_Shape(faceLower);
  myAISContext()->Display(targetLower, Standard_False);
  myObj->setObject_Face(targetLower, faceLower, lowerSurface, geomidLower);

  // alignment upper surface
  Handle(Geom_BSplineSurface) upperSurface;
  alignmentLayer(myObj, geomidLower, geomidUpper, nx, ny, &upperSurface);
  Handle(Geom_BSplineSurface)* bufUpperSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomidUpper)
    {
      myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
      myObj->geomids[i] = 0;
      break;
    }
  }
  BRepBuilderAPI_MakeFace makerUpper(upperSurface, Precision::Confusion());
  TopoDS_Face faceUpper = makerUpper.Face();
  Handle(AIS_InteractiveObject) targetUpper = new AIS_Shape(faceUpper);
  myAISContext()->Display(targetUpper, Standard_False);
  myObj->setObject_Face(targetUpper, faceUpper, upperSurface, geomidUpper);


  // create surface list
  Handle(Geom_BSplineSurface)* surface;
  surface = new Handle(Geom_BSplineSurface)[layerCnt];
  TopoDS_Face* face;
  face = new TopoDS_Face[layerCnt];
  Quantity_Color aCol = Quantity_Color(surfaceR / 255.0, surfaceG / 255.0, surfaceB / 255.0, Quantity_TOC_RGB);
  Handle(AIS_InteractiveObject)* dispSurface = new Handle(AIS_InteractiveObject)[layerCnt];

  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "create surface start:" << idx << "," << layerCnt << std::endl;
    Handle(Geom_BSplineSurface)* bufSurface = new Handle(Geom_BSplineSurface);
    createSurface(lowerSurface, upperSurface, layerCnt, idx, nx, ny, bufSurface);
    surface[idx] = *bufSurface;
    BRepBuilderAPI_MakeFace makerFace(surface[idx], Precision::Confusion());
    face[idx] = makerFace.Face();
    dispSurface[idx] = new AIS_Shape(face[idx]);
    int bufIdx = -(idx + 1 + preSliceStartLayer) * IDX_PATERN;
    myObj->setObject_Face(dispSurface[idx], face[idx], surface[idx], bufIdx);
    surfaceIdx[idx] = bufIdx;
    wallIdx[idx] = bufIdx - 1;
    infillIdx[idx] = bufIdx - 2;
    supportIdx[idx] = bufIdx - 3;
    void* surface_ptr = surface[idx].get();
    std::cout << "create surface end:" << idx << "," << layerCnt << std::endl;
  }

  // redraw
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "draw surface start:" << idx << "," << layerCnt << std::endl;
    myAISContext()->Display(dispSurface[idx], Standard_False);
    myAISContext()->SetColor(dispSurface[idx], aCol, Standard_False);
    if (idx == 0)
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.75, Standard_False);
    }
    else
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.0, Standard_False);
    }
    myAISContext()->SetViewAffinity(dispSurface[idx], myView(), Standard_True);
    std::cout << "draw surface end:" << idx << "," << layerCnt << std::endl;
  }
  myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
  myAISContext()->UpdateCurrentViewer();

  // next
  preSliceStartLayer += layerCnt;
  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory
  delete[] surface;
  delete[] face;
  delete[] dispSurface;

	return layerCnt;
}

void OCCTProxy::SetObjectColor(int id, int r, int g, int b)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == id)
    {
      Quantity_Color aCol = Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB);
      myAISContext()->SetColor(myObj->objects[i], aCol, Standard_False);
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::SetSurfaceTransparent(int surface)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] <= 0)
    {
      if (myObj->geomids[i] == surface)
      {
        myAISContext()->SetTransparency(myObj->objects[i], 0.75, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_True);
      }
      else
      {
        myAISContext()->SetTransparency(myObj->objects[i], 1.0, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_False);
      }
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::SetSurfaceCurveTransparent(int surface, int outerWall, int infill)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] <= 0)
    {
      if (myObj->geomids[i] == outerWall)
      {
        myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_True);
      }
      else if (myObj->geomids[i] == infill)
      {
        myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_True);
      }
      else if (myObj->geomids[i] == surface)
      {
        myAISContext()->SetTransparency(myObj->objects[i], 0.75, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_True);
      }
      else
      {
        myAISContext()->SetTransparency(myObj->objects[i], 1.0, false);
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_False);
      }
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::DeleteSurfaceByTransparent(int id)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
      if (myObj->geomids[i] == id)
      {
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_False);
        myAISContext()->SetTransparency(myObj->objects[i], 1.0, false);
      }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::DeleteCurveByTransparent(int id)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
      if (myObj->geomids[i] == id)
      {
        myAISContext()->SetViewAffinity(myObj->objects[i], myView(), Standard_False);
        myAISContext()->SetTransparency(myObj->objects[i], 1.0, false);
      }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::createOffsetObject(double offsetVal, int orgGeomId, int newGeomId)
{
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "create offset object start" << std::endl;

  int cnt = myObj->getLength();
  Handle(Geom_BSplineSurface) bufVolume;
  //#pragma omp parallel for schedule(static)
  for (int i = 0; i < cnt; i++) 
  {
    if (myObj->geomids[i] == orgGeomId)
    {
      bufVolume = myObj->surfaces[i];
      std::cout << "offset:" << i << "," << cnt << std::endl;
      Handle(Geom_OffsetSurface) offset = new Geom_OffsetSurface(bufVolume, offsetVal);
      GeomConvert_ApproxSurface approx(offset.get(), 0.1, GeomAbs_C2, GeomAbs_C2, 3, 3, 1000, 0);
      if (approx.IsDone())
      {
        Handle(Geom_BSplineSurface) bufSurface = approx.Surface();
        // add object
        BRepBuilderAPI_MakeFace makerFace(bufSurface, Precision::Confusion());
        TopoDS_Face face = makerFace.Face();
        //#pragma omp critical
        {
          myObj->setObject_Face(new AIS_Shape(face), face, bufSurface, newGeomId);
        }
      }
    }
  }
  //#pragma omp barrier

  // console
  std::cout << "create offset object end" << std::endl;
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();
}

int OCCTProxy::calcPoints(char* path, int layerCnt, double pointsDistance, double width,
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, 
  array<int>^ underIdx, int volumeCnt, int volumeGeomid,
  double prePointX, double prePointY, double prePointZ, 
  double& lastPointX, double& lastPointY, double& lastPointZ,
  double preNormI, double preNormJ, double preNormK,
  double& lastNormI, double& lastNormJ, double& lastNormK,
  bool infillEnable, int infillType, double infillPercent, array<int>^ underInfillIdx,
  bool gapFillEnable, double gapFillPercent,
  bool outerWallFlag, int numWall, double wallPercent)
{
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "main slice start" << std::endl;

  gp_Pnt prePnt = gp_Pnt(prePointX, prePointY, prePointZ);
  gp_Dir preNorm = gp_Dir(preNormI, preNormJ, preNormK);

  // wall slice
  Handle(Geom_BSplineSurface)* surfaces = new Handle(Geom_BSplineSurface)[layerCnt];
  int* dispEdgeCnt = new int[layerCnt];
  std::vector<Handle(Geom_Curve)>* wallCurves = new std::vector<Handle(Geom_Curve)>[layerCnt];
  std::vector<TopoDS_Face*>* wallFaces = new std::vector<TopoDS_Face*>[layerCnt];
  std::vector<Handle(Geom_BSplineSurface)>* wallVolumes = new std::vector<Handle(Geom_BSplineSurface)>[layerCnt];
  #pragma omp parallel for schedule(static)
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "create curve start:" << idx << "," << layerCnt << std::endl;
    int bufCnt;
    std::vector<Handle(Geom_Curve)> bufCurves;
    std::vector<TopoDS_Face*> bufFaces;
    std::vector<Handle(Geom_BSplineSurface)> bufVolumes;
    Handle(Geom_BSplineSurface) bufSurface = createCurves(myObj, 
      surfaceIdx[idx], wallIdx[idx], volumeGeomid, &bufCnt, &bufCurves, &bufFaces, &bufVolumes);
    surfaces[idx] = bufSurface;
    wallCurves[idx] = bufCurves;
    wallFaces[idx] = bufFaces;
    wallVolumes[idx] = bufVolumes;
    std::cout << "create curve end:" << idx << "," << layerCnt << std::endl;
  }
  #pragma omp barrier

  // check first point
  int firstLayerIdx = 0;
  for (int idx = 0; idx < layerCnt; idx++)
  {
    if (wallCurves[idx].size() > 0)
    {
      firstLayerIdx = idx;
      break;
    }
  }
  GeomAPI_ProjectPointOnSurf api(gp_Pnt(prePointX, prePointY, prePointZ), surfaces[firstLayerIdx]);
  Standard_Real preU, preV;
  api.LowerDistanceParameters(preU, preV);

  // sort curve
  std::vector<PointData>* wallDatas = new std::vector<PointData>[layerCnt];
  std::vector<PointData>* infillDatas = new std::vector<PointData>[layerCnt];
  std::vector<PointData>* gapFillDatas = new std::vector<PointData>[layerCnt];
  std::vector<std::vector<std::vector<std::pair<double, double>>>>* offsetUvLib
    = new std::vector<std::vector<std::vector<std::pair<double, double>>>>[layerCnt];
  double* real2uvLib = new double[layerCnt];
  int* loopLastIdxLib = new int[layerCnt];
  #pragma omp parallel for schedule(static)
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "sort curve start:" << idx << "," << layerCnt << std::endl;
    Handle(Geom_BSplineSurface)* under;
    for (int i = 0; i < myObj->getLength(); i++) 
    {
      if (myObj->geomids[i] == underIdx[idx])
      {
        under = &(myObj->surfaces[i]);
        break;
      }
    }

    // wall slice
    double real2uv;
    std::vector<std::vector<std::pair<double, double>>> curvesUv = calcUvMap(
      &wallCurves[idx], &wallFaces[idx], &wallVolumes[idx], &surfaces[idx], pointsDistance, &real2uv);
    real2uvLib[idx] = real2uv;
    std::vector<std::vector<std::pair<double, double>>> loopsUv = sortUvMap(
      curvesUv, real2uv, pointsDistance);
    std::vector<std::vector<std::vector<std::pair<double, double>>>> offsetUv = 
      calcOffsetUv(loopsUv, numWall + 1, real2uv, width * wallPercent, false);
    int loopLastIdx;
    wallDatas[idx] = std::vector<PointData>();
    for (int i = 0; i < offsetUv.size() - 1; i++)
    {
      std::vector<PointData> wallData = createWallPointData(
        offsetUv[i],
        width, defaultThickness, volumeCnt, idx,
        surfaces[idx], *under, &loopLastIdx, preNorm);
      loopLastIdxLib[idx] = loopLastIdx;
      //wallDatas[idx] = wallData;
      for (int j = 0; j < wallData.size(); j++)
      {
        wallDatas[idx].push_back(wallData[j]);
      }
      if (wallData.size() > 0)
      {
        preNorm = wallData[wallData.size() - 1].norm;
      }
    }

    // draw wall curve by points
    for (int i = 0; i < offsetUv.size() - 1; i++)
    {
      std::vector<std::vector<std::pair<double, double>>> drawWallUv = offsetUv[i];
      offsetUvLib[idx].push_back(offsetUv[i]);
      if (drawWallUv.size() > 0)
      {
        for (int j = 0; j < drawWallUv.size(); j++)
        {
          if (drawWallUv[j].size() >= 2)
          {
            double u = drawWallUv[j][0].first;
            double v = drawWallUv[j][0].second;
            gp_Pnt prePnt = surfaces[idx].get()->Value(u, v);
            for (int i = 1; i < drawWallUv[j].size(); i++)
            {
              u = drawWallUv[j][i].first;
              v = drawWallUv[j][i].second;
              gp_Pnt nowPnt = surfaces[idx].get()->Value(u, v);
              if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
              {
                Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                Standard_Real u1 = 0.0;
                Standard_Real u2 = prePnt.Distance(nowPnt);
                Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
#pragma omp critical
                {
                  Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                  myAISContext()->Display(ais, Standard_False);
                  myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[idx]);
                }
              }
              prePnt = nowPnt;
            }
          }
        }
      }
    }
    std::cout << "sort curve end:" << idx << "," << layerCnt << std::endl;
  }
  #pragma omp barrier


  // calc surface fill
  if (gapFillEnable)
  {
    std::vector<std::vector<std::pair<double, double>>>* fillCurveUvLib
      = new std::vector<std::vector<std::pair<double, double>>>[layerCnt];
#pragma omp parallel for schedule(static)
    for (int idx = 1; idx < layerCnt; idx++)
    {
      std::cout << "calc fill start:" << idx << "," << layerCnt << std::endl;

      std::vector<std::vector<std::pair<double, double>>> baseUpperInner;
      std::vector<std::vector<std::pair<double, double>>> baseUpperOuter;
      for (int i = 0; i < offsetUvLib[idx][offsetUvLib[idx].size() - 1].size(); i++)
      {
        baseUpperInner.push_back(offsetUvLib[idx][offsetUvLib[idx].size() - 1][i]);
      }
      for (int i = 0; i < offsetUvLib[idx][0].size(); i++)
      {
        baseUpperOuter.push_back(offsetUvLib[idx][0][i]);
      }
      std::vector<std::vector<std::pair<double, double>>> baseLowerInner;
      std::vector<std::vector<std::pair<double, double>>> baseLowerOuter;
      for (int i = 0; i < offsetUvLib[idx - 1][offsetUvLib[idx - 1].size() - 1].size(); i++)
      {
        baseLowerInner.push_back(offsetUvLib[idx - 1][offsetUvLib[idx - 1].size() - 1][i]);
      }
      for (int i = 0; i < offsetUvLib[idx - 1][0].size(); i++)
      {
        baseLowerOuter.push_back(offsetUvLib[idx - 1][0][i]);
      }

      std::vector<std::vector<std::pair<double, double>>> subFillCurveUv =
        calcGapFill(baseUpperInner, baseUpperOuter, baseLowerInner, baseLowerOuter, surfaces[idx], surfaces[idx - 1], real2uvLib[idx],
          width, gapFillPercent);
      fillCurveUvLib[idx] = subFillCurveUv;
      int loopLastIdx;
      std::vector<PointData> gapFillData = createWallPointData(
        subFillCurveUv,
        width, defaultThickness, volumeCnt, idx,
        surfaces[idx], surfaces[idx - 1], &loopLastIdx, preNorm);
      loopLastIdxLib[idx] = loopLastIdx;
      gapFillDatas[idx] = gapFillData;
      std::cout << "calc fill end:" << idx << "," << layerCnt << std::endl;

      std::cout << "draw fill start:" << idx << "," << layerCnt << std::endl;
      if (fillCurveUvLib[idx].size() > 0)
      {
        for (int j = 0; j < fillCurveUvLib[idx].size(); j++)
        {
          if (fillCurveUvLib[idx][j].size() > 0)
          {
            double u = fillCurveUvLib[idx][j][0].first;
            double v = fillCurveUvLib[idx][j][0].second;
            gp_Pnt prePnt = surfaces[idx].get()->Value(u, v);
            for (int i = 0; i < fillCurveUvLib[idx][j].size(); i++)
            {
              u = fillCurveUvLib[idx][j][i].first;
              v = fillCurveUvLib[idx][j][i].second;
              gp_Pnt nowPnt = surfaces[idx].get()->Value(u, v);
              if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
              {
                Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                Standard_Real u1 = 0.0;
                Standard_Real u2 = prePnt.Distance(nowPnt);
                Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
#pragma omp critical
                {
                  Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                  myAISContext()->Display(ais, Standard_False);
                  myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[idx]);
                }
              }
              prePnt = nowPnt;
            }
          }
        }
      }
      std::cout << "draw fill end:" << idx << "," << layerCnt << std::endl;
    }
#pragma omp barrier
  }


  // create infill
  if (infillEnable)
  {
    for (int idx = 0; idx < layerCnt; idx++)
    {
      Handle(Geom_BSplineSurface)* under;
      for (int i = 0; i < myObj->getLength(); i++) {
        if (myObj->geomids[i] == underIdx[idx])
        {
          under = &(myObj->surfaces[i]);
          break;
        }
      }
      std::vector<std::vector<std::vector<std::pair<double, double>>>> infillUv;
      std::cout << "create infill start:" << idx << "," << layerCnt << std::endl;
      std::vector<std::vector<std::pair<double, double>>> baseUpper;
      for (int i = 0; i < offsetUvLib[idx].size(); i++)
      {
        std::vector<std::pair<double, double>> subBase = offsetUvLib[idx][i][offsetUvLib[idx][i].size() - 1];
        baseUpper.push_back(subBase);
      }
      if (infillType == 0)
      {
        infillUv = calcInfillByGridUv(
          baseUpper, real2uvLib[idx], pointsDistance, width, infillPercent, surfaces[idx]);
        std::vector<PointData> infillData = createInfillPointData(infillUv,
          width, defaultThickness, volumeCnt, idx, loopLastIdxLib[idx],
          surfaces[idx], *under, &loopLastIdxLib[idx], preNorm);
        infillDatas[idx] = infillData;
        int bufK = infillUv.size();
        if (bufK > 0)
        {
          int bufJ = infillUv[bufK - 1].size();
          if (bufJ > 0)
          {
            int bufI = infillUv[bufK - 1][bufJ - 1].size();
            if (bufI > 0)
            {
              preU = infillUv[bufK - 1][bufJ - 1][bufI - 1].first;
              preV = infillUv[bufK - 1][bufJ - 1][bufI - 1].second;
            }
          }
        }
      }
      else if (infillType == 1)
      {
        infillUv = calcInfillByGridXyz(
          baseUpper, real2uvLib[idx], pointsDistance, width, infillPercent, surfaces[idx],
          underInfillIdx[idx], infillIdx[idx], myObj);
        std::vector<PointData> infillData = createInfillPointData(infillUv,
          width, defaultThickness, volumeCnt, idx, loopLastIdxLib[idx],
          surfaces[idx], *under, &loopLastIdxLib[idx], preNorm);
        infillDatas[idx] = infillData;
        int bufK = infillUv.size();
        if (bufK > 0)
        {
          int bufJ = infillUv[bufK - 1].size();
          if (bufJ > 0)
          {
            int bufI = infillUv[bufK - 1][bufJ - 1].size();
            if (bufI > 0)
            {
              preU = infillUv[bufK - 1][bufJ - 1][bufI - 1].first;
              preV = infillUv[bufK - 1][bufJ - 1][bufI - 1].second;
            }
          }
        }
      }

      // draw infill
      std::vector<std::vector<std::vector<std::pair<double, double>>>> drawInfillUv = infillUv;
      if (drawInfillUv.size() > 0)
      {
        for (int k = 0; k < drawInfillUv.size(); k++)
        {
          if (drawInfillUv[k].size() > 0)
          {
            for (int j = 0; j < drawInfillUv[k].size(); j++)
            {
              if (drawInfillUv[k][j].size() >= 2)
              {
                // draw infill curve by points
                double u = drawInfillUv[k][j][0].first;
                double v = drawInfillUv[k][j][0].second;
                gp_Pnt prePnt = surfaces[idx].get()->Value(u, v);
                for (int i = 1; i < drawInfillUv[k][j].size(); i++)
                {
                  u = drawInfillUv[k][j][i].first;
                  v = drawInfillUv[k][j][i].second;
                  gp_Pnt nowPnt = surfaces[idx].get()->Value(u, v);
                  if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
                  {
                    Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                    Standard_Real u1 = 0.0;
                    Standard_Real u2 = prePnt.Distance(nowPnt);
                    Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
                    #pragma omp critical
                    {
                      Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                      myAISContext()->Display(ais, Standard_False);
                      myObj->setObject_Edge(ais, edge, trimmedCurve, infillIdx[idx]);
                    }
                  }
                  prePnt = nowPnt;
                }
              }
            }
          }
        }
      }
      std::cout << "create infill end:" << idx << "," << layerCnt << std::endl;
    }
  }

  prePnt = gp_Pnt(prePointX, prePointY, prePointZ);
  preNorm = gp_Dir(preNormI, preNormJ, preNormK);
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "write csv start:" << idx << "," << layerCnt << std::endl;
    std::vector<PointData> writeGapFill = gapFillDatas[idx];
    if (writeGapFill.size() > 0)
    {
      prePnt = writeGapFill[writeGapFill.size() - 1].pnt;
      preNorm = writeGapFill[writeGapFill.size() - 1].norm;
      writeCsv(path, writeGapFill);
    }
    std::vector<PointData> rotateWall = rotateWallDatas(wallDatas[idx], prePnt);
    if (rotateWall.size() > 0)
    {
      prePnt = rotateWall[rotateWall.size() - 1].pnt;
      preNorm = rotateWall[rotateWall.size() - 1].norm;
      writeCsv(path, rotateWall);
    }
    std::vector<PointData> rotateInfill = rotateInfillDatas(infillDatas[idx], prePnt);
    if (rotateInfill.size() > 0)
    {
      prePnt = rotateWall[rotateWall.size() - 1].pnt;
      preNorm = rotateWall[rotateWall.size() - 1].norm;
      writeCsv(path, rotateInfill);
    }
    std::cout << "write csv end:" << idx << "," << layerCnt << std::endl;
  }
  lastPointX = prePnt.X();
  lastPointY = prePnt.Y();
  lastPointZ = prePnt.Z();
  lastNormI = preNorm.X();
  lastNormJ = preNorm.Y();
  lastNormK = preNorm.Z();

  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory
  delete[] surfaces;
  delete[] dispEdgeCnt;
  delete[] wallCurves;
  delete[] wallFaces;
  delete[] wallVolumes;
  delete[] wallDatas;
  delete[] infillDatas;
  delete[] gapFillDatas;
  delete[] offsetUvLib;
  delete[] real2uvLib;
  delete[] loopLastIdxLib;

  return 0;
}

int OCCTProxy::calcBottomPoints(char* path, double pointsDistance, double width,
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx,
  array<int>^ underIdx, int volumeCnt, int volumeGeomid, int numWall, double wallPercent, int direction,
  double prePointX, double prePointY, double prePointZ, 
  double& lastPointX, double& lastPointY, double& lastPointZ,
  double preNormI, double preNormJ, double preNormK,
  double& lastNormI, double& lastNormJ, double& lastNormK)
{
  int layerCnt = 1;
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "main slice start\n";

  gp_Pnt prePnt = gp_Pnt(prePointX, prePointY, prePointZ);
  gp_Dir preNorm = gp_Dir(preNormI, preNormJ, preNormK);

  // wall slice
  Handle(Geom_BSplineSurface)* surfaces = new Handle(Geom_BSplineSurface)[layerCnt];
  int* dispEdgeCnt = new int[layerCnt];
  std::vector<Handle(Geom_Curve)>* wallCurves = new std::vector<Handle(Geom_Curve)>[layerCnt];
  std::vector<TopoDS_Face*>* wallFaces = new std::vector<TopoDS_Face*>[layerCnt];
  std::vector<Handle(Geom_BSplineSurface)>* wallVolumes = new std::vector<Handle(Geom_BSplineSurface)>[layerCnt];
#pragma omp parallel for schedule(static)
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "create curve start:" << idx << "," << layerCnt << std::endl;
    int bufCnt;
    std::vector<Handle(Geom_Curve)> bufCurves;
    std::vector<TopoDS_Face*> bufFaces;
    std::vector<Handle(Geom_BSplineSurface)> bufVolumes;
    Handle(Geom_BSplineSurface) bufSurface = createCurves(myObj,
      surfaceIdx[idx], wallIdx[idx], volumeGeomid, &bufCnt, &bufCurves, &bufFaces, &bufVolumes);
    surfaces[idx] = bufSurface;
    wallCurves[idx] = bufCurves;
    wallFaces[idx] = bufFaces;
    wallVolumes[idx] = bufVolumes;
    std::cout << "create curve end:" << idx << "," << layerCnt << std::endl;
  }
#pragma omp barrier

  // check first point
  int firstLayerIdx = 0;
  for (int idx = 0; idx < layerCnt; idx++)
  {
    if (wallCurves[idx].size() > 0)
    {
      firstLayerIdx = idx;
      break;
    }
  }
  GeomAPI_ProjectPointOnSurf api(gp_Pnt(prePointX, prePointY, prePointZ), surfaces[firstLayerIdx]);
  Standard_Real preU, preV;
  api.LowerDistanceParameters(preU, preV);

  // sort curve
  std::vector<PointData> bottomDatas = std::vector<PointData>();
  std::vector<std::vector<std::vector<std::pair<double, double>>>> offsetUv;
#pragma omp parallel for schedule(static)
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "sort curve start:" << idx << "," << layerCnt << std::endl;
    Handle(Geom_BSplineSurface)* under;
    for (int i = 0; i < myObj->getLength(); i++) {
      if (myObj->geomids[i] == underIdx[idx])
      {
        under = &(myObj->surfaces[i]);
        break;
      }
    }

    // wall slice
    double real2uv;
    std::vector<std::vector<std::pair<double, double>>> curvesUv = calcUvMap(
      &wallCurves[idx], &wallFaces[idx], &wallVolumes[idx], &surfaces[idx], pointsDistance, &real2uv);
    std::vector<std::vector<std::pair<double, double>>> loopsUv = sortUvMap(
      curvesUv, real2uv, pointsDistance);
    offsetUv = calcOffsetUv(loopsUv, numWall + 1, real2uv, width * wallPercent, false);
    int loopLastIdx;
    for (int i = 0; i < offsetUv.size() - 1; i++)
    {
      std::vector<PointData> wallData = createWallPointData(
        offsetUv[i],
        width, defaultThickness, volumeCnt, idx,
        surfaces[idx], *under, &loopLastIdx, preNorm);
      //wallDatas[idx] = wallData;
      for (int j = 0; j < wallData.size(); j++)
      {
        bottomDatas.push_back(wallData[j]);
      }
      if (wallData.size() > 0)
      {
        preNorm = wallData[wallData.size() - 1].norm;
      }
    }

    // draw wall curve by points
    for (int i = 0; i < offsetUv.size() - 1; i++)
    {
      std::vector<std::vector<std::pair<double, double>>> drawWallUv = offsetUv[i];
      if (drawWallUv.size() > 0)
      {
        for (int j = 0; j < drawWallUv.size(); j++)
        {
          if (drawWallUv[j].size() >= 2)
          {
            double u = drawWallUv[j][0].first;
            double v = drawWallUv[j][0].second;
            gp_Pnt prePnt = surfaces[idx].get()->Value(u, v);
            for (int i = 1; i < drawWallUv[j].size(); i++)
            {
              u = drawWallUv[j][i].first;
              v = drawWallUv[j][i].second;
              gp_Pnt nowPnt = surfaces[idx].get()->Value(u, v);
              if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
              {
                Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                Standard_Real u1 = 0.0;
                Standard_Real u2 = prePnt.Distance(nowPnt);
                Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
#pragma omp critical
                {
                  Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                  myAISContext()->Display(ais, Standard_False);
                  myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[idx]);
                }
              }
              prePnt = nowPnt;
            }
          }
        }
      }
    }
    std::cout << "sort curve end:" << idx << "," << layerCnt << std::endl;

    // create bottom
    std::cout << "create bottom start:" << idx << "," << layerCnt << std::endl;
    std::vector<std::vector<std::vector<std::pair<double, double>>>> bottomUv = calcBottom(
      offsetUv[offsetUv.size() - 1], real2uv, pointsDistance, width, 0.3, surfaces[idx], direction);
    std::vector<PointData> bottomData = createBottomPointData(bottomUv,
      width, defaultThickness, volumeCnt, idx, 
      surfaces[idx], *under, &loopLastIdx, preNorm);
    for (int i = 0; i < bottomData.size(); i++)
    {
      bottomDatas.push_back(bottomData[i]);
    }

    // last
    lastPointX = bottomData[bottomData.size() - 1].pnt.X();
    lastPointY = bottomData[bottomData.size() - 1].pnt.Y();
    lastPointZ = bottomData[bottomData.size() - 1].pnt.Z();
    lastNormI = bottomData[bottomData.size() - 1].norm.X();
    lastNormJ = bottomData[bottomData.size() - 1].norm.Y();
    lastNormK = bottomData[bottomData.size() - 1].norm.Z();

    // draw 
    if (bottomUv.size() > 0)
    {
      for (int k = 0; k < bottomUv.size(); k++)
      {
        if (bottomUv[k].size() > 0)
        {
          for (int j = 0; j < bottomUv[k].size(); j++)
          {
            if (bottomUv[k][j].size() >= 2)
            {
              // draw bottom curve by points
              double u = bottomUv[k][j][0].first;
              double v = bottomUv[k][j][0].second;
              gp_Pnt prePnt = surfaces[idx].get()->Value(u, v);
              for (int i = 1; i < bottomUv[k][j].size(); i++)
              {
                u = bottomUv[k][j][i].first;
                v = bottomUv[k][j][i].second;
                gp_Pnt nowPnt = surfaces[idx].get()->Value(u, v);
                if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
                {
                  Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                  Standard_Real u1 = 0.0;
                  Standard_Real u2 = prePnt.Distance(nowPnt);
                  Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                  TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
                  #pragma omp critical
                  {
                    Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                    myAISContext()->Display(ais, Standard_False);
                    myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[idx]);
                  }
                }
                prePnt = nowPnt;
              }
            }
          }
        }
      }
    }
    std::cout << "create bottom start:" << idx << "," << layerCnt << std::endl;
  }
#pragma omp barrier


  std::cout << "write csv start:" << 0 << "," << layerCnt << std::endl;
  writeCsv(path, bottomDatas);
  std::cout << "write csv end:" << 0 << "," << layerCnt << std::endl;

  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory
  delete[] surfaces;
  delete[] dispEdgeCnt;
  delete[] wallCurves;
  delete[] wallFaces;
  delete[] wallVolumes;

  return 0;
}

int OCCTProxy::calcBottomLayer(
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx,
  int geomidUpper,int geomIdBase, int nx, int ny,
  int surfaceR, int surfaceG, int surfaceB)
{
  int layerCnt = 1;
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "pre slice start:\n";

  // openmp
  omp_set_num_threads(omp_get_max_threads());
  omp_set_dynamic(0);

  // alignment upper surface
  Handle(Geom_BSplineSurface) upperSurface;
  alignmentLayer(myObj, geomIdBase, geomidUpper, nx, ny, &upperSurface);
  Handle(Geom_BSplineSurface)* bufUpperSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomidUpper)
    {
      myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
      myObj->geomids[i] = 0;
      break;
    }
  }
  BRepBuilderAPI_MakeFace makerUpper(upperSurface, Precision::Confusion());
  TopoDS_Face faceUpper = makerUpper.Face();
  Handle(AIS_InteractiveObject) targetUpper = new AIS_Shape(faceUpper);
  myAISContext()->Display(targetUpper, Standard_False);
  myObj->setObject_Face(targetUpper, faceUpper, upperSurface, geomidUpper);


  // create surface list
  Handle(Geom_BSplineSurface)* surface;
  surface = new Handle(Geom_BSplineSurface)[layerCnt];
  TopoDS_Face* face;
  face = new TopoDS_Face[layerCnt];
  Quantity_Color aCol = Quantity_Color(surfaceR / 255.0, surfaceG / 255.0, surfaceB / 255.0, Quantity_TOC_RGB);
  Handle(AIS_InteractiveObject)* dispSurface = new Handle(AIS_InteractiveObject)[layerCnt];

  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "create surface start:" << idx << "," << layerCnt << std::endl;
    surface[idx] = upperSurface;
    BRepBuilderAPI_MakeFace makerFace(surface[idx], Precision::Confusion());
    face[idx] = makerFace.Face();
    dispSurface[idx] = new AIS_Shape(face[idx]);
    int bufIdx = -(idx + 1 + preSliceStartLayer) * IDX_PATERN;
    myObj->setObject_Face(dispSurface[idx], face[idx], surface[idx], bufIdx);
    surfaceIdx[idx] = bufIdx;
    wallIdx[idx] = bufIdx - 1;
    infillIdx[idx] = bufIdx - 2;
    supportIdx[idx] = bufIdx - 3;
    void* surface_ptr = surface[idx].get();
    std::cout << "create surface end:" << idx << "," << layerCnt << std::endl;
  }

  // redraw
  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "draw surface start:" << idx << "," << layerCnt << std::endl;
    myAISContext()->Display(dispSurface[idx], Standard_False);
    myAISContext()->SetColor(dispSurface[idx], aCol, Standard_False);
    if (idx == 0)
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.75, Standard_False);
    }
    else
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.0, Standard_False);
    }
    myAISContext()->SetViewAffinity(dispSurface[idx], myView(), Standard_True);
    std::cout << "draw surface end:" << idx << "," << layerCnt << std::endl;
  }
  myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
  myAISContext()->UpdateCurrentViewer();

  // next
  preSliceStartLayer += layerCnt;
  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory
  delete[] surface;
  delete[] face;
  delete[] dispSurface;

  return layerCnt;
}

int  OCCTProxy::getLayerCenter(int geomIdSurface, double& x, double& y, double& z)
{
  Handle(Geom_BSplineSurface)* surface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomIdSurface)
    {
      surface = &(myObj->surfaces[i]);
      break;
    }
  }
  double upperUmin, upperUmax, upperVmin, upperVmax;
  surface->get()->Bounds(upperUmin, upperUmax, upperVmin, upperVmax);

  // center
  double u = (upperUmax + upperUmin) / 2.0;
  double v = (upperVmax + upperVmin) / 2.0;
  gp_Pnt pnt = surface->get()->Value(u, v);
  x = pnt.X();
  y = pnt.Y();
  z = pnt.Z();

  return 0;
}

int OCCTProxy::calcAutoSliceLayer(
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx,
  int volumeGeomid, int geomIdBase, double checkDist,
  int surfaceR, int surfaceG, int surfaceB)
{
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  std::cout << "pre slice start\n";
  int layerCnt = 20;

  // slice
  Handle(Geom_BSplineSurface) bufBaseSurface;
  for (int i = 0; i < myObj->getLength(); i++) {
    if (myObj->geomids[i] == geomIdBase)
    {
      bufBaseSurface = myObj->surfaces[i];
      break;
    }
  }

  // value setting
  Handle(Geom_BSplineSurface)* surface;
  surface = new Handle(Geom_BSplineSurface)[layerCnt];
  TopoDS_Face* face;
  face = new TopoDS_Face[layerCnt];
  Quantity_Color aCol = Quantity_Color(surfaceR / 255.0, surfaceG / 255.0, surfaceB / 255.0, Quantity_TOC_RGB);
  Handle(AIS_InteractiveObject)* dispSurface = new Handle(AIS_InteractiveObject)[layerCnt];

  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "layer:" << idx << std::endl;
    std::vector<gp_Pnt> pnts;
    double firstXmin = DBL_MAX;
    double firstXmax = DBL_MIN;
    double firstYmin = DBL_MAX;
    double firstYmax = DBL_MIN;
    double firstZmin = DBL_MAX;
    double firstZmax = DBL_MIN;
    for (int i = 0; i < myObj->getLength(); i++)
    {
      if (myObj->geomids[i] == volumeGeomid)
      {
        Handle(Geom_BSplineSurface) volume;
        {
          volume = myObj->surfaces[i];
        }
        GeomInt_IntSS ss = GeomInt_IntSS(bufBaseSurface, volume, 0.001, Standard_False);
        for (int j = 0; j < ss.NbLines(); j++)
        {
          // pnt
          Handle(Geom_Curve) bufCurve = ss.Line(j + 1);
          double prmFirst = bufCurve.get()->FirstParameter();
          gp_Pnt pntFirst = bufCurve->Value(prmFirst);
          // volume norm
          GeomAPI_ProjectPointOnSurf apiFaceFirst(pntFirst, volume);
          Standard_Real minUfaceFirst, minVfaceFirst; 
          apiFaceFirst.LowerDistanceParameters(minUfaceFirst, minVfaceFirst);
          gp_Vec dUfaceFirst, dVfaceFirst;
          volume->D1(minUfaceFirst, minVfaceFirst, pntFirst, dUfaceFirst, dVfaceFirst);
          gp_Vec normFace = dUfaceFirst.Crossed(dVfaceFirst);
          normFace.Normalize();
          // surface norm
          GeomAPI_ProjectPointOnSurf apiSrfFirst(pntFirst, bufBaseSurface);
          Standard_Real minUsrfFirst, minVsrfFirst;
          apiSrfFirst.LowerDistanceParameters(minUsrfFirst, minVsrfFirst);
          gp_Vec dUsrfFirst, dVsrfFirst;
          bufBaseSurface->D1(minUsrfFirst, minVsrfFirst, pntFirst, dUsrfFirst, dVsrfFirst);
          gp_Vec normSrf = dUsrfFirst.Crossed(dVsrfFirst);
          normSrf.Normalize();
          // cross
          gp_Vec axis = normFace.Crossed(normSrf);
          axis.Normalize();
          double angle = acos(normFace.Dot(normSrf));
          double mat1[3 * 3], mat2[3 * 3];
          calcQuat2Mat(axis.X(), axis.Y(), axis.Z(), PI / 2.0, mat1);
          gp_Vec vec1 = calcMatVec(mat1, normFace);
          double dot1 = vec1.Dot(normSrf);
          calcQuat2Mat(axis.X(), axis.Y(), axis.Z(), -PI / 2.0, mat2);
          gp_Vec vec2 = calcMatVec(mat2, normFace);
          double dot2 = vec2.Dot(normSrf);
          gp_Pnt bufPnt;
          if (dot1 > dot2)
          {
            bufPnt = gp_Pnt(pntFirst.X() + vec1.X() * checkDist,
              pntFirst.Y() + vec1.Y() * checkDist, pntFirst.Z() + vec1.Z() * checkDist);
            pnts.push_back(bufPnt);
          }
          else
          {
            bufPnt = gp_Pnt(pntFirst.X() + vec2.X() * checkDist,
              pntFirst.Y() + vec2.Y() * checkDist, pntFirst.Z() + vec2.Z() * checkDist);
            pnts.push_back(bufPnt);
          }
          firstXmin = std::min(firstXmin, bufPnt.X());
          firstXmax = std::max(firstXmax, bufPnt.X());
          firstYmin = std::min(firstYmin, bufPnt.Y());
          firstYmax = std::max(firstYmax, bufPnt.Y());
          firstZmin = std::min(firstZmin, bufPnt.Z());
          firstZmax = std::max(firstZmax, bufPnt.Z());
        }
      }
    }
    // fit base plane
    std::vector<PointCgal> planePnts = std::vector<PointCgal>();
    for (int i = 0; i < pnts.size(); i++)
    {
      planePnts.push_back(PointCgal(pnts[i].X() - (firstXmin + firstXmax) / 2.0, 
        pnts[i].Y() - (firstYmin + firstYmax) / 2.0, pnts[i].Z() - (firstZmin + firstZmax) / 2.0));
    }
    PlaneCgal plane;
    CGAL::linear_least_squares_fitting_3(planePnts.begin(), planePnts.end(), plane, CGAL::Dimension_tag<0>());
    double normI = plane.a();
    double normJ = plane.b();
    double normK = plane.c();
    double rotX = asin(-normJ);
    double rotY = 0.0;
    if (cos(rotX) != 0.0) rotY = asin(normI / cos(rotX));
    // rotate
    double mat[9];
    mat[0] = cos(-rotY);
    mat[1] = sin(-rotY) * sin(-rotX);
    mat[2] = sin(-rotY) * cos(-rotX);
    mat[3] = 0.0;
    mat[4] = cos(-rotX);
    mat[5] = -sin(-rotX);
    mat[6] = -sin(-rotY);
    mat[7] = cos(-rotY) * sin(-rotX);
    mat[8] = cos(-rotY) * cos(-rotX);
    std::vector<PointCgal> rotPnts = std::vector<PointCgal>();
    double xmin = DBL_MAX;
    double xmax = DBL_MIN;
    double ymin = DBL_MAX;
    double ymax = DBL_MIN;
    double zsum = 0.0;
    for (int i = 0; i < planePnts.size(); i++)
    {
      PointCgal bufPnt = calcMatVec(mat, planePnts[i]);
      rotPnts.push_back(bufPnt);
      xmin = std::min(xmin, bufPnt.x());
      xmax = std::max(xmax, bufPnt.x());
      ymin = std::min(ymin, bufPnt.y());
      ymax = std::max(ymax, bufPnt.y());
      zsum += bufPnt.z();
    }
    double pitch = 1.0;
    double deltaMin = std::min(xmax - xmin, ymax - ymin);
    double rangeK = 2.0;
    double rangeXmin = (xmin + xmax) / 2.0 - (xmax - xmin) * rangeK / 2.0;
    double rangeXmax = (xmin + xmax) / 2.0 + (xmax - xmin) * rangeK / 2.0;
    double rangeYmin = (ymin + ymax) / 2.0 - (ymax - ymin) * rangeK / 2.0;
    double rangeYmax = (ymin + ymax) / 2.0 + (ymax - ymin) * rangeK / 2.0;
    rotPnts.push_back(PointCgal(rangeXmin, rangeYmin, zsum / (double)planePnts.size()));
    rotPnts.push_back(PointCgal(rangeXmax, rangeYmin, zsum / (double)planePnts.size()));
    rotPnts.push_back(PointCgal(rangeXmax, rangeYmax, zsum / (double)planePnts.size()));
    rotPnts.push_back(PointCgal(rangeXmin, rangeYmax, zsum / (double)planePnts.size()));
    // delaunay triangle
    DelaunayTriangulation dt(rotPnts);
    dt.execute();
    std::vector<size_t*> tris = dt.get_triangles();
    int nu = std::ceil((rangeXmax - rangeXmin) / pitch);
    int nv = std::ceil((rangeYmax - rangeYmin) / pitch);
    TColgp_Array2OfPnt srfPnts = TColgp_Array2OfPnt(1, nu - 2, 1, nv - 2);
    double invmat[9];
    //invmat[0] = cos(rotY);
    //invmat[1] = 0.0;
    //invmat[2] = sin(rotY);
    //invmat[3] = sin(rotX) * sin(rotY);
    //invmat[4] = cos(rotX);
    //invmat[5] = -sin(rotX) * cos(rotY);
    //invmat[6] = -cos(rotX) * sin(rotY);
    //invmat[7] = sin(rotX);
    //invmat[8] = cos(rotX) * cos(rotY);
    invmat[0] = cos(rotY);
    invmat[1] = sin(rotY) * sin(rotX);
    invmat[2] = sin(rotY) * cos(rotX);
    invmat[3] = 0.0;
    invmat[4] = cos(rotX);
    invmat[5] = -sin(rotX);
    invmat[6] = -sin(rotY);
    invmat[7] = cos(rotY) * sin(rotX);
    invmat[8] = cos(rotY) * cos(rotX);
    for (int i = 0; i < nu - 2; i++)
    {
      for (int j = 0; j < nv - 2; j++)
      {
        double x = (rangeXmax - rangeXmin) * (i + 1) / (double)(nu - 2) + rangeXmin;
        double y = (rangeYmax - rangeYmin) * (j + 1) / (double)(nv - 2) + rangeYmin;
        x = std::min(x, rangeXmax);
        x = std::max(x, rangeXmin);
        y = std::min(y, rangeYmax);
        y = std::max(y, rangeYmin);
        bool flag = true;
        for (int k = 0; k < tris.size(); k++)
        {
          double x0 = rotPnts[tris[k][0]].x();
          double y0 = rotPnts[tris[k][0]].y();
          double z0 = rotPnts[tris[k][0]].z();
          double x1 = rotPnts[tris[k][1]].x();
          double y1 = rotPnts[tris[k][1]].y();
          double z1 = rotPnts[tris[k][1]].z();
          double x2 = rotPnts[tris[k][2]].x();
          double y2 = rotPnts[tris[k][2]].y();
          double z2 = rotPnts[tris[k][2]].z();
          double s, t;
          if (inTriangle(x0, y0, x1, y1, x2, y2, x, y, &s, &t))
          {
            double z = (z1 - z0) * s + (z2 - z0) * t + z0;
            gp_Pnt buf = calcMatVec(invmat, x, y, z);
            gp_Pnt bufPnt = gp_Pnt(buf.X() + (firstXmin + firstXmax) / 2.0,
              buf.Y() + (firstYmin + firstYmax) / 2.0, buf.Z() + (firstZmin + firstZmax) / 2.0);
            srfPnts.SetValue(i + 1, j + 1, bufPnt);
            flag = false;
            break;
          }
        }
        if (flag)
        {
          double z = zsum / (double)planePnts.size();
          gp_Pnt buf = calcMatVec(invmat, x, y, z);
          gp_Pnt bufPnt = gp_Pnt(buf.X() + (firstXmin + firstXmax) / 2.0,
            buf.Y() + (firstYmin + firstYmax) / 2.0, buf.Z() + (firstZmin + firstZmax) / 2.0);
          srfPnts.SetValue(i + 1, j + 1, bufPnt);
        }
      }
    }
    GeomAPI_PointsToBSplineSurface api = GeomAPI_PointsToBSplineSurface(srfPnts, 3, 8, GeomAbs_G2, 0.001);
    bufBaseSurface = api.Surface();
    surface[idx] = bufBaseSurface;
    BRepBuilderAPI_MakeFace makerFace(surface[idx], Precision::Confusion());
    face[idx] = makerFace.Face();
    dispSurface[idx] = new AIS_Shape(face[idx]);
    int bufIdx = -(idx + 1 + preSliceStartLayer) * IDX_PATERN;
    myObj->setObject_Face(dispSurface[idx], face[idx], surface[idx], bufIdx);
    surfaceIdx[idx] = bufIdx;
    wallIdx[idx] = bufIdx - 1;
    infillIdx[idx] = bufIdx - 2;
    supportIdx[idx] = bufIdx - 3;
  }

  for (int idx = 0; idx < layerCnt; idx++)
  {
    std::cout << "draw surface start:" << idx << "," << layerCnt << std::endl;
    myAISContext()->Display(dispSurface[idx], Standard_False);
    myAISContext()->SetColor(dispSurface[idx], aCol, Standard_False);
    if (idx == 0)
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.75, Standard_False);
    }
    else
    {
      myAISContext()->SetTransparency(dispSurface[idx], 0.0, Standard_False);
    }
    myAISContext()->SetViewAffinity(dispSurface[idx], myView(), Standard_True);
    std::cout << "draw surface end:" << idx << "," << layerCnt << std::endl;
  }
  myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
  myAISContext()->UpdateCurrentViewer();


  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory
  delete[] surface;
  delete[] face;
  delete[] dispSurface;

  return layerCnt;
}

int OCCTProxy::calcPeelerLayer(
  array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx)
{
  int bufIdx = -(0 + 1 + preSliceStartLayer) * IDX_PATERN;
  surfaceIdx[0] = bufIdx;
  wallIdx[0] = bufIdx - 1;
  infillIdx[0] = bufIdx - 2;
  supportIdx[0] = bufIdx - 3;
  return 1;
}

int OCCTProxy::calcPeelerPoints(char* path, double pointsDistance, double width, double thick, double wallPercent,
  array<int>^ wallIdx, int peelerGeomid, int volumeCnt, bool flag,
  double prePointX, double prePointY, double prePointZ,
  double& lastPointX, double& lastPointY, double& lastPointZ,
  double preNormI, double preNormJ, double preNormK,
  double& lastNormI, double& lastNormJ, double& lastNormK)
{
  // create console
  AllocConsole();
  FILE* fpc;
  freopen_s(&fpc, "CONOUT$", "w", stdout);
  std::cout.clear();
  freopen_s(&fpc, "CONIN$", "r", stdin);
  std::cin.clear();
  freopen_s(&fpc, "CONOUT$", "w", stderr);
  std::cerr.clear();
  // peeler
  Handle(Geom_BSplineSurface) peeler;
  TopoDS_Face face;
  std::vector<PointData> list;
  int layerCnt = -1;
  gp_Pnt prePnt, nowPnt;
  prePnt = gp_Pnt(prePointX, prePointY, prePointZ);
  gp_Dir preNorm = gp_Dir(preNormI, preNormJ, preNormK);
  for (int i = 0; i < myObj->getLength(); i++) 
  {
    if (myObj->geomids[i] == peelerGeomid)
    {
      layerCnt++;
      std::cout << "peeler layer:" << layerCnt << std::endl;
      peeler = myObj->surfaces[i];
      face = myObj->faces[i];
      BRepTopAdaptor_FClass2d checkFace(face, Precision::Confusion());
      double bufPeelerUmin, bufPeelerUmax, bufPeelerVmin, bufPeelerVmax;
      peeler.get()->Bounds(bufPeelerUmin, bufPeelerUmax, bufPeelerVmin, bufPeelerVmax);
      gp_Pnt pnt0 = peeler.get()->Value(bufPeelerUmin, bufPeelerVmin);
      gp_Pnt pnt1 = peeler.get()->Value(bufPeelerUmax, bufPeelerVmin);
      gp_Pnt pnt2 = peeler.get()->Value(bufPeelerUmax, bufPeelerVmax);
      gp_Pnt pnt3 = peeler.get()->Value(bufPeelerUmin, bufPeelerVmax);
      double dist01 = pnt0.Distance(pnt1);
      double dist12 = pnt1.Distance(pnt2);
      double dist23 = pnt2.Distance(pnt3);
      double dist30 = pnt3.Distance(pnt0);
      double uv01 = bufPeelerUmax - bufPeelerUmin;
      double uv12 = bufPeelerVmax - bufPeelerVmin;
      double uv23 = uv01;
      double uv30 = uv12;
      double scale = (dist01 / uv01 + dist12 / uv12 + dist23 / uv23 + dist30 / uv30) / 4.0;

      double large = uv01 < uv12 ? uv12 : uv01;
      double small = uv01 > uv12 ? uv12 : uv01;
      double pitch = width / scale * sqrt(2.0) * wallPercent;
      int loopCnt = 0;
      double stu, stv, edu, edv;
      double bufstu, bufstv, bufedu, bufedv;

      stu = bufPeelerUmin;
      stv = bufPeelerVmin;
      edu = bufPeelerUmin;
      edv = bufPeelerVmin;
      while (stu < bufPeelerUmax || stv < bufPeelerVmax || edu < bufPeelerUmax || edv < bufPeelerVmax)
      {
        if (loopCnt % 2 == 0)
        {
          stu = bufPeelerUmin;
          stv = bufPeelerVmin + loopCnt * pitch;
          edu = bufPeelerUmin + loopCnt * pitch;
          edv = bufPeelerVmin;
          if (stv > bufPeelerVmax)
          {
            stu = stv - bufPeelerVmax + bufPeelerUmin;
            stv = bufPeelerVmax;
            if (stu > bufPeelerUmax)
            {
              stu = bufPeelerUmax;
            }
          }
          if (edu > bufPeelerUmax)
          {
            edv = edu - bufPeelerUmax + bufPeelerVmin;
            edu = bufPeelerUmax;
            if (edv > bufPeelerVmax)
            {
              edv = bufPeelerVmax;
            }
          }
        }
        else
        {
          stu = bufPeelerUmin + loopCnt * pitch;
          stv = bufPeelerVmin;
          edu = bufPeelerUmin;
          edv = bufPeelerVmin + loopCnt * pitch;
          if (stu > bufPeelerUmax)
          {
            stv = stu - bufPeelerUmax + bufPeelerVmin;
            stu = bufPeelerUmax;
            if (stv > bufPeelerVmax)
            {
              stv = bufPeelerVmax;
            }
          }
          if (edv > bufPeelerVmax)
          {
            edu = edv - bufPeelerVmax + bufPeelerUmin;
            edv = bufPeelerVmax;
            if (edu > bufPeelerUmax)
            {
              edu = bufPeelerUmax;
            }
          }
        }
        if (flag)
        {
          bufstu = stu;
          bufstv = stv;
          bufedu = edu;
          bufedv = edv;
        }
        else
        {
          bufstu = bufPeelerUmax + bufPeelerUmin - stu;
          bufstv = stv;
          bufedu = bufPeelerUmax + bufPeelerUmin - edu;
          bufedv = edv;
        }
        double dist = sqrt((bufedu - bufstu) * (bufedu - bufstu) + (bufedv - bufstv) * (bufedv - bufstv));
        int div = ceil(dist / (pointsDistance / scale));
        for (int j = 0; j < div; j++)
        {
          double u = (bufedu - bufstu) * j / (double)div + bufstu;
          double v = (bufedv - bufstv) * j / (double)div + bufstv;
          gp_Pnt2d chk = gp_Pnt2d(u, v);
          TopAbs_State state = checkFace.Perform(chk);
          if (state == TopAbs_IN || state == TopAbs_ON)
          {
            std::pair<double, double> uv = std::pair<double, double>(u, v);
            if (j == 0)
            {
              PointData item = calcPointData_uv(
                uv, peeler, thick, PrintType::OuterWallStart, width, volumeCnt, layerCnt, loopCnt, 0, preNorm);
              preNorm = item.norm;
              list.push_back(item);
              prePnt = item.pnt;
            }
            else
            {
              PointData item = calcPointData_uv(
                uv, peeler, thick, PrintType::OuterWallMiddle, width, volumeCnt, layerCnt, loopCnt, 0, preNorm);
              preNorm = item.norm;
              list.push_back(item);
              nowPnt = item.pnt;
              if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
              {
                Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
                Standard_Real u1 = 0.0;
                Standard_Real u2 = prePnt.Distance(nowPnt);
                Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
                {
                  Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
                  myAISContext()->Display(ais, Standard_False);
                  myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[0]);
                }
              }
              prePnt = item.pnt;
            }
          }
        }
        gp_Pnt2d chk = gp_Pnt2d(edu, edv);
        TopAbs_State state = checkFace.Perform(chk);
        if (state == TopAbs_IN || state == TopAbs_ON)
        {
          std::pair<double, double> uv = std::pair<double, double>(edu, edv);
          PointData item = calcPointData_uv(
            uv, peeler, thick, PrintType::OuterWallEnd, width, volumeCnt, layerCnt, loopCnt, 0, preNorm);
          preNorm = item.norm;
          list.push_back(item);
          nowPnt = item.pnt;
          if (!nowPnt.IsEqual(prePnt, EQUAL_POINT_REAL_TH))
          {
            Handle(Geom_Line) line = new Geom_Line(prePnt, gp_Dir(nowPnt.XYZ() - prePnt.XYZ()));
            Standard_Real u1 = 0.0;
            Standard_Real u2 = prePnt.Distance(nowPnt);
            Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(line, u1, u2);
            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
            {
              Handle(AIS_InteractiveObject) ais = new AIS_Shape(edge);
              myAISContext()->Display(ais, Standard_False);
              myObj->setObject_Edge(ais, edge, trimmedCurve, wallIdx[0]);
            }
          }
          prePnt = item.pnt;
        }
        loopCnt++;
      }
    }
  }

  lastPointX = list[list.size() - 1].pnt.X();
  lastPointY = list[list.size() - 1].pnt.Y();
  lastPointZ = list[list.size() - 1].pnt.Z();
  lastNormI = list[list.size() - 1].norm.X();
  lastNormJ = list[list.size() - 1].norm.Y();
  lastNormK = list[list.size() - 1].norm.Z();

  writeCsv(path, list);
  // console
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);
  FreeConsole();

  // memory

  return 0;
}