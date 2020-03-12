#include "inputvar.h"

INPUTVAR::INPUTVAR(QString input, int max_bit)
{
    //无关项
    if(input[0]=='*')
    {
        this->_isirr=true;
        if(input[1]=='m')
        {
            this->_no=input.mid(2).toInt();
            this->_str=this->noTOstr(max_bit);
        }
        else
        {
            this->_str=input.mid(1);
            this->_no=this->strTOno();
        }
    }
    else
    {
        this->_isirr=false;
        if(input[0]=='m')
        {
            this->_no=input.mid(1).toInt();
            this->_str=this->noTOstr(max_bit);
        }
        else
        {
            this->_str=input;
            this->_no=this->strTOno();
        }
    }
}

void INPUTVAR::reset0()
{
    this->_no=0;
    this->_str="";
    this->_isirr=false;
}

QString INPUTVAR::noTOstr(int max_bit)
{
    QString tmps;
    if(this->_no==0)
    {
        tmps="0";
    }
    else
    {
        int num=this->_no;
        while(num)
        {
            tmps.insert(0,QString::number(num%2));
            num/=2;
        }
    }
    //补0对齐
    for(int i=max_bit-tmps.length();i>0;--i)
    {
        tmps="0"+tmps;
    }
    return tmps;
}

int INPUTVAR::strTOno()
{
    int num=0;
    for(int i=0;i<this->_str.length();++i)
    {
        num*=2;
        num+=(this->_str[i]=='0' ?0 :1);
    }
    return num;
}
