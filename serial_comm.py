from serial import Serial
import serial.tools.list_ports
import time
import struct

def ser_print(data):
    try:
        print(data.decode(), end='')
    except UnicodeDecodeError:
        print(data, end='')

def identify_esp():
    ports = serial.tools.list_ports.comports()
    for p, _, hwid in sorted(ports):
        # VID and PID of ESP32
        if "VID:PID=10C4:EA60" in hwid:
            return p
        
def send_command(device: Serial, cmd:int, type:int, p1:float, p2:float):
    cmd_b = cmd.to_bytes(1, byteorder='big')
    type_b = type.to_bytes(1, byteorder='big')
    p1_b = struct.pack("f", p1)
    p2_b = struct.pack("f", p2)
    arrr = bytearray(cmd_b + type_b + p1_b + p2_b)
    device.write(arrr)

try:
    with Serial(identify_esp(), 115200) as esp:
        print('wait for ESP to boot...')
        time.sleep(2)
        print('done waiting')
        while esp.in_waiting > 0:
            resp = esp.readline()
            ser_print(resp)
        while True:
            a = input('AXES: ')
            d = float(input('DIST: '))
            if a == "X":
                send_command(esp, 0, 1, d, 0)
            elif a == "Y":
                send_command(esp, 0, 2, d, 0)
            elif a == "Z":
                send_command(esp, 0, 3, d, 0)
            elif a == "H":
                send_command(esp, 1, 1, d, 0)
            else:
                send_command(esp, 0, 1, 50, 0)
            time.sleep(0.05)
            while esp.in_waiting > 0:
                resp = esp.readline()
                ser_print(resp)
except KeyboardInterrupt:
    print("\n\nclosing...")
    