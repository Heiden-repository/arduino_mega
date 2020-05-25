#include <ros/ros.h>

#include "arduino_mega/arduino_output_signal.h"

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <time.h>
#include <thread>
#include <mutex>

#define protocol_size 5

#define send_protocol_start 0xFF
#define send_protocol_size 0x05
#define send_protocol_arduino_num_zero 0x00
#define send_protocol_arduino_num_one 0x01
#define send_protocol_low 0x00
#define send_protocol_high 0x01

#define rev_protocol_size_needed_data 4
#define rev_protocol_start 0xFF

#define GPIO_output_pin_two 0x02
#define GPIO_output_pin_three 0x03
#define GPIO_output_pin_four 0x04
#define GPIO_output_pin_five 0x05
#define GPIO_output_pin_six 0x06
#define GPIO_output_pin_seven 0x07
#define GPIO_output_pin_eight 0x08
#define GPIO_output_pin_nine 0x09
#define GPIO_output_pin_ten 0x0A
#define GPIO_output_pin_eleven 0x0B
#define GPIO_output_pin_twelve 0x0C
#define GPIO_output_pin_thirteen 0x0D
#define GPIO_output_pin_twentytwo 0x16
#define GPIO_output_pin_twentythree 0x17
#define GPIO_output_pin_twentyfour 0x18
#define GPIO_output_pin_twentyfive 0x19
#define GPIO_output_pin_twentysix 0x1A
#define GPIO_output_pin_twentyseven 0x1B
#define GPIO_output_pin_twentyeight 0x0C
#define GPIO_output_pin_twentynine 0x0D

#define GPIO_input_pin_thirty 0x1E
#define GPIO_input_pin_thirtyone 0x1F
#define GPIO_input_pin_thirtytwo 0x20
#define GPIO_input_pin_thirtythree 0x21
#define GPIO_input_pin_thirtyfour 0x22
#define GPIO_input_pin_thirtyfive 0x23
#define GPIO_input_pin_thirtysix 0x24
#define GPIO_input_pin_thirtyseven 0x25
#define GPIO_input_pin_thirtyeight 0x26
#define GPIO_input_pin_thirtynine 0x27
#define GPIO_input_pin_fourtyone 0x28
#define GPIO_input_pin_fourtytwo 0x2A
#define GPIO_input_pin_fourtythree 0x2B
#define GPIO_input_pin_fourtyfour 0x2C
#define GPIO_input_pin_fourtyfive 0x2D
#define GPIO_input_pin_fourtysix 0x2E
#define GPIO_input_pin_fourtyseven 0x2F
#define GPIO_input_pin_fourtyeight 0x30
#define GPIO_input_pin_fourtynine 0x31

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
    void send_serial(const arduino_mega::arduino_output_signal::ConstPtr &arduino_mega_msg);
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