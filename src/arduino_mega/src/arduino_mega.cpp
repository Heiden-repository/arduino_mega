#include "arduino_mega/arduino_mega.hpp"

void Arduino_mega::initvalue(void)
{
    serial_port = 0;
    arduino_num_for_send = 0;
    pin_num_for_send = 0;
    digital_signal_for_send = 0;

    rev_arduino_num = 0;
    rev_arduino_pin_num = 0;
    rev_arduino_status = 0;

}

void Arduino_mega::initPublisher(void)
{
}

void Arduino_mega::initSubscriber(void)
{
    arduino_sub = nh.subscribe("/arduino_signal", 10, &Arduino_mega::send_serial, this);
}

bool Arduino_mega::serial_connect(void)
{
    serial_port = 0;

    while (ros::ok())
    {
        serial_port = open("/dev/ttyACM1", O_RDWR | O_NOCTTY);
        if (serial_port < 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            printf("Error %i from open: %s\n", errno, strerror(errno));
        }
        else
            break;
    }

    struct termios termi;

    memset( &termi, 0, sizeof(termi) );

    termi.c_cflag = B9600;
    termi.c_cflag |= CS8;
    termi.c_cflag |= CLOCAL;
    termi.c_cflag |= CREAD;
    termi.c_iflag = 0;
    termi.c_oflag = 0;
    termi.c_lflag = 0;
    termi.c_cc[VTIME] = 0;
    termi.c_cc[VMIN] = 0;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &termi);
    printf("Arduino_uno connection\n");
}

void Arduino_mega::send_serial(const arduino_mega::arduino_output_signal::ConstPtr &arduino_mega_msg)
{
    unsigned char send_serial_protocol[protocol_size];
    memset(send_serial_protocol, 0, protocol_size);

    int pin_num = arduino_mega_msg->pin_num;
    int high_low = arduino_mega_msg->high_low;

    send_serial_protocol[0] = send_protocol_start;
    send_serial_protocol[1] = send_protocol_size;
    send_serial_protocol[2] = (unsigned char)pin_num;
    send_serial_protocol[3] = (unsigned char)high_low;
    send_serial_protocol[4] = calcChecksum(send_serial_protocol,protocol_size);

    send_protocol(send_serial_protocol);

    return;
}

bool Arduino_mega::send_protocol(unsigned char send_serial_protocol[])
{
    int write_data = -1;
    while (1)
    {
        write_data = write(serial_port, send_serial_protocol, protocol_size);
        if (write_data > 0)
        {
            return 1;
        }
    }
    return 0;
}

unsigned char Arduino_mega::calcChecksum(unsigned char *data, int leng)
{
    unsigned char csum;

    csum = 0xFF;
    for (; leng > 0; leng--)
        csum += *data++;

    return ~csum;
}

void Arduino_mega::recieve_protocol(void)
{
    int read_data = -1;
    unsigned char start_buf[1] = {0};
    unsigned char data_buf[rev_protocol_size_needed_data];
    memset(data_buf, 0, rev_protocol_size_needed_data);
    read_data = read(serial_port, start_buf, 1);

    if (read_data < 0)
    {
        printf("receive_data error \n");
        return;
    }

    if (start_buf[0] == rev_protocol_start)
    {
        read_data = read(serial_port, data_buf, rev_protocol_size_needed_data);

        rev_arduino_num = (int)data_buf[1];
        rev_arduino_pin_num = (int)data_buf[2];
        rev_arduino_status = (int)data_buf[3];
    }
    else
        return;
}

void Arduino_mega::runLoop(void)
{
    ros::Rate r(10);

    while (ros::ok())
    {

        ros::spinOnce();
        r.sleep();
    }
}