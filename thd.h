// -*- c-file-style: "gnu" -*-
////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <mutex>
# include <thread>
# include <vector>
# include <queue>
# include <functional>
# include <condition_variable>
////////////////////////////////////////////////////////////////
namespace thd {
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
class TaskForce {
// Its always nice to find a moment to yell at c++ cos all other
// time c++ compiler is yelling at you, my question is why can't
// I put: using namespace std; here?
private:
  using task_t = function<void()>;

  mutex mtx;

  condition_variable cv;
  bool fin = false;
  queue<task_t> q;

  vector<thread> shinobi;

public:
  TaskForce( int size=thread::hardware_concurrency() ){
    for( int j = 0; j < size; ++j ){
      shinobi.emplace_back( [this]
      { yosh:;
        task_t task;
        {
          unique_lock<mutex> lock( mtx );

          cv.wait( lock, [this]{ return fin or !q.empty(); });

          if( fin and q.empty() ){ return; }

          task = move( q.front() );
          q.pop();
        }
        task();
        goto yosh;
      });      
    }
  }
  void enqueue( task_t&& task ){
    {
      unique_lock<mutex> lock( mtx );
      q.push( move( task ));
    }
    cv.notify_one();
  }
  ~TaskForce() {
    {
      unique_lock<mutex> lock( mtx );
      fin = true;
    }
    cv.notify_all();
    for( auto& ninja: shinobi ){ ninja.join(); }
  }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
