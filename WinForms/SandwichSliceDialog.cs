using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IE_WinForms
{
  public partial class SandwichSliceDialog : Form
  {
    public string Path { get; set; }
    public double Distance { get; set; }
    public double Nozzle { get; set; }
    public bool infillEnable { get; set; }
    public int infillType{ get; set; }
    public double infillPercent { get; set; }
    public bool gapFillEnable { get; set; }
    public double gapFillPercent { get; set; }
    public int wallCount { get; set; }
    public bool wallOffset {get; set; }

    public SandwichSliceDialog()
    {
      InitializeComponent();
      this.numericPoints.DecimalPlaces = 2;
      this.numericPoints.Increment = 0.05M;
      this.numericPoints.Minimum = 0.05M;
      this.numericPoints.Enabled = true;
      this.numericPoints.Value = 0.5M;
      this.numericNozzle.DecimalPlaces = 2;
      this.numericNozzle.Increment = 0.1M;
      this.numericNozzle.Minimum = 0.1M;
      this.numericNozzle.Enabled = true;
      this.numericNozzle.Value = 0.8M;
      this.comboBoxInfill.Items.Add("Grid on UV");
      this.comboBoxInfill.Items.Add("Grid on XYZ");
      this.comboBoxInfill.SelectedIndex = 0;
      this.comboBoxInfill.DropDownStyle = ComboBoxStyle.DropDownList;
      this.comboBoxInfill.Enabled = false;
      this.numericInfillPercent.DecimalPlaces = 0;
      this.numericInfillPercent.Increment = 10M;
      this.numericInfillPercent.Minimum = 0M;
      this.numericInfillPercent.Maximum = 100M;
      this.numericInfillPercent.Enabled = false;
      this.numericInfillPercent.Value = 10M;
      this.numericGapFill.DecimalPlaces = 0;
      this.numericGapFill.Increment = 5M;
      this.numericGapFill.Minimum = 0M;
      this.numericGapFill.Maximum = 100M;
      this.numericGapFill.Enabled = false;
      this.numericGapFill.Value = 70M;
      this.numericWall.DecimalPlaces = 0;
      this.numericWall.Increment = 1M;
      this.numericWall.Minimum = 1M;
      this.numericWall.Enabled = true;
      this.numericWall.Value = 1M;
    }

    private void numericPoints_ValueChanged(object sender, EventArgs e)
    {
    }

    private void buttonOK_Click(object sender, EventArgs e)
    {
      this.Path = this.textBoxFile.Text;
      this.Distance = (double)this.numericPoints.Value;
      this.Nozzle = (double)this.numericNozzle.Value;
      this.infillEnable = this.checkBoxInfill.Checked;
      this.infillType = this.comboBoxInfill.SelectedIndex;
      this.gapFillEnable = this.checkBoxGap.Checked;
      this.wallCount = (int)this.numericWall.Value;
      this.wallOffset = this.checkWallOffset.Checked;
      if(this.checkBoxInfill.Checked)
      {
        this.infillPercent = (double)this.numericInfillPercent.Value / 100.0;
      }
      else
      {
        this.infillPercent = 0.0;
      }
      if(this.checkBoxGap.Checked)
      {
        this.gapFillPercent = (double)this.numericGapFill.Value / 100.0;
      }
      else
      {
        this.gapFillPercent = 0.0;
      }
      this.Close();
    }

    private void buttonCancel_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void buttonFile_Click(object sender, EventArgs e)
    {
      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
      ofd.FileName = "output.csv";
      ofd.Filter = "Csv file(*.csv)|*.csv";
      ofd.Title = "Output csv file";
      ofd.RestoreDirectory = true;
      ofd.CheckFileExists = false;
      ofd.CheckPathExists = true;
      if (ofd.ShowDialog() == DialogResult.OK)
      {
        this.textBoxFile.Text = ofd.FileName;
        this.textBoxFile.Enabled = true;
      }
    }

    private void checkBoxInfill_CheckedChanged(object sender, EventArgs e)
    {
      if(this.checkBoxInfill.Checked)
      {
        this.numericInfillPercent.Enabled = true;
        this.comboBoxInfill.Enabled = true;
      }
      else
      {
        this.numericInfillPercent.Enabled = false;
        this.comboBoxInfill.Enabled = false;
      }
    }

    private void comboBoxInfill_SelectedIndexChanged(object sender, EventArgs e)
    {
    }

    private void numericInfillPercent_ValueChanged(object sender, EventArgs e)
    {
    }

    private void checkBoxGap_CheckedChanged(object sender, EventArgs e)
    {
      if(this.checkBoxGap.Checked)
      {
        this.numericGapFill.Enabled = true;
      }
      else
      {
        this.numericGapFill.Enabled = false;
      }
    }
  }
}
