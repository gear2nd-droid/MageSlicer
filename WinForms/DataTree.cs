using OxyPlot;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;
using System.Xml;
using System.Collections.Generic;

namespace IE_WinForms
{

  public partial class DataTree : Form
  {
    private List<Geometry> geoms = new List<Geometry>();
    public List<TreeItem> items = new List<TreeItem>();
    private OcctView view;
    private DataProperty property;
    private SliceControl slice;
    public int SurfaceCnt = 1;
    public int BasePlaneGeomId;

    public DataTree(OcctView view, DataProperty property)
    {
      this.property = property;
      this.view = view;
      InitializeComponent();
      
      // read BasePlane.csv
      string basePlanePath = @"..\..\BasePlane.csv";
      int nx, ny;
      IntPtr ptr = Marshal.StringToHGlobalAnsi(basePlanePath);
      unsafe{this.view.myOCCTProxy.ImportCsv_NonTransform2((sbyte*)ptr, SurfaceCnt, &nx, &ny);}
      this.BasePlaneGeomId = this.SurfaceCnt;
      this.SurfaceCnt++;
      this.view.myOCCTProxy.UpdateView();
      this.view.myOCCTProxy.DeleteSurfaceByTransparent(this.BasePlaneGeomId);
      this.view.myOCCTProxy.SetTransparent(this.BasePlaneGeomId, false);
      this.view.myOCCTProxy.UpdateView();
    }

    public void SetSliceControlView(SliceControl slice)
    {
      this.slice = slice;
    }

    private void toolStripButtonAdd_Click(object sender, EventArgs e)
    {
      int val = this.treeView.Nodes.Count;
      if(val == 0)
      {
        AddBottomItem();
      }
      else
      {
        AddSandwichItem();
      }
    }

    public void AddBottomItem()
    {
      BottomItem item = new BottomItem();
      this.items.Add(item);
      this.treeView.Nodes.Add(item.SliceType);
    }

    public void AddBottomItem(int idx)
    {
      BottomItem buf = new BottomItem();
      this.items.Add(buf);
      this.treeView.Nodes.Insert(idx, buf.SliceType);
    }

    public void AddSandwichItem()
    {
      SandwichItem item = new SandwichItem();
      this.items.Add(item);
      this.treeView.Nodes.Add(item.SliceType);
    }

    public void AddSandwichItem(int idx)
    {
      SandwichItem buf = new SandwichItem();
      this.items.Add(buf);
      this.treeView.Nodes.Insert(idx, buf.SliceType);
    }

    public void AddAutoSliceItem()
    {
      AutoSliceItem item = new AutoSliceItem();
      this.items.Add(item);
      this.treeView.Nodes.Add(item.SliceType);
    }

    public void AddAutoSliceItem(int idx)
    {
      AutoSliceItem buf = new AutoSliceItem();
      this.items.Add(buf);
      this.treeView.Nodes.Insert(idx, buf.SliceType);
    }

    public void AddPeelerItem()
    {
      PeelerItem buf = new PeelerItem();
      this.items.Add(buf);
      this.treeView.Nodes.Add(buf.SliceType);
    }

    public void AddPeelerItem(int idx)
    {
      PeelerItem buf = new PeelerItem();
      this.items.Add(buf);
      this.treeView.Nodes.Insert(idx, buf.SliceType);
    }

    private void toolStripButtonDelete_Click(object sender, EventArgs e)
    {
      TreeNode node = (TreeNode)this.treeView.SelectedNode;
      if(node != null)
      {
        if(node.Parent != null)
        {
          TreeNode parent = (TreeNode)node.Parent;
          int idx = this.treeView.Nodes.IndexOf(parent);
          foreach(GeometryNode buf in parent.Nodes)
          {
            if(buf.IsExist)
            {
              this.DeleteGeometry(buf.Geometry);
            }
          }
          node.Parent.Remove();
          // item
          TreeItem bufItem = this.items[idx];
          this.items.Remove(bufItem);
        }
        else
        {
          int idx = this.treeView.Nodes.IndexOf(node);
          foreach(GeometryNode buf in node.Nodes)
          {
            if(buf.IsExist)
            {
              this.DeleteGeometry(buf.Geometry);
            }
          }
          node.Remove();
          // item
          TreeItem bufItem = this.items[idx];
          this.items.Remove(bufItem);
        }
      }
    }

    public void DeleteGeometry(Geometry geom)
    {
      geom.usingCnt--;
      if(geom.usingCnt == 0)
      {
        geom.GeomVisible = false;
        this.view.myOCCTProxy.SetTransparent(geom.Geomid, false);
        this.slice.deleteSurfaceObject(geom.Geomid);
        for(int i = this.geoms.Count - 1; i >= 0; i--)
        {
          if(this.geoms[i] == geom)
          {
            this.geoms.RemoveAt(i);
          }
        }
      }
    }

    public int DeleteItem(TreeItem item)
    {
      int ret = this.items.IndexOf(item);
      foreach(GeometryNode buf in item.SliceType.Nodes)
      {
        if(buf.IsExist)
        {
          this.DeleteGeometry(buf.Geometry);
        }
      }
      this.treeView.Nodes.Remove(item.SliceType);
      this.items.Remove(item);
      return ret;
    }

    private void toolStripButtonUp_Click(object sender, EventArgs e)
    {
      TreeNode node = (TreeNode)this.treeView.SelectedNode;
      if(node != null)
      {
        if(node.Parent != null)
        {
          TreeNode buf = (TreeNode)node.Parent;
          int idx = this.treeView.Nodes.IndexOf(buf);
          if(idx != 0)
          {
            buf.Remove();
            this.treeView.Nodes.Insert(idx - 1, buf);
            this.treeView.SelectedNode = buf;
            // item
            TreeItem bufItem = this.items[idx];
            this.items.Remove(bufItem);
            this.items.Insert(idx - 1, bufItem);
          }
        }
        else
        {
          TreeNode buf = node;
          int idx = this.treeView.Nodes.IndexOf(buf);
          if(idx != 0)
          {
            buf.Remove();
            this.treeView.Nodes.Insert(idx - 1, buf);
            this.treeView.SelectedNode = buf;
            // item
            TreeItem bufItem = this.items[idx];
            this.items.Remove(bufItem);
            this.items.Insert(idx - 1, bufItem);
          }
        }
      }
    }

    private void toolStripButtonDown_Click(object sender, EventArgs e)
    {
      TreeNode node = (TreeNode)this.treeView.SelectedNode;
      if(node != null)
      {
        if(node.Parent != null)
        {
          TreeNode buf = (TreeNode)node.Parent;
          int idx = this.treeView.Nodes.IndexOf(buf);
          if(idx != this.items.Count - 1)
          {
            buf.Remove();
            this.treeView.Nodes.Insert(idx + 1, buf);
            this.treeView.SelectedNode = buf;
            // item
            TreeItem bufItem = this.items[idx];
            this.items.Remove(bufItem);
            this.items.Insert(idx + 1, bufItem);
          }
        }
        else
        {
          TreeNode buf = node;
          int idx = this.treeView.Nodes.IndexOf(buf);
          if(idx != this.items.Count - 1)
          {
            buf.Remove();
            this.treeView.Nodes.Insert(idx + 1, buf);
            this.treeView.SelectedNode = buf;
            // item
            TreeItem bufItem = this.items[idx];
            this.items.Remove(bufItem);
            this.items.Insert(idx + 1, bufItem);
          }
        }
      }
    }

    private void toolStripButtonOpen_Click(object sender, EventArgs e)
    {
      TreeNode bufNode = (TreeNode)this.treeView.SelectedNode;
      if(bufNode is GeometryNode)
      {
        GeometryNode selectNode = (GeometryNode)bufNode;
        GeometryNode copyNode = selectNode.DeepCopy();
        int idx = this.treeView.Nodes.IndexOf(selectNode.Parent);
        GeometryImportDialog dialog = null;
        if(this.items[idx] is SandwichItem)
        {
          SandwichItem sandItem = (SandwichItem)this.items[idx];
          dialog = new GeometryImportDialog(copyNode, this.geoms, sandItem.Pitch, 0);
        }
        else if(this.items[idx] is BottomItem)
        {
          BottomItem bottomItem = (BottomItem)this.items[idx];
          dialog = new GeometryImportDialog(copyNode, this.geoms, bottomItem.Thickness, bottomItem.Direction);
        }
        else if(this.items[idx] is AutoSliceItem)
        {
          AutoSliceItem autoItem = (AutoSliceItem)this.items[idx];
          dialog = new GeometryImportDialog(copyNode, this.geoms, autoItem.Thickness, 0);
        }
        else if(this.items[idx] is PeelerItem)
        {
          PeelerItem peelerItem = (PeelerItem)this.items[idx];
          dialog = new GeometryImportDialog(copyNode, this.geoms, peelerItem.Thickness, peelerItem.Direction);
        }
        if(dialog.ShowDialog() == DialogResult.OK)
        {
          if(copyNode.Geometry.Equals(selectNode.Geometry))
          {
            //selectNode.Geometry.usingCnt++;
            selectNode = copyNode;
          }
          else
          {
            if(selectNode.IsExist)
            {
              selectNode.Geometry.usingCnt--;
              if(selectNode.Geometry.usingCnt == 0)
              {
                selectNode.Geometry.GeomVisible = false;
                this.view.myOCCTProxy.SetTransparent(selectNode.Geometry.Geomid, false);
                this.slice.deleteSurfaceObject(selectNode.Geometry.Geomid);
                for(int i = this.geoms.Count - 1; i >= 0; i--)
                {
                  if(this.geoms[i] == selectNode.Geometry)
                  {
                    this.geoms.RemoveAt(i);
                  }
                }
              }
            }
            // insert
            if(dialog.IsNewFile)
            {
              string extension = Path.GetExtension(copyNode.Geometry.Path);
              extension = extension.ToLower();
              if(extension == ".csv")
              {
                // format check
                int dimension;
                using(StreamReader reader = new StreamReader(copyNode.Geometry.Path))
                {
                  string line = reader.ReadLine();
                  dimension = int.Parse(line);
                }
                // read csv
                if(dimension == 2)
                {
                  IntPtr ptr = Marshal.StringToHGlobalAnsi(copyNode.Geometry.Path);
                  copyNode.IsExist = true;
                  if(copyNode.Geometry.IsTransform)
                  {
                    int nx, ny;
                    unsafe{this.view.myOCCTProxy.ImportCsv_Transform2((sbyte*)ptr, this.SurfaceCnt, &nx, &ny,
                      copyNode.Geometry.MoveX, copyNode.Geometry.MoveY, copyNode.Geometry.MoveZ, 
                      copyNode.Geometry.PointX, copyNode.Geometry.PointY, copyNode.Geometry.PointZ, 
                      copyNode.Geometry.DirectionX, copyNode.Geometry.DirectionY, copyNode.Geometry.DirectionZ, 
                      copyNode.Geometry.Angle);}
                    copyNode.Geometry.Geomid = this.SurfaceCnt;
                    copyNode.Geometry.Nx = nx;
                    copyNode.Geometry.Ny = ny;
                    this.SurfaceCnt++;
                  }
                  else
                  {
                    int nx, ny;
                    unsafe{this.view.myOCCTProxy.ImportCsv_NonTransform2((sbyte*)ptr, this.SurfaceCnt, &nx, &ny);}
                    copyNode.Geometry.Geomid = this.SurfaceCnt;
                    copyNode.Geometry.Nx = nx;
                    copyNode.Geometry.Ny = ny;
                    this.SurfaceCnt++;
                  }
                  copyNode.Text = copyNode.Geometry.Geomid + ":" + Path.GetFileName(copyNode.Geometry.Path);
                }
              }
              else
              {
                // read step
                IntPtr ptr = Marshal.StringToHGlobalAnsi(copyNode.Geometry.Path);
                copyNode.IsExist = true;
                if(copyNode.Geometry.IsTransform)
                {
                  unsafe{this.view.myOCCTProxy.ImportStep_Transform((sbyte*)ptr, this.SurfaceCnt,
                    copyNode.Geometry.MoveX, copyNode.Geometry.MoveY, copyNode.Geometry.MoveZ, 
                    copyNode.Geometry.PointX, copyNode.Geometry.PointY, copyNode.Geometry.PointZ, 
                    copyNode.Geometry.DirectionX, copyNode.Geometry.DirectionY, copyNode.Geometry.DirectionZ, 
                    copyNode.Geometry.Angle);}
                  copyNode.Geometry.Geomid = this.SurfaceCnt;
                  this.SurfaceCnt++;
                }
                else
                {
                  unsafe{this.view.myOCCTProxy.ImportStep_NonTransform((sbyte*)ptr, this.SurfaceCnt);}
                  copyNode.Geometry.Geomid = this.SurfaceCnt;
                  this.SurfaceCnt++;
                }
                copyNode.Text = copyNode.Geometry.Geomid + ":" + Path.GetFileName(copyNode.Geometry.Path);
              }
              this.view.myOCCTProxy.SetColor(copyNode.Geometry.Geomid, copyNode.ForeColor.R, copyNode.ForeColor.G, copyNode.ForeColor.B);
              Console.WriteLine("Open file: " + Path.GetFileName(copyNode.Geometry.Path) + " (geometry id: " + copyNode.Geometry.Geomid + ")");
              copyNode.Geometry.usingCnt = 1;
              selectNode.Geometry = copyNode.Geometry.DeepCopy();
              selectNode.IsExist = copyNode.IsExist;
              selectNode.Text = copyNode.Text;
              this.geoms.Add(selectNode.Geometry);
            }
            else
            {
              for(int i = 0; i < this.geoms.Count; i++)
              {
                if(copyNode.Geometry.Equals(this.geoms[i]))
                {
                  selectNode.Geometry = this.geoms[i];
                  selectNode.Geometry.usingCnt++;
                  selectNode.IsExist = true;
                  selectNode.Text = selectNode.Geometry.Geomid + ":" + Path.GetFileName(selectNode.Geometry.Path);
                  break;
                }
              }
            }
            if(this.items[idx] is SandwichItem)
            {
              SandwichItem sandItem = (SandwichItem)this.items[idx];
              sandItem.Pitch = dialog.Pitch;
            }
            else if(this.items[idx] is BottomItem)
            {
              BottomItem bottomItem = (BottomItem)this.items[idx];
              bottomItem.Thickness = dialog.Pitch;
              bottomItem.Direction = dialog.Direction;
            }
            else if(this.items[idx] is AutoSliceItem)
            {
              AutoSliceItem autoItem = (AutoSliceItem)this.items[idx];
              autoItem.Thickness = dialog.Pitch;
            }
            else if(this.items[idx] is PeelerItem)
            {
              PeelerItem peelerItem = (PeelerItem)this.items[idx];
              peelerItem.Thickness = dialog.Pitch;
              peelerItem.Direction = dialog.Direction;
            }
          }
        }
      }
      else if(bufNode is TreeNode)
      {
        int idx = 0;
        for(int i = 0; i < this.items.Count; i++)
        {
          if(bufNode == this.items[i].SliceType)
          {
            idx = i;
          }
        }
        TreeSelectDialog dialog = new TreeSelectDialog(this.items[idx], this.view, this.slice, this);
        dialog.ShowDialog();
      }
    }

    private void toolStripButtonColor_Click(object sender, EventArgs e)
    {
      TreeNode bufNode = (TreeNode)this.treeView.SelectedNode;
      if(bufNode is GeometryNode)
      {
        GeometryNode node = (GeometryNode)bufNode;
        ColorDialog cd = new ColorDialog();
        cd.Color = node.Geometry.Color;
        cd.AllowFullOpen = true;
        cd.SolidColorOnly = false;
        cd.CustomColors = new int[] {
            0x33, 0x66, 0x99, 0xCC, 0x3300, 0x3333,
            0x3366, 0x3399, 0x33CC, 0x6600, 0x6633,
            0x6666, 0x6699, 0x66CC, 0x9900, 0x9933};
        if (cd.ShowDialog() == DialogResult.OK)
        {
          node.Geometry.Color = cd.Color;
          for(int i = 0; i < this.treeView.Nodes.Count; i++)
          {
            TreeItem item = this.items[i];
            if(item is SandwichItem)
            {
              GeometryNode volume = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
              volume.ForeColor = volume.Geometry.Color;
              GeometryNode lower = (GeometryNode)this.treeView.Nodes[i].Nodes[1];
              lower.ForeColor = lower.Geometry.Color;
              GeometryNode upper = (GeometryNode)this.treeView.Nodes[i].Nodes[2];
              upper.ForeColor = upper.Geometry.Color;
            }
            else if(item is BottomItem)
            {
              GeometryNode volume = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
              volume.ForeColor = volume.Geometry.Color;
              GeometryNode surface = (GeometryNode)this.treeView.Nodes[i].Nodes[1];
              surface.ForeColor = surface.Geometry.Color;
            }
            else if(item is AutoSliceItem)
            {
              GeometryNode volume = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
              volume.ForeColor = volume.Geometry.Color;
            }
            else if(item is PeelerItem)
            {
              GeometryNode volume = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
              volume.ForeColor = volume.Geometry.Color;
            }
          }
          if(node.IsExist)
          {
            this.view.myOCCTProxy.SetColor(node.Geometry.Geomid, cd.Color.R, cd.Color.G, cd.Color.B);
          }
        }
      }
    }

    private void toolStripButtonVisible_Click(object sender, EventArgs e)
    {
      TreeNode bufNode = (TreeNode)this.treeView.SelectedNode;
      if(bufNode is GeometryNode)
      {
        GeometryNode node = (GeometryNode)bufNode;
        if(node.IsExist)
        {
          if (node.Geometry.GeomVisible)
          {
            this.toolStripButtonVisible.Image = Properties.Resources.cancel_FILL0_wght400_GRAD0_opsz24;
            node.Geometry.GeomVisible = false;
            this.view.myOCCTProxy.SetTransparent(node.Geometry.Geomid, false);
          }
          else
          {
            this.toolStripButtonVisible.Image = Properties.Resources.check_circle_FILL0_wght400_GRAD0_opsz24;
            node.Geometry.GeomVisible = true;
            this.view.myOCCTProxy.SetTransparent(node.Geometry.Geomid, true);
          }
        }
      }
    }

    private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
    {
      TreeNode bufNode = (TreeNode)this.treeView.SelectedNode;
      if(bufNode is GeometryNode)
      {
        GeometryNode node = (GeometryNode)bufNode;
        if (node.Geometry.GeomVisible)
        {
          this.toolStripButtonVisible.Image = Properties.Resources.check_circle_FILL0_wght400_GRAD0_opsz24;
        }
        else
        {
          this.toolStripButtonVisible.Image = Properties.Resources.cancel_FILL0_wght400_GRAD0_opsz24;
        }
      }
      // property
      this.property.viewDataTree(bufNode);
    }

    public System.Windows.Forms.TreeView getTreeView()
    {
      return this.treeView;
    }

    public void saveProject(string filename)
    {
      XmlDocument doc = new XmlDocument();
      XmlDeclaration xmlDeclaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
      doc.AppendChild(xmlDeclaration);
      // structure
      XmlElement root = doc.CreateElement("Project");
      doc.AppendChild(root);
      XmlElement targets = doc.CreateElement("Targets");
      root.AppendChild(targets);
      XmlElement surfaces = doc.CreateElement("Surfaces");
      root.AppendChild(surfaces);
      XmlElement volumes = doc.CreateElement("Volumes");
      root.AppendChild(volumes);
      int cnt = this.treeView.Nodes.Count;
      // target
      Dictionary<Geometry, int> dicTarget = new Dictionary<Geometry, int>();
      Dictionary<Geometry, int> dicSurface = new Dictionary<Geometry, int>();
      for(int i = 0; i < cnt; i++)
      {
        GeometryNode nodeTarget = null;
        GeometryNode nodeSurface1 = null;
        GeometryNode nodeSurface2 = null;
        if(this.items[i] is SandwichItem)
        {
          nodeTarget = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
          nodeSurface1 = (GeometryNode)this.treeView.Nodes[i].Nodes[1];
          nodeSurface2 = (GeometryNode)this.treeView.Nodes[i].Nodes[2];
        }
        else if(this.items[i] is BottomItem)
        {
          nodeTarget = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
          nodeSurface1 = (GeometryNode)this.treeView.Nodes[i].Nodes[1];
        }
        else if(this.items[i] is AutoSliceItem)
        {
          nodeTarget = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
        }
        else if(this.items[i] is PeelerItem)
        {
          nodeTarget = (GeometryNode)this.treeView.Nodes[i].Nodes[0];
        }
        if(nodeTarget != null)
        {
          if(!dicTarget.ContainsKey(nodeTarget.Geometry))
          {
            dicTarget.Add(nodeTarget.Geometry, dicTarget.Count);
            XmlElement target = doc.CreateElement("Target");
            targets.AppendChild(target);
            target.SetAttribute("Path", nodeTarget.Geometry.Path);
            target.SetAttribute("RotAngle", nodeTarget.Geometry.Angle.ToString());
            target.SetAttribute("RotCenterX", nodeTarget.Geometry.PointX.ToString());
            target.SetAttribute("RotCenterY", nodeTarget.Geometry.PointY.ToString());
            target.SetAttribute("RotCenterZ", nodeTarget.Geometry.PointZ.ToString());
            target.SetAttribute("RotDirectionX", nodeTarget.Geometry.DirectionX.ToString());
            target.SetAttribute("RotDirectionY", nodeTarget.Geometry.DirectionY.ToString());
            target.SetAttribute("RotDirectionZ", nodeTarget.Geometry.DirectionZ.ToString());
            target.SetAttribute("TransX", nodeTarget.Geometry.MoveX.ToString());
            target.SetAttribute("TransY", nodeTarget.Geometry.MoveY.ToString());
            target.SetAttribute("TransZ", nodeTarget.Geometry.MoveZ.ToString());
          }
        }
        if(nodeSurface1 != null)
        {
          if(!dicSurface.ContainsKey(nodeSurface1.Geometry))
          {
            dicSurface.Add(nodeSurface1.Geometry, dicSurface.Count);
            XmlElement surface = doc.CreateElement("Surface");
            surfaces.AppendChild(surface);
            surface.SetAttribute("Path", nodeSurface1.Geometry.Path);
            surface.SetAttribute("RotAngle", nodeSurface1.Geometry.Angle.ToString());
            surface.SetAttribute("RotCenterX", nodeSurface1.Geometry.PointX.ToString());
            surface.SetAttribute("RotCenterY", nodeSurface1.Geometry.PointY.ToString());
            surface.SetAttribute("RotCenterZ", nodeSurface1.Geometry.PointZ.ToString());
            surface.SetAttribute("RotDirectionX", nodeSurface1.Geometry.DirectionX.ToString());
            surface.SetAttribute("RotDirectionY", nodeSurface1.Geometry.DirectionY.ToString());
            surface.SetAttribute("RotDirectionZ", nodeSurface1.Geometry.DirectionZ.ToString());
            surface.SetAttribute("TransX", nodeSurface1.Geometry.MoveX.ToString());
            surface.SetAttribute("TransY", nodeSurface1.Geometry.MoveY.ToString());
            surface.SetAttribute("TransZ", nodeSurface1.Geometry.MoveZ.ToString());
          }
        }
        if(nodeSurface2 != null)
        {
          if(!dicSurface.ContainsKey(nodeSurface2.Geometry))
          {
            dicSurface.Add(nodeSurface2.Geometry, dicSurface.Count);
            XmlElement surface = doc.CreateElement("Surface");
            surfaces.AppendChild(surface);
            surface.SetAttribute("Path", nodeSurface2.Geometry.Path);
            surface.SetAttribute("RotAngle", nodeSurface2.Geometry.Angle.ToString());
            surface.SetAttribute("RotCenterX", nodeSurface2.Geometry.PointX.ToString());
            surface.SetAttribute("RotCenterY", nodeSurface2.Geometry.PointY.ToString());
            surface.SetAttribute("RotCenterZ", nodeSurface2.Geometry.PointZ.ToString());
            surface.SetAttribute("RotDirectionX", nodeSurface2.Geometry.DirectionX.ToString());
            surface.SetAttribute("RotDirectionY", nodeSurface2.Geometry.DirectionY.ToString());
            surface.SetAttribute("RotDirectionZ", nodeSurface2.Geometry.DirectionZ.ToString());
            surface.SetAttribute("TransX", nodeSurface2.Geometry.MoveX.ToString());
            surface.SetAttribute("TransY", nodeSurface2.Geometry.MoveY.ToString());
            surface.SetAttribute("TransZ", nodeSurface2.Geometry.MoveZ.ToString());
          }
        }
        XmlElement volume = doc.CreateElement("Volume");
        volumes.AppendChild(volume);
        TreeItem bufItem = this.items[i];
        if(bufItem is SandwichItem)
        {
          SandwichItem item = (SandwichItem)bufItem;
          volume.SetAttribute("Type", "Sandwich");
          volume.SetAttribute("TargetId", dicTarget[item.Volume.Geometry].ToString());
          volume.SetAttribute("LowerId", dicSurface[item.Lower.Geometry].ToString());
          volume.SetAttribute("UpperId", dicSurface[item.Upper.Geometry].ToString());
          volume.SetAttribute("Pitch", item.Pitch.ToString());
        }
        else if(bufItem is BottomItem)
        {
          BottomItem item = (BottomItem)bufItem;
          volume.SetAttribute("Type", "Bottom");
          volume.SetAttribute("TargetId", dicTarget[item.Volume.Geometry].ToString());
          volume.SetAttribute("SurfaceId", dicSurface[item.Surface.Geometry].ToString());
          volume.SetAttribute("Thickness", item.Thickness.ToString());
          volume.SetAttribute("Direction", item.Direction.ToString());
        }
        else if(bufItem is AutoSliceItem)
        {
          AutoSliceItem item = (AutoSliceItem)bufItem;
          volume.SetAttribute("Type", "AutoSlice");
          volume.SetAttribute("TargetId", dicTarget[item.Volume.Geometry].ToString());
          volume.SetAttribute("Thickness", item.Thickness.ToString());
        }
        else if(bufItem is PeelerItem)
        {
          PeelerItem item = (PeelerItem)bufItem;
          volume.SetAttribute("Type", "Peeler");
          volume.SetAttribute("TargetId", dicTarget[item.Volume.Geometry].ToString());
          volume.SetAttribute("Thickness", item.Thickness.ToString());
          volume.SetAttribute("Direction", item.Direction.ToString());
        }
      }
      // save
      doc.Save(filename);
    }

    public void openProject(string filename)
    {
      // param
      List<Geometry> bodies = new List<Geometry>();
      List<Geometry> layers = new List<Geometry>();
      // xml
      XmlDocument doc = new XmlDocument();
      doc.Load(filename);
      XmlElement root = doc.DocumentElement;
      XmlElement targets = root["Targets"];
      foreach(XmlElement target in targets)
      {
        Geometry buf = new Geometry();
        buf.Path = target.GetAttribute("Path");
        buf.Angle = double.Parse(target.GetAttribute("RotAngle"));
        buf.PointX = double.Parse(target.GetAttribute("RotCenterX"));
        buf.PointY = double.Parse(target.GetAttribute("RotCenterY"));
        buf.PointZ = double.Parse(target.GetAttribute("RotCenterZ"));
        buf.DirectionX = double.Parse(target.GetAttribute("RotDirectionX"));
        buf.DirectionY = double.Parse(target.GetAttribute("RotDirectionY"));
        buf.DirectionZ = double.Parse(target.GetAttribute("RotDirectionZ"));
        buf.MoveX = double.Parse(target.GetAttribute("TransX"));
        buf.MoveY = double.Parse(target.GetAttribute("TransY"));
        buf.MoveZ = double.Parse(target.GetAttribute("TransZ"));
        buf.IsTransform = false;
        buf.GeomVisible = true;
        if(buf.PointX != 0.0) buf.IsTransform = true;
        if(buf.PointY != 0.0) buf.IsTransform = true;
        if(buf.PointZ != 0.0) buf.IsTransform = true;
        if(buf.DirectionX != 0.0) buf.IsTransform = true;
        if(buf.DirectionY != 0.0) buf.IsTransform = true;
        if(buf.DirectionZ != 1.0) buf.IsTransform = true;
        if(buf.MoveX != 0.0) buf.IsTransform = true;
        if(buf.MoveY != 0.0) buf.IsTransform = true;
        if(buf.MoveZ != 0.0) buf.IsTransform = true;
        bodies.Add(buf);
        // read
        IntPtr ptr = Marshal.StringToHGlobalAnsi(buf.Path);
        if(buf.IsTransform)
        {
          unsafe{this.view.myOCCTProxy.ImportStep_Transform((sbyte*)ptr, this.SurfaceCnt,
            buf.MoveX, buf.MoveY, buf.MoveZ, 
            buf.PointX, buf.PointY, buf.PointZ, 
            buf.DirectionX, buf.DirectionY, buf.DirectionZ, 
            buf.Angle);}
          buf.Geomid = this.SurfaceCnt;
          this.SurfaceCnt++;
        }
        else
        {
          unsafe{this.view.myOCCTProxy.ImportStep_NonTransform((sbyte*)ptr, this.SurfaceCnt);}
          buf.Geomid = this.SurfaceCnt;
          this.SurfaceCnt++;
        }
        this.geoms.Add(buf);
      }
      XmlElement surfaces = root["Surfaces"];
      foreach(XmlElement surface in surfaces)
      {
        Geometry buf = new Geometry();
        buf.Path = surface.GetAttribute("Path");
        buf.Angle = double.Parse(surface.GetAttribute("RotAngle"));
        buf.PointX = double.Parse(surface.GetAttribute("RotCenterX"));
        buf.PointY = double.Parse(surface.GetAttribute("RotCenterY"));
        buf.PointZ = double.Parse(surface.GetAttribute("RotCenterZ"));
        buf.DirectionX = double.Parse(surface.GetAttribute("RotDirectionX"));
        buf.DirectionY = double.Parse(surface.GetAttribute("RotDirectionY"));
        buf.DirectionZ = double.Parse(surface.GetAttribute("RotDirectionZ"));
        buf.MoveX = double.Parse(surface.GetAttribute("TransX"));
        buf.MoveY = double.Parse(surface.GetAttribute("TransY"));
        buf.MoveZ = double.Parse(surface.GetAttribute("TransZ"));
        buf.IsTransform = false;
        buf.GeomVisible = true;
        if(buf.PointX != 0.0) buf.IsTransform = true;
        if(buf.PointY != 0.0) buf.IsTransform = true;
        if(buf.PointZ != 0.0) buf.IsTransform = true;
        if(buf.DirectionX != 0.0) buf.IsTransform = true;
        if(buf.DirectionY != 0.0) buf.IsTransform = true;
        if(buf.DirectionZ != 1.0) buf.IsTransform = true;
        if(buf.MoveX != 0.0) buf.IsTransform = true;
        if(buf.MoveY != 0.0) buf.IsTransform = true;
        if(buf.MoveZ != 0.0) buf.IsTransform = true;
        layers.Add(buf);
        // read
        IntPtr ptr = Marshal.StringToHGlobalAnsi(buf.Path);
        if(buf.IsTransform)
        {
          int nx, ny;
          unsafe{this.view.myOCCTProxy.ImportCsv_Transform2((sbyte*)ptr, this.SurfaceCnt, &nx, &ny,
            buf.MoveX, buf.MoveY, buf.MoveZ, 
            buf.PointX, buf.PointY, buf.PointZ, 
            buf.DirectionX, buf.DirectionY, buf.DirectionZ, 
            buf.Angle);}
          buf.Geomid = this.SurfaceCnt;
          buf.Nx = nx;
          buf.Ny = ny;
          this.SurfaceCnt++;
        }
        else
        {
          int nx, ny;
          unsafe{this.view.myOCCTProxy.ImportCsv_NonTransform2((sbyte*)ptr, this.SurfaceCnt, &nx, &ny);}
          buf.Geomid = this.SurfaceCnt;
          buf.Nx = nx;
          buf.Ny = ny;
          this.SurfaceCnt++;
        }
        this.geoms.Add(buf);
      }
      // volume
      XmlElement volumes = root["Volumes"];
      foreach(XmlElement volume in volumes)
      {
        if(volume.GetAttribute("Type") == "Sandwich")
        {
          SandwichItem item = new SandwichItem();
          // tree
          this.treeView.Nodes.Add(item.SliceType);
          // item
          this.items.Add(item);
          item.Volume.Geometry = bodies[int.Parse(volume.GetAttribute("TargetId"))];
          item.Volume.Geometry.usingCnt++;
          item.Volume.Text = item.Volume.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Volume.Geometry.Path);
          item.Volume.IsExist = true;
          item.Lower.Geometry = layers[int.Parse(volume.GetAttribute("LowerId"))];
          item.Lower.Geometry.usingCnt++;
          item.Lower.Text = item.Lower.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Lower.Geometry.Path);
          item.Lower.IsExist = true;
          item.Upper.Geometry = layers[int.Parse(volume.GetAttribute("UpperId"))];
          item.Upper.Geometry.usingCnt++;
          item.Upper.Text = item.Upper.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Upper.Geometry.Path);
          item.Upper.IsExist = true;
          item.Pitch = double.Parse(volume.GetAttribute("Pitch"));
        }
        else if(volume.GetAttribute("Type") == "Bottom")
        {
          BottomItem item = new BottomItem();
          // tree
          this.treeView.Nodes.Add(item.SliceType);
          // item
          this.items.Add(item);
          item.Volume.Geometry = bodies[int.Parse(volume.GetAttribute("TargetId"))];
          item.Volume.Geometry.usingCnt++;
          item.Volume.Text = item.Volume.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Volume.Geometry.Path);
          item.Volume.IsExist = true;
          item.Surface.Geometry = layers[int.Parse(volume.GetAttribute("SurfaceId"))];
          item.Surface.Geometry.usingCnt++;
          item.Surface.Text = item.Surface.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Surface.Geometry.Path);
          item.Surface.IsExist = true;
          item.Thickness = double.Parse(volume.GetAttribute("Thickness"));
          item.Direction = int.Parse(volume.GetAttribute("Direction"));
        }
        else if(volume.GetAttribute("Type") == "AutoSlice")
        {
          AutoSliceItem item = new AutoSliceItem();
          // tree
          this.treeView.Nodes.Add(item.SliceType);
          // item
          this.items.Add(item);
          item.Volume.Geometry = bodies[int.Parse(volume.GetAttribute("TargetId"))];
          item.Volume.Geometry.usingCnt++;
          item.Volume.Text = item.Volume.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Volume.Geometry.Path);
          item.Volume.IsExist = true;
          item.Thickness = double.Parse(volume.GetAttribute("Thickness"));
        }
        else if(volume.GetAttribute("Type") == "Peeler")
        {
          PeelerItem item = new PeelerItem();
          // tree
          this.treeView.Nodes.Add(item.SliceType);
          // item
          this.items.Add(item);
          item.Volume.Geometry = bodies[int.Parse(volume.GetAttribute("TargetId"))];
          item.Volume.Geometry.usingCnt++;
          item.Volume.Text = item.Volume.Geometry.Geomid + ":" + 
            Path.GetFileName(item.Volume.Geometry.Path);
          item.Volume.IsExist = true;
          item.Thickness = double.Parse(volume.GetAttribute("Thickness"));
          item.Direction = int.Parse(volume.GetAttribute("Direction"));
        }
      }
    }
  }
}
