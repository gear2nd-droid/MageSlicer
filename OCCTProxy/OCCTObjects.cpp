#include "OCCTProxy.hpp"
#include <stdio.h>

using namespace System::Runtime::InteropServices;


OCCTObjects::OCCTObjects(int len)
{
  length = -1;
  objects = new Handle(AIS_InteractiveObject)[len];
  shapes = new TopoDS_Shape[len];
  faces = new TopoDS_Face[len];
  edges = new TopoDS_Edge[len];
  surfaces = new Handle(Geom_BSplineSurface)[len];
  curves = new Handle(Geom_Curve)[len];
  geomids = new int[len];
  types = new int[len];
  // infill
  infillStPnt = new gp_Pnt*[CALC_LAYER_MAX];
  infillEdPnt = new gp_Pnt*[CALC_LAYER_MAX];
  infillLines = 0;
  for (int i = 0; i < CALC_LAYER_MAX; i++)
  {
    infillStPnt[i] = new gp_Pnt[CALC_POINT_PER_LAYER_MAX];
    infillEdPnt[i] = new gp_Pnt[CALC_POINT_PER_LAYER_MAX];
  }
}
OCCTObjects::~OCCTObjects()
{
  delete[] objects;
  delete[] shapes;
  delete[] faces;
  delete[] edges;
  delete[] surfaces;
  delete[] curves;
  delete[] geomids;
  delete[] types;
}
int OCCTObjects::getLength()
{
  return this->length + 1;
}
int OCCTObjects::setObject_ShapeWithSurface(Handle(AIS_InteractiveObject) object, TopoDS_Shape shape, 
  Handle(Geom_BSplineSurface) surface, int geomid)
{
  this->length++;
  this->objects[length] = object;
  this->shapes[length] = shape;
  this->surfaces[length] = surface;
  this->geomids[length] = geomid;
  this->types[length] = E_TOPODS_SHAPE;
  return length;
}
int OCCTObjects::setObject_Face(Handle(AIS_InteractiveObject) object, TopoDS_Face face, 
  Handle(Geom_BSplineSurface) surface, int geomid)
{
  this->length++;
  this->objects[length] = object;
  this->faces[length] = face;
  this->surfaces[length] = surface;
  this->geomids[length] = geomid;
  this->types[length] = E_TOPODS_FACE;
  return length;
}
int OCCTObjects::setObject_Edge(Handle(AIS_InteractiveObject) object, TopoDS_Edge edge, 
  Handle(Geom_Curve) curve, int geomid)
{
  this->length++;
  this->objects[length] = object;
  this->edges[length] = edge;
  this->curves[length] = curve;
  this->geomids[length] = geomid;
  this->types[length] = E_TOPODS_EDGE;
  return length;
}
void OCCTObjects::setObject(int idx, Handle(AIS_InteractiveObject) object)
//void setObject(int idx, void* object)
{
  this->objects[idx] = object;
}
Handle(AIS_InteractiveObject) OCCTObjects::getObject(int idx)
{
  return this->objects[idx];
}
void OCCTObjects::setGeomId(int idx, int geomid)
{
  this->geomids[idx] = geomid;
}
int OCCTObjects::getGeomId(int idx)
{
  return this->geomids[idx];
}
void OCCTObjects::setType(int idx, int type)
{
  this->types[idx] = type;
}
int OCCTObjects::getType(int idx)
{
  return this->types[idx];
}

std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (getline(ss, item, delim)) {
    if (!item.empty()) {
      elems.push_back(item);
    }
  }
  return elems;
}

void OCCTProxy::SetColor(int geomid, int r, int g, int b)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == geomid)
    {
      Quantity_Color aCol = Quantity_Color(r / 255.0, g / 255.0, b / 255.0, Quantity_TOC_RGB);
      myAISContext()->SetColor(myObj->objects[i], aCol, Standard_False);
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::SetTransparent(int geomid, bool trans)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == geomid)
    {
      if (trans) {
        myAISContext()->SetTransparency(myObj->objects[i], 0.0, false);
      }
      else {
        myAISContext()->SetTransparency(myObj->objects[i], 1.0, false);
      }
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::SetWireframe(int geomid)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == geomid)
    {
      myAISContext()->SetDisplayMode(AIS_WireFrame, Standard_False);
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::SetShading(int geomid)
{
  for (int i = 0; i < myObj->getLength(); i++)
  {
    if (myObj->geomids[i] == geomid)
    {
      myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

void OCCTProxy::ImportStep_NonTransform(char* path, int geomid)
{
  STEPControl_Reader reader = STEPControl_Reader();
  IFSelect_ReturnStatus status = reader.ReadFile(path);
  if (status == IFSelect_ReturnStatus::IFSelect_RetDone)
  {
    bool failsonly = false;
    reader.PrintCheckLoad(failsonly, IFSelect_PrintCount::IFSelect_ItemsByEntity);
    reader.PrintCheckTransfer(failsonly, IFSelect_PrintCount::IFSelect_ItemsByEntity);
    reader.TransferRoot(1);
    TopoDS_Shape shape = reader.Shape(1);
    TopExp_Explorer ex = TopExp_Explorer(shape, TopAbs_ShapeEnum::TopAbs_FACE);
    while (ex.More())
    {
      TopoDS_Face face = TopoDS::Face(ex.Current());
      Handle(Geom_Surface) bufSurface = BRep_Tool::Surface(face);

      ex.Next();
      /*
      // The trimmed area is restored because of the approximation.
      GeomConvert_ApproxSurface approx(bufSurface, 0.1, GeomAbs_C2, GeomAbs_C2, 3, 3, 1000, 0);
      if (approx.IsDone())
      {
        Handle(Geom_BSplineSurface) bufSurface = approx.Surface();
        // add object
        BRepBuilderAPI_MakeFace makerFace(bufSurface, Precision::Confusion());
        TopoDS_Face face = makerFace.Face();
        Handle(AIS_InteractiveObject) target = new AIS_Shape(face);
        myAISContext()->Display(target, Standard_False);
        myObj->setObject_Face(target, face, bufSurface, geomid);
        // bounding box
        Bnd_Box bbox;
        BRepBndLib::Add(face, bbox);
        double minX, maxX, minY, maxY, minZ, maxZ;
        bbox.Get(minX, minY, minZ, maxX, maxY, maxZ);
        if (myObj->bboxXmin > minX) myObj->bboxXmin = minX;
        if (myObj->bboxXmax < maxX) myObj->bboxXmax = maxX;
        if (myObj->bboxYmin > minY) myObj->bboxYmin = minY;
        if (myObj->bboxYmax < maxY) myObj->bboxYmax = maxY;
        if (myObj->bboxZmin > minZ) myObj->bboxZmin = minZ;
        if (myObj->bboxZmax < maxZ) myObj->bboxZmax = maxZ;
      }*/

      // Because of the conversion, trims, etc., are reflected.
      Handle(Geom_BSplineSurface) surface = GeomConvert::SurfaceToBSplineSurface(bufSurface);
      Handle(AIS_InteractiveObject) target = new AIS_Shape(face);
      myAISContext()->Display(target, Standard_False);
      myObj->setObject_Face(target, face, surface.get(), geomid);
      // bounding box
      Bnd_Box bbox;
      BRepBndLib::Add(face, bbox);
      double minX, maxX, minY, maxY, minZ, maxZ;
      bbox.Get(minX, minY, minZ, maxX, maxY, maxZ);
      if (myObj->bboxXmin > minX) myObj->bboxXmin = minX;
      if (myObj->bboxXmax < maxX) myObj->bboxXmax = maxX;
      if (myObj->bboxYmin > minY) myObj->bboxYmin = minY;
      if (myObj->bboxYmax < maxY) myObj->bboxYmax = maxY;
      if (myObj->bboxZmin > minZ) myObj->bboxZmin = minZ;
      if (myObj->bboxZmax < maxZ) myObj->bboxZmax = maxZ;
      
    }
    for (int i = 0; i < myObj->getLength(); i++)
    {
      if (myObj->geomids[i] == geomid)
      {
        myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
      }
    }
    myAISContext()->UpdateCurrentViewer();
  }
}

void OCCTProxy::ImportStep_Transform(char* path, int geomid, 
  double mvX, double mvY, double mvZ, 
  double pntX, double pntY, double pntZ, double dirX, double dirY, double dirZ, double angle)
{
  STEPControl_Reader reader = STEPControl_Reader();
  IFSelect_ReturnStatus status = reader.ReadFile(path);
  if (status == IFSelect_ReturnStatus::IFSelect_RetDone)
  {
    bool failsonly = false;
    reader.PrintCheckLoad(failsonly, IFSelect_PrintCount::IFSelect_ItemsByEntity);
    reader.PrintCheckTransfer(failsonly, IFSelect_PrintCount::IFSelect_ItemsByEntity);
    reader.TransferRoot(1);
    TopoDS_Shape shape = reader.Shape(1);
    TopExp_Explorer ex = TopExp_Explorer(shape, TopAbs_ShapeEnum::TopAbs_FACE);
    while (ex.More())
    {
      TopoDS_Face face = TopoDS::Face(ex.Current());
      Handle(Geom_Surface) bufSurface = BRep_Tool::Surface(face);
      Handle(Geom_BSplineSurface) surface = GeomConvert::SurfaceToBSplineSurface(bufSurface);
      // rotate
      TopoDS_Shape shape2;
      if ((dirX * dirX + dirY * dirY + dirZ * dirZ) > 0.0)
      {
        gp_Trsf trans;
        trans.SetRotation(gp_Ax1(gp_Pnt(pntX, pntY, pntZ), gp_Dir(dirX, dirY, dirZ)), angle);
        trans.SetTranslation(gp_Vec(mvX, mvY, mvZ));
        shape2 = BRepBuilderAPI_Transform(face, trans).Shape();
      }
      else
      {
        // move
        gp_Trsf trans;
        trans.SetTranslation(gp_Vec(mvX, mvY, mvZ));
        shape2 = BRepBuilderAPI_Transform(face, trans).Shape();
      }
      ex.Next();
      Handle(AIS_InteractiveObject) target = new AIS_Shape(shape2);
      myAISContext()->Display(target, Standard_False);
      myObj->setObject_ShapeWithSurface(target, shape2, surface, geomid);
      // bounding box
      Bnd_Box bbox;
      BRepBndLib::Add(face, bbox);
      double minX, maxX, minY, maxY, minZ, maxZ;
      bbox.Get(minX, minY, minZ, maxX, maxY, maxZ);
      if (myObj->bboxXmin > minX) myObj->bboxXmin = minX;
      if (myObj->bboxXmax < maxX) myObj->bboxXmax = maxX;
      if (myObj->bboxYmin > minY) myObj->bboxYmin = minY;
      if (myObj->bboxYmax < maxY) myObj->bboxYmax = maxY;
      if (myObj->bboxZmin > minZ) myObj->bboxZmin = minZ;
      if (myObj->bboxZmax < maxZ) myObj->bboxZmax = maxZ;
    }
    for (int i = 0; i < myObj->getLength(); i++)
    {
      if (myObj->geomids[i] == geomid)
      {
        myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
      }
    }
    myAISContext()->UpdateCurrentViewer();
  }
}

void OCCTProxy::ImportCsv_NonTransform2(char* path, int geomid, int* nx, int* ny)
{
  std::ifstream file(path);
  if (file.is_open())
  {
    std::string line;
    getline(file, line);  // trash
    getline(file, line);
    std::vector<std::string> buf_str;
    buf_str = split(line, ',');
    *nx = stoi(buf_str[0]);
    *ny = stoi(buf_str[1]);
    std::vector<std::double_t> list_x;
    std::vector<std::double_t> list_y;
    std::vector<std::double_t> list_z;
    // x
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_x.push_back(stof(buf_str[i]));
      }
    }
    // y
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_y.push_back(stof(buf_str[i]));
      }
    }
    // z
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_z.push_back(stof(buf_str[i]));
      }
    }
    // surface
    TColgp_Array2OfPnt pnts = TColgp_Array2OfPnt(1, *nx, 1, *ny);
    for (int j = 0; j < *ny; j++)
    {
      for (int i = 0; i < *nx; i++) 
      {
        int idx = j * *nx + i;
        gp_Pnt pnt = gp_Pnt(list_x[idx], list_y[idx], list_z[idx]);
        pnts.SetValue(i + 1, j + 1, pnt);
      }
    }
    GeomAPI_PointsToBSplineSurface api = GeomAPI_PointsToBSplineSurface(pnts, 3, 8, GeomAbs_G2, 0.001);
    Handle(Geom_BSplineSurface) surface = api.Surface();
    BRepBuilderAPI_MakeFace maker(surface, Precision::Confusion());
    TopoDS_Face face = maker.Face();
    Handle(AIS_InteractiveObject) target = new AIS_Shape(face);
    myAISContext()->Display(target, Standard_False);
    myObj->setObject_Face(target, face, surface, geomid);
    for (int i = 0; i < myObj->getLength(); i++)
    {
      if (myObj->geomids[i] == geomid)
      {
        myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
      }
    }
  }
}

void OCCTProxy::ImportCsv_Transform2(char* path, int geomid, int* nx, int* ny,
  double mvX, double mvY, double mvZ,
  double pntX, double pntY, double pntZ, double dirX, double dirY, double dirZ, double angle)
{
  std::ifstream file(path);
  if (file.is_open())
  {
    std::string line;
    getline(file, line);  // trash
    getline(file, line);
    std::vector<std::string> buf_str;
    buf_str = split(line, ',');
    *nx = stoi(buf_str[0]);
    *ny = stoi(buf_str[1]);
    std::vector<std::double_t> list_x;
    std::vector<std::double_t> list_y;
    std::vector<std::double_t> list_z;
    // x
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_x.push_back(stof(buf_str[i]));
      }
    }
    // y
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_y.push_back(stof(buf_str[i]));
      }
    }
    // z
    for (int j = 0; j < *ny; j++)
    {
      getline(file, line);
      buf_str = split(line, ',');
      for (int i = 0; i < *nx; i++)
      {
        list_z.push_back(stof(buf_str[i]));
      }
    }
    // surface
    TColgp_Array2OfPnt pnts = TColgp_Array2OfPnt(1, *nx, 1, *ny);
    for (int j = 0; j < *ny; j++)
    {
      for (int i = 0; i < *nx; i++)
      {
        int idx = j * *nx + i;
        gp_Pnt pnt = gp_Pnt(list_x[idx], list_y[idx], list_z[idx]);
        pnts.SetValue(i + 1, j + 1, pnt);
      }
    }
    GeomAPI_PointsToBSplineSurface api = GeomAPI_PointsToBSplineSurface(pnts, 3, 8, GeomAbs_G2, 0.001);
    Handle(Geom_BSplineSurface) surface = api.Surface();
    BRepBuilderAPI_MakeFace maker(surface, Precision::Confusion());
    TopoDS_Face face = maker.Face();
    // rotate
    TopoDS_Shape shape;
    if ((dirX * dirX + dirY * dirY + dirZ * dirZ) > 0.0)
    {
      gp_Trsf trans;
      trans.SetRotation(gp_Ax1(gp_Pnt(pntX, pntY, pntZ), gp_Dir(dirX, dirY, dirZ)), angle);
      trans.SetTranslation(gp_Vec(mvX, mvY, mvZ));
      shape = BRepBuilderAPI_Transform(face, trans).Shape();
    }
    else
    {
      // move
      gp_Trsf trans;
      trans.SetTranslation(gp_Vec(mvX, mvY, mvZ));
      shape = BRepBuilderAPI_Transform(face, trans).Shape();
    }
    Handle(AIS_InteractiveObject) target = new AIS_Shape(shape);
    myAISContext()->Display(target, Standard_False);
    myObj->setObject_ShapeWithSurface(target, shape, surface, geomid);
    for (int i = 0; i < myObj->getLength(); i++)
    {
      if (myObj->geomids[i] == geomid)
      {
        myAISContext()->SetDisplayMode(AIS_Shaded, Standard_False);
      }
    }
  }
}