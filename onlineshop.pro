QT = core
QT += widgets
QT += xml
QT += testlib
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/data/xml_data_manager.cpp \
        src/models/product.cpp \
        src/models/user.cpp \
        src/services/auth_service.cpp \
        src/services/encryption_util.cpp \
        src/services/product_service.cpp \
        src/ui/login_window.cpp \
        src/ui/main_window.cpp \
        src/ui/product_form.cpp \
        src/ui/product_list_widget.cpp \
        src/ui/register_window.cpp \
        src/tests/test_encryptionutil.cpp \
        src/tests/test_authservice.cpp \
        src/tests/test_intergration.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/products.xml \
    resources/users.xml

HEADERS += \
    includes/data/xml_data_manager.h \
    includes/models/product.h \
    includes/models/user.h \
    includes/services/auth_service.h \
    includes/services/encryption_util.h \
    includes/services/product_service.h \
    includes/ui/login_window.h \
    includes/ui/main_window.h \
    includes/ui/product_form.h \
    includes/ui/product_list_widget.h \
    includes/ui/register_window.h \
    includes/tests/test_encryptionutil.h \
    includes/tests/test_authservice.h \
    includes/tests/test_intergration.h

tests {
        DEFINES += RUN_TESTS
    }

