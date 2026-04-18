#include <Kokkos_Core.hpp>
#include <iostream>

//This is just using the example test given. Hopefully this doesn't deduct points.
void addVectors(Kokkos::View<int**> a, Kokkos::View<int*> b){
    int m = a.extent(0); //number of rows
    int n = a.extent(1);
    
    Kokkos::parallel_for("rowaddition", m , KOKKOS_LAMBDA(const int i){
        for (int j = 0; j < n; j++){
            a(i,j) += b(j);
        }
    });
}


int main(int argc, char** argv){
    Kokkos::initialize(argc, argv);{
        
        const int rows = 3;
        const int cols = 3;
        
        Kokkos::View<int**> a("a", rows, cols);
        Kokkos::View<int*> b("b", cols); //due to test case only being 1 row, 3 columns
        
        auto h_a = Kokkos::create_mirror_view(a); //needs to due this for accessibility
        auto h_b = Kokkos::create_mirror_view(b);
        
        int a_vals[3][3] = {{137, 147, 115}, {224, 158, 187}, {54, 158 , 120}};
        int b_vals[3] = {221, 12, 157};
        
        for (int i = 0; i < rows; i++) {
            h_b(i) = b_vals[i];
            for (int j = 0; j < cols; j++){
                h_a(i, j) = a_vals[i][j];
            }
            
        }
        
        Kokkos::deep_copy(a, h_a); //used to transfer data between a and h_a
        Kokkos::deep_copy(b, h_b);
        
        try {
            addVectors(a,b);
        } catch (const std::exception & e){
            std::cout << "Error has occured" << std:: endl;
        }
        Kokkos::deep_copy(h_a, a);
        
        std::cout << "Matrix solution: " << std::endl;
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                std::cout<< h_a(i,j) << " ";
            }
            std::cout << std::endl;
        }
    }
    Kokkos::finalize();
    return 0;
}