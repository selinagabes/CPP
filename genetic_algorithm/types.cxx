//
//------------------------------------------------------------------------------
//
//
//
//
// This file contains the definitions for the member functions of the types.
//
//------------------------------------------------------------------------------

#include "types.hxx"

#include <limits>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------

namespace cs340
{
  ostream& operator << (ostream& os, schedule const& c)
  {
    os << "{ ";
    for (size_t i{}; i < c.tasks(); ++i)
      os << c.task_assignment(i) << ' ';
    os << '}';
    return os;
  }

  std::ostream& operator << (std::ostream& os, runtime_matrix const& matrix)
  {
    for (size_t i{}; i < matrix.tasks(); ++i)
    {
      for (size_t j{}; j < matrix.machines(); ++j)
        os << matrix(i, j) << ' ';
      os << '\n';

    }
    return os;
  }

  // Generate a random matrix with the given dimensions.
  runtime_matrix create_random_matrix(size_t const t, size_t const m,
      size_t const time_max, random_generator& gen)
  {
    //Create a runtime matrix
    //INPUT: size_t tasks, size_t machines, time_max (convergence) and a ref to a random  number generator
    //OUTPUT: A runtime matrix
    //runtime matrix takes in machines, and tasks; constructs a mtrix accordingly
    //and fills it with random numbers.

    // 1.
    //runtime matrix constructor
    runtime_matrix matrix{t, m};

    // 2.
    //in dstribution from [0, time_max]

    std::uniform_int_distribution<size_t> dis(0, time_max);
    // 3.
    //fill matrix using random number generator and uniform int distribution.
    for(size_t i = 0; i < t; ++i)
    {
      for(size_t j = 0; j < m; ++j)
      {
        matrix(i,j) = dis(gen);

      }

    }

    return matrix;
  }

  // We compute the score of each schedule via it's makespan.  The
  // makespan of a schedule is simply the total time from start to
  // finish. In our case, that means the makespan is the maximum time
  // any given machine will take to finish all of the tasks assigned
  // to it.
  //
  // Since a lower makespan is better, but a higher score means a
  // solution is more fit, we invert the makespan and pass the result
  // through a smoothening function to get a decent score.

  double schedule::score(runtime_matrix const& matrix) const
  {
    //compute a schedules score
    //INPUT: ref to a runtime matrix
    //OUTPUT: the smoothened score of the schedule's makespan as a double.
    // Score is based on makespan of a schedule (total time from start to finish)
    //lower makespan = more fit.. therefore ours is inverted to achieve the smoothening
    // 1.
    // if data_ is empty. If so, return zero.
    if(data_.empty())
      return 0;


    // 2.
    //If we have computed the sccore already, return it

    if(has_cache_ == true)
      return cached_score_;

    // 3.
    //used to map machines to tasks that will run
    //this is dictated by schedule
    std::multimap<size_t, size_t> machine_schedule;
    // Populate the multimap with a mutable lambda and for_each.
    for_each(
        begin(data_),
        end(data_),
        [&machine_schedule, i = 0](auto const& m) mutable
        {
        machine_schedule.emplace(m, i);
        ++i;
        }
        );

    // 4.
    //Assign total runtime to min of size_t before the loop
    auto total_runtime = std::numeric_limits<size_t>::min();
    for (size_t m = 0; m < matrix.machines(); ++m)
    {
      // 5.
      //find the equal rand in machine_schedule for m
      auto range = machine_schedule.equal_range(m);
      //auto range = equal_range(machine_schedule.begin(), machine_schedule.end(), m);

      //
      // 6.
      //Compute the total run time for the current machine
      //Add the accumulated value to the next value.
      size_t total_rt = std::accumulate(range.first,
          range.second,
          size_t(0),
          [&matrix](auto const& sum, auto const& elem)
          {
          return sum + matrix(elem.second, elem.first);
          });

      // 7.
      //update the total_runtime to be the max of previously calculated
      //value or itself
      total_runtime = std::max( total_runtime, total_rt);
    }

    // 8.
    //set the cached_score
    //cached score = 1 / (total runtime + 1) * 1000
    cached_score_ = (double)1.0/(total_runtime+1) * 1000;
    //update has_cache
    has_cache_ = true;
    // 9.
    //return calculated value
    return cached_score_;
  }
}

//------------------------------------------------------------------------------
