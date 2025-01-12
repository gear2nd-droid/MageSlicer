namespace IE_WinForms
{
  partial class GeometryImportDialog
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
      this.label1 = new System.Windows.Forms.Label();
      this.buttonFile = new System.Windows.Forms.Button();
      this.textBoxFile = new System.Windows.Forms.TextBox();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.label8 = new System.Windows.Forms.Label();
      this.textBoxMvZ = new System.Windows.Forms.TextBox();
      this.label9 = new System.Windows.Forms.Label();
      this.textBoxMvY = new System.Windows.Forms.TextBox();
      this.label10 = new System.Windows.Forms.Label();
      this.textBoxMvX = new System.Windows.Forms.TextBox();
      this.label11 = new System.Windows.Forms.Label();
      this.checkBoxMv = new System.Windows.Forms.CheckBox();
      this.textBoxDirZ = new System.Windows.Forms.TextBox();
      this.label7 = new System.Windows.Forms.Label();
      this.textBoxPntZ = new System.Windows.Forms.TextBox();
      this.textBoxDirY = new System.Windows.Forms.TextBox();
      this.label6 = new System.Windows.Forms.Label();
      this.textBoxPntY = new System.Windows.Forms.TextBox();
      this.textBoxDirX = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.textBoxPntX = new System.Windows.Forms.TextBox();
      this.label4 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.textBoxAngle = new System.Windows.Forms.TextBox();
      this.checkBoxRot = new System.Windows.Forms.CheckBox();
      this.buttonImport = new System.Windows.Forms.Button();
      this.buttonCancel = new System.Windows.Forms.Button();
      this.labelPitch = new System.Windows.Forms.Label();
      this.numericPitch = new System.Windows.Forms.NumericUpDown();
      this.radioExist = new System.Windows.Forms.RadioButton();
      this.radioNew = new System.Windows.Forms.RadioButton();
      this.comboBox = new System.Windows.Forms.ComboBox();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.comboDirection = new System.Windows.Forms.ComboBox();
      this.label12 = new System.Windows.Forms.Label();
      this.groupBox1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericPitch)).BeginInit();
      this.groupBox2.SuspendLayout();
      this.SuspendLayout();
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(6, 23);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(58, 12);
      this.label1.TabIndex = 0;
      this.label1.Text = "Target file";
      // 
      // buttonFile
      // 
      this.buttonFile.Location = new System.Drawing.Point(70, 18);
      this.buttonFile.Name = "buttonFile";
      this.buttonFile.Size = new System.Drawing.Size(75, 23);
      this.buttonFile.TabIndex = 1;
      this.buttonFile.Text = "File open";
      this.buttonFile.UseVisualStyleBackColor = true;
      this.buttonFile.Click += new System.EventHandler(this.buttonFile_Click);
      // 
      // textBoxFile
      // 
      this.textBoxFile.Location = new System.Drawing.Point(151, 20);
      this.textBoxFile.Name = "textBoxFile";
      this.textBoxFile.Size = new System.Drawing.Size(257, 19);
      this.textBoxFile.TabIndex = 2;
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.label8);
      this.groupBox1.Controls.Add(this.textBoxMvZ);
      this.groupBox1.Controls.Add(this.label9);
      this.groupBox1.Controls.Add(this.textBoxMvY);
      this.groupBox1.Controls.Add(this.label10);
      this.groupBox1.Controls.Add(this.textBoxMvX);
      this.groupBox1.Controls.Add(this.label11);
      this.groupBox1.Controls.Add(this.checkBoxMv);
      this.groupBox1.Controls.Add(this.textBoxDirZ);
      this.groupBox1.Controls.Add(this.label7);
      this.groupBox1.Controls.Add(this.textBoxPntZ);
      this.groupBox1.Controls.Add(this.textBoxDirY);
      this.groupBox1.Controls.Add(this.label6);
      this.groupBox1.Controls.Add(this.textBoxPntY);
      this.groupBox1.Controls.Add(this.textBoxDirX);
      this.groupBox1.Controls.Add(this.label5);
      this.groupBox1.Controls.Add(this.textBoxPntX);
      this.groupBox1.Controls.Add(this.label4);
      this.groupBox1.Controls.Add(this.label3);
      this.groupBox1.Controls.Add(this.label2);
      this.groupBox1.Controls.Add(this.textBoxAngle);
      this.groupBox1.Controls.Add(this.checkBoxRot);
      this.groupBox1.Location = new System.Drawing.Point(6, 47);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(402, 192);
      this.groupBox1.TabIndex = 4;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Transform";
      // 
      // label8
      // 
      this.label8.AutoSize = true;
      this.label8.Location = new System.Drawing.Point(290, 149);
      this.label8.Name = "label8";
      this.label8.Size = new System.Drawing.Size(12, 12);
      this.label8.TabIndex = 25;
      this.label8.Text = "Z";
      // 
      // textBoxMvZ
      // 
      this.textBoxMvZ.Location = new System.Drawing.Point(292, 166);
      this.textBoxMvZ.Name = "textBoxMvZ";
      this.textBoxMvZ.Size = new System.Drawing.Size(100, 19);
      this.textBoxMvZ.TabIndex = 24;
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Location = new System.Drawing.Point(184, 149);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(12, 12);
      this.label9.TabIndex = 23;
      this.label9.Text = "Y";
      // 
      // textBoxMvY
      // 
      this.textBoxMvY.Location = new System.Drawing.Point(186, 166);
      this.textBoxMvY.Name = "textBoxMvY";
      this.textBoxMvY.Size = new System.Drawing.Size(100, 19);
      this.textBoxMvY.TabIndex = 22;
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Location = new System.Drawing.Point(78, 149);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(12, 12);
      this.label10.TabIndex = 21;
      this.label10.Text = "X";
      // 
      // textBoxMvX
      // 
      this.textBoxMvX.Location = new System.Drawing.Point(80, 166);
      this.textBoxMvX.Name = "textBoxMvX";
      this.textBoxMvX.Size = new System.Drawing.Size(100, 19);
      this.textBoxMvX.TabIndex = 20;
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Location = new System.Drawing.Point(6, 169);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(50, 12);
      this.label11.TabIndex = 19;
      this.label11.Text = "Distance";
      // 
      // checkBoxMv
      // 
      this.checkBoxMv.AutoSize = true;
      this.checkBoxMv.Location = new System.Drawing.Point(6, 131);
      this.checkBoxMv.Name = "checkBoxMv";
      this.checkBoxMv.Size = new System.Drawing.Size(51, 16);
      this.checkBoxMv.TabIndex = 18;
      this.checkBoxMv.Text = "Move";
      this.checkBoxMv.UseVisualStyleBackColor = true;
      this.checkBoxMv.CheckedChanged += new System.EventHandler(this.checkBoxMv_CheckedChanged);
      // 
      // textBoxDirZ
      // 
      this.textBoxDirZ.Location = new System.Drawing.Point(292, 104);
      this.textBoxDirZ.Name = "textBoxDirZ";
      this.textBoxDirZ.Size = new System.Drawing.Size(100, 19);
      this.textBoxDirZ.TabIndex = 17;
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(290, 62);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(12, 12);
      this.label7.TabIndex = 16;
      this.label7.Text = "Z";
      // 
      // textBoxPntZ
      // 
      this.textBoxPntZ.Location = new System.Drawing.Point(292, 79);
      this.textBoxPntZ.Name = "textBoxPntZ";
      this.textBoxPntZ.Size = new System.Drawing.Size(100, 19);
      this.textBoxPntZ.TabIndex = 15;
      // 
      // textBoxDirY
      // 
      this.textBoxDirY.Location = new System.Drawing.Point(186, 104);
      this.textBoxDirY.Name = "textBoxDirY";
      this.textBoxDirY.Size = new System.Drawing.Size(100, 19);
      this.textBoxDirY.TabIndex = 14;
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(184, 62);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(12, 12);
      this.label6.TabIndex = 13;
      this.label6.Text = "Y";
      // 
      // textBoxPntY
      // 
      this.textBoxPntY.Location = new System.Drawing.Point(186, 79);
      this.textBoxPntY.Name = "textBoxPntY";
      this.textBoxPntY.Size = new System.Drawing.Size(100, 19);
      this.textBoxPntY.TabIndex = 12;
      // 
      // textBoxDirX
      // 
      this.textBoxDirX.Location = new System.Drawing.Point(80, 104);
      this.textBoxDirX.Name = "textBoxDirX";
      this.textBoxDirX.Size = new System.Drawing.Size(100, 19);
      this.textBoxDirX.TabIndex = 11;
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(78, 62);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(12, 12);
      this.label5.TabIndex = 10;
      this.label5.Text = "X";
      // 
      // textBoxPntX
      // 
      this.textBoxPntX.Location = new System.Drawing.Point(80, 79);
      this.textBoxPntX.Name = "textBoxPntX";
      this.textBoxPntX.Size = new System.Drawing.Size(100, 19);
      this.textBoxPntX.TabIndex = 9;
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(6, 107);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(51, 12);
      this.label4.TabIndex = 8;
      this.label4.Text = "Direction";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(6, 82);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(68, 12);
      this.label3.TabIndex = 7;
      this.label3.Text = "Center point";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(6, 43);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(60, 12);
      this.label2.TabIndex = 6;
      this.label2.Text = "Angle[deg]";
      // 
      // textBoxAngle
      // 
      this.textBoxAngle.Location = new System.Drawing.Point(80, 40);
      this.textBoxAngle.Name = "textBoxAngle";
      this.textBoxAngle.Size = new System.Drawing.Size(100, 19);
      this.textBoxAngle.TabIndex = 5;
      // 
      // checkBoxRot
      // 
      this.checkBoxRot.AutoSize = true;
      this.checkBoxRot.Location = new System.Drawing.Point(6, 18);
      this.checkBoxRot.Name = "checkBoxRot";
      this.checkBoxRot.Size = new System.Drawing.Size(67, 16);
      this.checkBoxRot.TabIndex = 4;
      this.checkBoxRot.Text = "Rotation";
      this.checkBoxRot.UseVisualStyleBackColor = true;
      this.checkBoxRot.CheckedChanged += new System.EventHandler(this.checkBoxRot_CheckedChanged);
      // 
      // buttonImport
      // 
      this.buttonImport.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.buttonImport.Location = new System.Drawing.Point(123, 360);
      this.buttonImport.Name = "buttonImport";
      this.buttonImport.Size = new System.Drawing.Size(75, 23);
      this.buttonImport.TabIndex = 5;
      this.buttonImport.Text = "Import";
      this.buttonImport.UseVisualStyleBackColor = true;
      this.buttonImport.Click += new System.EventHandler(this.buttonImport_Click);
      // 
      // buttonCancel
      // 
      this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.buttonCancel.Location = new System.Drawing.Point(204, 360);
      this.buttonCancel.Name = "buttonCancel";
      this.buttonCancel.Size = new System.Drawing.Size(75, 23);
      this.buttonCancel.TabIndex = 6;
      this.buttonCancel.Text = "Cancel";
      this.buttonCancel.UseVisualStyleBackColor = true;
      this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
      // 
      // labelPitch
      // 
      this.labelPitch.AutoSize = true;
      this.labelPitch.Location = new System.Drawing.Point(12, 311);
      this.labelPitch.Name = "labelPitch";
      this.labelPitch.Size = new System.Drawing.Size(112, 12);
      this.labelPitch.TabIndex = 7;
      this.labelPitch.Text = "Slice layer thickness";
      // 
      // numericPitch
      // 
      this.numericPitch.Location = new System.Drawing.Point(130, 309);
      this.numericPitch.Name = "numericPitch";
      this.numericPitch.Size = new System.Drawing.Size(120, 19);
      this.numericPitch.TabIndex = 8;
      this.numericPitch.ValueChanged += new System.EventHandler(this.numericPitch_ValueChanged);
      // 
      // radioExist
      // 
      this.radioExist.AutoSize = true;
      this.radioExist.Location = new System.Drawing.Point(13, 13);
      this.radioExist.Name = "radioExist";
      this.radioExist.Size = new System.Drawing.Size(69, 16);
      this.radioExist.TabIndex = 9;
      this.radioExist.TabStop = true;
      this.radioExist.Text = "Exist file";
      this.radioExist.UseVisualStyleBackColor = true;
      this.radioExist.CheckedChanged += new System.EventHandler(this.radioExist_CheckedChanged);
      // 
      // radioNew
      // 
      this.radioNew.AutoSize = true;
      this.radioNew.Location = new System.Drawing.Point(13, 35);
      this.radioNew.Name = "radioNew";
      this.radioNew.Size = new System.Drawing.Size(65, 16);
      this.radioNew.TabIndex = 10;
      this.radioNew.TabStop = true;
      this.radioNew.Text = "New file";
      this.radioNew.UseVisualStyleBackColor = true;
      this.radioNew.CheckedChanged += new System.EventHandler(this.radioNew_CheckedChanged);
      // 
      // comboBox
      // 
      this.comboBox.FormattingEnabled = true;
      this.comboBox.Location = new System.Drawing.Point(89, 13);
      this.comboBox.Name = "comboBox";
      this.comboBox.Size = new System.Drawing.Size(340, 20);
      this.comboBox.TabIndex = 11;
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.groupBox1);
      this.groupBox2.Controls.Add(this.label1);
      this.groupBox2.Controls.Add(this.buttonFile);
      this.groupBox2.Controls.Add(this.textBoxFile);
      this.groupBox2.Location = new System.Drawing.Point(12, 57);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(417, 246);
      this.groupBox2.TabIndex = 12;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "New File";
      // 
      // comboDirection
      // 
      this.comboDirection.FormattingEnabled = true;
      this.comboDirection.Location = new System.Drawing.Point(130, 334);
      this.comboDirection.Name = "comboDirection";
      this.comboDirection.Size = new System.Drawing.Size(121, 20);
      this.comboDirection.TabIndex = 13;
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Location = new System.Drawing.Point(12, 337);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(51, 12);
      this.label12.TabIndex = 7;
      this.label12.Text = "Direction";
      // 
      // GeometryImportDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(437, 392);
      this.Controls.Add(this.comboDirection);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.comboBox);
      this.Controls.Add(this.radioNew);
      this.Controls.Add(this.radioExist);
      this.Controls.Add(this.numericPitch);
      this.Controls.Add(this.label12);
      this.Controls.Add(this.labelPitch);
      this.Controls.Add(this.buttonCancel);
      this.Controls.Add(this.buttonImport);
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "GeometryImportDialog";
      this.Text = "ImportDialog";
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericPitch)).EndInit();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Button buttonFile;
    private System.Windows.Forms.TextBox textBoxFile;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.CheckBox checkBoxRot;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.TextBox textBoxAngle;
    private System.Windows.Forms.Label label8;
    private System.Windows.Forms.TextBox textBoxMvZ;
    private System.Windows.Forms.Label label9;
    private System.Windows.Forms.TextBox textBoxMvY;
    private System.Windows.Forms.Label label10;
    private System.Windows.Forms.TextBox textBoxMvX;
    private System.Windows.Forms.Label label11;
    private System.Windows.Forms.CheckBox checkBoxMv;
    private System.Windows.Forms.TextBox textBoxDirZ;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.TextBox textBoxPntZ;
    private System.Windows.Forms.TextBox textBoxDirY;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.TextBox textBoxPntY;
    private System.Windows.Forms.TextBox textBoxDirX;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.TextBox textBoxPntX;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Button buttonImport;
    private System.Windows.Forms.Button buttonCancel;
    private System.Windows.Forms.Label labelPitch;
    private System.Windows.Forms.NumericUpDown numericPitch;
    private System.Windows.Forms.RadioButton radioExist;
    private System.Windows.Forms.RadioButton radioNew;
    private System.Windows.Forms.ComboBox comboBox;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.ComboBox comboDirection;
    private System.Windows.Forms.Label label12;
  }
}