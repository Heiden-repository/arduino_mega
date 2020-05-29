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
    pinMode(i,INPUT_PULLUP);
    input_pin_num[i-30] = 0;
  }
}

void loop() {
  Serial.flush();
  
  for(int i = 30; i <=49; i++)
  {
     int value = 0;
     value = digitalRead(i);
     if(value != input_pin_num[i-30])
     {
        unsigned char send_protocol[protocol_size];
        memset(send_protocol,0,protocol_size);
        send_protocol[0] = 0xFF;
        send_protocol[1] = (unsigned char)protocol_size;
        send_protocol[2] = (unsigned char)i;
        send_protocol[3] = (unsigned char)value;
        send_protocol[4] = calcChecksum(send_protocol,protocol_size);

        Serial.write(send_protocol,protocol_size);
     }
  }

  if(Serial.available() > 0)
  {
    rev_serial();
  }
  delay(100);
}

unsigned char calcChecksum(unsigned char *data, int leng)
{
    unsigned char csum;

    csum = 0xFF;
    for (; leng > 0; leng--)
        csum += *data++;

    return ~csum;
}

void rev_serial()
{
  Serial.println("serial_event");
  unsigned char rev_protocol[protocol_size];
  memset(rev_protocol,0,protocol_size);
  Serial.readBytes(rev_protocol,protocol_size);

  int pin_num  = rev_protocol[2];
  int high_low = rev_protocol[3];
  Serial.println(pin_num);
  Serial.println(high_low);
  digitalWrite(pin_num,high_low);
}
