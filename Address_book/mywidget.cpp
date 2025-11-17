#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

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
    
    // 設定表格欄寬自適應
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // 設定視窗背景色
    setStyleSheet("QWidget { background-color: #f5f6fa; }");
    
    // 連接搜尋框的文字變更信號以實現即時搜尋
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &Mywidget::on_searchLineEdit_textChanged);
    
    // 初始化狀態標籤
    updateStatusLabel();
}

Mywidget::~Mywidget()
{
    delete ui;
}

// 更新狀態標籤，顯示當前記錄數
void Mywidget::updateStatusLabel()
{
    int visibleRows = 0;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if (!ui->tableWidget->isRowHidden(i)) {
            visibleRows++;
        }
    }
    ui->statusLabel->setText(QString("📊 總記錄數：%1").arg(visibleRows));
}

// 過濾表格功能
void Mywidget::filterTable(const QString &searchText)
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = false;
        
        if (searchText.isEmpty()) {
            match = true;
        } else {
            // 檢查每一列的內容是否包含搜尋文字
            for (int col = 0; col < ui->tableWidget->columnCount(); col++) {
                QTableWidgetItem *item = ui->tableWidget->item(row, col);
                if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
        }
        
        ui->tableWidget->setRowHidden(row, !match);
    }
    
    updateStatusLabel();
}

// 新增一行資料
void Mywidget::on_pushButton_4_clicked()
{
    // 驗證輸入
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_3->text().isEmpty()) {
        QMessageBox::warning(this, "輸入錯誤", "請至少填寫學號和姓名！");
        return;
    }
    
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // 將輸入框中的資料添加到表格中
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ui->lineEdit->text()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ui->lineEdit_3->text()));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(ui->lineEdit_4->text()));
    
    // 清空輸入框
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    
    // 更新狀態
    updateStatusLabel();
    
    // 顯示成功訊息
    QMessageBox::information(this, "新增成功", "記錄已成功新增！");
}

// 匯出資料到 CSV 檔案
void Mywidget::on_pushButton_3_clicked()
{
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::warning(this, "匯出錯誤", "沒有資料可以匯出！");
        return;
    }
    
    QString saveFile = "";

    // 顯示儲存檔案的對話框
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "儲存檔案",
                                                    "C:/",
                                                    "CSV 檔案 (*.csv);;所有檔案 (*.*)");

    if (fileName.isEmpty()) {
        return;  // 使用者取消選擇
    }

    // 儲存檔案的邏輯
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        // 只匯出可見的行
        if (ui->tableWidget->isRowHidden(i)) continue;
        
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            saveFile += (item ? item->text() : "");
            if (j < ui->tableWidget->columnCount() - 1)
                saveFile += ",";
        }
        saveFile += "\n";
    }

    // 將資料寫入選擇的檔案
    write(fileName, saveFile);
    QMessageBox::information(this, "匯出完成", "資料已匯出至 " + fileName);
}

// 匯入資料從 CSV 檔案
void Mywidget::on_pushButton_6_clicked()
{
    // 顯示打開檔案的對話框
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "打開檔案",
                                                    "C:/",
                                                    "CSV 檔案 (*.csv);;所有檔案 (*.*)");

    if (fileName.isEmpty()) {
        return;  // 使用者取消選擇
    }

    // 讀取檔案資料
    QString fileData = readFile(fileName);
    if (fileData.isEmpty()) {
        QMessageBox::warning(this, "錯誤", "無法讀取檔案");
        return;
    }

    ui->tableWidget->setRowCount(0);  // 清空表格

    QStringList rows = fileData.split("\n");
    for (const QString &row : rows) {
        if (row.trimmed().isEmpty()) continue;

        QStringList cols = row.split(",");
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        for (int j = 0; j < cols.size() && j < 4; j++) {
            ui->tableWidget->setItem(newRow, j, new QTableWidgetItem(cols[j]));
        }
    }

    updateStatusLabel();
    QMessageBox::information(this, "匯入完成", "資料已匯入");
}

// 結束程式
void Mywidget::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "確認結束", "確定要結束程式嗎？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        close();
    }
}

// 清空輸入欄位
void Mywidget::on_pushButton_clear_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit->setFocus();
}

// 刪除選中的行
void Mywidget::on_pushButton_delete_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "刪除錯誤", "請先選擇要刪除的記錄！");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "確認刪除", "確定要刪除選中的記錄嗎？",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QSet<int> rowsToDelete;
        for (QTableWidgetItem* item : selectedItems) {
            rowsToDelete.insert(item->row());
        }
        
        // 從後往前刪除，避免索引變化
        QList<int> sortedRows = rowsToDelete.values();
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());
        
        for (int row : sortedRows) {
            ui->tableWidget->removeRow(row);
        }
        
        updateStatusLabel();
        QMessageBox::information(this, "刪除成功", "選中的記錄已刪除！");
    }
}

// 搜尋功能
void Mywidget::on_pushButton_search_clicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();
    
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "搜尋提示", "請輸入搜尋關鍵字！");
        return;
    }
    
    filterTable(searchText);
}

// 顯示全部記錄
void Mywidget::on_pushButton_showAll_clicked()
{
    ui->searchLineEdit->clear();
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        ui->tableWidget->setRowHidden(row, false);
    }
    updateStatusLabel();
}

// 即時搜尋功能（當搜尋框文字改變時）
void Mywidget::on_searchLineEdit_textChanged(const QString &text)
{
    filterTable(text);
}
