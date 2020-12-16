from flask_restful import Resource, reqparse, abort
import app_config
import threading
import time

Now_client = None


def SetClient(C):
    global Now_client
    Now_client = C


class Lock(threading.Thread):
    def run(self):
        global Now_client
        time.sleep(app_config.Lock_time)
        try:
            Now_client.send('7'.encode())
        except:
            AbandonClient = Now_client
            Now_client = None
            AbandonClient.close()
        return


class Unlock(Resource):
    def __init__(self):
        return

    def post(self):
        global Now_client
        parser = reqparse.RequestParser()
        parser.add_argument('data', type=str, required=True)
        args = parser.parse_args()
        if args['data'] != app_config.Token:
            abort(403, msg="Token错误!")
        if Now_client is None:
            abort(404, msg="门锁不在线!")
        try:
            Now_client.send('6'.encode())
        except:
            AbandonClient = Now_client
            Now_client = None
            AbandonClient.close()
            abort(500, msg='向门锁发送数据出错!')
        Lock().start()
        return {'msg': '开锁成功!'}
