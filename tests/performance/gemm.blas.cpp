#include "params.hpp"

namespace Random{
   struct random {
       random(){};
       double operator()(){return drand48();} 
       int IntRd(){return rand();}
   };
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gemm_blas, T, test_types){
    size_t x = get_input_x<T>();
    size_t y = get_input_y<T>();
    size_t nthreads = get_input_threads<T>();

    Random::random Rd;
    omp_set_num_threads(nthreads);
    typename T::value_type* ad;
    typename T::value_type* bd;
    typename T::value_type* cd;
    
    int m,n,k;
    int lda,ldb,ldc;
    double alpha(1.0), beta(1.0);

    m = (int)x;
    n = (int)y;
    k = (int)y;

    lda = ldb = ldc = (int)x;

    ad = new typename T::value_type[x*y]; 
    bd = new typename T::value_type[x*y]; 
    cd = new typename T::value_type[x*y]; 
  
    memset((void*)cd,0,x*y*sizeof(typename T::value_type));
   
    for(int i(0); i< x*y ; ++i){
        ad[i] = Rd();
        bd[i] = Rd();
    }
       
    ambient::timer time("blas");
    time.begin();
    ambient::numeric::kernels::helper_blas<typename T::value_type>::gemm("N","N", &m, &n, &k, &alpha, ad, &lda, bd, &ldb, &beta, cd, &ldc);
    time.end();

    report(time, GFlopsGemm, x, y, nthreads);
    delete [] ad;   
    delete [] bd;   
    delete [] cd;   
}

