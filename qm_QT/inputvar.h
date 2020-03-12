#ifndef INPUTVAR_H
#define INPUTVAR_H

#include <QString>

class INPUTVAR
{
public:
    int _no;
    QString _str;
    bool _isirr;

    INPUTVAR():_no(0),_str(""),_isirr(false){};
    INPUTVAR(QString input, int max_bit);
    ~INPUTVAR()=default;
    void reset0();

private:
    QString noTOstr(int max_bit);
    int strTOno();
};

#endif // INPUTVAR_H
