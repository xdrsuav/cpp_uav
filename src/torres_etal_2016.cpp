/**
* @file torres_etal_2016.cpp
* @brief Coverage path planner based on M. Torres et al, 2016
* @author Takaki Ueno
*/

// roscpp
#include <ros/ros.h>

// cpp standard libraries
#include <array>
#include <vector>

// geometry_msgs
#include <geometry_msgs/Point.h>

// Service
#include "cpp_uav/Torres16.h"

// header
#include <torres_etal_2016.hpp>

/**
* @brief Plans coverage path
* @param req[in] Contains values neccesary to plan a path
* @param res[out] Contains resulting waypoints
* @return bool
* @details For details of this service, cf. srv/Torres16.srv
*/
bool plan(cpp_uav::Torres16::Request &req, cpp_uav::Torres16::Response &res)
{
    // Initialization
    std::vector<geometry_msgs::Point> polygon_vertices, waypoints;
    geometry_msgs::Point start, end;
    std_msgs::Float64 footprint_length, footprint_width, horizontal_overwrap, vertical_overwrap;

    polygon_vertices = req.polygon_vertices;

    start = req.start;
    end = req.end;

    footprint_length = req.footprint_length;
    footprint_width = req.footprint_width;
    horizontal_overwrap = req.horizontal_overwrap;
    vertical_overwrap = req.vertical_overwrap;

    direction dir = sweep_direction(polygon_vertices);

    geometry_msgs::Point start_pt, mid_pt, end_pt;

    //start_pt.x = (dir.base_edge.back().x-dir.base_edge.front().x)/2;
    //start_pt.y = (dir.base_edge.back().y-dir.base_edge.front().y)/2;

    start_pt = dir.base_edge.front();
    mid_pt = dir.base_edge.back();
    end_pt = dir.opposed_vertex;

    waypoints.push_back(start_pt);
    waypoints.push_back(mid_pt);
    waypoints.push_back(end_pt);

    ROS_INFO("Start x: %f", start_pt.x);
    ROS_INFO("start y: %f", start_pt.y);
    ROS_INFO("mid x: %f", mid_pt.x);
    ROS_INFO("mid y: %f", mid_pt.y);
    ROS_INFO("end x: %f", end_pt.x);
    ROS_INFO("end y: %f", end_pt.y);

    res.waypoints = waypoints;

    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "torres_etal_2016");
    ros::NodeHandle nh;

    ros::ServiceServer planner = nh.advertiseService("cpp_torres16", plan);
    ROS_INFO("Ready to plan.");

    ros::spin();

    return 0;
}