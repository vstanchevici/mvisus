#include <mvisus-version.h>
#include <MLTest.h>
#include <iostream>

//https://www.boost.org/doc/libs/1_75_0/libs/serialization/doc/tutorial.html
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& degrees;
        ar& minutes;
        ar& seconds;
    }
    int degrees;
    int minutes;
    float seconds;
public:
    gps_position() {};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};

//#include <openblas/cblas.h>

/*
void test_open_blas()
{
    int i = 0;
    double A[6] = { 1.0,2.0,1.0,-3.0,4.0,-1.0 };
    double B[6] = { 1.0,2.0,1.0,-3.0,4.0,-1.0 };
    double C[9] = { .5,.5,.5,.5,.5,.5,.5,.5,.5 };
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, 3, 3, 2, 1, A, 3, B, 3, 2, C, 3);

    for (i = 0; i < 9; i++)
        std::cout << C[i] << " ";

    std::cout << std::endl;
}
*/

int main(int argc, char* argv[])
{
    std::cout << "Project Name: " << MVISUS_PROJECT_NAME << std::endl;
    std::cout << "Project Description: " << MVISUS_PROJECT_DESCRIPTION << std::endl;
    mv::MLTest app;
    app.Run();
    //test_open_blas();
    return 0;
}
