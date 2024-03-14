//*********************** © MEGKULOV E.V. 2024********************************************//

#include <mavlink.h>
#include <opencv2/opencv.hpp>

int main() {
    // Create OpenCV video capture object
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }

    // Initialize MAVLink
    mavlink_message_t msg;
    mavlink_status_t status;

    mavlink_message_t local_position_msg;
    mavlink_message_t global_position_msg;

    mavlink_message_t msg_to_send;
    mavlink_message_t ack_msg;

    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Setup the destination address for sending data
    struct sockaddr_in dest_addr;
    memset((char *) &dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(14550);
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        cv::Mat frame;
        cap >> frame;

        // Process the frame here...

        // Send data over MAVLink telemetry
        // For example, sending global position data
        mavlink_msg_global_position_int_pack(1, 1, &global_position_msg, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        mavlink_msg_to_send = global_position_msg;

        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_encode(1, 200, &msg_to_send, &ack_msg);

        // Send the message over UDP
        ssize_t bytes_sent = sendto(sockfd, buffer, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

        if (bytes_sent < 0) {
            perror("ERROR sending message");
            break;
        }

        if (cv::waitKey(1) == 27) { // Exit if ESC is pressed
            break;
        }
    }

    // Cleanup
    close(sockfd);
    cap.release();

    return 0;
}
