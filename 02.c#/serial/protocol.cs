using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;



namespace serial
{

    class protocol
    {

        public byte start1;//0
        public byte start2;//1
        public UInt16 mac;	//2
        public UInt16 cmd;	//4
        public UInt16 datalength;	//6
        public char[] data;			//8
        public UInt32 cksum1;			//12
        public byte end1;				//16
        public byte end2;				//17

        public float vrms;
        public float irms;
        public float p;
        public float ep;
        public float outputstatu;

        public UInt32 sendcksum;
        public UInt32 cksum2;
        public UInt16  MAX_DATALEN = 100;
        public int RcvDeal(byte[] arry)
        {
            Int16 i;
            Int16 seek;
            int ret = 0;
            int IsOK = 0;
            if (arry.Length < 116)
                return -1;
            for (i = 0; i < arry.Length; i++)
            {
                if (arry[i] == 0x55) break;
         
            }
            for (; i < arry.Length; i++)
            {
                if (arry[i] == 0XAA) break;
            
            }
            if ((arry[i + 111] == 0x3e)&&(arry[i + 112] == 0x3e))
            {
                seek = (Int16)(i - 1);
                ret = 0;
            }
            else
            {
                seek = 0;
                ret = -1;
            }
            //对比校验和
            cksum1 = BitConverter.ToUInt32(arry, seek + MAX_DATALEN + 8);
            cksum2 = 0;
            for (i = 0; i < MAX_DATALEN + 8; i++)
            {
                cksum2 += arry[i];
            }
            if (cksum1 == cksum2)
                IsOK = 1;
            else
                IsOK = 0;

            if (IsOK == 1)//校验和正常
            {
                start1 = arry[seek + 0];
                start2 = arry[seek + 1];
                mac = BitConverter.ToUInt16(arry, seek + 2);
                cmd = BitConverter.ToUInt16(arry, seek + 4);
                datalength = BitConverter.ToUInt16(arry, seek + 6);
                vrms = BitConverter.ToSingle(arry, seek + 8);
                irms = BitConverter.ToSingle(arry, seek + 12);
                p = BitConverter.ToSingle(arry, seek + 16);
                ep = BitConverter.ToSingle(arry, seek + 20);
                outputstatu = arry[seek + 24];
                end1 = arry[seek + MAX_DATALEN + 12];
                end2 = arry[seek + MAX_DATALEN + 13];
            }
            else//校验和错误
            {
                start1 = 0;
                start2 = 0;
                mac = 0;
                cmd = 0;
                datalength = 0;
                vrms = 0;
                irms = 0;
                p = 0;
                ep = 0;
                outputstatu = 0;
                end1 = 0;
                end2 = 0;
            }

            return ret;
        } 
        public byte[] PrepareCmd(UInt16 addr,UInt16 cmd)
        {
            byte[] array = new byte[116];
            byte[] addrtemp = BitConverter.GetBytes(addr);
            byte[] cmdtemp = BitConverter.GetBytes(cmd);
            array[0] = 0x55;
            array[1] = 0xaa;
            array[2] = addrtemp[0];
            array[3] = addrtemp[1];
            array[4] = cmdtemp[0];
            array[5] = cmdtemp[1];
            for (int i = 0; i < MAX_DATALEN + 2; i++)
            {
                array[i + 6] = 0;
            }
            sendcksum = 0;
            for (int i = 0;i < MAX_DATALEN + 8;i++)
            {
                sendcksum +=array[i];
            }
            byte[] sendcksumarray = BitConverter.GetBytes(sendcksum);
            array[MAX_DATALEN + 8] = sendcksumarray[0];
            array[MAX_DATALEN + 9] = sendcksumarray[1];
            array[MAX_DATALEN + 10] = sendcksumarray[2];
            array[MAX_DATALEN + 11] = sendcksumarray[3];
            array[MAX_DATALEN + 12] = 0x3e;
            array[MAX_DATALEN + 13] = 0x3e;
            array[MAX_DATALEN + 14] = 0;
            array[MAX_DATALEN + 15] = 0;


                return array;
        }
    }

}
