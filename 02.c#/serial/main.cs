using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace serial
{
   
    public partial class main : Form
    {
        SerialPort sp1 = new SerialPort();
        Thread t1;
        Thread t2;

        int ReadAddFlag = 0;
        int ReadDestAddrFlag = 0;
        int ReadACKFlag = 0;
        int ReadAddrEnableFlag = 0;
        int ReadConfig = 0;


        string RecvString = null;

        public main()
        {
            InitializeComponent();
        }

        #region 主窗口加载初始化
        private void main_Load(object sender, EventArgs e)
        {

            ParamentLoad();
            //检查是否含有串口
  
            string[] str = SerialPort.GetPortNames();
            if (str == null)
            {
                MessageBox.Show("can not found port", "Error");
                return;
            }
            cbBaudRate.Items.Add("9600");
            cbBaudRate.Items.Add("115200");
            cbBaudRate.SelectedIndex = 1;
            cbData.Items.Add("5");
            cbData.Items.Add("6");
            cbData.Items.Add("7");
            cbData.Items.Add("8");
            cbData.SelectedIndex = 3;
            cbStop.Items.Add("1");
            cbStop.Items.Add("1.5");
            cbStop.Items.Add("1.2");
            cbStop.SelectedIndex = 0;
            cbParity.Items.Add("NONE");
            cbParity.Items.Add("Odd");
            cbParity.Items.Add("Even");
            cbParity.SelectedIndex = 0;
            //添加串口
            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                cbSerial.Items.Add(s);
            }
            if (cbSerial.Items.Count > 0)
            {
                try
                {
                    cbSerial.SelectedIndex = 0;//必须有串口
                    sp1.BaudRate = 115200;
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error:" + ex.Message, "Error");

                    //tmSend.Enabled = false;
                    return;
                }
            }
            disableBtn();
            Control.CheckForIllegalCrossThreadCalls = false;
           // sp1.DataReceived += new SerialDataReceivedEventHandler(sp1_DataReceived);
            tsProgressBar.Minimum = 0;
            tsProgressBar.Maximum = 100;

            t2 = new Thread(new ThreadStart(serialRecv));
            t2.Start();



        }
        #endregion


        #region 和串口发送相关的按键使能与失能
        private void enableBtn()
        {
            
            btnDownload.Enabled = true;
            btnDownload.Text = "download";
            btnSend.Enabled = true;
            btnSettings.Enabled = true;
            btnRestart.Enabled = true;
            btnTest.Enabled = true;
            btnReadVersion.Enabled = true;

            btnAddr.Enabled = true;
            btnReadAddr.Enabled = true;
            btnSleep.Enabled = true;
            btnIDLE.Enabled = true;
            btnSend.Enabled = true;
            btnDestAddr.Enabled = true;
            btnLoRaSendData.Enabled = true;
            btnReadDestAddr.Enabled = true;
            btnLoRaSendLength.Enabled = true;
            btnReadRSSI.Enabled = true;
            btnRX.Enabled = true;
            btnACK.Enabled = true;
            btnAddrEn.Enabled = true;
            btnReadACK.Enabled = true;
            btnReadAddrEnable.Enabled = true;
            btnTransportIn.Enabled = true;
            btnTransportOut.Enabled = true;
            btnSaveParament.Enabled = true;
            btnReadDestAddr.Enabled = true;
            cbAutoSend.Enabled = true;

            cbBaudRate.Enabled = false;
            cbStop.Enabled = false;
            cbData.Enabled = false;
            cbParity.Enabled = false;
        }
        private void disableBtn()
        {

            btnDownload.Enabled = false;
            btnDownload.Text = "download";
            btnSend.Enabled = false;
            btnSettings.Enabled = false;
            btnRestart.Enabled = false;
            btnTest.Enabled = false;
            btnReadVersion.Enabled = false;

            btnAddr.Enabled = false;
            btnReadAddr.Enabled = false;
            btnSleep.Enabled = false;
            btnIDLE.Enabled = false;
            btnSend.Enabled = false;
            btnDestAddr.Enabled = false;
            btnLoRaSendData.Enabled = false;
            btnLoRaSendLength.Enabled = false;
            btnStopDownload.Enabled = false;
            btnReadRSSI.Enabled = false;
            btnRX.Enabled = false;
            btnACK.Enabled = false;
            btnAddrEn.Enabled = false;
            btnReadACK.Enabled = false;
            btnReadAddrEnable.Enabled = false;
            btnTransportIn.Enabled = false;
            btnTransportOut.Enabled = false;
            btnSaveParament.Enabled = false;
            cbAutoSend.Enabled = false;
            btnReadDestAddr.Enabled = false;
            cbBaudRate.Enabled = true;
            cbStop.Enabled = true;
            cbData.Enabled = true;
            cbParity.Enabled = true;
        }
        #endregion


        #region 串口开关控制
        private void btnSwitch_Click(object sender, EventArgs e)
        {
            if (!sp1.IsOpen)
            {
                try
                {
                    string serialName = cbSerial.SelectedItem.ToString();
                    sp1.PortName = serialName;
                    //串口的设置
                    string strBaudRate = cbBaudRate.Text;
                    string strDataBits = cbData.Text;
                    string strStopBits = cbStop.Text;
                    Int32 iBaudRate = Convert.ToInt32(strBaudRate);
                    Int32 iDataBits = Convert.ToInt32(strDataBits);
                    sp1.BaudRate = iBaudRate;
                    sp1.DataBits = iDataBits;
                    switch (cbStop.Text)
                    {
                        case "1":
                            sp1.StopBits = StopBits.One;
                            break;
                        case "1.5":
                            sp1.StopBits = StopBits.OnePointFive;
                            break;
                        case "2":
                            sp1.StopBits = StopBits.Two;
                            break;
                        default:
                            MessageBox.Show("Error:parament err", "Error");
                            break;
                    }
                    switch (cbParity.Text)
                    {
                        case "NONE":
                            sp1.Parity = Parity.None;
                            break;
                        case "Odd":
                            sp1.Parity = Parity.Odd;
                            break;
                        case "Even":
                            sp1.Parity = Parity.Even;
                            break;
                        default:
                            MessageBox.Show("Error：parament err", "Error");
                            break;
                    }
                    if (sp1.IsOpen == true)//如果打开状态，则先关闭一下
                    {

                        sp1.Close();
                    }


                    sp1.Open();
                    cbSerial.Enabled = false;
                    btnSwitch.Text = "close";
                    enableBtn();

                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error:" + ex.Message, "Error");
                   
                    //tmSend.Enabled = false;
                    return;
                }
            }
            else
            {
               
                btnSwitch.Text = "open";
                cbSerial.Enabled = true; 
                if ((t1 != null) )
                    t1.Abort();

                sp1.Close();
                disableBtn();


            }
        }

        #endregion
        private void btnExit_Click(object sender, EventArgs e)
        {
            sp1.Close();
            Application.Exit();
        }


        private void btnClear_Click(object sender, EventArgs e)
        {
            RecvString = null;
            txtRecvChar.Clear();

        }



        private void txtRecv_TextChanged(object sender, EventArgs e)
        {
            //txtRecvChar.Focus();
            this.txtRecvChar.Select(this.txtRecvChar.TextLength, 0);
            this.txtRecvChar.ScrollToCaret();
           // txtRecvChar.SelectionStart = txtRecvChar.Text.Length;
           // txtRecvChar.ScrollToCaret();


        }



        #region  打开文件以及加载


        uint page;
        FileStream fs;
        byte[] binFileArray = new byte[8192];
        int binFileLength = 0;
        OpenFileDialog binFile;

        private void openFile()
        {
            binFile = new OpenFileDialog();
            binFile.Filter = "All file|*bin";
            binFile.RestoreDirectory = true;
            if (binFile.ShowDialog() == DialogResult.OK)
            {

                tbFilePath.Text = binFile.FileName.ToString();

            }
        }
        private bool loadFile()
        {
            for (int i = 0; i < 8192; i++)
            {
                binFileArray[i] = 0;
            }
            
            try
            {
                fs = File.Open(binFile.FileName, FileMode.Open);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: file err" , "Error");

                //tmSend.Enabled = false;
                return false;
            }


            fs.Read(binFileArray, 0, (int)fs.Length);
            binFileLength = (int)fs.Length;
            tsProgressBar.Maximum = binFileLength;
            tbFileSize.Text = binFileLength.ToString() + "Byte";

            fs.Close();
            //txtRecvChar.Text = System.Text.Encoding.Default.GetString(binFileArray);
            string str = null;
            for (int i = 0; i < binFileLength; i++)
            {
                str += "0X";
                str += binFileArray[i].ToString("X02") + " ";
            }
            //txtRecvHex.Text = str;
            return true;
        }
        private void 打开binToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFile();
        }
        private void button2_Click(object sender, EventArgs e)
        {
            openFile();
        }
        private void cbLiangChan_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void btnDownload_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[64];

            RecvString = null;
            txtRecvChar.Clear();
            if(loadFile() == false)
            {
                    buf[0] = 0;
                    sp1.Write(buf, 0, 1);//传输错误命令，系统复位
                    return;
            }
            if(cbLiangChan.Checked == false )
            {
                string str = "AT+RST\r\n";
                for (int i = 0; i < str.Length; i++)
                    buf[i] = Convert.ToByte(str[i]);
                sp1.Write(buf, 0, str.Length);
                Thread.Sleep(100);
            }
 

            //Control.CheckForIllegalCrossThreadCalls = false;
            t1 = new Thread(new ThreadStart(DownloadProcces));
            t1.Start();
            t2.Suspend();

            disableBtn();
            btnDownload.Text = "Downloading...";
            btnStopDownload.Enabled = true;


        }
        private void btnStopDownload_Click(object sender, EventArgs e)
        {
            tsState.Text = "Stop";
            if(t1 != null)
                t1.Abort();
            if(t2.ThreadState == ThreadState.Suspended)
                t2.Resume();
            enableBtn();
            btnStopDownload.Enabled = false;

        }
        #endregion


        #region  下载文件线程
        int state = 0;
        void DownloadProcces()
        {
            byte[] bootStart = new byte[1];
            bootStart[0] = 0xa5;
            int blockBytes = 64;
            int pageCount = 0;
            byte bootWrite = 0xa7;
            byte bootGo = 0xa9;
            byte bootOK = 0xa0;
            byte bootERR = 0xa1;
            byte[] buf = new  byte[64];
            int timeout;
            int startTime = System.Environment.TickCount; ;
            int endTime ;  
            int runTime;
            int retry = 0;
            RecvString += "\r\n====start to download====\r\n";
            RecvString += "please restart your target\r\n";

            while (true)
            {
                switch (state)
                { 
                    case 0://等待链接发送同步字节
                        tsState.Text = "wait sync...";
                        RecvString += "?\t";
                        txtRecvChar.Text = RecvString;
                        sp1.Write(bootStart,0,1);
                        sp1.DiscardInBuffer();
                        state = 1;
                    break;
                    case 1:
                        retry++;
                        if (retry >= 100)
                        {
                            state = 4;
                            break;
                        }

                        timeout = 100;
                        while (sp1.BytesToRead == 0)
                        {
                            Thread.Sleep(1);
                            timeout--;
                            if (timeout == 0) break;
                        }
                        if (timeout == 0)//如果超时未响应，重新发送同步字节
                            state = 0;
                        else
                        {
                            if (sp1.BytesToRead > 0)
                            {
                                byte[] recData = new byte[sp1.BytesToRead];
                                sp1.Read(recData, 0, 1);//一次性读取之前累计的所有数据
                                RecvString += sp1.BytesToRead.ToString();
                                txtRecvChar.Text += RecvString;

                                if ((recData[0] & 0xf0) == 0xa0)//确定得到了起始位
                                {
                                    page = (uint)recData[0] & 0x0f;//得到最后一个字节
                                    state = 2;
                                }
                                //else
                                {
                                 //   state = 0;
                                }
                                startTime = System.Environment.TickCount;

                            }
                        }

                        break;
                    case 2:
                        tsState.Text = "sending...(" + tsProgressBar.Value.ToString() +"bytes)";
                        timeout = 500;
                        byte verify = 0;
                        buf[0] = bootWrite;
                        sp1.Write(buf,0,1);//传输写控制
                        buf[0] = (byte)(page);
                        RecvString += page.ToString() + "\t";
                        txtRecvChar.Text = RecvString;

                        sp1.Write(buf,0,1);//传输页地址
                        Thread.Sleep(1);
                        for (int i = 0; i < blockBytes; i++)
                            verify += binFileArray[pageCount * blockBytes + i];
                        sp1.Write(binFileArray, pageCount * 64, 64);//传输页内容
                        Thread.Sleep(1);
                        buf[0] = verify;
                        sp1.Write(buf, 0, 1);//传输校验和
                        pageCount++;
                        page++;

                        //等待下位机发送校验结果
                        while (sp1.BytesToRead != 1)
                        {
                            Thread.Sleep(1);
                            timeout--;
                            if (timeout == 0) break;
                        }
                        if (timeout == 0)//如果超时未响应，重新发送同步字节
                        {
                            state = 4;
                        }
                        else
                        {
                            byte[] recData = new byte[sp1.BytesToRead];
                            sp1.Read(recData, 0, recData.Length);
                            if (recData[0] == bootOK)
                            {
                                state = 2;
                            }
                            else if (recData[0] == bootERR)
                                state = 4;

                        }
                        if (pageCount * 64 > binFileLength)
                        {
                            state = 3;
                            tsProgressBar.Value = tsProgressBar.Maximum;
                        }
                        else
                            tsProgressBar.Value = pageCount * 64;


                    break;
                    case 3:
                         endTime = System.Environment.TickCount;  
                         runTime=endTime-startTime;
                         RecvString += "\r\nprogram pages:" + pageCount.ToString() + "\r\n";
                         RecvString += "Use time:" + runTime.ToString() + "\r\n";
                         RecvString += "========BootLoader  successed!========\r\n\r\n";
                         txtRecvChar.Text = RecvString;
                        buf[0] = bootGo;
                        sp1.Write(buf,0,1);//传输写控制
                        state = 0;
                        tsState.Text = "sended" + "(" + tsProgressBar.Value+ "bytes)";
                        //tsProgressBar.Value = tsProgressBar.Minimum;

                        enableBtn();
                        t2.Resume();
                        btnStopDownload.Enabled = false;
                        if (cbLiangChan.Checked == true)
                        {
                            btnDownload.PerformClick();
                        }
                        return;
                        break;
                    case 4:
                        state = 0;
                        endTime = System.Environment.TickCount;
                        runTime = endTime - startTime;
                        tsState.Text = "send failed" + "(" + tsProgressBar.Value + ")";
                        RecvString += "\r\nprogram pages:" + pageCount.ToString() + "\r\n";
                        RecvString += "Use time:" + runTime.ToString() + "\r\n";
                        RecvString += "========BootLoader  failed!========\r\n\r\n";

                        txtRecvChar.Text = RecvString;

                        enableBtn();
                        t2.Resume();
                        btnStopDownload.Enabled = false;
                        if (cbLiangChan.Checked == true)
                        {
                            btnDownload.PerformClick();
                        }
                        return;
                        break;
                    default:
                        state = 0;
                    break;

                }

            }
        }
        #endregion


        #region 串口接收线程
        private void serialRecv()
        {
            while (true)
            {
                if (sp1.IsOpen == true)
                {
                    if (t1 == null  || t1.ThreadState != ThreadState.Running)
                    {
                        if (sp1.BytesToRead > 0)
                        {
                            byte[] recData = new byte[sp1.BytesToRead];
                            sp1.Read(recData, 0, recData.Length);
                            for (int i = 0; i < recData.Length;i++ )
                            {
                                RecvString += (char)recData[i];
                                //updataConfig(recData[i]);
                                updataAddr(recData[i]);
                                updataDestAddr(recData[i]);
                                updataACK(recData[i]);
                                updataAddrEnable(recData[i]);

                            }
                            txtRecvChar.Text = RecvString;

                        }
                    }

                }
            }
        
        }
        #endregion

        #region 根据读取串口的数据更新控制面板
        int cmdByteCount = 0;
        byte[] config = new byte[100];

        void updataConfig(byte ch)
        {
            if (ReadConfig == 0) return;
            if (ch != '\r')
            {
                config[cmdByteCount++] = ch;
            }
            else
            {
                ReadConfig = 0;
                cmdByteCount = 0; 
                if (config[1] == 'R')
                {
                    return;
                }

                string str = System.Text.Encoding.Default.GetString(config);
                string[] strArray = str.Split(',');
                strArray[12] = strArray[12].Substring(0, 4);
                UInt32[] para = new UInt32[13];
                UInt32 x;
                for (int i = 0; i < strArray.Length; i++)
                {
                    para[i] = (UInt32)Convert.ToInt32(strArray[i], 16);
                }
                tbPFrq.Text = para[0].ToString();
                tbPPower.Text = para[1].ToString();
                tbPBW.Text = para[2].ToString();
                tbPSF.Text = para[3].ToString();
                tbPER.Text = para[4].ToString();
                if (para[5] == 1)
                    cbCRC.SelectedIndex = 0;
                else
                    cbCRC.SelectedIndex = 1;

                if (para[6] == 1)
                    cbHeader.SelectedIndex = 0;
                else
                    cbHeader.SelectedIndex = 1;

                if (para[7] == 1)
                    cbRXSingle.SelectedIndex = 0;
                else
                    cbRXSingle.SelectedIndex = 1;

                if (para[8] == 1)
                    cbHop.SelectedIndex = 0;
                else
                    cbHop.SelectedIndex = 1;

                tbPHopTime.Text = para[9].ToString();
                tbPRXTimeout.Text = para[10].ToString();
                tbPPayloadLen.Text = para[11].ToString();
                tbPPreambleLen.Text = para[12].ToString();
                
                    
                   

            }

        }
        void updataAddr(byte ch)
        {
            if (ReadAddFlag == 0) return;

            if (ch != '\r')
            {
                config[cmdByteCount++] = ch;
            }
            else
            {
                ReadAddFlag = 0;
                cmdByteCount = 0;
                string str = System.Text.Encoding.Default.GetString(config);
                str = str.Substring(3, 4);
                tbAddr.Text = str;

            }
        }
        void updataDestAddr(byte ch)
        {
            if (ReadDestAddrFlag == 0) return;
            if (ch != '\r')
            {
                config[cmdByteCount++] = ch;
            }
            else
            {
                ReadDestAddrFlag = 0;
                cmdByteCount = 0;
                string str = System.Text.Encoding.Default.GetString(config);
                str = str.Substring(3, 4);
                tbDestAddr.Text = str;
            }
        }
        void updataACK(byte ch)
        {
            if (ReadACKFlag == 0) return;
            if (ch != '\r')
            {
                config[cmdByteCount++] = ch;
            }
            else
            {
                ReadACKFlag = 0;
                cmdByteCount = 0;
                string str = System.Text.Encoding.Default.GetString(config);
                str = str.Substring(3, 1);
                tbACK.Text = str;
            }
        }
        void updataAddrEnable(byte ch)
        {
            if (ReadAddrEnableFlag == 0) return;
            if (ch != '\r')
            {
                config[cmdByteCount++] = ch;
            }
            else
            {
                ReadAddrEnableFlag = 0;
                cmdByteCount = 0;
                string str = System.Text.Encoding.Default.GetString(config);
                str = str.Substring(3, 1);
                tbAddrEnable.Text = str;
            }
        }
        #endregion

        #region LoRa命令设置功能
        private void ParamentLoad()
        {
            tbPFrq.Text = "433000000";
            tbPPower.Text = "20";
            tbPBW.Text = "9";
            tbPSF.Text = "10";
            tbPER.Text = "1";
            cbCRC.Items.Add("TRUE");
            cbCRC.Items.Add("FALSE");
            cbCRC.SelectedIndex = 0;

            cbHeader.Items.Add("TRUE");
            cbHeader.Items.Add("FALSE");
            cbHeader.SelectedIndex = 1;

            cbRXSingle.Items.Add("TRUE");
            cbRXSingle.Items.Add("FALSE");
            cbRXSingle.SelectedIndex = 1;


            cbHop.Items.Add("TRUE");
            cbHop.Items.Add("FALSE");
            cbHop.SelectedIndex = 1;

            tbPHopTime.Text = "0";
            tbPRXTimeout.Text = "3000";
            tbPPayloadLen.Text = "8";
            tbPPreambleLen.Text = "4";


        }
        private void btnSettings_Click(object sender, EventArgs e)
        {
            string str = null;
            str += "AT+CFG=";
            str += tbPFrq.Text + ",";
            str += tbPPower.Text + ",";
            str += tbPBW.Text + ",";
            str += tbPSF.Text + ",";
            str += tbPER.Text + ",";

            if (cbCRC.Text == "TRUE")
                str += "1,";
            else
                str += "0,";
            if (cbHeader.Text == "TRUE")
                str += "1,";
            else
                str += "0,";
            if (cbRXSingle.Text == "TRUE")
                str += "1,";
            else
                str += "0,";
            if (cbHop.Text == "TRUE")
                str += "1,";
            else
                str += "0,";


            str += tbPHopTime.Text + ",";
            str += tbPRXTimeout.Text + ",";
            str += tbPPayloadLen.Text + ",";
            str += tbPPreambleLen.Text;
            str += "\r\n";

            byte[] buf = new byte[64];
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+SAVE\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
            ReadConfig = 1;
        }

        

        private void btnTest_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;


        }
        private void btnRestart_Click(object sender, EventArgs e)
        {

            byte[] buf = new byte[256];
            string str = "AT+RST\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;


        }
        private void btnSleep_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+SLEEP=1\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }
        private void btnWakeUp_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+IDLE\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnRX_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+RX\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnReadVersion_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+VER\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

        }

        private void RadioBtnPB0_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str;
            if (((RadioButton)sender).Text.ToString() == "SET")
            {
                str = "AT+PB0=1\r\n";
            }
            else
            {
                str = "AT+PB0=0\r\n";
            }
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

        }

        private void RadioBtnPD0_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str;
            if (((RadioButton)sender).Text.ToString() == "SET")
            {
                str = "AT+PD0=1\r\n";
            }
            else
            {
                str = "AT+PD0=0\r\n";
            }
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

        }

        private void RadioBtnPC4_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str;
            if (((RadioButton)sender).Text.ToString() == "SET")
            {
                str = "AT+PC4=1\r\n";
            }
            else
            {
                str = "AT+PC4=0\r\n";
            }
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

        }
 
        private void btnLoRa_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[1024];
            string str =  tbLoRa.Text ;
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }
        private void btnAddr_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = null;
            if(tbAddr.Text != "")
                 str = "AT+ADDR=" +  tbAddr.Text  + "\r\n";
            else
                str = "AT+ADDR=\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnReadAddr_Click(object sender, EventArgs e)
        {


            byte[] buf = new byte[256];
            string str = "AT+ADDR?\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;


            ReadAddFlag = 1;
            //Thread.Sleep(100);
            //byte[] recData = new byte[sp1.BytesToRead];
            //sp1.Read(recData, 0, 4);//一次性读取之前累计的所有数据

            //str = System.Text.Encoding.Default.GetString(recData);

        }
        private void btnDestAddr_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = null;

            if (tbDestAddr.Text != "")
                str = "AT+DEST=" + tbDestAddr.Text + "\r\n";
            else
                str = "AT+DEST=\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
            
        }
        private void btnReadACK_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+ACK?\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;


            ReadACKFlag = 1;
        }
        private void btnReadAddrEnable_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+ADDREN?\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;


            ReadAddrEnableFlag = 1;
        }

        private void btnReadDestAddr_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+DEST?\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
            ReadDestAddrFlag = 1;
        }
        private void btnLoRaSendLength_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+SEND=" +  tbLoRaSendLength.Text + "\r\n";
            
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }
        private void btnReadRSSI_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+RSSI?\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }



        private void btnACK_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+ACK=" + tbACK.Text  + "\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnTransportIn_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+TSP\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnAddrEn_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+ADDREN=" + tbAddrEnable.Text + "\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        private void btnTransportOut_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "+++";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;
        }

        #endregion

        #region 发送按键
        private void btnSend_Click_1(object sender, EventArgs e)
        {
            byte[] buf = new byte[1024];
            string str = tbSend.Text.ToString();
            if (cbEnter.Checked == true)
                str += "\r\n";
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
        }
        #endregion
        
        #region 其他功能
        private void 版本ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            help form1 = new help();
            form1.Show();
        }
        #endregion

        #region 关闭事件

        private void main_FormClosing(object sender, FormClosingEventArgs e)
        {
            sp1.Close();
            if(t1 != null)
            t1.Abort();
            t2.Abort();
            Application.Exit();
        }
        #endregion



        #region 自动发送功能
        private void cbAutoSend_CheckedChanged(object sender, EventArgs e)
        {
            if (cbAutoSend.Checked == true)
            {
                timer1.Enabled  = true;
                timer1.Interval =Convert.ToInt32(tbSendGapTime.Text);
            }
            else
            {
                timer1.Enabled = false;

            }

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            byte[] buf = new byte[256];
            string str = "AT+SEND=" + tbLoRaSendLength.Text + "\r\n";

            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

            Thread.Sleep(200);

            str = tbLoRa.Text;
            for (int i = 0; i < str.Length; i++)
                buf[i] = Convert.ToByte(str[i]);
            sp1.Write(buf, 0, str.Length);
            tsLoRaCMD.Text = str;

        }
        #endregion



        #region 输入限制
        private void textInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((char.IsNumber(e.KeyChar)) ||
                (e.KeyChar >= 'a' && e.KeyChar <= 'f') ||
                (e.KeyChar >= 'A' && e.KeyChar <= 'F')) &&
                (tbAddr.Text.Count() < 4) ||
                (e.KeyChar == (char)8))
            {


            }
            else
            {
                e.Handled = true;
                // MessageBox.Show("请输入十六进制字符", "err");
            }

        }
        private void textInput16_4_Dest_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((char.IsNumber(e.KeyChar)) ||
                (e.KeyChar >= 'a' && e.KeyChar <= 'f') ||
                (e.KeyChar >= 'A' && e.KeyChar <= 'F')) &&
                (tbDestAddr.Text.Count() < 4) ||
                (e.KeyChar == (char)8))
            {


            }
            else
            {
                e.Handled = true;
                // MessageBox.Show("请输入十六进制字符", "err");
            }

        }
        private void textInput10_5_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char.IsNumber(e.KeyChar) && (tbSendGapTime.Text.Count() < 5)) || (e.KeyChar == (char)8))
            {


            }
            else
            {
                e.Handled = true;
                // MessageBox.Show("请输入十六进制字符", "err");
            }

        }
        #endregion



    }
}

