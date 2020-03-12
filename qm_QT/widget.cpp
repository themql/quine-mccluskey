#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->lineEdit_maxbit,SIGNAL(returnPressed()),this,SLOT(input_maxbit()));
    connect(ui->lineEdit_inputvar,SIGNAL(returnPressed()),this,SLOT(input_inputvar()));
    connect(ui->pushButton_creat,SIGNAL(pressed()),this,SLOT(button_creat()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::displayInputList()
{
    QString info;
    for(int i=0;i<this->inputvar.size();++i)
    {
        if(inputvar[i]._str!="")
        {
            QString numS;
            info=info+(inputvar[i]._isirr ?"*m" :" m")+numS.setNum(inputvar[i]._no)+
                    "\t"+inputvar[i]._str+"\n";
        }
    }
    ui->label_inputvarDisplay->setText(info);
}

void Widget::input_maxbit()
{
    int space=1;
    this->MAX_BIT=ui->lineEdit_maxbit->text().toInt();
    for(int i=0;i<this->MAX_BIT;++i)
    {
        space*=2;
    }
    inputvar.resize(space);

    ui->lineEdit_inputvar->clear();
    ui->label_inputvarDisplay->clear();
    ui->label_resultDisplay->clear();
}

void Widget::input_inputvar()
{
    if(this->MAX_BIT==0)
    {
        QMessageBox::information(this,tr("ERROR"),tr("请先输入变量位数！"));
    }
    else
    {
        INPUTVAR tmpVar(ui->lineEdit_inputvar->text(),this->MAX_BIT);
        //添加
        if(this->inputvar[tmpVar._no]._str=="")
        {
            this->inputvar[tmpVar._no]=tmpVar;
        }
        //删除
        else
        {
            this->inputvar[tmpVar._no].reset0();
        }
        this->displayInputList();
        ui->lineEdit_inputvar->clear();
        ui->label_resultDisplay->clear();
    }
}

void Widget::button_creat()
{
    QVector<QString> result;
    result=this->creat();
    if(result.empty())
    {
        return;
    }
    else
    {
        QString info;
        for(int i=0;i<result.size();++i)
        {
            info=info+result[i]+"\n";
        }
        ui->label_resultDisplay->setText(info);
    }
}

QVector<QString> Widget::creat()
{
    QVector<QString> result;
    if(this->MAX_BIT==0)
    {
        QMessageBox::information(this,tr("ERROR"),tr("请先输入变量位数！"));
        return result;
    }
    else
    {
        QString tmpc;//临时存放逻辑结果
        bool flag1,flag2;
        QVector<QVector<QString> > v;//包含无关项
        QVector<QString> relative;//不包含无关项
        QVector<QString> prime;

        //input progress
        v.resize(this->MAX_BIT+1);
        for(int i=0;i<this->inputvar.size();++i)
        {
            if(inputvar[i]._str!="")
            {
                for(int j=0;j<inputvar[i]._str.length();++j)
                {
                    if(inputvar[i]._str[j]!='0'&& inputvar[i]._str[j]!="1")
                    {
                        QMessageBox::information(this,tr("ERROR"),tr("变量输入有误！"));
                        return result;
                    }
                }
                v[0].push_back(inputvar[i]._str);
                if(!inputvar[i]._isirr)
                {
                    relative.push_back(inputvar[i]._str);
                }
            }
        }

        //creat prime
        //i1：查找遍数 i2,i3：遍历最小项 i4：遍历本次查找已生成的最小项
        //flag1：判断质蕴含项  flag2：判断生成的最小项是否已存在
        for (int i1 = 0; i1 < v.size(); ++i1)
        {
            if (v[i1].empty())
                break;
            for (int i2 = 0; i2 < v[i1].size(); ++i2)
            {
                flag1 = false;
                for (int i3 = 0; i3 < v[i1].size(); ++i3)
                {
                    if (this->creat_implicant(v[i1][i2], v[i1][i3]) != -1)
                    {
                        tmpc=v[i1][i2];
                        tmpc[this->creat_implicant(v[i1][i2], v[i1][i3])] = '-';
                        flag1 = true;
                        flag2 = true;
                        for (int i4 = 0; i4 < v[i1 + 1].size(); ++i4)
                        {
                            if (v[i1 + 1][i4]==tmpc) flag2 = false;
                        }
                        if (flag2) v[i1 + 1].push_back(tmpc);
                    }
                }
                if (!flag1)
                {
                    prime.push_back(v[i1][i2]);
                }
            }
        }

        //creat EPI

        //若某列仅有一行覆盖，直接生成实质蕴含项
        for (int i = 0; i < relative.size() && relative.empty(); ++i)
        {
            int cnt = 0,tmpi;
            for (int j = 0; j < prime.size() && !prime.empty(); ++j)
            {
                if (this->creat_comp(relative[i], prime[j]))
                {
                    ++cnt;
                    tmpi = j;
                }
            }
            if (cnt == 1)
            {
                result.push_back(prime[tmpi]);

                for (int j = 0; j < relative.size(); ++j)
                {
                    if (this->creat_comp(relative[j], prime[tmpi]))
                    {
                        relative.erase(relative.begin() + tmpi);
                        --j;
                    }
                }
                prime.erase(prime.begin() + tmpi);
                --i;
            }
        }

        while (!relative.empty())
        {
            tmpc= prime.back();
            prime.pop_back();
            int cnt = 0;
            for (int i = 0; i < relative.size(); ++i)
            {
                if (this->creat_comp(relative[i], tmpc))
                {
                    relative.erase(relative.begin() + i);
                    --i;
                    ++cnt;
                }
            }
            if (cnt > 0)
            {
                result.push_back(tmpc);
            }
        }

        return result;
    }
}

//仅1位不相同，返回不相同位下标，否则返回1
int Widget::creat_implicant(QString a, QString b)
{
    int cnt = 0, tmp;
    for (int i = 0; i < this->MAX_BIT; ++i)
    {
        if (a[i] != b[i])
        {
            ++cnt;
            tmp = i;
        }
    }
    if (cnt == 1)
        return tmp;
    else
        return -1;
}

//仅判断数字位(0,1)是否相同，忽略'-'
bool Widget::creat_comp(QString a, QString b)
{
    for (int i = 0; i < this->MAX_BIT; ++i)
    {
        if (a[i] != b[i] && (a[i] != '-' && b[i] != '-'))
            return false;
    }
    return true;
}
