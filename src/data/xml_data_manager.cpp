#include "../../includes/data/xml_data_manager.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>
#include <random>
#include <iostream>
#include <algorithm>

XMLDataManager::XMLDataManager(const std::string& users_file,
                               const std::string& products_file)
    : users_file_(users_file), products_file_(products_file) {
   // QDir().mkpath("resource");
   // InitializeDemoData();
}

void XMLDataManager::InitializeDemoData() {
    QFile users_file(QString::fromStdString(users_file_));
    if (!users_file.exists()) {
        std::cout << "Creating demo users..." << std::endl;

        //create demo users
        User admin_user("admin", "admin", "admin123",
                        "admin@shop.com", "123456789", "admin");
        SaveUser(admin_user);

        User user1("user1", "user1", "password1",
                   "user1@shop.com", "111111111", "user");
        SaveUser(user1);

        User user2("user2", "user2", "password2",
                   "user2@shop.com", "222222222", "user");
        SaveUser(user2);

        User user3("user3","user3","password3","user3@shop.com","333333333","user");
        SaveUser(user3);
    }

    QFile products_file(QString::fromStdString(products_file_));
    if (!products_file.exists()) {
        std::cout << "Creating demo products..." << std::endl;

        //create demo products
        std::time_t current_time = std::time(nullptr);

        Product product1("1", "Laptop", "High performance gaming laptop", 999.99,
                         "electronics", "", current_time, "user1", 5);
        SaveProduct(product1);

        Product product2("2", "Book", "C++ Programming Guide", 29.99,
                         "books", "", current_time, "user2", 10);
        SaveProduct(product2);

        Product product3("3", "Smartphone", "Latest smartphone model", 699.99,
                         "electronics", "", current_time, "user1", 3);
        SaveProduct(product3);
    }
}

bool XMLDataManager::SaveUser(const User& user) {
    QFile file(QString::fromStdString(users_file_));
    QDomDocument doc;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        if (!doc.setContent(&file)) {
            file.close();
            return false;
        }
        file.close();
    } else {
        QDomElement root = doc.createElement("users");
        doc.appendChild(root);
    }

    QDomElement root = doc.documentElement();

    QDomNodeList users = root.elementsByTagName("user");
    for (int i = 0; i < users.count(); ++i) {
        QDomElement user_elem = users.at(i).toElement();
        QString stored_username = user_elem.elementsByTagName("username").at(0).toElement().text();

        if (stored_username == QString::fromStdString(user.GetUsername())) {
            user_elem.elementsByTagName("password").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(user.GetPassword()));
            user_elem.elementsByTagName("email").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(user.GetEmail()));
            user_elem.elementsByTagName("phone").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(user.GetPhone()));
            user_elem.elementsByTagName("role").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(user.GetRole()));

            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QTextStream stream(&file);
                doc.save(stream, 2);
                file.close();
                return true;
            }
            return false;
        }
    }

    //create new user elements
    QDomElement user_elem = doc.createElement("user");

    QDomElement user_id = doc.createElement("user_id");
    std::string user_id_value = user.GetUserId().empty() ? GenerateUserId() : user.GetUserId();
    user_id.appendChild(doc.createTextNode(QString::fromStdString(user_id_value)));

    QDomElement username = doc.createElement("username");
    username.appendChild(doc.createTextNode(QString::fromStdString(user.GetUsername())));

    QDomElement password = doc.createElement("password");
    password.appendChild(doc.createTextNode(QString::fromStdString(user.GetPassword())));

    QDomElement email = doc.createElement("email");
    email.appendChild(doc.createTextNode(QString::fromStdString(user.GetEmail())));

    QDomElement phone = doc.createElement("phone");
    phone.appendChild(doc.createTextNode(QString::fromStdString(user.GetPhone())));

    QDomElement role = doc.createElement("role");
    role.appendChild(doc.createTextNode(QString::fromStdString(user.GetRole())));

    user_elem.appendChild(user_id);
    user_elem.appendChild(username);
    user_elem.appendChild(password);
    user_elem.appendChild(email);
    user_elem.appendChild(phone);
    user_elem.appendChild(role);

    root.appendChild(user_elem);

    //save to file
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        doc.save(stream, 2);
        file.close();
        std::cout << "User saved successfully: " << user.GetUsername() << std::endl;
        return true;
    }

    std::cout << "Failed to save user: " << user.GetUsername() << std::endl;
    return false;
}

User* XMLDataManager::GetUser(const std::string& username) {
    QFile file(QString::fromStdString(users_file_));
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << "Cannot open users file: " << users_file_ << std::endl;
        return nullptr;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        std::cout << "Failed to parse XML content" << std::endl;
        return nullptr;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList users = root.elementsByTagName("user");
    std::cout<<users.count()<<std::endl;
    for (int i = 0; i < users.count(); ++i) {
        QDomElement user_elem = users.at(i).toElement();
        QString stored_username = user_elem.elementsByTagName("username").at(0).toElement().text();
        std::cout<<stored_username.toStdString()<<std::endl;
        if (stored_username == QString::fromStdString(username)) {
            std::string user_id = user_elem.elementsByTagName("user_id").at(0).toElement().text().toStdString();
            std::string password = user_elem.elementsByTagName("password").at(0).toElement().text().toStdString();
            std::string email = user_elem.elementsByTagName("email").at(0).toElement().text().toStdString();
            std::string phone = user_elem.elementsByTagName("phone").at(0).toElement().text().toStdString();
            std::string role = user_elem.elementsByTagName("role").at(0).toElement().text().toStdString();

            std::cout << "User found: " << username << " (Role: " << role << ")" <<"password:"<<password<< std::endl;
            return new User(user_id, username, password, email, phone, role);
        }
    }

    std::cout << "User not found: " << username << std::endl;
    return nullptr;
}

bool XMLDataManager::SaveProduct(const Product& product) {
    QFile file(QString::fromStdString(products_file_));
    QDomDocument doc;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        if (!doc.setContent(&file)) {
            file.close();
            return false;
        }
        file.close();
    } else {
        QDomElement root = doc.createElement("products");
        doc.appendChild(root);
    }

    QDomElement root = doc.documentElement();

    // check whether the product exist
    QDomNodeList products = root.elementsByTagName("product");
    for (int i = 0; i < products.count(); ++i) {
        QDomElement product_elem = products.at(i).toElement();
        QString stored_product_id = product_elem.elementsByTagName("product_id").at(0).toElement().text();

        if (stored_product_id == QString::fromStdString(product.GetProductId())) {
           //exist,update elements
            product_elem.elementsByTagName("name").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(product.GetProductName()));
            product_elem.elementsByTagName("description").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(product.GetDescription()));
            product_elem.elementsByTagName("price").at(0).toElement().firstChild().setNodeValue(
                QString::number(product.GetPrice()));
            product_elem.elementsByTagName("category").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(product.GetCategory()));
            product_elem.elementsByTagName("image_path").at(0).toElement().firstChild().setNodeValue(
                QString::fromStdString(product.GetImagePath()));

            //save to file
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QTextStream stream(&file);
                doc.save(stream, 2);
                file.close();
                return true;
            }
            return false;
        }
    }

    // create new product elements
    QDomElement product_elem = doc.createElement("product");

    QDomElement product_id = doc.createElement("product_id");
    std::string product_id_value = product.GetProductId().empty() ?
                                       GenerateProductId() : product.GetProductId();
    product_id.appendChild(doc.createTextNode(QString::fromStdString(product_id_value)));

    QDomElement name = doc.createElement("name");
    name.appendChild(doc.createTextNode(QString::fromStdString(product.GetProductName())));

    QDomElement description = doc.createElement("description");
    description.appendChild(doc.createTextNode(QString::fromStdString(product.GetDescription())));

    QDomElement price = doc.createElement("price");
    price.appendChild(doc.createTextNode(QString::number(product.GetPrice())));

    QDomElement category = doc.createElement("category");
    category.appendChild(doc.createTextNode(QString::fromStdString(product.GetCategory())));

    QDomElement quantity = doc.createElement("quantity");
    quantity.appendChild(doc.createTextNode(QString::number(product.GetQuantity())));

    QDomElement image_path = doc.createElement("image_path");
    image_path.appendChild(doc.createTextNode(QString::fromStdString(product.GetImagePath())));

    QDomElement publish_time = doc.createElement("publish_time");
    publish_time.appendChild(doc.createTextNode(QString::number(product.GetPublishTime())));

    QDomElement seller_id = doc.createElement("seller_id");
    seller_id.appendChild(doc.createTextNode(QString::fromStdString(product.GetSellerId())));

    product_elem.appendChild(product_id);
    product_elem.appendChild(name);
    product_elem.appendChild(description);
    product_elem.appendChild(price);
    product_elem.appendChild(quantity);
    product_elem.appendChild(category);
    product_elem.appendChild(image_path);
    product_elem.appendChild(publish_time);
    product_elem.appendChild(seller_id);

    root.appendChild(product_elem);

    // save to file
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        doc.save(stream, 2);
        file.close();
        std::cout << "Product saved successfully: " << product.GetProductName() << std::endl;
        return true;
    }

    std::cout << "Failed to save product: " << product.GetProductName() << std::endl;
    return false;
}

std::vector<Product*> XMLDataManager::GetProducts() {
    std::vector<Product*> products;
    QFile file(QString::fromStdString(products_file_));

    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << "Cannot open products file: " << products_file_ << std::endl;
        return products;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        std::cout << "Failed to parse products XML" << std::endl;
        return products;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList product_nodes = root.elementsByTagName("product");

    for (int i = 0; i < product_nodes.count(); ++i) {
        QDomElement product_elem = product_nodes.at(i).toElement();

        std::string product_id = product_elem.elementsByTagName("product_id").at(0).toElement().text().toStdString();
        std::string name = product_elem.elementsByTagName("name").at(0).toElement().text().toStdString();
        std::string description = product_elem.elementsByTagName("description").at(0).toElement().text().toStdString();
        double price = product_elem.elementsByTagName("price").at(0).toElement().text().toDouble();
        int quantity = product_elem.elementsByTagName("quantity").at(0).toElement().text().toInt();  // 读取数量
        std::string category = product_elem.elementsByTagName("category").at(0).toElement().text().toStdString();
        std::string image_path = product_elem.elementsByTagName("image_path").at(0).toElement().text().toStdString();
        std::time_t publish_time = product_elem.elementsByTagName("publish_time").at(0).toElement().text().toLong();
        std::string seller_id = product_elem.elementsByTagName("seller_id").at(0).toElement().text().toStdString();

        products.push_back(new Product(product_id, name, description, price,
                                       category, image_path, publish_time, seller_id, quantity));
    }

    std::cout << "Loaded " << products.size() << " products from file" << std::endl;
    return products;
}

std::vector<Product*> XMLDataManager::SearchProducts(const std::string& keyword,
                                                      const std::string& category) {
    auto all_products = GetProducts();
    std::vector<Product*> result;

    std::string keyword_lower = keyword;
    std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);

    for (auto* product : all_products) {
        bool match = true;

        // categorys
        if (!category.empty() && product->GetCategory() != category) {
            match = false;
        }

        // key words
        if (match && !keyword.empty()) {
            std::string name_lower = product->GetProductName();
            std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);

            std::string desc_lower = product->GetDescription();
            std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);

            if (name_lower.find(keyword_lower) == std::string::npos &&
                desc_lower.find(keyword_lower) == std::string::npos) {
                match = false;
            }
        }

        if (match) {
            result.push_back(product);
        } else {
            delete product;
        }
    }

    std::cout << "Search found " << result.size() << " products" << std::endl;
    return result;
}

bool XMLDataManager::DeleteProduct(const std::string& product_id) {
    QFile file(QString::fromStdString(products_file_));
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList products = root.elementsByTagName("product");
    bool found = false;

    for (int i = 0; i < products.count(); ++i) {
        QDomElement product_elem = products.at(i).toElement();
        QString stored_id = product_elem.elementsByTagName("product_id").at(0).toElement().text();

        if (stored_id == QString::fromStdString(product_id)) {
            root.removeChild(products.at(i));
            found = true;
            break;
        }
    }

    if (found && file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        doc.save(stream, 2);
        file.close();
        std::cout << "Product deleted successfully: " << product_id << std::endl;
        return true;
    }

    std::cout << "Product not found for deletion: " << product_id << std::endl;
    return false;
}

std::string XMLDataManager::GenerateUserId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return "user_" + std::to_string(dis(gen));
}

std::string XMLDataManager::GenerateProductId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return "prod_" + std::to_string(dis(gen));
}
