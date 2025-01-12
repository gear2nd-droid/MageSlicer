using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Linq;
using System.IO;
using System.Threading;

namespace IE_WinForms
{
  /// <summary>
  /// Summary description for Form1.
  /// </summary>
  public class MainForm : System.Windows.Forms.Form
  {
    private System.Windows.Forms.MainMenu mainMenu1;
    private System.Windows.Forms.ImageList imageList1;
    private System.Windows.Forms.StatusBar myStatusBar;
    private System.ComponentModel.IContainer components;

    protected IE_WinForms.ModelFormat myModelFormat;
    private System.Windows.Forms.ToolBarButton ZoomAll;
    private System.Windows.Forms.ToolBarButton Front;
    private System.Windows.Forms.ToolBarButton Back;
    private System.Windows.Forms.ToolBarButton TOP;
    private System.Windows.Forms.ToolBarButton BOTTOM;
    private System.Windows.Forms.ToolBarButton RIGHT;
    private System.Windows.Forms.ToolBarButton LEFT;
    private System.Windows.Forms.ToolBarButton Axo;
    private System.Windows.Forms.ToolBarButton Reset;
    private System.Windows.Forms.ToolBar toolBarView;
    protected static int myNbOfChildren;
    private SplitContainer splitContainer1;
    private SplitContainer splitContainer2;
    private SplitContainer splitContainer3;
    private OcctView occtview;
    private DataProperty property;
    private DataTree datatree;
    private SplitContainer splitContainer4;
    private SliceControl slicecon;
    private TextBox consoleTextBox;
    private StringWriter sw;
    private Thread consoleThread;
    private MenuItem menuItem1;
    private MenuItem menuItemOpen;
    private MenuItem menuItemSave;
    private MenuItem menuItemClose;
    private bool consoleRunning;

    public MainForm()
    {
      //
      // Required for Windows Form Designer support
      //
      InitializeComponent();
      setConsoleTextBox();

      //
      IE_WinForms.MainForm.myNbOfChildren = 0;

      // Initialize Forms
      this.OnNewFile();
      this.VisibleDataProperty();
      this.VisibleDataTree(this.occtview);
      this.VisibleSliceControl(this.occtview, this.datatree);
      this.property.setDataTree(this.datatree);
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
      this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
      this.menuItem1 = new System.Windows.Forms.MenuItem();
      this.menuItemOpen = new System.Windows.Forms.MenuItem();
      this.menuItemSave = new System.Windows.Forms.MenuItem();
      this.menuItemClose = new System.Windows.Forms.MenuItem();
      this.imageList1 = new System.Windows.Forms.ImageList(this.components);
      this.myStatusBar = new System.Windows.Forms.StatusBar();
      this.toolBarView = new System.Windows.Forms.ToolBar();
      this.ZoomAll = new System.Windows.Forms.ToolBarButton();
      this.Front = new System.Windows.Forms.ToolBarButton();
      this.Back = new System.Windows.Forms.ToolBarButton();
      this.TOP = new System.Windows.Forms.ToolBarButton();
      this.BOTTOM = new System.Windows.Forms.ToolBarButton();
      this.LEFT = new System.Windows.Forms.ToolBarButton();
      this.RIGHT = new System.Windows.Forms.ToolBarButton();
      this.Axo = new System.Windows.Forms.ToolBarButton();
      this.Reset = new System.Windows.Forms.ToolBarButton();
      this.splitContainer1 = new System.Windows.Forms.SplitContainer();
      this.splitContainer3 = new System.Windows.Forms.SplitContainer();
      this.splitContainer4 = new System.Windows.Forms.SplitContainer();
      this.splitContainer2 = new System.Windows.Forms.SplitContainer();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
      this.splitContainer1.Panel1.SuspendLayout();
      this.splitContainer1.Panel2.SuspendLayout();
      this.splitContainer1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
      this.splitContainer3.Panel1.SuspendLayout();
      this.splitContainer3.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer4)).BeginInit();
      this.splitContainer4.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
      this.splitContainer2.SuspendLayout();
      this.SuspendLayout();
      // 
      // mainMenu1
      // 
      this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem1});
      // 
      // menuItem1
      // 
      this.menuItem1.Index = 0;
      this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemOpen,
            this.menuItemSave,
            this.menuItemClose});
      this.menuItem1.Text = "File(&F)";
      // 
      // menuItemOpen
      // 
      this.menuItemOpen.Index = 0;
      this.menuItemOpen.Text = "Open project(&O)";
      this.menuItemOpen.Click += new System.EventHandler(this.menuItemOpen_Click);
      // 
      // menuItemSave
      // 
      this.menuItemSave.Index = 1;
      this.menuItemSave.Text = "Save project(&S)";
      this.menuItemSave.Click += new System.EventHandler(this.menuItemSave_Click);
      // 
      // menuItemClose
      // 
      this.menuItemClose.Index = 2;
      this.menuItemClose.Text = "Close(&C)";
      this.menuItemClose.Click += new System.EventHandler(this.menuItemClose_Click);
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
      this.imageList1.Images.SetKeyName(16, "");
      this.imageList1.Images.SetKeyName(17, "");
      this.imageList1.Images.SetKeyName(18, "");
      this.imageList1.Images.SetKeyName(19, "");
      this.imageList1.Images.SetKeyName(20, "");
      this.imageList1.Images.SetKeyName(21, "");
      this.imageList1.Images.SetKeyName(22, "");
      this.imageList1.Images.SetKeyName(23, "");
      // 
      // myStatusBar
      // 
      this.myStatusBar.AccessibleRole = System.Windows.Forms.AccessibleRole.StatusBar;
      this.myStatusBar.Location = new System.Drawing.Point(0, 640);
      this.myStatusBar.Name = "myStatusBar";
      this.myStatusBar.Size = new System.Drawing.Size(1264, 20);
      this.myStatusBar.TabIndex = 3;
      // 
      // toolBarView
      // 
      this.toolBarView.AccessibleRole = System.Windows.Forms.AccessibleRole.ToolBar;
      this.toolBarView.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.ZoomAll,
            this.Front,
            this.Back,
            this.TOP,
            this.BOTTOM,
            this.LEFT,
            this.RIGHT,
            this.Axo,
            this.Reset});
      this.toolBarView.DropDownArrows = true;
      this.toolBarView.ImageList = this.imageList1;
      this.toolBarView.Location = new System.Drawing.Point(0, 0);
      this.toolBarView.Name = "toolBarView";
      this.toolBarView.ShowToolTips = true;
      this.toolBarView.Size = new System.Drawing.Size(1264, 28);
      this.toolBarView.TabIndex = 5;
      this.toolBarView.Visible = false;
      this.toolBarView.Wrappable = false;
      this.toolBarView.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.toolBarView_ButtonClick);
      this.toolBarView.MouseLeave += new System.EventHandler(this.toolBarView_MouseLeave);
      this.toolBarView.MouseHover += new System.EventHandler(this.toolBarView_MouseHover);
      // 
      // ZoomAll
      // 
      this.ZoomAll.ImageIndex = 8;
      this.ZoomAll.Name = "ZoomAll";
      this.ZoomAll.ToolTipText = "FitAll";
      // 
      // Front
      // 
      this.Front.ImageIndex = 13;
      this.Front.Name = "Front";
      this.Front.ToolTipText = "Front";
      // 
      // Back
      // 
      this.Back.ImageIndex = 14;
      this.Back.Name = "Back";
      this.Back.ToolTipText = "Back";
      // 
      // TOP
      // 
      this.TOP.ImageIndex = 15;
      this.TOP.Name = "TOP";
      this.TOP.ToolTipText = "Top";
      // 
      // BOTTOM
      // 
      this.BOTTOM.ImageIndex = 16;
      this.BOTTOM.Name = "BOTTOM";
      this.BOTTOM.ToolTipText = "Bottom";
      // 
      // LEFT
      // 
      this.LEFT.ImageIndex = 17;
      this.LEFT.Name = "LEFT";
      this.LEFT.ToolTipText = "Left";
      // 
      // RIGHT
      // 
      this.RIGHT.ImageIndex = 18;
      this.RIGHT.Name = "RIGHT";
      this.RIGHT.ToolTipText = "Right";
      // 
      // Axo
      // 
      this.Axo.ImageIndex = 19;
      this.Axo.Name = "Axo";
      this.Axo.ToolTipText = "Axo";
      // 
      // Reset
      // 
      this.Reset.ImageIndex = 21;
      this.Reset.Name = "Reset";
      this.Reset.ToolTipText = "Reset";
      // 
      // splitContainer1
      // 
      this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer1.Location = new System.Drawing.Point(0, 28);
      this.splitContainer1.Name = "splitContainer1";
      // 
      // splitContainer1.Panel1
      // 
      this.splitContainer1.Panel1.Controls.Add(this.splitContainer3);
      // 
      // splitContainer1.Panel2
      // 
      this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
      this.splitContainer1.Size = new System.Drawing.Size(1264, 612);
      this.splitContainer1.SplitterDistance = 906;
      this.splitContainer1.TabIndex = 13;
      // 
      // splitContainer3
      // 
      this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer3.Location = new System.Drawing.Point(0, 0);
      this.splitContainer3.Name = "splitContainer3";
      // 
      // splitContainer3.Panel1
      // 
      this.splitContainer3.Panel1.Controls.Add(this.splitContainer4);
      this.splitContainer3.Size = new System.Drawing.Size(906, 612);
      this.splitContainer3.SplitterDistance = 791;
      this.splitContainer3.TabIndex = 0;
      // 
      // splitContainer4
      // 
      this.splitContainer4.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer4.Location = new System.Drawing.Point(0, 0);
      this.splitContainer4.Name = "splitContainer4";
      this.splitContainer4.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.splitContainer4.Size = new System.Drawing.Size(791, 612);
      this.splitContainer4.SplitterDistance = 484;
      this.splitContainer4.TabIndex = 0;
      // 
      // splitContainer2
      // 
      this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer2.Location = new System.Drawing.Point(0, 0);
      this.splitContainer2.Name = "splitContainer2";
      this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.splitContainer2.Size = new System.Drawing.Size(354, 612);
      this.splitContainer2.SplitterDistance = 427;
      this.splitContainer2.TabIndex = 0;
      // 
      // MainForm
      // 
      this.AccessibleRole = System.Windows.Forms.AccessibleRole.Application;
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
      this.ClientSize = new System.Drawing.Size(1264, 660);
      this.Controls.Add(this.splitContainer1);
      this.Controls.Add(this.toolBarView);
      this.Controls.Add(this.myStatusBar);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.IsMdiContainer = true;
      this.Menu = this.mainMenu1;
      this.Name = "MainForm";
      this.Text = "MAGE Slicer";
      this.Activated += new System.EventHandler(this.Form1_Activated);
      this.splitContainer1.Panel1.ResumeLayout(false);
      this.splitContainer1.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
      this.splitContainer1.ResumeLayout(false);
      this.splitContainer3.Panel1.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
      this.splitContainer3.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer4)).EndInit();
      this.splitContainer4.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
      this.splitContainer2.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

    }
    #endregion

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Application.Run(new MainForm());
    }

    public void OnNewFile()
    {
      OcctView newForm = new OcctView();
      this.occtview = newForm;
      newForm.MdiParent = this;
      newForm.TopLevel = false;
      newForm.FormBorderStyle = FormBorderStyle.None;
      IE_WinForms.MainForm.myNbOfChildren = IE_WinForms.MainForm.myNbOfChildren + 1;
      newForm.SetIndex (IE_WinForms.MainForm.myNbOfChildren, 1);
      this.splitContainer4.Panel1.Controls.Add(newForm);
      newForm.Dock = DockStyle.Fill;
      newForm.Width = this.splitContainer4.Panel1.Width;
      newForm.Height = this.splitContainer4.Panel1.Height;
      newForm.Show();
      newForm.InitView();
      newForm.InitV3D();
      this.Cursor = System.Windows.Forms.Cursors.Default;
      this.toolBarView.Visible = true;
    }

    private void VisibleDataProperty()
    {
      DataProperty newForm = new DataProperty();
      this.property = newForm;
      newForm.MdiParent = this;
      newForm.TopLevel = false;
      newForm.FormBorderStyle = FormBorderStyle.None;
      IE_WinForms.MainForm.myNbOfChildren = IE_WinForms.MainForm.myNbOfChildren + 1;
      this.splitContainer2.Panel2.Controls.Add(newForm);
      newForm.Dock = DockStyle.Fill;
      newForm.Show();
    }

    private void VisibleDataTree(OcctView form)
    {
      DataTree newForm = new DataTree(form, this.property);
      this.datatree = newForm;
      newForm.MdiParent = this;
      newForm.TopLevel = false;
      newForm.FormBorderStyle = FormBorderStyle.None;
      IE_WinForms.MainForm.myNbOfChildren = IE_WinForms.MainForm.myNbOfChildren + 1;
      this.splitContainer2.Panel1.Controls.Add(newForm);
      newForm.Dock = DockStyle.Fill;
      newForm.Show();
    }

    private void VisibleSliceControl(OcctView form, DataTree tree)
    {
      SliceControl newForm = new SliceControl(form, tree);
      this.slicecon = newForm;
      newForm.MdiParent = this;
      newForm.TopLevel = false;
      newForm.FormBorderStyle = FormBorderStyle.None;
      IE_WinForms.MainForm.myNbOfChildren = IE_WinForms.MainForm.myNbOfChildren + 1;
      this.splitContainer3.Panel2.Controls.Add(newForm);
      newForm.Dock = DockStyle.Fill;
      newForm.Show();
    }

    public StatusBar StatusBar
    {
      get
      {
        return this.myStatusBar;
      }
    }

    private void toolBarView_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
    {
      IE_WinForms.OcctView curForm = this.occtview;
      if (curForm == null)
        return;
      switch (toolBarView.Buttons.IndexOf(e.Button))
      {
        case 0:
          curForm.View.ZoomAllView();
          break;
        case 1:
          curForm.View.FrontView();
          break;
        case 2:
          curForm.View.BackView();
          break;
        case 3:
          curForm.View.TopView();
          break;
        case 4:
          curForm.View.BottomView();
          break;
        case 5:
          curForm.View.LeftView();
          break;
        case 6:
          curForm.View.RightView();
          break;
        case 7:
          curForm.View.AxoView();
          break;
        case 8:
          curForm.View.Reset();
          break;
        default:
          break;
      }
    }

    private void Form1_Activated(object sender, System.EventArgs e)
    {
    }

    private void toolBarView_MouseHover(object sender, System.EventArgs e)
    {
      this.myStatusBar.Text = "View toolbar";
    }

    private void toolBarView_MouseLeave(object sender, System.EventArgs e)
    {
      this.myStatusBar.Text = "";
    }

    private void setConsoleTextBox()
    {
      this.consoleTextBox = new TextBox();
      this.splitContainer4.Panel2.Controls.Add(this.consoleTextBox);
      this.consoleTextBox.Dock = DockStyle.Fill;
      this.consoleTextBox.Multiline = true;
      this.consoleTextBox.Enabled = false;

      // console
      this.sw = new StringWriter();
      Console.SetOut(this.sw);
      this.consoleTextBox.AppendText(this.sw.ToString()); 

      this.consoleRunning = true;
      this.consoleThread = new Thread(new ThreadStart(ConsoleUpdate));
      this.consoleThread.Start();
    }

    private void ConsoleUpdate()
    {
      while(this.consoleRunning)
      {
        string output = this.sw.ToString();
        if(!string.IsNullOrEmpty(output))
        {
          this.Invoke((MethodInvoker)delegate
          {
            this.consoleTextBox.AppendText(output);
            this.sw.GetStringBuilder().Clear();
          }
          );
        }
        Thread.Sleep(100);
      }
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
      this.consoleRunning = false;
      this.consoleThread.Join();
      base.OnFormClosing(e);
    }

    private void menuItemClose_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void menuItemSave_Click(object sender, EventArgs e)
    {
      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
      ofd.FileName = "slice_project.xml";
      ofd.Filter = "XML file(*.xml)|*.xml";
      ofd.Title = "Save xml project file";
      ofd.RestoreDirectory = true;
      ofd.CheckFileExists = false;
      ofd.CheckPathExists = true;
      if (ofd.ShowDialog() == DialogResult.OK)
      {
        this.datatree.saveProject(ofd.FileName);
        Console.WriteLine("Save project file: " + ofd.FileName);
      }
    }

    private void menuItemOpen_Click(object sender, EventArgs e)
    {
      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
      ofd.FileName = "slice_project.xml";
      ofd.Filter = "XML file(*.xml)|*.xml";
      ofd.Title = "Open xml project file";
      ofd.RestoreDirectory = true;
      ofd.CheckFileExists = true;
      ofd.CheckPathExists = true;
      if (ofd.ShowDialog() == DialogResult.OK)
      {
        Console.WriteLine("Open project file: " + ofd.FileName);
        this.datatree.openProject(ofd.FileName);
      }
    }
  }

}