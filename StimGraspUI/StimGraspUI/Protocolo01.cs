using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StimGraspUI
{
    public partial class Protocolo0 : UserControl
    {

        private string[] tempo = { "Volvo", "BMW", "Ford", "Mazda" };
        private double[] cmbTempo = {0.25,0.50,0.75,1.00,1.25,1.50,1.75,2.00,2.25,2.50,2.75,3.00,3.25,3.50,3.75,4.00 };
        private double[] cmbAmpli = { 0.00, 0.6, 1.2, 1.8, 2.4, 3.0, 3.6, 4.2, 4.8, 5.4, 6.0, 6.6, 7.2, 7.8, 8.4, 9.0, 9.6, 10.2, 10.8, 11.4, 12.0, 12.6, 13.2, 14.4, 15.0, 15.6, 16.2, 16.8, 17.4, 18.0, 18.6, 19.2, 19.8, 20.4, 21.0, 21.6, 22.2, 22.8, 23.4, 24.0, 24.6, 25.2, 25.8, 26.4, 27.0, 27.6, 28.2, 28.8, 29.4, 30.0, 30.6, 31.2, 31.8, 32.4, 33.0, 33.6, 34.2, 34.8, 35.4, 36.0, 36.6, 37.2, 37.8, 38.4, 39.0, 39.6, 40.2, 40.8, 41.4, 42.0, 42.6, 43.2, 43.8, 44.4, 45.0, 45.6, 46.2, 46.8, 47.4, 48.0 };
        public Protocolo0()
        {
            InitializeComponent();
       
        }

        private void Protocolo01_Load(object sender, EventArgs e)
        {
            foreach (var tempo in cmbTempo)
            {
                var time = tempo.ToString("N2") + "s";
                cmbCH1RS.Items.Add(time);
                cmbCH2RS.Items.Add(time);
                cmbCH3RS.Items.Add(time);
                cmbCH4RS.Items.Add(time);
                cmbCH5RS.Items.Add(time);
                cmbCH6RS.Items.Add(time);
                cmbCH7RS.Items.Add(time);
                cmbCH8RS.Items.Add(time);
                cmbCH1RD.Items.Add(time);
                cmbCH2RD.Items.Add(time);
                cmbCH3RD.Items.Add(time);
                cmbCH4RD.Items.Add(time);
                cmbCH5RD.Items.Add(time);
                cmbCH6RD.Items.Add(time);
                cmbCH7RD.Items.Add(time);
                cmbCH8RD.Items.Add(time);
            }

            cmbCH1RS.SelectedIndex = 5;
            cmbCH2RS.SelectedIndex = 5;
            cmbCH3RS.SelectedIndex = 5;
            cmbCH4RS.SelectedIndex = 5;
            cmbCH5RS.SelectedIndex = 5;
            cmbCH6RS.SelectedIndex = 5;
            cmbCH7RS.SelectedIndex = 5;
            cmbCH8RS.SelectedIndex = 5;


            cmbCH1RD.SelectedIndex = 1;
            cmbCH2RD.SelectedIndex = 1;
            cmbCH3RD.SelectedIndex = 1;
            cmbCH4RD.SelectedIndex = 1;
            cmbCH5RD.SelectedIndex = 1;
            cmbCH6RD.SelectedIndex = 1;
            cmbCH7RD.SelectedIndex = 1;
            cmbCH8RD.SelectedIndex = 1;


            foreach (var amplitude in cmbAmpli)
            {
                var amp = amplitude.ToString("N2") + "mA";
                cmbCH1AM.Items.Add(amp);
                cmbCH1PP1.Items.Add(amp);
                cmbCH1PP2.Items.Add(amp);
                cmbCH1PL1.Items.Add(amp);
                cmbCH1PL2.Items.Add(amp);
                cmbCH1EI1.Items.Add(amp);
                cmbCH1EI2.Items.Add(amp);
                cmbCH1IE.Items.Add(amp);

                cmbCH2AM.Items.Add(amp);
                cmbCH2PP1.Items.Add(amp);
                cmbCH2PP2.Items.Add(amp);
                cmbCH2PL1.Items.Add(amp);
                cmbCH2PL2.Items.Add(amp);
                cmbCH2EI1.Items.Add(amp);
                cmbCH2EI2.Items.Add(amp);
                cmbCH2IE.Items.Add(amp);

                cmbCH3AM.Items.Add(amp);
                cmbCH3PP1.Items.Add(amp);
                cmbCH3PP2.Items.Add(amp);
                cmbCH3PL1.Items.Add(amp);
                cmbCH3PL2.Items.Add(amp);
                cmbCH3EI1.Items.Add(amp);
                cmbCH3EI2.Items.Add(amp);
                cmbCH3IE.Items.Add(amp);

                cmbCH4AM.Items.Add(amp);
                cmbCH4PP1.Items.Add(amp);
                cmbCH4PP2.Items.Add(amp);
                cmbCH4PL1.Items.Add(amp);
                cmbCH4PL2.Items.Add(amp);
                cmbCH4EI1.Items.Add(amp);
                cmbCH4EI2.Items.Add(amp);
                cmbCH4IE.Items.Add(amp);

                cmbCH5AM.Items.Add(amp);
                cmbCH5PP1.Items.Add(amp);
                cmbCH5PP2.Items.Add(amp);
                cmbCH5PL1.Items.Add(amp);
                cmbCH5PL2.Items.Add(amp);
                cmbCH5EI1.Items.Add(amp);
                cmbCH5EI2.Items.Add(amp);
                cmbCH5IE.Items.Add(amp);

                cmbCH6AM.Items.Add(amp);
                cmbCH6PP1.Items.Add(amp);
                cmbCH6PP2.Items.Add(amp);
                cmbCH6PL1.Items.Add(amp);
                cmbCH6PL2.Items.Add(amp);
                cmbCH6EI1.Items.Add(amp);
                cmbCH6EI2.Items.Add(amp);
                cmbCH6IE.Items.Add(amp);

                cmbCH7AM.Items.Add(amp);
                cmbCH7PP1.Items.Add(amp);
                cmbCH7PP2.Items.Add(amp);
                cmbCH7PL1.Items.Add(amp);
                cmbCH7PL2.Items.Add(amp);
                cmbCH7EI1.Items.Add(amp);
                cmbCH7EI2.Items.Add(amp);
                cmbCH7IE.Items.Add(amp);

                cmbCH8AM.Items.Add(amp);
                cmbCH8PP1.Items.Add(amp);
                cmbCH8PP2.Items.Add(amp);
                cmbCH8PL1.Items.Add(amp);
                cmbCH8PL2.Items.Add(amp);
                cmbCH8EI1.Items.Add(amp);
                cmbCH8EI2.Items.Add(amp);
                cmbCH8IE.Items.Add(amp);


                cmbCH1AM.SelectedIndex = 0;
                cmbCH1PP1.SelectedIndex = 0;
                cmbCH1PP2.SelectedIndex = 0;
                cmbCH1PL1.SelectedIndex = 0;
                cmbCH1PL2.SelectedIndex = 0;
                cmbCH1EI1.SelectedIndex = 0;
                cmbCH1EI2.SelectedIndex = 0;
                cmbCH1IE.SelectedIndex = 0;


                cmbCH2AM.SelectedIndex = 0;
                cmbCH2PP1.SelectedIndex = 0;
                cmbCH2PP2.SelectedIndex = 0;
                cmbCH2PL1.SelectedIndex = 0;
                cmbCH2PL2.SelectedIndex = 0;
                cmbCH2EI1.SelectedIndex = 0;
                cmbCH2EI2.SelectedIndex = 0;
                cmbCH2IE.SelectedIndex = 0;

                cmbCH3AM.SelectedIndex = 0;
                cmbCH3PP1.SelectedIndex = 0;
                cmbCH3PP2.SelectedIndex = 0;
                cmbCH3PL1.SelectedIndex = 0;
                cmbCH3PL2.SelectedIndex = 0;
                cmbCH3EI1.SelectedIndex = 0;
                cmbCH3EI2.SelectedIndex = 0;
                cmbCH3IE.SelectedIndex = 0;


                cmbCH4AM.SelectedIndex = 0;
                cmbCH4PP1.SelectedIndex = 0;
                cmbCH4PP2.SelectedIndex = 0;
                cmbCH4PL1.SelectedIndex = 0;
                cmbCH4PL2.SelectedIndex = 0;
                cmbCH4EI1.SelectedIndex = 0;
                cmbCH4EI2.SelectedIndex = 0;
                cmbCH4IE.SelectedIndex = 0;


                cmbCH5AM.SelectedIndex = 0;
                cmbCH5PP1.SelectedIndex = 0;
                cmbCH5PP2.SelectedIndex = 0;
                cmbCH5PL1.SelectedIndex = 0;
                cmbCH5PL2.SelectedIndex = 0;
                cmbCH5EI1.SelectedIndex = 0;
                cmbCH5EI2.SelectedIndex = 0;
                cmbCH5IE.SelectedIndex = 0;

                cmbCH6AM.SelectedIndex = 0;
                cmbCH6PP1.SelectedIndex = 0;
                cmbCH6PP2.SelectedIndex = 0;
                cmbCH6PL1.SelectedIndex = 0;
                cmbCH6PL2.SelectedIndex = 0;
                cmbCH6EI1.SelectedIndex = 0;
                cmbCH6EI2.SelectedIndex = 0;
                cmbCH6IE.SelectedIndex = 0;

                cmbCH7AM.SelectedIndex = 0;
                cmbCH7PP1.SelectedIndex = 0;
                cmbCH7PP2.SelectedIndex = 0;
                cmbCH7PL1.SelectedIndex = 0;
                cmbCH7PL2.SelectedIndex = 0;
                cmbCH7EI1.SelectedIndex = 0;
                cmbCH7EI2.SelectedIndex = 0;
                cmbCH7IE.SelectedIndex = 0;

                cmbCH8AM.SelectedIndex = 0;
                cmbCH8PP1.SelectedIndex = 0;
                cmbCH8PP2.SelectedIndex = 0;
                cmbCH8PL1.SelectedIndex = 0;
                cmbCH8PL2.SelectedIndex = 0;
                cmbCH8EI1.SelectedIndex = 0;
                cmbCH8EI2.SelectedIndex = 0;
                cmbCH8IE.SelectedIndex = 0;
            }
        }
    }
}
