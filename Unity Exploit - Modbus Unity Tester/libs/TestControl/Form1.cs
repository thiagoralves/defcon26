using System;
using System.Drawing;
using System.Windows.Forms;

namespace TestControl
{
   public partial class Form1 : Form
   {
      public Form1()
      {
         InitializeComponent();
         textBox1.Text = "Make sure your WinForms application targets \".NET Framework 4\" rather than the default \".NET Framework 4 Client Profile\".";
      }
   }
}
