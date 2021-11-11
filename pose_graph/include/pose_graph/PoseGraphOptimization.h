#pragma once

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <std_srvs/Trigger.h>

#include <map>
#include <memory>
#include <mutex>

#include "pose_graph/GlobalMapping.h"
#include "pose_graph/KFMatcher.h"
#include "pose_graph/LoopClosing.h"
#include "pose_graph/Parameters.h"
#include "pose_graph/Publisher.h"
#include "pose_graph/Subscriber.h"
#include "utility/CameraPoseVisualization.h"

class PoseGraphOptimization {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  PoseGraphOptimization();
  ~PoseGraphOptimization() = default;

  void setup();
  void run();

  void updatePublishGlobalMap(const ros::TimerEvent& event);
  void updateGlobalMap();
  void getGlobalPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr& pointcloud);  // NOLINT (already pointer)
  bool savePointCloud(std_srvs::TriggerRequest& request, std_srvs::TriggerResponse& response);  // NOLINT
  Publisher publisher;

  // ros::Publisher pubSparseMap;

 private:
  ros::NodeHandle nh_private_;

  std::shared_ptr<Parameters> params_;
  std::unique_ptr<Subscriber> subscriber_;
  std::unique_ptr<LoopClosing> loop_closing_;
  std::unique_ptr<CameraPoseVisualization> camera_pose_visualizer_;
  std::unique_ptr<GlobalMap> global_map_;
  std::map<int, KFMatcher*> kfMapper_;  // Mapping between kf_index and KFMatcher*; to make KFcounter

  std::mutex processMutex_;
  int frame_index_;
  int sequence_;
  // TODO(bjoshi): these varibales are bullshit. Keep for now
  int SKIP_CNT;
  int skip_cnt;
  double SKIP_DIS;
  Eigen::Vector3d last_translation_;

  BriefVocabulary* voc_;

  ros::Timer timer_;  // for periodic publishing
  ros::ServiceServer save_pointcloud_service_;
};