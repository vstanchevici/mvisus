/*
 * MVISUSDatabase.cpp
 *
 * This file is part of the "mvisus" project.
 * See "LICENSE.md" for license information.
 */

#include "MVISUSDatabase.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include <iostream>
#include <iomanip>

#include <streambuf>

//https://www.boost.org/doc/libs/1_78_0/libs/serialization/doc/serialization.html
//https://stackoverflow.com/questions/41801826/do-cereal-and-boost-serialization-use-zero-copy

class PlayerInventory
{
    private:
        friend class boost::serialization::access;
        template <class Archive> void serialize(Archive& ar, unsigned)
        {
            ar& itemID& itemCount;
        }

    public:
        int itemID;
        int itemCount;
};

class player
{
    private:
        friend class boost::serialization::access;
        template <class Archive> void serialize(Archive& ar, unsigned)
        {
            ar& username& password& inv;
        }        
    public:
        std::string                  username;
        std::string                  password;
        std::vector<PlayerInventory> inv;
};

BOOST_CLASS_VERSION(player, 1)



/*
size_t length = 100;
auto pBuf = new char[length]; // allocate memory

struct membuf : std::streambuf // derive because std::streambuf constructor is protected
{
    membuf(char* p, size_t size)
    {
        setp(p, p + size); // set start end end pointers
    }
    size_t written() { return pptr() - pbase(); } // how many bytes were really written?
};

membuf sbuf(pBuf, length); // our buffer object
std::ostream out(&sbuf);   // stream using our buffer

out << 12345.654e10 << std::endl;
out.flush();

std::cout << "Nr of written bytes: " << sbuf.written() << std::endl;
std::cout << "Content: " << (char*)pBuf << std::endl;

delete[] pBuf; // free memory 
*/


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
      
    bool MVISUSDatabase::Connect()
    {
        std::stringstream ss;

        {
            boost::archive::text_oarchive oa(ss, boost::archive::no_header);
            
            player to_save;
            to_save.username = "user_name";
            to_save.password = "password";
            to_save.inv = {
                    { 1, 17 },
                    { 2, 11 },
                    {83, 8800 },
                    { 4, 0 },
                    { 5, 1 },
                    { 6, 1 },
                    { 7, 1 },
                    { 8, 1 },
                    { 9, 42 },
            };

            oa << to_save;
        } // <-- destructor of text_oarchive

        std::cout << "Serialized stream: " << std::quoted(ss.str()) << std::endl;

        player loaded;
        {
            boost::archive::text_iarchive ia(ss, boost::archive::no_header);
            ia >> loaded;
        }

        std::cout << "Roundtrip username:" << std::quoted(loaded.username)
            << " password:" << std::quoted(loaded.password) << std::endl;

        for (auto [id, count] : loaded.inv) {
            std::cout << " - item " << id << " count:" << count << std::endl;
        }

        std::cin.get();
        return true;
    }


} // /namespace mvisus