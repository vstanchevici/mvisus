#include "MLTest.h"

#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include <iostream>
//#include <armadillo>

using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // ManhattanDistance

namespace mv
{
    
    MLTest::MLTest()
	{
	}

    MLTest::~MLTest()
	{
	}

	void MLTest::Run()
	{
        // Load the data from data.csv (hard-coded).  Use CLI for simple command-line
        // parameter handling.
        arma::mat data("0.339406815,0.843176636,0.472701471; \
                  0.212587646,0.351174901,0.81056695;  \
                  0.160147626,0.255047893,0.04072469;  \
                  0.564535197,0.943435462,0.597070812");
        data = data.t();

        // Use templates to specify that we want a NeighborSearch object which uses
        // the Manhattan distance.
        NeighborSearch<NearestNeighborSort, ManhattanDistance> nn(data);

        // Create the object we will store the nearest neighbors in.
        arma::Mat<size_t> neighbors;
        arma::mat distances; // We need to store the distance too.

        // Compute the neighbors.
        nn.Search(1, neighbors, distances);

        
        // Write each neighbor and distance using Log.
        for (size_t i = 0; i < neighbors.n_elem; ++i)
        {
            std::cout << "Nearest neighbor of point " << i << " is point "
                << neighbors[i] << " and the distance is " << distances[i] << "." << std::endl;
        }

        //arma::arma_rng::set_seed_random();
        //
        //// Create a 4x4 random matrix and print it on the screen
        //arma::Mat<double> A = arma::randu(4,4);
        //std::cout << "A:\n" << A << "\n";
        //
        //// Multiply A with his transpose:
        //std::cout << "A * A.t() =\n";
        //std::cout << A * A.t() << "\n";
        //
        //// Access/Modify rows and columns from the array:
        //A.row(0) = A.row(1) + A.row(3);
        //A.col(3).zeros();
        //std::cout << "add rows 1 and 3, store result in row 0, also fill 4th column with zeros:\n";
        //std::cout << "A:\n" << A << "\n";
        //
        //// Create a new diagonal matrix using the main diagonal of A:
        //arma::Mat<double>B = arma::diagmat(A);
        //std::cout << "B:\n" << B << "\n";
	}
}
