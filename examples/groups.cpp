#include "ambient/ambient.hpp"
#include "ambient/container/numeric/matrix.hpp"

template<class Matrix>
void fill(std::vector<Matrix>& left, std::vector<Matrix>& right, std::vector<Matrix>& res, 
          size_t x, size_t y, size_t length)
{
    std::cout << "scope size: " << ambient::scope::size() << "\n"; 
    auto it = ambient::scope::begin();
    for(int i = 0; i < length; i++){
        if(it == ambient::scope::end()) it = ambient::scope::begin();
        ambient::actor proc(it++); 
        std::cout << "executing fill on " << ambient::which() << "\n"; 

        left.push_back(Matrix(y,x));
        right.push_back(Matrix(y,x));
        res.push_back(Matrix(y,x));
        generate(left.back());
        generate(right.back());
    }
}

template<class Matrix>
void mul(std::vector<Matrix>& left, std::vector<Matrix>& right, std::vector<Matrix>& res){
    assert(left.size() == right.size());
    assert(left.size() == res.size());
    auto it = ambient::scope::begin();

    for(int i = 0; i < left.size(); i++){
        if(it == ambient::scope::end()) it = ambient::scope::begin();
        ambient::actor proc(it++);
        std::cout << "executing gemm on " << ambient::which() << "\n"; 

        gemm(left[i], right[i], res[i]);
    }
}

int main(){
    typedef ambient::tiles<ambient::matrix<double> > mtx;
    size_t x = 1024, y = 1024, length = 10;

    std::vector<mtx> left_first;  std::vector<mtx> left_second;
    std::vector<mtx> right_first; std::vector<mtx> right_second;
    std::vector<mtx> res_first;   std::vector<mtx> res_second;

    left_first.reserve(length);   left_second.reserve(length);
    right_first.reserve(length);  right_second.reserve(length);
    res_first.reserve(length);    res_second.reserve(length);
    
    std::cout << "fill...\n"; 
    {
        ambient::scope scopeA(ambient::scope::begin(), ambient::scope::begin()+2);
        fill(left_first, right_first, res_first, x, y, length);
    }
    {
        ambient::scope scopeB(ambient::scope::begin()+2, ambient::scope::begin()+4);
        fill(left_second, right_second, res_second, x, y, length);
    }
    ambient::sync();
    std::cout << "gemm...\n"; 
    {
        ambient::scope scopeA(ambient::scope::begin(), ambient::scope::begin()+2);
        mul(left_first, right_first, res_first);
    }
    {
        ambient::scope scopeB(ambient::scope::begin()+2, ambient::scope::begin()+4);
        mul(left_second, right_second, res_second);
    }
    ambient::sync();

    std::cout << "Done.\n";
    return 0;
}

