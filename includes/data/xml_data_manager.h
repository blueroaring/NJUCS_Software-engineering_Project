#ifndef XML_DATA_MANAGER_H
#define XML_DATA_MANAGER_H

#include <string>
#include <vector>
#include "../models/user.h"
#include "../models/product.h"

/**
 * @brief Manages data persistence using XML files.
 *
 * The XMLDataManager class handles all CRUD operations for users and products
 * using XML files as the storage backend.
 */
class XMLDataManager {
public:
    XMLDataManager(const std::string& users_file, const std::string& products_file);

    /**
   * @brief Saves a user to the XML file.
   * @param user The user object to save.
   * @return true if save is successful, false otherwise.
   */
    bool SaveUser(const User& user);

    /**
   * @brief Retrieves a user by username.
   * @param username The username to search for.
   * @return Pointer to User object if found, nullptr otherwise.
   */
    User* GetUser(const std::string& username);

    /**
   * @brief Saves a product to the XML file.
   * @param product The product object to save.
   * @return true if save is successful, false otherwise.
   */
    bool SaveProduct(const Product& product);

    /**
   * @brief Retrieves all products from the XML file.
   * @return Vector of pointers to Product objects.
   */
    std::vector<Product*> GetProducts();

    /**
   * @brief Searches products by keyword and category.
   * @param keyword The search keyword.
   * @param category The product category filter.
   * @return Vector of pointers to matching Product objects.
   */
    std::vector<Product*> SearchProducts(const std::string& keyword,
                                          const std::string& category);

    /**
   * @brief Deletes a product by ID.
   * @param product_id The ID of the product to delete.
   * @return true if deletion is successful, false otherwise.
   */
    bool DeleteProduct(const std::string& product_id);

private:
    std::string users_file_;
    std::string products_file_;

    /**
   * @brief Initializes demo data if XML files don't exist.
   */
    void InitializeDemoData();

    /**
   * @brief Generates a unique ID for new users.
   * @return A unique user ID string.
   */
    std::string GenerateUserId();

    /**
   * @brief Generates a unique ID for new products.
   * @return A unique product ID string.
   */
    std::string GenerateProductId();
};

#endif  // XML_DATA_MANAGER_H
