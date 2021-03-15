

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
		
		std::lock_guard< std::mutex> guard(mtx); //������� ������ guard ������� ������������ � ������
			//������ �� ������� ���������.
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
	
	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime)); //������� ��������� ������ ��������� �������
				//������ �� ���� ��� �� ���������� �����������
	mtx.lock(); //������� ��������� ������� ����� �� ���������� ������ �������.
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
	mtx.unlock(); //���� ������ �������������� ������, �� � ������� ���� �� ����� ������� ��� ������ �������
			//��� �� ��� �����������, ����� ������������ std::lock_guard
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
	//mtx2.lock(); //�������� ������� ������� ���������
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
	un.lock();  //��������������, ���� ������ ������ ��� ��������� defer_lock (std::unique_lock< std::mutex> un(mtx);)
			//!!!!����� unlock() � lock() ���������� � ������� ������ unique_lock!!!!
				//std::unique_lock< std::mutex> un(mtx); 
			//������� ������ unique_lock ������� ������������ � ������
			//������ �� ������� ���������. � ������� �� lock_guard, � ��� ����� ������������� ������� �������
			// ��� ����� ����� �������� � ����������� �������� std::defer_lock
		
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
		//mtx.unlock(); //�������������, �� ���������� ������� un ������ unique_lock ������������ ������� 
			//!!!!����� unlock() � lock() ���������� � ������� ������ unique_lock!!!!
			//�� ������ �� ������� ���������
			// ������ 
	

	std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));

}

/////////////MAIN/////////////////////
int main()
{
	//thread
	{
		std::cout << std::endl << "SECTION 1: THREAD" << std::endl << std::endl;
		std::thread th(myFunction); //� ����� ���������� ��������� �� �������
	//th.detach(); //�������� ����� �� ��������� ������. �������� ����� �� ���������� ���������� th
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << "thread id = \t" << std::this_thread::get_id() << " \ti = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
		}
		th.join(); //����� �������� ������ �������� main
		std::thread th2(summ, 4, 3, "user messenge");
		//��������� � ������, ���������� � ������ ���������� ��� ��������� ������
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << "thread id = \t" << std::this_thread::get_id() << "\ti = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(slpTime));
		}
		th2.join();
	}	

	//������ ������� ��� �������� ���������� �� ������
	
	{
		std::cout << std::endl << "SECTION 2: LAMBDA" << std::endl << std::endl;
		int result;
		auto f = [&result]() {result = summ(4, 9, "msg"); };//��������� �� ������ �������
				//������ ������� � ���� �������� ������� ���� � �� ������ [&result] ��������� 
				//���������� result ��� �������� �������� �� ������
		std:: thread t(f);
		
		t.join(); //!!!
		std::cout << "result = " << result << std::endl;
	}
	//������ ������ � ��������� ������
	{
		std::cout << std::endl << "SECTION 3: CLASSES METHODS IN THE THREAD" << std::endl << std::endl;
		MyClassT m;
		std::thread t(&MyClassT::DoWork, m, 50); // ����� ������ ����� MyClassT � ������ t
					//� �������� ���������� ������ ���������� ������ �� ����� ������ &MyClassT::DoWork, ������ ������ m, 
					//��������� ������ ������
		std::thread t2([&m]() //����� ������ ����� MyClassT � ������ t ����� ������-���������
					//� ������� ������ ����� ���� �������� ������ �� ������ ������ &m
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
		//�������� ��� ���������� ������� print, � ��� ��������, ��� � ��� �������� �������������
		//� ������ �������. � ������ ������� ����  ����� 1 �� ����� �������� ��������, ��� ���� 
		//��� �� print2  ������ ��������� ������� mtx2
		
		std::thread t1(print_dead1, '#');
		std::thread t2(print_dead2, '@');

		//� ����� �������� ���������� ��������� ���������������, �.�. ��� �������� ��������������

		//��� �� �������� �����, �������� ����� �������� ������ ����������� � ����� � ��� �� �������		
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

