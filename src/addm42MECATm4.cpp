#include<iostream>
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<string>
#include<math.h>
#include<cstdlib>
#include"string.h"
#include<malloc.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<sys/types.h>

using namespace std;

string BEFORE="";
string AFTER="";
string realireads="";
string allreads="";
string ADD="";
char **sample = NULL;
char **readname=NULL;
int *length=NULL;
int *name_length=NULL;
int read_count=0;
char **TmpReadName=NULL;
int *TmpReadNameLength=NULL;
int TmpReadName_count=0;

FILE * f_BEFORE;
FILE * f_ADD;
FILE * f_allreads;
FILE * f_realireads;
int i=0,j=0;
string Qname="";
string Tname="";
ofstream fout;
int parseargs(int argc, char * argv[])
{
	BEFORE=argv[1];
	ADD=argv[2];
	realireads=argv[3];
	AFTER=argv[4];
}

int countFileLineNum(string filename)
{
	ifstream aFile(filename.c_str());
	int lines_count = 0;
	string line;
	while(std::getline(aFile,line))
		++ lines_count;

	return lines_count;
}
int num_realiread = 0;
int num_m4info = 0;

typedef struct m4
{
	string flag1;
	string flag2;
	string flag3;
	string flag4;
	string flag5;
	string flag6;
	string flag7;
	string flag8;
	string flag9;
	string flag10;
	string flag11;
	string flag12;
	string flag13;
}m4;

m4 *ADDname = NULL;
string * realiread= NULL;


void free_memory()
{
	delete[] ADDname;
	delete[] realiread;
}

int countADDm4=0;
void readm4()
{
	char ch;
//	ch=fgetc(f_ADD)
//by bao: fgetc needs empty judgement before hand
	if(f_ADD != NULL) ch=fgetc(f_ADD); else return;
	int flag=0;

	while(!feof(f_ADD))
	{
		if(ch==-1)
		{
			break;
		}
		
		if(ch=='\t'&&flag==0)
		{
			flag=1;
			ADDname[countADDm4].flag1=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==1)
		{
			flag=2;
			ADDname[countADDm4].flag2=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==2)
		{
			flag=3;
			ADDname[countADDm4].flag3=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==3)
		{
			flag=4;
			ADDname[countADDm4].flag4=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==4)
		{
			flag=5;
			ADDname[countADDm4].flag5=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==5)
		{
			flag=6;
			ADDname[countADDm4].flag6=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==6)
		{
			flag=7;
			ADDname[countADDm4].flag7=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==7)
		{
			flag=8;
			ADDname[countADDm4].flag8=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==8)
		{
			flag=9;
			ADDname[countADDm4].flag9=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==9)
		{
			flag=10;
			ADDname[countADDm4].flag10=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==10)
		{
			flag=11;
			ADDname[countADDm4].flag11=Tname;
			Tname.clear();
		}
		else if(ch=='\t'&&flag==11)
		{
			flag=12;
			ADDname[countADDm4].flag12=Tname;
			Tname.clear();
		}
		
		else if(ch=='\n')
		{
			flag=0;
			ADDname[countADDm4].flag13=Tname;
			Tname.clear();
			countADDm4++;
		}
		else
		{
			
				Tname+=ch;
			
		}

		ch=fgetc(f_ADD);
	}
	fclose(f_ADD);
}
int countread=0;
void readreali()
{
	char ch;
	ch=fgetc(f_realireads);
	int flag1=0;
	
	while(!feof(f_realireads))
	{	
		if(ch==-1)
		{
			break;
		}
		
		if(ch=='\n'&&flag1==0)
		{
			flag1=1;
			realiread[countread]=Qname;
			Qname.clear();
		}
		else if(ch=='\n'&&flag1==1)
		{
			flag1=0;
			countread++;
		}
		
		else
		{
			if(flag1==0&&ch!='>')
			{
				Qname+=ch;
			}
			
		}

		ch=fgetc(f_realireads);
	}
	fclose(f_realireads);

}
int countreads=0;
/*
void readallreads()
{
	char ch;
	string Qname="";
	ch=fgetc(f_allreads);
	int flag2=0;
	
	while(!feof(f_allreads))
	{	
		if(ch==-1)
		{
			break;
		}
		
		if(ch=='\n'&&flag2==0)
		{
			flag2=1;
			allreadslist[countreads]=Qname;
		}
		else if(ch=='\n'&&flag2==1)
		{
			flag2=0;
			countreads++;
		}
		
		else
		{
			if(flag2==0&&ch!='>')
			{
				Qname+=ch;
			}
			
		}

		ch=fgetc(f_allreads);
	}
	fclose(f_allreads);
}
*/
void write()
{
	for(int i=0;i<countADDm4;i++)
	{
		//cout<<i<<endl;
		int num=0;
		int alinum1=0;
		int alinum2=0;
		string read1="";
		string read2="";
		//alinum1=atoi(ADDname[i][0].c_str());
		//alinum2=atoi(ADDname[i][1].c_str());
		read1=realiread[atoi(ADDname[i].flag1.c_str())].c_str();
		read2=realiread[atoi(ADDname[i].flag2.c_str())].c_str();

		
		
		fout<<read1<<"\t"<<read2<<"\t"<<ADDname[i].flag3<<"\t"<<ADDname[i].flag4<<"\t"<<ADDname[i].flag5<<"\t"<<ADDname[i].flag6<<"\t"<<ADDname[i].flag7<<"\t"<<ADDname[i].flag8<<"\t"<<ADDname[i].flag9<<"\t"<<ADDname[i].flag10<<"\t"<<ADDname[i].flag11<<"\t"<<ADDname[i].flag12<<"\t"<<ADDname[i].flag13<<"\n";

	}
}

void ReadSample()
{
	char ch;
	ch=fgetc(f_BEFORE);

	while(!feof(f_BEFORE))
	{	
		if(ch==-1)
		{
			break;
		}
		fout<<ch;
		ch=fgetc(f_BEFORE);
		
	}
	fclose(f_BEFORE);

}


int main(int argc,char * argv[])
{
	parseargs(argc,argv);

	const char* filename;
	filename=BEFORE.c_str();
	f_BEFORE=fopen(filename,"r");

	const char* filename1;
	filename1=AFTER.c_str();
	fout.open(filename1,ios::trunc);
	
	const char* filename2;
	filename2=ADD.c_str();
	f_ADD=fopen(filename2,"r");
	
	
	
	const char* filename3;
	filename3=realireads.c_str();
	f_realireads=fopen(filename3,"r");
	
	if(f_BEFORE==NULL)
	{
		perror("open");
	}
	num_m4info = countFileLineNum(ADD);	
	num_realiread = countFileLineNum(realireads) / 2;
//	cout<<"num_m4info "<<num_m4info<<endl;
//	cout<<"num_realiread"<<num_realiread<<endl;
	ADDname = new m4[num_m4info];
//	cout<<"ADDname = new m4[num_m4info];"<<num_m4info<<endl;
	realiread = new string[num_realiread];
//	cout<<"	realiread = new string[num_realiread];"<<num_realiread<<endl;
	ReadSample();
//	cout<<"	ReadSample();"<<endl;
	readm4();
//	cout<<"	readm4();"<<endl;
	readreali();
//	cout<<"	readreali();"<<endl;

	//readallreads();
	write();
//	cout<<"	write();"<<endl;

	free_memory();
	return 0;
}
