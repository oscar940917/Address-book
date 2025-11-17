#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

namespace Ui {
class Mywidget;
}

class Mywidget : public QWidget
{
    Q_OBJECT

public:
    explicit Mywidget(QWidget *parent = nullptr);
    ~Mywidget();

private slots:
    void on_pushButton_4_clicked();   // 新增按鈕
    void on_pushButton_3_clicked();   // 匯出按鈕
    void on_pushButton_6_clicked();   // 匯入按鈕
    void on_pushButton_5_clicked();   // 結束按鈕

private:
    Ui::Mywidget *ui;
};

#endif // MYWIDGET_H
