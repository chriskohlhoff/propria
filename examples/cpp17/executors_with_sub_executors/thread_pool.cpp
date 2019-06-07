#include "execution.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

//------------------------------------------------------------------------------

class thread_pool
{
private:
  class queue;

public:
  class sub_executor_type
  {
  public:
    static constexpr std::size_t query(execution::occupancy_t)
    {
      return 1;
    }

    friend bool operator==(const sub_executor_type& a, const sub_executor_type& b) noexcept
    {
      return &a.queue_ == &b.queue_;
    }

    friend bool operator!=(const sub_executor_type& a, const sub_executor_type& b) noexcept
    {
      return &a.queue_ != &b.queue_;
    }

    template <class F>
    void execute(F f) const
    {
      std::unique_lock<std::mutex> lock(queue_.mutex_);
      queue_.functions_.emplace_front(new function<F>(std::move(f)));
      queue_.condition_.notify_one();
    }

  private:
    friend class thread_pool;

    queue& queue_;

    sub_executor_type(queue& q)
      : queue_(q)
    {
    }
  };

  class executor_type
  {
  public:
    std::size_t query(execution::occupancy_t) const
    {
      return pool_.thread_queues_.size();
    }

    friend bool operator==(const executor_type& a, const executor_type& b) noexcept
    {
      return &a.pool_ == &b.pool_;
    }

    friend bool operator!=(const executor_type& a, const executor_type& b) noexcept
    {
      return &a.pool_ != &b.pool_;
    }

    template <class F>
    void execute(F f) const
    {
      auto qn = pool_.next_queue_++ % pool_.thread_queues_.size();
      queue& q = pool_.thread_queues_[qn];
      std::unique_lock<std::mutex> lock(q.mutex_);
      q.functions_.emplace_back(new function<F>(std::move(f)));
      q.condition_.notify_one();
    }

  private:
    friend class thread_pool;

    thread_pool& pool_;

    executor_type(thread_pool& p)
      : pool_(p)
    {
    }
  };

  explicit thread_pool(std::size_t n)
    : thread_queues_(n)
  {
    for (auto& q : thread_queues_)
    {
      threads_.emplace_back(
          [&q]
          {
            std::unique_lock<std::mutex> lock(q.mutex_);
            for (;;)
            {
              while (!q.stopped_ && q.functions_.empty())
                q.condition_.wait(lock);
              if (q.stopped_)
                break;
              auto f = std::move(q.functions_.front());
              q.functions_.pop_front();
              lock.unlock();
              f->invoke(sub_executor_type(q));
              lock.lock();
            }
          });
    }
  }

  ~thread_pool()
  {
    if (!threads_.empty())
    {
      stop();
      wait();
    }

    for (auto& q : thread_queues_)
    {
      std::unique_lock<std::mutex> lock(q.mutex_);
      if (!q.functions_.empty())
      {
        auto functions = std::move(q.functions_);
        lock.unlock();
        std::unique_lock<std::mutex> lock2(failed_queue_.mutex_);
        failed_queue_.functions_.splice(failed_queue_.functions_.end(), functions);
      }
    }

    std::unique_lock<std::mutex> lock(failed_queue_.mutex_);
    while (!failed_queue_.functions_.empty())
    {
      auto f = std::move(failed_queue_.functions_.front());
      failed_queue_.functions_.pop_front();
      lock.unlock();
      f->invoke(sub_executor_type(failed_queue_));
      lock.lock();
    }
  }

  executor_type executor()
  {
    return executor_type(*this);
  }

  sub_executor_type failed_executor()
  {
    return sub_executor_type(failed_queue_);
  }

  void stop()
  {
    for (auto& q : thread_queues_)
    {
      std::unique_lock<std::mutex> lock(q.mutex_);
      q.stopped_ = true;
      q.condition_.notify_one();
    }
  }

  void wait()
  {
    while (!threads_.empty())
    {
      threads_.front().join();
      threads_.pop_front();
    }
  }

private:
  struct function_base
  {
    virtual ~function_base() = default;
    virtual void invoke(const sub_executor_type&) noexcept = 0;
  };

  template <class F>
  struct function : function_base
  {
    F f_;

    explicit function(F f)
      : f_(std::move(f))
    {
    }

    void invoke(const sub_executor_type& sub_ex) noexcept override
    {
      std::invoke(std::move(f_), sub_ex);
    }
  };

  struct queue
  {
    std::mutex mutex_;
    std::condition_variable condition_;
    std::list<std::unique_ptr<function_base>> functions_;
    bool stopped_ = false;
  };

  std::atomic<std::size_t> next_queue_{0};
  std::vector<queue> thread_queues_;
  std::list<std::thread> threads_;
  queue failed_queue_;
};

//------------------------------------------------------------------------------

int main()
{
  thread_pool pool(3);

  pool.executor().execute(
      [](const auto&) {
        std::fprintf(stderr, "task 1: starts\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::fprintf(stderr, "task 1: ends\n");
      }
    );

  pool.executor().execute(
      [](const auto&) {
        std::fprintf(stderr, "task 2: starts\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::fprintf(stderr, "task 2: ends\n");
      }
    );

  pool.executor().execute(
      [](thread_pool::sub_executor_type sub_ex) {
        std::fprintf(stderr, "task 3a: starts\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        sub_ex.execute(
            [](const auto&) {
              std::fprintf(stderr, "task 3b: starts\n");
              std::this_thread::sleep_for(std::chrono::milliseconds(500));
              std::fprintf(stderr, "task 3b: ends\n");
            }
          );
        std::fprintf(stderr, "task 3a: ends\n");
      }
    );

  pool.executor().execute(
      [&](thread_pool::sub_executor_type sub_ex)
      {
        if (sub_ex == pool.failed_executor())
          std::fprintf(stderr, "task 4: failed to start\n");
        else
        {
          std::fprintf(stderr, "task 4: starts\n");
          std::this_thread::sleep_for(std::chrono::seconds(1));
          std::fprintf(stderr, "task 4: ends\n");
        }
      });

  execution::executor<> ex(pool.executor());
  execution::executor<> failed_ex(pool.failed_executor());
  ex.execute(
      [failed_ex](execution::executor<> sub_ex)
      {
        if (sub_ex == failed_ex)
          std::fprintf(stderr, "task 5: failed to start\n");
        else
        {
          std::fprintf(stderr, "task 5: starts\n");
          std::this_thread::sleep_for(std::chrono::seconds(1));
          std::fprintf(stderr, "task 5: ends\n");
        }
      });

  std::this_thread::sleep_for(std::chrono::milliseconds(900));
}
