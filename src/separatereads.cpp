#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <string.h>
#include<vector>
#include<fstream>

using namespace std;
char ch;

FILE *f_corrected;
FILE *f_all;
FILE *f_incorrected;

string method="";

int readslist[200000000]={0};

int read_genome_num=0;
ofstream fout;
void breakpoint()
{

}

void cigar(FILE* file)
{
	int read_index=0;
	string str_readname="";
	ch=fgetc(file);
	int flag=0;
	while (!feof(file))
	{
		int flag_invalid=0;
		
		if(ch=='_'&&flag==0)
		{
			
			read_index=atoi(str_readname.c_str());
			str_readname.clear();
			readslist[read_index]=1;
			while(!feof(file))
			{
				ch=fgetc(file);
				if(ch=='\n')
				{
					flag=1;
					break;
				}
			}
			
		}
		else if(ch!='>')
		{
			str_readname+=ch;
		}
		if(flag==1)
		{
			while(!feof(file))
			{
				ch=fgetc(file);
				if(ch=='\n')
				{
					flag=0;
					break;
				}
			}
		}
		
		ch=fgetc(file);
	}
}



void f2(FILE *f_fp)
{
	string str_readname="";
	ch=fgetc(f_fp);
	int read_index=0;

	while (!feof(f_fp))
	{
		int flag_invalid=0;
		
		if(ch=='\n')
		{
			
			read_index=atoi(str_readname.c_str());
			str_readname.clear();
			if(readslist[read_index]==0)
			{
				fout<<">"<<read_index<<endl;
				int iii = 0;
				while(!feof(f_fp))
				{
					ch=fgetc(f_fp);
					if(ch!=-1)
					{
						fout<<ch;
					}
					
					
					iii++;
					if(read_index == 9999 &&iii == 5240)
					{
						breakpoint();
					}
					if(ch=='\n'||ch==-1)
					{
						break;
					}
				}
			}
			else
			{
				while(!feof(f_fp))
				{
					ch=fgetc(f_fp);
					if(ch=='\n')
					{
						break;
					}
				}
			}
		}
		else if(ch!='>')
		{
			str_readname+=ch;
		}
		
		ch=fgetc(f_fp);
	}
}



int main(int argc,char * argv[])
{
	//argv[1] corrected.fasta
	string filename="0";
	filename=argv[1];
	const char* filename2;
	filename2=filename.c_str();
	f_corrected=fopen(filename2,"r");
	cigar(f_corrected);
	fclose(f_corrected);


	filename=argv[2];
	const char* filename1;
	filename1=filename.c_str();
	f_all=fopen(filename1,"r");

	
	filename=argv[3];
	const char* filename5;
	filename5=filename.c_str();
	fout.open(filename5,ios::trunc);

	f2(f_all);
	fclose(f_all);
	
	return 0;
}