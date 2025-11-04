#include "../../includes/ui/product_list_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QTextEdit>
#include "../../includes/services/product_service.h"
#include "../../includes/ui/product_form.h"
#include "../../includes/ui/main_window.h"

ProductListWidget::ProductListWidget(MainWindow* main_window, QWidget* parent)
    : QWidget(parent), main_window_(main_window), current_user_(nullptr) {
    SetupUI();
}

void ProductListWidget::SetCurrentUser(User* user) {
    current_user_ = user;
    RefreshProductList();
}

void ProductListWidget::LoadProducts() {
    RefreshProductList();
}

void ProductListWidget::SetupUI() {
    main_layout_ = new QVBoxLayout(this);

    // Header with welcome message and logout button
    header_layout_ = new QHBoxLayout();
    welcome_label_ = new QLabel("Welcome to Online Shop", this);
    QFont welcome_font = welcome_label_->font();
    welcome_font.setPointSize(14);
    welcome_font.setBold(true);
    welcome_label_->setFont(welcome_font);

    logout_button_ = new QPushButton("Logout", this);

    header_layout_->addWidget(welcome_label_);
    header_layout_->addStretch();
    header_layout_->addWidget(logout_button_);
    main_layout_->addLayout(header_layout_);

    // Search and filter section
    search_layout_ = new QHBoxLayout();
    search_edit_ = new QLineEdit(this);
    search_edit_->setPlaceholderText("Search products...");

    category_combo_ = new QComboBox(this);
    category_combo_->addItem("All Categories", "");
    category_combo_->addItem("Electronics", "electronics");
    category_combo_->addItem("Books", "books");
    category_combo_->addItem("Clothing", "clothing");
    category_combo_->addItem("Home & Garden", "home");
    category_combo_->addItem("Sports", "sports");
    category_combo_->addItem("Other", "other");

    search_button_ = new QPushButton("Search", this);

    search_layout_->addWidget(search_edit_);
    search_layout_->addWidget(category_combo_);
    search_layout_->addWidget(search_button_);
    main_layout_->addLayout(search_layout_);

    // Action buttons
    button_layout_ = new QHBoxLayout();
    add_product_button_ = new QPushButton("Add Product", this);
    edit_product_button_ = new QPushButton("Edit Product", this);
    delete_product_button_ = new QPushButton("Delete Product", this);
    contact_seller_button_ = new QPushButton("Contact Seller", this);

    edit_product_button_->setEnabled(false);
    delete_product_button_->setEnabled(false);
    contact_seller_button_->setEnabled(false);

    button_layout_->addWidget(add_product_button_);
    button_layout_->addWidget(edit_product_button_);
    button_layout_->addWidget(delete_product_button_);
    button_layout_->addWidget(contact_seller_button_);
    button_layout_->addStretch();
    main_layout_->addLayout(button_layout_);

    // Product list and details
    QHBoxLayout* content_layout = new QHBoxLayout();

    // Product list (左侧)
    product_list_ = new QListWidget(this);
    product_list_->setMinimumWidth(300);
    content_layout->addWidget(product_list_, 1);

    // Product details (右侧)
    product_details_ = new QTextEdit(this);
    product_details_->setReadOnly(true);
    product_details_->setPlaceholderText("Select a product to view details and seller contact information");
    product_details_->setMinimumWidth(300);
    content_layout->addWidget(product_details_, 1);

    main_layout_->addLayout(content_layout, 1);

    // Connect signals
    connect(logout_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleLogout);
    connect(add_product_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleAddProduct);
    connect(edit_product_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleEditProduct);
    connect(delete_product_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleDeleteProduct);
    connect(contact_seller_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleContactSeller);
    connect(search_button_, &QPushButton::clicked,
            this, &ProductListWidget::HandleSearch);
    connect(product_list_, &QListWidget::itemSelectionChanged,
            this, &ProductListWidget::HandleProductSelectionChanged);
    connect(category_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProductListWidget::HandleCategoryFilterChanged);

    setLayout(main_layout_);
}

void ProductListWidget::RefreshProductList() {
    if (!main_window_ || !main_window_->GetProductService() || !current_user_) {
        return;
    }

    QString search_text = search_edit_->text();
    QString category = category_combo_->currentData().toString();

    current_products_ = main_window_->GetProductService()->SearchProducts(
        search_text.toStdString(), category.toStdString());

    DisplayProducts(current_products_);

    // Update welcome message
    welcome_label_->setText(QString("Welcome, %1!").arg(
        QString::fromStdString(current_user_->GetUsername())));
}


void ProductListWidget::DisplayProducts(const std::vector<Product*>& products) {
    product_list_->clear();

    for (const auto* product : products) {

        QString item_text = QString("%1 - $%2 - Qty: %3")
                                .arg(QString::fromStdString(product->GetProductName()))
                                .arg(product->GetPrice(), 0, 'f', 2)
                                .arg(product->GetQuantity());

        QListWidgetItem* item = new QListWidgetItem(item_text, product_list_);
        item->setData(Qt::UserRole, QString::fromStdString(product->GetProductId()));


        QString tooltip = QString("Description: %1\nCategory: %2\nPublished: %3")
                              .arg(QString::fromStdString(product->GetDescription()))
                              .arg(QString::fromStdString(product->GetCategory()))
                              .arg(QDateTime::fromSecsSinceEpoch(product->GetPublishTime())
                                       .toString("yyyy-MM-dd hh:mm"));
        item->setToolTip(tooltip);
    }
}

void ProductListWidget::ClearProductList() {
    for (auto* product : current_products_) {
        delete product;
    }
    current_products_.clear();
    product_list_->clear();
}

Product* ProductListWidget::GetSelectedProduct() {
    QList<QListWidgetItem*> selected_items = product_list_->selectedItems();
    if (selected_items.isEmpty()) {
        return nullptr;
    }

    QString product_id = selected_items.first()->data(Qt::UserRole).toString();

    for (auto* product : current_products_) {
        if (product->GetProductId() == product_id.toStdString()) {
            return product;
        }
    }

    return nullptr;
}

void ProductListWidget::HandleSearch() {
    RefreshProductList();
}

void ProductListWidget::HandleAddProduct() {
    if (!current_user_ || !main_window_ || !main_window_->GetProductService()) {
        return;
    }

    ProductForm form(this);
    form.setWindowTitle("Add New Product");

    if (form.exec() == QDialog::Accepted) {
        std::map<std::string, std::string> product_data = form.GetProductData();

        if (main_window_->GetProductService()->PublishProduct(product_data, current_user_->GetUserId())) {
            QMessageBox::information(this, "Success", "Product published successfully!");
            RefreshProductList();
        } else {
            QMessageBox::warning(this, "Error", "Failed to publish product.");
        }
    }
}

void ProductListWidget::HandleEditProduct() {
    Product* selected_product = GetSelectedProduct();
    if (!selected_product || !current_user_ || !main_window_ || !main_window_->GetProductService()) {
        return;
    }

    // 权限检查：只有管理员或产品所有者可以编辑产品
    if (selected_product->GetSellerId() != current_user_->GetUserId() &&
        !current_user_->IsAdmin()) {
        QMessageBox::warning(this, "Permission Denied",
                             "You can only edit your own products.");
        return;
    }

    ProductForm form(this);
    form.setWindowTitle("Edit Product");
    form.SetProductData(selected_product);

    if (form.exec() == QDialog::Accepted) {
        std::map<std::string, std::string> updates = form.GetProductData();

        // 传递当前用户ID和权限信息
        if (main_window_->GetProductService()->UpdateProduct(selected_product->GetProductId(), updates,
                                                             current_user_->GetUserId(),
                                                             current_user_->IsAdmin())) {
            QMessageBox::information(this, "Success", "Product updated successfully!");
            RefreshProductList();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to update product. You may not have permission.");
        }
    }
}

void ProductListWidget::HandleDeleteProduct() {
    Product* selected_product = GetSelectedProduct();
    if (!selected_product || !current_user_ || !main_window_ || !main_window_->GetProductService()) {
        return;
    }

    // 权限检查：只有管理员或产品所有者可以删除产品
    if (selected_product->GetSellerId() != current_user_->GetUserId() &&
        !current_user_->IsAdmin()) {
        QMessageBox::warning(this, "Permission Denied",
                             "You can only delete your own products.");
        return;
    }

    // Confirm deletion
    int result = QMessageBox::question(
        this, "Confirm Deletion",
        QString("Are you sure you want to delete '%1'?")
            .arg(QString::fromStdString(selected_product->GetProductName())),
        QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        // 传递当前用户ID和权限信息
        if (main_window_->GetProductService()->DeleteProduct(selected_product->GetProductId(),
                                                             current_user_->GetUserId(),
                                                             current_user_->IsAdmin())) {
            QMessageBox::information(this, "Success", "Product deleted successfully!");
            RefreshProductList();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to delete product. You may not have permission.");
        }
    }
}

void ProductListWidget::HandleProductSelectionChanged() {
    bool has_selection = !product_list_->selectedItems().isEmpty();

    if (has_selection && current_user_) {
        Product* selected_product = GetSelectedProduct();
        if (selected_product) {
            // 只有管理员或产品所有者可以编辑和删除
            bool can_edit_delete = (current_user_->IsAdmin() ||
                                    selected_product->GetSellerId() == current_user_->GetUserId());
            edit_product_button_->setEnabled(can_edit_delete);
            delete_product_button_->setEnabled(can_edit_delete);

            // 任何人都可以联系卖家，但不能联系自己
            bool can_contact = (selected_product->GetSellerId() != current_user_->GetUserId());
            contact_seller_button_->setEnabled(can_contact);

            // 显示产品详情和卖家信息
            ShowSellerContactInfo(selected_product);

            // 如果是管理员操作他人产品，显示提示
            if (current_user_->IsAdmin() && selected_product->GetSellerId() != current_user_->GetUserId()) {
                edit_product_button_->setToolTip("Admin: Editing another user's product");
                delete_product_button_->setToolTip("Admin: Deleting another user's product");
            } else {
                edit_product_button_->setToolTip("");
                delete_product_button_->setToolTip("");
            }
        }
    } else {
        edit_product_button_->setEnabled(false);
        delete_product_button_->setEnabled(false);
        contact_seller_button_->setEnabled(false);
        edit_product_button_->setToolTip("");
        delete_product_button_->setToolTip("");
        product_details_->clear();
    }
}

void ProductListWidget::HandleLogout() {
    ClearProductList();
    emit LogoutRequested();
}


void ProductListWidget::HandleCategoryFilterChanged(int index) {
    RefreshProductList();
}

void ProductListWidget::HandleContactSeller() {
    Product* selected_product = GetSelectedProduct();
    if (!selected_product || !current_user_ || !main_window_ || !main_window_->GetDataManager()) {
        return;
    }

    // 获取卖家信息
    User* seller = main_window_->GetDataManager()->GetUser(selected_product->GetSellerId());

    if (seller) {
        QString contact_info = QString(
                                   "<h3>Contact Seller: %1</h3>"
                                   "<p><b>Product:</b> %2</p>"
                                   "<p><b>Seller Name:</b> %3</p>"
                                   "<p><b>Email:</b> %4</p>"
                                   "<p><b>Phone:</b> %5</p>"
                                   "<hr>"
                                   "<p>You can contact the seller directly using the information above "
                                   "to discuss the product, negotiate price, or arrange a meeting.</p>")
                                   .arg(QString::fromStdString(seller->GetUsername()))
                                   .arg(QString::fromStdString(selected_product->GetProductName()))
                                   .arg(QString::fromStdString(seller->GetUsername()))
                                   .arg(QString::fromStdString(seller->GetEmail()))
                                   .arg(QString::fromStdString(seller->GetPhone()));

        QMessageBox::information(this, "Seller Contact Information", contact_info);

        delete seller;  // 释放内存
    } else {
        QMessageBox::warning(this, "Error", "Could not retrieve seller information.");
    }
}

void ProductListWidget::ShowSellerContactInfo(const Product* product) {
    if (!product || !main_window_ || !main_window_->GetDataManager()) return;

    // 创建产品详情文本
    QString details = QString("<h3>%1</h3>")
                          .arg(QString::fromStdString(product->GetProductName()));

    details += QString("<p><b>Description:</b> %1</p>")
                   .arg(QString::fromStdString(product->GetDescription()));

    details += QString("<p><b>Price:</b> $%1</p>")
                   .arg(product->GetPrice(), 0, 'f', 2);

    details += QString("<p><b>Quantity Available:</b> %1</p>")
                   .arg(product->GetQuantity());

    details += QString("<p><b>Category:</b> %1</p>")
                   .arg(QString::fromStdString(product->GetCategory()));

    details += QString("<p><b>Published:</b> %1</p>")
                   .arg(QDateTime::fromSecsSinceEpoch(product->GetPublishTime())
                            .toString("yyyy-MM-dd hh:mm"));

    // 获取卖家信息
    User* seller = main_window_->GetDataManager()->GetUser(product->GetSellerId());

    if (seller) {
        details += QString("<hr><h4>Seller Information</h4>");
        details += QString("<p><b>Seller:</b> %1</p>")
                       .arg(QString::fromStdString(seller->GetUsername()));
        details += QString("<p><b>Email:</b> %1</p>")
                       .arg(QString::fromStdString(seller->GetEmail()));
        details += QString("<p><b>Phone:</b> %1</p>")
                       .arg(QString::fromStdString(seller->GetPhone()));

        delete seller;  // 释放内存
    }

    product_details_->setHtml(details);
}
