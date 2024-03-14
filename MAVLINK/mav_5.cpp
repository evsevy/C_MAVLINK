//*********************** © MEGKULOV E.V. 2024********************************************//

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using boost::asio::ip::tcp;

mutex mtx;

void handle_connection(tcp::socket socket) {
    try {
        for (;;) {
            boost::asio::streambuf buf;
            boost::asio::read_until(socket, buf, "\n");
            std::istream input(&buf);
            
            std::string data;
            std::getline(input, data);

            mtx.lock();
            cout << "Received data: " << data << endl;
            mtx.unlock();
        }
    } catch (std::exception& e) {
        mtx.lock();
        cout << "Exception: " << e.what() << endl;
        mtx.unlock();
    }
}

void send_data(tcp::socket socket) {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "Error opening camera" << endl;
        return;
    }

    Mat frame;
    stringstream encoded;
    vector<uchar> buffer;

    while (true) {
        cap >> frame;
        imencode(".jpg", frame, buffer);

        boost::asio::write(socket, boost::asio::buffer(buffer));

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    try {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));

        for (;;) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            thread t1(handle_connection, std::move(socket));
            thread t2(send_data, std::move(socket));

            t1.join();
            t2.join();
        }
    } catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
