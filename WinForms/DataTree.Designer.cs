namespace IE_WinForms
{
    partial class DataTree
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
      this.toolStrip = new System.Windows.Forms.ToolStrip();
      this.toolStripButtonAdd = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonDelete = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonUp = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonDown = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonOpen = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonColor = new System.Windows.Forms.ToolStripButton();
      this.toolStripButtonVisible = new System.Windows.Forms.ToolStripButton();
      this.panel = new System.Windows.Forms.Panel();
      this.treeView = new System.Windows.Forms.TreeView();
      this.toolStrip.SuspendLayout();
      this.panel.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStrip
      // 
      this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonAdd,
            this.toolStripButtonDelete,
            this.toolStripButtonUp,
            this.toolStripButtonDown,
            this.toolStripButtonOpen,
            this.toolStripButtonColor,
            this.toolStripButtonVisible});
      this.toolStrip.Location = new System.Drawing.Point(0, 0);
      this.toolStrip.Name = "toolStrip";
      this.toolStrip.Size = new System.Drawing.Size(214, 25);
      this.toolStrip.TabIndex = 1;
      this.toolStrip.Text = "toolStrip1";
      // 
      // toolStripButtonAdd
      // 
      this.toolStripButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonAdd.Image = global::IE_WinForms.Properties.Resources.add_box_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonAdd.Name = "toolStripButtonAdd";
      this.toolStripButtonAdd.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonAdd.Text = "Add";
      this.toolStripButtonAdd.Click += new System.EventHandler(this.toolStripButtonAdd_Click);
      // 
      // toolStripButtonDelete
      // 
      this.toolStripButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonDelete.Image = global::IE_WinForms.Properties.Resources.delete_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonDelete.Name = "toolStripButtonDelete";
      this.toolStripButtonDelete.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonDelete.Text = "Delete";
      this.toolStripButtonDelete.Click += new System.EventHandler(this.toolStripButtonDelete_Click);
      // 
      // toolStripButtonUp
      // 
      this.toolStripButtonUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonUp.Image = global::IE_WinForms.Properties.Resources.arrow_upward_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonUp.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonUp.Name = "toolStripButtonUp";
      this.toolStripButtonUp.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonUp.Text = "Up";
      this.toolStripButtonUp.Click += new System.EventHandler(this.toolStripButtonUp_Click);
      // 
      // toolStripButtonDown
      // 
      this.toolStripButtonDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonDown.Image = global::IE_WinForms.Properties.Resources.arrow_downward_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonDown.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonDown.Name = "toolStripButtonDown";
      this.toolStripButtonDown.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonDown.Text = "Down";
      this.toolStripButtonDown.Click += new System.EventHandler(this.toolStripButtonDown_Click);
      // 
      // toolStripButtonOpen
      // 
      this.toolStripButtonOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonOpen.Image = global::IE_WinForms.Properties.Resources.draft_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonOpen.Name = "toolStripButtonOpen";
      this.toolStripButtonOpen.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonOpen.Text = "Open";
      this.toolStripButtonOpen.Click += new System.EventHandler(this.toolStripButtonOpen_Click);
      // 
      // toolStripButtonColor
      // 
      this.toolStripButtonColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonColor.Image = global::IE_WinForms.Properties.Resources.apps_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonColor.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonColor.Name = "toolStripButtonColor";
      this.toolStripButtonColor.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonColor.Text = "Color";
      this.toolStripButtonColor.Click += new System.EventHandler(this.toolStripButtonColor_Click);
      // 
      // toolStripButtonVisible
      // 
      this.toolStripButtonVisible.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.toolStripButtonVisible.Image = global::IE_WinForms.Properties.Resources.check_circle_FILL0_wght400_GRAD0_opsz24;
      this.toolStripButtonVisible.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButtonVisible.Name = "toolStripButtonVisible";
      this.toolStripButtonVisible.Size = new System.Drawing.Size(23, 22);
      this.toolStripButtonVisible.Text = "Visible";
      this.toolStripButtonVisible.Click += new System.EventHandler(this.toolStripButtonVisible_Click);
      // 
      // panel
      // 
      this.panel.Controls.Add(this.treeView);
      this.panel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.panel.Location = new System.Drawing.Point(0, 0);
      this.panel.Name = "panel";
      this.panel.Size = new System.Drawing.Size(214, 450);
      this.panel.TabIndex = 2;
      // 
      // treeView
      // 
      this.treeView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.treeView.Location = new System.Drawing.Point(12, 28);
      this.treeView.Name = "treeView";
      this.treeView.Size = new System.Drawing.Size(190, 410);
      this.treeView.TabIndex = 1;
      this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
      // 
      // DataTree
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(214, 450);
      this.Controls.Add(this.toolStrip);
      this.Controls.Add(this.panel);
      this.Name = "DataTree";
      this.Text = "DataTree";
      this.toolStrip.ResumeLayout(false);
      this.toolStrip.PerformLayout();
      this.panel.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

        }

    #endregion
    private System.Windows.Forms.ToolStrip toolStrip;
    private System.Windows.Forms.ToolStripButton toolStripButtonAdd;
    private System.Windows.Forms.ToolStripButton toolStripButtonDelete;
    private System.Windows.Forms.ToolStripButton toolStripButtonUp;
    private System.Windows.Forms.ToolStripButton toolStripButtonDown;
    private System.Windows.Forms.ToolStripButton toolStripButtonOpen;
    private System.Windows.Forms.ToolStripButton toolStripButtonColor;
    private System.Windows.Forms.ToolStripButton toolStripButtonVisible;
    private System.Windows.Forms.Panel panel;
    private System.Windows.Forms.TreeView treeView;
  }
}