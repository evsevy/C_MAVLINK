#include <mavlink.h>
#include <opencv2/opencv.hpp>

void sendTelemetryData(cv::Mat frame) {
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_heartbeat_t heartbeat = {0};
    heartbeat.custom_mode = 0;
    heartbeat.type = MAV_TYPE_GCS;
    heartbeat.autopilot = MAV_AUTOPILOT_INVALID;
    heartbeat.base_mode = 0;
    heartbeat.system_status = MAV_STATE_ACTIVE;

    mavlink_msg_heartbeat_encode(0, 0, &msg, &heartbeat);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // Send the telemetry data over a UDP socket
    // sendto(socket_fd, buf, len, 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera" << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap.read(frame);

        if (frame.empty()) {
            std::cerr << "Error reading frame from camera" << std::endl;
            break;
        }

        sendTelemetryData(frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}