

//
// This file contains the implementation of the genetic algorithm simulation.
// There are many helper functions inside of an anonymous namespace. That
// ensures those functions have static linkage and are not visible outside
// of this translation unit.
//
// The helper functions all take in a random generator by reference. The
// reason for passing it in as a parameter instead of relying on a glboal
// RNG is to avoid having to lock the generator when it is being used in
// different threads. The way that these functions are written, each thread
// can have its own RNG (seeded appropriately BEFORE firing off the thread)
// without introducing data races and other nasty surprises.
//
//------------------------------------------------------------------------------

#include "ga.hxx"
#include "types.hxx"

#include <utility>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cstddef>
#include <future>

using namespace std;

//------------------------------------------------------------------------------

namespace cs340
{
  namespace
  {
    // Special sorting function object to keeping our gene pools sorted.
    struct schedule_compare
    {
      explicit schedule_compare(runtime_matrix const& m)
        : matrix_{m}
      { }


      //Function overload
      //INPUT: 2 const references to schedule objects
      //OUTPUT: Bool value (schedule1 scores greater than schedule 2 scores)
      //Used for sorting.
      bool operator () (schedule const& sched1, schedule const& sched2)
      {
        return (sched1.score(matrix_) > sched2.score(matrix_));
      }

      private:
      runtime_matrix const& matrix_;
    };

    // Populate the gene pool with random values. Each machine in each
    // schedule has equal probability of occuring.
    auto populate_gene_pool(runtime_matrix const& matrix,
        size_t const pool_size, random_generator& gen)
    {
      //Populate Gene Pool Function
      //INPUT: Reference to matrix, Size of Gene pool and Random number generator
      //OUTPUT: Populated Schedule objects
      //Populates a pool with(schedule random values,
      //where the distribution of each machine in each schedule
      //has equal probability of occurring

      //1.
      //Allocate container for schedules as well as space in memory
      std::vector<schedule> schedules;
      schedules.reserve(pool_size);


      //2.
      //Distribution used to take samples from and place in schedules
      std::uniform_int_distribution<size_t> dis(0 ,matrix.machines()-1);

      //3.
      //Fills each schedule in the vector using the distribution above
      //and the random number generator.
      std::generate_n(std::back_inserter(schedules),
          pool_size,
          [&dis, &gen, &matrix]()
          {
          schedule sched{matrix.tasks()};
          for(size_t i = 0; i < matrix.tasks(); ++i )
          sched.set_task_assignment(i, dis(gen));

          return sched;
          });

      // 4.
      // Sort  the populated pool based on their scores
      // using the schedule_compare function defined in types.cxx
      std::stable_sort(schedules.begin(), schedules.end(), schedule_compare(matrix));

      // 5. Return the pool of schedules.
      return schedules;
    }

    // Crossing two schedules over involves selecting a random spot
    // in the first schedule, and copying everything from the
    // beginning to that spot into a new schedule. Everything after
    // that spot is taken from the second schedule. The result is a
    // new schedule that (hopefully) inherits some of the desirable
    // traits from the two parents.
    auto cross_over(schedule c1, schedule const& c2, random_generator& gen)
    {
      //Crossover function
      //INPUT: 1 schedule by value, one reference to a schedule and a random number generator
      //OUTPUT: the schedule passed by value with inherited desirable scores
      //A random spot is chosen from the first schedule, n,
      //from the values from the beginning of c1 to n remain in c1 (or are copied)
      //from n+1 to the end of c1, the values are overwritten with the
      //values from c2

      // 1.
      //Select random point used to split values

      uniform_int_distribution<size_t> dis(0,c1.tasks()-1);

      // 2.
      // IF we assume that the scores are sorted in the schedules,
      // Copy the values from c2 (from beginning to crossover point)
      // into c1 (from crossover point to end)
      // HOWEVER
      // since these are randomly seeded, we cannot guarantee the
      // good traits to be at the beginning of the container

      size_t const offset = dis(gen);
      //size_t c2_val = 0;
      for(auto j = offset; j < c1.tasks(); ++j)
      {
        c1.set_task_assignment(j, c2.task_assignment(j));
        //next value in c2
       // ++c2_val;
      }

      // 3. Return the new, modified, c1.

      return c1;
    }

    // Randomly change one of the task entries in the schedule.
    void mutate(runtime_matrix const& matrix, schedule& c,
        random_generator& gen)
    {
      //Mutate function
      //INPUT: reference to runtime matrics, reference to schedule and random number generator
      //OUTPUT: Void
      // Mutate a random value in the schedule. This will randomly
      // modify the randomly chosen machine/task in the schedule

      //1.
      //Using the distribution, select a random machine and task to be mutated
      std::uniform_int_distribution<size_t> dis_t(0, matrix.tasks()-1);
      std::uniform_int_distribution<size_t> dis_m(0, matrix.machines()-1);

      auto task = dis_t(gen);
      auto machine = dis_m(gen);
      //set the mutation in the schedule.
      c.set_task_assignment(machine,task);
    }

    // Run through a single generation of the genetic algorithm.
    //
    // First we do crossover to create new schedules. Afterward we
    // perform random mutations to the genes already in the pool.
    //
    // ASSUMPTION: The gene pool is in sorted order before calling
    // this function. Each operation done that modifies or produces
    // a new schedule ensures that the sorting invariant is
    // upheld. But that means nothing at all if they aren't sorted
    // from the get-go. This sorting must happen in the calling code
    // because we explicitly want to avoid re-sorting the entire
    // pool every time a change is made.
    void run_single_generation(runtime_matrix const& matrix,
        vector<schedule>& gene_pool, random_generator& gen)
    {
      //Run a single generation
      //INPUT: ref to runtime matrix, a vector of schedules (gene_pool) and a random number generator
      //OUTPUT: void
      //ASSUMPTION: Sorted gene_pool
      //Runs the genetic algorithm for a single generation.
      //Starts by doing a crossover to create new (sorted)schedules,
      //followed by random mutations within those new schedules, (while remaining sorted).

      // Some sane defaults.
      size_t const min_max_crossovers{(gene_pool.size() / 2) + 1};
      size_t const min_max_mutations{(gene_pool.size() / 3) + 1};

      size_t const max_crossovers{min(size_t{10}, min_max_crossovers)};
      size_t const max_mutations{min(size_t{25}, min_max_mutations)};

      // Initialize the global distribtions.
      uniform_int_distribution<size_t> x_pairs_dist{0, max_crossovers};
      uniform_int_distribution<size_t> mut_dist{0, max_mutations};

      //1.
      //Generate a random amount of cross-over pairs to be used for schedule crossovers
      size_t const x_pairs_count = x_pairs_dist(gen);


      //2.
      //Perform crossover iff
      // -> 0 < |x_pairs| < gene_pool.size()

      if(0 < x_pairs_count && x_pairs_count < gene_pool.size())
      {
        //2a.
        //Make room for new schedules by removing schedules
        //from the back of the current vector (NOTE: remains sorted)
        //Iterator to ensure proper number of schedules removed.
        auto cust_it = gene_pool.begin() + (gene_pool.size() - x_pairs_count);
        gene_pool.erase(cust_it, gene_pool.end());

        //2b.
        //Declare a vector and insert the scores from
        //the gene pool
        //This will be used for the crossover selection

        // Map an (i,j) position down to the 1-D rep
        std::vector<double> totals;

        std::transform(gene_pool.begin(),
            gene_pool.end(),
            back_inserter(totals),
            [&matrix](auto const& sched)
            {
            return sched.score(matrix);
            });

        //2c.
        //Compute partial sum of the vector of scores
        //The partial sum will be placed back in the totals vector
        //This is used for the selection of the cross-over to ensure that a score
        //is chosen within the range of [0,sum(all_scores))

        std::partial_sum(totals.begin(), totals.end(), totals.begin());

        //2d.
        //distribution from [0,sum(all_scores)) for roulette style
        //used for selection
        uniform_real_distribution<double> roulette_dis(0, totals.back());

        // 2e.
        // for loop to execute crossover by x_pairs_count

        for(size_t i = 0; i <x_pairs_count; ++i)
        {
          // 2e i.
          //Two random numbers from our range.

          auto rand_score1 = roulette_dis(gen);
          auto rand_score2 = roulette_dis(gen);
          // 2e ii.
          //find first value NOT LESS than rand_score1
          auto score1_val = lower_bound(totals.begin(), totals.end(), rand_score1);
          // 2e iii.
          //find offset(or place) of the score1_value in the vector

          auto score1_val_offset = distance(totals.begin(), score1_val);

          // 2e iv.
          //find first value NOT LESS that rand_score2
          //find offset(or place) of the score2_value in the vector
          auto score2_val = lower_bound(totals.begin(), totals.end(), rand_score2);

          auto score2_val_offset = distance(totals.begin(), score2_val);

          // 2e v.
          // Using the offsets, declaire iterators pointing to the random values selected

          auto gene1_it = gene_pool.begin()+ score1_val_offset;

          auto gene2_it = gene_pool.begin()+ score2_val_offset;

          // 2e vi.
          // cross over the two schedules pointed to by the iterators
          // and produce a new schedule

          auto crossed_sched = cross_over(*gene1_it, *gene2_it, gen);

          // 2e vii.
          // MAINTAIN SORTED INVARIANT
          // probe the gene pool for the first schedule
          // that is not GREATER than the new schedule
          // this is achieved using schedule_compare
          auto sorting_it = std::lower_bound(gene_pool.begin(), gene_pool.end(), crossed_sched, schedule_compare{matrix});

          gene_pool.insert(sorting_it,crossed_sched);
        }
        // 3. End your if-statement guarding the crossover code.

      }
      //CROSSOVER COMPLETE


      // 4.
      // number of mutations taken from  mutation distribution
      uniform_int_distribution<size_t> dis(mut_dist);
      size_t num_mutations = dis(gen);

      uniform_int_distribution<size_t> m_sel_dist(0, gene_pool.size() - 1);
      //MUTATION TIME
      for (size_t j{}; j < num_mutations; ++j)
      {
        // 4a.
        // Sample of a solution
        // Add the sample taken from m_sel_dist to the gene_pool
        // to get the desired solution

        auto sample = m_sel_dist(gen);
        auto solution = gene_pool.begin() + sample;

        // 4b.
        // call mutate

        mutate(matrix, *solution, gen);

        // 4c.
        // Find the position in the gene pool
        // where schedule is not greater than our solution
        // use schedule_compare

        auto pos = lower_bound(gene_pool.begin(), gene_pool.end(), *solution, schedule_compare(matrix));

        // 4d.
        // if the solution is greater than the position
        // we are going to rotate the the gene_pool to insert
        // the solution, while maintaining the sorted variant

        if( pos > solution )
          //rotate(gene_pool.begin(), sol, gene_pool.end());
          rotate(solution, next(solution), pos);
        else
          //rotate from "back"
          rotate(pos, solution, next(solution));
      }
    }

    // Run the simulation for a fixed number of generations. The
    // gene pool is taken in by reference just in case the updates
    // need to be seen in the calling code.
    //
    // Returns the best schedule seen.
    auto run_simulation_n_times(runtime_matrix const& matrix,
        vector<schedule>& gene_pool,
        size_t const num_generations,
        random_generator& gen,
        size_t const time_til_convergence = 30)
    {
      //run simulation n times
      //INPUT: ref to matric and random number generator. # of generations and a default convergence
      //OUTPUT: the best schedule seen
      //Will sun_single_generation * num_generations to select the best schedule from that


      double best{};
      size_t how_long_unchanged{};

      if (gene_pool.empty()) return schedule{}; // Should never happen.

      for (size_t i{}; i < num_generations; ++i) {
        run_single_generation(matrix, gene_pool, gen);
        auto& best_schedule = gene_pool.front(); //start at the front

        if (best_schedule.score(matrix) > best) {//compare scores
          best = best_schedule.score(matrix);
          how_long_unchanged = 0;
        }
        else
          ++how_long_unchanged;
        if (how_long_unchanged > time_til_convergence) break;
      }

      return gene_pool.front();
    }
  }

  schedule run_simulation(runtime_matrix const& matrix,
      simulation_parameters const& args,
      random_generator& gen)
  {
    //run_simulation
    //INPUT: refs to input matrix, simulation arguments/paraments and random number generation
    //OUTPUT: the best schedule
    //If used properly, it will populate the gene pool
    // and then run the simulation


    // 1.
    //if num of threads in cmd is less than 1 throw.
    if(args.threads < 1)
    {
      throw std::runtime_error("Cannot run on less than 1 thread");
    }
    // 2.
    // if there is only 1 thread, no need for multithreading
   if(args.threads == 1)
    {
      auto gene_pool = populate_gene_pool(matrix, args.pool_size, gen);
      return run_simulation_n_times(matrix, gene_pool, args.generations, gen);
    }
    // Otherwise, multithread
   
      //Each thread will run a pool of solutions and return the best solution

      // 3.
      // vector of future<schedule> that will hold the best schedule
      // from each independent thread simulation

      vector<std::future<schedule>> future_winners;
      future_winners.reserve(args.threads);
      // 4.
      //uniform int distribution from [0,100]
      //used for initial seed

      uniform_int_distribution<size_t> dis(0,100);
      for (size_t i{}; i < args.threads; ++i)
      {
        // 4a.
        // Now create a vector of size_t objects to store the seeds.
        // Populate the vector by sampling your distribution six times.

        vector<size_t> seeds {dis(gen),dis(gen),dis(gen),dis(gen),dis(gen),dis(gen)};


        // 4b.
        // the result from the lamdba will be pushed back
        // into our vector of future winners

        future_winners.push_back(
            async(
              launch::async,
              // This lambda function will execute on a separate thread. We can safely
              // hold a reference to the arguments struct and the matrix since they
              // will be read from only. We move the seeds vector into the lambda
              // (this is a C++14 feature) since they will only be used in the lambda
              // body and nowhere else. This saves us a copy.
              //
              // The return result of the lambda is a schedule object. By passing this
              // lambda into std::async, it converts it into a std::future<schedule> that
              // we then store in our vector of future schedules.
              [&matrix, &args, seeds = move(seeds), i]() -> schedule {
              // 4b i.
              // turn the vector of seeds into a sequence using seed_seq


              std::seed_seq seq(seeds.begin(), seeds.end());

              // 4b ii.
              // Create random_generator for thread
              // using the seed sequence

              random_generator thread_gen(seq);
              // Some constants to help us determine the size of
              // this thread's pool.
              bool const last_iteration{i == args.threads - 1};
              bool const even_split{args.pool_size % args.threads == 0};
              size_t const pool_size
              {
                !last_iteration || even_split
                  ? args.pool_size / args.threads
                  : args.pool_size % args.threads
              };

              // 4b iii.
              // populate the gene pool
              auto gene_pool = populate_gene_pool(matrix, pool_size, thread_gen);
              // 4b iv.
              // run the simulation n times returning the result

              auto result = run_simulation_n_times(matrix, gene_pool, args.generations, thread_gen);
              return result;
              }
        ));
      }


      // Now we need to collect the schedules from our threads.
      schedule best{};

      // 5.
      //find the best schedule in the vector of potential future winner
      for(auto& s :future_winners)
      {
       
        auto&& next_sched = s.get();
        //compare the current schedule with the best
        //if the current score is better than the best
        //reassign it
        if(schedule_compare{matrix}(next_sched, best)){
          // cout << "CHANGING\tfrom "<< best.score(matrix);
          best = std::move(next_sched);
          // cout << " to " << best.score(matrix) << endl;
        }
      }

      // 6.
      //return the best machine!

      return best;
    }
  }


//
//------------------------------------------------------------------------------
