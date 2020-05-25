#define protocol_size 5
#define send_protocol_start 0xFF
#define rev_protocol_start 0xFF

int8_t input_pin_num[20];

void setup() {
  Serial.begin(9600);
  
  for(int i = 2;i<=13;i++)
  {
    pinMode(i,OUTPUT);
  }
  for(int i = 22;i<=29;i++)
  {
    pinMode(i,OUTPUT);
  }
  for(int i=30; i<=49;i++)
  {
    pinMode(i,INPUT);
    input_pin_num[i-30] = 0;
  }
}

void loop() {
  unsigned char send_protocol[protocol_size];
  for(int i = 30; i <=49; i++)
  {
     int value = 0;
     value = digitalRead(i);
     if(value != input_pin_num[i-30])
     {
        send_protocol[0] = 0xFF;
        send_protocol[1] = protocol_size;
        send_protocol[2] = (unsigned char)i;
        send_protocol[3] = (unsigned char)value;
        send_protocol[4] = calcChecksum(send_protocol,protocol_size);

        send_serial_protocol(send_protocol);
     }
  }
}

unsigned char calcChecksum(unsigned char *data, int leng)
{
    unsigned char csum;

    csum = 0xFF;
    for (; leng > 0; leng--)
        csum += *data++;

    return ~csum;
}

void send_serial_protocol(unsigned char send_protocol[])
{
    Serial.write(send_protocol,protocol_size);
}

void serial_Event()
{
  unsigned char rev_start_protocol;
  unsigned char rev_protocol[protocol_size-1];
  rev_start_protocol = Serial.read();
  
  if(rev_start_protocol == 0xFF)
  {
    Serial.readBytes(rev_protocol,protocol_size-1);

    int pin_num  = rev_protocol[1];
    int high_low = rev_protocol[2];

    digitalWrite(pin_num,high_low);
  }
}
