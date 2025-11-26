#! /usr/bin/python3
# coding = utf=8

import rospy
from demo.srv import addint,addintResponse,addintRequest

if __name__ == "__main__":
    rospy.init_node(name="client_node")
    client = rospy.ServiceProxy("addintnums",addint)
    # rospy.wait_for_service("addintnums",timeout=15) 两种写法，rospy的话就要指定服务名称，client下的等待服务则不需要
    client.wait_for_service(timeout=15)
    # response 接收服务器端的响应 response应该是addintRequest类
    resp = client.call(12,34)
    # 下面就可以处理服务器端发回来的响应了
    rospy.loginfo("接收回服务器端发送回的响应sum是: %d",resp.sum)

    