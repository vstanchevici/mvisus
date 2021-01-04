#include <mvisus-version.h>
#include <Core/App.h>
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Project Name: " << MVISUS_PROJECT_DESCRIPTION;
    mv::App app;
    app.Run();
    return 0;
}
