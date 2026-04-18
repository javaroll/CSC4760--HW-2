#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        int n; 
        int m; 
        
        std::cout << "Rows: " << std::endl;
        std::cin >> n;
        std::cout << "Columns: " << std::endl;
        std::cin >> m;

        // nxm view
        Kokkos::View<long **> v("my_view", n, m);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                v(i, j) = 1000 * i * j;
            }
        }

        // Prints out view
        std::cout << "View contents (" << n << "x" << m << "):" << std::endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                std::cout << v(i, j) << "\t";
            }
            std::cout << std::endl;
        }
    }
    Kokkos::finalize();
    return 0;
}
