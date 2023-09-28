import time
import serial
import random

ser = serial.Serial(
    port='/dev/ttyUSB7',                              # !!!write the port name
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)

if ser.isOpen():

    while True:
        time.sleep(2)                                    # delay 2 seconds
        random_value = random.uniform(90.00, 110.00)     # range of random values ​​generated
        formatted_random_value = "%.2f" % random_value
        formatted_random_value = float(formatted_random_value)

        if formatted_random_value > 99.99:
            formatted_random_value = (format(formatted_random_value,".2f"))
            str_to_send = "S    " + str(formatted_random_value) + " g\r\n"    # generate the send string
        else:
            formatted_random_value = (format(formatted_random_value,".2f"))
            str_to_send = "S     " + str(formatted_random_value) + " g\r\n"   # generate the send string

        print(str_to_send)                                                    # print the value on the terminal
        ser.write(str_to_send)

else:
    print("No connection")