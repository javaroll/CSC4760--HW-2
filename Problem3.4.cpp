#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);{
  int n;
    

  std::cout << "4th dim. size: " << std::endl;
  std::cin >> n;

  Kokkos::View<double****> v("my_view", 5, 7, 12, n);
  
  std::cout << "5 x 7 x 12 x " << n << " = " << v.size() <<" total elements." << std::endl;
    }
  Kokkos::finalize();
  return 0;

}