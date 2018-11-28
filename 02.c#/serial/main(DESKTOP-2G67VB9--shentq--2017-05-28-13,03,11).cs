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
        public main()
        {
            InitializeComponent();
        }

        private void main_Load(object sender, EventArgs e)
        {
            //检查是否含有串口
  
            string[] str = SerialPort.GetPortNames();
            if (str == null)
            {
                MessageBox.Show("本机没有串口", "Error");
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
            cbParity.Items.Add("无");
            cbParity.Items.Add("奇校验");
            cbParity.Items.Add("偶校验");
            cbParity.SelectedIndex = 0;
            cbTime.SelectedIndex = 0;
            cbAuto.SelectedIndex = 2;
            //添加串口
            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                cbSerial.Items.Add(s);
            }
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

            Control.CheckForIllegalCrossThreadCalls = false;
            sp1.DataReceived += new SerialDataReceivedEventHandler(sp1_DataReceived);

            Thread t1 = new Thread(new ThreadStart(TestMethod));
            t1.Start();
        }
        void TestMethod()
        {
            txtRecvChar.Text += "不带参数的线程函数";
            //Console.WriteLine("不带参数的线程函数");
        }

        void sp1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int err = 0;
            if (sp1.IsOpen)
            {
                try
                {
                    protocol prot1 = new protocol();
                    Byte[] receiveData = new Byte[sp1.BytesToRead];
                    sp1.Read(receiveData, 0, receiveData.Length);
                    sp1.DiscardInBuffer();
                    byte[] strRcv = new byte[receiveData.Length];
                    //string strRcv1 = Encoding.Default.GetString(receiveData);

                    
                    string strRcvHex = null;
                    string strRcvChar = null;
                    for (int i = 0; i < receiveData.Length; i++)
                    {
                        strRcvHex += " 0x";
                        strRcvHex += receiveData[i].ToString("X");
                        strRcvChar += (char)receiveData[i];

                    }
 
                    err = prot1.RcvDeal(receiveData);

                    if (err == 0)
                    {
                        //截取帧头
                        string str = null;
                        str += "0x";
                        str += prot1.start1.ToString("X");
                        str += " 0x";
                        str += prot1.start2.ToString("X");
                        tbRStart.Text = str;
                        //获得Mac
                        str = null;
                        str += "0X";
                        str += prot1.mac.ToString("X");
                        tbRAddr.Text = str;
                        //获得cmd
                        str = null;
                        str += "0X";
                        str += prot1.cmd.ToString("X");
                        tbRCmd.Text = str;
                        //获得cmd
                        str = null;
                        str += "0X";
                        str += prot1.datalength.ToString("X");
                        tbRValidDataLen.Text = str;
                        //获得帧尾
                        str = null;
                        str += "0x";
                        str += prot1.end1.ToString("X");
                        str += " 0x";
                        str += prot1.end2.ToString("X");
                        tbREnd.Text = str;
                        str = null;
                        str = prot1.vrms.ToString();
                        tbVrms.Text = str;
                        str = null;
                        str = prot1.irms.ToString();
                        tbIrms.Text = str;
                        str = null;
                        str = prot1.p.ToString();
                        tbP.Text = str;
                        str = null;
                        str = prot1.ep.ToString();
                        tbEp.Text = str;
                        str = null;
                        if (prot1.outputstatu == 1)
                            str = "开启";
                        else
                            str = "关闭";
                        tbOutput.Text = str;
                        str = null;
                        str += "0x";
                        str += prot1.cksum1.ToString("X");
                        tbRCkSum1.Text = str;
                        str = null;
                        str += "0x";
                        str += prot1.cksum2.ToString("X");
                        tbRCkSum2.Text = str;
                        str = null;
                        str += "0x";
                        str += prot1.MAX_DATALEN.ToString("X");
                        tbRMaxDataLen.Text = str;
                        tsCmd.Text = "接收完成";
                        timerRepeat.Enabled = false;
                        repeattimes = 0;
  
                    }
                    else
                    {
                        ;
                    }


                   
                    txtRecvChar.Text += strRcvChar;
                    txtRecvHex.Text += strRcvHex + "\r\n";
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message, "出错提示");
                    
                }
            }
        }

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
                            MessageBox.Show("Error:参数不正确", "Error");
                            break;
                    }
                    switch (cbParity.Text)
                    {
                        case "无":
                            sp1.Parity = Parity.None;
                            break;
                        case "奇校验":
                            sp1.Parity = Parity.Odd;
                            break;
                        case "偶校验":
                            sp1.Parity = Parity.Even;
                            break;
                        default:
                            MessageBox.Show("Error：参数不正确!", "Error");
                            break;
                    }
                    if (sp1.IsOpen == true)//如果打开状态，则先关闭一下
                    {
                        sp1.Close();
                    }
                    cbSerial.Enabled = false;
                    //cbBaudRate.Enabled = false;
                    //cbData.Enabled = false;
                    //cbStop.Enabled = false;
                    //cbParity.Enabled = false;
                    sp1.Open();
                    btnSwitch.Text = "关闭串口";
                    btnSend.Enabled = true;
                    ckbAuto.Enabled = true;

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
                cbSerial.Enabled = true;
                ckbAuto.Enabled = false;
                //cbBaudRate.Enabled = true;
                //cbData.Enabled = true;
                //cbStop.Enabled = true;
                //cbParity.Enabled = true;

                sp1.Close();
                btnSwitch.Text = "打开串口";
                btnSend.Enabled = false;

            }
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            sp1.Close();
            Application.Exit();
        }

        private void tabPage2_Click(object sender, EventArgs e)
        {

        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            txtRecvChar.Clear();


            tbRStart.Clear();
            tbRAddr.Clear();
            tbRCmd.Clear();
            tbRValidDataLen.Clear();
            tbRCkSum1.Clear();
            tbRCkSum2.Clear();
            tbREnd.Clear();
            tbVrms.Clear();
            tbIrms.Clear();
            tbP.Clear();
        }


        private void btnSend_Click(object sender, EventArgs e)
        {
            protocol prot = new protocol();

            UInt16 addr = Convert.ToUInt16(tbSAddr.Text);
            UInt16 cmd = Convert.ToUInt16(tbSCmd.Text);
 
            byte[] array = prot.PrepareCmd(addr,cmd);
            string str = null;
            str += "0x";
            str += prot.sendcksum.ToString("X");
            tbSCkSum.Text = str;
            try
            { 
                sp1.Write(array, 0, array.Length);
                tsCmd.Text = "发送完成，并等待回复";
                if (!ckbAuto.Checked)
                    timerRepeat.Enabled = true;
                else
                    timerRepeat.Enabled = false;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message, "出错提示");
            }


        }

        private void tbSAddr_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char.IsNumber(e.KeyChar)) || (e.KeyChar == (char)8))
            {

            }
            else
            {
                e.Handled = true;
                MessageBox.Show("请输入0~9之间的数字", "err");
            }

        }
        private void tbSCmd_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char.IsNumber(e.KeyChar)) || (e.KeyChar == (char)8))
            {

            }
            else
            {
                e.Handled = true;
                MessageBox.Show("请输入0~9之间的数字", "err");
            }
        }
        private void txtRecv_TextChanged(object sender, EventArgs e)
        {
            //txtRecvChar.Focus();
            this.txtRecvChar.Select(this.txtRecvChar.TextLength, 0);
            this.txtRecvChar.ScrollToCaret();

        }

        private void txtRecvHex_TextChanged(object sender, EventArgs e)
        {
            //txtRecvHex.Focus();
            this.txtRecvHex.Select(this.txtRecvHex.TextLength, 0);
            this.txtRecvHex.ScrollToCaret();
        }
        int repeattimes = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
               
                int isec = int.Parse(cbTime.Text) * 500;
                timerRepeat.Interval = isec;
                if (timerRepeat.Enabled == true)
                {
                    repeattimes++;
                    btnSend.PerformClick();
                    tsCmd.Text = "重发：" + repeattimes.ToString() + "次";
                }
            }
            catch(System.Exception ex)
            {
                MessageBox.Show("err", "err");
            }

        }

        private void ckbAuto_CheckedChanged(object sender, EventArgs e)
        {
            if (ckbAuto.Checked)
            {
                timerAuto.Enabled = true;
     
            }
            else
            {
                timerAuto.Enabled = false;
               
            }
                
        }

        private void timerAuto_Tick(object sender, EventArgs e)
        {
            try
            {
                int i = 0;
                int isec = int.Parse(cbAuto.Text) * 1000;
                timerAuto.Interval = isec;
                if (timerAuto.Enabled == true)
                {
                    btnSend.PerformClick();
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("err", "err");
            }

        }

        private void 打开binToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog binFile = new OpenFileDialog();
            binFile.Filter = "所有文件|*txt";
            byte[] byteArray = new byte[1024];
            if(binFile.ShowDialog() == DialogResult.OK)
            {
                
                FileStream fs = new  FileStream(binFile.FileName,FileMode.Open);
                fs.Read(byteArray, 0, 1024);
                txtRecvChar.Text = System.Text.Encoding.Default.GetString(byteArray);
            }
        }

        private void btnDownload_Click(object sender, EventArgs e)
        {

        }








    }
}
