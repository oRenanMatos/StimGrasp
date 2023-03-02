
namespace StimGraspUI
{
    partial class Form1
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.panel1 = new System.Windows.Forms.Panel();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button3 = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.UART1 = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.btnTeste = new System.Windows.Forms.Button();
            this.btnCM = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.button10 = new System.Windows.Forms.Button();
            this.button11 = new System.Windows.Forms.Button();
            this.button12 = new System.Windows.Forms.Button();
            this.button13 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.cartesianChart2 = new LiveCharts.WinForms.CartesianChart();
            this.cartesianChart1 = new LiveCharts.WinForms.CartesianChart();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(246)))), ((int)(((byte)(42)))), ((int)(((byte)(102)))));
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.pictureBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(250, 811);
            this.panel1.TabIndex = 0;
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(246)))), ((int)(((byte)(42)))), ((int)(((byte)(102)))));
            this.button2.FlatAppearance.BorderSize = 0;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Font = new System.Drawing.Font("Gotham Rounded Book", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.ForeColor = System.Drawing.Color.White;
            this.button2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button2.Location = new System.Drawing.Point(0, 253);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(250, 50);
            this.button2.TabIndex = 1;
            this.button2.Text = "Rampa de Subida";
            this.button2.UseVisualStyleBackColor = false;
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(217)))), ((int)(((byte)(51)))));
            this.button1.FlatAppearance.BorderSize = 0;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Font = new System.Drawing.Font("Gotham Rounded Book", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.ForeColor = System.Drawing.Color.White;
            this.button1.Image = global::StimGraspUI.Properties.Resources.icon_home;
            this.button1.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button1.Location = new System.Drawing.Point(0, 197);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(250, 50);
            this.button1.TabIndex = 1;
            this.button1.Text = "Home";
            this.button1.UseVisualStyleBackColor = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::StimGraspUI.Properties.Resources.flash;
            this.pictureBox1.Location = new System.Drawing.Point(61, 24);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(128, 128);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(1302, 52);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(111, 44);
            this.button3.TabIndex = 2;
            this.button3.Text = "Enviar";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.PortName = "COM8";
            // 
            // UART1
            // 
            this.UART1.Location = new System.Drawing.Point(1029, 336);
            this.UART1.Multiline = true;
            this.UART1.Name = "UART1";
            this.UART1.Size = new System.Drawing.Size(204, 142);
            this.UART1.TabIndex = 4;
            // 
            // timer1
            // 
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // timer2
            // 
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // btnTeste
            // 
            this.btnTeste.Location = new System.Drawing.Point(984, 52);
            this.btnTeste.Name = "btnTeste";
            this.btnTeste.Size = new System.Drawing.Size(208, 65);
            this.btnTeste.TabIndex = 6;
            this.btnTeste.Text = "Teste";
            this.btnTeste.UseVisualStyleBackColor = true;
            this.btnTeste.Click += new System.EventHandler(this.btnTeste_Click);
            // 
            // btnCM
            // 
            this.btnCM.Location = new System.Drawing.Point(984, 135);
            this.btnCM.Name = "btnCM";
            this.btnCM.Size = new System.Drawing.Size(208, 65);
            this.btnCM.TabIndex = 6;
            this.btnCM.Text = "Resposta padrão";
            this.btnCM.UseVisualStyleBackColor = true;
            this.btnCM.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(1273, 118);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(140, 43);
            this.button4.TabIndex = 6;
            this.button4.Text = "Rampa de Subida";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(1273, 167);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(140, 43);
            this.button5.TabIndex = 6;
            this.button5.Text = "Rampa de Descida";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(1273, 216);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(140, 43);
            this.button6.TabIndex = 6;
            this.button6.Text = "Abertura da Mão";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(1273, 265);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(140, 43);
            this.button7.TabIndex = 6;
            this.button7.Text = "Preensão Palmar (Estágio 01)";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(1273, 314);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(140, 43);
            this.button8.TabIndex = 6;
            this.button8.Text = "Preensão Palmar (Estágio 02)";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(1273, 363);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(140, 43);
            this.button9.TabIndex = 6;
            this.button9.Text = "Preensão Lateral (Estágio 01)";
            this.button9.UseVisualStyleBackColor = true;
            this.button9.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(1273, 412);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(140, 43);
            this.button10.TabIndex = 6;
            this.button10.Text = "Preensão Lateral (Estágio 02)";
            this.button10.UseVisualStyleBackColor = true;
            this.button10.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(1273, 461);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(140, 43);
            this.button11.TabIndex = 6;
            this.button11.Text = "Extensão do Indicador  (Estágio 01)";
            this.button11.UseVisualStyleBackColor = true;
            this.button11.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(1273, 510);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(140, 43);
            this.button12.TabIndex = 6;
            this.button12.Text = "Extensão do Indicador  (Estágio 02)";
            this.button12.UseVisualStyleBackColor = true;
            this.button12.Click += new System.EventHandler(this.btnCM_Click);
            // 
            // button13
            // 
            this.button13.Location = new System.Drawing.Point(1020, 216);
            this.button13.Name = "button13";
            this.button13.Size = new System.Drawing.Size(140, 43);
            this.button13.TabIndex = 6;
            this.button13.Text = "Inicia Abertura da Mão";
            this.button13.UseVisualStyleBackColor = true;
            this.button13.Click += new System.EventHandler(this.button13_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(1029, 288);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(149, 20);
            this.textBox1.TabIndex = 7;
            // 
            // cartesianChart2
            // 
            this.cartesianChart2.Location = new System.Drawing.Point(276, 24);
            this.cartesianChart2.Name = "cartesianChart2";
            this.cartesianChart2.Size = new System.Drawing.Size(711, 320);
            this.cartesianChart2.TabIndex = 8;
            this.cartesianChart2.Text = "cartesianChart2";
            // 
            // cartesianChart1
            // 
            this.cartesianChart1.Location = new System.Drawing.Point(289, 350);
            this.cartesianChart1.Name = "cartesianChart1";
            this.cartesianChart1.Size = new System.Drawing.Size(711, 320);
            this.cartesianChart1.TabIndex = 8;
            this.cartesianChart1.Text = "cartesianChart2";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(73)))), ((int)(((byte)(85)))));
            this.ClientSize = new System.Drawing.Size(1454, 811);
            this.Controls.Add(this.cartesianChart1);
            this.Controls.Add(this.cartesianChart2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button12);
            this.Controls.Add(this.button11);
            this.Controls.Add(this.button10);
            this.Controls.Add(this.button9);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button13);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.btnCM);
            this.Controls.Add(this.btnTeste);
            this.Controls.Add(this.UART1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.panel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "StimGrasp";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.TextBox UART1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Button btnTeste;
        private System.Windows.Forms.Button btnCM;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.Button button13;
        private System.Windows.Forms.TextBox textBox1;
        private LiveCharts.WinForms.CartesianChart cartesianChart2;
        private LiveCharts.WinForms.CartesianChart cartesianChart1;
    }
}

