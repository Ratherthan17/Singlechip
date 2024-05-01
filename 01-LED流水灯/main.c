#include <REGX52.H>
//#include <windows.h>//单片机中没有这两个头文件
//#include <unistd.h>//所以要自己写延时函数

//延时函数，while 里的内容由STC的 "软件延时计算器"生成，
//系统频率12MHz，定时长度为1ms；8051指令集：STC-Y1
//执行1次，延时1秒，循环time次，即延时time毫秒
//time 单位为 毫秒 
void Delay(unsigned int time)
{
	while(time--)
	{
		unsigned char i, j;

		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}		
}


//移位法——单个灯亮
void Light1(unsigned int time)
{
	int i = 0;
	for(i=0; i<8; i++)
	{
		//右移0次为 1000 0000，取反后是 0111 1111
		//右移1次为 0100 0000，取反后是 1011 1111
		//......
		P2 = ~(0x80>>i);//地址是无符数整型，右移空出的高位用0填补
		
		Delay(time);
	}
	
}

//除以2法——单个灯亮
void Light2(unsigned int time)
{
	int i = 0;
	int j = 0x80;//1000 0000
	for(i = 0; i<8; i++)
	{				
		if(i == 0)
		{
			P2 = ~j;//0111 1111
		}
		else
		{
			//1000 0000 (128)每次除以2，1会向右移1位
			//0100 0000 (64)
			//0010 0000 (32)
			//......
			P2 = ~(j / 2);
			j /= 2;
		}
		
		Delay(time);
	}
}

//逐渐点亮所有灯——移位法
void Light3(unsigned int time)
{
	int i = 0;
	for(i=0; i<9; i++)
	{
		if(i==0)
		{
			P2 = 0xff;//一开始全灭
		}
		else
		{
			//0x7f 0111 1111
			P2 = (0x7f>>(i-1));
		}		
		Delay(time);
	}		
}

//灯逐渐熄灭
//temp 一开始 0000 0000 
//j 1000 0000 -> 0100 0000 -> 0010 0000 -> 0001 0000 ......
// temp + j 1000 0000 -> 1100 0000 -> 1110 0000 ......
void Light4(unsigned int time)
{		
	int i = 0;
	int j = 0x80;//1000 0000
	int temp = 0x00;
	
	//P2 = 0x00;//一开始全亮
	
	//Delay(5);	
	
	for(i=0; i<9; i++)
	{
		if(i==0)
		{
			P2 = 0x00;//一开始全亮			
		}
		else if(i==8)	//全灭		
		{
			P2 = 0xff;
		}
		else
		{
			temp += j;
			P2 = temp;//直接 P2 += j 不行
			j /= 2;						
		}		

		Delay(time);
	}

}

//灯逐渐点亮，又逐渐熄灭
//其实就是Light3和Light4合在一起
void Light5(unsigned int time)
{
	char i = 0;
	int j = 0x80;//1000 0000
	int temp = 0x00;
	
	for(i=0; i<9; i++)
	{
		if(i==0)
		{
			P2 = 0xff;//一开始全灭
		}
		else
		{
			//0x7f 0111 1111
			P2 = (0x7f>>(i-1));
		}		
		Delay(time);
	}		
	
	
	for(i=0; i<9; i++)
	{
		if(i==0)
		{
			P2 = 0x00;//一开始全亮			
		}
		else if(i==8)	//全灭		
		{
			P2 = 0xff;
		}
		else
		{
			temp += j;
			P2 = temp;//直接 P2 += j 不行
			j /= 2;						
		}		

		Delay(time);
	}
	
	
}


void main()
{
	//int i = 0;
	
	
	while(1)
	{
		//Light1();//移位法——单个灯亮
		//Light2();//除以2法——单个灯亮
		//Light3();//逐渐点亮所有灯——移位法
		//Light4();	//灯逐渐熄灭
			Light5(300);//灯逐渐点亮，又逐渐熄灭
				
		//Sleep(2000);		
		//usleep(500000);//延迟 0.5s
				
	}
	
}