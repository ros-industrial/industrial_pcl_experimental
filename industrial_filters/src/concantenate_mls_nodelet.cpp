/*
 * Copyright 2013 Southwest Research Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * concantenate_mls_nodelet.cpp
 *
 *
 *  Created on: Jan 31, 2013
 *      Author: cgomez
 *
 */

#include <pluginlib/class_list_macros.h>
#include "concantenate_mls_nodelet.h"

//////////////////////////////////////////////////////////////////////////////////////////////
bool
industrial_filters_nodelets::ConcantenateMLS::child_init (ros::NodeHandle &nh, bool &has_service)
{
  // Enable the dynamic reconfigure service
  has_service = true;
  srv_ = boost::make_shared <dynamic_reconfigure::Server<pcl_ros::FilterConfig> > (nh);
  dynamic_reconfigure::Server<pcl_ros::FilterConfig>::CallbackType f = boost::bind (&industrial_filters_nodelets::ConcantenateMLS::config_callback, this, _1, _2);
  srv_->setCallback (f);

  return (true);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void
industrial_filters_nodelets::ConcantenateMLS::config_callback (pcl_ros::FilterConfig &config, uint32_t level)
{
  boost::mutex::scoped_lock lock (mutex_);

  double filter_min, filter_max;
  impl_.getFilterLimits (filter_min, filter_max);

  // Check the current values for filter min-max
  if (filter_min != config.filter_limit_min)
  {
    filter_min = config.filter_limit_min;
    NODELET_DEBUG ("[%s::config_callback] Setting the minimum filtering value a point will be considered from to: %f.", getName ().c_str (), filter_min);
    // Set the filter min-max if different
    impl_.setFilterLimits (filter_min, filter_max);
  }
  // Check the current values for filter min-max
  if (filter_max != config.filter_limit_max)
  {
    filter_max = config.filter_limit_max;
    NODELET_DEBUG ("[%s::config_callback] Setting the maximum filtering value a point will be considered from to: %f.", getName ().c_str (), filter_max);
    // Set the filter min-max if different
    impl_.setFilterLimits (filter_min, filter_max);
  }

  // Check the current value for the filter field
  //std::string filter_field = impl_.getFilterFieldName ();
  if (impl_.getFilterFieldName () != config.filter_field_name)
  {
    // Set the filter field if different
    impl_.setFilterFieldName (config.filter_field_name);
    NODELET_DEBUG ("[%s::config_callback] Setting the filter field name to: %s.", getName ().c_str (), config.filter_field_name.c_str ());
  }

  // Check the current value for keep_organized
  if (impl_.getKeepOrganized () != config.keep_organized)
  {
    NODELET_DEBUG ("[%s::config_callback] Setting the filter keep_organized value to: %s.", getName ().c_str (), config.keep_organized ? "true" : "false");
    // Call the virtual method in the child
    impl_.setKeepOrganized (config.keep_organized);
  }

  // Check the current value for the negative flag
  if (impl_.getFilterLimitNegative () != config.filter_limit_negative)
  {
    NODELET_DEBUG ("[%s::config_callback] Setting the filter negative flag to: %s.", getName ().c_str (), config.filter_limit_negative ? "true" : "false");
    // Call the virtual method in the child
    impl_.setFilterLimitNegative (config.filter_limit_negative);
  }

  // The following parameters are updated automatically for all PCL_ROS Nodelet Filters as they are inexistent in PCL
  if (tf_input_frame_ != config.input_frame)
  {
    tf_input_frame_ = config.input_frame;
    NODELET_DEBUG ("[%s::config_callback] Setting the input TF frame to: %s.", getName ().c_str (), tf_input_frame_.c_str ());
  }
  if (tf_output_frame_ != config.output_frame)
  {
    tf_output_frame_ = config.output_frame;
    NODELET_DEBUG ("[%s::config_callback] Setting the output TF frame to: %s.", getName ().c_str (), tf_output_frame_.c_str ());
  }
}

typedef industrial_filters_nodelets::ConcantenateMLS ConcantenateMLS;
PLUGINLIB_DECLARE_CLASS (industrial_filters, ConcantenateMLS, ConcantenateMLS, nodelet::Nodelet);

