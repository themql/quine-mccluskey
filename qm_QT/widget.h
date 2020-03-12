#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include "inputvar.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    int MAX_BIT=0;
    QVector<INPUTVAR> inputvar;

    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void displayInputList();
    QVector<QString> creat();
    int  creat_implicant(QString a, QString b);
    bool creat_comp(QString a, QString b);

public slots:
    void input_maxbit();
    void input_inputvar();
    void button_creat();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
