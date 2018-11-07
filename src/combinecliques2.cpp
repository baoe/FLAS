#include"FLAS.h"
#include"combinecliques2.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<algorithm>
#include<string>
#include<sstream>
extern string cliquefile;
extern string edgefile;
extern string printreads;
extern string printcliques;
extern string readfile;
extern string printmatrix;
extern string m4file ;
extern string printm4info;
extern string C0str;
FILE* f_clique;
FILE* f_edge;
FILE* f_read;
FILE* f_m4;
extern int C0;
//clique_matrix clique_matrix1[num_clique][num_clique];
//vector < vector <clique_matrix> > clique_matrix1;
vector <matrix_list> clique_matrix1;
//int cliquelimit = 0;
//clique_list cliquelist[num_read];
//clique_list cliqueslist[num_read];

//read_list readlist[num_read];
//read_list readslist[num_read];
//read_list readslist2[num_read];

//combine_read_node* combine_list[num_read]={NULL};

int num_read, num_clique;

//clique_matrix **clique_matrix1 = NULL;
clique_list *cliquelist = NULL, *cliqueslist = NULL;
read_list *readlist = NULL, *readslist = NULL, *readslist2=NULL;

vector <m4info> m4infolist;
vector <m4info> m4infolist2;

void breakpoint()
{}

//by bao: readslist is converted to accelerate program
void convert(read_list *rl, read_list *readslist)
{
	for(int i = 0; i < num_read; i++)
		if(rl[i].read_name != "")
			readslist[atoi(rl[i].read_name.c_str())] = rl[i];
}

int count_clique=0,count_cliques=0,count_read=0,count_reads=0;

void init_cliquelist()
{
	for(int i=0;i<num_clique;i++)
	{
		cliquelist[i].clique_name="";
		cliquelist[i].clique_ID=0;
		cliquelist[i].next=NULL;
	}
}

void init_cliqueslist()
{
	for(int i=0;i<num_clique;i++)
	{
		cliqueslist[i].clique_name="";
		cliqueslist[i].clique_ID=0;
		cliqueslist[i].next=NULL;
	}
}

void init_readlist()
{
	for(int i=0;i<num_read;i++)
	{
		readlist[i].read_name="";
		readlist[i].flag=0;
		readlist[i].length=0;
		readlist[i].read_ID=0;
		readlist[i].next=NULL;
	}
}

void init_readslist()
{
	for(int i=0;i<num_read;i++)
	{
		readslist[i].read_name="";
		readslist[i].read_ID=0;
		readslist[i].length=0;
		readslist[i].region_flag=0;
		readslist[i].flag=0;
		readslist[i].next=NULL;
		readslist2[i].read_name="";
		readslist2[i].read_ID=0;
		readslist2[i].length=0;
		readslist2[i].region_flag=0;
		readslist2[i].flag=0;
		readslist2[i].next=NULL;
	}
}
/*
void init_matrix()
{

	for(int i=0;i<num_clique;i++)
	{
		for(int j =0;j<num_clique;j++)
		{
			clique_matrix1[i][j].clique_name1=cliqueslist[i].clique_name.c_str();
			clique_matrix1[i][j].clique_ID1=i;
			clique_matrix1[i][j].clique_name2=cliqueslist[j].clique_name.c_str();
			clique_matrix1[i][j].clique_ID2=j;
			clique_matrix1[i][j].num_reads=0;
			clique_matrix1[i][j].next=NULL;
		}
	}
}
*/
void create_clique_matrix()
{
	for(int i=0;i<count_cliques;i++)
	{
		matrix_list newmatrix;
		newmatrix.clique_name=cliqueslist[i].clique_name.c_str();
		newmatrix.clique_ID= i;
		newmatrix.clique_num= 0;
		newmatrix.next = NULL;
		clique_matrix1.push_back(newmatrix);
	}
}

void readcliquefile()
{
	const char * filename1;
	filename1=cliquefile.c_str();
	f_clique=fopen(filename1,"r");
	char ch;
	ch= fgetc(f_clique);
	string readname="";
	string str="";
	read_node * tailr;
	clique_node * tailc;

	while(!feof(f_clique))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\n')
		{
			char a[10];  
			sprintf(a,"%d",count_clique);  
			str=a;  
			cliquelist[count_clique].clique_name="clique"+str;

			count_clique++;
			
			str.clear();
			readname.clear();
		}
		else if(ch==' ')
		{
			

			//clique to read list
			int readID=0;
			//find readname
			int readnum=0;
			int flag_find=0;
			for(readnum=0;readnum<count_read;readnum++)
			{
				if(readlist[readnum].read_name.compare(readname)==0)
				{
					flag_find=1;
					readID=readnum;
					break;
				}
			}
			if(flag_find==0)//not found .new read
			{
				//read to clique list
				read_node * read=new(read_node);
				read->read=readname.c_str();
				read->next=NULL;
				read->read_ID=count_read;
				if(cliquelist[count_clique].next==NULL)
				{
					cliquelist[count_clique].next=read;
					tailr=cliquelist[count_clique].next;
				}
				else
				{
					tailr->next=read;
					tailr=read;
				}

				readlist[count_read].read_name=readname.c_str();
				readlist[count_read].read_ID=count_read;
				clique_node * clique=new(clique_node);
				char a[10];  
				sprintf(a,"%d",count_clique);  
				str=a;  
				clique->clique="clique"+str;
				clique->flag=1;
				clique->next=NULL;

				if(readlist[count_read].next==NULL)
				{
					readlist[count_read].next=clique;
					tailc=readlist[count_read].next;
				}
				else
				{
					tailc->next=clique;
					tailc=clique;
				}
				count_read++;
			}
			else if(flag_find==1)
			{
				//read to clique list
				read_node * read=new(read_node);
				read->read=readname.c_str();
				read->next=NULL;
				read->read_ID=readID;
				if(cliquelist[count_clique].next==NULL)
				{
					cliquelist[count_clique].next=read;
					tailr=cliquelist[count_clique].next;
				}
				else
				{
					tailr->next=read;
					tailr=read;
				}

				clique_node * clique=new(clique_node);
				char a[10];  
				sprintf(a,"%d",count_clique);  
				str=a;  
				clique->clique="clique"+str;
				clique->flag=1;
				clique->next=NULL;
				if(readlist[count_read].next==NULL)
				{
					readlist[count_read].next=clique;
					tailc=readlist[count_read].next;
				}
				else
				{
					tailc->next=clique;
					tailc=clique;
				}

			}
			
			readname.clear();
		}
		else
		{
			readname+=ch;
		}
		ch=fgetc(f_clique);
	}
	fclose(f_clique);
	//cout<<"cliques: "<<count_clique<<endl;
	//cout<<"reads: "<<count_read<<endl;

}

void insertd_cliquelist()
{
	for(int i=0;i<count_clique;i++)
	{
		int count=0;
		read_node * r;
		r=cliquelist[i].next;
		while(r!=NULL)
		{
			count++;
			r=r->next;
		}
		r=cliquelist[i].next;
		while(r!=NULL)
		{
			if(count==2)
			{
				r->d=1;
				r=r->next;
			}
			else
			{
				r->d=count-1;
				r=r->next;
			}
		}
	}
}

void combine_clique()
{
	string str1="",str2="";
	clique_node * tailc;
	int flag_oldread=0;
	int readnum=0;
	int flag_find=0;
	for(int i=0;i<count_clique;i++)
	{
		read_node * readstr2;
		readstr2=cliquelist[i].next;
		str2=readstr2->read.c_str();

		if(str1.compare(str2)==0)//same cliques
		{
			read_node* r;
			r=cliquelist[i].next;
			while(r!=NULL)
			{
				read_node * rs;
				rs=cliqueslist[count_cliques-1].next;
				while(rs!=NULL)
				{
					if(rs->read.compare(r->read)==0)//writen in the cliques
					{
						rs->d+=r->d;
						flag_oldread=1;
						break;
					}
					else//newnode
					{
						rs=rs->next;
					}
				}
				if(flag_oldread==0)//new read
				{
					//insert the "cliquesID" into readslist

					for( readnum=0;readnum<count_reads;readnum++)
					{
						if(readslist[readnum].read_name.compare(r->read)==0)
						{
							flag_find=1;
							break;
						}
					}
					if(flag_find==0)//not found .new read
					{
						readslist[count_reads].read_name=r->read.c_str();
						readslist[count_reads].read_ID=count_reads;
						r->read_ID=count_reads;
						clique_node * cliques=new(clique_node);
						char a[10];  
						string str;
						sprintf(a,"%d",count_cliques-1);  
						str=a;  
						cliques->clique="cliques"+str;
						cliques->clique_ID=count_cliques-1;
						cliques->flag=1;
						cliques->next=NULL;

						if(readslist[count_reads].next==NULL)
						{
							readslist[count_reads].next=cliques;
							tailc=readslist[count_reads].next;
						}
						else
						{
							tailc->next=cliques;
							tailc=cliques;
						}
						count_reads++;
					}
					else if(flag_find==1)
					{
						flag_find=0;
						clique_node * cliques=new(clique_node);
						char a[10];  
						string str;
						sprintf(a,"%d",count_cliques-1);  
						str=a;  
						cliques->clique="cliques"+str;
						cliques->clique_ID=count_cliques-1;
						cliques->flag=1;
						cliques->next=NULL;
						clique_node * tail;
						tail=readslist[readnum].next;
						while(tail->next!=NULL)
						{
							tail=tail->next;
						}
						tail->next=cliques;
					}


					read_node * read=new(read_node);
					read->read=r->read.c_str();
					read->read_ID=r->read_ID;
					read->d=r->d;
					read->next=NULL;
					read_node * tail;
					tail=cliqueslist[count_cliques-1].next;
					while(tail->next!=NULL)
					{
						tail=tail->next;
					}
					tail->next=read;
				}

				flag_oldread=0;
				r=r->next;


			}
		}
		else//new cliques
		{
			char a[10];  
			string str;
			sprintf(a,"%d",count_cliques);  
			str=a;  
			cliqueslist[count_cliques].clique_name="cliques"+str;//store cliques name
			cliqueslist[count_cliques].clique_ID=count_cliques;
			cliqueslist[count_cliques].next=cliquelist[i].next;

			//insert the "cliquesID" into readslist
			read_node * r;
			r=cliquelist[i].next;
			while(r!=NULL)
			{
				for(readnum=0;readnum<count_reads;readnum++)
				{
					if(readslist[readnum].read_name.compare(r->read)==0)
					{
						flag_find=1;
						break;
					}
				}
				if(flag_find==0)//not found .new read
				{
					readslist[count_reads].read_name=r->read.c_str();
					readslist[count_reads].read_ID=count_reads;
					r->read_ID=count_reads;
					clique_node * cliques=new(clique_node);
					char a[10];  
					sprintf(a,"%d",count_cliques);  
					str=a;  
					cliques->clique="cliques"+str;
					cliques->clique_ID=count_cliques;
					cliques->flag=1;
					cliques->next=NULL;

					if(readslist[count_reads].next==NULL)
					{
						readslist[count_reads].next=cliques;
						tailc=readslist[count_reads].next;
					}
					else
					{
						tailc->next=cliques;
						tailc=cliques;
					}
					count_reads++;

				}
				else if(flag_find==1)
				{
					flag_find=0;
					clique_node * cliques=new(clique_node);
					char a[10];  
					sprintf(a,"%d",count_cliques);  
					str=a;  
					cliques->clique="cliques"+str;
					cliques->clique_ID=count_cliques;
					cliques->flag=1;
					cliques->next=NULL;
					clique_node * tail;
					tail=readslist[readnum].next;
					while(tail->next!=NULL)
					{
						tail=tail->next;
					}
					tail->next=cliques;

				}
				r=r->next;
			}
			count_cliques++;

		}
		str1=str2.c_str();
		str2.clear();
	}
}

void insertd_readslist()
{

	for(int i=0;i<count_reads;i++)
	{
		int count=0;
		clique_node * c;
		c=readslist[i].next;
		while(c!=NULL)
		{
			int cliquenum=0;
			if(c->flag==1)
			{
				/*
				for(cliquenum=0;cliquenum<count_cliques;cliquenum++)
				{
				if(c->clique.compare(cliqueslist[cliquenum].clique_name)==0)
				{
				break;
				}
				}
				*/
				read_node * r;
				r=cliqueslist[c->clique_ID].next;
				while(r!=NULL)
				{
					if(r->read.compare(readslist[i].read_name)==0)
					{
						c->d=r->d;
						break;
					}
					r=r->next;
				}



			}
			else if(c->flag==2)
			{
				c->d=1;
			}
			c=c->next;
			count++;
		}
		if(count>=2)
		{
			readslist[i].flag=1;
		}
	}
}

void insert_edge(string read1,string read2)
{


	int readnum1=0;//read1 
	for(readnum1=0;readnum1<count_reads;readnum1++)
	{
		if(readslist[readnum1].read_name.compare(read1)==0)
		{
			break;
		}
	}
	if(readnum1==count_reads)
	{
		readslist[count_reads].read_name=read1.c_str();
		readslist[count_reads].read_ID=count_reads;
		readslist[count_reads].flag=0;
		readslist[count_reads].length=0;
		readslist[count_reads].next=NULL;
		readslist[count_reads].region_flag=0;
		count_reads++;
	}
	int readnum2=0;//read2 
	for(readnum2=0;readnum2<count_reads;readnum2++)
	{
		if(readslist[readnum2].read_name.compare(read2)==0)
		{
			break;
		}
	}
	if(readnum2==count_reads)
	{
		readslist[count_reads].read_name=read2.c_str();
		readslist[count_reads].read_ID=count_reads;
		readslist[count_reads].flag=0;
		readslist[count_reads].length=0;
		readslist[count_reads].next=NULL;
		readslist[count_reads].region_flag=0;
		count_reads++;
	}
	//read1
	clique_node * c2;
	c2=readslist[readnum2].next;
	int flag1=0;//if writen 1
	int flag2=1;//if to write 0
	while(c2!=NULL)
	{
		if(c2->flag==1)
		{
			flag2=0;
			clique_node * c1;
			c1=readslist[readnum1].next;
			while (c1!=NULL)
			{
				if(c1->clique.compare(c2->clique)==0)//c2 already writen in read1
				{
					if(c1->flag==2)
					{
						c1->d++;

					}
					flag1=1;
					break;
				}
				c1=c1->next;
			}
		}


		if(flag1==0&&flag2==0)
		{
			clique_node * cliques=new(clique_node);
			cliques->clique=c2->clique.c_str();
			cliques->clique_ID=c2->clique_ID;
			cliques->d=1;
			cliques->flag=2;
			cliques->next=NULL;
			clique_node * c1;
			c1=readslist[readnum1].next;
			if(c1==NULL)
			{
				readslist[readnum1].next=cliques;
			}
			else
			{
				while (c1->next!=NULL)
				{
					c1=c1->next;
				}
				c1->next=cliques;
			}

			flag1=0;
			flag2=1;
		}
		else 
		{
			flag1=0;
			flag2=1;
		}

		c2=c2->next;
	}
	//read2
	flag1=0;
	flag2=1;
	clique_node * c11;
	c11=readslist[readnum1].next;
	while(c11!=NULL)
	{
		if(c11->flag==1)
		{
			flag2=0;
			clique_node * c22;
			c22=readslist[readnum2].next;
			while (c22!=NULL)
			{
				if(c22->clique.compare(c11->clique)==0)//c2 already writen in read1
				{
					if(c22->flag==2)
					{
						c22->d++;
					}
					flag1=1;
					break;
				}
				c22=c22->next;
			}
		}
		if(flag1==0&&flag2==0)
		{
			clique_node * cliques=new(clique_node);
			cliques->clique=c11->clique.c_str();
			cliques->clique_ID=c11->clique_ID;
			cliques->flag=2;
			cliques->d=1;
			cliques->next=NULL;
			clique_node * c22;
			c22=readslist[readnum2].next;
			if(c22==NULL)
			{
				readslist[readnum2].next=cliques;
			}
			else
			{
				while (c22->next!=NULL)
				{
					c22=c22->next;
				}
				c22->next=cliques;
			}
			flag1=0;
			flag2=1;
		}
		else 
		{
			flag1=0;
			flag2=1;
		}
		c11=c11->next;
	}
}

void readedgefile()
{
	const char * filename2;
	filename2=edgefile.c_str();
	f_edge=fopen(filename2,"r");
	char ch;
	ch= fgetc(f_edge);
	string readname="";
	string str="";
	string read1="",read2="";
	read_node * tailr;
	clique_node * tailc;
	int flag=0;
	while(!feof(f_edge))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\n')
		{
			insert_edge(read1,read2);
			read1.clear();
			read2.clear();
		}
		else if(ch==' ')
		{
			//clique to read list
			if(flag==0)
			{
				flag=1;
				read1=readname.c_str();
				readname.clear();
			}
			else if(flag==1)
			{
				flag=0;
				read2=readname.c_str();
				readname.clear();
			}
		}
		else
		{
			readname+=ch;
		}
		ch=fgetc(f_edge);
	}
	fclose(f_edge);
}

void insertflag_readslist()
{
	int count_flag=0;
	for(int i=0;i<count_reads;i++)
	{
		int count=0;
		clique_node * c;
		c=readslist[i].next;
		while(c!=NULL)
		{

			c=c->next;
			count++;
		}
		if(count>=2)
		{
			readslist[i].flag=count;
			count_flag++;
		}

	}
	//cout<<"count_reads = "<<count_reads<<endl;
	//cout<<"count_flag = "<<count_flag<<endl;
}

void readreadfile()
{
	const char * filename6;
	filename6=readfile.c_str();
	f_read=fopen(filename6,"r");
	char ch;
	ch= fgetc(f_read);
	string readname="";
	string str="";
	int flag=0;
	int count=0;
	while(!feof(f_read))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\n'&&flag==0)
		{
			flag=1;
			for(int i=0;i<count_reads;i++)
			{
				if(readslist[i].read_name.compare(readname)==0)
				{
					flag=0;
					readname.clear();

					while(!feof(f_read))
					{
						ch= fgetc(f_read);
						if(ch=='A'||ch=='T'||ch=='C'||ch=='G'||ch=='a'||ch=='t'||ch=='c'||ch=='g'||ch=='N')
						{
							count++;
						}
						else if(ch=='\n')
						{
							readslist[i].length=count;
							if(count>=L)
							{
								vector <region_node> a(count);
								readslist[i].region=a;
							}
							
							count=0;
							break;
						}
					}
					break;
				}
			}
		}
		if(ch=='\n'&&flag==1)
		{
			flag=0;
			readname.clear();
			while(!feof(f_read))
			{
				ch= fgetc(f_read);
				if(ch=='\n')
				{
					break;
				}
			}
		}
		else if(ch!='>'&&ch!='\n')
		{
			readname+=ch;

		}
		ch= fgetc(f_read);
	}
	fclose(f_read);
}

void readm4file()
{
	const char * filename7;
	filename7=m4file.c_str();
	f_m4=fopen(filename7,"r");
	char ch;
	ch= fgetc(f_m4);
	string readname="";
	string str="";
	int flag=0;
	int count=0;
	m4info tmpm4info;
	while(!feof(f_m4))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\t'&&flag==0)
		{
			tmpm4info.readname1=str.c_str();
			flag=1;
			str.clear();
		}
		else if(ch=='\t'&&flag==1)
		{
			tmpm4info.readname2=str.c_str();
			flag=2;
			str.clear();
		}
		else if(ch=='\t'&&flag==2)
		{
			flag=3;
			str.clear();
		}
		else if(ch=='\t'&&flag==3)
		{
			tmpm4info.identity=atof(str.c_str());
			flag=4;
			str.clear();
		}
		else if(ch=='\t'&&flag==4)
		{
			flag=5;
			str.clear();
		}
		else if(ch=='\t'&&flag==5)
		{
			tmpm4info.start1=atof(str.c_str());
			flag=6;
			str.clear();
		}
		else if(ch=='\t'&&flag==6)
		{
			tmpm4info.end1=atof(str.c_str());
			flag=7;
			str.clear();
		}
		else if(ch=='\t'&&flag==7)
		{
			flag=8;
			str.clear();
		}
		else if(ch=='\t'&&flag==8)
		{
			flag=9;
			str.clear();
		}
		else if(ch=='\t'&&flag==9)
		{
			tmpm4info.start2=atof(str.c_str());
			flag=10;
			str.clear();
		}
		else if(ch=='\t'&&flag==10)
		{
			tmpm4info.end2=atof(str.c_str());
			flag=11;
			str.clear();
		}
		else if(ch=='\t'&&flag==11)
		{
			flag=12;
			str.clear();
		}
		else if(ch=='\t'&&flag==12)
		{
			flag=13;
			str.clear();
		}
		else if(ch=='\n')
		{
			flag=0;
			str.clear();
			if(tmpm4info.readname1.compare(tmpm4info.readname2)!=0)
			{
				m4infolist.push_back(tmpm4info);
			}
		}
		else
		{
			str+=ch;
		}
		ch=fgetc(f_m4);
	}
}

//by bao: readslist is converted to accelerate program
void readsalign(read_list *rl)
{
	read_list *readslist = new read_list[num_read];
	convert(rl, readslist);

	for(int i=0;i<m4infolist.size();i++)
	{
//		for(int j=0;j<count_reads;j++)
//		{
//			if((m4infolist[i].readname1.compare(readslist[j].read_name)==0))
//			{
				m4info_node *info=new (m4info_node);
				info->info.readname1=m4infolist[i].readname1;
				info->info.readname2=m4infolist[i].readname2;
				info->info.start1=m4infolist[i].start1;
				info->info.end1=m4infolist[i].end1;
				info->info.start2=m4infolist[i].start2;
				info->info.end2=m4infolist[i].end2;
				info->info.identity=m4infolist[i].identity;
//				info->next=readslist[j].info;
//				readslist[j].info=info;
//				clique_node * cj;
//				cj=readslist[j].next;
				info->next=rl[readslist[atoi(m4infolist[i].readname1.c_str())].read_ID].info;
				rl[readslist[atoi(m4infolist[i].readname1.c_str())].read_ID].info = info;

				/*
				while(cj!=NULL)
				{
					read_node* r;
					r=cliqueslist[cj->clique_ID].next;
					while(r!=NULL)
					{
						if(r->read.compare(info->info.readname2.c_str())==0&&cj->identity<info->info.identity)
						{
							cj->identity=info->info.identity;
						}
						r=r->next;
					}
					cj=cj->next;
				}
				*/
//			}
//		}
	}
}

void init()
{
	init_cliquelist();
	init_cliqueslist();
	init_readlist();
	//init_matrix();

}

void printreadslist()
{
	const char* filename3;
	filename3=printreads.c_str();
	ofstream fout1;
	int flag=0;
	fout1.open(filename3,ios::trunc);
	clique_node * r;

	for(int i=0;i<count_reads;i++)
	{
		r=readslist[i].next;
		fout1<<readslist[i].read_name.c_str()<<" length="<<readslist[i].length<<" flag="<<readslist[i].flag<<" read_ID="<<readslist[i].read_ID<<" ";
		while(r!=NULL)
		{
			fout1<<"("<<r->clique<<" "<<r->flag<<" "<<r->d<<" "<<r->clique_ID<<")";
			r=r->next;
		}
		fout1<<endl;
	}
}

void printcliqueslist()
{
	const char* filename4;
	filename4=printcliques.c_str();
	ofstream fout1;
	int flag=0;
	fout1.open(filename4,ios::trunc);
	read_node * r;

	for(int i=0;i<count_cliques;i++)
	{
		r=cliqueslist[i].next;
		fout1<<cliqueslist[i].clique_name.c_str()<<" ";
		while(r!=NULL)
		{
			fout1<<"("<<r->read<<" "<<r->d<<" "<<r->read_ID<<") ";
			r=r->next;
		}
		fout1<<endl;
	}
}

void printm4infoslist()
{
	const char* filename8;
	filename8=printm4info.c_str();
	ofstream fout;
	int flag=0;
	fout.open(filename8,ios::trunc);
	read_node * r;

	for(int i=0;i<m4infolist.size();i++)
	{
		fout<<m4infolist[i].readname1<<" "<<m4infolist[i].start1<<" "<<m4infolist[i].end1<<" "<<m4infolist[i].readname2<<" "<<m4infolist[i].start2<<" "<<m4infolist[i].end2<<" "<<m4infolist[i].identity<<endl;

	}
}
void printm4infoslist2()
{
	const char* filename8;
	filename8=printm4info2.c_str();
	ofstream fout;
	int flag=0;
	fout.open(filename8,ios::trunc);
	read_node * r;

	for(int i=0;i<m4infolist.size();i++)
	{
		fout<<m4infolist[i].readname1<<" "<<m4infolist[i].start1<<" "<<m4infolist[i].end1<<" "<<m4infolist[i].readname2<<" "<<m4infolist[i].start2<<" "<<m4infolist[i].end2<<" "<<m4infolist[i].identity<<endl;

	}
}

void insert_clique_matrix()
{
//	cout<<"count_reads = "<<count_reads<<endl;
	for(int i=0;i<count_reads;i++)
	{
		//cout<<"insert_clique_matrix i = "<<i<<"\tin\t"<<count_reads<<",\t cliquenum = "<<readslist[i].flag<<endl;
		if(readslist[i].flag==0||readslist[i].flag>100)
		{
			continue;
		}
		int countread=0;
		clique_node * c1,*c2;
		c1=readslist[i].next;
		while(c1->next!=NULL)
		{
			
			c2=c1->next;
			while(c2!=NULL)
			{
				read_node *r1,*r2;

				read_node *read1=new(read_node);
				read1->read=readslist[i].read_name.c_str();
				read1->length=readslist[i].length;
				read1->read_ID=readslist[i].read_ID;
				read1->region_flag=0;
				read1->d=0;
				read1->next=NULL;
				int id=0;
				int flag_clique = 0 ;
				matrix_node *m;
				m = clique_matrix1[c1->clique_ID].next;
				while(m!= NULL)
				{
					if(m->clique_ID == c2-> clique_ID)
					{
						flag_clique= 1 ;
						break;
					}
					m = m->next_matrix_node;
				}
				if(flag_clique==1)
				{
					read1->next= m->next_read_node;
					m->next_read_node = read1;
					m->read_num++;
					flag_clique = 0;
				}
				else
				{
					matrix_node *newmatrix_node = new(matrix_node);
					
					newmatrix_node->clique_name = c2->clique.c_str();
					newmatrix_node->clique_ID = c2->clique_ID;
					newmatrix_node->read_num = 1;
					newmatrix_node->next_read_node = read1;
					clique_matrix1[c1->clique_ID].clique_num++;
					newmatrix_node->next_matrix_node = clique_matrix1[c1->clique_ID].next;
					clique_matrix1[c1->clique_ID].next = newmatrix_node;

				}
				

				read_node *read2=new(read_node);
				read2->read=readslist[i].read_name.c_str();
				read2->length=readslist[i].length;
				read2->region_flag=0;
				read2->read_ID=readslist[i].read_ID;
				read2->d=0;
				read2->next=NULL;
				m = clique_matrix1[c2->clique_ID].next;
				while(m!= NULL)
				{
					if(m->clique_ID == c1-> clique_ID)
					{
						flag_clique= 1 ;
						break;
					}
					m = m->next_matrix_node;
				}
				
				if(flag_clique==1)
				{
					read2->next= m->next_read_node;
					m->next_read_node = read2;
					m->read_num++;
					flag_clique = 0;
				}
				else
				{
					matrix_node *newmatrix_node = new(matrix_node);
					
					newmatrix_node->clique_name = c1->clique.c_str();
					newmatrix_node->clique_ID = c1->clique_ID;
					newmatrix_node->read_num = 1;
					newmatrix_node->next_read_node = read2;
					clique_matrix1[c2->clique_ID].clique_num++;

					newmatrix_node->next_matrix_node = clique_matrix1[c2->clique_ID].next;
					clique_matrix1[c2->clique_ID].next = newmatrix_node;
				}
				c2=c2->next;
			}
			
			c1=c1->next;
		}
	}
}


int count_files=0;
void combine_cliques()
{
	//cout<<"combine_cliques\n";
	ofstream foutdelete;  

	const char * outdeletefile;

	outdeletefile=delete_edges_file.c_str();
	foutdelete.open(outdeletefile,ios::out);  
	
	for(int i=0;i<count_cliques;i++)
	{
		for(int j =i+1;j<count_cliques;j++)
		{
			
			if(i==j)
			{
				continue;
			}
			//cout<<"combine_cliques\t"<<i<<"\t"<<j<<endl;
			int count=0;
			double min=0;
			if(cliqueslist[i].read_num<cliqueslist[j].read_num)
			{
				min=(double)cliqueslist[i].read_num;
			}
			else
			{
				min=(double)cliqueslist[j].read_num;
			}
			int flag_clique = 0;
			matrix_node *mij;
			mij = clique_matrix1[i].next;
			int clique_matrix1ijnum_reads = 0;
			while(mij!= NULL)
			{
				if(mij->clique_ID == j)
				{
					flag_clique= 1 ;
					clique_matrix1ijnum_reads = mij->read_num;
					break;
				}
				mij = mij->next_matrix_node;
			}
			if(flag_clique==0)
			{
				continue;
			}

			if(clique_matrix1ijnum_reads>=f1*min)
			{
				read_node * r1,*r2,*r3;
				
				r1=cliqueslist[i].next;
				while(r1!=NULL)
				{
					readslist[r1->read_ID].flag_realign=1;
					r1=r1->next;
				}
				r2=cliqueslist[j].next;
				while(r2!=NULL)
				{
					readslist[r2->read_ID].flag_realign=1;
					r2=r2->next;
				}
				r3=mij->next_read_node;
				while(r3!=NULL)
				{
					readslist[r3->read_ID].flag_realign=1;
					r3=r3->next;
				}
				/*
				read_node * r1,*r2,*r3;
				
				r1=cliqueslist[i].next;
				while(r1!=NULL)
				{
					//fout<<r1->read<<endl;
					combine_read_node* com=new(combine_read_node);
					com->readname=r1->read.c_str();
					com->next=NULL;
					combine_read_node* comend;
					comend=combine_list[count_files];
					if(comend==NULL)
					{
						combine_list[count_files]=com;
					}
					else
					{
						while(comend->next!=NULL)
						{
							comend=comend->next;
						}
						comend->next=com;
					}
					
					r1=r1->next;
				}
				
				r2=cliqueslist[j].next;

				while(r2!=NULL)
				{
					//fout<<r2->read<<endl;
					combine_read_node* com=new(combine_read_node);
					com->readname=r2->read.c_str();
					com->next=NULL;
					combine_read_node* comend;
					comend=combine_list[count_files];
					if(comend==NULL)
					{
						combine_list[count_files]=com;
					}
					else
					{
						while(comend->next!=NULL)
						{
							comend=comend->next;
						}
						comend->next=com;
					}
					r2=r2->next;
				}
				r3=clique_matrix1[i][j].next;
				while(r3!=NULL)
				{

					//fout<<r3->read<<endl;
					combine_read_node* com=new(combine_read_node);
					com->readname=r3->read.c_str();
					com->next=NULL;
					combine_read_node* comend;
					comend=combine_list[count_files];
					if(comend==NULL)
					{
						combine_list[count_files]=com;
					}
					else
					{
						while(comend->next!=NULL)
						{
							comend=comend->next;
						}
						comend->next=com;
					}
					r3=r3->next;
				}
				*/
				count_files++;
			}
			else
			{
				read_node * r_count;
				r_count=mij->next_read_node;
				int count_s=0;
				while(r_count!=NULL)
				{
					if(r_count->length>=L)
					{
						count_s++;
					}
					r_count=r_count->next;
				}

				if(count_s>=f2*(double)(clique_matrix1ijnum_reads))
				{
					read_node * r;
					r=mij->next_read_node;
					while (r!=NULL)
					{
						if(r->length>=L)
						{
						//readslist[r->read_ID].region_flag=1;
						//if long read cover 2 regions, region_flag=1
							int count_reads1=0,count_reads2=0;
							m4info_node * m;
							m=readslist[r->read_ID].info;
							while(m!=NULL)
							{
								if(m->info.readname1.compare(readslist[r->read_ID].read_name.c_str())==0)
								{
									read_node *ra1;
									ra1=cliqueslist[i].next;
									while(ra1!=NULL)
									{
										if(m->info.readname2.compare(ra1->read.c_str())==0)
										{
											for(int i1=m->info.start1;i1<m->info.end1;i1++)
											{
												readslist[r->read_ID].region[i1].count_clique1++;
											}
											count_reads1++;
										}
										ra1=ra1->next;
									}
									read_node *ra2;
									ra2=cliqueslist[j].next;
									while(ra2!=NULL)
									{
										if(m->info.readname2.compare(ra2->read.c_str())==0)
										{
											for(int i2=m->info.start1;i2<m->info.end1;i2++)
											{
												readslist[r->read_ID].region[i2].count_clique2++;
											}
											count_reads2++;
										}
										ra2=ra2->next;
									}	

								}
								else if(m->info.readname2.compare(readslist[r->read_ID].read_name.c_str())==0)
								{
									read_node *ra1;
									ra1=cliqueslist[i].next;
									while(ra1!=NULL)
									{
										if(m->info.readname1.compare(ra1->read.c_str())==0)
										{
											for(int i1=m->info.start2;i1<m->info.end2;i1++)
											{
												readslist[r->read_ID].region[i1].count_clique1++;
											}
											count_reads1++;
										}
										ra1=ra1->next;
									}
									read_node *ra2;
									ra2=cliqueslist[j].next;
									while(ra2!=NULL)
									{
										if(m->info.readname1.compare(ra2->read.c_str())==0)
										{
											for(int i2=m->info.start2;i2<m->info.end2;i2++)
											{
												readslist[r->read_ID].region[i2].count_clique2++;
											}
											count_reads2++;
										}
										ra2=ra2->next;
								}	

								}
								m=m->next;
							}
							for(int i3=0;i3<readslist[r->read_ID].region.size();i3++)
							{
								if(readslist[r->read_ID].region[i3].count_clique1<count_reads1*X)
								{
									readslist[r->read_ID].region[i3].count_clique1=0;
								}
								if(readslist[r->read_ID].region[i3].count_clique2<count_reads2*X)
								{
									readslist[r->read_ID].region[i3].count_clique2=0;
								}
							}	

							int count_reads3=0;
							for(int i3=0;i3<readslist[r->read_ID].region.size();i3++)
							{
								if(readslist[r->read_ID].region[i3].count_clique1>0&&readslist[r->read_ID].region[i3].count_clique2>0)
								{
									count_reads3++;
								}
							}
							if(count_reads3<Y)
							{
								readslist[r->read_ID].region_flag=1;
							}
						}
						
						r=r->next;
					}
					r=mij->next_read_node;
					while (r!=NULL)
					{
						if(readslist[r->read_ID].region_flag==1)//cover 2 regions, delete longreads r's alignment
						{
							readslist[r->read_ID].region_flag=0;
							int flag1=0,d1=0;
							int flag2=0,d2=0;
							clique_node * c;
							c=readslist[r->read_ID].next;
							while(c!=NULL)
							{
								if(c->clique_ID==i)
								{
									if(c->flag==1)
									{
										flag1=1;
									}
									else
									{	
										d1=c->d;
									}
								}
								if(c->clique_ID==j)
								{
									if(c->flag==1)
									{
										flag2=1;
									}
									else
									{
										d2=c->d;
										break;
									}
								}
								c=c->next;
							}
							if(flag1==0&&flag2==0)
							{
								if(d1<d2)
								{
									read_node *r1;
									r1=cliqueslist[i].next;
									while(r1!=NULL)
									{
										foutdelete<<r->read.c_str()<<" "<<r1->read.c_str()<<endl;
										r1=r1->next;
									}
								}
								else if(d1>d2)
								{
									read_node *r2;
									r2=cliqueslist[j].next;
									while(r2!=NULL)
									{
										foutdelete<<r->read.c_str()<<" "<<r2->read.c_str()<<endl;
										r2=r2->next;
									}
								}
							}
							else if(flag1==0&&flag2==1)
							{
								flag2=0;
								read_node *r1;
								r1=cliqueslist[i].next;
								while(r1!=NULL)
								{
									foutdelete<<r->read.c_str()<<" "<<r1->read.c_str()<<endl;
									r1=r1->next;
								}
							}
							else if(flag1==1&&flag2==0)
							{
								flag1=0;
								read_node *r2;
								r2=cliqueslist[j].next;
								while(r2!=NULL)
								{
									foutdelete<<r->read.c_str()<<" "<<r2->read.c_str()<<endl;
									r2=r2->next;
								}
							}
							else if(flag1==1&&flag2==1)
							{
								flag1=0;
								flag2=0;
							}
						}
						else//long read covers 1 region. combine these two cliques.
						{
							read_node * r1,*r2,*r3;
				
							r1=cliqueslist[i].next;
							while(r1!=NULL)
							{
								readslist[r1->read_ID].flag_realign=1;
								r1=r1->next;
							}
							r2=cliqueslist[j].next;
							while(r2!=NULL)
							{
								readslist[r2->read_ID].flag_realign=1;
								r2=r2->next;
							}
							r3=mij->next_read_node;
							while(r3!=NULL)
							{
								readslist[r3->read_ID].flag_realign=1;
								r3=r3->next;
							}
						}
						r=r->next;
					}
				}
				else
				{
					read_node * r;
					r=mij->next_read_node;
					while (r!=NULL)
					{
						if(readslist[r->read_ID].region_flag==1)//cover 2 regions, delete longreads r's alignment
						{
							int flag1=0,d1=0;
							int flag2=0,d2=0;
							clique_node * c;
							c=readslist[r->read_ID].next;
							while(c!=NULL)
							{
								if(c->clique_ID==i)
								{
									if(c->flag==1)
									{
										flag1=1;
									}
									else
									{	
										d1=c->d;
									}
								}
								if(c->clique_ID==j)
								{
									if(c->flag==1)
									{
										flag2=1;
									}
									else
									{
										d2=c->d;
										break;
									}
								}
								c=c->next;
							}
							if(flag1==0&&flag2==0)
							{
								if(d1<d2)
								{
									read_node *r1;
									r1=cliqueslist[i].next;
									while(r1!=NULL)
									{
										foutdelete<<r->read.c_str()<<" "<<r1->read.c_str()<<endl;
										r1=r1->next;
									}
								}
								else if(d1>d2)
								{
									read_node *r2;
									r2=cliqueslist[j].next;
									while(r2!=NULL)
									{
										foutdelete<<r->read.c_str()<<" "<<r2->read.c_str()<<endl;
										r2=r2->next;
									}
								}
							}
							else if(flag1==0&&flag2==1)
							{
								flag2=0;
								read_node *r1;
								r1=cliqueslist[i].next;
								while(r1!=NULL)
								{
									foutdelete<<r->read.c_str()<<" "<<r1->read.c_str()<<endl;
									r1=r1->next;
								}
							}
							else if(flag1==1&&flag2==0)
							{
								flag1=0;
								read_node *r2;
								r2=cliqueslist[j].next;
								while(r2!=NULL)
								{
									foutdelete<<r->read.c_str()<<" "<<r2->read.c_str()<<endl;
									r2=r2->next;
								}
							}
							else if(flag1==1&&flag2==1)
							{
								flag1=0;
								flag2=0;
							}
						}
						r=r->next;
					}
				}
			}
		}
	}
}





/////////////////////////////////////////////////////////////////////////
int count_reads2=0;

void create_readslist2()
{
	const char * filename6;
	filename6=rawreadfile.c_str();
	f_read=fopen(filename6,"r");
	char ch;
	ch= fgetc(f_read);
	string readname="";
	string str="";
	int flag=0;
	int count=0;
	int flag_writen=0;
	while(!feof(f_read))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\n'&&flag==0)
		{
			flag=1;
			/*
			for(int i=0;i<count_reads2;i++)
			{
				if(readslist2[i].read_name.compare(readname)==0)
				{
					flag_writen=1;
					readname.clear();
					break;
				}
			}

			if(flag_writen==0)*/
			{
				if(count_reads2%100==0)
				{
					//cout<<"count_reads2 = "<<count_reads2<<"\t"<<readname.c_str()<<endl;
				}
				readslist2[count_reads2].flag=0;
				readslist2[count_reads2].info=NULL;
				readslist2[count_reads2].next=NULL;
				readslist2[count_reads2].read_ID=count_reads2;
				readslist2[count_reads2].read_name=readname.c_str();
				readslist2[count_reads2].region_flag=0;
				count_reads2++;
			}
			flag_writen=0;
		}
		if(ch=='\n'&&flag==1)
		{
			flag=0;
			readname.clear();
			while(!feof(f_read))
			{
				ch= fgetc(f_read);
				count++;
				if(ch=='\n')
				{
					readslist2[count_reads2].length=count;
					count=0;
					break;
				}
			}
		}
		else if(ch!='>'&&ch!='\n')
		{
			readname+=ch;

		}
		ch= fgetc(f_read);

	}
	fclose(f_read);
}

void create_region2()
{
	for(int i=0 ;i<count_reads2;i++)
	{
//		vector<int> a(readslist2[i].length/1000+1);
//by bao: initialization is necessary to avoid memory leak
		vector<int> a(readslist2[i].length/1000+1, 0);
		readslist2[i].region2=a;
	}
}

void readm4file2()
{
	const char * filename7;
	filename7=alignment.c_str();
	f_m4=fopen(filename7,"r");
	char ch;
	ch= fgetc(f_m4);
	string readname="";
	string str="";
	int flag=0;
	int count=0;
	m4info tmpm4info;
	while(!feof(f_m4))
	{
		if(ch==-1)
		{
			break;
		}
		if(ch=='\t'&&flag==0)
		{
			tmpm4info.readname1=str.c_str();
			flag=1;
			str.clear();
		}
		else if(ch=='\t'&&flag==1)
		{
			tmpm4info.readname2=str.c_str();
			flag=2;
			str.clear();
		}
		else if(ch=='\t'&&flag==2)
		{
			flag=3;
			str.clear();
		}
		else if(ch=='\t'&&flag==3)
		{
			tmpm4info.identity=atof(str.c_str());
			flag=4;
			str.clear();
		}
		else if(ch=='\t'&&flag==4)
		{
			flag=5;
			str.clear();
		}
		else if(ch=='\t'&&flag==5)
		{
			tmpm4info.start1=atof(str.c_str());
			flag=6;
			str.clear();
		}
		else if(ch=='\t'&&flag==6)
		{
			tmpm4info.end1=atof(str.c_str());
			flag=7;
			str.clear();
		}
		else if(ch=='\t'&&flag==7)
		{
			flag=8;
			str.clear();
		}
		else if(ch=='\t'&&flag==8)
		{
			flag=9;
			str.clear();
		}
		else if(ch=='\t'&&flag==9)
		{
			tmpm4info.start2=atof(str.c_str());
			flag=10;
			str.clear();
		}
		else if(ch=='\t'&&flag==10)
		{
			tmpm4info.end2=atof(str.c_str());
			flag=11;
			str.clear();
		}
		else if(ch=='\t'&&flag==11)
		{
			flag=12;
			str.clear();
		}
		else if(ch=='\n')
		{
			flag=0;
			str.clear();
			if(tmpm4info.readname1.compare(tmpm4info.readname2)!=0)
			{
				m4infolist2.push_back(tmpm4info);
			}
			if(m4infolist2.size()%1000==0)
			{
				//cout<<"m4infolist2.size() = "<<m4infolist2.size()<<endl;
			}
		}
		else
		{
			str+=ch;
		}
		ch=fgetc(f_m4);
	}
}

//by bao: readslist is converted to accelerate program
void ali_raw_co(read_list *rl)
{
	read_list *readslist = new read_list[num_read];
	convert(rl, readslist);

	int rawreadID=0;
	int coreadID=0;
	int flag_found=0;
	for(int i=0;i<m4infolist2.size();i++)
	{
/*		
		flag_found=0;
		for(int j=0;j<count_reads;j++)
		{
			//if(atoi(m4infolist2[i].readname1.c_str())==readslist[j].read_ID)
			if(m4infolist2[i].readname1.compare(readslist[j].read_name)==0)
			{
				//coreadID=readslist[j].read_ID;
				coreadID=j;
				flag_found=1;
				break;
			}
		}
		if(flag_found==0)
		{
			continue;
		}
		if(i%100==0)
		{
			//cout<<"ali_raw_co() : m4info = "<<m4infolist2.size()<<"   i = "<<i<<endl;
		}
		for(int j=0;j<count_reads2;j++)
		{
			if(atoi(m4infolist2[i].readname2.c_str())==readslist2[j].read_ID)
			//if(m4infolist2[i].readname2.compare(readslist2[j].read_name)==0)
			{
				rawreadID=j;
				break;
			}
		}
		//cout<<m4infolist2.size()<<"\t"<<i<<"\t"<<rawreadID<<"\t"<<readslist[coreadID].read_name<<endl;
*/
		coreadID = atoi(m4infolist2[i].readname1.c_str());
		rawreadID = atoi(m4infolist2[i].readname2.c_str());

		clique_node *c1;
		c1=readslist[coreadID].next;
		while(c1!=NULL)
		{
			clique_node* c2;
			c2=readslist2[rawreadID].next;
			if(c2==NULL)
			{
				clique_node* c=new(clique_node);
				c->clique=c1->clique.c_str();
				c->clique_ID=c1->clique_ID;
				c->d=c1->d;
				c->flag=c1->flag;
				c->identity=c1->identity;
				c->next=NULL;
				readslist2[rawreadID].next=c;
			}
			else
			{
				int flag_writen=0;
				while(c2!=NULL)
				{
					if(c1->clique_ID==c2->clique_ID)
					{
						flag_writen=1;
						if(c2->identity<m4infolist2[i].identity)
						{
							c2->identity=m4infolist2[i].identity;
						}
						break;
					}
					c2=c2->next;
				}
				if(flag_writen==0)
				{
					clique_node* c=new(clique_node);
					c->clique=c1->clique.c_str();
					c->clique_ID=c1->clique_ID;
					c->d=c1->d;
					c->flag=1;
					c->identity=c1->identity;
					c->next=readslist2[rawreadID].next;
					readslist2[rawreadID].next=c;
				}
			}
			c1=c1->next;
		}
		m4info_node *info=new (m4info_node);
		info->info.readname1=m4infolist2[i].readname1;
		info->info.readname2=m4infolist2[i].readname2;
		info->info.start1=m4infolist2[i].start1;
		info->info.end1=m4infolist2[i].end1;
		info->info.start2=m4infolist2[i].start2;
		info->info.end2=m4infolist2[i].end2;
		info->info.identity=m4infolist2[i].identity;
		info->next=readslist2[rawreadID].info;
		readslist2[rawreadID].info=info;
		
	}
}

void insertflag_readslist2()
{
	int count_flag=0;
	for(int i=0;i<count_reads2;i++)
	{
		int count=0;
		clique_node * c;
		c=readslist2[i].next;
		while(c!=NULL)
		{

			c=c->next;
			count++;
		}
		if(count>=2)
		{
			readslist2[i].flag++;
			count_flag++;
		}
	}
	//cout<<count_flag<<endl;
}

void readsalign2()
{
	/*
	for(int i=0;i<m4infolist2.size();i++)
	{
		if(i%100==0)
		{
			//cout<<m4infolist2.size()<<" "<<i<<endl;
		}
		//for(int j=once_num;j<count_reads2;j++)
		{
			int j=0;
			j=atoi(m4infolist2[i].readname2.c_str());
			if((readslist2[i].flag!=0))
			//if(m4infolist2[i].readname2.compare(readslist2[j].read_name)==0)
			{
				breakpoint();
				m4info_node *info=new (m4info_node);
				info->info.readname1=m4infolist2[i].readname1.c_str();
				info->info.readname2=m4infolist2[i].readname2.c_str();
				info->info.start1=m4infolist2[i].start1;
				info->info.end1=m4infolist2[i].end1;
				info->info.start2=m4infolist2[i].start2;
				info->info.end2=m4infolist2[i].end2;
				info->info.identity=m4infolist2[i].identity;
				info->next=readslist2[j].info;
				readslist2[j].info=info;
				
			}
		}
	}
	*/
	for(int i=0;i<m4infolist.size();i++)
	{
		if(i%100==0)
		{
			//cout<<"readsalign2()\t"<<m4infolist.size()<<"\t"<<i<<endl;
		}
		//for(int j=0;j<count_reads;j++)
		{
			int j=0;
			
			j=atoi(m4infolist2[i].readname2.c_str());
			if(j>once_num)
			//if(m4infolist[i].readname2.compare(readslist[j].read_name)==0)
			{
				m4info_node *info=new (m4info_node);
				info->info.readname1=m4infolist[i].readname1;
				info->info.readname2=m4infolist[i].readname2;
				info->info.start1=m4infolist[i].start1;
				info->info.end1=m4infolist[i].end1;
				info->info.start2=m4infolist[i].start2;
				info->info.end2=m4infolist[i].end2;
				info->info.identity=m4infolist[i].identity;
				info->next=readslist2[j].info;
				readslist2[j].info=info;
				clique_node * cj;
				cj=readslist2[j].next;
				while(cj!=NULL)
				{
					read_node* r;
					r=cliqueslist[cj->clique_ID].next;
					while(r!=NULL)
					{
						if(r->read.compare(info->info.readname1.c_str())==0&&cj->identity<info->info.identity)
						{
							cj->identity=info->info.identity;
						}
						r=r->next;
					}
					cj=cj->next;
				}
			}
		}
	}
}

void insert_readnum()
{
	for(int i=0;i<count_cliques;i++)
	{
		int count=0;
		read_node * r;
		r=cliqueslist[i].next;
		while(r!=NULL)
		{
			count++;
			r=r->next;
		}
		cliqueslist[i].read_num=count;
	}
}

double Pclique(int clique_ID_A,int clique_ID_B)
{
	double P_clique=0;
	
	int C_clique_A=0;
	int C_clique_B=0;
	C_clique_A=cliqueslist[clique_ID_A].read_num;
	C_clique_B=cliqueslist[clique_ID_B].read_num;

	if(C0>C_clique_A&&C0>C_clique_B)
	{
		P_clique=((double)(C0-C_clique_A))/((double)(C0-C_clique_A+C0-C_clique_B));
	}
	else if(C0<=C_clique_A&&C0>C_clique_B)
	{
		P_clique=0;
	}
	else if(C0>C_clique_A&&C0<=C_clique_B)
	{
		P_clique=1;
	}
	else if(C0<=C_clique_A&&C0<=C_clique_B)
	{
		P_clique=0.5;
	}
	return P_clique;
}

void printreadslist2()
{
	const char* filename3;
	filename3=printreads2.c_str();
	ofstream fout1;
	int flag=0;
	fout1.open(filename3,ios::trunc);
	clique_node * r;

	for(int i=0;i<count_reads2;i++)
	{
		r=readslist2[i].next;
		if(readslist2[i].flag==1)
		{
			fout1<<readslist2[i].read_ID<<" "<<readslist2[i].read_name.c_str()<<" flag="<<readslist2[i].flag<<" read_ID=";
			while(r!=NULL)
			{
				fout1<<"("<<r->clique<<" "<<r->flag<<" "<<r->d<<" "<<r->clique_ID<<")";
				r=r->next;
			}
			fout1<<endl;
		}
	}
}
void HASAL2()
{
	ofstream foutdelete;  

	const char * outdeletefile;

	vector<deletepair_node> deletepairlist;
	int count_delete=0;
	outdeletefile=delete_edges_file2.c_str();
	foutdelete.open(outdeletefile,ios::out);  
	int Csum=0;
	for(int i=0;i<count_cliques;i++)
	{
		Csum+=cliqueslist[i].read_num;
	}
//	cout<<"cout_cliques "<<count_cliques<<endl;
	C0=Csum/count_cliques;
	C0=atoi(C0str.c_str());
	for(int i=once_num;i<count_reads2;i++)
	{
		//cout<<"i =  "<<i<<"\tin\t"<<count_reads2<<endl;
		if(readslist2[i].flag!=0&&readslist2[i].flag<50)
		{
			//cout<<"count_reads2 = "<<count_reads2<<"\tread "<<readslist2[i].read_ID<<endl;
			double PA,PB;
			clique_node *s,*e;
			s=readslist2[i].next;
			e=s->next;
			while(e!=NULL)
			{
				if((s->identity-e->identity)>=d1)
				{
					PA=1;
					PB=0;
				}
				else if((e->identity-s->identity)>=d1)
				{
					PA=0;
					PB=1;
				}
				else
				{
					double m1=0;
					double m2=0;
					if(C0>s->clique_ID)
					{
						m1=(double)(C0-s->clique_ID);
					}
					if(C0>e->clique_ID)
					{
						m2=(double)(C0-e->clique_ID);
					}
					if((m1+m2)!=0)
					{
						if(m1>=m2)
						{
							if((m1-m2)/(m1+m2)>=0.2)
							{
								PA=1;
								PB=0;
							}
						}
						else
						{
							if((m1-m2)/(m1+m2)>=0.2)
							{
								PA=0;
								PB=1;
							}
						}
					}
				}		
				//PA=s->identity*Pclique(s->clique_ID,e->clique_ID);
				//PB=e->identity*Pclique(e->clique_ID,s->clique_ID);
				if(PA>=PB)
				{
					e=e->next;
				}
				else
				{
					s=e;
					e=e->next;
				}
			}
			
			m4info_node * m;
			m=readslist2[i].info;
			while(m!=NULL)
			{
				if(cliqueslist[s->clique_ID].read_num>50)
				{
					m=m->next;
					continue;
				}
				read_node * r;
				r=cliqueslist[s->clique_ID].next;

				while(r!=NULL)
				{
					if(m->info.readname1.compare(r->read.c_str())==0&&readslist2[r->read_ID].flag>=100)
					{

					}
					else
					{
						int flag=0;

						int st=0;
						int en=0;
						st=m->info.start2/1000+1;
						en=m->info.end2/1000+1;
						for(int j=st;j<en;j++)
						{
							if(readslist2[i].region2[j]<20)
							{
								readslist2[i].region2[j]++;
								flag=1;
							}
						}
						if(flag==0)
						{
							int flag_writen=0;
							deletepair_node d;
							d.readname1=m->info.readname1.c_str();
							d.readname2=m->info.readname2.c_str();
							/*
							for(int k=0;k<count_delete;k++)
							{
								if(deletepairlist[k].readname1.compare(m->info.readname1.c_str())==0&&deletepairlist[k].readname2.compare(m->info.readname2.c_str())==0)
								{
									flag_writen=1;
								}
							}
							*/
							//if(flag_writen!=1)
							{
								//count_delete++;
								//deletepairlist.push_back(d);
								foutdelete<<m->info.readname1.c_str()<<" "<<m->info.readname2.c_str()<<endl;
							}
						}
					}
					r=r->next;
				}
				m=m->next;
			}

			
		}
	}
	//cout<<"process HASAL2 end"<<endl;
}


void write_combinedreads()
{
	std::ofstream fout(realireads.c_str(),std::ios::out);
	std::ifstream fin(readfile.c_str(), std::ios::in);   
	string readname="";
	//string sreadname="";
	string read="";
	int flag=0;
	int count=0;
	int count_writen=0;
	int flag_find=0;
	int flag_writen=0;
	int count_realireads=0;
	int readID=0;
	while(getline(fin,readname))
	{
		if(readname[0]=='>')
		{
			readname.erase(0,1);
		}
		getline(fin,read);
		for(int i=0;i<count_reads;i++)
		{
			if(readslist[i].read_name.compare(readname.c_str())==0)
			{
				flag_find=1;
				readID=i;
				break;
			}
		}
		if(flag_find==1)
		{
			flag_find=0;
			if(readslist[readID].flag_realign==1)
			{
//				cout<<">"<<readname.c_str()<<endl;
				fout<<">"<<readname.c_str()<<endl;
				readname.clear();
				fout<<read.c_str()<<endl;
			}
		}
	}
	fin.clear();  
    fin.close();  
    fout.clear();  
    fout.close();  
}


void print_clique_matrix()
{
	const char* filename5;
	filename5=printmatrix.c_str();
	ofstream fout1;
	int flag=0;
	fout1.open(filename5,ios::trunc);
	matrix_node * m;
	for(int i=0;i<count_cliques;i++)
	{
		m = clique_matrix1[i].next;
		fout1<<i<<" "<<clique_matrix1[i].clique_name<<" & ";

		while(m!=NULL)
		{
			fout1<<m->clique_name<<" read num = "<<" "<<m->read_num<<" ";
			read_node *r;
			r = m->next_read_node;
			while (r!=NULL)
			{
				fout1<<"("<<r->read<<" "<<" "<<r->read_ID<<") ";
				r = r->next;
			}
			m=m->next_matrix_node;
		}
	}
	fout1.close();
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

void init_global_variables(int flag)
{
	if(flag==1)
	{
		num_clique = countFileLineNum(cliquefile);	
		num_read = countFileLineNum(readfile) / 2;
//		cout<<"num_clique "<<num_clique<<endl;
//		cout<<"num_read "<<num_read<<endl;
	}
	else if(flag==2)
	{
		num_clique = countFileLineNum(cliquefile);	
		num_read = countFileLineNum(rawreadfile) / 2;
		if(num_clique < 100)
			num_clique = num_read;
//		cout<<"num_clique "<<num_clique<<endl;
//		cout<<"num_read "<<num_read<<endl;
	}
	cliquelist = new clique_list[num_clique];
	cliqueslist = new clique_list[num_clique];
	
	readlist = new read_list[num_read];
	readslist = new read_list[num_read];
	readslist2 = new read_list[num_read];

	//combine_list = new combine_read_node[num_read];

	//clique_matrix1 = new clique_matrix*[num_clique];
	//for(int i = 0; i < num_clique; i++)
	//	clique_matrix1[i] = new clique_matrix[num_clique];
}

void free_memory()
{
	delete[] cliquelist;
	delete[] cliqueslist;
	delete[] readlist;
	delete[] readslist;
	delete[] readslist2;
	//delete[] combine_list;

	//for(int i = 0; i < num_clique; i++)
	//	delete[] clique_matrix1[i];
	//delete[] clique_matrix1;
}

void combine()
{
	if(workflow==1)
	{
//		cout<<"workflow=1"<<endl;

		init_global_variables(1);
//		cout<<"finish init global variables"<<endl;

		init();
//		cout<<"finish init"<<endl;
		readm4file();
//		cout<<"finish readm4file"<<endl;
		readcliquefile();
//		cout<<"finish readcliquefile"<<endl;
		insertd_cliquelist();
//		cout<<"finish insertd_cliquelist"<<endl;
		combine_clique();
//		cout<<"finish combine_clique"<<endl;
		insertd_readslist();
//		cout<<"finish insertd_readslist"<<endl;
		readedgefile();
//		cout<<"finish readedgefile"<<endl;
		readreadfile();
//		cout<<"finish readreadfile"<<endl;
		insertflag_readslist();
//		cout<<"finish insertflag_readslist"<<endl;
		printcliqueslist();
//		cout<<"finish printcliqueslist"<<endl;
		printreadslist();
//		cout<<"finish printreadslist"<<endl;
//		cout<<"count_cliques = "<<count_cliques<<endl;
		readsalign(readslist);
//		cout<<"finish readsalign"<<endl;
		insert_readnum();
//		cout<<"finish insert_readnum"<<endl;
		create_clique_matrix();
//		cout<<"finish create_clique_matrix"<<endl;
		insert_clique_matrix();
//		cout<<"finish insert_clique_matrix"<<endl;
		print_clique_matrix();
//		cout<<"finish print_clique_matrix"<<endl;
		
		combine_cliques();
//		cout<<"finish combine_cliques"<<endl;
		write_combinedreads();
		
		free_memory();
	
	}
	if(workflow==2)
	{
//		cout<<"workflow=2"<<endl;

		init_global_variables(2);
//		cout<<"init global variables"<<endl;

		init();
//		cout<<"init"<<endl;

		readcliquefile();
//		cout<<"readcliquefile"<<endl;

		insertd_cliquelist();
//		cout<<"insertd_cliquelist"<<endl;

		combine_clique();
//		cout<<"combine_clique"<<endl;

		insertd_readslist();
//		cout<<"insertd_readslist"<<endl;

		readedgefile();
//		cout<<"readedgefile"<<endl;

		readreadfile();
//		cout<<"readreadfile"<<endl;

		insertflag_readslist();
//		cout<<"insertflag_readslist"<<endl;

		//printcliqueslist();
		//printreadslist();
		create_clique_matrix();
//		cout<<"create_clique_matrix"<<endl;

		insert_clique_matrix();
//		cout<<"insert_clique_matrix"<<endl;

		//print_clique_matrix();
		readm4file();
//		cout<<"readm4file"<<endl;

//by bao: readslist is converted to accelerate program
		readsalign(readslist);
//		cout<<"readsalign"<<endl;

		combine_cliques();
//		cout<<"combine_cliques"<<endl;

		//write_combinedreads();
		//printm4infoslist();

		insert_readnum();	
//		cout<<"insert_readnum"<<endl;


	/////////////////////////////////////
		create_readslist2();
//		cout<<"create_readslist2"<<endl;

		create_region2();
//		cout<<"create_region2"<<endl;

		readm4file2();
//		cout<<"readm4file2"<<endl;

//by bao: readslist is converted to accelerate program
		ali_raw_co(readslist);
//		cout<<"ali_raw_co"<<endl;

		readsalign2();
//		cout<<"readsalign2"<<endl;

		insertflag_readslist2();
//		cout<<"insertflag_readslist2"<<endl;

		//printreadslist2();
		HASAL2();
//		cout<<"HASAL2"<<endl;

		free_memory();

	}
	
}
