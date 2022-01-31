/*
 * Application.cpp
 *
 * This file is part of the "mvisus" project.
 * See "LICENSE.md" for license information.
 */

#include "Application.h"
#include <mvisus-version.h>
#include <iostream>
#include <cstdlib>

namespace mvisus
{
    bool Application::Config::Load(int argc, char** argv)
    {
        // read json
        mConfig = "{ \"happy\": true, \"pi\": 3.141 }"_json;

        //read env variabiles
        if (const char* env_p = std::getenv("PATH"))
            std::cout << "Your PATH is: " << env_p << '\n';

        //read command line varabiles

        return true;
    }

   /*
    * ======= Public: =======
    */

    void Application::Run(Config& config)
    {
        std::cout << "Project Name: " << MVISUS_PROJECT_NAME << std::endl;
        std::cout << "Project Description: " << MVISUS_PROJECT_DESCRIPTION << std::endl;
    }


} // /namespace mvisus