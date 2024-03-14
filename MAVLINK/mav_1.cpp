//*********************** © MEGULOV E.V. 2024********************************************//

#include <mavlink.h>
#include <iostream>

void sendMavlinkData() {
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    // Создание mavlink сообщения
    mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_ARDUPILOTMEGA, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);

    // Кодировка mavlink сообщения
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // Отсылка данных:
    // Example: Send the data over a serial port
    // write(fd, buf, len);

    std::cout << "Отсылаем MAVLink данные" << std::endl;
}

int main() {
    sendMavlinkData();

    return 0;
}
/*
 MAVLink сообщение типа HEARTBEAT, упаковывает его в буфер и отправляет данные по серийному порту (например, UART).
*/
