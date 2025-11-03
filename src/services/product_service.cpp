#include "../../includes/services/product_service.h"
#include <ctime>
#include <algorithm>

ProductService::ProductService(std::shared_ptr<XMLDataManager> data_manager)
    : data_manager_(data_manager) {}

bool ProductService::PublishProduct(
    const std::map<std::string, std::string>& product_data,
    const std::string& seller_id) {

    std::string product_name = product_data.at("name");
    std::string description = product_data.at("description");
    double price = std::stod(product_data.at("price"));
    std::string category = product_data.at("category");
    std::string image_path = product_data.count("image_path") > 0 ?
                                 product_data.at("image_path") : "";
    int quantity = std::stoi(product_data.at("quantity"));  // 获取数量

    std::time_t current_time = std::time(nullptr);
    Product product("", product_name, description, price, category,
                    image_path, current_time, seller_id, quantity);  // 传递数量

    return data_manager_->SaveProduct(product);
}

std::vector<Product*> ProductService::SearchProducts(
    const std::string& keyword, const std::string& category) {

    return data_manager_->SearchProducts(keyword, category);
}

bool ProductService::UpdateProduct(
    const std::string& product_id,
    const std::map<std::string, std::string>& updates,
    const std::string& user_id, bool is_admin) {

    // 获取产品信息以检查权限
    auto products = data_manager_->GetProducts();
    Product* target_product = nullptr;

    for (auto* product : products) {
        if (product->GetProductId() == product_id) {
            target_product = product;
            break;
        }
    }

    if (!target_product) {
        // 清理内存
        for (auto* product : products) {
            delete product;
        }
        return false;
    }

    // 权限检查：只有管理员或产品所有者可以修改产品
    if (!is_admin && target_product->GetSellerId() != user_id) {
        // 清理内存
        for (auto* product : products) {
            delete product;
        }
        return false;
    }

    // 更新产品信息
    std::string product_name = updates.count("name") > 0 ?
                                   updates.at("name") : target_product->GetProductName();
    std::string description = updates.count("description") > 0 ?
                                  updates.at("description") : target_product->GetDescription();
    double price = updates.count("price") > 0 ?
                       std::stod(updates.at("price")) : target_product->GetPrice();
    std::string category = updates.count("category") > 0 ?
                               updates.at("category") : target_product->GetCategory();
    std::string image_path = updates.count("image_path") > 0 ?
                                 updates.at("image_path") : target_product->GetImagePath();

    // 创建更新后的产品
    Product updated_product(product_id, product_name, description, price,
                            category, image_path, target_product->GetPublishTime(),
                            target_product->GetSellerId());

    // 删除旧产品并保存更新后的产品
    data_manager_->DeleteProduct(product_id);
    bool success = data_manager_->SaveProduct(updated_product);

    // 清理内存
    for (auto* product : products) {
        delete product;
    }

    return success;
}

bool ProductService::DeleteProduct(const std::string& product_id,
                                   const std::string& user_id, bool is_admin) {

    // 获取产品以检查权限
    auto products = data_manager_->GetProducts();
    Product* target_product = nullptr;

    for (auto* product : products) {
        if (product->GetProductId() == product_id) {
            target_product = product;
            break;
        }
    }

    if (!target_product) {
        // 清理内存
        for (auto* product : products) {
            delete product;
        }
        return false;
    }

    // 权限检查：只有管理员或产品所有者可以删除产品
    if (!is_admin && target_product->GetSellerId() != user_id) {
        // 清理内存
        for (auto* product : products) {
            delete product;
        }
        return false;
    }

    // 清理内存
    for (auto* product : products) {
        delete product;
    }

    return data_manager_->DeleteProduct(product_id);
}

std::vector<Product*> ProductService::GetAllProducts() {
    return data_manager_->GetProducts();
}

std::vector<Product*> ProductService::GetProductsBySeller(const std::string& seller_id) {
    auto all_products = data_manager_->GetProducts();
    std::vector<Product*> seller_products;

    for (auto* product : all_products) {
        if (product->GetSellerId() == seller_id) {
            seller_products.push_back(product);
        } else {
            delete product;
        }
    }

    return seller_products;
}
