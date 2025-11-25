#include <ros/ros.h>
// 1. 导入服务数据类型格式
#include <demo/addint.h>

bool func(demo::addint::Request &request,demo::addint::Response &response)
{
    //这里对象的成员是被定义过的，是存在的，代码风格问题
    int num1 = request.num1;
    int num2 = request.num2;
    ROS_INFO("接收到的请求数据为：%d,%d",num1,num2);
    if (num1<0 || num2<0) return false;
    // 已经处理完数据了
    response.sum = num1+num2;
    ROS_INFO("求和结果sum为: %d",response.sum);
    
    return true;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    // 2.初始化ros节点
    ros::init(argc,argv,"server_node");
    // 3.创建master句柄
    ros::NodeHandle nh;
    // 4.创建服务端对象 advertise：发布 ； service ：服务
    /*
        该函数2个参数
    service：(const std::string &) 指的是处理服务的名称
    srv_func: (bool) 回调函数指针，意在告诉ros处理成功，中间可以写入核心的处理请求操作

    */
    ros::ServiceServer server = nh.advertiseService("addintnum",func);
    ROS_INFO("ROS Server is booting");
    ros::spin();
    return 0;
}
