#include "../../includes/models/product.h"

Product::Product() : product_id_(""), product_name_(""), description_(""),
    price_(0.0), category_(""), image_path_(""),
    publish_time_(0), seller_id_(""), quantity_(1) {}

Product::Product(const std::string& product_id, const std::string& product_name,
                 const std::string& description, double price,
                 const std::string& category, const std::string& image_path,
                 std::time_t publish_time, const std::string& seller_id,
                 int quantity)
    : product_id_(product_id), product_name_(product_name),
    description_(description), price_(price), category_(category),
    image_path_(image_path), publish_time_(publish_time),
    seller_id_(seller_id), quantity_(quantity) {}

bool Product::Publish() {
    return true;
}

bool Product::Update() {
    return true;
}

bool Product::Delete() {
    return true;
}
