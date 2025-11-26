#include <ros/ros.h>
#include <demo/addint.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    // 1.初始化节点，这个节点名称在ros运行的时候必须唯一
    ros::init(argc,argv,"client_node");
    ros::NodeHandle nh;
    // 2.创建了一个服务的客户端，这个serviceClient函数有多个重载，这里选择发起“addintnum”服务
    ros::ServiceClient client = nh.serviceClient<demo::addint>("addintnum");
    // 3.服务等待，防止后面的client.call失败
    ros::service::waitForService("addintnum");
    //这里超级关键，demo::addint是一个服务类型的对象，ros对它进行了封装，所以add是一个含有两个成员的对象
    //分别是request和response，可以转到devel/include/demo/addint.h查看
    //定义一个addint类对象,封装了request和response
    demo::addint add;
    add.request.num1 = 100;
    add.request.num2 = 200;

    /*
        发起服务调用过程：
        1.将服务对象add（包含request）发送给server
        2.server端处理后，将结果写回给add对象中的response，即add.response
        3.若1，2无异常的完成，则返回true，反之则返回false
    */
    bool flag = client.call(add);

    if (flag) 
    { 
        ROS_INFO("响应成功");
        ROS_INFO("响应结果：%d",add.response.sum);
    }
    else ROS_INFO("响应失败");


    
    return 0;
}

