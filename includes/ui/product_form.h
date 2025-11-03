#ifndef PRODUCT_FORM_H
#define PRODUCT_FORM_H

#include <QDialog>
#include <map>
#include <string>
#include "../models/product.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTextEdit;
class QDoubleSpinBox;
class QComboBox;
class QPushButton;
class QLabel;
class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
QT_END_NAMESPACE

/**
 * @brief Provides a form for creating and editing products.
 */
class ProductForm : public QDialog {
    Q_OBJECT

public:
    explicit ProductForm(QWidget* parent = nullptr);

    std::map<std::string, std::string> GetProductData() const;
    void SetProductData(const Product* product);

private slots:
    void HandleSave();
    void HandleCancel();

private:
    void SetupUI();
    bool ValidateInput();

    QLineEdit* name_edit_;
    QTextEdit* description_edit_;
    QDoubleSpinBox* price_spin_;
    QSpinBox* quantity_spin_;
    QComboBox* category_combo_;
    QLineEdit* image_path_edit_;
    QPushButton* save_button_;
    QPushButton* cancel_button_;
};

#endif  // PRODUCT_FORM_H
