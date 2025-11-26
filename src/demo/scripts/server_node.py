#! /usr/bin/python3
# coding = utf-8

import rospy
from demo.srv import addint,addintRequest,addintResponse

def func(request:addintRequest):
    # 解析提交的两个整数信息
    num1 = request.num1 
    num2 = request.num2
    # 逻辑处理
    sum = num1 + num2
    # 将结果封装进response对象中然后响应
    response = addintResponse()
    response.sum = sum
    rospy.loginfo("服务端已经处理好数据num1 = %d,num2 = %d,sum = %d",num1,num2,sum)
    return response



if __name__ == "__main__":
    rospy.init_node("server_node")
    server = rospy.Service(name="addintnums",service_class=addint,handler=func) #handler is callback funciton
    rospy.loginfo("server is booting")
    rospy.spin()
