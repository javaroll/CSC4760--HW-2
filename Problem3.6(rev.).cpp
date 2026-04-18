#include <Kokkos_Core.hpp>
#include <iostream>
#include <vector>

int main (int argc, char** argv){
    Kokkos::initialize(argc, argv);{
    int n;
    int m;
        
    std::cout << "enter a value for n: " << std::endl;
    std::cin >> n;
    std::cout << "enter a value for m: " << std::endl;
    std::cin >> m;
    
    Kokkos::View<double**> data("data", m, n);
    Kokkos::View<double*> sums_serial("sums_serial", m);
    Kokkos::View<double*> sums_parallel("sums_parallel", m);
    
    Kokkos::parallel_for("init", m, KOKKOS_LAMBDA(const int i){
    for(int j = 0; j < n; j++){
        data(i, j) += 1.0;
    }
    });
    Kokkos::Timer timer;
    
    timer.reset();
    for(int i = 0; i < m; i++){
        double row_sum = 0;
        for (int j = 0; j < n; j++){
            row_sum += cos(exp(sin(data(i, j))));  //had to use compute bound math due to memory speed limitations??
        }
        sums_serial(i) = row_sum;
    }
    double time_serial = timer.seconds();
    

    timer.reset();
    Kokkos::parallel_for("row_sum_parallel", m, KOKKOS_LAMBDA(const int i){
        double row_sum = 0;
        for(int j = 0; j < n; j++){
            row_sum += cos(exp(sin(data(i, j)))); //had to use compute bound math due to memory speed limitations??
        }
        sums_parallel(i) = row_sum;
    });
    Kokkos::fence();
    double time_parallel = timer.seconds();
    
    std::cout << std::fixed << std::setprecision(6);
    
        std::cout << "\n--- Performance Results ---" << std::endl;
        std::cout << "Serial time: " << time_serial << " s" << std::endl;
        std::cout << "Parallel Loop Time: " << time_parallel << " s" << std::endl;
        std::cout << "Speedup:            " << (time_serial / time_parallel) << "x" << std::endl;
    
    
    
    }
    Kokkos::finalize();
    return 0;
}
