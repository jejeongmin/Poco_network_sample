
// Timespan�� ����ũ�μ�����, Timespan(long seconds, long microseconds); ����ϱ�
// �������� Ŭ���̾�Ʈ���� ������ ������ �Ϻη� 3�� ���Ŀ� ������.
// Ŭ���̾�Ʈ�� poll���� 2�ʷ� ��Ƽ� ��ȯ ���� ��� �������� ����. ���� ��¥ ���� ���� true�� ���� ��
// ���� ��Ȳ�� �°� ������ �����ؾ� �Ѵ�.

//
//EchoServer echoServer;
//StreamSocket ss;
//ss.connect(SocketAddress("localhost", echoServer.port()));
//ss.setBlocking(false);
//
//Timespan timeout(1000000);
//assert(ss.poll(timeout, Socket::SELECT_WRITE));
//int n = ss.sendBytes("hello", 5);
//assert(n == 5);
//
//char buffer[256];
//assert(ss.poll(timeout, Socket::SELECT_READ));
//n = ss.receiveBytes(buffer, sizeof(buffer));
//assert(n == 5);
//assert(std::string(buffer, n) == "hello");
//ss.close();
#include "Poco/Net/Net.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"

const Poco::UInt16 PORT = 32452;

int main()
{
	std::cout << "������ ���� �õ�..." << std::endl;
	Poco::Net::StreamSocket ss;

	try
	{
		ss.connectNB(Poco::Net::SocketAddress("localhost", PORT));

		char szMessage[] = "Send Message From Client";
		auto nMsgLen = (int)strnlen_s(szMessage, 128 - 1);

		ss.sendBytes(szMessage, nMsgLen);
		std::cout << "������ ���� �޽���: " << szMessage << std::endl;


		Poco::DateTime now1;
		std::cout << "poll ����: " << now1.second() << std::endl;
		Poco::Timespan timeout(2, 0); // 2��
		auto pollResult = ss.poll(timeout, Poco::Net::Socket::SELECT_READ);
		Poco::DateTime now2;
		std::cout << "poll �Ϸ�: " << now2.second() << std::endl;
		
		if (pollResult)
		{
			char buffer[256] = { 0, };
			auto len = ss.receiveBytes(buffer, sizeof(buffer));
			std::cout << "�����κ��� ���� �޽���: " << buffer << std::endl;			
		}
		else
		{
			std::cout << "poll ���: false" << std::endl;
			std::cout << "poll �ٽ� ����: " << &Poco::DateTime::second << std::endl;

			pollResult = ss.poll(timeout, Poco::Net::Socket::SELECT_READ);

			if (pollResult)
			{
				char buffer[256] = { 0, };
				auto len = ss.receiveBytes(buffer, sizeof(buffer));
				std::cout << "�����κ��� ���� �޽���: " << buffer << std::endl;
			}
		}

		ss.close();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << "���� ���� ����: " << exc.displayText() << std::endl;
	}
	
	getchar();
	return 0;
}