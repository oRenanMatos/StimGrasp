using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Media;
using LiveCharts;
using LiveCharts.Wpf;

namespace StimGraspUI
{
    public partial class Form1 : Form
    {
        private double[] DACP = new double[90];
        private double[] ChannelOut = new double[10000];
        private double[] timeDAC = new double[30];

        //Vetores Amplitudes
        private int DAC_Index = 0;
        private int[] DAC_value = new int [30000];
        private int [] AMP_SETPOINT_CHANNEL = new int [8];  //Amplitude desejada para o canal (0 a 8)
        private int [] AMP_ACTUAL_CHANNEL = new int [8];        //Amplitude atual do canal (0 a 8)
        private int []  AMP_CHANNEL =  new int [8];               //Amplitude do canal (0 a 8)

        //Vetores Passos
        private int [] PASSO_TENSAO_CHANNEL =  new int [8];  //Passo de tensão do canal (0 a 8)
        private int [] PASSO_RISE_CHANNEL = new int [8];        //Passo do tempo de subida do canal (0 a 8)
        private int [] PASSO_FALL_CHANNEL =  new int [8];        //Passo do tempo de descida do canal (0 a 8)

        //Vetores Presets
        private int [] PRESETS_AMP =  new int [8];               //Presets de amplitudes do canal (0 a 8)
        private int [] PRESETS_ABERTURA = new int [8];          //Presets de amplitudes abertura do canal (0 a 8)
        private int [] PRESETS_PALMAR1 =  new int [8];           //Presets de amplitudes palmar 01 do canal (0 a 8)
        private int [] PRESETS_PALMAR2 =  new int [8];           //Presets de amplitudes palmar 02 do canal (0 a 8)
        private int [] PRESETS_LATERAL1 = new int [8];          //Presets de amplitudes lateral 01 do canal (0 a 8)
        private int [] PRESETS_LATERAL2 = new int [8];          //Presets de amplitudes lateral 02 canal (0 a 8)
        private int [] PRESETS_EXTENSAO1 = new int[8];         //Presets de amplitudes extensao 01 do canal (0 a 8)
        private int [] PRESETS_EXTENSAO2 =  new int [8];        //Presets de amplitudes extensao 02 do canal (0 a 8)

        private byte[] PROTOCOLO = new byte [41];//"<STIMC00AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH>" };	//Comando Padrão

        //Flags de Controle
        private uint FLAG_STRING_AMP = 0;                                  //Flag de controle para calculo do passo de amplitude dos canais
        private uint FLAG_STRING_RECEBIDA = 0;                             //Flag de controle que indica que um pacote foi recebido na serial
        private uint FLAG_MEDIA_VBAT = 0;                         //Flag de controle para realizar uma amostragem na tensão da bateria
        private uint FLAG_HABILITA_VBAT = 0;                      //Flag de controle para verificar se a bateria esta ok para funcionar
        private uint FLAG_ATUALIZA_CANAIS = 0;                     //Flag de controle para atualização dos canais
        private uint FLAG_RAMPA = 0;                                       //Flag de controle para controle de rampa de subida e descidad do sinal
        private uint FLAG_PALMAR_AUTOMATICO = 0;
        private uint FLAG_EXTENSAO_AUTOMATICO = 0;
        private uint FLAG_DESLIGAMENTO_AUTOMATICO = 0;


        private uint FATOR_CONTAGEM_GERAL = 0;
        private uint counter_ch1 = 0;
        private uint CONTAGEM_MEDIA_VBAT = 0;

        public Form1()
        {
            InitializeComponent();
            //serialPort1.Open();
        }

        private void Form1_Load(object sender, EventArgs e)
        {



            pulsePositive();
         //   rampa();
            Chart01();
        //    Chart02();



        }
        public SeriesCollection SeriesCollection { get; set; }

        private void Chart01()
        {
            cartesianChart2.Series = new SeriesCollection
            {        
               

            new LineSeries
                {
                    Title = "Pulso 300 us",
                    Values = new ChartValues<double> (),
                    PointGeometry = null
                }
            
                /*
                new LineSeries
                {
                    Title = "Series 1",
                    Values = new ChartValues<double> {4, 6, 5, 2, 7}
                },
                new LineSeries
                {
                    Title = "Series 2",
                    Values = new ChartValues<double> {6, 7, 3, 4, 6},
                    PointGeometry = null
                },
                new LineSeries
                {
                    Title = "Series 2",
                    Values = new ChartValues<double> {5, 2, 8, 3},
                    PointGeometry = DefaultGeometries.Square,
                    PointGeometrySize = 15
                }*/
            };

            int i; 

            for (i = 0; i < 80; ++i)
            {
                cartesianChart2.Series[0].Values.Add(DACP[i]);
            }

  
            cartesianChart2.AxisX.Add(new Axis
            {
                Title = "Time(10us)",
            });


            cartesianChart2.AxisY.Add(new Axis
            {
                Title = "Tensão de Saída",
                LabelFormatter = value => value.ToString()
            });

            cartesianChart2.LegendLocation = LegendLocation.Right;
            cartesianChart2.BackColor = SystemColors.Window;

        }

        private void Chart02()
        {
            cartesianChart1.Series = new SeriesCollection
            {


            new LineSeries
                {
                    Title = "Pulso 300 us",
                    Values = new ChartValues<double> (),
                    PointGeometry = null
                }
            
                /*
                new LineSeries
                {
                    Title = "Series 1",
                    Values = new ChartValues<double> {4, 6, 5, 2, 7}
                },
                new LineSeries
                {
                    Title = "Series 2",
                    Values = new ChartValues<double> {6, 7, 3, 4, 6},
                    PointGeometry = null
                },
                new LineSeries
                {
                    Title = "Series 2",
                    Values = new ChartValues<double> {5, 2, 8, 3},
                    PointGeometry = DefaultGeometries.Square,
                    PointGeometrySize = 15
                }*/
            };

            int i;

            for (i = 0; i < 10000; ++i)
            {
                cartesianChart1.Series[0].Values.Add(ChannelOut[i]);
            }


            cartesianChart1.AxisX.Add(new Axis
            {
                Title = "Time(10us)",
            });


            cartesianChart1.AxisY.Add(new Axis
            {
                Title = "Tensão de Saída",
                LabelFormatter = value => value.ToString()
            });

            cartesianChart1.LegendLocation = LegendLocation.Right;
            cartesianChart1.BackColor = SystemColors.Window;

        }

        private void CartesianChart1OnDataClick(object sender, ChartPoint chartPoint)
        {
            MessageBox.Show("You clicked (" + chartPoint.X + "," + chartPoint.Y + ")");
        }

        private void pulsePositive()
        {
            int i;
            for (i = 0; i < 10; ++i)
            {
                DACP[i] = 0;
            }
            
            for (i = 10; i < 40; ++i)
            {
                int ponto = i-10;
                double tau = 50e-6;
                double Amp = 3.3;
                double time = ponto * 10 * 1e-6;
                double value1 = (Amp * (1 - Math.Pow(2.71, -1 * (time / tau)))) + 1.65;
                double value2 = value1 - (3.3 / 2);
                double value3 = (value2 / 3.3) * 4096;
                DACP[i] = value2;
              //  timeDAC[i] = time;
            }

 
            for (i = 40; i < 70; ++i)
            {
                int ponto = i - 40 ;
                double tau = 50e-6;
                double Amp = 3.3;
                double time = ponto* 10 * 1e-6;
                double value1 = Amp * (Math.Pow(2.71, -1 * (time / tau))) - 1.65;
                double value2 = value1 - (3.3 / 2);
                double value3 = (value2 / 3.3) * 4096;
                DACP[i] = value2;
               // timeDAC[i] = time;
            }


            for (i = 70; i < 80; ++i)
            {
                DACP[i] = 0;
            }

        }

        private void rampa()
        {
            int i;
            int j;
            for (i = 0; i < 100; ++i)
            {
                ChannelOut[i] = 0;
            }
            for (i = 100; i < 10000; ++i)
            {
                if (i == 100 || i == 1000 || i == 1500 || i == 1900)
                {
                    for (j = 10; j < 70; ++j)
                    {
                        ChannelOut[i+j-10] = DACP[j];
                    }
                }
            }

        }

        private void button3_Click(object sender, EventArgs e)
        {
            serialPort1.Write("<STIM>");
        }

        #region PIC
        //####################################
        //#  Atualiza Amplitudes             #
        //####################################
        void UpdateAmplitudes(uint Selector)
        {
            if (Selector == 0)    //Troca de preensão palmar estágio 1 para estágio 2
            {
                int i;
                for (i = 0; i <= 7; ++i)
                {
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR2[i];
                }

                FLAG_STRING_AMP = 1;
                FLAG_PALMAR_AUTOMATICO = 0;
            }

            else if (Selector == 1)     //Troca de extensão de indicador estágio 1 para estágio 2
            {
                int i;
                for (i = 0; i <= 7; ++i)
                {
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO2[i];
                }

                FLAG_STRING_AMP = 1;
                FLAG_EXTENSAO_AUTOMATICO = 0;
            }

            else if (Selector == 2)     //Troca de abertura de mão para desligamento
            {
                int i;
                for (i = 0; i <= 7; ++i)
                {
                    AMP_SETPOINT_CHANNEL[i] = 0;
                }

                FLAG_STRING_AMP = 1;
                FLAG_DESLIGAMENTO_AUTOMATICO = 0;
            }
        }

        //####################################
        //#  Atualiza Amplitudes dos Canais  #
        //####################################
        void UpdateChannel(int Channel, int Valor)
        {

            switch (Channel)
            {
                case 0:
                    {
                        AMP_CHANNEL[0] = Valor;
                        break;
                    }
                case 1:
                    {
                        AMP_CHANNEL[1] = Valor;
                        break;
                    }
                case 2:
                    {
                        AMP_CHANNEL[2] = Valor;
                        break;
                    }
                case 3:
                    {
                        AMP_CHANNEL[3] = Valor;
                        break;
                    }
                case 4:
                    {
                        AMP_CHANNEL[4] = Valor;
                        break;
                    }
                case 5:
                    {
                        AMP_CHANNEL[5] = Valor;
                        break;
                    }
                case 6:
                    {
                        AMP_CHANNEL[6] = Valor;
                        break;
                    }
                case 7:
                    {
                        AMP_CHANNEL[7] = Valor;
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        //################################
        //#   Subrotina Verify_Protocol01#
        //# Analisa string serial válida #
        //#		  Protocolo Antigo		 #
        //################################
        void Verify_Protocol01()
        {


            uint I = 0;

            /*
             * Comando 00 - Shutdown do estimulador
             * Comando 01 - Resposta padrão
             * Comando 02 - Desligamento de canais
             * Comando 03 - Atualização imediata de canais
             * Comando 04 - Abertura de mão
             * Comando 05 - Estágio 1 de fechamento de mão na preensão palmar
             * Comando 06 - Estágio 2 de fechamento de mão na preensão palmar
             * Comando 07 - Estágio 1 de fechamento de mão na preensão lateral
             * Comando 08 - Estágio 2 de fechamento de mão na preensão lateral
             * Comando 09 - Estágio 1 de extensão do indicador
             * Comando 10 - Estágio 2 de extensão do indicador
             *
             * Valor 00 - Transforma e armazena os presets das 7 rampas de subida
             * Valor 01 - Transforma e armazena os presets das 7 rampas de descida
             * Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão
             * Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar
             * Valor 04 - Transforma e armazena os presets de amplitude do estágio 2 da preensão palmar
             * Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral
             * Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral
             * Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador
             * Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador
             */

            // Comando 00
            // Shutdown do estimulador
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '0')
            {

                int i;
                for (i = 0; i <= 7; ++i)
                {
                    AMP_SETPOINT_CHANNEL[i] = 0;        // Músculo revers revers 
                }
            }

            // Comando 01
            // Resposta padrão: "Stimgrasp: Ready to Go"
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '1')
            {

                UART1.AppendText("Comando 01 - Resposta padrão : Stimgrasp: Ready to Go\r\n");
            }

            // Comando 01
            // Resposta padrão: "AT"
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '3' &&
               PROTOCOLO[7] == '0')
            {
                UART1.AppendText("Comando 01 - Resposta padrão : AT\r\n");
            }

            // Comando 01
            // Resposta padrão: "AT+NAMEStimgrasp"
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '3' &&
               PROTOCOLO[7] == '1')
            {
                UART1.AppendText("Comando 01 - Resposta padrão : AT+NAMEStimgrasp\r\n");
            }

            // Comando 02
            // Desligamento de canais
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '2')
            {
                UART1.AppendText("Comando 02 - Desligamento de canais\r\n");

                int i;
                for (i = 0; i <= 7; ++i)
                {
                    AMP_SETPOINT_CHANNEL[i] = 0;        // Músculo revers revers 
                }

                // Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
                FLAG_PALMAR_AUTOMATICO = 0;
                FLAG_EXTENSAO_AUTOMATICO = 0;
            }

            // Comando 03
            // Atualização imediata de canais
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '3')
            {

                UART1.AppendText("Comando 03 - Atualização imediata de canaisr\n");

                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_AMP[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                    //Aplica as amplitudes de presets para amplitude desejada de todos os canais
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_AMP[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;

            }

            // Comando 04
            // Abertura de mão
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '4')
            {

                UART1.AppendText("Comando 04 - Abertura de mão\r\n");

                int i;
                for (i = 0; i <= 7; ++i)
                {
                    //Aplica as amplitudes de presets de abertura para amplitude desejada de todos os canais
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_ABERTURA[i];
                }
                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;

            }

            // Comando 05
            // Estágio 1 da preensão palmar
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '5')
            {

                UART1.AppendText("Comando 05 - Estágio 1 da preensão palmar\r\n");


                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de preensão palmar estágio 01
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR1[i];
                }



                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
                FLAG_PALMAR_AUTOMATICO = 1;
            }

            // Comando 06
            // Estágio 2 da preensão palmar
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '6')
            {

                UART1.AppendText("Comando 06 - Estágio 2 da preensão palmar\r\n");


                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de preensão palmar estágio 02
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR2[i];
                }


                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
                //FLAG_PALMAR_AUTOMATICO = 1;//Verificar porque não tem flag palmar automático

            }

            // Comando 07
            // Estágio 1 da preensão lateral
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '7')
            {

                UART1.AppendText("Comando 07 - Estágio 1 da preensão lateral\r\n");


                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de preensão lateral estágio 01
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_LATERAL1[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
            }

            // Comando 08
            // Estágio 2 da preensão lateral
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '8')
            {

                UART1.AppendText("Comando 08 - Estágio 2 da preensão lateral\r\n");


                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de preensão lateral estágio 02
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_LATERAL2[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
            }


            // Comando 09
            // Estágio 1 da extensao do indicador
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '9')
            {

                UART1.AppendText("Comando 09 - Estágio 1 da extensao do indicador\r\n");

                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de extensão do indicador estágio 01
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO1[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
                FLAG_EXTENSAO_AUTOMATICO = 1;

            }

            // Comando 10
            // Estágio 2 da extensao do indicador
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '1' &&
               PROTOCOLO[7] == '0')
            {

                UART1.AppendText("Comando 10 - Estágio 2 da extensao do indicadorgio 1 da preensão lateral\r\n");


                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de extensão do indicador estágio 02
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO2[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
            }


            // Comando 11
            // Abertura de mão com desligamento de canais automático
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '1' &&
               PROTOCOLO[7] == '1')
            {

                UART1.AppendText("Comando 11 - Abertura de mão com desligamento de canais automático\r\n");

                int i;
                for (i = 0; i <= 7; ++i)
                {
                    /*Aplica as amplitudes de presets de abertura
                    para amplitude desejada de todos os canais*/
                    AMP_SETPOINT_CHANNEL[i] = PRESETS_ABERTURA[i];
                }

                //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
                FLAG_STRING_AMP = 1;
                FLAG_DESLIGAMENTO_AUTOMATICO = 1;
            }


            // Comando 20
            // Recupera presets da memória eeprom
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '0')
            {

                UART1.AppendText("Comando 20 - Recupera presets da memória eeprom\r\n");

            }


            // Comando 21
            // Escrita na EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '1')
            {

                UART1.AppendText("Comando 21 - Recupera presets da memória eeprom\r\n");

            }

            // Comando 22   
            // Escrita na EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '2')
            {

                UART1.AppendText("Comando 22 - Recupera presets da memória eeprom\r\n");
 
            }

            // Comando 23
            // Leitura na EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '3')
            {

                // PRESET_LOW_TEMP = I2Cread(ADDRESS,SUBADDRESS);


            }

            // Comando 24
            // Escrita na EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '4')
            {
        

            }

            // Comando 25
            // Exibiçao dos presets da EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '5')
            {

            }

            // Comando 26
            // Formatação da EEPROM
            if (PROTOCOLO[5] == 'C' &&
               PROTOCOLO[6] == '2' &&
               PROTOCOLO[7] == '6')
            {
 
            }

            // Ajuste de rampas de subida
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '0')
            {

                UART1.AppendText("Valor 00 - Transforma e armazena os presets das 7 rampas de subida\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PASSO_RISE_CHANNEL[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48)) / 100;
                    n = n + 4;
                }

            }

            // Ajuste de rampas de descida
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '1')
            {

                UART1.AppendText("Valor 01 - Transforma e armazena os presets das 7 rampas de descida\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PASSO_FALL_CHANNEL[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48)) / 100;
                    n = n + 4;
                }

            }

            // Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '2')
            {


                UART1.AppendText("Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_ABERTURA[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }

            }

            // Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '3')
            {

                UART1.AppendText("Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_PALMAR1[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }


            }

            // Valor 04 - Transforma e armazena os presets de amplitude da preensão palmar
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '4')
            {


                UART1.AppendText("Valor 04 - Transforma e armazena os presets de amplitude do estágio 2 da preensão palmar\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_PALMAR2[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }


            }

            // Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '5')
            {


                UART1.AppendText("Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_LATERAL1[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }

            }

            // Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '6')
            {


                UART1.AppendText("Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_LATERAL2[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }


            }

            // Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '7')
            {

                UART1.AppendText("Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_EXTENSAO1[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }


            }

            // Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador
            if (PROTOCOLO[5] == 'V' &&
               PROTOCOLO[6] == '0' &&
               PROTOCOLO[7] == '8')
            {

                UART1.AppendText("Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador\n");


                int i;
                int n = 8;
                for (i = 0; i <= 7; ++i)
                {
                    //Converte o valor da serial para valores de amplitude
                    // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
                    PRESETS_EXTENSAO2[i] = (((PROTOCOLO[n] - 48) * 1000) +
                                        ((PROTOCOLO[n + 1] - 48) * 100) +
                                        ((PROTOCOLO[n + 2] - 48) * 10) +
                                        (PROTOCOLO[n + 3] - 48));
                    n = n + 4;
                }

            }
        }

        void Init_Vars()
        {
            int i;
            for (i = 0; i <= 7; ++i)
            {
                AMP_SETPOINT_CHANNEL[i] = 0;
                AMP_ACTUAL_CHANNEL[i] = 0;
                PASSO_TENSAO_CHANNEL[i] = 0;
                PASSO_RISE_CHANNEL[i] = 0;
                PASSO_FALL_CHANNEL[i] = 0;
            }
        }

        //####################################
        //#  Subrotina Rampa de Subida	     #
        //####################################
        void Rise_Ramp()
        {
            int i;
            //Manuseia a rampa de todos os canais 
            for (i = 0; i <= 7; ++i)
            {
                int AMP_TEMPORARIA = 0;

                if (AMP_ACTUAL_CHANNEL[i] < AMP_SETPOINT_CHANNEL[i])
                {
                    if ((AMP_SETPOINT_CHANNEL[i] - AMP_ACTUAL_CHANNEL[i]) < PASSO_TENSAO_CHANNEL[i])
                    {
                        AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i];
                    }
                    else
                    {
                        AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i] + PASSO_TENSAO_CHANNEL[i];
                    }
                    UpdateChannel(i, AMP_TEMPORARIA);
                    AMP_ACTUAL_CHANNEL[i] = AMP_TEMPORARIA;
                }
                else if (AMP_ACTUAL_CHANNEL[i] > AMP_SETPOINT_CHANNEL[i])
                {
                    if (AMP_ACTUAL_CHANNEL[i] > PASSO_TENSAO_CHANNEL[i])
                    {
                        AMP_TEMPORARIA = (AMP_ACTUAL_CHANNEL[i] - PASSO_TENSAO_CHANNEL[i]);
                    }
                    else
                    {
                        AMP_TEMPORARIA = 0;
                    }
                    UpdateChannel(i, AMP_TEMPORARIA);
                    AMP_ACTUAL_CHANNEL[i] = AMP_TEMPORARIA;
                }
            }

        }

        //####################################
        //#  Calculo dos passos de amplitude #
        //####################################	

        void Step_Amp()
        {
            //Acionar essa função cada vez que que receber um comando serial de amplitude

            // Ajuste dos Canais, verifica para quanto tem que ir e calcula
            // o passo a ser dado, de acordo com a rampa selecionada previamente
            int i;
            for (i = 0; i <= 7; ++i)
            {

                if (AMP_SETPOINT_CHANNEL[i] != AMP_ACTUAL_CHANNEL[i])
                {
                    if (AMP_SETPOINT_CHANNEL[i] > AMP_ACTUAL_CHANNEL[i])
                    {
                        PASSO_TENSAO_CHANNEL[i] = ((AMP_SETPOINT_CHANNEL[i] - AMP_ACTUAL_CHANNEL[i]) / PASSO_RISE_CHANNEL[i]);
                    }
                    else if (AMP_SETPOINT_CHANNEL[i] < AMP_ACTUAL_CHANNEL[i])
                    {
                        PASSO_TENSAO_CHANNEL[i] = ((AMP_ACTUAL_CHANNEL[i] - AMP_SETPOINT_CHANNEL[i]) / PASSO_FALL_CHANNEL[i]);
                    }
                    FLAG_ATUALIZA_CANAIS = 1;
                    //Aciona uma flag para atualiza os canais
                }
            }
        }

        void Principal()
        {
        //    int value = DAC_value[DAC_Index-1];
    //        textBox1.Text = Convert.ToString(value);

                //verificação e chamada da rotina de tratamento de dado serial, a partir de
                //flag gerado na interrupção serial, em recebendo um bloco válido
                if (FLAG_STRING_RECEBIDA == 1)
                {
                    FLAG_STRING_RECEBIDA = 0;
                    Verify_Protocol01();
                }

                // Verificação de tensão de bateria

                if (FLAG_MEDIA_VBAT == 1)   //Já esta na hora de colher uma nova amostra?
                {
                    //Request_VBAT();
                }

                if (FLAG_HABILITA_VBAT == 1)     //Já passou o delay do início do programa?
                {
                    //Verify_VBAT();
                }

                //rotina de rampa de subida
                if ((FLAG_ATUALIZA_CANAIS == 1) && (FLAG_RAMPA == 1))
                {
                    FLAG_RAMPA = 0;
                    Rise_Ramp();
                }


                // Verifica se a preensão anterior foi finalizada, e se algum dos flags de segundo estágio automático
                // está setado, então chama função que recarrega as variáveis de amplitude desejada
                if ((AMP_ACTUAL_CHANNEL[0] == AMP_SETPOINT_CHANNEL[0]) &&
                   (AMP_ACTUAL_CHANNEL[1] == AMP_SETPOINT_CHANNEL[1]) &&
                   (AMP_ACTUAL_CHANNEL[2] == AMP_SETPOINT_CHANNEL[2]) &&
                   (AMP_ACTUAL_CHANNEL[3] == AMP_SETPOINT_CHANNEL[3]) &&
                   (AMP_ACTUAL_CHANNEL[4] == AMP_SETPOINT_CHANNEL[4]) &&
                   (AMP_ACTUAL_CHANNEL[5] == AMP_SETPOINT_CHANNEL[5]) &&
                   (AMP_ACTUAL_CHANNEL[6] == AMP_SETPOINT_CHANNEL[6]) &&
                   (AMP_ACTUAL_CHANNEL[7] == AMP_SETPOINT_CHANNEL[7]))
                {
                    if (FLAG_PALMAR_AUTOMATICO == 1)
                    {
                        UpdateAmplitudes(0); //Chama recarga de preensão palmar
                    }
                    else if (FLAG_EXTENSAO_AUTOMATICO == 1)
                    {
                        UpdateAmplitudes(1); //Chama recarga de extensão de indicador
                    }
                    else if (FLAG_DESLIGAMENTO_AUTOMATICO == 1)
                    {
                        UpdateAmplitudes(2); //Chama recarga de desligamento
                    }
                }


                // Caso tenha recebido comando serial, determina passos para cada mudança de amplitude
                // nos 8 canais

                if (FLAG_STRING_AMP == 1)
                {
                    FLAG_STRING_AMP = 0;
                    FATOR_CONTAGEM_GERAL = 0;
                    Step_Amp();
                }
            
        }

        //####################################
        //#  Seleção de Canal do STIMGRASP   #
        //####################################

        void MUXSelect(uint MUXChannel)
        {
            switch (MUXChannel)
            {
                case 0:

                    UART1.AppendText("Canal 0 Selecionado\r\n");

                    break;
                case 1:

                    UART1.AppendText("Canal 1 Selecionado\r\n");


                    break;
                case 2:

                    UART1.AppendText("Canal 2 Selecionado\r\n");


                    break;
                case 3:

                    UART1.AppendText("Canal 3 Selecionado\r\n");

  
                    break;
                case 4:

                    UART1.AppendText("Canal 4 Selecionado\r\n");


                    break;
                case 5:

                    UART1.AppendText("Canal 5 Selecionado\r\n");


                    break;
                case 6:

                    UART1.AppendText("Canal 6 Selecionado\r\n");


                    break;
                case 7:

                    UART1.AppendText("Canal 7 Selecionado\r\n");

                    break;
                default:

                    UART1.AppendText("Canais Desligados\r\n");

                    break;
            }
        }

        void DAC_I2C(int sample)
        {  
            DAC_value[DAC_Index] = sample;
            DAC_Index++;
        }
        void T1Interrupt()
        {
            /* Interrupt Service Routine code goes here */


            /*
            10,0us por interrupção

            TS210 metaltex
            Ton máximo: 1,0ms
            Toff máximo: 1,5ms
        */


            //  if(BLUE == 0) // Se o LED Azul estiver ligado
            //   {
            //      BLUE = 1; //Desliga o LED Azul
            //   }
            //  else    // Se o LED Azul estiver desligado
            //  {
            //       BLUE = 0; //Liga o LED Azul
            //   }


            counter_ch1++;


            FATOR_CONTAGEM_GERAL++;
            if (FATOR_CONTAGEM_GERAL >= 100)
            {
                FATOR_CONTAGEM_GERAL = 0;
                FLAG_RAMPA = 1;     // Estoura a cada 10000x10us = 100ms
                FLAG_HABILITA_VBAT = 1;
            }

            CONTAGEM_MEDIA_VBAT++;
            if (CONTAGEM_MEDIA_VBAT > 500)
            {
                CONTAGEM_MEDIA_VBAT = 0;
                FLAG_MEDIA_VBAT = 1;     // Hora de pegar uma amostra
            }

            switch (counter_ch1)
            {

                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 0 #
                //         ##########################
                case 1:
                    {
                        if (AMP_CHANNEL[0] != 0) //Se a amplitude é zero, nem seleciona o canal
                        {
                            MUXSelect(0); //Seleciona o canal 0
                        }
                        break;
                    }

                case 101: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[0] != 0) //Se a amplitude é zero, nem desliga a descarga
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[0] + 2048);
                        break;
                    }

                case 126:
                    {
                        if (AMP_CHANNEL[0] != 0) //Se a amplitude é zero, nem desliga a descarga
                        {
                            //CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[0]);
                        break;
                    }

                case 151: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 1 #
                //         ##########################
                case 311:
                    {
                        if (AMP_CHANNEL[1] != 0)
                        {
                            MUXSelect(1); //Seleciona o canal 1
                        }
                        break;
                    }

                case 411: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[1] != 0)
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[1] + 2048);
                        break;
                    }

                case 436:
                    {
                        if (AMP_CHANNEL[1] != 0)
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[1]);
                        break;
                    }

                case 461: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 2 #
                //         ##########################
                case 621:
                    {
                        if (AMP_CHANNEL[2] != 0)
                        {
                            MUXSelect(2); //Seleciona o canal 2
                        }
                        break;
                    }

                case 721: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[2] != 0)
                        {
                          //  CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[2] + 2048);
                        break;
                    }

                case 746:
                    {
                        if (AMP_CHANNEL[2] != 0)
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[2]);
                        break;
                    }

                case 771: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 3 #
                //         ##########################
                case 931:
                    {
                        if (AMP_CHANNEL[3] != 0)
                        {
                            MUXSelect(3); //Seleciona o canal 3
                        }
                        break;
                    }

                case 1031: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[3] != 0)
                        {
                          //  CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[3] + 2048);
                        break;
                    }

                case 1056:
                    {
                        if (AMP_CHANNEL[3] != 0)
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[3]);
                        break;
                    }

                case 1081: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 4 #
                //         ##########################
                case 1241:
                    {
                        if (AMP_CHANNEL[4] != 0)
                        {
                            MUXSelect(4); //Seleciona o canal 4
                        }
                        break;
                    }

                case 1341: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[4] != 0)
                        {
                           // CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[4] + 2048);
                        break;
                    }

                case 1366:
                    {
                        if (AMP_CHANNEL[4] != 0)
                        {
                         //   CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[4]);
                        break;
                    }

                case 1391: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8);        //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 5 #
                //         ##########################
                case 1551:
                    {
                        if (AMP_CHANNEL[5] != 0)
                        {
                            MUXSelect(5); //Seleciona o canal 5
                        }
                        break;
                    }

                case 1651: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[5] != 0)
                        {
                         //   CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[5] + 2048);
                        break;
                    }

                case 1676:
                    {
                        if (AMP_CHANNEL[5] != 0)
                        {
                         //   CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[5]);
                        break;
                    }

                case 1701: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 6 #
                //         ##########################
                case 1861:
                    {
                        if (AMP_CHANNEL[6] != 0)
                        {
                            MUXSelect(6); //Seleciona o canal 6
                        }
                        break;
                    }

                case 1961: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[6] != 0)
                        {
                          //  CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[6] + 2048);
                        break;
                    }

                case 1986:
                    {
                        if (AMP_CHANNEL[6] != 0)
                        {
                          //  CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[6]);
                        break;
                    }

                case 2011: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                       // CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }

                //         ##########################
                //         # ATUALIZAÇÃO DO CANAL 7 #
                //         ##########################
                case 2171:
                    {
                        if (AMP_CHANNEL[7] != 0)
                        {
                            MUXSelect(7); //Seleciona o canal 7
                        }
                        break;
                    }

                case 2271: // considerando 52us para a rotina de atualizar o DAC
                    {
                        if (AMP_CHANNEL[7] != 0)
                        {
                          //  CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(AMP_CHANNEL[7] + 2048);
                        break;
                    }

                case 2296:
                    {
                        if (AMP_CHANNEL[7] != 0)
                        {
                            //CARGA = 1;          // Liga carga na saída
                        }
                        DAC_I2C(2048 - AMP_CHANNEL[7]);
                        break;
                    }

                case 2321: //considerando 52us para a rotina de atualizar o dAC
                    {
                        DAC_I2C(2048);
                        //CARGA = 0;          // Desliga carga na saída
                        MUXSelect(8); //Desliga canais
                        break;
                    }


                //         ########################
                //         # REINÍCIO DE CONTAGEM #
                //         ########################
                case 2322:
                    {
                        counter_ch1 = 0;
                        break;
                    }

                default:
                    {
                        break;
                    }

            }


        }

        #endregion

        private void timer1_Tick(object sender, EventArgs e)
        {
            T1Interrupt();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            Principal();
        }

        private void btnCM_Click(object sender, EventArgs e)
        {
            FLAG_STRING_RECEBIDA = 1;

            PROTOCOLO[0] = Convert.ToByte('<'); 
            PROTOCOLO[1] = Convert.ToByte('S'); 
            PROTOCOLO[2] = Convert.ToByte('T'); 
            PROTOCOLO[3] = Convert.ToByte('I'); 
            PROTOCOLO[4] = Convert.ToByte('M'); 
            PROTOCOLO[5] = Convert.ToByte('C'); 
            PROTOCOLO[6] = Convert.ToByte('0'); 
            PROTOCOLO[7] = Convert.ToByte('1'); 
            PROTOCOLO[8] = Convert.ToByte('x'); 
            PROTOCOLO[9] = Convert.ToByte('x'); 
            PROTOCOLO[10] = Convert.ToByte('x'); 
            PROTOCOLO[11] = Convert.ToByte('x'); 
            PROTOCOLO[12] = Convert.ToByte('x'); 
            PROTOCOLO[13] = Convert.ToByte('x'); 
            PROTOCOLO[14] = Convert.ToByte('x'); 
            PROTOCOLO[15] = Convert.ToByte('x'); 
            PROTOCOLO[16] = Convert.ToByte('x'); 
            PROTOCOLO[17] = Convert.ToByte('x'); 
            PROTOCOLO[18] = Convert.ToByte('x'); 
            PROTOCOLO[19] = Convert.ToByte('x'); 
            PROTOCOLO[20] = Convert.ToByte('x'); 
            PROTOCOLO[21] = Convert.ToByte('x'); 
            PROTOCOLO[22] = Convert.ToByte('x'); 
            PROTOCOLO[23] = Convert.ToByte('x'); 
            PROTOCOLO[24] = Convert.ToByte('x'); 
            PROTOCOLO[25] = Convert.ToByte('x'); 
            PROTOCOLO[26] = Convert.ToByte('x'); 
            PROTOCOLO[27] = Convert.ToByte('x'); 
            PROTOCOLO[28] = Convert.ToByte('x'); 
            PROTOCOLO[29] = Convert.ToByte('x'); 
            PROTOCOLO[30] = Convert.ToByte('x'); 
            PROTOCOLO[31] = Convert.ToByte('x'); 
            PROTOCOLO[32] = Convert.ToByte('x'); 
            PROTOCOLO[33] = Convert.ToByte('x'); 
            PROTOCOLO[34] = Convert.ToByte('x'); 
            PROTOCOLO[35] = Convert.ToByte('x'); 
            PROTOCOLO[36] = Convert.ToByte('x'); 
            PROTOCOLO[37] = Convert.ToByte('x'); 
            PROTOCOLO[38] = Convert.ToByte('x'); 
            PROTOCOLO[39] = Convert.ToByte('x'); 
            PROTOCOLO[40] = Convert.ToByte('>'); 
          //  PROTOCOLO[41] = Convert.ToByte('x'); 
        }

        private void btnTeste_Click(object sender, EventArgs e)
        {
            Init_Vars();
            timer1.Start();
            timer2.Start();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            FLAG_STRING_RECEBIDA = 1;
        

            PROTOCOLO[0] = Convert.ToByte('<');
            PROTOCOLO[1] = Convert.ToByte('S');
            PROTOCOLO[2] = Convert.ToByte('T');
            PROTOCOLO[3] = Convert.ToByte('I');
            PROTOCOLO[4] = Convert.ToByte('M');
            PROTOCOLO[5] = Convert.ToByte('V');
            PROTOCOLO[6] = Convert.ToByte('0');
            PROTOCOLO[7] = Convert.ToByte('0');
            PROTOCOLO[8] = Convert.ToByte('1');
            PROTOCOLO[9] = Convert.ToByte('5');
            PROTOCOLO[10] = Convert.ToByte('0');
            PROTOCOLO[11] = Convert.ToByte('0');
            PROTOCOLO[12] = Convert.ToByte('1');
            PROTOCOLO[13] = Convert.ToByte('5');
            PROTOCOLO[14] = Convert.ToByte('0');
            PROTOCOLO[15] = Convert.ToByte('0');
            PROTOCOLO[16] = Convert.ToByte('1');
            PROTOCOLO[17] = Convert.ToByte('5');
            PROTOCOLO[18] = Convert.ToByte('0');
            PROTOCOLO[19] = Convert.ToByte('0');
            PROTOCOLO[20] = Convert.ToByte('1');
            PROTOCOLO[21] = Convert.ToByte('5');
            PROTOCOLO[22] = Convert.ToByte('0');
            PROTOCOLO[23] = Convert.ToByte('0');
            PROTOCOLO[24] = Convert.ToByte('1');
            PROTOCOLO[25] = Convert.ToByte('5');
            PROTOCOLO[26] = Convert.ToByte('0');
            PROTOCOLO[27] = Convert.ToByte('0');
            PROTOCOLO[28] = Convert.ToByte('1');
            PROTOCOLO[29] = Convert.ToByte('5');
            PROTOCOLO[30] = Convert.ToByte('0');
            PROTOCOLO[31] = Convert.ToByte('0');
            PROTOCOLO[32] = Convert.ToByte('1');
            PROTOCOLO[33] = Convert.ToByte('5');
            PROTOCOLO[34] = Convert.ToByte('0');
            PROTOCOLO[35] = Convert.ToByte('0');
            PROTOCOLO[36] = Convert.ToByte('1');
            PROTOCOLO[37] = Convert.ToByte('5');
            PROTOCOLO[38] = Convert.ToByte('0');
            PROTOCOLO[39] = Convert.ToByte('0');
            PROTOCOLO[40] = Convert.ToByte('>');
        }

        private void button5_Click(object sender, EventArgs e)
        {
            FLAG_STRING_RECEBIDA = 1;

            PROTOCOLO[0] = Convert.ToByte('<');
            PROTOCOLO[1] = Convert.ToByte('S');
            PROTOCOLO[2] = Convert.ToByte('T');
            PROTOCOLO[3] = Convert.ToByte('I');
            PROTOCOLO[4] = Convert.ToByte('M');
            PROTOCOLO[5] = Convert.ToByte('V');
            PROTOCOLO[6] = Convert.ToByte('0');
            PROTOCOLO[7] = Convert.ToByte('1');
            PROTOCOLO[8] = Convert.ToByte('0');
            PROTOCOLO[9] = Convert.ToByte('5');
            PROTOCOLO[10] = Convert.ToByte('0');
            PROTOCOLO[11] = Convert.ToByte('0');
            PROTOCOLO[12] = Convert.ToByte('0');
            PROTOCOLO[13] = Convert.ToByte('5');
            PROTOCOLO[14] = Convert.ToByte('0');
            PROTOCOLO[15] = Convert.ToByte('0');
            PROTOCOLO[16] = Convert.ToByte('0');
            PROTOCOLO[17] = Convert.ToByte('5');
            PROTOCOLO[18] = Convert.ToByte('0');
            PROTOCOLO[19] = Convert.ToByte('0');
            PROTOCOLO[20] = Convert.ToByte('0');
            PROTOCOLO[21] = Convert.ToByte('5');
            PROTOCOLO[22] = Convert.ToByte('0');
            PROTOCOLO[23] = Convert.ToByte('0');
            PROTOCOLO[24] = Convert.ToByte('0');
            PROTOCOLO[25] = Convert.ToByte('5');
            PROTOCOLO[26] = Convert.ToByte('0');
            PROTOCOLO[27] = Convert.ToByte('0');
            PROTOCOLO[28] = Convert.ToByte('0');
            PROTOCOLO[29] = Convert.ToByte('5');
            PROTOCOLO[30] = Convert.ToByte('0');
            PROTOCOLO[31] = Convert.ToByte('0');
            PROTOCOLO[32] = Convert.ToByte('0');
            PROTOCOLO[33] = Convert.ToByte('5');
            PROTOCOLO[34] = Convert.ToByte('0');
            PROTOCOLO[35] = Convert.ToByte('0');
            PROTOCOLO[36] = Convert.ToByte('0');
            PROTOCOLO[37] = Convert.ToByte('5');
            PROTOCOLO[38] = Convert.ToByte('0');
            PROTOCOLO[39] = Convert.ToByte('0');
            PROTOCOLO[40] = Convert.ToByte('>');
        }

        private void button6_Click(object sender, EventArgs e)
        {
            FLAG_STRING_RECEBIDA = 1;

            PROTOCOLO[0] = Convert.ToByte('<');
            PROTOCOLO[1] = Convert.ToByte('S');
            PROTOCOLO[2] = Convert.ToByte('T');
            PROTOCOLO[3] = Convert.ToByte('I');
            PROTOCOLO[4] = Convert.ToByte('M');
            PROTOCOLO[5] = Convert.ToByte('V');
            PROTOCOLO[6] = Convert.ToByte('0');
            PROTOCOLO[7] = Convert.ToByte('2');
            PROTOCOLO[8] = Convert.ToByte('0');
            PROTOCOLO[9] = Convert.ToByte('4');
            PROTOCOLO[10] = Convert.ToByte('0');
            PROTOCOLO[11] = Convert.ToByte('0');
            PROTOCOLO[12] = Convert.ToByte('0');
            PROTOCOLO[13] = Convert.ToByte('4');
            PROTOCOLO[14] = Convert.ToByte('0');
            PROTOCOLO[15] = Convert.ToByte('0');
            PROTOCOLO[16] = Convert.ToByte('0');
            PROTOCOLO[17] = Convert.ToByte('0');
            PROTOCOLO[18] = Convert.ToByte('0');
            PROTOCOLO[19] = Convert.ToByte('0');
            PROTOCOLO[20] = Convert.ToByte('0');
            PROTOCOLO[21] = Convert.ToByte('2');
            PROTOCOLO[22] = Convert.ToByte('5');
            PROTOCOLO[23] = Convert.ToByte('0');
            PROTOCOLO[24] = Convert.ToByte('0');
            PROTOCOLO[25] = Convert.ToByte('0');
            PROTOCOLO[26] = Convert.ToByte('0');
            PROTOCOLO[27] = Convert.ToByte('0');
            PROTOCOLO[28] = Convert.ToByte('0');
            PROTOCOLO[29] = Convert.ToByte('0');
            PROTOCOLO[30] = Convert.ToByte('0');
            PROTOCOLO[31] = Convert.ToByte('0');
            PROTOCOLO[32] = Convert.ToByte('0');
            PROTOCOLO[33] = Convert.ToByte('0');
            PROTOCOLO[34] = Convert.ToByte('0');
            PROTOCOLO[35] = Convert.ToByte('0');
            PROTOCOLO[36] = Convert.ToByte('0');
            PROTOCOLO[37] = Convert.ToByte('0');
            PROTOCOLO[38] = Convert.ToByte('0');
            PROTOCOLO[39] = Convert.ToByte('0');
            PROTOCOLO[40] = Convert.ToByte('>');
        }

        private void button13_Click(object sender, EventArgs e)
        {
            FLAG_STRING_RECEBIDA = 1;

            PROTOCOLO[0] = Convert.ToByte('<');
            PROTOCOLO[1] = Convert.ToByte('S');
            PROTOCOLO[2] = Convert.ToByte('T');
            PROTOCOLO[3] = Convert.ToByte('I');
            PROTOCOLO[4] = Convert.ToByte('M');
            PROTOCOLO[5] = Convert.ToByte('C');
            PROTOCOLO[6] = Convert.ToByte('0');
            PROTOCOLO[7] = Convert.ToByte('4');
            PROTOCOLO[8] = Convert.ToByte('x');
            PROTOCOLO[9] = Convert.ToByte('x');
            PROTOCOLO[10] = Convert.ToByte('x');
            PROTOCOLO[11] = Convert.ToByte('x');
            PROTOCOLO[12] = Convert.ToByte('x');
            PROTOCOLO[13] = Convert.ToByte('x');
            PROTOCOLO[14] = Convert.ToByte('x');
            PROTOCOLO[15] = Convert.ToByte('x');
            PROTOCOLO[16] = Convert.ToByte('x');
            PROTOCOLO[17] = Convert.ToByte('x');
            PROTOCOLO[18] = Convert.ToByte('x');
            PROTOCOLO[19] = Convert.ToByte('x');
            PROTOCOLO[20] = Convert.ToByte('x');
            PROTOCOLO[21] = Convert.ToByte('x');
            PROTOCOLO[22] = Convert.ToByte('x');
            PROTOCOLO[23] = Convert.ToByte('x');
            PROTOCOLO[24] = Convert.ToByte('x');
            PROTOCOLO[25] = Convert.ToByte('x');
            PROTOCOLO[26] = Convert.ToByte('x');
            PROTOCOLO[27] = Convert.ToByte('x');
            PROTOCOLO[28] = Convert.ToByte('x');
            PROTOCOLO[29] = Convert.ToByte('x');
            PROTOCOLO[30] = Convert.ToByte('x');
            PROTOCOLO[31] = Convert.ToByte('x');
            PROTOCOLO[32] = Convert.ToByte('x');
            PROTOCOLO[33] = Convert.ToByte('x');
            PROTOCOLO[34] = Convert.ToByte('x');
            PROTOCOLO[35] = Convert.ToByte('x');
            PROTOCOLO[36] = Convert.ToByte('x');
            PROTOCOLO[37] = Convert.ToByte('x');
            PROTOCOLO[38] = Convert.ToByte('x');
            PROTOCOLO[39] = Convert.ToByte('x');
            PROTOCOLO[40] = Convert.ToByte('>');

        }
    }
}
