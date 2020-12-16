from flask import Flask
import app_config
from flask_restful import Api
import threading
import socket
import unlock
import time



class TCPListen(threading.Thread):
    def __init__(self, port, client=None):
        threading.Thread.__init__(self)
        self.port = port
        self.client = client

    def run(self):
        if self.client is None:
            s = socket.socket()  # 创建 socket 对象
            host = '0.0.0.0'  # 获取本地主机名
            port = self.port  # 设置端口
            s.bind((host, port))  # 绑定端口
            s.listen(5)  # 等待客户端连接
            while True:
                c, addr = s.accept()  # 建立客户端连接
                # 创建新线程
                if unlock.Now_client is not None:
                    unlock.Now_client.close()
                c.setblocking(False)
                unlock.SetClient(c)
                localtime = time.asctime(time.localtime(time.time()))
                print(f'{localtime}: 门锁上线')
                TCPListen(app_config.Port, c).start()
        else:
            # 等待返回数据
            localtime = time.asctime(time.localtime(time.time()))
            print(f'{localtime}: 等待门锁发送数据')
            while True:
                try:
                    time.sleep(130)
                    by = self.client.recv(1024)
                    localtime = time.asctime(time.localtime(time.time()))
                    print(f'{localtime}: 发送数据{by}')
                except BaseException as e:
                    localtime = time.asctime(time.localtime(time.time()))
                    print(f'{localtime}: 等待接收数据出错，关闭连接，原因:{str(e)}')
                    if unlock.Now_client == self.client:
                        unlock.SetClient(None)
                    self.client.close()
                    return


app = Flask(__name__)
app.config.from_object(app_config)
api = Api(app)
api.add_resource(unlock.Unlock, '/api/unlock')
TCPListen(app_config.Port).start()
