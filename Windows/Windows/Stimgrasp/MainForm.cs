/*
 * Created by SharpDevelop.
 * User: Renato
 * Date: 17/11/2016
 * Time: 22:38
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Stimgrasp
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		
		String[] serial = { "0000", "0025", "0050", "0075", "0100", "0125", "0150", "0175", "0200", "0225", "0250", "0275", "0300", "0325", "0350", "0375", "0400", "0425", "0450", "0475", "0500", "0525", "0550", "0575", "0600", "0625", "0650", "0675", "0700", "0725", "0750", "0775", "0800", "0825", "0850", "0875", "0900", "0925", "0950", "0975", "1000", "1025", "1050", "1075", "1100", "1125", "1150", "1175", "1200", "1225", "1250", "1275", "1300", "1325", "1350", "1375", "1400", "1425", "1450", "1475", "1500", "1525", "1550", "1575", "1600", "1625", "1650", "1675", "1700", "1725", "1750", "1775", "1800", "1825", "1850", "1875", "1900", "1925", "1950", "1975", "2000" };

        String[] correntes = {"0,0mA", "0,6mA ", "1,2mA", "1,8mA", "2,4mA", "3,0mA", "3,6mA", "4,2mA", "4,8mA", "5,4mA", "6,0mA", "6,6mA", "7,2mA", "7,8mA", "8,4mA", "9,0mA", "9,6mA", "10,2mA", "10,8mA", "11,4mA", "12,0mA", "12,6mA", "13,2mA", "13,8mA", "14,4mA", "15,0mA", "15,6mA", "16,2mA", "16,8mA", "17,4mA", "18,0mA", "18,6mA", "19,2mA", "19,8mA", "20,4mA", "21,0mA", "21,6mA", "22,2mA", "22,8mA", "23,4mA", "24,0mA", "24,6mA", "25,2mA", "25,8mA", "26,4mA", "27,0mA", "27,6mA", "28,2mA", "28,8mA", "29,4mA", "30,0mA", "30,6mA", "31,2mA", "31,8mA", "32,4mA", "33,0mA", "33,6mA", "34,2mA", "34,8mA", "35,4mA", "36,0mA", "36,6mA", "37,2mA", "37,8mA", "38,4mA", "39,0mA", "39,6mA", "40,2mA", "40,8mA", "41,4mA", "42,0mA", "42,6mA", "43,2mA", "43,8mA", "44,4mA", "45,0mA", "45,6mA", "46,2mA", "46,8mA", "47,4mA", "48,0mA" };
        
        String[] serialrampa = { "0250", "0500", "0750", "1000", "1250", "1500", "1750", "2000", "2250", "2500", "2750", "3000", "3250", "3500", "3750", "4000"};

        String[] rampas = {"0,25s", "0,50s ", "0,75s", "1,00s", "1,25s", "1,50s", "1,75s", "2,00s", "2,25s", "2,50s", "2,75s", "3,00s", "3,25s", "3,50s", "3,75s", "4,00s"};
        
        int IndiceAmp1Valor2,IndiceAmp2Valor2,IndiceAmp3Valor2,IndiceAmp4Valor2,IndiceAmp5Valor2,IndiceAmp6Valor2,IndiceAmp7Valor2,IndiceAmp8Valor2;
		string ConteudoAmp1Valor2,ConteudoAmp2Valor2,ConteudoAmp3Valor2,ConteudoAmp4Valor2,ConteudoAmp5Valor2,ConteudoAmp6Valor2,ConteudoAmp7Valor2,ConteudoAmp8Valor2;

        int IndiceAmp1Valor3,IndiceAmp2Valor3,IndiceAmp3Valor3,IndiceAmp4Valor3,IndiceAmp5Valor3,IndiceAmp6Valor3,IndiceAmp7Valor3,IndiceAmp8Valor3;
		string ConteudoAmp1Valor3,ConteudoAmp2Valor3,ConteudoAmp3Valor3,ConteudoAmp4Valor3,ConteudoAmp5Valor3,ConteudoAmp6Valor3,ConteudoAmp7Valor3,ConteudoAmp8Valor3;

        int IndiceAmp1Valor4,IndiceAmp2Valor4,IndiceAmp3Valor4,IndiceAmp4Valor4,IndiceAmp5Valor4,IndiceAmp6Valor4,IndiceAmp7Valor4,IndiceAmp8Valor4;
		string ConteudoAmp1Valor4,ConteudoAmp2Valor4,ConteudoAmp3Valor4,ConteudoAmp4Valor4,ConteudoAmp5Valor4,ConteudoAmp6Valor4,ConteudoAmp7Valor4,ConteudoAmp8Valor4;
		
        int IndiceAmp1Valor5,IndiceAmp2Valor5,IndiceAmp3Valor5,IndiceAmp4Valor5,IndiceAmp5Valor5,IndiceAmp6Valor5,IndiceAmp7Valor5,IndiceAmp8Valor5;
		string ConteudoAmp1Valor5,ConteudoAmp2Valor5,ConteudoAmp3Valor5,ConteudoAmp4Valor5,ConteudoAmp5Valor5,ConteudoAmp6Valor5,ConteudoAmp7Valor5,ConteudoAmp8Valor5;
		
        int IndiceAmp1Valor6,IndiceAmp2Valor6,IndiceAmp3Valor6,IndiceAmp4Valor6,IndiceAmp5Valor6,IndiceAmp6Valor6,IndiceAmp7Valor6,IndiceAmp8Valor6;
		string ConteudoAmp1Valor6,ConteudoAmp2Valor6,ConteudoAmp3Valor6,ConteudoAmp4Valor6,ConteudoAmp5Valor6,ConteudoAmp6Valor6,ConteudoAmp7Valor6,ConteudoAmp8Valor6;
		
		int IndiceAmp1Valor7,IndiceAmp2Valor7,IndiceAmp3Valor7,IndiceAmp4Valor7,IndiceAmp5Valor7,IndiceAmp6Valor7,IndiceAmp7Valor7,IndiceAmp8Valor7;
		string ConteudoAmp1Valor7,ConteudoAmp2Valor7,ConteudoAmp3Valor7,ConteudoAmp4Valor7,ConteudoAmp5Valor7,ConteudoAmp6Valor7,ConteudoAmp7Valor7,ConteudoAmp8Valor7;
		
		int IndiceAmp1Valor8,IndiceAmp2Valor8,IndiceAmp3Valor8,IndiceAmp4Valor8,IndiceAmp5Valor8,IndiceAmp6Valor8,IndiceAmp7Valor8,IndiceAmp8Valor8;
		string ConteudoAmp1Valor8,ConteudoAmp2Valor8,ConteudoAmp3Valor8,ConteudoAmp4Valor8,ConteudoAmp5Valor8,ConteudoAmp6Valor8,ConteudoAmp7Valor8,ConteudoAmp8Valor8;
		
		int IndiceAmp1Comando3,IndiceAmp2Comando3,IndiceAmp3Comando3,IndiceAmp4Comando3,IndiceAmp5Comando3,IndiceAmp6Comando3,IndiceAmp7Comando3,IndiceAmp8Comando3;
		string ConteudoAmp1Comando3,ConteudoAmp2Comando3,ConteudoAmp3Comando3,ConteudoAmp4Comando3,ConteudoAmp5Comando3,ConteudoAmp6Comando3,ConteudoAmp7Comando3,ConteudoAmp8Comando3;

		int IndiceTmp1Valor0,IndiceTmp2Valor0,IndiceTmp3Valor0,IndiceTmp4Valor0,IndiceTmp5Valor0,IndiceTmp6Valor0,IndiceTmp7Valor0,IndiceTmp8Valor0;
		string ConteudoTmp1Valor0,ConteudoTmp2Valor0,ConteudoTmp3Valor0,ConteudoTmp4Valor0,ConteudoTmp5Valor0,ConteudoTmp6Valor0,ConteudoTmp7Valor0,ConteudoTmp8Valor0;

		int IndiceTmp1Valor1,IndiceTmp2Valor1,IndiceTmp3Valor1,IndiceTmp4Valor1,IndiceTmp5Valor1,IndiceTmp6Valor1,IndiceTmp7Valor1,IndiceTmp8Valor1;
		string ConteudoTmp1Valor1,ConteudoTmp2Valor1,ConteudoTmp3Valor1,ConteudoTmp4Valor1,ConteudoTmp5Valor1,ConteudoTmp6Valor1,ConteudoTmp7Valor1,ConteudoTmp8Valor1;
		
		
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
			

			Tempo1Valor0.Items.AddRange(rampas); Tempo1Valor0.SelectedIndex = 5;
			Tempo2Valor0.Items.AddRange(rampas); Tempo2Valor0.SelectedIndex = 5;
			Tempo3Valor0.Items.AddRange(rampas); Tempo3Valor0.SelectedIndex = 5;
			Tempo4Valor0.Items.AddRange(rampas); Tempo4Valor0.SelectedIndex = 5;
			Tempo5Valor0.Items.AddRange(rampas); Tempo5Valor0.SelectedIndex = 5;
			Tempo6Valor0.Items.AddRange(rampas); Tempo6Valor0.SelectedIndex = 5;
			Tempo7Valor0.Items.AddRange(rampas); Tempo7Valor0.SelectedIndex = 5;
			Tempo8Valor0.Items.AddRange(rampas); Tempo8Valor0.SelectedIndex = 5;

			Tempo1Valor1.Items.AddRange(rampas); Tempo1Valor1.SelectedIndex = 1;
			Tempo2Valor1.Items.AddRange(rampas); Tempo2Valor1.SelectedIndex = 1;
			Tempo3Valor1.Items.AddRange(rampas); Tempo3Valor1.SelectedIndex = 1;
			Tempo4Valor1.Items.AddRange(rampas); Tempo4Valor1.SelectedIndex = 1;
			Tempo5Valor1.Items.AddRange(rampas); Tempo5Valor1.SelectedIndex = 1;
			Tempo6Valor1.Items.AddRange(rampas); Tempo6Valor1.SelectedIndex = 1;
			Tempo7Valor1.Items.AddRange(rampas); Tempo7Valor1.SelectedIndex = 1;
			Tempo8Valor1.Items.AddRange(rampas); Tempo8Valor1.SelectedIndex = 1;
		
			Amp1Comando3.Items.AddRange(correntes); Amp1Comando3.SelectedIndex = 0;
			Amp2Comando3.Items.AddRange(correntes); Amp2Comando3.SelectedIndex = 0;
			Amp3Comando3.Items.AddRange(correntes); Amp3Comando3.SelectedIndex = 0;			
			Amp4Comando3.Items.AddRange(correntes); Amp4Comando3.SelectedIndex = 0;			
			Amp5Comando3.Items.AddRange(correntes); Amp5Comando3.SelectedIndex = 0;			
			Amp6Comando3.Items.AddRange(correntes); Amp6Comando3.SelectedIndex = 0;			
			Amp7Comando3.Items.AddRange(correntes); Amp7Comando3.SelectedIndex = 0;			
			Amp8Comando3.Items.AddRange(correntes); Amp8Comando3.SelectedIndex = 0;			
			
			Amp1Valor2.Items.AddRange(correntes); Amp1Valor2.SelectedIndex = 16;
			Amp2Valor2.Items.AddRange(correntes); Amp2Valor2.SelectedIndex = 16;
			Amp3Valor2.Items.AddRange(correntes); Amp3Valor2.SelectedIndex = 0;
			Amp4Valor2.Items.AddRange(correntes); Amp4Valor2.SelectedIndex = 10;
			Amp5Valor2.Items.AddRange(correntes); Amp5Valor2.SelectedIndex = 0;
			Amp6Valor2.Items.AddRange(correntes); Amp6Valor2.SelectedIndex = 0;
			Amp7Valor2.Items.AddRange(correntes); Amp7Valor2.SelectedIndex = 0;
			Amp8Valor2.Items.AddRange(correntes); Amp8Valor2.SelectedIndex = 0;
			
			Amp1Valor3.Items.AddRange(correntes); Amp1Valor3.SelectedIndex = 15;
			Amp2Valor3.Items.AddRange(correntes); Amp2Valor3.SelectedIndex = 0;
			Amp3Valor3.Items.AddRange(correntes); Amp3Valor3.SelectedIndex = 4;
			Amp4Valor3.Items.AddRange(correntes); Amp4Valor3.SelectedIndex = 6;
			Amp5Valor3.Items.AddRange(correntes); Amp5Valor3.SelectedIndex = 0;
			Amp6Valor3.Items.AddRange(correntes); Amp6Valor3.SelectedIndex = 0;
			Amp7Valor3.Items.AddRange(correntes); Amp7Valor3.SelectedIndex = 0;
			Amp8Valor3.Items.AddRange(correntes); Amp8Valor3.SelectedIndex = 0;	
			
			Amp1Valor4.Items.AddRange(correntes); Amp1Valor4.SelectedIndex = 15;
			Amp2Valor4.Items.AddRange(correntes); Amp2Valor4.SelectedIndex = 0;
			Amp3Valor4.Items.AddRange(correntes); Amp3Valor4.SelectedIndex = 4;
			Amp4Valor4.Items.AddRange(correntes); Amp4Valor4.SelectedIndex = 6;
			Amp5Valor4.Items.AddRange(correntes); Amp5Valor4.SelectedIndex = 4;
			Amp6Valor4.Items.AddRange(correntes); Amp6Valor4.SelectedIndex = 4;
			Amp7Valor4.Items.AddRange(correntes); Amp7Valor4.SelectedIndex = 0;
			Amp8Valor4.Items.AddRange(correntes); Amp8Valor4.SelectedIndex = 0;
			
			Amp1Valor5.Items.AddRange(correntes); Amp1Valor5.SelectedIndex = 15;
			Amp2Valor5.Items.AddRange(correntes); Amp2Valor5.SelectedIndex = 0;
			Amp3Valor5.Items.AddRange(correntes); Amp3Valor5.SelectedIndex = 0;
			Amp4Valor5.Items.AddRange(correntes); Amp4Valor5.SelectedIndex = 10;
			Amp5Valor5.Items.AddRange(correntes); Amp5Valor5.SelectedIndex = 16;
			Amp6Valor5.Items.AddRange(correntes); Amp6Valor5.SelectedIndex = 0;
			Amp7Valor5.Items.AddRange(correntes); Amp7Valor5.SelectedIndex = 0;
			Amp8Valor5.Items.AddRange(correntes); Amp8Valor5.SelectedIndex = 0;
			
			Amp1Valor6.Items.AddRange(correntes); Amp1Valor6.SelectedIndex = 15;
			Amp2Valor6.Items.AddRange(correntes); Amp2Valor6.SelectedIndex = 0;
			Amp3Valor6.Items.AddRange(correntes); Amp3Valor6.SelectedIndex = 0;
			Amp4Valor6.Items.AddRange(correntes); Amp4Valor6.SelectedIndex = 0;
			Amp5Valor6.Items.AddRange(correntes); Amp5Valor6.SelectedIndex = 16;
			Amp6Valor6.Items.AddRange(correntes); Amp6Valor6.SelectedIndex = 4;
			Amp7Valor6.Items.AddRange(correntes); Amp7Valor6.SelectedIndex = 0;
			Amp8Valor6.Items.AddRange(correntes); Amp8Valor6.SelectedIndex = 0;
			
			Amp1Valor7.Items.AddRange(correntes); Amp1Valor7.SelectedIndex = 0;
			Amp2Valor7.Items.AddRange(correntes); Amp2Valor7.SelectedIndex = 0;
			Amp3Valor7.Items.AddRange(correntes); Amp3Valor7.SelectedIndex = 0;
			Amp4Valor7.Items.AddRange(correntes); Amp4Valor7.SelectedIndex = 0;
			Amp5Valor7.Items.AddRange(correntes); Amp5Valor7.SelectedIndex = 10;
			Amp6Valor7.Items.AddRange(correntes); Amp6Valor7.SelectedIndex = 0;
			Amp7Valor7.Items.AddRange(correntes); Amp7Valor7.SelectedIndex = 0;
			Amp8Valor7.Items.AddRange(correntes); Amp8Valor7.SelectedIndex = 0;	
			
			Amp1Valor8.Items.AddRange(correntes); Amp1Valor8.SelectedIndex = 0;
			Amp2Valor8.Items.AddRange(correntes); Amp2Valor8.SelectedIndex = 0;
			Amp3Valor8.Items.AddRange(correntes); Amp3Valor8.SelectedIndex = 0;
			Amp4Valor8.Items.AddRange(correntes); Amp4Valor8.SelectedIndex = 0;
			Amp5Valor8.Items.AddRange(correntes); Amp5Valor8.SelectedIndex = 10;
			Amp6Valor8.Items.AddRange(correntes); Amp6Valor8.SelectedIndex = 4;
			Amp7Valor8.Items.AddRange(correntes); Amp7Valor8.SelectedIndex = 0;
			Amp8Valor8.Items.AddRange(correntes); Amp8Valor8.SelectedIndex = 0;
			
		
			
		}
		

		
		
		
		
		
		
		void MainFormLoad(object sender, EventArgs e)
		{
			//
			string[] ports = System.IO.Ports.SerialPort.GetPortNames();
       		cboPort.Items.AddRange(ports);
       		cboPort.SelectedIndex = 0;
       		
       		btnOpen.Enabled = true;
			btnClose.Enabled = false;
			
			groupBox2.Enabled = false;
			groupBox3.Enabled = false;
			groupBox4.Enabled = false;
	
			
			
			
		/*	Tempo1Valor0.SelectedIndex = 4;
			Tempo2Valor0.SelectedIndex = 4;
			Tempo3Valor0.SelectedIndex = 4;
			Tempo4Valor0.SelectedIndex = 4;
			Tempo5Valor0.SelectedIndex = 4;
			Tempo6Valor0.SelectedIndex = 4;
			Tempo7Valor0.SelectedIndex = 4;
			Tempo8Valor0.SelectedIndex = 4;			
			
			Tempo1Valor1.SelectedIndex = 1;
			Tempo2Valor1.SelectedIndex = 1;
			Tempo3Valor1.SelectedIndex = 1;
			Tempo4Valor1.SelectedIndex = 1;
			Tempo5Valor1.SelectedIndex = 1;
			Tempo6Valor1.SelectedIndex = 1;
			Tempo7Valor1.SelectedIndex = 1;
			Tempo8Valor1.SelectedIndex = 1;	
			
			Amp1Comando3.SelectedIndex = 0;
			Amp2Comando3.SelectedIndex = 0;
			Amp3Comando3.SelectedIndex = 0;			
			Amp4Comando3.SelectedIndex = 0;			
			Amp5Comando3.SelectedIndex = 0;			
			Amp6Comando3.SelectedIndex = 0;			
			Amp7Comando3.SelectedIndex = 0;			
			Amp8Comando3.SelectedIndex = 0;			
			
			Amp1Valor2.SelectedIndex = 16;
			Amp2Valor2.SelectedIndex = 16;
			Amp3Valor2.SelectedIndex = 0;
			Amp4Valor2.SelectedIndex = 10;
			Amp5Valor2.SelectedIndex = 0;
			Amp6Valor2.SelectedIndex = 0;
			Amp7Valor2.SelectedIndex = 0;
			Amp8Valor2.SelectedIndex =	0;
			
			Amp1Valor3.SelectedIndex = 15;
			Amp2Valor3.SelectedIndex = 0;
			Amp3Valor3.SelectedIndex = 4;
			Amp4Valor3.SelectedIndex = 6;
			Amp5Valor3.SelectedIndex = 0;
			Amp6Valor3.SelectedIndex = 0;
			Amp7Valor3.SelectedIndex = 0;
			Amp8Valor3.SelectedIndex = 0;	
			
			Amp1Valor4.SelectedIndex = 15;
			Amp2Valor4.SelectedIndex = 0;
			Amp3Valor4.SelectedIndex = 4;
			Amp4Valor4.SelectedIndex = 6;
			Amp5Valor4.SelectedIndex = 4;
			Amp6Valor4.SelectedIndex = 4;
			Amp7Valor4.SelectedIndex = 0;
			Amp8Valor4.SelectedIndex = 0;
			
			Amp1Valor5.SelectedIndex = 15;
			Amp2Valor5.SelectedIndex = 0;
			Amp3Valor5.SelectedIndex = 0;
			Amp4Valor5.SelectedIndex = 10;
			Amp5Valor5.SelectedIndex = 16;
			Amp6Valor5.SelectedIndex = 0;
			Amp7Valor5.SelectedIndex = 0;
			Amp8Valor5.SelectedIndex = 0;
			
			Amp1Valor6.SelectedIndex = 15;
			Amp2Valor6.SelectedIndex = 0;
			Amp3Valor6.SelectedIndex = 0;
			Amp4Valor6.SelectedIndex = 0;
			Amp5Valor6.SelectedIndex = 16;
			Amp6Valor6.SelectedIndex = 4;
		    Amp7Valor6.SelectedIndex = 0;
			Amp8Valor6.SelectedIndex =	0;
			
			Amp1Valor7.SelectedIndex = 0;
			Amp2Valor7.SelectedIndex = 0;
			Amp3Valor7.SelectedIndex = 0;
			Amp4Valor7.SelectedIndex = 0;
			Amp5Valor7.SelectedIndex = 10;
			Amp6Valor7.SelectedIndex = 0;
			Amp7Valor7.SelectedIndex = 0;
			Amp8Valor7.SelectedIndex = 0;	
			
			Amp1Valor8.SelectedIndex = 0;
			Amp1Valor8.SelectedIndex = 0;
			Amp1Valor8.SelectedIndex = 0;
			Amp1Valor8.SelectedIndex = 0;
			Amp1Valor8.SelectedIndex = 10;
			Amp1Valor8.SelectedIndex = 4;
			Amp1Valor8.SelectedIndex = 0;
			Amp1Valor8.SelectedIndex = 0; */	
			
			
			/*this.RampaSubida1.Text = "3000";
			this.RampaSubida2.Text = "3000";
			this.RampaSubida3.Text = "3000";
			this.RampaSubida4.Text = "3000";
			this.RampaSubida5.Text = "3000";
			this.RampaSubida6.Text = "3000";
			this.RampaSubida7.Text = "3000";
			this.RampaSubida8.Text = "3000";			
		
			this.AmpCanal1.Text= "0500";
			this.AmpCanal2.Text= "0500";
			this.AmpCanal3.Text= "0500";
			this.AmpCanal4.Text= "0500";
			this.AmpCanal5.Text= "0500";
			this.AmpCanal6.Text= "0500";
			this.AmpCanal7.Text= "0500";
			this.AmpCanal8.Text= "0500";
			
			this.RampaDescida1.Text = "1000";
			this.RampaDescida2.Text = "1000";			
			this.RampaDescida3.Text = "1000";			
			this.RampaDescida4.Text = "1000";
			this.RampaDescida5.Text = "1000";
			this.RampaDescida6.Text = "1000";
			this.RampaDescida7.Text = "1000";
			this.RampaDescida8.Text = "1000";			
			
			this.RampaSubida1.Text = "3000";
			this.RampaSubida2.Text = "3000";
			this.RampaSubida3.Text = "3000";
			this.RampaSubida4.Text = "3000";
			this.RampaSubida5.Text = "3000";
			this.RampaSubida6.Text = "3000";
			this.RampaSubida7.Text = "3000";
			this.RampaSubida8.Text = "3000";	*/	

			
			//String ListaDeAmplitudes = {0000,0100,0200,0300,0400,0500,0600,0700,0800,0900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};

		/*	   MessageBox.Show ("Ajuda do Stimgrasp"+"\n"+"\n"+
								"1 - Conecte o Stimgrasp à porta USB e ligue-o;"+"\n"+
								"2 - Ajuste as amplitudes e rampas a cada usuário novo;" +"\n"+
			                    "3 - Após configurar, controlar a estimulação com os botões de comando"+"\n"+"\n"+
			                    "Amiguinhos forever S2"
								, "Stimgrasp 1.2",
				MessageBoxButtons.OK, MessageBoxIcon.Asterisk);*/
			
			
		}
		
		void BtnOpenClick(object sender, EventArgs e)
		{

			
			try 
			{
				serialPort1.PortName = cboPort.Text;
				serialPort1.Open();
				
				btnOpen.Enabled = false;
				btnClose.Enabled = true;
				groupBox2.Enabled = true;
				groupBox3.Enabled = true;
				groupBox4.Enabled = true;
				
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
			
		}
		
		void BtnCloseClick(object sender, EventArgs e)
		{
			try 
			{
				serialPort1.Close();

				btnOpen.Enabled = true;
				btnClose.Enabled = false;
				groupBox2.Enabled = false;	
				groupBox3.Enabled = false;
				groupBox4.Enabled = false;
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		
		void MainFormFormClosing(object sender, FormClosingEventArgs e)
		{
			if(serialPort1.IsOpen)
			{
				serialPort1.Close();
			}
		}
		
		
		void BtnValor0Click(object sender, EventArgs e)
		{
			IndiceTmp1Valor0 = Tempo1Valor0.SelectedIndex;
			IndiceTmp2Valor0 = Tempo2Valor0.SelectedIndex;
			IndiceTmp3Valor0 = Tempo3Valor0.SelectedIndex;
			IndiceTmp4Valor0 = Tempo4Valor0.SelectedIndex;
			IndiceTmp5Valor0 = Tempo5Valor0.SelectedIndex;
			IndiceTmp6Valor0 = Tempo6Valor0.SelectedIndex;
			IndiceTmp7Valor0 = Tempo7Valor0.SelectedIndex;
			IndiceTmp8Valor0 = Tempo8Valor0.SelectedIndex;
			
			ConteudoTmp1Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp2Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp3Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp4Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp5Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp6Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp7Valor0 = serialrampa[IndiceTmp1Valor0];
			ConteudoTmp8Valor0 = serialrampa[IndiceTmp1Valor0];

			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV00");
		          serialPort1.Write(ConteudoTmp1Valor0+
		                      		ConteudoTmp2Valor0+
		                      		ConteudoTmp3Valor0+
		                      		ConteudoTmp4Valor0+
		                      		ConteudoTmp5Valor0+
		                      		ConteudoTmp6Valor0+
		                      		ConteudoTmp7Valor0+
		                      		ConteudoTmp8Valor0);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		
		
		void BtnValor1Click(object sender, EventArgs e)
		{
			IndiceTmp1Valor1 = Tempo1Valor1.SelectedIndex;
			IndiceTmp2Valor1 = Tempo2Valor1.SelectedIndex;
			IndiceTmp3Valor1 = Tempo3Valor1.SelectedIndex;
			IndiceTmp4Valor1 = Tempo4Valor1.SelectedIndex;
			IndiceTmp5Valor1 = Tempo5Valor1.SelectedIndex;
			IndiceTmp6Valor1 = Tempo6Valor1.SelectedIndex;
			IndiceTmp7Valor1 = Tempo7Valor1.SelectedIndex;
			IndiceTmp8Valor1 = Tempo8Valor1.SelectedIndex;
			
			ConteudoTmp1Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp2Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp3Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp4Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp5Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp6Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp7Valor1 = serialrampa[IndiceTmp1Valor1];
			ConteudoTmp8Valor1 = serialrampa[IndiceTmp1Valor1];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV01");
		        //  serialPort1.Write(AmpCanal1.Text+AmpCanal2.Text+AmpCanal3.Text+AmpCanal4.Text+AmpCanal5.Text+AmpCanal6.Text+AmpCanal7.Text+AmpCanal8.Text);
		   /*       serialPort1.Write(Tempo1Valor1.Text+
		                            Tempo2Valor1.Text+
		                            Tempo3Valor1.Text+
		                            Tempo4Valor1.Text+
		                            Tempo5Valor1.Text+
		                            Tempo6Valor1.Text+
		                            Tempo7Valor1.Text+
		                            Tempo8Valor1.Text);*/
		          serialPort1.Write(ConteudoTmp1Valor1+
		                      		ConteudoTmp2Valor1+
		                      		ConteudoTmp3Valor1+
		                      		ConteudoTmp4Valor1+
		                      		ConteudoTmp5Valor1+
		                      		ConteudoTmp6Valor1+
		                      		ConteudoTmp7Valor1+
		                      		ConteudoTmp8Valor1);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor2Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor2 = Amp1Valor2.SelectedIndex;
			IndiceAmp2Valor2 = Amp2Valor2.SelectedIndex;
			IndiceAmp3Valor2 = Amp3Valor2.SelectedIndex;
			IndiceAmp4Valor2 = Amp4Valor2.SelectedIndex;
			IndiceAmp5Valor2 = Amp5Valor2.SelectedIndex;
			IndiceAmp6Valor2 = Amp6Valor2.SelectedIndex;
			IndiceAmp7Valor2 = Amp7Valor2.SelectedIndex;
			IndiceAmp8Valor2 = Amp8Valor2.SelectedIndex;
			
			ConteudoAmp1Valor2 = serial[IndiceAmp1Valor2];
			ConteudoAmp2Valor2 = serial[IndiceAmp2Valor2];
			ConteudoAmp3Valor2 = serial[IndiceAmp3Valor2];
			ConteudoAmp4Valor2 = serial[IndiceAmp4Valor2];
			ConteudoAmp5Valor2 = serial[IndiceAmp5Valor2];
			ConteudoAmp6Valor2 = serial[IndiceAmp6Valor2];
			ConteudoAmp7Valor2 = serial[IndiceAmp7Valor2];
			ConteudoAmp8Valor2 = serial[IndiceAmp8Valor2];			
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV02");
		         
		          serialPort1.Write(ConteudoAmp1Valor2+
		                      		ConteudoAmp2Valor2+
		                      		ConteudoAmp3Valor2+
		                      		ConteudoAmp4Valor2+
		                      		ConteudoAmp5Valor2+
		                      		ConteudoAmp6Valor2+
		                      		ConteudoAmp7Valor2+
		                      		ConteudoAmp8Valor2);
		         
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		
		
		void BtnValor3Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor3 = Amp1Valor3.SelectedIndex;
			IndiceAmp2Valor3 = Amp2Valor3.SelectedIndex;
			IndiceAmp3Valor3 = Amp3Valor3.SelectedIndex;
			IndiceAmp4Valor3 = Amp4Valor3.SelectedIndex;
			IndiceAmp5Valor3 = Amp5Valor3.SelectedIndex;
			IndiceAmp6Valor3 = Amp6Valor3.SelectedIndex;
			IndiceAmp7Valor3 = Amp7Valor3.SelectedIndex;
			IndiceAmp8Valor3 = Amp8Valor3.SelectedIndex;
			
			ConteudoAmp1Valor3 = serial[IndiceAmp1Valor3];
			ConteudoAmp2Valor3 = serial[IndiceAmp2Valor3];
			ConteudoAmp3Valor3 = serial[IndiceAmp3Valor3];
			ConteudoAmp4Valor3 = serial[IndiceAmp4Valor3];
			ConteudoAmp5Valor3 = serial[IndiceAmp5Valor3];
			ConteudoAmp6Valor3 = serial[IndiceAmp6Valor3];
			ConteudoAmp7Valor3 = serial[IndiceAmp7Valor3];
			ConteudoAmp8Valor3 = serial[IndiceAmp8Valor3];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV03");
		          serialPort1.Write(ConteudoAmp1Valor3+
		                      		ConteudoAmp2Valor3+
		                      		ConteudoAmp3Valor3+
		                      		ConteudoAmp4Valor3+
		                      		ConteudoAmp5Valor3+
		                      		ConteudoAmp6Valor3+
		                      		ConteudoAmp7Valor3+
		                      		ConteudoAmp8Valor3);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor4Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor4 = Amp1Valor4.SelectedIndex;
			IndiceAmp2Valor4 = Amp2Valor4.SelectedIndex;
			IndiceAmp3Valor4 = Amp3Valor4.SelectedIndex;
			IndiceAmp4Valor4 = Amp4Valor4.SelectedIndex;
			IndiceAmp5Valor4 = Amp5Valor4.SelectedIndex;
			IndiceAmp6Valor4 = Amp6Valor4.SelectedIndex;
			IndiceAmp7Valor4 = Amp7Valor4.SelectedIndex;
			IndiceAmp8Valor4 = Amp8Valor4.SelectedIndex;
			
			ConteudoAmp1Valor4 = serial[IndiceAmp1Valor4];
			ConteudoAmp2Valor4 = serial[IndiceAmp2Valor4];
			ConteudoAmp3Valor4 = serial[IndiceAmp3Valor4];
			ConteudoAmp4Valor4 = serial[IndiceAmp4Valor4];
			ConteudoAmp5Valor4 = serial[IndiceAmp5Valor4];
			ConteudoAmp6Valor4 = serial[IndiceAmp6Valor4];
			ConteudoAmp7Valor4 = serial[IndiceAmp7Valor4];
			ConteudoAmp8Valor4 = serial[IndiceAmp8Valor4];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV04");
		          serialPort1.Write(ConteudoAmp1Valor4+
		                      		ConteudoAmp2Valor4+
		                      		ConteudoAmp3Valor4+
		                      		ConteudoAmp4Valor4+
		                      		ConteudoAmp5Valor4+
		                      		ConteudoAmp6Valor4+
		                      		ConteudoAmp7Valor4+
		                      		ConteudoAmp8Valor4);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor5Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor5 = Amp1Valor5.SelectedIndex;
			IndiceAmp2Valor5 = Amp2Valor5.SelectedIndex;
			IndiceAmp3Valor5 = Amp3Valor5.SelectedIndex;
			IndiceAmp4Valor5 = Amp4Valor5.SelectedIndex;
			IndiceAmp5Valor5 = Amp5Valor5.SelectedIndex;
			IndiceAmp6Valor5 = Amp6Valor5.SelectedIndex;
			IndiceAmp7Valor5 = Amp7Valor5.SelectedIndex;
			IndiceAmp8Valor5 = Amp8Valor5.SelectedIndex;
			
			ConteudoAmp1Valor5 = serial[IndiceAmp1Valor5];
			ConteudoAmp2Valor5 = serial[IndiceAmp2Valor5];
			ConteudoAmp3Valor5 = serial[IndiceAmp3Valor5];
			ConteudoAmp4Valor5 = serial[IndiceAmp4Valor5];
			ConteudoAmp5Valor5 = serial[IndiceAmp5Valor5];
			ConteudoAmp6Valor5 = serial[IndiceAmp6Valor5];
			ConteudoAmp7Valor5 = serial[IndiceAmp7Valor5];
			ConteudoAmp8Valor5 = serial[IndiceAmp8Valor5];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV05");
		          serialPort1.Write(ConteudoAmp1Valor5+
		                      		ConteudoAmp2Valor5+
		                      		ConteudoAmp3Valor5+
		                      		ConteudoAmp4Valor5+
		                      		ConteudoAmp5Valor5+
		                      		ConteudoAmp6Valor5+
		                      		ConteudoAmp7Valor5+
		                      		ConteudoAmp8Valor5);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor6Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor6 = Amp1Valor6.SelectedIndex;
			IndiceAmp2Valor6 = Amp2Valor6.SelectedIndex;
			IndiceAmp3Valor6 = Amp3Valor6.SelectedIndex;
			IndiceAmp4Valor6 = Amp4Valor6.SelectedIndex;
			IndiceAmp5Valor6 = Amp5Valor6.SelectedIndex;
			IndiceAmp6Valor6 = Amp6Valor6.SelectedIndex;
			IndiceAmp7Valor6 = Amp7Valor6.SelectedIndex;
			IndiceAmp8Valor6 = Amp8Valor6.SelectedIndex;
			
			ConteudoAmp1Valor6 = serial[IndiceAmp1Valor6];
			ConteudoAmp2Valor6 = serial[IndiceAmp2Valor6];
			ConteudoAmp3Valor6 = serial[IndiceAmp3Valor6];
			ConteudoAmp4Valor6 = serial[IndiceAmp4Valor6];
			ConteudoAmp5Valor6 = serial[IndiceAmp5Valor6];
			ConteudoAmp6Valor6 = serial[IndiceAmp6Valor6];
			ConteudoAmp7Valor6 = serial[IndiceAmp7Valor6];
			ConteudoAmp8Valor6 = serial[IndiceAmp8Valor6];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV06");
		          serialPort1.Write(ConteudoAmp1Valor6+
		                      		ConteudoAmp2Valor6+
		                      		ConteudoAmp3Valor6+
		                      		ConteudoAmp4Valor6+
		                      		ConteudoAmp5Valor6+
		                      		ConteudoAmp6Valor6+
		                      		ConteudoAmp7Valor6+
		                      		ConteudoAmp8Valor6);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor7Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor7 = Amp1Valor7.SelectedIndex;
			IndiceAmp2Valor7 = Amp2Valor7.SelectedIndex;
			IndiceAmp3Valor7 = Amp3Valor7.SelectedIndex;
			IndiceAmp4Valor7 = Amp4Valor7.SelectedIndex;
			IndiceAmp5Valor7 = Amp5Valor7.SelectedIndex;
			IndiceAmp6Valor7 = Amp6Valor7.SelectedIndex;
			IndiceAmp7Valor7 = Amp7Valor7.SelectedIndex;
			IndiceAmp8Valor7 = Amp8Valor7.SelectedIndex;
			
			ConteudoAmp1Valor7 = serial[IndiceAmp1Valor7];
			ConteudoAmp2Valor7 = serial[IndiceAmp2Valor7];
			ConteudoAmp3Valor7 = serial[IndiceAmp3Valor7];
			ConteudoAmp4Valor7 = serial[IndiceAmp4Valor7];
			ConteudoAmp5Valor7 = serial[IndiceAmp5Valor7];
			ConteudoAmp6Valor7 = serial[IndiceAmp6Valor7];
			ConteudoAmp7Valor7 = serial[IndiceAmp7Valor7];
			ConteudoAmp8Valor7 = serial[IndiceAmp8Valor7];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV07");
		          serialPort1.Write(ConteudoAmp1Valor7+
		                      		ConteudoAmp2Valor7+
		                      		ConteudoAmp3Valor7+
		                      		ConteudoAmp4Valor7+
		                      		ConteudoAmp5Valor7+
		                      		ConteudoAmp6Valor7+
		                      		ConteudoAmp7Valor7+
		                      		ConteudoAmp8Valor7);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnValor8Click(object sender, EventArgs e)
		{
			IndiceAmp1Valor8 = Amp1Valor8.SelectedIndex;
			IndiceAmp2Valor8 = Amp2Valor8.SelectedIndex;
			IndiceAmp3Valor8 = Amp3Valor8.SelectedIndex;
			IndiceAmp4Valor8 = Amp4Valor8.SelectedIndex;
			IndiceAmp5Valor8 = Amp5Valor8.SelectedIndex;
			IndiceAmp6Valor8 = Amp6Valor8.SelectedIndex;
			IndiceAmp7Valor8 = Amp7Valor8.SelectedIndex;
			IndiceAmp8Valor8 = Amp8Valor8.SelectedIndex;
			
			ConteudoAmp1Valor8 = serial[IndiceAmp1Valor8];
			ConteudoAmp2Valor8 = serial[IndiceAmp2Valor8];
			ConteudoAmp3Valor8 = serial[IndiceAmp3Valor8];
			ConteudoAmp4Valor8 = serial[IndiceAmp4Valor8];
			ConteudoAmp5Valor8 = serial[IndiceAmp5Valor8];
			ConteudoAmp6Valor8 = serial[IndiceAmp6Valor8];
			ConteudoAmp7Valor8 = serial[IndiceAmp7Valor8];
			ConteudoAmp8Valor8 = serial[IndiceAmp8Valor8];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMV08");
		          serialPort1.Write(ConteudoAmp1Valor8+
		                      		ConteudoAmp2Valor8+
		                      		ConteudoAmp3Valor8+
		                      		ConteudoAmp4Valor8+
		                      		ConteudoAmp5Valor8+
		                      		ConteudoAmp6Valor8+
		                      		ConteudoAmp7Valor8+
		                      		ConteudoAmp8Valor8);
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}


		
		
		void BtnComando0Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC00xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando1Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC01xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}		
		void BtnComando2Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC02xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando3Click(object sender, EventArgs e)
		{
			IndiceAmp1Comando3 = Amp1Comando3.SelectedIndex;
			IndiceAmp2Comando3 = Amp2Comando3.SelectedIndex;
			IndiceAmp3Comando3 = Amp3Comando3.SelectedIndex;
			IndiceAmp4Comando3 = Amp4Comando3.SelectedIndex;
			IndiceAmp5Comando3 = Amp5Comando3.SelectedIndex;
			IndiceAmp6Comando3 = Amp6Comando3.SelectedIndex;
			IndiceAmp7Comando3 = Amp7Comando3.SelectedIndex;
			IndiceAmp8Comando3 = Amp8Comando3.SelectedIndex;
			
			ConteudoAmp1Comando3 = serial[IndiceAmp1Comando3];
			ConteudoAmp2Comando3 = serial[IndiceAmp2Comando3];
			ConteudoAmp3Comando3 = serial[IndiceAmp3Comando3];
			ConteudoAmp4Comando3 = serial[IndiceAmp4Comando3];
			ConteudoAmp5Comando3 = serial[IndiceAmp5Comando3];
			ConteudoAmp6Comando3 = serial[IndiceAmp6Comando3];
			ConteudoAmp7Comando3 = serial[IndiceAmp7Comando3];
			ConteudoAmp8Comando3 = serial[IndiceAmp8Comando3];
			
			try 
			{
				if(serialPort1.IsOpen)
				{
		          serialPort1.Write("<STIMC03");
		          serialPort1.Write(ConteudoAmp1Comando3+
		                      		ConteudoAmp2Comando3+
		                      		ConteudoAmp3Comando3+
		                      		ConteudoAmp4Comando3+
		                      		ConteudoAmp5Comando3+
		                      		ConteudoAmp6Comando3+
		                      		ConteudoAmp7Comando3+
		                      		ConteudoAmp8Comando3);
		          
		         /* serialPort1.Write(Amp1Comando3.Text+
		                          	Amp2Comando3.Text+
		                          	Amp3Comando3.Text+
		                          	Amp4Comando3.Text+
		                          	Amp5Comando3.Text+
		                          	Amp6Comando3.Text+
		                          	Amp7Comando3.Text+
		                          	Amp8Comando3.Text);*/
		          serialPort1.Write(">");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando4Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC04xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando5Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC05xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando6Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC06xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando7Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC07xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando8Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC08xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando9Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC09xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnComando10Click(object sender, EventArgs e)
		{
			try 
			{
				if(serialPort1.IsOpen)
				{
					serialPort1.Write("<STIMC10xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx>");
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message, "Message",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
		void BtnAjudaClick(object sender, EventArgs e)
		{
	    	Form1 frm = new Form1();
    		frm.Show();
		}


		

	}
}
