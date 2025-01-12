namespace IE_WinForms
{
  partial class SliceControl
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
      this.buttonSurface = new System.Windows.Forms.Button();
      this.buttonPreslice = new System.Windows.Forms.Button();
      this.buttonSlice = new System.Windows.Forms.Button();
      this.numericUpDown = new System.Windows.Forms.NumericUpDown();
      this.trackBar = new System.Windows.Forms.TrackBar();
      this.buttonCurve = new System.Windows.Forms.Button();
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.trackBar)).BeginInit();
      this.SuspendLayout();
      // 
      // buttonSurface
      // 
      this.buttonSurface.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonSurface.Location = new System.Drawing.Point(12, 12);
      this.buttonSurface.Name = "buttonSurface";
      this.buttonSurface.Size = new System.Drawing.Size(215, 23);
      this.buttonSurface.TabIndex = 0;
      this.buttonSurface.Text = "Surface color";
      this.buttonSurface.UseVisualStyleBackColor = true;
      this.buttonSurface.Click += new System.EventHandler(this.buttonSurfaceColor_Click);
      // 
      // buttonPreslice
      // 
      this.buttonPreslice.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonPreslice.Location = new System.Drawing.Point(12, 70);
      this.buttonPreslice.Name = "buttonPreslice";
      this.buttonPreslice.Size = new System.Drawing.Size(215, 23);
      this.buttonPreslice.TabIndex = 1;
      this.buttonPreslice.Text = "Pre slice";
      this.buttonPreslice.UseVisualStyleBackColor = true;
      this.buttonPreslice.Click += new System.EventHandler(this.buttonPreslice_Click);
      // 
      // buttonSlice
      // 
      this.buttonSlice.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonSlice.Location = new System.Drawing.Point(12, 415);
      this.buttonSlice.Name = "buttonSlice";
      this.buttonSlice.Size = new System.Drawing.Size(215, 23);
      this.buttonSlice.TabIndex = 2;
      this.buttonSlice.Text = "Slice";
      this.buttonSlice.UseVisualStyleBackColor = true;
      this.buttonSlice.Click += new System.EventHandler(this.buttonSlice_Click);
      // 
      // numericUpDown
      // 
      this.numericUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.numericUpDown.Location = new System.Drawing.Point(12, 390);
      this.numericUpDown.Name = "numericUpDown";
      this.numericUpDown.Size = new System.Drawing.Size(215, 19);
      this.numericUpDown.TabIndex = 3;
      this.numericUpDown.ValueChanged += new System.EventHandler(this.numericUpDown_ValueChanged);
      // 
      // trackBar
      // 
      this.trackBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
      this.trackBar.Location = new System.Drawing.Point(92, 99);
      this.trackBar.Name = "trackBar";
      this.trackBar.Orientation = System.Windows.Forms.Orientation.Vertical;
      this.trackBar.Size = new System.Drawing.Size(45, 285);
      this.trackBar.TabIndex = 4;
      this.trackBar.Scroll += new System.EventHandler(this.trackBar_Scroll);
      // 
      // buttonCurve
      // 
      this.buttonCurve.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonCurve.Location = new System.Drawing.Point(12, 41);
      this.buttonCurve.Name = "buttonCurve";
      this.buttonCurve.Size = new System.Drawing.Size(215, 23);
      this.buttonCurve.TabIndex = 5;
      this.buttonCurve.Text = "Curve color";
      this.buttonCurve.UseVisualStyleBackColor = true;
      this.buttonCurve.Click += new System.EventHandler(this.buttonCurveColor_Click);
      // 
      // SliceControl
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(239, 450);
      this.Controls.Add(this.buttonCurve);
      this.Controls.Add(this.trackBar);
      this.Controls.Add(this.numericUpDown);
      this.Controls.Add(this.buttonSlice);
      this.Controls.Add(this.buttonPreslice);
      this.Controls.Add(this.buttonSurface);
      this.Name = "SliceControl";
      this.Text = "SliceControl";
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.trackBar)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button buttonSurface;
    private System.Windows.Forms.Button buttonPreslice;
    private System.Windows.Forms.Button buttonSlice;
    private System.Windows.Forms.NumericUpDown numericUpDown;
    private System.Windows.Forms.TrackBar trackBar;
    private System.Windows.Forms.Button buttonCurve;
  }
}