#ifndef STORE_H
#define STORE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

class Product {
public:
    std::string imagePath;
    std::string name;
    int price;
    int remainingQuantity;
    int quantity;
    bool onSale;

    Product(std::string i, std::string n, int p, int rq, int q, bool sale)
        : imagePath(i), name(n), price(p), remainingQuantity(rq), quantity(q), onSale(sale) {}

    void display() const;  // Thêm const để đảm bảo hàm không thay đổi đối tượng
};

class MyStore {
private:
    std::vector<Product> product_list;
    std::vector<Product> bill;
public:
    void addProduct(const Product&);  // Thêm const vào tham số product
    void loadProduct(const std::string&) ;
    void showAllProduct();              // Thêm const để không thay đổi đối tượng
    void ShowInformProduct(std::vector<Product>&);
    std::vector<Product> getProducts() const; // Thêm const vào phương thức
    void sortProductsByPrice(bool ascending);
    void getOutOfStockProduct(); // Thêm const vào phương thức
    void getSaleProduct();       // Thêm const vào phương thức
    void saveProductsToFile(const std::string&, const std::vector<Product>&);
    int calculateTotal();
    void BuyProduct(const std::string&);
    void RemoveProduct(const std::string&);
    void saveBillToFile(const std::string&, const std::vector<Product>&);
    void saveFinalBill();
};

#endif
