#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct day
{
    int dd;
    int mm;
};

struct movie
{
    string name;
    int house[20][20];
    string time;
    string date;
    int price;
};

struct statByDate
{
    string date;
    int income;
    int num;
};

struct statByMovie
{
    string name;
    int income;
    int num;
};

void showDay(int i, day dlist[], movie mlist[][10])
{
    cout << "*** Movies showing on " <<setfill('0')<<setw(2)<< dlist[i].dd <<"/"<<setfill('0')<<setw(2)<<dlist[i].mm<<" ***"<<endl;
    for(int j=0; j<10 && mlist[i][j].name!="---";j++)
        cout << mlist[i][j].time <<"..."<<mlist[i][j].name<<"...$"<<mlist[i][j].price<<endl;
    cout << endl;
}

void processFile(int dd, int mm, day dlist[],string date[], movie mlist[][10])
{
    string redun,day,name,time;
    stringstream ss;
    int price;
    int i=0,j=0,k=0;
    char dummy=',';
    int daysInMonth[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
    dlist[0].dd=dd;
    dlist[0].mm=mm;
    ss <<setfill('0')<<setw(2)<< dd <<"/"<< setfill('0') << setw(2) << mm;
    ss >> date[0];
    ss.clear();
    for(int i=1;i<3;i++)
    {
        if(dd==daysInMonth[mm-1])
        {
            mm+=1;
            dd = 1;
        }
        else
            dd++;
        dlist[i].dd=dd;
        dlist[i].mm=mm;
        ss <<setfill('0')<<setw(2)<< dd<<"/" << setfill('0') << setw(2)<< mm;
        ss >> date[i];
        ss.clear();
    }
    ifstream fin;
    fin.open("schedule.txt");
    while(fin.good())
    {
        getline(fin,name,',');
        dummy = ',';
        while(dummy!='*')
        {
            getline(fin,day,',');
            getline(fin,time,',');
            fin >> price >> dummy;
            if(day==date[0])
            {
                mlist[0][i].name=name;
                mlist[0][i].time=time;
                mlist[0][i].price=price;
                mlist[0][i].date=date[0];
                i++;
            }
            else if(day==date[1])
            {
                mlist[1][j].name=name;
                mlist[1][j].time=time;
                mlist[1][j].price=price;
                mlist[1][j].date=date[1];
                j++;
            }
            else if(day==date[2]) 
            {               
                mlist[2][k].name=name;
                mlist[2][k].time=time;
                mlist[2][k].price=price;
                mlist[2][k].date=date[2];
                k++;
            }
        }
        getline(fin,redun);
    }
    fin.close();
}

void initiateHouse(int day, string time, int& ticketLeft, movie mlist[3][10], int& index)
{
    string redun,name,date,time1;
    char dummy,current,row;
    int colNum(0),rowNum(0),num(0),col;
    for(int i=0;i<10;i++)
        if(mlist[day][i].time==time)
            index=i;
    ifstream fin,finput;
    fin.open("house.txt");
    finput.open("saleLog.txt");
    fin >> dummy;
    while(dummy!='|')
    {
        colNum ++;
        fin >> dummy;
    }
    getline(fin,redun);
    getline(fin,redun);
    getline(fin,redun);
    int j=0;
    while(fin.good())
    {
        for(int i=0;i<colNum;i++)
        {
            fin >> current;
            if(current=='.')
                mlist[day][index].house[j][i]=1;
            else if(current=='o')
                mlist[day][index].house[j][i]=2;
        }
        getline(fin,redun);
        j++;
        rowNum++;
    }
    while(finput.good())
    {
        getline(finput,date,',');
        finput >> num >> dummy;
        getline(finput,time1,',');
        for(int j=0;j<num && num!=0;j++)
        {
            finput >> row >> col >> dummy;
            if(time1==time && date==mlist[day][index].date)
                mlist[day][index].house[row-'A'][col-1]=3;
        }
        getline(finput,redun);  
    }
    cout << "  .............."<<endl<<"  ... SCREEN ..."<<endl<<"  .............."<<endl;
    cout <<"  00000000011111111112" << endl <<"  12345678901234567890"<<endl<<endl;
    for(int m=0;m<rowNum;m++)
    {
        current = 'A'+m;
        cout << current <<" ";
        for(int n=0;n<colNum;n++)
        {
            if(mlist[day][index].house[m][n]==1)
                cout << " ";
            else if(mlist[day][index].house[m][n]==2)
            {
                cout << "o";
                ticketLeft++;
            }
            else if(mlist[day][index].house[m][n]==3)
                cout << "-";
        }
        cout << endl;
    }
    fin.close();
    finput.close();
}

void buyTickets(int num, movie mlist[3][10],int index,int day)
{
    ofstream fout1,fout2,fout3;
    fout1.open("saleLog.txt",ios::app);
    fout2.open("todayLog.txt",ios::app);
    int col,counter(0);
    char row;
    string redun;
    cout << "please enter the seat(s) you want(e.g. B12):"<<endl;
    getline(cin,redun);
    int income = mlist[day][index].price*num;
    fout1 << mlist[day][index].date << ","<<num<<","<<mlist[day][index].time<<",";
    while(counter<num)
    {
        cin >> row >> col;
        if(mlist[day][index].house[row-'A'][col-1]==2) 
        {
            fout1 << row << col <<",";
            cout << "ticket sold"<<endl;
            mlist[day][index].house[row-'A'][col-1]=3;
            counter++;
        }
        else
            cout << "invalid seat"<<endl;
        getline(cin,redun);
    }
    fout1 << "*" << mlist[day][index].name<<","<<income<<endl;
    fout2 << mlist[day][index].date << ","<<num<<","<<mlist[day][index].name<<","<<income<<","<<endl;
    fout1.close();
    fout2.close();
}

void statAll(statByDate listD[], statByMovie listM[])
{
	ifstream fin0;
    ofstream fout0;
    string redun,date,name;
    int num,income,i(0),m(0),total(0),total2(0);
    char dummy;
    bool found=false;
    fin0.open("saleLog.txt");
    fout0.open("statAll.txt");
 	while(fin0.good())
    {
        getline(fin0,date,',');
        fin0 >> num >> dummy;
        getline(fin0,redun,'*');
        getline(fin0,name,',');
        fin0 >> income;
        getline(fin0,redun);
        found = false;
        for(int j=0;j<i && !found;j++)
        {
            if(date==listD[j].date)
            {
                listD[j].income += income;
                listD[j].num += num;
                found=true;
            }
        }
        if(!found && date.length()>4)
        {
            listD[i].date=date;
            listD[i].income=income;
            listD[i].num=num;
            i++;
        }
        found = false;
        for(int n=0;n<m && !found;n++)
        {
            if(name==listM[n].name)
            {
                listM[n].income += income;
                listM[n].num+=num;
                found=true;
            }
        }
        if(!found&&name!="---")
        {
            listM[m].name=name;
            listM[m].income=income;
            listM[m].num=num;
            m++;
        }
        name = "---";
	}
    cout << "generating statistics up to today..."<<endl;
    fout0<<"by date"<<endl;
    for(int j=0;j<i;j++)
    {
        fout0 << listD[j].date << ","<<listD[j].num << ",$"<<listD[j].income<<endl;
        total += listD[j].income;
    }
    fout0 <<"total $"<< total << endl;
    fout0<<"by movie"<<endl;
    for(int n=0;n<m;n++)
    {
        fout0<<listM[n].name <<","<<listM[n].num<<",$"<<listM[n].income<<endl;
        total2+=listM[n].income;
    }
    fout0 << "total $"<<total2<<endl;
    fout0.close();
}

void statToday(statByDate listD[],statByMovie listM[])
{
    ifstream fin0;
    ofstream fout0;
    string redun,date,name;
    int num,income,i(0),m(0),total(0),total2(0);
    char dummy;
    bool found=false;
    fin0.open("todayLog.txt");
    fout0.open("stat.txt");             
    while(fin0.good())
    {
        getline(fin0,date,',');
        fin0 >> num >> dummy;
        getline(fin0,name,',');
        fin0 >> income;
        getline(fin0,redun);
        found = false;
        for(int j=0;j<i && !found;j++)
            if(date==listD[j].date)
            {
                listD[j].income += income;
                listD[j].num += num;
                found=true;
            }
        if(!found && date.length()>4)
        {
            listD[i].date=date;
            listD[i].income=income;
            listD[i].num=num;
            i++;
        }
        found = false;
        for(int n=0;n<m && !found;n++)
        {
            if(name==listM[n].name)
            {
                listM[n].income += income;
                listM[n].num+=num;
                found=true;
            }
        }
        if(!found && name!="---")
        {
            listM[m].name=name;
            listM[m].income=income;
            listM[m].num=num;
            m++;
        }
        name = "---";
    }
    cout << "generating statistics of today..."<<endl;
    fout0<<"by date"<<endl;
    for(int j=0;j<i;j++)
    {
        fout0 << listD[j].date << ","<<listD[j].num << ",$"<<listD[j].income<<endl;
        total += listD[j].income;
    }
    fout0 <<"total $"<< total << endl;
    fout0<<"by movie"<<endl;
    for(int n=0;n<m;n++)
    {
        fout0<<listM[n].name <<","<<listM[n].num<<",$"<<listM[n].income<<endl;
        total2+=listM[n].income;
    }
    fout0 << "total $"<<total2<<endl;
    fout0.close();
}

int main()
{
    int day,month,dayConsulted,ticketNum,ticketLeft(0),index,pos;
    char dummy;
    string command,left,redun,date[3],timeConsulted;
    ofstream fout0;
    fout0.open("saleLog.txt");
    fout0.close();
    struct day dlist[3];
    struct movie mlist[3][10];
    struct statByMovie listM1[50],listM2[30];
    struct statByDate listD1[366],listD2[3];
    ofstream fout;
    cout << "commands: " << endl;
    cout << "open dd/mm | close | show all | show day x | show day x hh:mm | buy day x hh:mm N | stat | stat all | end"<<endl;
    cin >> command;
    while(command!="end")
    {
        if(command=="open")
        {
            cin >> day >> dummy >> month;
            cout << "day 0 (today) is " <<setfill('0')<<setw(2)<< day << dummy << setfill('0') << setw(2) << month << endl<< endl;
            getline(cin,redun);
            cout << "open dd/mm | close | show all | show day x | show day x hh:mm | buy day x hh:mm N | stat | stat all | end"<<endl;
            cin >> command;
            fout.open("todayLog.txt");
            fout.close();
            while(command!="close")
            {
                for(int i=0;i<3;i++)
                    for(int j=0;j<10;j++)
                        mlist[i][j].name="---";
                processFile(day, month, dlist,date,mlist);
                if(command=="show")
                {
                    cin >> command;
                    if(command=="all")
                    {
                        showDay(0,dlist,mlist);
                        showDay(1,dlist,mlist);
                        showDay(2,dlist,mlist);
                        getline(cin,redun);
                    }
                    else if(command=="day")
                    {
                        cin >> dayConsulted;
                        getline(cin,left);
                        pos=left.find(":");
                        if(pos!=-1)
                        {
                            left=left.substr(pos-2,5);
                            initiateHouse(dayConsulted,left,ticketLeft,mlist,index);
                        }
                        else
                            showDay(dayConsulted,dlist,mlist);
                    }
                }
                else if(command=="buy")
                {
                    cin >> redun;
                    cin >> dayConsulted;
                    cin >> timeConsulted;
                    cin >> ticketNum;
                    initiateHouse(dayConsulted,timeConsulted,ticketLeft,mlist,index);
                    if(ticketNum>0 && ticketNum<=ticketLeft)
                        buyTickets(ticketNum, mlist, index, dayConsulted);
                    else      
                        getline(cin,redun);
                }
                else if(command=="stat")
                {
                    for(int i=0;i<366;i++)
                    {
                        listD1[i].income=0;
                        listD1[i].num=0;
                    }
                    for(int i=0;i<50;i++)
                    {
                        listM1[i].income=0;
                        listM1[i].num=0;
                    }
                    for(int i=0;i<3;i++)
                    {
                        listD2[i].income=0;
                        listD2[i].num=0;
                    }
                    for(int i=0;i<30;i++)
                    {
                        listM2[i].income=0;
                        listM2[i].num=0;
                    }
                    getline(cin,left);
                    pos=left.find("all");
                    if(pos!=-1)
                    	statAll(listD1,listM1);
                    else
                        statToday(listD2,listM2);
                }
                cout << "open dd/mm | close | show all | show day x | show day x hh:mm | buy day x hh:mm N | stat | stat all | end" <<endl;
                cin >> command;
            }
            cout << "ticket counter closed for today"<<endl;
            getline(cin,redun);
            cout << "open dd/mm | close | show all | show day x | show day x hh:mm | buy day x hh:mm N | stat | stat all | end" <<endl;
            cin >> command;
        }
    }
    return 0;
}
