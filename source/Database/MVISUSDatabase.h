/*
 * MVISUSDatabase.h
 *
 * This file is part of the "mvisus" project.
 * See "LICENSE.md" for license information.
 */

#ifndef MVISUS_DATABASE_H
#define MVISUS_DATABASE_H

#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include <argparse/argparse.hpp>

namespace mvisus
{


    /**
    \brief MVISUSDatabase interface.
    \remarks Here is an example to print the attributes of all displays:
    \code
    auto myDisplayList = arcl::Display::InstantiateList();
    for (const auto& myDisplay : myDisplayList) {
        auto myDisplayOffset = myDisplay->GetOffset();
        auto myDisplayMode   = myDisplay->GetDisplayMode();
        std::wcout << L"Display: \""  << myDisplay->GetDeviceName() << L"\"" << std::endl;
        std::cout << "|-Primary = " << std::boolalpha << myDisplay->IsPrimary() << std::endl;
        std::cout << "|-X       = " << myDisplayOffset.x << std::endl;
        std::cout << "|-Y       = " << myDisplayOffset.y << std::endl;
        std::cout << "|-Width   = " << myDisplayMode.resolution.width << std::endl;
        std::cout << "|-Height  = " << myDisplayMode.resolution.height << std::endl;
        std::cout << "`-Hz      = " << myDisplayMode.refreshRate << std::endl;
    }
    \endcode
    */
    class MVISUSDatabase
    {
        public:

        public:
            bool Connect();

    };


} // /namespace mvisus


#endif