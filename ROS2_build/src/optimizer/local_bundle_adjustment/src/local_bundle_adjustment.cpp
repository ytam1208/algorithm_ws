#include <rclcpp/rclcpp.hpp>

// AprilTag LBA Example (Ceres Solver)
// - One fixed AprilTag in world frame
// - Simulated observations from different camera viewpoints
// - Optimize camera poses to reduce reprojection error

#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <iostream>
#include <vector>
#include <array>
#include <random>

// Fixed camera intrinsics (pinhole model)
constexpr double fx = 500.0, fy = 500.0, cx = 320.0, cy = 240.0;

// Observation residual: camera pose -> reproject fixed tag -> minimize pixel error
struct ReprojectionError {
  ReprojectionError(double u, double v, const std::array<double, 3>& tag_pos)
    : u_(u), v_(v), tag_(tag_pos) {}

  template <typename T>
  bool operator()(const T* const cam_pose, T* residuals) const {
    // cam_pose = [x, y, z, yaw (rotation around Z)]
    T dx = T(tag_[0]) - cam_pose[0];
    T dy = T(tag_[1]) - cam_pose[1];
    T dz = T(tag_[2]) - cam_pose[2];

    T yaw = cam_pose[3];
    T cosa = ceres::cos(yaw);
    T sina = ceres::sin(yaw);

    // Rotate into camera frame (2D yaw only)
    T x_cam = cosa * dx + sina * dy;
    T y_cam = -sina * dx + cosa * dy;
    T z_cam = dz;

    T u_proj = T(fx) * x_cam / z_cam + T(cx);
    T v_proj = T(fy) * y_cam / z_cam + T(cy);

    residuals[0] = u_proj - T(u_);
    residuals[1] = v_proj - T(v_);
    return true;
  }

  static ceres::CostFunction* Create(double u, double v, const std::array<double, 3>& tag_pos) {
    return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 4>(
      new ReprojectionError(u, v, tag_pos)));
  }

  double u_, v_;
  std::array<double, 3> tag_;
};

int main() {
  // Ground truth Tag position in world
  std::array<double, 3> tag_pos = {1.0, 1.0, 5.0};

  // Simulate 5 camera poses: [x, y, z, yaw]
  std::vector<std::array<double, 4>> cam_poses_gt;
  std::vector<std::array<double, 2>> observations;

  int num_views = 5;
  double radius = 1.0;
  std::default_random_engine rng;
  std::normal_distribution<double> noise(0.0, 0.01);  // pixel noise

  for (int i = 0; i < num_views; ++i) {
    double angle = -M_PI/6 + i * (M_PI/12);  // -30 to +30 deg
    double cam_x = tag_pos[0] + radius * std::cos(angle);
    double cam_y = tag_pos[1] + radius * std::sin(angle);
    double cam_z = 1.0;
    double yaw = angle;

    cam_poses_gt.push_back({cam_x, cam_y, cam_z, yaw});

    // Project the tag to pixel coordinates
    double dx = tag_pos[0] - cam_x;
    double dy = tag_pos[1] - cam_y;
    double dz = tag_pos[2] - cam_z;
    double x_cam =  std::cos(yaw) * dx + std::sin(yaw) * dy;
    double y_cam = -std::sin(yaw) * dx + std::cos(yaw) * dy;
    double u = fx * (x_cam / dz) + cx + noise(rng);
    double v = fy * (y_cam / dz) + cy + noise(rng);

    observations.push_back({u, v});
  }
  std::cout << "Ground Truth Camera Poses:\n";
  for (auto& pose : cam_poses_gt) {
    std::cout << "[x y z yaw] = [" << pose[0] << ", " << pose[1] << ", "
              << pose[2] << ", " << pose[3] << "]\n";
  }

  // Initial estimates (add noise to gt)
  std::vector<std::array<double, 4>> cam_poses_est = cam_poses_gt;
  for (auto& pose : cam_poses_est) {
    pose[0] += 0.1 * noise(rng);
    pose[1] += 0.1 * noise(rng);
    pose[3] += 0.05 * noise(rng); // yaw
  }
  std::cout << "Observations:\n";
  for (auto& pose : cam_poses_est) {
    std::cout << "[x y z yaw] = [" << pose[0] << ", " << pose[1] << ", "
              << pose[2] << ", " << pose[3] << "]\n";
  }

  ceres::Problem problem;
  for (size_t i = 0; i < cam_poses_est.size(); ++i) {
    ceres::CostFunction* cost_function = ReprojectionError::Create(
      observations[i][0], observations[i][1], tag_pos);
    problem.AddResidualBlock(cost_function, nullptr, cam_poses_est[i].data());
  }

  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;

  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);

  std::cout << "\nOptimized Camera Poses:\n";
  for (const auto& pose : cam_poses_est) {
    std::cout << "[x y z yaw] = [" << pose[0] << ", " << pose[1] << ", "
              << pose[2] << ", " << pose[3] << "]\n";
  }

  return 0;
}
