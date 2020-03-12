#include <iostream>
#include <vector>
#include <string>

using namespace std;

int MAX_BIT;
string tmpc;               //临时存放逻辑结果
vector<vector<string> > v; //包含无关项
vector<string> relative;   //不包含无关项
vector<string> prime;
vector<string> result;

int implicant(string a, string b); //仅1位不相同，返回不相同位下标，否则返回1
bool comp(string a, string b);     //仅判断数字位(0,1)是否相同，忽略'-'
void getInput();
void creat_prime();
void creat_EPI();
void oput();

int main()
{
    getInput();
    creat_prime();
    creat_EPI();
    oput();
    return 0;
}

bool comp(string a, string b)
{
    for (int i = 0; i < MAX_BIT; ++i)
    {
        if (a[i] != b[i] && (a[i] != '-' && b[i] != '-'))
            return false;
    }
    return true;
}

int implicant(string a, string b)
{
    int cnt = 0, tmp;
    for (int i = 0; i < MAX_BIT; ++i)
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

void getInput()
{
    bool flag;
    cout << "输入变量个数" << endl;
    cin >> MAX_BIT;
    v.resize(MAX_BIT + 1);

    cout << "输入最小项（以2结束）" << endl;
    flag = true;
    while (flag)
    {
        cin >> tmpc;
        for (int i = 0; i < MAX_BIT; ++i)
        {
            if (tmpc[i] != '0' && tmpc[i] != '1')
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            v[0].push_back(tmpc);
            relative.push_back(tmpc);
        }
    }

    cout << "无关项（以2结束）" << endl;
    flag = true;
    while (flag)
    {
        cin >> tmpc;
        for (int i = 0; i < MAX_BIT; ++i)
        {
            if (tmpc[i] != '0' && tmpc[i] != '1')
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            v[0].push_back(tmpc);
        }
    }
}

void creat_prime()
{
    //i1：查找遍数 i2,i3：遍历最小项 i4：遍历本次查找已生成的最小项
    int i1, i2, i3, i4;
    //flag1：判断质蕴含项  flag2：判断生成的最小项是否已存在
    bool flag1, flag2;

    for (i1 = 0; i1 < v.size(); ++i1)
    {
        if (v[i1].empty())
            break;
        for (i2 = 0; i2 < v[i1].size(); ++i2)
        {
            flag1 = false;
            for (i3 = 0; i3 < v[i1].size(); ++i3)
            {
                if (implicant(v[i1][i2], v[i1][i3]) != -1)
                {
                    tmpc = v[i1][i2];
                    tmpc[implicant(v[i1][i2], v[i1][i3])] = '-';
                    flag1 = true;
                    flag2 = true;
                    for (i4 = 0; i4 < v[i1 + 1].size(); ++i4)
                    {
                        if (v[i1 + 1][i4] == tmpc)
                            flag2 = false;
                    }
                    if (flag2)
                    {
                        v[i1 + 1].push_back(tmpc);
                    }
                }
            }
            if (!flag1)
            {
                prime.push_back(v[i1][i2]);
            }
        }
    }
}

void creat_EPI()
{
    int i, j, cnt, tmpi;
    //若某列仅有一行覆盖，直接生成实质蕴含项
    for (i = 0; i < relative.size() && relative.empty(); ++i)
    {
        cnt = 0;
        for (j = 0; j < prime.size() && !prime.empty(); ++j)
        {
            if (comp(relative[i], prime[j]))
            {
                ++cnt;
                tmpi = j;
            }
        }
        if (cnt == 1)
        {
            result.push_back(prime[tmpi]);
            for (j = 0; j < relative.size(); ++j)
            {
                if (comp(relative[j], prime[tmpi]))
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
        tmpc = prime.back();
        prime.pop_back();
        cnt = 0;
        for (i = 0; i < relative.size(); ++i)
        {
            if (comp(relative[i], tmpc))
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
}

void oput()
{
    cout << endl
         << "------------------------------" << endl
         << endl;
    ;
    while (!result.empty())
    {
        cout << result.back();
        cout << endl;
        result.pop_back();
    }
    cout << endl
         << "------------------------------" << endl;
    cout << endl;
    system("pause");
}