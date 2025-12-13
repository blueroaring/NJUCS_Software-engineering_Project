#define RUN_TESTS
#ifdef RUN_TESTS
#include <QtTest/QTest>
#include "includes/tests/test_encryptionutil.h"
#include "includes/tests/test_authservice.h"
#include "includes/tests/test_intergration.h"
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    TestEncryptionUtil testEncryption;
    TestAuthService testAuth;
    TestIntegration testIntergration;

    int status = 0;
    status |= QTest::qExec(&testEncryption, argc, argv);
    status |= QTest::qExec(&testAuth, argc, argv);
    status |= QTest::qExec(&testIntergration,argc,argv);

    return status;
}
#else
#include <QApplication>
#include <QDir>
#include "includes/ui/main_window.h"

/**
 * @brief Main entry point for the Online Shop application.
 *
 * Initializes the Qt application and creates the main window.
 * Ensures necessary resource directories exist.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create resources directory if it doesn't exist
    QDir().mkpath("resources");

    // Set application properties
    app.setApplicationName("Online Shop");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ShopCorp");

    // Create and show main window
    MainWindow main_window;
    main_window.show();

    return app.exec();
}
#endif

