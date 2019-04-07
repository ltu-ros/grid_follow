#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/UInt32.h>

// Includes for dynamic reconfigure
#include <dynamic_reconfigure/server.h>
#include <grid_follow/GridFollowConfig.h>

#include <nav_msgs/OccupancyGrid.h>


class GridFollow
{
public:
    GridFollow();
    void configCallback(grid_follow::GridFollowConfig &config, uint32_t level);
    void gridCallback(const nav_msgs::OccupancyGrid& grid);

private:

    ros::NodeHandle nh_;
    ros::Publisher pub_;
    ros::Subscriber grid_sub_;

    dynamic_reconfigure::Server<grid_follow::GridFollowConfig> server_;
    grid_follow::GridFollowConfig config_;

    int dist_;
};

GridFollow::GridFollow() : nh_{"~"}
{
    grid_sub_  = nh_.subscribe("/birds_eye/grid", 10, &GridFollow::gridCallback, this);

    // Publish on the twist command topic
    pub_ = nh_.advertise<geometry_msgs::Twist>("/prizm/twist_controller/twist_cmd", 10);

    // Dynamic Reconfigure
    server_.setCallback(boost::bind(&GridFollow::configCallback, this, _1, _2));

    // Load defaults
    server_.getConfigDefault(config_);

    dist_ = 1023;
}

void GridFollow::configCallback(grid_follow::GridFollowConfig &config, uint32_t level)
{
    config_ = config;
}

void GridFollow::gridCallback(const nav_msgs::OccupancyGrid& grid)
{
    int width = grid.info.width;
    int height = grid.info.height;

    int origin_x = grid.info.origin.position.x;
    int origin_y = grid.info.origin.position.y;

    /*
       0,0    y
         +-------+
       x |       |
         |       |
         +-------+

    */

    /***** YOUR CODE HERE *****/

    int count = 0;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            // Convert x,y to array index
            // grid.data is 1D so we use the following to convert a 2D index to 1D
            int index = x + y * width;

            // Non-zero = occupied
            if (grid.data[index] != 0)
            {
                count++;
            }
        }
    }

    /**************************/
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "grid_follow");

    GridFollow sd{};

    ROS_INFO_STREAM("grid_follow running!");
    ros::spin();
    return 0;
}
