#include <ros/ros.h>
#include <demo/addint.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"client_node");
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<demo::addint>("addintnum");
    ros::service::waitForService("addintnum");
    //这里超级关键，demo::addint是一个服务类型的对象，ros对它进行了封装，所以add是一个含有两个成员的对象
    //分别是request和response，可以转到devel/include/demo/addint.h查看
    //定义一个addint类对象,封装了request和response
    demo::addint add;
    add.request.num1 = 100;
    add.request.num2 = 200;
    bool flag = client.call(add);
    if (flag) 
    { 
        ROS_INFO("响应成功");
        ROS_INFO("响应结果：%d",add.response.sum);
    }
    else ROS_INFO("响应失败");


    
    return 0;
}

