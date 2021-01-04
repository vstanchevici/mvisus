#include <mvisus-version.h>
#include <Core/App.h>
#include <iostream>

int main()
{
    std::cout << "Project Name:" << MVISUS_PROJECT_NAME;
    App app;
    app.Run();
    return 0;
}
