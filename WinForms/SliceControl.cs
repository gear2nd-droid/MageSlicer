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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;
using OxyPlot; // need NuGet package OxyPlot.Core
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using System.Drawing.Printing; // need NuGe packaget OxyPlot.WindowsForms

public struct LayerIndex 
{
  public int surfaceId;
  public int wallId;
  public int infillId;
  public int supportId;
}

public enum SliceState
{
  Setting,
  PreSlice,
  Slice,
}

namespace IE_WinForms
{

  public partial class SliceControl : Form
  {
    bool IsPreSliceDone;
    private OcctView view;
    private DataTree tree;
    int layerCount;
    int layerTarget = -1;
    private SandwichSliceDialog slice;
    int[] volumeLayerCnt;
    List<int>[] surfaceIdx;
    List<int>[] wallIdx;
    List<int>[] infillIdx;
    List<int>[] supportIdx;
    SliceState state;

    static int MAX_LAYER_CNT = 10000;
    static int IDX_PATERN = 10;
        

    public SliceControl(OcctView view, DataTree tree)
    {
      this.IsPreSliceDone = false;
      this.view = view;
      this.tree = tree;
      InitializeComponent();
      this.tree.SetSliceControlView(this);
      this.state = SliceState.Setting;
    }

    private void buttonSurfaceColor_Click(object sender, EventArgs e)
    {
      ColorDialog cd = new ColorDialog();
      cd.Color = this.buttonSurface.ForeColor;
      cd.AllowFullOpen = true;
      cd.SolidColorOnly = false;
      cd.CustomColors = new int[] {
          0x33, 0x66, 0x99, 0xCC, 0x3300, 0x3333,
          0x3366, 0x3399, 0x33CC, 0x6600, 0x6633,
          0x6666, 0x6699, 0x66CC, 0x9900, 0x9933};
      if (cd.ShowDialog() == DialogResult.OK)
      {
        this.buttonSurface.ForeColor = cd.Color;
        //if(this.IsPreSliceDone)
        //{
        //  this.view.myOCCTProxy.SetSliceColor(
        //    this.buttonSurface.ForeColor.R, this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
        //}
      }
    }

    private void buttonCurveColor_Click(object sender, EventArgs e)
    {
      ColorDialog cd = new ColorDialog();
      cd.Color = this.buttonCurve.ForeColor;
      cd.AllowFullOpen = true;
      cd.SolidColorOnly = false;
      cd.CustomColors = new int[] {
          0x33, 0x66, 0x99, 0xCC, 0x3300, 0x3333,
          0x3366, 0x3399, 0x33CC, 0x6600, 0x6633,
          0x6666, 0x6699, 0x66CC, 0x9900, 0x9933};
      if (cd.ShowDialog() == DialogResult.OK)
      {
        this.buttonCurve.ForeColor = cd.Color;
        //if(this.IsPreSliceDone)
        //{
        //  this.view.myOCCTProxy.SetSliceColor(
        //    this.buttonSurface.ForeColor.R, this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
        //}
      }
    }

    public void deleteOldLayer()
    {
      for(int vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
      {
        for(int idx = 0; idx < this.volumeLayerCnt[vol]; idx++)
        {
          this.view.myOCCTProxy.DeleteSurfaceByTransparent(this.surfaceIdx[vol][idx]);
          this.view.myOCCTProxy.DeleteCurveByTransparent(this.wallIdx[vol][idx]);
        }
      }
    }
    public void deleteSurfaceObject(int id)
    {
      this.view.myOCCTProxy.DeleteSurfaceByTransparent(id);
    }
    public void deleteCurveObject(int id)
    {
      this.view.myOCCTProxy.DeleteCurveByTransparent(id);
    }

    private void buttonPreslice_Click(object sender, EventArgs e)
    {
      if(this.tree.getTreeView().Nodes.Count == 0)
      {
        Console.WriteLine("Please complete setting.");
        return;
      }
      for(int vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
      {
        TreeItem bufItem = this.tree.items[vol];
        if(bufItem is SandwichItem)
        {
          GeometryNode volumeNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[0];
          GeometryNode lowerNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[1];
          GeometryNode upperNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[2];
          if(!volumeNode.IsExist || !lowerNode.IsExist || !upperNode.IsExist)
          {
            Console.WriteLine("Please complete setting.");
            return;
          }
        }
        else if(bufItem is BottomItem)
        {
          GeometryNode volumeNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[0];
          GeometryNode surfaceNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[1];
          if(!volumeNode.IsExist || !surfaceNode.IsExist)
          {
            Console.WriteLine("Please complete setting.");
            return;
          }
        }
        else if(bufItem is AutoSliceItem)
        {
          GeometryNode volumeNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[0];
          if(!volumeNode.IsExist)
          {
            Console.WriteLine("Please complete setting.");
            return;
          }
          if(this.tree.getTreeView().Nodes.Count != 1)
          {
            Console.WriteLine("Auto slice is only single block.");
            return;
          }
        }
      }
      this.state = SliceState.PreSlice;
      // Delete old layer
      if(this.volumeLayerCnt != null)
      {
        this.deleteOldLayer();
      }
      int volCnt = this.tree.getTreeView().Nodes.Count;
      this.volumeLayerCnt = new int[volCnt];
      this.surfaceIdx = new List<int>[volCnt];
      this.wallIdx = new List<int>[volCnt];
      this.infillIdx = new List<int>[volCnt];
      this.supportIdx = new List<int>[volCnt];
      for(int vol = 0; vol < volCnt; vol++)
      {
        this.volumeLayerCnt[vol] = 0;
        this.surfaceIdx[vol] = new List<int>();
        this.wallIdx[vol] = new List<int>();
        this.infillIdx[vol] = new List<int>();
        this.supportIdx[vol] = new List<int>();
      }

      for(int vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
      {
        // Traget check
        TreeItem bufItem = this.tree.items[vol];
        if(bufItem is SandwichItem)
        {
          SandwichItem item = (SandwichItem)bufItem;
          int volumeGeomid = item.Volume.Geometry.Geomid;
          int lowerGeomid = item.Lower.Geometry.Geomid;
          int upperGeomid = item.Upper.Geometry.Geomid;

          double ux, uy, uz, lx, ly, lz;
          double distance = 0.0;
          unsafe
          {
            this.view.myOCCTProxy.getLayerCenter(upperGeomid, &ux, &uy, &uz); 
            this.view.myOCCTProxy.getLayerCenter(lowerGeomid, &lx, &ly, &lz);
            distance = Math.Sqrt((ux - lx) * (ux - lx) + (uy - ly) * (uy - ly) + (uz - lz) * (uz - lz));
          }
          int subLayerCnt = 0;
          if(distance > 0)
          {
            subLayerCnt = (int)Math.Round(distance / item.Pitch, 0);
          }
          else
          {
            subLayerCnt = MAX_LAYER_CNT;
          }
          //MessageBox.Show("distance:" + distance + "/layerCnt:" + subLayerCnt);
          Console.WriteLine("Pre slice start for vol:" + vol);
          int[] subSurfaceIdx = new int[subLayerCnt];
          int[] subWallIdx = new int[subLayerCnt];
          int[] subInfillIdx = new int[subLayerCnt];
          int[] subSupportIdx = new int[subLayerCnt];

          // Calculate layer
          int nx = Math.Max(item.Lower.Geometry.Nx, item.Upper.Geometry.Nx);
          int ny = Math.Max(item.Lower.Geometry.Ny, item.Upper.Geometry.Ny);
          this.IsPreSliceDone = true;
          int outLayerCount = this.view.myOCCTProxy.calcLayer(subLayerCnt, 
            subSurfaceIdx, subWallIdx, subInfillIdx, subSupportIdx, 
            upperGeomid, lowerGeomid, this.tree.BasePlaneGeomId,
            nx, ny, item.Pitch,
            this.buttonSurface.ForeColor.R, this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
          this.layerTarget = 1;
          this.view.myOCCTProxy.SetSurfaceTransparent(subSurfaceIdx[this.layerTarget]);
          if(!item.Lower.Geometry.GeomVisible)
          {
            this.view.myOCCTProxy.SetTransparent(item.Lower.Geometry.Geomid, false);
          }
          if(!item.Upper.Geometry.GeomVisible)
          {
            this.view.myOCCTProxy.SetTransparent(item.Upper.Geometry.Geomid, false);
          }
          Console.WriteLine("Pre slice end for vol:" + vol);

          // array
          this.volumeLayerCnt[vol] = outLayerCount;
          for(int idx = 0; idx < subLayerCnt; idx++)
          {
            this.surfaceIdx[vol].Add(subSurfaceIdx[idx]);
            this.wallIdx[vol].Add(subWallIdx[idx]);
            this.infillIdx[vol].Add(subInfillIdx[idx]);
            this.supportIdx[vol].Add(subSupportIdx[idx]);
          }
        }
        else if(bufItem is BottomItem)
        {
          BottomItem item = (BottomItem)bufItem;
          int volumeGeomid = item.Volume.Geometry.Geomid;
          int surfaceGeomid = item.Surface.Geometry.Geomid;
          int[] subSurfaceIdx = new int[1];
          int[] subWallIdx = new int[1];
          int[] subInfillIdx = new int[1];
          int[] subSupportIdx = new int[1];
          int nx = item.Surface.Geometry.Nx;
          int ny = item.Surface.Geometry.Ny;
          this.IsPreSliceDone = true;
          int outLayerCount = this.view.myOCCTProxy.calcBottomLayer(
            subSurfaceIdx, subWallIdx, subInfillIdx, subSupportIdx,
            surfaceGeomid, this.tree.BasePlaneGeomId, nx, ny,
            this.buttonSurface.ForeColor.R, this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
          
          // array
          this.volumeLayerCnt[vol] = outLayerCount;
          for(int idx = 0; idx < 1; idx++)
          {
            this.surfaceIdx[vol].Add(subSurfaceIdx[idx]);
            this.wallIdx[vol].Add(subWallIdx[idx]);
            this.infillIdx[vol].Add(subInfillIdx[idx]);
            this.supportIdx[vol].Add(subSupportIdx[idx]);
          }
        }
        else if(bufItem is AutoSliceItem)
        {
          AutoSliceItem item = (AutoSliceItem)bufItem;
          int volumeGeomid = item.Volume.Geometry.Geomid;
          int[] subSurfaceIdx = new int[1000];
          int[] subWallIdx = new int[1000];
          int[] subInfillIdx = new int[1000];
          int[] subSupportIdx = new int[1000];
          this.IsPreSliceDone = true;
          int outLayerCount = this.view.myOCCTProxy.calcAutoSliceLayer(
            subSurfaceIdx, subWallIdx, subInfillIdx, subSupportIdx, 
            volumeGeomid, this.tree.BasePlaneGeomId, item.Thickness,
            this.buttonSurface.ForeColor.R, this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
          // array
          this.volumeLayerCnt[vol] = outLayerCount;
          for(int idx = 0; idx < 1000; idx++)
          {
            this.surfaceIdx[vol].Add(subSurfaceIdx[idx]);
            this.wallIdx[vol].Add(subWallIdx[idx]);
            this.infillIdx[vol].Add(subInfillIdx[idx]);
            this.supportIdx[vol].Add(subSupportIdx[idx]);
          }
        }
        else if(bufItem is PeelerItem)
        {
          PeelerItem item = (PeelerItem)bufItem;
          int surafaceGeomid = item.Volume.Geometry.Geomid;
          int[] subSurfaceIdx = new int[1];
          int[] subWallIdx = new int[1];
          int[] subInfillIdx = new int[1];
          int[] subSupportIdx = new int[1];
          this.IsPreSliceDone = true;
          int outLayerCount = this.view.myOCCTProxy.calcPeelerLayer(
            subSurfaceIdx, subWallIdx, subInfillIdx, subSupportIdx);
          // array
          this.volumeLayerCnt[vol] = outLayerCount;
          for(int idx = 0; idx < 1; idx++)
          {
            this.surfaceIdx[vol].Add(subSurfaceIdx[idx]);
            this.wallIdx[vol].Add(subWallIdx[idx]);
            this.infillIdx[vol].Add(subInfillIdx[idx]);
            this.supportIdx[vol].Add(subSupportIdx[idx]);
          }
        }
      }
      this.view.myOCCTProxy.SetObjectColor(this.surfaceIdx[0][0], this.buttonSurface.ForeColor.R, 
        this.buttonSurface.ForeColor.G, this.buttonSurface.ForeColor.B);
      this.view.myOCCTProxy.RedrawView();

      int totalCnt = 0;
      for(int vol = 0; vol < volCnt; vol++)
      {
        totalCnt += this.volumeLayerCnt[vol];
      }

      // trackBar and numericUpDown
      this.trackBar.Maximum = totalCnt - 1;
      this.trackBar.Minimum = 0;
      this.trackBar.Value = 0;
      this.numericUpDown.Maximum = totalCnt - 1;
      this.numericUpDown.Minimum = 0;
      this.numericUpDown.Value = 0;
    }

    private void trackBar_Scroll(object sender, EventArgs e)
    {
      this.layerTarget = this.trackBar.Value;
      this.trackBar.Value = this.layerTarget;
      this.numericUpDown.Value = this.layerTarget;
      int vol;
      int idx = 0;
      int total = 0;
      for(vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
      {
        if(this.layerTarget < total + this.volumeLayerCnt[vol])
        {
          idx = this.layerTarget - total;
          break;
        }
        else
        {
          total += this.volumeLayerCnt[vol];
        }
      }
      if(this.state == SliceState.PreSlice)
      {
        Color surfaceColor = this.buttonSurface.ForeColor;
        this.view.myOCCTProxy.SetObjectColor(this.surfaceIdx[vol][idx], surfaceColor.R, 
          surfaceColor.G, surfaceColor.B);
        this.view.myOCCTProxy.SetSurfaceTransparent(this.surfaceIdx[vol][idx]);
      }else if(this.state == SliceState.Slice)
      {
        Color surfaceColor = this.buttonSurface.ForeColor;
        Color curveColor = this.buttonCurve.ForeColor;
        this.view.myOCCTProxy.SetObjectColor(this.surfaceIdx[vol][idx], surfaceColor.R, 
          surfaceColor.G, surfaceColor.B);
        this.view.myOCCTProxy.SetObjectColor(this.wallIdx[vol][idx], curveColor.R, 
          curveColor.G, curveColor.B);
        this.view.myOCCTProxy.SetSurfaceCurveTransparent(this.surfaceIdx[vol][idx], 
          this.wallIdx[vol][idx], this.infillIdx[vol][idx]);
      }
    }

    private void numericUpDown_ValueChanged(object sender, EventArgs e)
    {
      this.layerTarget = (int)Math.Ceiling(this.numericUpDown.Value);
      this.trackBar.Value = this.layerTarget;
      this.numericUpDown.Value = this.layerTarget;
      int vol;
      int idx = 0;
      int total = 0;
      for(vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
      {
        if(this.layerTarget < total + this.volumeLayerCnt[vol])
        {
          idx = this.layerTarget - total;
          break;
        }
        else
        {
          total += this.volumeLayerCnt[vol];
        }
      }
      if(this.state == SliceState.PreSlice)
      {
        Color surfaceColor = this.buttonSurface.ForeColor;
        this.view.myOCCTProxy.SetObjectColor(this.surfaceIdx[vol][idx], surfaceColor.R, 
          surfaceColor.G, surfaceColor.B);
        this.view.myOCCTProxy.SetSurfaceTransparent(this.surfaceIdx[vol][idx]);
      }else if(this.state == SliceState.Slice)
      {
        Color surfaceColor = this.buttonSurface.ForeColor;
        Color curveColor = this.buttonCurve.ForeColor;
        this.view.myOCCTProxy.SetObjectColor(this.surfaceIdx[vol][idx], surfaceColor.R, 
          surfaceColor.G, surfaceColor.B);
        this.view.myOCCTProxy.SetObjectColor(this.wallIdx[vol][idx], curveColor.R, 
          curveColor.G, curveColor.B);
        this.view.myOCCTProxy.SetSurfaceCurveTransparent(this.surfaceIdx[vol][idx], 
          this.wallIdx[vol][idx], this.infillIdx[vol][idx]);
      }
    }

    private void buttonSlice_Click(object sender, EventArgs e)
    {
      System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
      sw.Start();
      if(this.state != SliceState.PreSlice)
      {
        Console.WriteLine("Please complete pre slice.");
        return;
      }
      this.slice = new SandwichSliceDialog();
      if(this.slice.ShowDialog() == DialogResult.OK)
      {
        if(this.slice.Path == "")
        {
          MessageBox.Show("Please select output filename.");
          return;
        }
        this.state = SliceState.Slice;
        // offset
        Dictionary<int, int> dictOffsetGeomId = new Dictionary<int, int>();
        int ndCnt = this.tree.getTreeView().Nodes.Count;
        int wlCnt = this.slice.wallCount;
        for(int vol = 0; vol < ndCnt; vol++)
        {
          TreeItem bufItem = this.tree.items[vol];
          if(bufItem is SandwichItem)
          {
            SandwichItem item = (SandwichItem)bufItem;
            int volumeGeomid = item.Volume.Geometry.Geomid;
            // Changed from a style that offsets the entire 3D model inward to one that offsets each layer individually
            //for(int wall = 0; wall < wlCnt; wall++)
            for(int wall = 0; wall < 1; wall++)
            {
              int key = volumeGeomid * wlCnt + wall;
              if(dictOffsetGeomId.ContainsKey(key))
              {
                item.OffsetGeomId.Add(dictOffsetGeomId[key]);
              }
              else
              {
                int newGeomid = this.tree.SurfaceCnt;
                //if(this.slice.wallOffset == false && this.slice.wallCount == 1)
                if(true)
                {
                  dictOffsetGeomId.Add(key, volumeGeomid);
                  item.OffsetGeomId.Add(volumeGeomid);
                }
                else
                {
                  this.tree.SurfaceCnt++;
                  dictOffsetGeomId.Add(key, newGeomid);
                  item.OffsetGeomId.Add(newGeomid);
                  double offset;
                  if(this.slice.wallOffset)
                  {
                    offset = -(wall + 0.5) * this.slice.Nozzle;
                  }
                  else
                  {
                    offset = -wall * this.slice.Nozzle;
                  }
                  this.view.myOCCTProxy.createOffsetObject(offset, volumeGeomid, newGeomid);
                }
              }
            }
          }
          else if(bufItem is BottomItem)
          {
          }
          else if(bufItem is AutoSliceItem)
          {
          }
          else if(bufItem is PeelerItem)
          {
          }
        }
        // file create
        using (FileStream fs = File.Create(this.slice.Path));
        double preX = 0.0;
        double preY = 0.0;
        double preZ = 0.0;
        double preI = 0.0;
        double preJ = 0.0;
        double preK = 1.0;
        for(int vol = 0; vol < this.tree.getTreeView().Nodes.Count; vol++)
        {
          TreeItem bufItem = this.tree.items[vol];
          if(bufItem is SandwichItem)
          {
            SandwichItem item = (SandwichItem)bufItem;

            // points
            double lastX, lastY, lastZ, lastI, lastJ, lastK;
            IntPtr ptr = Marshal.StringToHGlobalAnsi(this.slice.Path);
            GeometryNode lowerNode = (GeometryNode)this.tree.getTreeView().Nodes[vol].Nodes[1];
            int[] under = new int[this.volumeLayerCnt[vol]];
            int[] underInfill = new int[this.volumeLayerCnt[vol]];
            for(int i = 0; i < this.volumeLayerCnt[vol]; i++) 
            {
              if(i == 0)
              {
                if(vol == 0)
                {
                  under[i] = lowerNode.Geometry.Geomid;
                  underInfill[i] = 0;
                }
                else if(vol == 1 && this.tree.items[0] is BottomItem)
                {
                  under[i] = lowerNode.Geometry.Geomid;
                  underInfill[i] = 0;
                }
                else
                {
                  int buf = this.infillIdx[vol - 1].Count;
                  under[i] = this.surfaceIdx[vol - 1][buf - 1];
                  underInfill[i] = this.infillIdx[vol - 1][buf - 1];
                }
              }
              else
              {
                under[i] = this.surfaceIdx[vol][i - 1];
                underInfill[i] = this.infillIdx[vol][i - 1];
              }
            }
            unsafe
            {
              for(int loop = 0; loop < item.OffsetGeomId.Count; loop++)
              {
                this.view.myOCCTProxy.calcPoints((sbyte*)ptr, this.volumeLayerCnt[vol], this.slice.Distance, this.slice.Nozzle,  
                  this.surfaceIdx[vol].ToArray(), this.wallIdx[vol].ToArray(), this.infillIdx[vol].ToArray(), 
                  under, vol, item.OffsetGeomId[loop],
                  preX, preY, preZ, &lastX, &lastY, &lastZ,
                  preI, preJ, preK, &lastI, &lastJ, &lastK,
                  this.slice.infillEnable, this.slice.infillType, this.slice.infillPercent, underInfill,
                  this.slice.gapFillEnable, this.slice.gapFillPercent,
                  this.slice.wallOffset, this.slice.wallCount, this.slice.wallPercent);
                preX = lastX;
                preY = lastY;
                preZ = lastZ;
                preI = lastI;
                preJ = lastJ;
                preK = lastK;
              }
            }
          }
          else if(bufItem is BottomItem)
          {
            BottomItem item = (BottomItem)bufItem;
            int volumeGeomid = item.Volume.Geometry.Geomid;
            // points
            double lastX, lastY, lastZ, lastI, lastJ, lastK;
            IntPtr ptr = Marshal.StringToHGlobalAnsi(this.slice.Path);
            int[] under = new int[1];
            under[0] = this.tree.BasePlaneGeomId;
            unsafe
            {
              this.view.myOCCTProxy.calcBottomPoints((sbyte*)ptr, this.slice.Distance, this.slice.Nozzle, 
                this.surfaceIdx[vol].ToArray(), this.wallIdx[vol].ToArray(), this.infillIdx[vol].ToArray(), 
                under, vol, volumeGeomid, this.slice.wallCount, this.slice.wallPercent, item.Direction,
                preX, preY, preZ, &lastX, &lastY, &lastZ,
                preI, preJ, preK, &lastI, &lastJ, &lastK);
              preX = lastX;
              preY = lastY;
              preZ = lastZ;  
              preI = lastI;
              preJ = lastJ;
              preK = lastK;
            }
          }
          else if(bufItem is AutoSliceItem)
          {

          }
          else if(bufItem is PeelerItem)
          {
            PeelerItem item = (PeelerItem)bufItem;
            int peelerGeomid = item.Volume.Geometry.Geomid;
            // points
            double lastX, lastY, lastZ, lastI, lastJ, lastK;
            IntPtr ptr = Marshal.StringToHGlobalAnsi(this.slice.Path);
            unsafe
            {
              bool flag = true;
              if(item.Direction != 0) flag = false;
              this.view.myOCCTProxy.calcPeelerPoints(
                (sbyte*)ptr, this.slice.Distance, this.slice.Nozzle, item.Thickness, this.slice.wallPercent, 
                this.wallIdx[vol].ToArray(), peelerGeomid, vol, flag, 
                preX, preY, preZ, &lastX, &lastY, &lastZ,
                preI, preJ, preK, &lastI, &lastJ, &lastK);
              preX = lastX;
              preY = lastY;
              preZ = lastZ;  
              preI = lastI;
              preJ = lastJ;
              preK = lastK;
            }
          }
        }
        sw.Stop();
        this.view.myOCCTProxy.RedrawView();
        Console.WriteLine("Slice end.: " + sw.ElapsedMilliseconds + "msec");
      }
    }
  }
}
