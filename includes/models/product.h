#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <ctime>

/**
 * @brief Represents a product in the online shop system.
 */
class Product {
public:
    Product();
    Product(const std::string& product_id, const std::string& product_name,
            const std::string& description, double price,
            const std::string& category, const std::string& image_path,
            std::time_t publish_time, const std::string& seller_id,
            int quantity = 1);

    std::string GetProductId() const { return product_id_; }
    std::string GetProductName() const { return product_name_; }
    std::string GetDescription() const { return description_; }
    double GetPrice() const { return price_; }
    std::string GetCategory() const { return category_; }
    std::string GetImagePath() const { return image_path_; }
    std::time_t GetPublishTime() const { return publish_time_; }
    std::string GetSellerId() const { return seller_id_; }
    int GetQuantity() const { return quantity_; }

    void SetProductId(const std::string& product_id) { product_id_ = product_id; }
    void SetProductName(const std::string& product_name) { product_name_ = product_name; }
    void SetDescription(const std::string& description) { description_ = description; }
    void SetPrice(double price) { price_ = price; }
    void SetCategory(const std::string& category) { category_ = category; }
    void SetImagePath(const std::string& image_path) { image_path_ = image_path; }
    void SetPublishTime(std::time_t publish_time) { publish_time_ = publish_time; }
    void SetSellerId(const std::string& seller_id) { seller_id_ = seller_id; }
    void SetQuantity(int quantity) { quantity_ = quantity; }

    bool Publish();
    bool Update();
    bool Delete();

private:
    std::string product_id_;
    std::string product_name_;
    std::string description_;
    double price_;
    std::string category_;
    std::string image_path_;
    std::time_t publish_time_;
    std::string seller_id_;
    int quantity_;
};

#endif  // PRODUCT_H
