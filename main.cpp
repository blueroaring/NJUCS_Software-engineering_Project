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
