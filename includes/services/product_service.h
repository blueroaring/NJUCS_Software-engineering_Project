#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../../includes/models/product.h"
#include "../../includes/data/xml_data_manager.h"

/**
 * @brief Handles product-related business operations.
 */
class ProductService {
public:
    explicit ProductService(std::shared_ptr<XMLDataManager> data_manager);

    bool PublishProduct(const std::map<std::string, std::string>& product_data,
                        const std::string& seller_id);
    std::vector<Product*> SearchProducts(const std::string& keyword,
                                          const std::string& category);

    // 修改方法签名，添加权限参数
    bool UpdateProduct(const std::string& product_id,
                       const std::map<std::string, std::string>& updates,
                       const std::string& user_id, bool is_admin);

    bool DeleteProduct(const std::string& product_id,
                       const std::string& user_id, bool is_admin);
    std::vector<Product*> GetAllProducts();
    std::vector<Product*> GetProductsBySeller(const std::string& seller_id);

private:
    std::shared_ptr<XMLDataManager> data_manager_;
};

#endif  // PRODUCT_SERVICE_H
