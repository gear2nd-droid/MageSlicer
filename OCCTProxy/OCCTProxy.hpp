#ifndef OCCT_PROXY
#define OCCT_PROXY

// include required OCCT headers
#include <opencascade/Standard_Version.hxx>
#include <opencascade/Message_ProgressIndicator.hxx>
#include <opencascade/Message_ProgressScope.hxx>
//for OCC graphic
#include <opencascade/Aspect_DisplayConnection.hxx>
#include <opencascade/WNT_Window.hxx>
#include <opencascade/OpenGl_GraphicDriver.hxx>
//for object display
#include <opencascade/V3d_Viewer.hxx>
#include <opencascade/V3d_View.hxx>
#include <opencascade/AIS_InteractiveContext.hxx>
#include <opencascade/AIS_Shape.hxx>
//topology
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/TopoDS_Compound.hxx>
//brep tools
#include <opencascade/BRep_Builder.hxx>
#include <opencascade/BRepTools.hxx>
// iges I/E
#include <opencascade/IGESControl_Reader.hxx>
#include <opencascade/IGESControl_Controller.hxx>
#include <opencascade/IGESControl_Writer.hxx>
#include <opencascade/IFSelect_ReturnStatus.hxx>
#include <opencascade/Interface_Static.hxx>
//step I/E
#include <opencascade/STEPControl_Reader.hxx>
#include <opencascade/STEPControl_Writer.hxx>
//for stl export
#include <opencascade/StlAPI_Writer.hxx>
//for vrml export
#include <opencascade/VrmlAPI_Writer.hxx>
//wrapper of pure C++ classes to ref classes
#include <opencascade/NCollection_Haft.h>
//for slicetool
#include <opencascade/gp_Pnt.hxx>
#include <opencascade/BRepPRimAPI_MakeSphere.hxx>
#include <opencascade/TopoDS_Shape.hxx>

// for slice
#include "SliceObjects.hpp"

// for import and view
#include "OCCTObjects.hpp"

// for utility
#include "CalcUtility.hpp"

// for infill
#include "SliceTools.hpp"

// openmp
#include <omp.h>



// common
#include <vcclr.h>
#include <string>

#define LENGTH_TOPOS 1000000






//static TCollection_AsciiString toAsciiString(String^ theString);
//! Auxiliary tool for converting C# string into UTF-8 string.
static TCollection_AsciiString toAsciiString(String^ theString)
{
  if (theString == nullptr)
  {
    return TCollection_AsciiString();
  }

  pin_ptr<const wchar_t> aPinChars = PtrToStringChars(theString);
  const wchar_t* aWCharPtr = aPinChars;
  if (aWCharPtr == NULL
    || *aWCharPtr == L'\0')
  {
    return TCollection_AsciiString();
  }
  return TCollection_AsciiString(aWCharPtr);
}

public ref class OCCTProxy
{
public:
  bool InitViewer(System::IntPtr theWnd);
  bool Dump(const TCollection_AsciiString& theFileName);
  void RedrawView(void);
  void UpdateView(void);
  void SetDegenerateModeOn(void);
  void SetDegenerateModeOff(void);
  void WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax);
  void Place(int theX, int theY, float theZoomFactor);
  void Zoom(int theX1, int theY1, int theX2, int theY2);
  void Pan(int theX, int theY);
  void Rotation(int theX, int theY);
  void StartRotation(int theX, int theY);
  void Select(int theX1, int theY1, int theX2, int theY2);
  void Select(void);
  void MoveTo(int theX, int theY);
  void ShiftSelect(int theX1, int theY1, int theX2, int theY2);
  void ShiftSelect(void);
  void BackgroundColor(int& theRed, int& theGreen, int& theBlue);
  int GetBGColR(void);
  int GetBGColG(void);
  int GetBGColB(void);
  void UpdateCurrentViewer(void);
  void FrontView(void);
  void TopView(void);
  void LeftView(void);
  void BackView(void);
  void RightView(void);
  void BottomView(void);
  void AxoView(void);
  float Scale(void);
  void ZoomAllView(void);
  void Reset(void);
  void SetDisplayMode(int theMode);
  void SetColor(int theR, int theG, int theB);
  int GetObjColR(void);
  int GetObjColG(void);
  int GetObjColB(void);
  void ObjectColor(int& theRed, int& theGreen, int& theBlue);
  void SetBackgroundColor(int theRed, int theGreen, int theBlue);
  void EraseObjects(void);
  float GetOCCVersion(void);
  void SetMaterial(int theMaterial);
  void SetTransparency(int theTrans);
  bool IsObjectSelected(void);
  int DisplayMode(void);
  void CreateNewView(System::IntPtr theWnd);
  bool SetAISContext(OCCTProxy^ theViewer);
  Handle(AIS_InteractiveContext) GetContext(void);
  bool ImportBrep(System::String^ theFileName);
  bool ImportBrep(const TCollection_AsciiString& theFileName);
  bool ImportStep(const TCollection_AsciiString& theFileName);
  bool ImportIges(const TCollection_AsciiString& theFileName);
  bool ExportBRep(const TCollection_AsciiString& theFileName);
  bool ExportStep(const TCollection_AsciiString& theFileName);
  bool ExportIges(const TCollection_AsciiString& theFileName);
  bool ExportVrml(const TCollection_AsciiString& theFileName);
  bool ExportStl(const TCollection_AsciiString& theFileName);
  bool TranslateModel(System::String^ theFileName, int theFormat, bool theIsImport);
  void InitOCCTProxy(void);
  
  // add OCCTObjects
  void SetColor(int geomid, int r, int g, int b);
  void SetTransparent(int geomid, bool trans);
  void SetWireframe(int geomid);
  void SetShading(int geomid);
  void ImportStep_NonTransform(char* path, int geomid);
  void ImportStep_Transform(char* path, int geomid,
    double mvX, double mvY, double mvZ, 
    double pntX, double pntY, double pntZ, double dirX, double dirY, double dirZ, double angle);
  void ImportCsv_NonTransform2(char* path, int geomid, int* nx, int* ny);
  void ImportCsv_Transform2(char* path, int geomid, int* nx, int* ny,
    double mvX, double mvY, double mvZ,
    double pntX, double pntY, double pntZ, double dirX, double dirY, double dirZ, double angle);

  // add SliceObjects
  void createOffsetObject(double offsetVal, int orgGeomId, int newGeomId);
  int calcLayer(int layerCnt,
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx,
    int geomidUpper, int geomIdlower, int geomIdBase,
    int nx, int ny, double pitch, int surfaceR, int surfaceG, int surfaceB);
  void SetObjectColor(int id, int r, int g, int b);
  void SetSurfaceTransparent(int surface);
  void SetSurfaceCurveTransparent(int surface, int outerWall, int infill);
  int calcPoints(char* path, int layerCnt, double pointsDistance, double width,
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, 
    array<int>^ underIdx, int volumeCnt, int geomidVolume,
    double prePointX, double prePointY, double prePointZ, 
    double& lastPointX, double& lastPointY, double& lastPointZ,
    double preNormI, double preNormJ, double preNormK,
    double& lastNormI, double& lastNormJ, double& lastNormK,
    bool infillEnable, int infillType, double infillPercent, array<int>^ underInfillIdx,
    bool gapFillEnable, double gapFillPercent,
    bool outerWallFlag, int numWall);
  int getLayerCenter(int geomIdSurface, double& x, double& y, double& z);
  void DeleteSurfaceByTransparent(int id);
  void DeleteCurveByTransparent(int id);
  int OCCTProxy::calcBottomPoints(char* path, double pointsDistance, double width,
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx,
    array<int>^ underIdx, int volumeCnt, int volumeGeomid, int numWall, int direction,
    double prePointX, double prePointY, double prePointZ, 
    double& lastPointX, double& lastPointY, double& lastPointZ,
    double preNormI, double preNormJ, double preNormK,
    double& lastNormI, double& lastNormJ, double& lastNormK);
  int OCCTProxy::calcBottomLayer(
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx,
    int geomidUpper, int geomIdBase, int nx, int ny,
    int surfaceR, int surfaceG, int surfaceB);
  int OCCTProxy::calcAutoSliceLayer(
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx,
    int volumeGeomid, int geomIdBase, double checkDist,
    int surfaceR, int surfaceG, int surfaceB);
  int OCCTProxy::calcPeelerLayer(
    array<int>^ surfaceIdx, array<int>^ wallIdx, array<int>^ infillIdx, array<int>^ supportIdx);
  int OCCTProxy::calcPeelerPoints(char* path, double pointsDistance, double width, double thick,
    array<int>^ wallIdx, int peelerGeomid, int volumeCnt, bool flag,
    double prePointX, double prePointY, double prePointZ,
    double& lastPointX, double& lastPointY, double& lastPointZ,
    double preNormI, double preNormJ, double preNormK,
    double& lastNormI, double& lastNormJ, double& lastNormK);



private:
  // fields
  NCollection_Haft<Handle(V3d_Viewer)> myViewer;
  NCollection_Haft<Handle(V3d_View)> myView;
  NCollection_Haft<Handle(AIS_InteractiveContext)> myAISContext;
  NCollection_Haft<Handle(OpenGl_GraphicDriver)> myGraphicDriver;
  // add
  OCCTObjects^ myObj = nullptr;
  SliceObjects^ mySlice = nullptr;
  double defaultThickness;
  int preSliceStartLayer = 0;

};

#endif