#include "Application.h"

int main(int argc, char** argv)
{
    mvisus::Application::Config config;
    if (config.Load(argc, argv))
    {
        mvisus::Application app;
        app.Run(config);
    }
    return 0;
}
