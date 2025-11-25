#include <ros/ros.h>
#include <std_msgs/String.h>
#include <demo/person.h>
#include <random>
#include <cstdlib>
#include <sstream>
#include <cstdint>

struct persondata{
    std::string name;
    int32_t age;
    _Float32 height;
};


int randomnums() {
    // 创建随机设备（用于种子）
    std::random_device rd;
    // 使用 Mersenne Twister 算法（高质量）
    std::mt19937 gen(rd());
    // 定义均匀分布：[1, 65535]
    std::uniform_int_distribution<int> dis(1, 65535);

    int nums = dis(gen);
    std::cout << "Random number: " << nums << std::endl;

    return nums;
}


int main(int argc, char *argv[])
{
    // 1.设置程序本地化指令
    setlocale(LC_ALL,"");

    // 2.设置个人信息vector
    std::vector <persondata> people;
    people.push_back({"Alice",25,165.5f});
    people.push_back({"Bob",30,180.0f});
    people.push_back({"Cherry",22,168.5f});
    people.push_back({"Debian",28,178.0f});

    // 3.初始化节点，名称为publisher
    ros::init(argc,argv,"publisher");

    // 4.创建master句柄，管理节点
    ros::NodeHandle nh;

    // 5.创建发布者对象，用pub来记录；发布的话题是personlist
    ros::Publisher pub = nh.advertise<demo::person>("personlist",10);

    // 6.设置循环频率为10Hz
    ros::Rate rate(10);

    // 7.定义一个消息对象
    demo::person msg;

    // 8.设置循环条件，发布信息 int nums,count,index = 0 这个写法是未定义的，只能设置index=0，其他都是具有未定义风险的
    int nums = 0,count =0,index = 0; 
    // 设置随机数
    nums = randomnums();
    // ROS_INFO("初始的count = %d",count);
    while (ros::ok())
    {
        msg.name = people[index].name;
        msg.age = people[index].age;
        msg.height = people[index].height;
        // 设置index数值，保证循环发送数据
        index = (index+1)%(people.size());
        ROS_INFO_STREAM("发布的消息是: "<<msg.name<<" "<<msg.age<<" "<<msg.height<<" "<<"randomnums = "<<nums);
        // 将获取到的信息发布到话题中
        pub.publish(msg);
        if (nums%2==1) nums = 3*nums + 1;
        else nums = nums/2;
        if (nums == 2) 
        {
            ROS_INFO_STREAM("发布的消息是: "<<msg.name<<" "<<msg.age<<" "<<msg.height<<" "<<"randomnums = "<<nums);
            ROS_INFO("执行次数为：%d",count);
            break;
        }
        count += 1;
        //设置频率
        rate.sleep();
        ros::spinOnce();

    }
    return 0;
}
