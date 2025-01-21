using IE_WinForms;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IE_WinForms
{
  public class Geometry
  {
    public string Path { get; set; }
    public bool GeomVisible { get; set; }
    public int Geomid { get; set; }
    public double MoveX { get; set; }
    public double MoveY { get; set; }
    public double MoveZ { get; set; }
    public double PointX { get; set; }
    public double PointY { get; set; }
    public double PointZ { get; set; }
    public double DirectionX { get; set; }
    public double DirectionY { get; set; }
    public double DirectionZ { get; set; }
    public double Angle { get; set; }
    public bool IsTransform { get; set; }
    public int usingCnt { get; set; }
    public Color Color { get; set; }
    public int Nx { get; set; }
    public int Ny { get; set; }

    public override bool Equals(object obj)
    {
      bool ret = true;
      if(obj is Geometry)
      {
        Geometry other = (Geometry)obj;
        if(this.Path != other.Path){ret = false;}
        if(this.Geomid != other.Geomid){ret = false;}
        if(this.Angle != other.Angle){ret = false;}
        if(this.PointX != other.PointX){ret = false;}
        if(this.PointY != other.PointY){ret = false;}
        if(this.PointZ != other.PointZ){ret = false;}
        if(this.DirectionX != other.DirectionX){ret = false;}
        if(this.DirectionY != other.DirectionY){ret = false;}
        if(this.DirectionZ != other.DirectionZ){ret = false;}
        if(this.MoveX != other.MoveX){ret = false;}
        if(this.MoveY != other.MoveY){ret = false;}
        if(this.MoveZ != other.MoveZ){ret = false;}
        return ret;
      }
      else
      {
        return false;
      }
    }

    public Geometry DeepCopy()
    {
      return new Geometry
      {
        Path = this.Path,
        GeomVisible = this.GeomVisible,
        Geomid = this.Geomid,
        MoveX = this.MoveX,
        MoveY = this.MoveY,
        MoveZ = this.MoveZ,
        PointX = this.PointX,
        PointY = this.PointY,
        PointZ = this.PointZ,
        DirectionX = this.DirectionX,
        DirectionY = this.DirectionY,
        DirectionZ = this.DirectionZ,
        Angle = this.Angle,
        IsTransform = this.IsTransform,
        usingCnt = this.usingCnt,
        Color = this.Color,
        Nx = this.Nx,
        Ny = this.Ny,
      };
    }
  }

  public enum NodeType
  {
    Sandwich,
    Bottom,
    AutoSlice,
    Peeler
  }

  public class TreeItem
  {
    public NodeType NodeType;
    public TreeNode SliceType { get; set; }
  }

  public enum NodePartType
  {
    SandwichVolume,
    SandwichUpper,
    SandwichLower,
    BottomSurface,
    BottomVolume,
    AutoSliceVolume,
    PeelerSurface
  }

  public class GeometryNode : TreeNode
  {
    public Geometry Geometry { get; set; }
    public NodePartType Type {get; set; }
    public bool IsExist { get; set; }


    public GeometryNode()
    {
      this.Geometry = new Geometry();
      this.IsExist = false;
      this.Geometry.GeomVisible = true;
      this.Geometry.usingCnt = 0;
    }
    
    public override bool Equals(object obj)
    {
      bool ret = true;
      if(obj is Geometry)
      {
        GeometryNode other = (GeometryNode)obj;
        if(this.Geometry != other.Geometry){ret = false;}
        if(this.Type != other.Type){ret = false;}
        return ret;
      }
      else
      {
        return false;
      }
    }
    public GeometryNode DeepCopy()
    {
      return new GeometryNode
      {
        Geometry = this.Geometry.DeepCopy(),
        Type = this.Type,
        IsExist = this.IsExist,
      };
    }

  }

  public class SandwichItem: TreeItem
  {
    public GeometryNode Volume { get; set; }
    public GeometryNode Upper { get; set; }
    public GeometryNode Lower { get; set; }
    public double Pitch { get; set; }
    public List<int> OffsetGeomId { get; set; }

    public SandwichItem()
    {
      this.OffsetGeomId = new List<int>();
      this.NodeType = NodeType.Sandwich;
      this.SliceType = new TreeNode();
      this.SliceType.Text = "Sandwich";
      this.SliceType.ForeColor = Color.Black;
      this.Volume = new GeometryNode();
      this.Volume.Text = "Volume";
      this.Volume.Type = NodePartType.SandwichVolume;
      this.Volume.ForeColor = Color.Black;
      this.Volume.Geometry.Geomid = 0;
      this.Pitch = 0.3;
      this.Upper = new GeometryNode();
      this.Upper.Text = "Upper";
      this.Upper.Type = NodePartType.SandwichUpper;
      this.Upper.ForeColor = Color.Black;
      this.Upper.Geometry.Geomid = 0;
      this.Lower = new GeometryNode();
      this.Lower.Text = "Lower";
      this.Lower.Type = NodePartType.SandwichLower;
      this.Lower.ForeColor = Color.Black;
      this.Lower.Geometry.Geomid = 0;
      this.SliceType.Nodes.Add(this.Volume);
      this.SliceType.Nodes.Add(this.Lower);
      this.SliceType.Nodes.Add(this.Upper);
      this.SliceType.Expand();
    }
  }

  public class BottomItem: TreeItem
  {
    public GeometryNode Volume { get; set; }
    public GeometryNode Surface { get; set; }
    public double Thickness { get; set; }
    public int Direction { get; set; }
    public BottomItem() 
    { 
      this.NodeType = NodeType.Bottom;
      this.SliceType = new TreeNode();
      this.SliceType.Text = "Bottom";
      this.SliceType.ForeColor = Color.Black;
      this.Volume = new GeometryNode();
      this.Volume.Text = "Volume";
      this.Volume.Type = NodePartType.BottomVolume;
      this.Volume.ForeColor = Color.Black;
      this.Volume.Geometry.Geomid = 0;
      this.Surface = new GeometryNode();
      this.Surface.Text = "Surface";
      this.Surface.Type = NodePartType.BottomSurface;
      this.Surface.ForeColor = Color.Black;
      this.SliceType.Nodes.Add(this.Volume);
      this.SliceType.Nodes.Add(this.Surface);
      this.SliceType.Expand();
      this.Thickness = 0.3;
      this.Direction = 0;
    }
  }

  public class AutoSliceItem: TreeItem
  {
    public GeometryNode Volume { get; set; }
    public double Thickness { get; set; }
    public AutoSliceItem() 
    { 
      this.NodeType = NodeType.AutoSlice;
      this.SliceType = new TreeNode();
      this.SliceType.Text = "AutoSlice";
      this.SliceType.ForeColor = Color.Black;
      this.Volume = new GeometryNode();
      this.Volume.Text = "Volume";
      this.Volume.Type = NodePartType.AutoSliceVolume;
      this.Volume.ForeColor = Color.Black;
      this.Volume.Geometry.Geomid = 0;
      this.SliceType.Nodes.Add(this.Volume);
      this.SliceType.Expand();
      this.Thickness = 0.3;
    }
  }

  public class PeelerItem: TreeItem
  {
    public GeometryNode Volume { get; set; }
    public double Thickness { get; set; }
    public int Direction { get; set; }
    public PeelerItem() 
    { 
      this.NodeType = NodeType.Peeler;
      this.SliceType = new TreeNode();
      this.SliceType.Text = "Peeler";
      this.SliceType.ForeColor = Color.Black;
      this.Volume = new GeometryNode();
      this.Volume.Text = "Volume";
      this.Volume.Type = NodePartType.PeelerSurface;
      this.Volume.ForeColor = Color.Black;
      this.SliceType.Nodes.Add(this.Volume);
      this.SliceType.Expand();
      this.Thickness = 0.3;
      this.Direction = 0;
    }
  }
}
