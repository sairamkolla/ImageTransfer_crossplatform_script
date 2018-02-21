# Import socket module
import sys
import socket               
import numpy as np
import imageio

def GetImage(s,h,w,c):
    buff = ""
    while True:
        temp = s.recv(256)
        if(temp.endswith("EOM")):
            buff+=temp[:-3]
            break
        else:
            buff+=temp
    x = np.frombuffer(buff, dtype=np.uint8)
    print x.shape
    if c:  
        final_img = np.zeros((h,w,c))
        final_img[:, :, 0] = x[2::3].reshape((h, w))
        final_img[:, :, 1] = x[1::3].reshape((h, w))
        final_img[:, :, 2] = x[0::3].reshape((h, w))
    else:
        final_img = x.reshape((h,w))
    return final_img

def start_client(host_ip,portnumber):
    s = socket.socket()
    s.connect((host_ip,portnumber))
    return s

if __name__ == "__main__":
    port = int(sys.argv[1])
    s = start_client('127.0.0.1', port)
    s.send('start')
    my_image = GetImage(s,4000,3000,3) #height,width,channels(0 or 3)
    # for checking
    # imageio.imwrite("yoyoyoyo.jpg",my_image)
    s.send('ok')
    s.close()    
