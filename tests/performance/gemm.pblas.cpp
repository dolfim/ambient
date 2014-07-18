#include "params.hpp"

#define AA(i,j) AA[(i)*M+(j)]
#define BB(i,j) BB[(i)*M+(j)]

extern "C" void Cblacs_pinfo( int* mypnum, int* nprocs); 
extern "C" void Cblacs_gridinfo( int nContxt, int* nRows, int* nCols, int* nMyRow, int* nMyCol);
extern "C" void Cblacs_gridinit(  int* nContxt, char * order,  int np_row,  int np_col);
extern "C" void Cblacs_gridexit( int nContxt);
extern "C" void Cblacs_exit( int nContxt);
extern "C" void Cblacs_get(int Contxt, int what, int* val); 
extern "C" int numroc_(int* nN, int* nB, int* nIProc, int* nISrcProc, int* nNProcs);
extern "C" void descinit_(int *, int*, int* , int *, int *, int *, int *, int *, int *, int*);
extern "C" void pdgemm_(char *jobu, char *jobvt,
		int *m, int *n, int *k,
		double *alpha, double * a, int *ia, int *ja, int *desca,
		               double * b, int *ib, int *jb, int *descb,
		double *beta , double * c, int *ic, int *jc, int *descc);


BOOST_AUTO_TEST_CASE_TEMPLATE( test, T, test_types)
{ 
   size_t x = get_input_x<T>();
   size_t y = get_input_y<T>(); // not used (square only)
   int argc=0;
   char ** argv;
   int M(x);
   srand(3);
   int i, j, k;
   int minusone(-1);
   int myrank_mpi, nprocs_mpi;
   int ictxt, nprow, npcol, myrow, mycol,nb;

   int info,itemp;
   int zero=0,one=1;
 // ok it is too large ...
   double* AA = (double*)malloc(M*M*sizeof(double));
   double* BB = (double*)malloc(M*M*sizeof(double));
   double* CC = (double*)malloc(M*M*sizeof(double));

   int descA[9],descB[9],descC[9];
   for(i=0;i<M;i++ )
     for(j=0;j<M;j++){
        AA[i*M+j]=rand();
        BB[i*M+j]=rand();
        CC[i*M+j]=-1;
     }
   ambient::async_timer time("time");
   time.begin();
/************  BLACS ***************************/
   Cblacs_pinfo( &myrank_mpi, &nprocs_mpi) ;

   nprow = 1; // 1-d = one row
   npcol = nprocs_mpi; // number of col for 1d cyclic distribution
   nb = 128; // # of element (width x direction of each column) also workgroup into ambient

   Cblacs_get(minusone, zero, &ictxt);
   Cblacs_gridinit( &ictxt, "C", nprow, npcol );
   Cblacs_gridinfo( ictxt, &nprow, &npcol, &myrow, &mycol );

   int mA = numroc_( &M, &nb, &mycol, &zero, &npcol);
   int mB = numroc_( &M, &nb, &mycol, &zero, &npcol);
   int mC = numroc_( &M, &nb, &mycol, &zero, &npcol);

   //arg 3 = M  number of row of the block issue from the block cyclic decomposition
   //arg 4 = nb number of col of the block issue from the block cyclic decomposition
   //arg 8 = lda issue from numroc here use less because 1d distribution 
 
   descinit_(descA, &M,   &M,   &nb,  &nb,  &zero, &zero, &ictxt, &M,  &info);
   descinit_(descB, &M,   &M,   &nb,  &nb,  &zero, &zero, &ictxt, &M,  &info);
   descinit_(descC, &M,   &M,   &nb,  &nb,  &zero, &zero, &ictxt, &M,  &info);

   double alpha = 1.0; double beta = 0.0;
   pdgemm_("N","N",&M,&M,&M,&alpha,AA,&one,&one,descA,BB,&one,&one,descB,&beta,CC,&one,&one,descC);
   time.end();

   if(myrank_mpi == 0) report(time, GFlopsGemm, x, x, nprocs_mpi);
   MPI_Barrier(MPI_COMM_WORLD);
   Cblacs_exit(0);
}
