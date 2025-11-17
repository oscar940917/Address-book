#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

// 檔案路徑（你可以根據需要修改）
QString mfilename = "C:\\Users\\user\\Desktop\\EX\\Address-book\\data.csv";

// 寫入檔案
void write(QString Filename, QString str)
{
    QFile mfile(Filename);
    if (!mfile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "could not open file for write";
        return;
    }
    QTextStream out(&mfile);
    out << str;
    mfile.close();
}

// 讀取檔案
QString readFile(QString Filename)
{
    QFile mfile(Filename);
    if (!mfile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "could not open file for read";
        return "";
    }
    QTextStream in(&mfile);
    QString data = in.readAll();
    mfile.close();
    return data;
}

Mywidget::Mywidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Mywidget)
{
    ui->setupUi(this);

    // 設定表格欄位
    QStringList ColTotle;
    ui->tableWidget->setColumnCount(4);
    ColTotle << "學號" << "班級" << "姓名" << "電話";
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

Mywidget::~Mywidget()
{
    delete ui;
}

// 新增一行資料
void Mywidget::on_pushButton_4_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // 將輸入框中的資料添加到表格中
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ui->lineEdit->text()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ui->lineEdit_3->text()));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(ui->lineEdit_4->text()));
}

// 匯出資料到 CSV 檔案
void Mywidget::on_pushButton_3_clicked()
{
    QString saveFile = "";

    // 遍歷表格資料，生成 CSV 格式字串
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            saveFile += (item ? item->text() : "");
            if (j < ui->tableWidget->columnCount() - 1)
                saveFile += ",";
        }
        saveFile += "\n";
    }

    // 寫入 CSV 檔案
    write(mfilename, saveFile);
    QMessageBox::information(this, "匯出完成", "資料已匯出至 data.csv");
}

// 匯入資料從 CSV 檔案
void Mywidget::on_pushButton_6_clicked()
{
    QString fileData = readFile(mfilename);
    if (fileData.isEmpty()) {
        QMessageBox::warning(this, "錯誤", "無法讀取檔案");
        return;
    }

    // 清空表格資料
    ui->tableWidget->setRowCount(0);

    // 解析檔案中的每一行
    QStringList rows = fileData.split("\n");
    for (const QString &row : rows) {
        if (row.trimmed().isEmpty()) continue;

        QStringList cols = row.split(",");
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        // 填充表格的每一個儲存格
        for (int j = 0; j < cols.size() && j < 4; j++) {
            ui->tableWidget->setItem(newRow, j, new QTableWidgetItem(cols[j]));
        }
    }

    QMessageBox::information(this, "匯入完成", "資料已匯入");
}

// 結束程式
void Mywidget::on_pushButton_5_clicked()
{
    close();  // 關閉應用程式
}
