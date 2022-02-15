/*
 * Application.cpp
 *
 * This file is part of the "mvisus" project.
 * See "LICENSE.md" for license information.
 */

#include "Application.h"
#include <mvisus-version.h>
#include <build_defs.h>
#include <iostream>
#include <fstream>
#include <cstdlib>


namespace mvisus
{
   /*
    * ======= Public: =======
    */

    /**
    \brief qq
    \remarks aa
    \code ss
    */
      
    Application::Config::Config():mProgram("mvisus")
    {
        mConfigVariables = {
                    {"render_system",{  {"env", "MVISUS_RENDER_SYSTEM"},
                                        {"cnf", "render_system"},
                                        {"cmd", {"--render_system", "-r"}}}},
                    {"network_data",{   {"env", "MVISUS_NETWORK_DATA"},
                                        {"cnf", "network_data"},
                                        {"cmd", {"--network_data", "-n"}}}}
        };

        for (nlohmann::json::iterator it = mConfigVariables.begin(); it != mConfigVariables.end(); ++it)
        {
            for (auto& var : (*it)["cmd"])
            {
                mProgram.add_argument(var.get<std::string>());
            }

        }

    }

    bool Application::Config::Load(int argc, char** argv)
    {
        // read json
        std::ifstream config_file("config.json");

        if (config_file.is_open())
        {
            nlohmann::json config_json;
            config_file >> config_json;
            mConfigValues.merge_patch(config_json);
        }

        try {
            mProgram.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << mProgram;
        }

        for (nlohmann::json::iterator it = mConfigVariables.begin(); it != mConfigVariables.end(); ++it)
        {
            if (const char* env_var = std::getenv((*it)["env"].get<std::string>().c_str()))
                mConfigValues[it.key()] = std::string(env_var);

            for (nlohmann::json::iterator it = mConfigVariables.begin(); it != mConfigVariables.end(); ++it)
            {
                for (auto& var : (*it)["cmd"])
                {
                    std::string& var_name = var.get<std::string>();
                    if (mProgram.is_used(var_name))
                    {
                        mConfigValues[it.key()] = mProgram.get<std::string>(var_name);
                    }
                }

            }

        }

        if (mConfigValues.is_object())
            std::cout << mConfigValues.dump() << std::endl;

        std::cin.get();
        return true;
    }

    void Application::Run(Config& config)
    {
        std::cout << "Project Description   : " << MVISUS_PROJECT_DESCRIPTION << std::endl;
        std::cout << "Version               : " << software_version << "\n" << std::endl;
        MVISUSDatabase a;
        a.Connect();
    }


} // /namespace mvisus