#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "store.h"
#include <QFile>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget ->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidget_2 ->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    // MyStore cuahang;
     QString FileName = QCoreApplication::applicationDirPath() + "/Products.csv";
    cuahang.loadProduct(FileName.toStdString());
    loadCSVData(FileName.toStdString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCSVData(const std::string& FileName)
{

    ui->tableWidget->clearContents(); // Xóa nội dung bảng
    ui->tableWidget->setRowCount(0);   // Đặt lại số hàng về 0


    int rowCount = getRowCount(QString::fromStdString(FileName));
    ui -> tableWidget -> setRowCount(rowCount - 1);
    QFile CSVFile(QString::fromStdString(FileName));
    int row = 0;

    if (CSVFile.open(QIODevice::ReadOnly)) {
        QTextStream Stream(&CSVFile);
        Stream.readLine(); // Bỏ qua dòng đầu tiên (header)

        while (!Stream.atEnd()) {
            QString LineData = Stream.readLine();
            QStringList Data = LineData.split(",");

            if (Data.size() < 6) {
                qDebug() << "Insufficient data in line:" << LineData;
                continue;
            }

            // Thiết lập ảnh cho cột 0
            setupImageCell(row, Data[0]);

            // Tạo các item cho từng cột và thiết lập font và căn giữa
            QTableWidgetItem* nameItem = new QTableWidgetItem(Data[1]);
            QFont nameFont = nameItem->font();
            nameFont.setPointSize(12); // Tăng kích thước chữ cho tên
            nameItem->setFont(nameFont);
            nameItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 1, nameItem); // Name

            QTableWidgetItem* priceItem = new QTableWidgetItem(Data[2] + "đ");
            QFont priceFont = priceItem->font();
            priceFont.setPointSize(14); // Tăng kích thước chữ cho giá
            priceItem->setFont(priceFont);
            priceItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 2, priceItem); // Price

            QTableWidgetItem* quantityItem = new QTableWidgetItem(Data[3] + "/" + Data[4]);
            QFont quantityFont = quantityItem->font();
            quantityFont.setPointSize(14); // Tăng kích thước chữ cho số lượng
            quantityItem->setFont(quantityFont);
            quantityItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 3, quantityItem); // Quantity/Other

            QTableWidgetItem* otherInfoItem = new QTableWidgetItem(Data[5]);
            QFont otherInfoFont = otherInfoItem->font();
            otherInfoFont.setPointSize(14); // Tăng kích thước chữ cho thông tin khác
            otherInfoItem->setFont(otherInfoFont);
            otherInfoItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 4, otherInfoItem); // Other Info

            // Thêm nút "Buy" vào cột 5
            QPushButton* buyButton = new QPushButton("Buy");
            ui->tableWidget->setCellWidget(row, 5, buyButton);
            connect(buyButton, &QPushButton::clicked, this, [this, Data]() {
                onBuyButtonClicked(Data[1]);
            });

            ui->tableWidget->setRowHeight(row, 80);
            row++;
        }
        ui->tableWidget->setColumnWidth(0, 80);
    } else {
        qDebug() << "Failed to open the CSV file.";
    }
}
void MainWindow::loadCSV_BillData(const std::string& FileName)
{

    ui->tableWidget_2->clearContents(); // Xóa nội dung bảng
    ui->tableWidget_2->setRowCount(0);   // Đặt lại số hàng về 0


    int rowCount = getRowCount(QString::fromStdString(FileName));
    ui -> tableWidget_2 -> setRowCount(rowCount - 1);
    QFile CSVFile(QString::fromStdString(FileName));
    int row = 0;

    if (CSVFile.open(QIODevice::ReadOnly)) {
        QTextStream Stream(&CSVFile);
        while (!Stream.atEnd()) {
            QString LineData = Stream.readLine();
            QStringList Data = LineData.split(",");

            if (Data.size() < 6) {
                qDebug() << "Insufficient data in line:" << LineData;
                continue;
            }

            // Thiết lập ảnh cho cột 0
            setupImageCell_Bill(row, Data[0]);

            // Tạo các item cho từng cột và thiết lập font và căn giữa
            QTableWidgetItem* nameItem = new QTableWidgetItem(Data[1]);
            QFont nameFont = nameItem->font();
            nameFont.setPointSize(12); // Tăng kích thước chữ cho tên
            nameItem->setFont(nameFont);
            nameItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_2->setItem(row, 1, nameItem); // Name

            QTableWidgetItem* priceItem = new QTableWidgetItem(Data[2] + "đ");
            QFont priceFont = priceItem->font();
            priceFont.setPointSize(14); // Tăng kích thước chữ cho giá
            priceItem->setFont(priceFont);
            priceItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_2->setItem(row, 2, priceItem); // Price

            QTableWidgetItem* quantityItem = new QTableWidgetItem(Data[3]);
            QFont quantityFont = quantityItem->font();
            quantityFont.setPointSize(14); // Tăng kích thước chữ cho số lượng
            quantityItem->setFont(quantityFont);
            quantityItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_2->setItem(row, 3, quantityItem); // Quantity/Other

            QPushButton* removeButton = new QPushButton("Remove");
            ui->tableWidget_2->setCellWidget(row, 4, removeButton);
            connect(removeButton, &QPushButton::clicked, this, [this, Data]() {
                onRemoveButtonClicked(Data[1]);
            });
            ui->tableWidget_2->setRowHeight(row, 80);
            row++;
        }
        ui->tableWidget_2->setColumnWidth(0, 80);
    } else {
        qDebug() << "Failed to open the CSV file.";
    }
    ui -> label_2 -> setText("Total = " + QString::number(cuahang.calculateTotal())+ "đ");
}
void MainWindow::setupImageCell(int row, const QString& imageName) {
    // QString imagePath = "C:\\Users\\Uncle Cuong\\Documents\\ProjectOPP\\" + imageName;
    QString imagePath = QCoreApplication::applicationDirPath()+ "\\" + imageName;
    QPixmap pixmap(imagePath);

    if (!pixmap.isNull()) {
        QLabel* label = new QLabel();
        label->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter); // Căn giữa ảnh
        ui->tableWidget->setCellWidget(row, 0, label);
    } else {
        qDebug() << "Image not found:" << imagePath;
    }
}
void MainWindow::setupImageCell_Bill(int row, const QString& imageName) {
    // QString imagePath = "C:\\Users\\Uncle Cuong\\Documents\\ProjectOPP\\" + imageName;
    QString imagePath = QCoreApplication::applicationDirPath() + "\\" + imageName;
    QPixmap pixmap(imagePath);

    if (!pixmap.isNull()) {
        QLabel* label = new QLabel();
        label->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter); // Căn giữa ảnh
        ui->tableWidget_2->setCellWidget(row, 0, label);
    } else {
        qDebug() << "Image not found:" << imagePath;
    }
}
int MainWindow::getRowCount(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.";
        return 0;
    }

    QString content = file.readAll();
    file.close();

    QStringList lines = content.split('\n'); // Bỏ qua dòng trống

    return lines.size();
}
//Sort by Increase
void MainWindow::on_pushButton_clicked()
{
    cuahang.sortProductsByPrice(true);
    QString FileName = QCoreApplication::applicationDirPath() + "/Products_Increase.csv";
    loadCSVData(FileName.toStdString());
    qDebug() << "Sort by Increase" << Qt::endl;
}

//Sort by Decrease
void MainWindow::on_pushButton_2_clicked()
{
    cuahang.sortProductsByPrice(false);
    QString FileName = QCoreApplication::applicationDirPath() + "/Products_Decrease.csv";
    loadCSVData(FileName.toStdString());
    qDebug() << "Sort by Decrease" << Qt::endl;
}
//No Sort
void MainWindow::on_pushButton_3_clicked()
{
    QString FileName = QCoreApplication::applicationDirPath() + "/Products.csv";
    loadCSVData(FileName.toStdString());
}

//Sale Product
void MainWindow::on_pushButton_4_clicked()
{
    cuahang.getSaleProduct();
    QString FileName = QCoreApplication::applicationDirPath() + "/Products_Sale.csv";
    loadCSVData(FileName.toStdString());
}

//Out Of Stock Product
void MainWindow::on_pushButton_5_clicked()
{
    cuahang.getOutOfStockProduct();
    QString FileName = QCoreApplication::applicationDirPath() + "/Products_OutOfStock.csv";
    loadCSVData(FileName.toStdString());
}
//Buy Product
void MainWindow::onBuyButtonClicked(const QString& productName) {
    // Gọi hàm BuyProduct của cuahang
    cuahang.BuyProduct(productName.toStdString());

    // Cập nhật lại bảng sau khi mua hàng
    QString FileName = QCoreApplication::applicationDirPath() + "/Products.csv";
    loadCSVData(FileName.toStdString());
    loadCSV_BillData(QString(QCoreApplication::applicationDirPath() + "/bill.csv").toStdString());
}
//Remove Product
void MainWindow::onRemoveButtonClicked(const QString& productName) {
    // Gọi hàm RemoveProduct của cuahang
    cuahang.RemoveProduct(productName.toStdString());
    // Cập nhật lại bảng sau khi mua hàng
    QString FileName = QCoreApplication::applicationDirPath() + "/Products.csv";
    loadCSVData(FileName.toStdString());
    loadCSV_BillData(QString(QCoreApplication::applicationDirPath() + "/bill.csv").toStdString());
    qDebug() << "Remove";
}

void MainWindow::on_pushButton_6_clicked()
{
    cuahang.saveFinalBill();
}

