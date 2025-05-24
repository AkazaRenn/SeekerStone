#include "GamepadAdapter.hpp"
#include "IdleManager.hpp"

class Main {
    public:
        explicit Main() = default;
        ~Main()         = default;
        int run(int argc, char* argv[]);

    private:
        GamepadAdapter gamepadAdapter;
        IdleManager    idleManager;
};
