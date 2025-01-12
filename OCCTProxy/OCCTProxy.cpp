#include "OCCTProxy.hpp"


/// <summary>
/// Proxy class encapsulating calls to OCCT C++ classes within 
/// C++/CLI class visible from .Net (CSharp)
/// </summary>

// ============================================
// Viewer functionality
// ============================================

/// <summary>
///Initialize a viewer
/// </summary>
/// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>
bool OCCTProxy::InitViewer(System::IntPtr theWnd)
{
  try
  {
    Handle(Aspect_DisplayConnection) aDisplayConnection;
    myGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
  }
  catch (Standard_Failure)
  {
    return false;
  }

  myViewer() = new V3d_Viewer(myGraphicDriver());
  myViewer()->SetDefaultLights();
  myViewer()->SetLightOn();
  myView() = myViewer()->CreateView();
  Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
  myView()->SetWindow(aWNTWindow);
  if (!aWNTWindow->IsMapped())
  {
    aWNTWindow->Map();
  }
  myAISContext() = new AIS_InteractiveContext(myViewer());
  myAISContext()->UpdateCurrentViewer();
  myView()->Redraw();
  myView()->MustBeResized();
  return true;
}

/// <summary>
/// Make dump of current view to file
/// </summary>
/// <param name="theFileName">Name of dump file</param>
bool OCCTProxy::Dump(const TCollection_AsciiString& theFileName)
{
  if (myView().IsNull())
  {
    return false;
  }
  myView()->Redraw();
  return myView()->Dump(theFileName.ToCString()) != Standard_False;
}

/// <summary>
///Redraw view
/// </summary>
void OCCTProxy::RedrawView(void)
{
  if (!myView().IsNull())
  {
    myView()->Redraw();
  }
}

/// <summary>
///Update view
/// </summary>
void OCCTProxy::UpdateView(void)
{
  if (!myView().IsNull())
  {
    myView()->MustBeResized();
  }
}

/// <summary>
///Set computed mode in false
/// </summary>
void OCCTProxy::SetDegenerateModeOn(void)
{
  if (!myView().IsNull())
  {
    myView()->SetComputedMode(Standard_False);
    myView()->Redraw();
  }
}

/// <summary>
///Set computed mode in true
/// </summary>
void OCCTProxy::SetDegenerateModeOff(void)
{
  if (!myView().IsNull())
  {
    myView()->SetComputedMode(Standard_True);
    myView()->Redraw();
  }
}

/// <summary>
///Fit all
/// </summary>
void OCCTProxy::WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax)
{
  if (!myView().IsNull())
  {
    myView()->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
  }
}

/// <summary>
///Current place of window
/// </summary>
/// <param name="theZoomFactor">Current zoom</param>
void OCCTProxy::Place(int theX, int theY, float theZoomFactor)
{
  Standard_Real aZoomFactor = theZoomFactor;
  if (!myView().IsNull())
  {
    myView()->Place(theX, theY, aZoomFactor);
  }
}

/// <summary>
///Set Zoom
/// </summary>
void OCCTProxy::Zoom(int theX1, int theY1, int theX2, int theY2)
{
  if (!myView().IsNull())
  {
    myView()->Zoom(theX1, theY1, theX2, theY2);
  }
}

/// <summary>
///Set Pan
/// </summary>
void OCCTProxy::Pan(int theX, int theY)
{
  if (!myView().IsNull())
  {
    myView()->Pan(theX, theY);
  }
}

/// <summary>
///Rotation
/// </summary>
void OCCTProxy::Rotation(int theX, int theY)
{
  if (!myView().IsNull())
  {
    myView()->Rotation(theX, theY);
  }
}

/// <summary>
///Start rotation
/// </summary>
void OCCTProxy::StartRotation(int theX, int theY)
{
  if (!myView().IsNull())
  {
    myView()->StartRotation(theX, theY);
  }
}

/// <summary>
///Select by rectangle
/// </summary>
void OCCTProxy::Select(int theX1, int theY1, int theX2, int theY2)
{
  if (!myAISContext().IsNull())
  {
    myAISContext()->SelectRectangle(Graphic3d_Vec2i(theX1, theY1),
      Graphic3d_Vec2i(theX2, theY2),
      myView());
    myAISContext()->UpdateCurrentViewer();
  }
}

/// <summary>
///Select by click
/// </summary>
void OCCTProxy::Select(void)
{
  if (!myAISContext().IsNull())
  {
    myAISContext()->SelectDetected();
    myAISContext()->UpdateCurrentViewer();
  }
}

/// <summary>
///Move view
/// </summary>
void OCCTProxy::MoveTo(int theX, int theY)
{
  if ((!myAISContext().IsNull()) && (!myView().IsNull()))
  {
    myAISContext()->MoveTo(theX, theY, myView(), Standard_True);
  }
}

/// <summary>
///Select by rectangle with pressed "Shift" key
/// </summary>
void OCCTProxy::ShiftSelect(int theX1, int theY1, int theX2, int theY2)
{
  if ((!myAISContext().IsNull()) && (!myView().IsNull()))
  {
    myAISContext()->SelectRectangle(Graphic3d_Vec2i(theX1, theY1),
      Graphic3d_Vec2i(theX2, theY2),
      myView(),
      AIS_SelectionScheme_XOR);
    myAISContext()->UpdateCurrentViewer();
  }
}

/// <summary>
///Select by "Shift" key
/// </summary>
void OCCTProxy::ShiftSelect(void)
{
  if (!myAISContext().IsNull())
  {
    myAISContext()->SelectDetected(AIS_SelectionScheme_XOR);
    myAISContext()->UpdateCurrentViewer();
  }
}

/// <summary>
///Set background color
/// </summary>
void OCCTProxy::BackgroundColor(int& theRed, int& theGreen, int& theBlue)
{
  Standard_Real R1;
  Standard_Real G1;
  Standard_Real B1;
  if (!myView().IsNull())
  {
    myView()->BackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
  }
  theRed = (int)R1 * 255;
  theGreen = (int)G1 * 255;
  theBlue = (int)B1 * 255;
}

/// <summary>
///Get background color Red
/// </summary>
int OCCTProxy::GetBGColR(void)
{
  int aRed, aGreen, aBlue;
  BackgroundColor(aRed, aGreen, aBlue);
  return aRed;
}

/// <summary>
///Get background color Green
/// </summary>
int OCCTProxy::GetBGColG(void)
{
  int aRed, aGreen, aBlue;
  BackgroundColor(aRed, aGreen, aBlue);
  return aGreen;
}

/// <summary>
///Get background color Blue
/// </summary>
int OCCTProxy::GetBGColB(void)
{
  int aRed, aGreen, aBlue;
  BackgroundColor(aRed, aGreen, aBlue);
  return aBlue;
}

/// <summary>
///Update current viewer
/// </summary>
void OCCTProxy::UpdateCurrentViewer(void)
{
  if (!myAISContext().IsNull())
  {
    myAISContext()->UpdateCurrentViewer();
  }
}

/// <summary>
///Front side
/// </summary>
void OCCTProxy::FrontView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Yneg);
  }
}

/// <summary>
///Top side
/// </summary>
void OCCTProxy::TopView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Zpos);
  }
}

/// <summary>
///Left side
/// </summary>
void OCCTProxy::LeftView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Xneg);
  }
}

/// <summary>
///Back side
/// </summary>
void OCCTProxy::BackView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Ypos);
  }
}

/// <summary>
///Right side
/// </summary>
void OCCTProxy::RightView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Xpos);
  }
}

/// <summary>
///Bottom side
/// </summary>
void OCCTProxy::BottomView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_Zneg);
  }
}

/// <summary>
///Axo side
/// </summary>
void OCCTProxy::AxoView(void)
{
  if (!myView().IsNull())
  {
    myView()->SetProj(V3d_XposYnegZpos);
  }
}

/// <summary>
///Scale
/// </summary>
float OCCTProxy::Scale(void)
{
  if (myView().IsNull())
  {
    return -1;
  }
  else
  {
    return (float)myView()->Scale();
  }
}

/// <summary>
///Zoom in all view
/// </summary>
void OCCTProxy::ZoomAllView(void)
{
  if (!myView().IsNull())
  {
    myView()->FitAll();
    myView()->ZFitAll();
  }
}

/// <summary>
///Reset view
/// </summary>
void OCCTProxy::Reset(void)
{
  if (!myView().IsNull())
  {
    myView()->Reset();
  }
}

/// <summary>
///Set display mode of objects
/// </summary>
/// <param name="theMode">Set current mode</param>
void OCCTProxy::SetDisplayMode(int theMode)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  AIS_DisplayMode aCurrentMode;
  if (theMode == 0)
  {
    aCurrentMode = AIS_WireFrame;
  }
  else
  {
    aCurrentMode = AIS_Shaded;
  }

  if (myAISContext()->NbSelected() == 0)
  {
    myAISContext()->SetDisplayMode(aCurrentMode, Standard_False);
  }
  else
  {
    for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
    {
      myAISContext()->SetDisplayMode(myAISContext()->SelectedInteractive(), theMode, Standard_False);
    }
  }
  myAISContext()->UpdateCurrentViewer();
}

/// <summary>
///Set color
/// </summary>
void OCCTProxy::SetColor(int theR, int theG, int theB)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  Quantity_Color aCol = Quantity_Color(theR / 255., theG / 255., theB / 255., Quantity_TOC_RGB);
  for (; myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    myAISContext()->SetColor(myAISContext()->SelectedInteractive(), aCol, Standard_False);
  }
  myAISContext()->UpdateCurrentViewer();
}

/// <summary>
///Get object color red
/// </summary>
int OCCTProxy::GetObjColR(void)
{
  int aRed, aGreen, aBlue;
  ObjectColor(aRed, aGreen, aBlue);
  return aRed;
}

/// <summary>
///Get object color green
/// </summary>
int OCCTProxy::GetObjColG(void)
{
  int aRed, aGreen, aBlue;
  ObjectColor(aRed, aGreen, aBlue);
  return aGreen;
}

/// <summary>
///Get object color blue
/// </summary>
int OCCTProxy::GetObjColB(void)
{
  int aRed, aGreen, aBlue;
  ObjectColor(aRed, aGreen, aBlue);
  return aBlue;
}

/// <summary>
///Get object color R/G/B
/// </summary>
void OCCTProxy::ObjectColor(int& theRed, int& theGreen, int& theBlue)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  theRed = 255;
  theGreen = 255;
  theBlue = 255;
  Handle(AIS_InteractiveObject) aCurrent;
  myAISContext()->InitSelected();
  if (!myAISContext()->MoreSelected())
  {
    return;
  }
  aCurrent = myAISContext()->SelectedInteractive();
  if (aCurrent->HasColor())
  {
    Quantity_Color anObjCol;
    myAISContext()->Color(aCurrent, anObjCol);
    Standard_Real r1, r2, r3;
    anObjCol.Values(r1, r2, r3, Quantity_TOC_RGB);
    theRed = (int)r1 * 255;
    theGreen = (int)r2 * 255;
    theBlue = (int)r3 * 255;
  }
}

/// <summary>
///Set background color R/G/B
/// </summary>
void OCCTProxy::SetBackgroundColor(int theRed, int theGreen, int theBlue)
{
  if (!myView().IsNull())
  {
    myView()->SetBackgroundColor(Quantity_TOC_RGB, theRed / 255., theGreen / 255., theBlue / 255.);
  }
}

/// <summary>
///Erase objects
/// </summary>
void OCCTProxy::EraseObjects(void)
{
  if (myAISContext().IsNull())
  {
    return;
  }

  myAISContext()->EraseSelected(Standard_False);
  myAISContext()->ClearSelected(Standard_True);
}

/// <summary>
///Get version
/// </summary>
float OCCTProxy::GetOCCVersion(void)
{
  return (float)OCC_VERSION;
}

/// <summary>
///set material
/// </summary>
void OCCTProxy::SetMaterial(int theMaterial)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    myAISContext()->SetMaterial(myAISContext()->SelectedInteractive(), (Graphic3d_NameOfMaterial)theMaterial, Standard_False);
  }
  myAISContext()->UpdateCurrentViewer();
}

/// <summary>
///set transparency
/// </summary>
void OCCTProxy::SetTransparency(int theTrans)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    myAISContext()->SetTransparency(myAISContext()->SelectedInteractive(), ((Standard_Real)theTrans) / 10.0, Standard_False);
  }
  myAISContext()->UpdateCurrentViewer();
}

/// <summary>
///Return true if object is selected
/// </summary>
bool OCCTProxy::IsObjectSelected(void)
{
  if (myAISContext().IsNull())
  {
    return false;
  }
  myAISContext()->InitSelected();
  return myAISContext()->MoreSelected() != Standard_False;
}

/// <summary>
///Return display mode
/// </summary>
int OCCTProxy::DisplayMode(void)
{
  if (myAISContext().IsNull())
  {
    return -1;
  }
  int aMode = -1;
  bool OneOrMoreInShading = false;
  bool OneOrMoreInWireframe = false;
  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    if (myAISContext()->IsDisplayed(myAISContext()->SelectedInteractive(), 1))
    {
      OneOrMoreInShading = true;
    }
    if (myAISContext()->IsDisplayed(myAISContext()->SelectedInteractive(), 0))
    {
      OneOrMoreInWireframe = true;
    }
  }
  if (OneOrMoreInShading && OneOrMoreInWireframe)
  {
    aMode = 10;
  }
  else if (OneOrMoreInShading)
  {
    aMode = 1;
  }
  else if (OneOrMoreInWireframe)
  {
    aMode = 0;
  }

  return aMode;
}

/// <summary>
///Create new view
/// </summary>
/// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>
void OCCTProxy::CreateNewView(System::IntPtr theWnd)
{
  if (myAISContext().IsNull())
  {
    return;
  }
  myView() = myAISContext()->CurrentViewer()->CreateView();
  if (myGraphicDriver().IsNull())
  {
    myGraphicDriver() = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
  }
  Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
  myView()->SetWindow(aWNTWindow);
  Standard_Integer w = 100, h = 100;
  aWNTWindow->Size(w, h);
  if (!aWNTWindow->IsMapped())
  {
    aWNTWindow->Map();
  }
}

/// <summary>
///Set AISContext
/// </summary>
bool OCCTProxy::SetAISContext(OCCTProxy^ theViewer)
{
  this->myAISContext() = theViewer->GetContext();
  if (myAISContext().IsNull())
  {
    return false;
  }
  return true;
}

/// <summary>
///Get AISContext
/// </summary>
Handle(AIS_InteractiveContext) OCCTProxy::GetContext(void)
{
  return myAISContext();
}

// ============================================
// Import / export functionality
// ============================================

/// <summary>
///Import BRep file
/// </summary>
/// <param name="theFileName">Name of import file</param>
bool OCCTProxy::ImportBrep(System::String^ theFileName)
{
  return ImportBrep(toAsciiString(theFileName));
}

/// <summary>
///Import BRep file
/// </summary>
/// <param name="theFileName">Name of import file</param>
bool OCCTProxy::ImportBrep(const TCollection_AsciiString& theFileName)
{
  TopoDS_Shape aShape;
  BRep_Builder aBuilder;
  Standard_Boolean isResult = BRepTools::Read(aShape, theFileName.ToCString(), aBuilder);
  if (!isResult)
  {
    return false;
  }

  myAISContext()->Display(new AIS_Shape(aShape), Standard_True);
  return true;
}

/// <summary>
///Import Step file
/// </summary>
/// <param name="theFileName">Name of import file</param>
bool OCCTProxy::ImportStep(const TCollection_AsciiString& theFileName)
{
  STEPControl_Reader aReader;
  IFSelect_ReturnStatus aStatus = aReader.ReadFile(theFileName.ToCString());
  if (aStatus == IFSelect_RetDone)
  {
    bool isFailsonly = false;
    aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

    int aNbRoot = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
    for (Standard_Integer n = 1; n <= aNbRoot; n++)
    {
      Standard_Boolean ok = aReader.TransferRoot(n);
      int aNbShap = aReader.NbShapes();
      if (aNbShap > 0)
      {
        for (int i = 1; i <= aNbShap; i++)
        {
          TopoDS_Shape aShape = aReader.Shape(i);
          myAISContext()->Display(new AIS_Shape(aShape), Standard_False);
        }
        myAISContext()->UpdateCurrentViewer();
      }
    }
  }
  else
  {
    return false;
  }

  return true;
}

/// <summary>
///Import Iges file
/// </summary>
/// <param name="theFileName">Name of import file</param>
bool OCCTProxy::ImportIges(const TCollection_AsciiString& theFileName)
{
  IGESControl_Reader aReader;
  int aStatus = aReader.ReadFile(theFileName.ToCString());

  if (aStatus == IFSelect_RetDone)
  {
    aReader.TransferRoots();
    TopoDS_Shape aShape = aReader.OneShape();
    myAISContext()->Display(new AIS_Shape(aShape), Standard_False);
  }
  else
  {
    return false;
  }

  myAISContext()->UpdateCurrentViewer();
  return true;
}

/// <summary>
///Export BRep file
/// </summary>
/// <param name="theFileName">Name of export file</param>
bool OCCTProxy::ExportBRep(const TCollection_AsciiString& theFileName)
{
  myAISContext()->InitSelected();
  if (!myAISContext()->MoreSelected())
  {
    return false;
  }

  Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
  Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
  return BRepTools::Write(anIS->Shape(), theFileName.ToCString()) != Standard_False;
}

/// <summary>
///Export Step file
/// </summary>
/// <param name="theFileName">Name of export file</param>
bool OCCTProxy::ExportStep(const TCollection_AsciiString& theFileName)
{
  STEPControl_StepModelType aType = STEPControl_AsIs;
  IFSelect_ReturnStatus aStatus;
  STEPControl_Writer aWriter;
  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
    Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
    TopoDS_Shape aShape = anIS->Shape();
    aStatus = aWriter.Transfer(aShape, aType);
    if (aStatus != IFSelect_RetDone)
    {
      return false;
    }
  }

  aStatus = aWriter.Write(theFileName.ToCString());
  if (aStatus != IFSelect_RetDone)
  {
    return false;
  }

  return true;
}

/// <summary>
///Export Iges file
/// </summary>
/// <param name="theFileName">Name of export file</param>
bool OCCTProxy::ExportIges(const TCollection_AsciiString& theFileName)
{
  IGESControl_Controller::Init();
  IGESControl_Writer aWriter(Interface_Static::CVal("XSTEP.iges.unit"),
    Interface_Static::IVal("XSTEP.iges.writebrep.mode"));

  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
    Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
    TopoDS_Shape aShape = anIS->Shape();
    aWriter.AddShape(aShape);
  }

  aWriter.ComputeModel();
  return aWriter.Write(theFileName.ToCString()) != Standard_False;
}

/// <summary>
///Export Vrml file
/// </summary>
/// <param name="theFileName">Name of export file</param>
bool OCCTProxy::ExportVrml(const TCollection_AsciiString& theFileName)
{
  TopoDS_Compound aRes;
  BRep_Builder aBuilder;
  aBuilder.MakeCompound(aRes);

  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
    Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
    TopoDS_Shape aShape = anIS->Shape();
    if (aShape.IsNull())
    {
      return false;
    }

    aBuilder.Add(aRes, aShape);
  }

  VrmlAPI_Writer aWriter;
  aWriter.Write(aRes, theFileName.ToCString());

  return true;
}

/// <summary>
///Export Stl file
/// </summary>
/// <param name="theFileName">Name of export file</param>
bool OCCTProxy::ExportStl(const TCollection_AsciiString& theFileName)
{
  TopoDS_Compound aComp;
  BRep_Builder aBuilder;
  aBuilder.MakeCompound(aComp);

  for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
  {
    Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
    Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
    TopoDS_Shape aShape = anIS->Shape();
    if (aShape.IsNull())
    {
      return false;
    }
    aBuilder.Add(aComp, aShape);
  }

  StlAPI_Writer aWriter;
  aWriter.Write(aComp, theFileName.ToCString());
  return true;
}

/// <summary>
///Define which Import/Export function must be called
/// </summary>
/// <param name="theFileName">Name of Import/Export file</param>
/// <param name="theFormat">Determines format of Import/Export file</param>
/// <param name="theIsImport">Determines is Import or not</param>
bool OCCTProxy::TranslateModel(System::String^ theFileName, int theFormat, bool theIsImport)
{
  bool isResult;

  const TCollection_AsciiString aFilename = toAsciiString(theFileName);
  if (theIsImport)
  {
    switch (theFormat)
    {
    case 0:
      isResult = ImportBrep(aFilename);
      break;
    case 1:
      isResult = ImportStep(aFilename);
      break;
    case 2:
      isResult = ImportIges(aFilename);
      break;
    default:
      isResult = false;
    }
  }
  else
  {
    switch (theFormat)
    {
    case 0:
      isResult = ExportBRep(aFilename);
      break;
    case 1:
      isResult = ExportStep(aFilename);
      break;
    case 2:
      isResult = ExportIges(aFilename);
      break;
    case 3:
      isResult = ExportVrml(aFilename);
      break;
    case 4:
      isResult = ExportStl(aFilename);
      break;
    case 5:
      isResult = Dump(aFilename);
      break;
    default:
      isResult = false;
    }
  }
  return isResult;
}

/// <summary>
///Initialize OCCTProxy
/// </summary>
void OCCTProxy::InitOCCTProxy(void)
{
  myGraphicDriver() = NULL;
  myViewer() = NULL;
  myView() = NULL;
  myAISContext() = NULL;
  // add
  myObj = gcnew OCCTObjects(LENGTH_TOPOS);
  mySlice = gcnew SliceObjects(LENGTH_TOPOS);
}
