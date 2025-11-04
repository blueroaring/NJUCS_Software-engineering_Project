#include "../../includes/ui/product_form.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>  // 添加这个头文件
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

ProductForm::ProductForm(QWidget* parent) : QDialog(parent) {
    SetupUI();
}

void ProductForm::SetupUI() {
    setWindowTitle("Product Form");
    setModal(true);
    setMinimumSize(400, 350);

    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Form layout
    QFormLayout* form_layout = new QFormLayout();

    name_edit_ = new QLineEdit(this);
    name_edit_->setPlaceholderText("Enter product name");
    form_layout->addRow("Product Name:", name_edit_);

    description_edit_ = new QTextEdit(this);
    description_edit_->setPlaceholderText("Enter product description");
    description_edit_->setMaximumHeight(100);
    form_layout->addRow("Description:", description_edit_);

    price_spin_ = new QDoubleSpinBox(this);
    price_spin_->setRange(0.0, 100000.0);
    price_spin_->setDecimals(2);
    price_spin_->setPrefix("$");
    form_layout->addRow("Price:", price_spin_);

    quantity_spin_ = new QSpinBox(this);
    quantity_spin_->setRange(1, 10000);
    quantity_spin_->setValue(1);
    form_layout->addRow("Quantity:", quantity_spin_);

    category_combo_ = new QComboBox(this);
    category_combo_->addItem("Electronics", "electronics");
    category_combo_->addItem("Books", "books");
    category_combo_->addItem("Clothing", "clothing");
    category_combo_->addItem("Home & Garden", "home");
    category_combo_->addItem("Sports", "sports");
    category_combo_->addItem("Other", "other");
    form_layout->addRow("Category:", category_combo_);

    image_path_edit_ = new QLineEdit(this);
    image_path_edit_->setPlaceholderText("Optional image path or URL");
    form_layout->addRow("Image Path:", image_path_edit_);

    main_layout->addLayout(form_layout);

    // Buttons
    QHBoxLayout* button_layout = new QHBoxLayout();

    save_button_ = new QPushButton("Save", this);
    cancel_button_ = new QPushButton("Cancel", this);

    button_layout->addWidget(save_button_);
    button_layout->addWidget(cancel_button_);

    main_layout->addLayout(button_layout);

    // Connect signals
    connect(save_button_, &QPushButton::clicked, this, &ProductForm::HandleSave);
    connect(cancel_button_, &QPushButton::clicked, this, &ProductForm::HandleCancel);

    setLayout(main_layout);
}

std::map<std::string, std::string> ProductForm::GetProductData() const {
    std::map<std::string, std::string> data;
    data["name"] = name_edit_->text().toStdString();
    data["description"] = description_edit_->toPlainText().toStdString();
    data["price"] = QString::number(price_spin_->value()).toStdString();
    data["quantity"] = QString::number(quantity_spin_->value()).toStdString();
    data["category"] = category_combo_->currentData().toString().toStdString();
    data["image_path"] = image_path_edit_->text().toStdString();
    return data;
}

void ProductForm::SetProductData(const Product* product) {
    if (!product) return;

    name_edit_->setText(QString::fromStdString(product->GetProductName()));
    description_edit_->setText(QString::fromStdString(product->GetDescription()));
    price_spin_->setValue(product->GetPrice());
    quantity_spin_->setValue(product->GetQuantity());

    // Set category
    QString category = QString::fromStdString(product->GetCategory());
    int index = category_combo_->findData(category);
    if (index >= 0) {
        category_combo_->setCurrentIndex(index);
    }

    image_path_edit_->setText(QString::fromStdString(product->GetImagePath()));
}

void ProductForm::HandleSave() {
    if (ValidateInput()) {
        accept();
    }
}

void ProductForm::HandleCancel() {
    reject();
}

bool ProductForm::ValidateInput() {
    if (name_edit_->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Product name is required.");
        return false;
    }

    if (description_edit_->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Product description is required.");
        return false;
    }

    if (price_spin_->value() <= 0) {
        QMessageBox::warning(this, "Validation Error",
                             "Price must be greater than 0.");
        return false;
    }

    if (quantity_spin_->value() <= 0) {
        QMessageBox::warning(this, "Validation Error",
                             "Quantity must be greater than 0.");
        return false;
    }

    return true;
}
