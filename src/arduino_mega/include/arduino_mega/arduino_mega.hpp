#include <ros/ros.h>

#include "std_msgs/Int8.h"

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <time.h>
#include <thread>
#include <mutex>

#define protocol_size 5

#define send_protocol_start 0xFF
#define send_protocol_size 0x06
#define send_protocol_arduino_num_zero 0x00
#define send_protocol_arduino_num_one 0x01
#define send_protocol_low 0x00
#define send_protocol_high 0x01

#define rev_protocol_size_needed_data 4
#define rev_protocol_start 0xFF

class Arduino_mega
{
private:
    ros::NodeHandle &nh;

    int serial_port;
    int arduino_num_for_send;
    int pin_num_for_send;
    int digital_signal_for_send;

    int rev_arduino_num;
    int rev_arduino_pin_num;
    int rev_arduino_status;

    void initvalue(void);
    void initPublisher(void);
    void initSubscriber(void);
    bool serial_connect(void);
    void send_serial(const std_msgs::Int8::ConstPtr &arduino_mega_msg);
    bool send_protocol(unsigned char send_serial_protocol[]);
    void recieve_protocol(void);

    unsigned char calcChecksum(unsigned char *data, int leng);

    ros::Publisher arduino_pub;

    ros::Subscriber arduino_sub;

public:
    void runLoop(void);

    Arduino_mega(ros::NodeHandle &_nh) : nh(_nh)
    {
        initvalue();
        serial_connect();
        initPublisher();
        initSubscriber();
    }

    ~Arduino_mega()
    {
        close(serial_port);
    }
};