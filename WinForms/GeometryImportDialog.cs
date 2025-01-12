using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IE_WinForms
{
  public partial class GeometryImportDialog : Form
  {
    private GeometryNode node;
    private List<Geometry> geoms;
    private NodePartType type;
    public bool IsNewFile;
    public double Pitch;
    public int Direction;
    /// <summary>
    /// 
    /// </summary>
    /// <param name="node">Target file</param>
    /// <param name="volumeOrsurface">T:volume / F:surface</param>
    public GeometryImportDialog(GeometryNode node, List<Geometry> geoms, 
      double pitch, int direction)
    {
      this.node = node;
      this.geoms = geoms;
      this.type = node.Type;
      InitializeComponent();
      this.textBoxFile.Enabled = true;
      this.checkBoxRot.Checked = false;
      this.checkBoxMv.Checked = false;
      this.textBoxDirX.Text = "0.0";
      this.textBoxDirY.Text = "0.0";
      this.textBoxDirZ.Text = "1.0";
      this.textBoxPntX.Text = "0.0";
      this.textBoxPntY.Text = "0.0";
      this.textBoxPntZ.Text = "0.0";
      this.textBoxAngle.Text = "0.0";
      this.textBoxMvX.Text = "0.0";
      this.textBoxMvY.Text = "0.0";
      this.textBoxMvZ.Text = "0.0";
      this.textBoxAngle.Enabled = false;
      this.textBoxDirX.Enabled = false;
      this.textBoxDirY.Enabled = false;
      this.textBoxDirZ.Enabled = false;
      this.textBoxPntX.Enabled = false;
      this.textBoxPntY.Enabled = false;
      this.textBoxPntZ.Enabled = false;
      this.textBoxMvX.Enabled = false;
      this.textBoxMvY.Enabled = false;
      this.textBoxMvZ.Enabled = false;
      this.checkBoxMv.Checked = false;
      this.checkBoxRot.Checked = false;
      this.numericPitch.DecimalPlaces = 2;
      this.numericPitch.Increment = 0.05M;
      this.numericPitch.Minimum = 0.05M;
      if(this.type == NodePartType.SandwichVolume)
      {
        this.labelPitch.Text = "Slice layer pitch";
        this.numericPitch.Enabled = true;
        this.numericPitch.Value = (decimal)pitch;
        this.comboDirection.Enabled = false;
      }
      else if(this.type == NodePartType.BottomVolume)
      {
        this.labelPitch.Text = "Slice layer thickeness";
        this.numericPitch.Enabled = true;
        this.numericPitch.Value = (decimal)pitch;
        this.comboDirection.Enabled = true;
        this.comboDirection.Items.Add("0 degrees");
        this.comboDirection.Items.Add("90 degrees");
        this.comboDirection.Items.Add("45 degrees");
        this.comboDirection.Items.Add("-45 degrees");
        this.comboDirection.SelectedIndex = direction;
      }
      else if(this.type == NodePartType.AutoSliceVolume)
      {
        this.labelPitch.Text = "Slice layer thickeness";
        this.numericPitch.Enabled = true;
        this.numericPitch.Value = (decimal)pitch;
        this.comboDirection.Enabled = false;
      }
      else if(this.type == NodePartType.PillerSurface)
      {
        this.labelPitch.Text = "Slice layer thickeness";
        this.numericPitch.Enabled = true;
        this.numericPitch.Value = (decimal)pitch;
        this.comboDirection.Enabled = true;
        this.comboDirection.Items.Clear();
        this.comboDirection.Items.Add("45 degrees");
        this.comboDirection.Items.Add("-45 degrees");
        this.comboDirection.SelectedIndex = direction;
      }
      else
      {
        this.labelPitch.Text = "Unuse";
        this.numericPitch.Enabled = false;
        this.numericPitch.Value = 0.3M;
      }
      if(geoms.Count > 0)
      {
        this.radioExist.Checked = true;
        this.radioNew.Checked = false;
      }
      else
      {
        this.radioNew.Checked = true;
        this.radioExist.Checked = false;
      }
      this.comboBox.Enabled = radioExist.Checked;
      this.buttonFile.Enabled = radioNew.Checked;
      this.textBoxFile.Enabled = radioNew.Checked;
      this.textBoxAngle.Enabled = radioNew.Checked;
      this.textBoxDirX.Enabled = radioNew.Checked;
      this.textBoxDirY.Enabled = radioNew.Checked;
      this.textBoxDirZ.Enabled = radioNew.Checked;
      this.textBoxMvX.Enabled = radioNew.Checked;
      this.textBoxMvY.Enabled = radioNew.Checked;
      this.textBoxMvZ.Enabled = radioNew.Checked;
      this.textBoxPntX.Enabled = radioNew.Checked;
      this.textBoxPntY.Enabled = radioNew.Checked;
      this.textBoxPntZ.Enabled = radioNew.Checked;
      this.checkBoxMv.Enabled = radioNew.Checked;
      this.checkBoxRot.Enabled = radioNew.Checked;
      // combo set
      foreach(Geometry geom in this.geoms)
      {
        this.comboBox.Items.Add(geom.Geomid + ":" + Path.GetFileName(geom.Path));
      }
      if(node.Geometry.usingCnt != 0)
      {
        this.radioExist.Checked = true;
        this.radioNew.Checked = false;
        this.textBoxFile.Text = node.Geometry.Path;
        this.textBoxAngle.Text = node.Geometry.Angle.ToString();
        this.textBoxDirX.Text = node.Geometry.DirectionX.ToString();
        this.textBoxDirY.Text = node.Geometry.DirectionY.ToString();
        this.textBoxDirZ.Text = node.Geometry.DirectionZ.ToString();
        this.textBoxMvX.Text = node.Geometry.MoveX.ToString();
        this.textBoxMvY.Text = node.Geometry.MoveY.ToString();
        this.textBoxMvZ.Text = node.Geometry.MoveZ.ToString();
        this.textBoxPntX.Text = node.Geometry.PointX.ToString();
        this.textBoxPntY.Text = node.Geometry.PointY.ToString();
        this.textBoxPntZ.Text = node.Geometry.PointZ.ToString();
        // combo check
        int idx = this.comboBox.FindString(node.Geometry.Geomid.ToString());
        this.comboBox.SelectedIndex = idx;
      }
    }

    private void buttonFile_Click(object sender, EventArgs e)
    {
      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
      if(this.type == NodePartType.SandwichVolume || this.type == NodePartType.BottomVolume || 
        this.type == NodePartType.AutoSliceVolume || this.type == NodePartType.PillerSurface)
      {
        ofd.FileName = "volume.step";
        ofd.Filter = "STEP file(*.step;*.stp)|*.step;*.stp";
        ofd.Title = "Volume file";
      }
      else
      {
        ofd.FileName = "surface.csv";
        ofd.Filter = "CSV file(*.csv)|*.csv|STEP file(*.step;*.stp)|*.step;*.stp";
        ofd.Title = "Surface file";
      }
      ofd.RestoreDirectory = true;
      ofd.CheckFileExists = true;
      ofd.CheckPathExists = true;
      if (ofd.ShowDialog() == DialogResult.OK)
      {
        this.textBoxFile.Text = ofd.FileName;
        this.textBoxFile.Enabled = false;
      }
    }

    private void checkBoxRot_CheckedChanged(object sender, EventArgs e)
    {
      if(this.checkBoxRot.Checked)
      {
        this.textBoxAngle.Enabled = true;
        this.textBoxDirX.Enabled = true;
        this.textBoxDirY.Enabled = true;
        this.textBoxDirZ.Enabled = true;
        this.textBoxPntX.Enabled = true;
        this.textBoxPntY.Enabled = true;
        this.textBoxPntZ.Enabled = true;
      }
      else
      {
        this.textBoxAngle.Enabled = false;
        this.textBoxDirX.Enabled = false;
        this.textBoxDirY.Enabled = false;
        this.textBoxDirZ.Enabled = false;
        this.textBoxPntX.Enabled = false;
        this.textBoxPntY.Enabled = false;
        this.textBoxPntZ.Enabled = false;
      }
    }

    private void checkBoxMv_CheckedChanged(object sender, EventArgs e)
    {
      if(this.checkBoxMv.Checked)
      {
        this.textBoxMvX.Enabled = true;
        this.textBoxMvY.Enabled = true;
        this.textBoxMvZ.Enabled = true;
      }
      else
      {
        this.textBoxMvX.Enabled = false;
        this.textBoxMvY.Enabled = false;
        this.textBoxMvZ.Enabled = false;
      }
    }

    private void buttonImport_Click(object sender, EventArgs e)
    {
      if(this.radioNew.Checked)
      {
        this.node.Geometry.Path = this.textBoxFile.Text;
        this.node.Geometry.IsTransform = false;
        if(this.checkBoxRot.Checked)
        {
          this.node.Geometry.IsTransform = true;
          this.node.Geometry.PointX = Convert.ToDouble(this.textBoxPntX.Text);
          this.node.Geometry.PointY = Convert.ToDouble(this.textBoxPntY.Text);
          this.node.Geometry.PointZ = Convert.ToDouble(this.textBoxPntZ.Text);
          this.node.Geometry.DirectionX = Convert.ToDouble(this.textBoxDirX.Text);
          this.node.Geometry.DirectionY = Convert.ToDouble(this.textBoxDirY.Text);
          this.node.Geometry.DirectionZ = Convert.ToDouble(this.textBoxDirZ.Text);
          this.node.Geometry.Angle = Convert.ToDouble(this.textBoxAngle.Text) / 180.0 * Math.PI;
        }
        else
        {
          this.node.Geometry.PointX = 0.0;
          this.node.Geometry.PointY = 0.0;
          this.node.Geometry.PointZ = 0.0;
          this.node.Geometry.DirectionX = 0.0;
          this.node.Geometry.DirectionY = 0.0;
          this.node.Geometry.DirectionZ = 1.0;
          this.node.Geometry.Angle = 0.0;
        }
        if(this.checkBoxMv.Checked)
        {
          this.node.Geometry.IsTransform = true;
          this.node.Geometry.MoveX = Convert.ToDouble(this.textBoxMvX.Text);
          this.node.Geometry.MoveY = Convert.ToDouble(this.textBoxMvY.Text);
          this.node.Geometry.MoveZ = Convert.ToDouble(this.textBoxMvZ.Text);
        }
        else
        {
          this.node.Geometry.MoveX = 0.0;
          this.node.Geometry.MoveY = 0.0;
          this.node.Geometry.MoveZ = 0.0;
        }
        this.Pitch = (double)this.numericPitch.Value;
        this.Direction = this.comboDirection.SelectedIndex;
        this.IsNewFile = true;
        this.Close();
      }
      else
      {
        int idx = this.comboBox.SelectedIndex;
        if(idx == -1)
        {
          MessageBox.Show("Select one of the existing files.");
        }
        else
        {
          this.node.Geometry = this.geoms[idx];
          this.Pitch = (double)this.numericPitch.Value;
          this.Direction = this.comboDirection.SelectedIndex;
          this.IsNewFile = false;
          this.Close();
        }
      }
    }

    private void buttonCancel_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void numericPitch_ValueChanged(object sender, EventArgs e)
    {
      double val = (double)this.numericPitch.Value;
      if(val < (double)this.numericPitch.Minimum)
      {
        this.numericPitch.Value = this.numericPitch.Minimum;
      }
    }

    private void radioExist_CheckedChanged(object sender, EventArgs e)
    {
      this.comboBox.Enabled = radioExist.Checked;
    }

    private void radioNew_CheckedChanged(object sender, EventArgs e)
    {
      this.buttonFile.Enabled = radioNew.Checked;
      this.textBoxFile.Enabled = radioNew.Checked;
      this.textBoxAngle.Enabled = radioNew.Checked;
      this.textBoxDirX.Enabled = radioNew.Checked;
      this.textBoxDirY.Enabled = radioNew.Checked;
      this.textBoxDirZ.Enabled = radioNew.Checked;
      this.textBoxMvX.Enabled = radioNew.Checked;
      this.textBoxMvY.Enabled = radioNew.Checked;
      this.textBoxMvZ.Enabled = radioNew.Checked;
      this.textBoxPntX.Enabled = radioNew.Checked;
      this.textBoxPntY.Enabled = radioNew.Checked;
      this.textBoxPntZ.Enabled = radioNew.Checked;
      this.checkBoxMv.Enabled = radioNew.Checked;
      this.checkBoxRot.Enabled = radioNew.Checked;
    }
  }
}
