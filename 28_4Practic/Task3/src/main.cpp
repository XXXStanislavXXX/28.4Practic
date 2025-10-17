#include <ctime>
#include <cstdlib>
#include "Restaurant.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Restaurant app;
    app.run();
    return 0;
}
