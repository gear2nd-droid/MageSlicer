namespace IE_WinForms
{
  partial class SandwichSliceDialog
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
      this.numericPoints = new System.Windows.Forms.NumericUpDown();
      this.label1 = new System.Windows.Forms.Label();
      this.buttonOK = new System.Windows.Forms.Button();
      this.buttonCancel = new System.Windows.Forms.Button();
      this.textBoxFile = new System.Windows.Forms.TextBox();
      this.buttonFile = new System.Windows.Forms.Button();
      this.label2 = new System.Windows.Forms.Label();
      this.checkBoxInfill = new System.Windows.Forms.CheckBox();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.numericInfillPercent = new System.Windows.Forms.NumericUpDown();
      this.label4 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.comboBoxInfill = new System.Windows.Forms.ComboBox();
      this.label5 = new System.Windows.Forms.Label();
      this.numericNozzle = new System.Windows.Forms.NumericUpDown();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.numericGapFill = new System.Windows.Forms.NumericUpDown();
      this.label6 = new System.Windows.Forms.Label();
      this.checkBoxGap = new System.Windows.Forms.CheckBox();
      this.numericWall = new System.Windows.Forms.NumericUpDown();
      this.label7 = new System.Windows.Forms.Label();
      this.checkWallOffset = new System.Windows.Forms.CheckBox();
      ((System.ComponentModel.ISupportInitialize)(this.numericPoints)).BeginInit();
      this.groupBox1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericInfillPercent)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.numericNozzle)).BeginInit();
      this.groupBox2.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericGapFill)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.numericWall)).BeginInit();
      this.SuspendLayout();
      // 
      // numericPoints
      // 
      this.numericPoints.Location = new System.Drawing.Point(102, 39);
      this.numericPoints.Name = "numericPoints";
      this.numericPoints.Size = new System.Drawing.Size(120, 19);
      this.numericPoints.TabIndex = 0;
      this.numericPoints.ValueChanged += new System.EventHandler(this.numericPoints_ValueChanged);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 41);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(84, 12);
      this.label1.TabIndex = 1;
      this.label1.Text = "Points distance";
      // 
      // buttonOK
      // 
      this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.buttonOK.Location = new System.Drawing.Point(128, 346);
      this.buttonOK.Name = "buttonOK";
      this.buttonOK.Size = new System.Drawing.Size(75, 23);
      this.buttonOK.TabIndex = 2;
      this.buttonOK.Text = "OK";
      this.buttonOK.UseVisualStyleBackColor = true;
      this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
      // 
      // buttonCancel
      // 
      this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.buttonCancel.Location = new System.Drawing.Point(209, 346);
      this.buttonCancel.Name = "buttonCancel";
      this.buttonCancel.Size = new System.Drawing.Size(75, 23);
      this.buttonCancel.TabIndex = 3;
      this.buttonCancel.Text = "Cancel";
      this.buttonCancel.UseVisualStyleBackColor = true;
      this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
      // 
      // textBoxFile
      // 
      this.textBoxFile.Location = new System.Drawing.Point(157, 12);
      this.textBoxFile.Name = "textBoxFile";
      this.textBoxFile.Size = new System.Drawing.Size(257, 19);
      this.textBoxFile.TabIndex = 6;
      // 
      // buttonFile
      // 
      this.buttonFile.Location = new System.Drawing.Point(76, 10);
      this.buttonFile.Name = "buttonFile";
      this.buttonFile.Size = new System.Drawing.Size(75, 23);
      this.buttonFile.TabIndex = 5;
      this.buttonFile.Text = "File open";
      this.buttonFile.UseVisualStyleBackColor = true;
      this.buttonFile.Click += new System.EventHandler(this.buttonFile_Click);
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 15);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(58, 12);
      this.label2.TabIndex = 4;
      this.label2.Text = "Target file";
      // 
      // checkBoxInfill
      // 
      this.checkBoxInfill.AutoSize = true;
      this.checkBoxInfill.Location = new System.Drawing.Point(6, 18);
      this.checkBoxInfill.Name = "checkBoxInfill";
      this.checkBoxInfill.Size = new System.Drawing.Size(84, 16);
      this.checkBoxInfill.TabIndex = 7;
      this.checkBoxInfill.Text = "Enable infill";
      this.checkBoxInfill.UseVisualStyleBackColor = true;
      this.checkBoxInfill.CheckedChanged += new System.EventHandler(this.checkBoxInfill_CheckedChanged);
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.numericInfillPercent);
      this.groupBox1.Controls.Add(this.label4);
      this.groupBox1.Controls.Add(this.label3);
      this.groupBox1.Controls.Add(this.comboBoxInfill);
      this.groupBox1.Controls.Add(this.checkBoxInfill);
      this.groupBox1.Location = new System.Drawing.Point(12, 169);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(399, 96);
      this.groupBox1.TabIndex = 8;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Infill";
      // 
      // numericInfillPercent
      // 
      this.numericInfillPercent.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
      this.numericInfillPercent.Location = new System.Drawing.Point(74, 66);
      this.numericInfillPercent.Name = "numericInfillPercent";
      this.numericInfillPercent.Size = new System.Drawing.Size(143, 19);
      this.numericInfillPercent.TabIndex = 9;
      this.numericInfillPercent.ValueChanged += new System.EventHandler(this.numericInfillPercent_ValueChanged);
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(6, 68);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(62, 12);
      this.label4.TabIndex = 10;
      this.label4.Text = "Percentage";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(6, 43);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(53, 12);
      this.label3.TabIndex = 9;
      this.label3.Text = "Infill type";
      // 
      // comboBoxInfill
      // 
      this.comboBoxInfill.FormattingEnabled = true;
      this.comboBoxInfill.Location = new System.Drawing.Point(74, 40);
      this.comboBoxInfill.Name = "comboBoxInfill";
      this.comboBoxInfill.Size = new System.Drawing.Size(143, 20);
      this.comboBoxInfill.TabIndex = 8;
      this.comboBoxInfill.SelectedIndexChanged += new System.EventHandler(this.comboBoxInfill_SelectedIndexChanged);
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(12, 66);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(86, 12);
      this.label5.TabIndex = 10;
      this.label5.Text = "Nozzle diameter";
      // 
      // numericNozzle
      // 
      this.numericNozzle.Location = new System.Drawing.Point(102, 64);
      this.numericNozzle.Name = "numericNozzle";
      this.numericNozzle.Size = new System.Drawing.Size(120, 19);
      this.numericNozzle.TabIndex = 9;
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.numericGapFill);
      this.groupBox2.Controls.Add(this.label6);
      this.groupBox2.Controls.Add(this.checkBoxGap);
      this.groupBox2.Location = new System.Drawing.Point(14, 271);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(399, 69);
      this.groupBox2.TabIndex = 8;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Gap fill";
      // 
      // numericGapFill
      // 
      this.numericGapFill.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
      this.numericGapFill.Location = new System.Drawing.Point(74, 40);
      this.numericGapFill.Name = "numericGapFill";
      this.numericGapFill.Size = new System.Drawing.Size(143, 19);
      this.numericGapFill.TabIndex = 9;
      this.numericGapFill.Value = new decimal(new int[] {
            70,
            0,
            0,
            0});
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(6, 42);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(62, 12);
      this.label6.TabIndex = 10;
      this.label6.Text = "Percentage";
      // 
      // checkBoxGap
      // 
      this.checkBoxGap.AutoSize = true;
      this.checkBoxGap.Location = new System.Drawing.Point(6, 18);
      this.checkBoxGap.Name = "checkBoxGap";
      this.checkBoxGap.Size = new System.Drawing.Size(97, 16);
      this.checkBoxGap.TabIndex = 7;
      this.checkBoxGap.Text = "Enable gap fill";
      this.checkBoxGap.UseVisualStyleBackColor = true;
      this.checkBoxGap.CheckedChanged += new System.EventHandler(this.checkBoxGap_CheckedChanged);
      // 
      // numericWall
      // 
      this.numericWall.Location = new System.Drawing.Point(102, 89);
      this.numericWall.Name = "numericWall";
      this.numericWall.Size = new System.Drawing.Size(120, 19);
      this.numericWall.TabIndex = 11;
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(12, 91);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(58, 12);
      this.label7.TabIndex = 10;
      this.label7.Text = "Wall count";
      // 
      // checkWallOffset
      // 
      this.checkWallOffset.AutoSize = true;
      this.checkWallOffset.Location = new System.Drawing.Point(228, 91);
      this.checkWallOffset.Name = "checkWallOffset";
      this.checkWallOffset.Size = new System.Drawing.Size(142, 16);
      this.checkWallOffset.TabIndex = 12;
      this.checkWallOffset.Text = "Offset to inside of wall";
      this.checkWallOffset.UseVisualStyleBackColor = true;
      // 
      // SandwichSliceDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(425, 381);
      this.Controls.Add(this.checkWallOffset);
      this.Controls.Add(this.numericWall);
      this.Controls.Add(this.label7);
      this.Controls.Add(this.label5);
      this.Controls.Add(this.numericNozzle);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.textBoxFile);
      this.Controls.Add(this.buttonFile);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.buttonCancel);
      this.Controls.Add(this.buttonOK);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.numericPoints);
      this.Name = "SandwichSliceDialog";
      this.Text = "SliceDialog";
      ((System.ComponentModel.ISupportInitialize)(this.numericPoints)).EndInit();
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericInfillPercent)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.numericNozzle)).EndInit();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericGapFill)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.numericWall)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.NumericUpDown numericPoints;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Button buttonOK;
    private System.Windows.Forms.Button buttonCancel;
    private System.Windows.Forms.TextBox textBoxFile;
    private System.Windows.Forms.Button buttonFile;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.CheckBox checkBoxInfill;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.NumericUpDown numericInfillPercent;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.ComboBox comboBoxInfill;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.NumericUpDown numericNozzle;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.NumericUpDown numericGapFill;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.CheckBox checkBoxGap;
    private System.Windows.Forms.NumericUpDown numericWall;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.CheckBox checkWallOffset;
  }
}