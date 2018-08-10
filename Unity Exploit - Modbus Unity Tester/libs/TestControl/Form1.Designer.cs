namespace TestControl
{
   partial class Form1
   {
      /// <summary>
      /// Required designer variable.
      /// </summary>
      private System.ComponentModel.IContainer components = null;

      /// <summary>
      /// Clean up any resources being used.
      /// </summary>
      /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
      protected override void Dispose( bool disposing )
      {
         if ( disposing && ( components != null ) )
         {
            components.Dispose();
         }
         base.Dispose( disposing );
      }

      #region Windows Form Designer generated code

      /// <summary>
      /// Required method for Designer support - do not modify
      /// the contents of this method with the code editor.
      /// </summary>
      private void InitializeComponent()
      {
         this.ipAddressControl1 = new IPAddressControlLib.IPAddressControl();
         this.label1 = new System.Windows.Forms.Label();
         this.textBox1 = new System.Windows.Forms.TextBox();
         this.SuspendLayout();
         // 
         // ipAddressControl1
         // 
         this.ipAddressControl1.AllowInternalTab = false;
         this.ipAddressControl1.AutoHeight = true;
         this.ipAddressControl1.BackColor = System.Drawing.SystemColors.Window;
         this.ipAddressControl1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
         this.ipAddressControl1.Cursor = System.Windows.Forms.Cursors.IBeam;
         this.ipAddressControl1.Location = new System.Drawing.Point(79, 12);
         this.ipAddressControl1.MinimumSize = new System.Drawing.Size(87, 20);
         this.ipAddressControl1.Name = "ipAddressControl1";
         this.ipAddressControl1.ReadOnly = false;
         this.ipAddressControl1.Size = new System.Drawing.Size(122, 20);
         this.ipAddressControl1.TabIndex = 0;
         this.ipAddressControl1.Text = "...";
         // 
         // label1
         // 
         this.label1.AutoSize = true;
         this.label1.Location = new System.Drawing.Point(12, 15);
         this.label1.Name = "label1";
         this.label1.Size = new System.Drawing.Size(61, 13);
         this.label1.TabIndex = 1;
         this.label1.Text = "IP Address:";
         // 
         // textBox1
         // 
         this.textBox1.Enabled = false;
         this.textBox1.Location = new System.Drawing.Point(15, 55);
         this.textBox1.Multiline = true;
         this.textBox1.Name = "textBox1";
         this.textBox1.Size = new System.Drawing.Size(186, 108);
         this.textBox1.TabIndex = 2;
         // 
         // Form1
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(213, 175);
         this.Controls.Add(this.textBox1);
         this.Controls.Add(this.label1);
         this.Controls.Add(this.ipAddressControl1);
         this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
         this.Name = "Form1";
         this.Text = "TestControl";
         this.ResumeLayout(false);
         this.PerformLayout();

      }

      #endregion

      private IPAddressControlLib.IPAddressControl ipAddressControl1;
      private System.Windows.Forms.Label label1;
      private System.Windows.Forms.TextBox textBox1;
   }
}

