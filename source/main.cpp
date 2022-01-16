#include <mvisus-version.h>
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Project Name: " << MVISUS_PROJECT_NAME << std::endl;
    std::cout << "Project Description: " << MVISUS_PROJECT_DESCRIPTION << std::endl;
    return 0;
}
