#include <Kokkos_Core.hpp>
#include <iostream>
int main(int argc, char** argv) {
Kokkos::initialize( argc, argv );
{
int n;
int result = 0;

std::cout << "Number of elements: " << std::endl;
std::cin >> n;

Kokkos::View<int*> v("my_view", n);
// fill array
Kokkos::parallel_for("fill_view", n, KOKKOS_LAMBDA(const int i){
    v(i) = i;
});

Kokkos::parallel_reduce("parallel_sum", v.extent(0), KOKKOS_LAMBDA(const int&
i, int& lsum){
    
    printf("Processing index %d | Current local sum before adding: %d\n", i, lsum);
    
lsum+=v(i);

}, result);
std::cout << "Result: " << result << std::endl;
}
Kokkos::finalize();
return 0;
}