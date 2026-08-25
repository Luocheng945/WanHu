import socket
import time
import pygame

def get_controller(): #try to connect to controller
    while True:
        pygame.event.pump()
        if pygame.joystick.get_count() > 0:
            controller = pygame.joystick.Joystick(0)
            print("Joystick found")
            return controller
        print("No joystick found")
        time.sleep(1)

UDP_IP = "10.3.18.2" #reciever ip
UDP_PORT = 50000
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP
pygame.init() #initialize pygame
pygame.joystick.init()
js = get_controller()
js.init()

direction = 0
thor = 0
steering = 90

while True:
    try:
        if pygame.joystick.get_count() == 0: #if no controller found, sent default command and try to reconnect
            print("No joystick found")
            sock.sendto(bytes([90,0,0]), (UDP_IP, UDP_PORT))
            js = get_controller()
            js.init()
            continue

        pygame.event.pump()
        throttle = js.get_axis(1)
        if throttle < -0.05 or throttle > 0.05: #dead zone
            if throttle < 0: #value from -1 - 1, if negative forward
                direction = 0
                thor = int(50 + abs(throttle) * 150)
            else: #if positive backward
                direction = 1
                thor = int(50 + abs(throttle) * 150)
        else:
            thor = 0
            direction = 0

        if  js.get_axis(3) < -0.05: #0.05 dead zone, and if negative turn left
            steering = int(90 - abs(js.get_axis(3)) * 40)
        elif js.get_axis(3) > 0.05: #if positive turn right
            steering = int(90 + js.get_axis(3) * 40)
        else:
            steering = 90

        message = bytes([steering,direction,thor]) #3 bytes message

        sock.sendto(message, (UDP_IP, UDP_PORT)) #send udp message
        time.sleep(0.02)
    except pygame.error as error: #if any pygame error, reconnect controller
        print("error")
        js = get_controller()