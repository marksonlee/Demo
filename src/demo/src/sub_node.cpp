#include <ros/ros.h>
#include <demo/person.h>

void func(const demo::person::ConstPtr &p)
{
    // ros_info函数中%s是c语言的输入输出流，此时p->name是string类对象，需要转为c格式流输出
    ROS_INFO("订阅到的信息是:%s,%d,%.2f",p->name.c_str(),p->age,p->height);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("订阅方实现");
    // 1.初始化订阅方节点
    ros::init(argc,argv,"sub_node");
    // 2.创建master句柄管理节点
    ros::NodeHandle nh;
    // 3.定义订阅者对象sub , 需要注意回调函数
    ros::Subscriber sub = nh.subscribe<demo::person>("personlist",10,func);
    // 4.spin（）循环接收
    ros::spin();
    return 0;
}
