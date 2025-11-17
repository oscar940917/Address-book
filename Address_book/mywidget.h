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
    void on_pushButton_4_clicked();      // 新增按鈕
    void on_pushButton_3_clicked();      // 匯出按鈕
    void on_pushButton_6_clicked();      // 匯入按鈕
    void on_pushButton_5_clicked();      // 結束按鈕
    void on_pushButton_clear_clicked();  // 清空輸入欄位
    void on_pushButton_delete_clicked(); // 刪除選中的行
    void on_pushButton_search_clicked(); // 搜尋功能
    void on_pushButton_showAll_clicked();// 顯示全部
    void on_searchLineEdit_textChanged(const QString &text); // 即時搜尋

private:
    Ui::Mywidget *ui;
    void updateStatusLabel();            // 更新狀態標籤
    void filterTable(const QString &searchText); // 過濾表格
};

#endif // MYWIDGET_H
