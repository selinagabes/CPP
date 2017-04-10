//main.cxx used to run Genetic Algorithm
//Nikola Zjalic, Selina Gabriele, Saman Arif
//03-60-340
//Winter 2017

//------------------------------------------------------------------------------
//
// This program runs a genetic algorithm for computing task/machine
// schedules.  It is mainly for testing the GA routine on subsequently
// larger and larger pool sizes to see how it compares when run
// serially vs multithreaded.
//
//------------------------------------------------------------------------------

#include "types.hxx"
#include "ga.hxx"
#include "program_options.hxx"

#include <random>
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>

#define TIME_MAX 30 //used later to generate matrix
//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  using namespace std;

  // 1. 
  //Pass the values taken from the command line to program_options

  cs340::program_options args{argc, argv};

  // 2.
  //Set up iterators to pass into seed_seq (first, last+1)
  auto first_seed = begin(args.seeds);
  auto past_last_seed = end(args.seeds);
  // create a seed sequence using the seeds passed in from args
  std::seed_seq seeds(first_seed, past_last_seed);
  //create a number generating engine using your seed sequence
  cs340::random_generator engine(seeds);
  // 3. 
  //More details in program_options.hxx
  cs340::simulation_parameters params
  {args.generations, //number of generations is represented by args.generations
  args.min_pool_size, // min pool size
  args.threads}; //number of threads


  // 4. 
  // random matrix generated
  // using task and machines, taken from args
  // using the #define from above,the TIME_MAX is set to 30
  // and the random number generator

  // Create a matrix object by calling the function
  // cs340::create_random_matrix. Pass in the correct parameters
  // (see types.hxx and types.cxx) for the interface. Use the value
  // 30 for the time_max parameter.
  //cout << args.tasks << "\t" << args.machines << endl;

auto randomMatrix = cs340::create_random_matrix(args.tasks,args.machines,TIME_MAX,engine);
cout << "Pool\tResult\tTime (s)\n";
for ( ;
 params.pool_size <= args.max_pool_size; 
 params.pool_size += args.pool_size_step)
{

    //a. record current CPU for start time of simulation, using chrono
    auto start_time = std::chrono::high_resolution_clock::now(); 
    // b. RUN SIMULATION
    auto result = cs340::run_simulation(randomMatrix,params,engine); 
    //c. record CPU for end time of simulation 
    auto end_time = std::chrono::high_resolution_clock::now(); 

    //6.
    //number of seconds taken from start to finish
    std::chrono::duration<double> ellapsed_time{end_time-start_time}; 
    std::cout 
/*i*/   << params.pool_size   
        << "\t" 
        //<< std::setprecision(6)
/*ii*/  << result.score(randomMatrix) 
        << "\t"
/*iii*/ << ellapsed_time.count() 
        << endl;

  }
}
