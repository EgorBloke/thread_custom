

#include <iostream>
#include<thread>
#include<chrono>
#include<mutex>
#include"C:\myprograms\C++\classes\classes\classes\Classes.h"

std::mutex mtx;
std::mutex mtx1;
std::mutex mtx2;
std::recursive_mutex rm;
int slpTime = 50;

void myFunction()
{
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "thread id = \t" << std::this_thread::get_id() << "\ti = " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
	}
	return;
}
int summ(int a, int b, std::string msg="msg default")
{
	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
	std::cout << "summ started" << std::endl;
	std::cout << "a + b = " << a + b << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
	std::cout << "summ ended" << std::endl;
	return a + b;
}
void print(char c)
{
	
			
	{
		
		std::lock_guard< std::mutex> guard(mtx); //создает объект guard который уничтожается в момент
			//выхода из области видимости.
		std::cout << "Print thread id:\t" << std::this_thread::get_id() << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				std::cout << c;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
	
}

void print_mtx(char c)
{
	
	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime)); //мьютекс блокирует только указанный учсаток
				//секции до него так же выполнются параллельно
	mtx.lock(); //мьютекс блокирует участок когда от выполнения другим потоком.
	std::cout << "Print_mtx thread id:\t" << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			std::cout << c;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	mtx.unlock(); //если забыть разблокировать мьтекс, то к участку кода не будет доступа для других потоков
			//что бы это предотврать, можно использовать std::lock_guard
}

void print_dead1(char c)
{
	mtx1.lock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	mtx2.lock();
	{			
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				std::cout << c;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	mtx2.unlock();
	mtx1.unlock();
}

void print_dead2(char c)
{
	//mtx2.lock(); //неверный порядок захвата мьютексов
	//mtx1.lock();

	mtx1.lock();
	mtx2.lock();

	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				std::cout << c;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	mtx2.unlock();
	mtx1.unlock();

	//mtx1.unlock();
	//mtx2.unlock();
}
void f(int a)
{

	rm.lock();
	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
	std::cout << a << " ";
	if (a <= 1)
	{
		rm.unlock();
		return;
	}
	--a;
	f(a);
	rm.unlock();
	return;
}
void print_uniq(char c)
{
	
	std::unique_lock< std::mutex> un(mtx, std::defer_lock);
	un.lock();  //необзязательно, если создан объект без параметра defer_lock (std::unique_lock< std::mutex> un(mtx);)
			//!!!!метод unlock() и lock() вызывается у объекта класса unique_lock!!!!
				//std::unique_lock< std::mutex> un(mtx); 
			//создает объект unique_lock который уничтожается в момент
			//выхода из области видимости. В отличии от lock_guard, в нем можно заблокировать мьютекс вручную
			// для этого нужно передать в конструктор параметр std::defer_lock
		
		std::cout << "Print thread id:\t" << std::this_thread::get_id() << std::endl;
		
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				std::cout << c;
			}
			std::cout << std::endl;
		}
		
		std::cout << std::endl;
		//mtx.unlock(); //необязательно, тк деструктор объекта un класса unique_lock разблокирует мьютекс 
			//!!!!метод unlock() и lock() вызывается у объекта класса unique_lock!!!!
			//по выходу из области видимости
			// однако 
	

	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));

}

/////////////MAIN/////////////////////
int main()
{
	//thread
	{
		std::cout << std::endl << "SECTION 1: THREAD" << std::endl << std::endl;
		std::thread th(myFunction); //в поток передается указатель на функцию
	//th.detach(); //отрывает поток от основного потока. основной поток не дожидается выполнения th
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << "thread id = \t" << std::this_thread::get_id() << " \ti = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
		}
		th.join(); //точка ожидания потока функцией main
		std::thread th2(summ, 4, 3, "user messenge");
		//параметры в функци, вызываемую в потоке передаются как аргументы потока
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << "thread id = \t" << std::this_thread::get_id() << "\ti = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
		}
		th2.join();
	}	

	//ЛЯМБДА функция для возврата результата из потока
	
	{
		std::cout << std::endl << "SECTION 2: LAMBDA" << std::endl << std::endl;
		int result;
		auto f = [&result]() {result = summ(4, 9, "msg"); };//Указатель на лямбда функцию
				//лямбда функция в теле вызывает функции сумм и по ссылке [&result] принимает 
				//переменную result для возврата значения из потока
		std:: thread t(f);
		
		t.join(); //!!!
		std::cout << "result = " << result << std::endl;
	}
	//методы класса в отдельном потоке
	{
		std::cout << std::endl << "SECTION 3: CLASSES METHODS IN THE THREAD" << std::endl << std::endl;
		MyClassT m;
		std::thread t(&MyClassT::DoWork, m, 50); // Вызов метода класс MyClassT в потоке t
					//в качестве аргументов потока передается ссылка на метод класса &MyClassT::DoWork, объект класса m, 
					//аргументы метода класса
		std::thread t2([&m]() //Вызов метода класс MyClassT в потоке t через лямбда-выражение
					//в контекс лямбды долна быть передана ссылка на объект класса &m
			{ 
			m.Dowork2();
			});
		t.join();
		t2.join();
	}
	//mutex, lock_guard
	{
		std::cout << std::endl << "SECTION 4: MUTEX, LOCK_GUARD" << std::endl << std::endl;
		std::thread t1(print, '#');
		std::thread t2(print, '@');
		t1.join();
		t2.join();

		std::cout << std::endl;
		
		std::thread t3(print_mtx, '#');
		std::thread t4(print_mtx, '@');
		t3.join();
		t4.join();
	}
	//deadlock
	{
		std::cout << std::endl << "SECTION 5: DEADLOCK" << std::endl << std::endl;
		//вызываем две идентичные функции print, с той разницой, что в них мьютексы захватываются
		//в разном порядке. В первой функции есть  пауза 1 мс между захватом мьютекса, для того 
		//что бы print2  успела захватить мьютекс mtx2
		
		std::thread t1(print_dead1, '#');
		std::thread t2(print_dead2, '@');

		//В такой ситуации выполнение программы останавливается, т.к. оба мьютекса заблокированны

		//что бы избежать этого, мьютесыв обеих функциях должны захватывать в одном и том же порядке		
		t1.join();
		t2.join();
	}

	//recursive_mutex
	{
		std::cout << std::endl << "SECTION 6: RECURSIVE_MUTEX" << std::endl << std::endl;
		std::thread t1(f, 10);
		std::thread t2(f, 12);

		t1.join();
		std::cout << std::endl;
		t2.join();
	}
	//unique_lock vs lock_gueard
	{
		std::cout << std::endl << "SECTION 7: unique_lock vs lock_gueard" << std::endl << std::endl;
		std::thread t1(print_uniq, 'q');
		std::thread t2(print_uniq, 'r');
		t1.join();
		t2.join();
	}
    return 0;
}

