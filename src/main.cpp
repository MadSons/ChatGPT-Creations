#include "engine/Application.hpp"
#include "engine/Config.hpp"

int main() {
    Config cfg;
    Application app;
    if (!app.init(cfg)) return 1;
    app.run();
    app.shutdown();
    return 0;
}
