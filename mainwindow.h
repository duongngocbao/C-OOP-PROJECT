#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "store.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    MyStore cuahang;
    Ui::MainWindow *ui;
    void loadCSVData(const std::string&);
    void loadCSV_BillData(const std::string&);
    void setupImageCell(int, const QString&);
    void setupImageCell_Bill(int, const QString&);
    int getRowCount(const QString&);
    void onBuyButtonClicked(const QString&);
    void onRemoveButtonClicked(const QString&);

};
#endif // MAINWINDOW_H
