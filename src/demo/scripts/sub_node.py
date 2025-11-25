#! usr/bin/python3
# coding = utf-8

import rospy
from demo.msg import person

def func(p:person): # 这里用（别名：类型）的格式来表达形参
    rospy.loginfo("订阅到的信息;"+"姓名:%s,年龄:%d,体重:%.1f",p.name,p.age,p.height)


if __name__ == "__main__":
    rospy.init_node("sub_node")
    pub = rospy.Subscriber(name="personlistpy",data_class=person,callback=func,queue_size=10)
    rospy.spin() # 循环接收
