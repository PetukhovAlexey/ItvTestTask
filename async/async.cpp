// async.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <messages/protocol/protocol.pb.h>

#include <atomic>
#include <future>
#include <thread_pool/ThreadPool.h>

class TPool
{
public:
  TPool():
    pool(4)
  {}

  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;

  void mark_as_finish()
  {
    dec_in_work();
  }

  bool can_add_in_work()
  {
    std::unique_lock<std::mutex> lock(_mutex);
    if ( _in_work < 20 )
    {
      ++_in_work;
      return true;
    }
    return false;
  }

  void dec_in_work()
  {
    std::unique_lock<std::mutex> lock(_mutex);
    --_in_work;
  }
  
private:
  ThreadPool pool;
  int _in_work;
  std::mutex _mutex;

};

template<typename T>
class saveResult
{
private:
  saveResult(const T& t = T()):
    tmp(t)
  { }
public:
  saveResult(T&& t):
    tmp(std::move(t))
  { }

  T&& get()
  {
    return std::move( tmp );
  }

private:
  T tmp;
};

template<>
class saveResult<void>
{
private:
public:
  saveResult()
  { }

  void get()
  {
  }

};

template<typename T>
saveResult<T> sRes( T&& v )
{
  return saveResult<T>( std::move(v) );
}

// add new work item to the pool
template<class F, class... Args>
auto TPool::enqueue( F&& f, Args&&... args )
-> std::future<typename std::result_of<F( Args... )>::type>
{
  using return_type = typename std::result_of<F( Args... )>::type;
  if ( can_add_in_work() )
  {
    return pool.enqueue( [this, f]( Args... a ) -> return_type
    {
      auto res = sRes( f( a ... ) );
      mark_as_finish();
      return res.get();
    }, args ... );
  }
  return  std::future<typename std::result_of<F( Args... )>::type>();
}

void test(TPool& pool)
{

  for(int i = 0; i < 800; ++i) {
      pool.enqueue([i] {
        int copy = i;
      //std::cout << "hello " << copy << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "world " << copy<< std::endl;
      return copy*copy;
      } );
  }

  std::cout << std::endl;
}

int main()
{
  TPool pool;
  test(pool);
  std::this_thread::sleep_for(std::chrono::seconds(10));

  return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
