#ifndef PRODUCT_LIST_WIDGET_H
#define PRODUCT_LIST_WIDGET_H

#include <QWidget>
#include <vector>
#include <memory>
#include "../models/user.h"
#include "../models/product.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;
class QListWidget;
class QListWidgetItem;
class ProductService;
class QTextEdit;
class MainWindow;
QT_END_NAMESPACE

/**
 * @brief Displays and manages the list of products in the marketplace.
 */
class ProductListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProductListWidget(MainWindow* main_window, QWidget* parent = nullptr);

    void SetCurrentUser(User* user);
    void LoadProducts();

signals:
    void LogoutRequested();

private slots:
    void HandleSearch();
    void HandleAddProduct();
    void HandleEditProduct();
    void HandleDeleteProduct();
    void HandleLogout();
    void HandleProductSelectionChanged();
    void HandleCategoryFilterChanged(int index);
    void HandleContactSeller();

private:
    void SetupUI();
    void RefreshProductList();
    void DisplayProducts(const std::vector<Product*>& products);
    void ClearProductList();
    Product* GetSelectedProduct();
    void ShowSellerContactInfo(const Product* product);

    MainWindow* main_window_;
    User* current_user_;

    QVBoxLayout* main_layout_;
    QHBoxLayout* header_layout_;
    QHBoxLayout* search_layout_;
    QHBoxLayout* button_layout_;

    QLabel* welcome_label_;
    QPushButton* logout_button_;
    QPushButton* add_product_button_;
    QPushButton* edit_product_button_;
    QPushButton* delete_product_button_;
    QPushButton* contact_seller_button_;

    QLineEdit* search_edit_;
    QComboBox* category_combo_;
    QPushButton* search_button_;

    QListWidget* product_list_;
    QTextEdit* product_details_;

    std::vector<Product*> current_products_;
};

#endif  // PRODUCT_LIST_WIDGET_H
