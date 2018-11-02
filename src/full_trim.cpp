#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <string.h>
#include<vector>
#include<fstream>

using namespace std;
char ch;
 
FILE *f_corrected;
FILE *f_allreads;
FILE *f_full;
FILE *f_trimmed;

typedef struct corrected_read_info
{
	string sequence;
	int ori_read_ID;
	int ori_start_pos;
	int ori_end_pos;
	int length;
}corrected_read_info;

vector<string> allreads;
vector<corrected_read_info> corrected;

ofstream fout_full,fout_trim;
void breakpoint()
{

}

void readcorrected(FILE * f_corrected)
{
	corrected_read_info read;
	char ch;
	string namestring;
	while (!feof(f_corrected))
	{
		ch=fgetc(f_corrected);
		if(ch=='>')
		{
			while(!feof(f_corrected))
			{
				ch=fgetc(f_corrected);
				if(ch=='_')
				{
					read.ori_read_ID=atoi(namestring.c_str());
					namestring.clear();
					break;
				}
				else
				{
					namestring+=ch;
				}
			}
			while(!feof(f_corrected))
			{
				ch=fgetc(f_corrected);
				if(ch=='_')
				{
					read.ori_start_pos=atoi(namestring.c_str());
					namestring.clear();
					break;
				}
				else
				{
					namestring+=ch;
				}
			}
			while(!feof(f_corrected))
			{
				ch=fgetc(f_corrected);
				if(ch=='_')
				{
					read.ori_end_pos=atoi(namestring.c_str());
					namestring.clear();
					break;
				}
				else
				{
					namestring+=ch;
				}
			}
			while(!feof(f_corrected))
			{
				ch=fgetc(f_corrected);
				if(ch=='\n')
				{
					read.length=atoi(namestring.c_str());
					namestring.clear();
					break;
				}
				else
				{
					namestring+=ch;
				}
			}
		}
		while(!feof(f_corrected))
		{
			ch=fgetc(f_corrected);
			if(ch=='\n')
			{
				read.sequence=namestring.c_str();
				namestring.clear();
				break;
			}
			else
			{
				namestring+=ch;
			}
		}
		corrected.push_back(read);
	}
	fclose(f_corrected);
}
void readallreads(FILE * f_allreads)
{
	char ch;
	string namestring;
	while (!feof(f_allreads))
	{
		ch=fgetc(f_allreads);
		if(ch=='>')
		{
			while(!feof(f_allreads))
			{
				ch=fgetc(f_allreads);
				if(ch=='\n')
				{
					break;
				}
			}
		}
		while(!feof(f_allreads))
		{
			ch=fgetc(f_allreads);
			if(ch=='\n')
			{
				allreads.push_back(namestring.c_str());
				namestring.clear();
				break;
			}
			else
			{
				if(ch=='A')
				{
					namestring+="a";
				}
				if(ch=='T')
				{
					namestring+="t";
				}
				if(ch=='C')
				{
					namestring+="c";
				}
				if(ch=='G')
				{
					namestring+="g";
				}
			}
		}
	}
	fclose(f_allreads);
}

void replaceread()
{
	for(int i=0 ;i<corrected.size();i++)
	{
		int readid;
		readid=corrected[i].ori_read_ID;
		if(readid<allreads.size())
		{
			const char* str;
			str=corrected[i].sequence.c_str();
//			if(corrected[i].ori_start_pos > allreads[readid].size() || corrected[i].ori_end_pos > allreads[readid].size())
//			{
//				cout << corrected[i].ori_read_ID << ", " << corrected[i].ori_start_pos << ", " << corrected[i].ori_end_pos << ", " << corrected[i].length << ": " << allreads[readid].size();
//			}
			if(corrected[i].ori_start_pos<allreads[readid].size()&&corrected[i].ori_start_pos<allreads[readid].size())
			{
				allreads[readid]=allreads[readid].replace(allreads[readid].begin()+corrected[i].ori_start_pos,allreads[readid].begin()+corrected[i].ori_end_pos,str);
			}
		}
	}
}

void writefullreads()
{
	for(int i=0;i<allreads.size();i++)
	{
		fout_full<<">"<<i<<endl;
		fout_full<<allreads[i].c_str()<<endl;
	}
}

void writetrimreads()
{
	for(int i=0;i<allreads.size();i++)
	{
		int headcut=0;
		int tailcut=0;
		for(int j=0;j<allreads[i].size();j++)
		{
			if(allreads[i][j]>=65&&allreads[i][j]<=90)
			{
				headcut=j;
				break;
			}
		}
		for(int j=allreads[i].size();j>0;j--)
		{
			if(allreads[i][j]>=65&&allreads[i][j]<=90)
			{
				tailcut=j;
				break;
			}
		}
		allreads[i].erase(allreads[i].begin(),allreads[i].begin()+headcut);
//		allreads[i].erase(allreads[i].begin()+tailcut,allreads[i].end());
//by bao: the offset should be tailcut-headcut+1
		allreads[i].erase(allreads[i].begin()+tailcut-headcut,allreads[i].end());
		if(allreads[i].size()!=0)
		{
			fout_trim<<">"<<i<<endl;
			fout_trim<<allreads[i].c_str()<<endl;
		}
	}
}


int main(int argc,char * argv[])
{
	//argv[1] corrected.fasta
	string corrected="0";
	corrected=argv[1];
	const char* filename1;
	filename1=corrected.c_str();
	f_corrected=fopen(filename1,"r");
	readcorrected(f_corrected);
	//cout<<"	readcorrected(f_corrected);"<<endl;

	//argv[2] allreads.fasta
	string allreads="0";
	allreads=argv[2];
	const char* filename2;
	filename2=allreads.c_str();
	f_allreads=fopen(filename2,"r");
	readallreads(f_allreads);
	//cout<<"	readallreads(f_allreads);"<<endl;

	//argv[3] fullreads.fasta
	string fullreads="0";
	fullreads=argv[3];
	const char* filename3;
	filename3=fullreads.c_str();
	fout_full.open(filename3,ios::trunc);
	
	
	//argv[4] trimmedreads.fasta
	string trimmedreads="0";
	trimmedreads=argv[4];
	const char* filename4;
	filename4=trimmedreads.c_str();
	fout_trim.open(filename4,ios::trunc);
	
	replaceread();
	//cout<<"replaceread()"<<endl;

	writefullreads();
	//cout<<"writefullreads()"<<endl;

	writetrimreads();
	//cout<<"writetrimreads()"<<endl;
//./output/split_reads.fasta ./temp/step1/allreads.fasta ./output/full_reads.fasta ./output/trimmed_reads.fasta

	return 0;
}
