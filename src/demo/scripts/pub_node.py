#! usr/bin/python3
# coding=utf-8

# 1. 先设置默认python3路径

# 2.导入必要的模块
import rospy
from demo.msg import person 
"""
    2025.11.24:
    尝试理解这个结构:demo代表的是包名,然后demo下有个msg文件夹,所以msg是demo下的一个成员,故用.来表示msg文件夹
    person.msg是自己写的消息类型,所以需要将person这个消息类型导入,完成一次import


"""

if __name__ == "__main__":

    # 3.初始化发布者节点
    rospy.init_node("pub_node") # 发布者节点的名称

    # 4.设立发布者对象
    pub = rospy.Publisher("personlistpy",person,queue_size=10) # 发布话题名称，发布的消息类型，缓冲队列长度

    # 5.设置person数据类型对象
    data = person()

    # 6.为消息内容赋值，然后才能发送
    data.name = "Liming"
    data.age = 22
    data.height = 65.0

    # 7.创建频率Rate对象
    rate = rospy.Rate(10)

    # 8.不断发布消息
    while not rospy.is_shutdown():
        pub.publish(data) #发布者对象.发布(内容)
        rospy.loginfo("姓名：%s,年龄：%d,体重：%.1f",data.name,data.age,data.height)
        rate.sleep()



