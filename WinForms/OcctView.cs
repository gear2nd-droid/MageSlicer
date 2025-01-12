using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace IE_WinForms
{
  public enum CurrentAction3d
  {
    CurAction3d_Nothing,
    CurAction3d_DynamicZooming,
    CurAction3d_WindowZooming,
    CurAction3d_DynamicPanning,
    CurAction3d_GlobalPanning,
    CurAction3d_DynamicRotation,
    CurAction3d_Multi,
  }
  public enum CurrentPressedKey
  {
    CurPressedKey_Nothing,
    CurPressedKey_Ctrl,
    CurPressedKey_Shift
  }
  public enum ModelFormat
  {
    BREP,
    STEP,
    IGES,
    VRML,
    STL,
    IMAGE
  }
  /// <summary>
  /// Summary description for Form2.
  /// </summary>
  public class OcctView : System.Windows.Forms.Form
  {
    private System.ComponentModel.IContainer components;

    public OcctView()
    {
      //
      // Required for Windows Form Designer support
      //
      InitializeComponent();

      //
      // Create OCCT proxy object
      //
      myOCCTProxy = new OCCTProxy();
      //myCurrentMode = CurrentAction3d.CurAction3d_Nothing;
      myCurrentMode = CurrentAction3d.CurAction3d_Multi;
      myCurrentPressedKey = CurrentPressedKey.CurPressedKey_Nothing;
      myDegenerateModeIsOn = true;
      IsRectVisible = false;
    }

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    protected override void Dispose(bool disposing)
    {
      if (disposing)
      {
        if (components != null)
        {
          components.Dispose();
        }
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OcctView));
      this.imageList1 = new System.Windows.Forms.ImageList(this.components);
      this.myPopup = new System.Windows.Forms.ContextMenu();
      this.menuItem1 = new System.Windows.Forms.MenuItem();
      this.myPopupObject = new System.Windows.Forms.ContextMenu();
      this.ContextWireframe = new System.Windows.Forms.MenuItem();
      this.ContextShading = new System.Windows.Forms.MenuItem();
      this.ContextColor = new System.Windows.Forms.MenuItem();
      this.ContextMaterial = new System.Windows.Forms.MenuItem();
      this.ContMatBrass = new System.Windows.Forms.MenuItem();
      this.ContMenBronze = new System.Windows.Forms.MenuItem();
      this.ContMenCopper = new System.Windows.Forms.MenuItem();
      this.ContMenGold = new System.Windows.Forms.MenuItem();
      this.ContMenPewt = new System.Windows.Forms.MenuItem();
      this.ContMenPlaster = new System.Windows.Forms.MenuItem();
      this.ContMenPlastic = new System.Windows.Forms.MenuItem();
      this.ContMenSilver = new System.Windows.Forms.MenuItem();
      this.ContMenTranc = new System.Windows.Forms.MenuItem();
      this.ContMenDelete = new System.Windows.Forms.MenuItem();
      this.SuspendLayout();
      // 
      // imageList1
      // 
      this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
      this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
      this.imageList1.Images.SetKeyName(0, "");
      this.imageList1.Images.SetKeyName(1, "");
      this.imageList1.Images.SetKeyName(2, "");
      this.imageList1.Images.SetKeyName(3, "");
      this.imageList1.Images.SetKeyName(4, "");
      this.imageList1.Images.SetKeyName(5, "");
      this.imageList1.Images.SetKeyName(6, "");
      this.imageList1.Images.SetKeyName(7, "");
      this.imageList1.Images.SetKeyName(8, "");
      this.imageList1.Images.SetKeyName(9, "");
      this.imageList1.Images.SetKeyName(10, "");
      this.imageList1.Images.SetKeyName(11, "");
      this.imageList1.Images.SetKeyName(12, "");
      this.imageList1.Images.SetKeyName(13, "");
      this.imageList1.Images.SetKeyName(14, "");
      this.imageList1.Images.SetKeyName(15, "");
      // 
      // myPopup
      // 
      this.myPopup.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem1});
      // 
      // menuItem1
      // 
      this.menuItem1.Index = 0;
      this.menuItem1.Text = "Change &Background";
      this.menuItem1.Click += new System.EventHandler(this.menuItem1_Click);
      // 
      // OcctView
      // 
      this.AccessibleRole = System.Windows.Forms.AccessibleRole.Window;
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
      this.ClientSize = new System.Drawing.Size(624, 441);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.Name = "OcctView";
      this.Text = "Document";
      this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
      this.Closed += new System.EventHandler(this.Form2_Closed);
      this.SizeChanged += new System.EventHandler(this.Form2_SizeChanged);
      this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form2_Paint);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form2_KeyDown);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form2_KeyUp);
      this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form2_MouseDown);
      this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form2_MouseMove);
      this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form2_MouseUp);
      this.ResumeLayout(false);

    }
    #endregion

    private System.Windows.Forms.ImageList imageList1;
    private System.Windows.Forms.ContextMenu myPopup;
    private System.Windows.Forms.ContextMenu myPopupObject;
    private System.Windows.Forms.MenuItem ContextWireframe;
    private System.Windows.Forms.MenuItem ContextShading;
    private System.Windows.Forms.MenuItem ContextColor;
    private System.Windows.Forms.MenuItem ContextMaterial;
    private System.Windows.Forms.MenuItem ContMatBrass;
    private System.Windows.Forms.MenuItem ContMenBronze;
    private System.Windows.Forms.MenuItem ContMenCopper;
    private System.Windows.Forms.MenuItem ContMenGold;
    private System.Windows.Forms.MenuItem ContMenPewt;
    private System.Windows.Forms.MenuItem ContMenPlaster;
    private System.Windows.Forms.MenuItem ContMenPlastic;
    private System.Windows.Forms.MenuItem ContMenSilver;
    private System.Windows.Forms.MenuItem ContMenTranc;
    private System.Windows.Forms.MenuItem ContMenDelete;
    private System.Windows.Forms.MenuItem menuItem1;

    public OCCTProxy myOCCTProxy;
    private int myDocumentIndex, myViewIndex;

    public void InitV3D()
    {
      if (!myOCCTProxy.InitViewer(this.Handle))
        MessageBox.Show("Fatal Error during the graphic initialisation", "Error!",
                MessageBoxButtons.OK, MessageBoxIcon.Error);
    }

    private void Form2_SizeChanged(object sender, System.EventArgs e)
    {
      myOCCTProxy.UpdateView();
    }

    private void Form2_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      myOCCTProxy.RedrawView();
      myOCCTProxy.UpdateView();
    }

    protected CurrentAction3d myCurrentMode;
    protected CurrentPressedKey myCurrentPressedKey;
    protected float myCurZoom;
    protected bool myDegenerateModeIsOn;
    protected int myXmin;
    protected int myYmin;
    protected int myXmax;
    protected int myYmax;
    protected int theButtonDownX;
    protected int theButtonDownY;
    // for erasing of rectangle
    protected int theRectDownX;
    protected int theRectDownY;
    protected bool IsRectVisible;

    private void Form2_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      switch (e.Button)
      {
        case MouseButtons.Left:
          myXmin = e.X; myYmin = e.Y;
          myXmax = e.X; myYmax = e.Y;
            switch (myCurrentMode)
            {
              case CurrentAction3d.CurAction3d_Multi:
                if (!myDegenerateModeIsOn)
                  myOCCTProxy.SetDegenerateModeOn();
                //start the rotation
                myOCCTProxy.StartRotation(e.X, e.Y);
                break;
              default:
                break;
            }
          //}
          break;
        case MouseButtons.Right:
          break;
        default:
          break;
      }
    }

    private void Form2_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      if (e.Shift)
        myCurrentPressedKey = CurrentPressedKey.CurPressedKey_Shift;
      else if (e.Control)
        myCurrentPressedKey = CurrentPressedKey.CurPressedKey_Ctrl;
    }

    private void Form2_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      myCurrentPressedKey = CurrentPressedKey.CurPressedKey_Nothing;
    }

    protected void MultiDragEvent(int x, int y, int theState)
    {
      if (theState == -1)
      {
        theButtonDownX = x;
        theButtonDownY = y;
      }
      else if (theState == 1)
        myOCCTProxy.ShiftSelect(Math.Min(theButtonDownX, x), Math.Min(theButtonDownY, y),
                Math.Max(theButtonDownX, x), Math.Max(theButtonDownY, y));
    }

    private void Form2_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      switch (e.Button)
      {
        case MouseButtons.Left:
          switch (myCurrentMode)
          {
            case CurrentAction3d.CurAction3d_Multi:
              if (!myDegenerateModeIsOn)
              {
                myOCCTProxy.SetDegenerateModeOff();
                myDegenerateModeIsOn = false;
              }
              else
              {
                myOCCTProxy.SetDegenerateModeOn();
                myDegenerateModeIsOn = true;
              }
              break;
            default:
              break;

          }
          break;
        case MouseButtons.Right:
          break;
        default:
          break;
      }
    }

    private void Form2_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (e.Button == MouseButtons.Left) //left button is pressed
      {
        if (myCurrentPressedKey == CurrentPressedKey.CurPressedKey_Ctrl)
        {
          myOCCTProxy.Zoom(myXmax, myYmax, e.X, e.Y);
          myXmax = e.X; myYmax = e.Y;
        }
        else
        {
          switch (myCurrentMode)
          {
            case CurrentAction3d.CurAction3d_Multi:
              myOCCTProxy.Rotation(e.X, e.Y);
              myOCCTProxy.RedrawView();
              break;
            default:
              break;
          }
        }
      } // e.Button == MouseButtons.Left
      else if (e.Button == MouseButtons.Middle)
      {
        switch (myCurrentMode)
        {
          case CurrentAction3d.CurAction3d_Multi:
            myOCCTProxy.Zoom(myXmax, myYmax, e.X, e.Y);
            myXmax = e.X; myYmax = e.Y;
            break;
        }
      }//e.Button=MouseButtons.Middle
      else if (e.Button == MouseButtons.Right) //right button is pressed
      {
        switch (myCurrentMode)
        {
          case CurrentAction3d.CurAction3d_Multi:
            myOCCTProxy.Pan(e.X - myXmax, myYmax - e.Y);
            myXmax = e.X; myYmax = e.Y;
            break;
        }
      }


    }

    public void ChangeColor(bool IsObjectColor)
    {
      int r, g, b;
      if (IsObjectColor)
      {
        r = myOCCTProxy.GetObjColR();
        g = myOCCTProxy.GetObjColG();
        b = myOCCTProxy.GetObjColB();
      }
      else
      {
        r = myOCCTProxy.GetBGColR();
        g = myOCCTProxy.GetBGColG();
        b = myOCCTProxy.GetBGColB();
      }
      System.Windows.Forms.ColorDialog ColDlg = new ColorDialog();
      ColDlg.Color = System.Drawing.Color.FromArgb(r, g, b);
      if (ColDlg.ShowDialog() == DialogResult.OK)
      {
        Color c = ColDlg.Color;
        r = c.R;
        g = c.G;
        b = c.B;
        if (IsObjectColor)
          myOCCTProxy.SetColor(r, g, b);
        else
          myOCCTProxy.SetBackgroundColor(r, g, b);
      }
      this.myOCCTProxy.UpdateCurrentViewer();

    }

    private void menuItem1_Click(object sender, System.EventArgs e)
    {
      this.myOCCTProxy.UpdateCurrentViewer();
      this.ChangeColor(false);
    }

    public OCCTProxy View
    {
      get
      {
        return this.myOCCTProxy;
      }
      set
      {
        this.myOCCTProxy = value;
      }
    }

    public void InitView()
    {
      this.myOCCTProxy.InitOCCTProxy();
    }

    public void SetIndex(int documentIndex, int viewIndex)
    {
      this.myDocumentIndex = documentIndex;
      this.myViewIndex = viewIndex;
      this.Text = System.String.Format("Document {0}:{1}", documentIndex, viewIndex);
    }

    private void Form2_Closed(object sender, System.EventArgs e)
    {
      IE_WinForms.MainForm parent = (IE_WinForms.MainForm)this.ParentForm;
    }

    public CurrentAction3d Mode
    {
      get
      {
        return this.myCurrentMode;
      }
      set
      {
        this.myCurrentMode = value;
      }
    }

    public float Zoom
    {
      set
      {
        this.myCurZoom = value;
      }
    }

    public bool DegenerateMode
    {
      get
      {
        return this.myDegenerateModeIsOn;
      }
      set
      {
        this.myDegenerateModeIsOn = value;
      }
    }

  }

}