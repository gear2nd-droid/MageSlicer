namespace IE_WinForms
{
  partial class SliceEndDialog
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
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
      System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
      System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
      System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
      this.splitContainer1 = new System.Windows.Forms.SplitContainer();
      this.colorView = new SharpGL.OpenGLControl();
      this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
      this.splitContainer1.Panel1.SuspendLayout();
      this.splitContainer1.Panel2.SuspendLayout();
      this.splitContainer1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.colorView)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
      this.SuspendLayout();
      // 
      // splitContainer1
      // 
      this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.splitContainer1.Location = new System.Drawing.Point(12, 12);
      this.splitContainer1.Name = "splitContainer1";
      this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
      // 
      // splitContainer1.Panel1
      // 
      this.splitContainer1.Panel1.Controls.Add(this.colorView);
      // 
      // splitContainer1.Panel2
      // 
      this.splitContainer1.Panel2.Controls.Add(this.chart1);
      this.splitContainer1.Size = new System.Drawing.Size(600, 845);
      this.splitContainer1.SplitterDistance = 597;
      this.splitContainer1.TabIndex = 0;
      // 
      // colorView
      // 
      this.colorView.DrawFPS = false;
      this.colorView.Location = new System.Drawing.Point(3, 3);
      this.colorView.Name = "colorView";
      this.colorView.OpenGLVersion = SharpGL.Version.OpenGLVersion.OpenGL2_1;
      this.colorView.RenderContextType = SharpGL.RenderContextType.DIBSection;
      this.colorView.RenderTrigger = SharpGL.RenderTrigger.TimerBased;
      this.colorView.Size = new System.Drawing.Size(594, 591);
      this.colorView.TabIndex = 0;
      this.colorView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.colorView_MouseDown);
      this.colorView.MouseMove += new System.Windows.Forms.MouseEventHandler(this.colorView_MouseMove);
      // 
      // chart1
      // 
      chartArea1.Name = "ChartArea1";
      this.chart1.ChartAreas.Add(chartArea1);
      legend1.Name = "Legend1";
      this.chart1.Legends.Add(legend1);
      this.chart1.Location = new System.Drawing.Point(3, 3);
      this.chart1.Name = "chart1";
      series1.ChartArea = "ChartArea1";
      series1.Legend = "Legend1";
      series1.Name = "Series1";
      this.chart1.Series.Add(series1);
      this.chart1.Size = new System.Drawing.Size(594, 238);
      this.chart1.TabIndex = 0;
      this.chart1.Text = "chart1";
      // 
      // SliceEndDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(624, 869);
      this.Controls.Add(this.splitContainer1);
      this.Name = "SliceEndDialog";
      this.Text = "SliceEndDialog";
      this.Load += new System.EventHandler(this.SliceEndDialog_Load);
      this.splitContainer1.Panel1.ResumeLayout(false);
      this.splitContainer1.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
      this.splitContainer1.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.colorView)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.SplitContainer splitContainer1;
    private SharpGL.OpenGLControl colorView;
    private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
  }
}