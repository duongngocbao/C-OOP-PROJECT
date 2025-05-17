#include "store.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>       // Thư viện lấy thời gian hiện tại
#include <iomanip>     // Thư viện định dạng đầu ra
#include <random>      // Thư viện tạo mã ngẫu nhiên
#include <QCoreApplication>
using namespace std;

// ---------------------------Class Product---------------------------
void Product::display() const {
    cout << imagePath << " " << name << " " << price << " "
         << remainingQuantity << " " << quantity << " " << onSale << endl;
}

// ---------------------------Class MyStore---------------------------
// Thêm sản phẩm vào cửa hàng
void MyStore::addProduct(const Product& product) {
    product_list.push_back(product);
}
// Load sản phẩm từ file CSV
void MyStore::loadProduct(const std::string& filename) {
    ifstream inputFile(filename);

    if (!inputFile) {
        cout << "Không thể mở file!" << endl;
        return;
    } else {
        cout << "Đọc file thành công" << endl;
    }

    string line;
    getline(inputFile, line); // Bỏ qua dòng tiêu đề

    while (getline(inputFile, line)) {
        stringstream ss(line);

        string imagePath;
        string name;
        int price, remainingQuantity, totalQuantity, onSale;

        getline(ss, imagePath, ',');
        getline(ss, name, ',');
        ss >> price;
        ss.ignore(1, ',');
        ss >> remainingQuantity;
        ss.ignore(1, ',');
        ss >> totalQuantity;
        ss.ignore(1, ',');
        ss >> onSale;

        Product newProduct(imagePath, name, price, remainingQuantity, totalQuantity, onSale);
        addProduct(newProduct);
    }

    inputFile.close();
}
// In tất cả thông tin sản phẩm ra terminal
void MyStore::showAllProduct(){
    for (const auto& product : product_list) {
        product.display();
    }
}
// In thông tin sản phẩm theo nhu cầu
void MyStore::ShowInformProduct(vector<Product>& products){
    for (const auto& product : products){
        product.display();
    }
}
// Lấy danh sách sản phẩm
vector<Product> MyStore::getProducts() const {
    return product_list;
}
// Sắp sếp sản phẩm theo giá
void MyStore::sortProductsByPrice(bool ascending) {
    // qDebug() << "số sản phẩm trước khi sắp xếp: " << product_list.size();
    QString outputPath;
    if (ascending) {
        sort(product_list.begin(), product_list.end(), [](const Product& a, const Product& b) {
            return a.price < b.price;
        });
        outputPath = QCoreApplication::applicationDirPath() + "/Products_Increase.csv";
    } else {
        sort(product_list.begin(), product_list.end(), [](const Product& a, const Product& b) {
            return a.price > b.price;
        });
        outputPath = QCoreApplication::applicationDirPath() + "/Products_Decrease.csv";
    }
    // qDebug() << "số sản phẩm sau khi sắp xếp: " << product_list.size();
    saveProductsToFile(outputPath.toStdString(), product_list); // Lưu lại dữ liệu sau khi sắp xếp
}

// lấy danh sách sản phẩm hết hàng
void MyStore::getOutOfStockProduct(){
    vector<Product> outOfstock;
    QString outputPath = QCoreApplication::applicationDirPath() + "/Products_OutOfStock.csv";
    for (const auto& product : product_list) {
        if (product.remainingQuantity == 0) {
            outOfstock.push_back(product);
        }
    }
    ShowInformProduct(outOfstock);
    saveProductsToFile(outputPath.toStdString(), outOfstock);
}
// lấy danh sách sản phẩm có khuyến mãi
void MyStore::getSaleProduct(){
    vector<Product> saleProduct;
    QString outputPath = QCoreApplication::applicationDirPath() + "/Products_Sale.csv";
    for (const auto& product : product_list) {
        if (product.onSale) {
            saleProduct.push_back(product);
        }
    }
    ShowInformProduct(saleProduct);
    saveProductsToFile(outputPath.toStdString(), saleProduct);
}

// Hàm lưu danh sách sản phẩm hiện tại vào file CSV
void MyStore::saveProductsToFile(const std::string& filename, const vector<Product>& product_list) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cout << "Không thể mở file để ghi!" << endl;
        return;
    }

    // Ghi tiêu đề vào file CSV
    outputFile << "ImagePath,Name,Price,RemainingQuantity,TotalQuantity,OnSale\n";

    // Ghi từng sản phẩm vào file
    for (const auto& product : product_list) {
        outputFile << product.imagePath << ","
                   << product.name << ","
                   << product.price << ","
                   << product.remainingQuantity << ","
                   << product.quantity << ","
                   << product.onSale << "\n";
    }

    outputFile.close();
    cout << "Danh sách sản phẩm đã được lưu vào file: " << filename << endl;
}
//---------------------------Bill---------------------------
// Tính tổng tiền các sản phẩm trong hóa đơn, bao gồm giảm giá nếu onSale = 1
int MyStore::calculateTotal() {
    int total = 0;
    for (const auto& product : bill) {
        int effectivePrice = product.price;
        // Nếu sản phẩm đang onSale, giảm giá 20%
        if (product.onSale == 1) {
            effectivePrice = static_cast<int>(product.price * 0.8); // Giảm giá 20%
        }
        total += effectivePrice * product.remainingQuantity;
    }
    return total;
}

//Mua sản phẩm và nhập vào bill
void MyStore::BuyProduct(const std::string& productName) {
    // Tìm sản phẩm trong danh sách sản phẩm
    auto it = std::find_if(product_list.begin(), product_list.end(),
                           [&productName](const Product& p) { return p.name == productName; });

    // Nếu không tìm thấy sản phẩm
    if (it == product_list.end()) {
        cout << "Sản phẩm không tồn tại!" << endl;
        return;
    }

    // Nếu sản phẩm còn hàng
    if (it->remainingQuantity > 0) {
        // Giảm remainingQuantity của sản phẩm
        it->remainingQuantity--;

        // Tìm xem sản phẩm đã có trong hóa đơn chưa
        auto billIt = std::find_if(bill.begin(), bill.end(),
                                   [&productName](const Product& p) { return p.name == productName; });

        if (billIt != bill.end()) {
            // Nếu sản phẩm đã có trong hóa đơn, tăng số lượng lên
            billIt->remainingQuantity++; // Tăng số lượng đã mua
        } else {
            // Nếu sản phẩm chưa có trong hóa đơn, thêm mới vào hóa đơn
            Product purchasedProduct = *it; // Lưu sản phẩm đã mua
            purchasedProduct.remainingQuantity = 1; // Đặt số lượng là 1
            bill.push_back(purchasedProduct); // Thêm vào hóa đơn
        }

        // Cập nhật hóa đơn vào file bill.csv
        saveBillToFile(QString(QCoreApplication::applicationDirPath() + "/bill.csv").toStdString(), bill);
        // Lưu lại danh sách sản phẩm hiện tại vào file Products.csv
        saveProductsToFile(QString(QCoreApplication::applicationDirPath() + "/Products.csv").toStdString(), product_list);

        cout << "Mua sản phẩm thành công: " << productName << endl;
    } else {
        cout << "Sản phẩm đã hết hàng!" << endl;
    }
}
void MyStore::RemoveProduct(const std::string& productName) {
    // Tìm sản phẩm trong bill
    auto billIt = std::find_if(bill.begin(), bill.end(),
                               [&productName](const Product& p) { return p.name == productName; });

    // Nếu không tìm thấy sản phẩm trong hóa đơn
    if (billIt == bill.end()) {
        cout << "Sản phẩm không có trong hóa đơn!" << endl;
        return;
    }

    // Giảm remainingQuantity của sản phẩm trong bill
    billIt->remainingQuantity--;

    // Nếu remainingQuantity trong bill là 0, xóa sản phẩm khỏi hóa đơn
    if (billIt->remainingQuantity == 0) {
        bill.erase(billIt);
    }

    // Tìm sản phẩm trong danh sách sản phẩm (product_list)
    auto productIt = std::find_if(product_list.begin(), product_list.end(),
                                  [&productName](const Product& p) { return p.name == productName; });

    if (productIt != product_list.end()) {
        // Tăng remainingQuantity của sản phẩm trong product_list
        productIt->remainingQuantity++;
    } else {
        cout << "Sản phẩm không tồn tại trong cửa hàng!" << endl;
        return;
    }

    // Lưu lại hóa đơn vào file bill.csv
    saveBillToFile(QString(QCoreApplication::applicationDirPath() + "/bill.csv").toStdString(), bill);
    // Lưu lại danh sách sản phẩm hiện tại vào file Products.csv
    saveProductsToFile(QString(QCoreApplication::applicationDirPath() + "/Products.csv").toStdString(), product_list);

    cout << "Đã trả lại sản phẩm thành công: " << productName << endl;
}


// Hàm lưu hóa đơn vào file bill.csv
void MyStore::saveBillToFile(const std::string& filename, const vector<Product>& bill) {
    ofstream outputFile(filename); // Mở file để thêm thông tin
    if (!outputFile) {
        cout << "Không thể mở file để ghi hóa đơn!" << endl;
        return;
    }

    // Ghi từng sản phẩm vào file hóa đơn
    for (const auto& product : bill) {
        outputFile << product.imagePath << ","
                   << product.name << ","
                   << product.price << ","
                   << product.remainingQuantity << ","
                   << product.quantity << ","
                   << product.onSale << "\n";
    }

    outputFile.close();
    cout << "Hóa đơn đã được lưu vào file: " << filename << endl;
}



// Hàm tạo mã đơn hàng ngẫu nhiên
std::string generateOrderID(int length) {
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    std::string result;
    result.resize(length);

    std::default_random_engine rng(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    for (int i = 0; i < length; ++i) {
        result[i] = charset[dist(rng)];
    }

    return result;
}

// Hàm xuất hóa đơn cuối cùng và lưu vào file finnal_bill.txt
void MyStore::saveFinalBill() {
    ofstream outputFile(QString(QCoreApplication::applicationDirPath() + "/finnal_bill.txt").toStdString());
    if (!outputFile) {
        cout << "Không thể mở file để ghi hóa đơn cuối!" << endl;
        return;
    }

    // Lấy thời gian hiện tại
    time_t now = time(0);
    char* dt = ctime(&now);

    // Tạo mã đơn hàng ngẫu nhiên 10 ký tự
    std::string orderID = generateOrderID(10);

    // Tính tổng tiền
    int total = calculateTotal();

    // Ghi thời gian, mã đơn hàng và thông tin sản phẩm vào file
    outputFile << "Time: " << dt;
    outputFile << "Ma don hang: " << orderID << "\n\n";
    outputFile << left << setw(20) << "Name"
               << setw(10) << "Price"
               << setw(10) << "Quantity" << "\n";
    outputFile << string(40, '-') << "\n";  // Dòng ngăn cách

    for (const auto& product : bill) {
        outputFile << left << setw(20) << product.name
                   << setw(10) << product.price
                   << setw(10) << product.remainingQuantity << "\n";
    }

    outputFile << string(40, '-') << "\n";
    outputFile << "Total = " << total << "đ\n";
    outputFile.close();

    cout << "Hóa đơn cuối đã được lưu vào file: "<< QString(QCoreApplication::applicationDirPath() + "/finnal_bill.txt").toStdString()   << endl;
}
