using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;

namespace IE_WinForms
{

  public partial class DataProperty : Form
  {
    DataTree datatree;
    public DataProperty()
    {
      InitializeComponent();
    }

    public void setDataTree(DataTree dataTree)
    {
      this.datatree = dataTree;
    }

    public void viewDataTree(TreeNode node)
    {
      if(node is GeometryNode)
      {
        this.dataGridView.Rows.Clear();
        this.dataGridView.Columns.Clear();
        this.dataGridView.ColumnCount = 2;
        GeometryNode bufNode = node as GeometryNode;
        switch(bufNode.Type)
        {
          case NodePartType.SandwichVolume:
            this.dataGridView.Rows.Add("Node type", "Sandwich");
            this.dataGridView.Rows.Add("Geometry type", "Volume");
            break;
          case NodePartType.SandwichUpper:
            this.dataGridView.Rows.Add("Node type", "Sandwich");
            this.dataGridView.Rows.Add("Geometry type", "Upper");
            break;
          case NodePartType.SandwichLower:
            this.dataGridView.Rows.Add("Node type", "Sandwich");
            this.dataGridView.Rows.Add("Geometry type", "Lower");
            break;
          case NodePartType.BottomSurface:
            this.dataGridView.Rows.Add("Node type", "Bottom");
            this.dataGridView.Rows.Add("Geometry type", "Surface");
            break;
          case NodePartType.BottomVolume:
            this.dataGridView.Rows.Add("Node type", "Bottom");
            this.dataGridView.Rows.Add("Geometry type", "Volume");
            break;
          case NodePartType.AutoSliceVolume:
            this.dataGridView.Rows.Add("Node type", "AutoSlice");
            this.dataGridView.Rows.Add("Geometry type", "Target");
            break;
          case NodePartType.PeelerSurface:
            this.dataGridView.Rows.Add("Node type", "Peeler");
            this.dataGridView.Rows.Add("Geometry type", "Target");
            break;
        }
        if(bufNode.Geometry.usingCnt != 0)
        {
          this.dataGridView.Rows.Add("Geom id", bufNode.Geometry.Geomid);
          this.dataGridView.Rows.Add("File path", bufNode.Geometry.Path);
          this.dataGridView.Rows.Add("Angle", bufNode.Geometry.Angle.ToString());
          this.dataGridView.Rows.Add("DirX", bufNode.Geometry.DirectionX.ToString());
          this.dataGridView.Rows.Add("DirY", bufNode.Geometry.DirectionY.ToString());
          this.dataGridView.Rows.Add("DirZ", bufNode.Geometry.DirectionZ.ToString());
          this.dataGridView.Rows.Add("MvX", bufNode.Geometry.MoveX.ToString());
          this.dataGridView.Rows.Add("MvY", bufNode.Geometry.MoveY.ToString());
          this.dataGridView.Rows.Add("MvZ", bufNode.Geometry.MoveZ.ToString());
          this.dataGridView.Rows.Add("PntX", bufNode.Geometry.PointX.ToString());
          this.dataGridView.Rows.Add("PntY", bufNode.Geometry.PointY.ToString());
          this.dataGridView.Rows.Add("PntZ", bufNode.Geometry.PointZ.ToString());
        }
      }
      else if(node is TreeNode)
      {
        this.dataGridView.Rows.Clear();
        this.dataGridView.Columns.Clear();
        this.dataGridView.ColumnCount = 2;
        this.dataGridView.Rows.Add("Node type", node.Text);
        int idx = this.datatree.getTreeView().Nodes.IndexOf(node);
        if(this.datatree.items[idx] is SandwichItem)
        {
          SandwichItem item = (SandwichItem)this.datatree.items[idx];
          this.dataGridView.Rows.Add("Pitch", item.Pitch);
        }
        else if(this.datatree.items[idx] is BottomItem)
        {
          BottomItem item = (BottomItem)this.datatree.items[idx];
          this.dataGridView.Rows.Add("Thickness", item.Thickness);
          switch(item.Direction)
          {
            case 0:
              this.dataGridView.Rows.Add("Direction", "0 degrees");
              break;
            case 1:
              this.dataGridView.Rows.Add("Direction", "90 degrees");
              break;
            case 2:
              this.dataGridView.Rows.Add("Direction", "45 degrees");
              break;
            case 3:
              this.dataGridView.Rows.Add("Direction", "-45 degrees");
              break;
          }
        }
        else if(this.datatree.items[idx] is AutoSliceItem)
        {
          AutoSliceItem item = (AutoSliceItem)this.datatree.items[idx];
          this.dataGridView.Rows.Add("Thickness", item.Thickness);
        }
        else if(this.datatree.items[idx] is PeelerItem)
        {
          PeelerItem item = (PeelerItem)this.datatree.items[idx];
          this.dataGridView.Rows.Add("Thickness", item.Thickness);
          switch(item.Direction)
          {
            case 0:
              this.dataGridView.Rows.Add("Direction", "45 degrees");
              break;
            case 1:
              this.dataGridView.Rows.Add("Direction", "-45 degrees");
              break;
          }
        }
      }
    }
  }
}
