using SharpGL;
using SharpGL.SceneGraph.Cameras;
using SharpGL.SceneGraph.Primitives;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace IE_WinForms
{
  public partial class SliceEndDialog : Form
  {
    private string fullpath;
    private List<double> x;
    private List<double> y;
    private List<double> z;
    private List<double> th;
    private double nozzle;
    private int preMouseX;
    private int preMouseY;
    private int actKeyState;
    private double maxAngleX = Math.PI - 0.01;
    private double minAngleX = -Math.PI - 0.01;
    private double maxAngleZ = Math.PI - 0.01;
    private double minAngleZ = -Math.PI + 0.01;
    private double cameraDistance = 150.0;
    private double cameraAngleX = Math.PI * 1.0 / 4.0;
    private double cameraAngleZ = 0.0;
    private double targetPosX = 0.0;
    private double targetPosY = 0.0;
    private double targetPosZ = 0.0;


    public SliceEndDialog(string fullpath)
    {
      InitializeComponent();
      this.fullpath = fullpath;
      this.x = new List<double>();
      this.y = new List<double>();
      this.z = new List<double>();
      this.th = new List<double>();
      this.readCSv();
      OpenGL gl = this.colorView.OpenGL;
      this.drawInit();
      this.drawColorView();
      this.drawHistogram();
      //gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
      //gl.LoadIdentity();
      //gl.Translate(0.0, 0.0, -5.0);
      //Teapot teapot = new Teapot();
      //teapot.Draw(gl, 14, 1.0f, OpenGL.GL_FILL);
      //gl.Flush();
      //this.fitView();
      //this.drawColorView();
    }
    private static Color hsv2rgb(double h, double s, double v)
    {
        double c = v * s;
        double x = c * (1 - Math.Abs((h / 60) % 2 - 1));
        double m = v - c;

        double r = 0, g = 0, b = 0;

        if (h >= 0 && h < 60) { r = c; g = x; b = 0; }
        else if (h >= 60 && h < 120) { r = x; g = c; b = 0; }
        else if (h >= 120 && h < 180) { r = 0; g = c; b = x; }
        else if (h >= 180 && h < 240) { r = 0; g = x; b = c; }
        else if (h >= 240 && h < 300) { r = x; g = 0; b = c; }
        else if (h >= 300 && h < 360) { r = c; g = 0; b = x; }

        int R = (int)((r + m) * 255);
        int G = (int)((g + m) * 255);
        int B = (int)((b + m) * 255);

        return Color.FromArgb(R, G, B);
    }

    private void readCSv()
    {
      this.nozzle = 0.0;
      using(StreamReader reader = new StreamReader(this.fullpath))
      {
        while(!reader.EndOfStream)
        {
          string line = reader.ReadLine();
          string[] buf = line.Split(',');
          double bx = double.Parse(buf[1]);
          double by = double.Parse(buf[2]);
          double bz = double.Parse(buf[3]);
          double bth = double.Parse(buf[7]);
          double bnz = double.Parse(buf[8]);
          this.x.Add(bx);
          this.y.Add(by);
          this.z.Add(bz);
          this.th.Add(bth);
          this.nozzle += bnz;
        }
      }
      this.nozzle /= this.th.Count;
    }

    private void drawColorView()
    {
      OpenGL gl = this.colorView.OpenGL;

      // point
      gl.PointSize(1.0f);
      gl.Begin(OpenGL.GL_POINTS);
      int cnt = this.th.Count;
      for(int i = 0; i < cnt; i++)
      {
        double bth = this.th[i];
        if(bth > this.nozzle)
        {
          bth = this.nozzle;
        }
        double h = 10.0 + 340.0 * bth / this.nozzle;
        double s = 1.0;
        double v = 1.0;
        Color rgb = hsv2rgb(h, s, v);
        gl.Color(rgb.R, rgb.G, rgb.B);
        gl.Vertex(this.x[i], this.y[i], this.z[i]);
      }
      gl.End();
      gl.Flush();
    }
    private void fitView()
    {
      OpenGL gl = this.colorView.OpenGL;
      double minX = this.x.Min();
      double maxX = this.x.Max();
      double minY = this.y.Min();
      double maxY = this.y.Max();
      double minZ = this.z.Min();
      double maxZ = this.z.Max();
      double cenX = (minX + maxX) / 2.0;
      double cenY = (minY + maxY) / 2.0;
      double cenZ = (minZ + maxZ) / 2.0;
      double sizeX = maxX - minX;
      double sizeY = maxY - minY;
      double sizeZ = maxZ - minZ;
      double maxSize = Math.Max(sizeX, Math.Max(sizeY, sizeZ));
      double distance = maxSize * 2.0;
      gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
      gl.LoadIdentity();
      gl.MatrixMode(OpenGL.GL_PROJECTION);
      gl.Perspective(45.0, (double)this.colorView.Width / this.colorView.Height, 0.1, distance * 3.0);
      gl.MatrixMode(OpenGL.GL_MODELVIEW);
      gl.LookAt(cenX + distance, cenY + distance, cenZ + distance, cenX, cenY, cenZ, 0, 1, 0);
    }

    private void colorView_MouseMove(object sender, MouseEventArgs e)
    {
      int deltaX = e.X - this.preMouseX;
      int deltaY = e.Y - this.preMouseY;
      if((Control.MouseButtons & MouseButtons.Left) == MouseButtons.Left)
      {
        if((Control.ModifierKeys & Keys.Shift) == Keys.Shift)
        {
          // with shift & left click
          double sensitivity = 0.5;
          this.cameraDistance += deltaY * sensitivity;
          if (this.cameraDistance < 1.0) this.cameraDistance = 1.0;
			    if (this.cameraDistance > 500.0) this.cameraDistance = 500.0;
        }
        else
        {
          // only left click
          double sensitivity = 0.005;
			    this.cameraAngleZ += deltaX * sensitivity;
			    this.cameraAngleX += deltaY * sensitivity;
			    if (this.cameraAngleX > maxAngleX) this.cameraAngleX = maxAngleX;
			    if (this.cameraAngleX < minAngleX) this.cameraAngleX = minAngleX;
			    if (this.cameraAngleZ > maxAngleZ) this.cameraAngleZ = maxAngleZ;
			    if (this.cameraAngleZ < minAngleZ) this.cameraAngleZ = minAngleZ;
        }
      }
      else if((Control.MouseButtons & MouseButtons.Right) == MouseButtons.Right)
      {
        if((Control.ModifierKeys & Keys.Shift) == Keys.Shift)
        {
          // with shift & right click
          double sensitivity = 0.5;
          this.targetPosZ += deltaY * sensitivity;
          if (this.targetPosZ < -500.0) this.targetPosZ = -500.0;
			    if (this.targetPosZ > 500.0) this.targetPosZ = 500.0;
        }
        else
        {
          // only right click
			    double sensitivity = 0.5f;
			    this.targetPosX += deltaX * sensitivity;
			    this.targetPosY += deltaY * sensitivity;
			    if (this.targetPosX > 500.0) this.targetPosX = 500.0;
			    if (this.targetPosX < -500.0) this.targetPosX = -500.0;
			    if (this.targetPosY > 500.0) this.targetPosY = 500.0;
			    if (this.targetPosY < -500.0) this.targetPosY = -500.0;
        }
      }
      this.drawInit();
      this.drawColorView();
      this.preMouseX = e.X;
      this.preMouseY = e.Y;
    }

    private void colorView_MouseDown(object sender, MouseEventArgs e)
    {
      this.preMouseX = e.X;
      this.preMouseY = e.Y;
    }

    private void drawInit()
    {
      OpenGL gl = this.colorView.OpenGL;
	    gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
	    gl.MatrixMode(OpenGL.GL_PROJECTION);
	    gl.LoadIdentity();
      gl.Perspective(45.0, this.colorView.Width / this.colorView.Height, 0.1, 1000.0);
	    gl.MatrixMode(OpenGL.GL_MODELVIEW);
	    gl.LoadIdentity();
      double[] bufCamera = {
        Math.Sin(this.cameraAngleZ) * Math.Cos(this.cameraAngleX),
        -Math.Sin(this.cameraAngleX),
		    Math.Cos(this.cameraAngleZ) * Math.Cos(this.cameraAngleX) };
      double[] eyePos = {
        bufCamera[0] * this.cameraDistance + this.targetPosX,
        bufCamera[1] * this.cameraDistance + this.targetPosY,
		    bufCamera[2] * this.cameraDistance + this.targetPosZ };
      gl.LookAt(eyePos[0], eyePos[1], eyePos[2], this.targetPosX, this.targetPosY, this.targetPosZ, 0, 0, 1);
    }

    private void drawHistogram()
    {
      this.chart1.ChartAreas.Clear();
      ChartArea area = new ChartArea();
      area.AxisX.LabelStyle.Format = "0.00";
      this.chart1.ChartAreas.Add(area);
      Series series = new Series();
      series.ChartType = SeriesChartType.Column;
      series.IsValueShownAsLabel = false;
      series.IsVisibleInLegend = false;
      this.chart1.Series.Clear();
      this.chart1.Series.Add(series);
      this.chart1.Legends.Clear();
      // histogram
      int binCnt = 20;
      double bin = this.nozzle / (double)binCnt;
      int[] arrCnt = new int[binCnt + 1];
      int cnt = this.th.Count;
      for(int i = 0; i < cnt; i++)
      {
        int buf = (int)(this.th[i] / bin);
        if(buf >= binCnt) buf = binCnt;
        arrCnt[buf] += 1;
      }
      for(int i = 0; i < binCnt; i++)
      {
        double h = 10.0 + 340.0 * (i + 0.5) * bin / this.nozzle;
        double s = 1.0;
        double v = 1.0;
        Color clr = hsv2rgb(h, s, v);
        DataPoint point = new DataPoint((i + 0.5) * bin, arrCnt[i]);
        point.Color = clr;
        series.Points.Add(point);
      }
      Color lastClr = hsv2rgb(360.0, 1.0, 1.0);
      DataPoint lastPoint = new DataPoint((binCnt + 0.5) * bin, arrCnt[binCnt]);
      lastPoint.Color = lastClr;
      series.Points.Add(lastPoint);
    }

    private void SliceEndDialog_Load(object sender, EventArgs e)
    {

    }
  }
}
