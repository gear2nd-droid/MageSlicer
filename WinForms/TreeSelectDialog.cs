using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;

namespace IE_WinForms
{
  public partial class TreeSelectDialog : Form
  {
    private TreeItem item;
    private OcctView view;
    private SliceControl slice;
    private DataTree tree;
    public TreeSelectDialog(TreeItem item, OcctView view, SliceControl slice, DataTree tree)
    {
      InitializeComponent();
      this.item =  item;
      this.view = view;
      this.slice = slice;
      this.tree = tree;
      this.comboBox.Items.Add("Sandwich");
      this.comboBox.Items.Add("Bottom");
      this.comboBox.Items.Add("AutoSlice");
      this.comboBox.Items.Add("Piller");
      switch(item.NodeType)
      {
        case NodeType.Sandwich:
          this.comboBox.SelectedItem = 0;
          break;
        case NodeType.Bottom:
          this.comboBox.SelectedItem = 1;
          break;
        case NodeType.AutoSlice:
          this.comboBox.SelectedItem = 2;
          break;
        case NodeType.Piller:
          this.comboBox.SelectedItem = 3;
          break;
      }
    }

    private void buttonOK_Click(object sender, EventArgs e)
    {
      NodeType selected = NodeType.Sandwich;
      switch(this.comboBox.SelectedIndex)
      {
        case 0:
          selected = NodeType.Sandwich;
          break;
        case 1:
          selected = NodeType.Bottom;
          break;
        case 2:
          selected = NodeType.AutoSlice;
          break;
        case 3:
          selected = NodeType.Piller;
          break;
      }
      if(item.NodeType != selected)
      {
        // delete
        int idx = this.tree.DeleteItem(item);
        // add
        switch(selected)
        {
          case NodeType.Sandwich:
            this.item.SliceType.Text = "Sandwich";
            this.tree.AddSandwichItem(idx);
            break;
          case NodeType.Bottom:
            this.item.SliceType.Text = "Bottom";
            this.tree.AddBottomItem(idx);
            break;
          case NodeType.AutoSlice:
            this.item.SliceType.Text = "AutoSlice";
            this.tree.AddAutoSliceItem(idx);
            break;
          case NodeType.Piller:
            this.item.SliceType.Text = "Piller";
            this.tree.AddPillerItem(idx);
            break;
        }
      }
      this.Close();
    }

    private void buttonCancel_Click(object sender, EventArgs e)
    {
      this.Close();
    }
  }
}
