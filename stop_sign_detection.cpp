#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

/**
 * @file stop_sign_detection.cpp
 * @brief Example of stop sign detection in C++ using OpenCV.
 *
 * This example demonstrates a basic approach to detecting red,
 * octagonal shapes in an image and then integrating it into a
 * simplified map routing scenario.
 */

/**
 * @struct DetectedStopSign
 * @brief Holds information about a detected stop sign in an image.
 */
struct DetectedStopSign {
    cv::Rect boundingBox;     /**< The bounding box of the detected stop sign. */
    std::vector<cv::Point> contour; /**< The contour (set of points) for the detected stop sign. */
};

/**
 * @fn std::vector<DetectedStopSign> detectStopSigns(const cv::Mat &frame)
 * @brief Detect stop signs in a given image frame.
 *
 * @param frame The input image (BGR) in which to detect stop signs.
 * @return A vector of DetectedStopSign objects with bounding boxes and contours.
 *
 * This function uses color thresholding in the HSV space to detect red regions,
 * followed by contour detection and shape approximation to find 8-sided polygons.
 */
std::vector<DetectedStopSign> detectStopSigns(const cv::Mat &frame)
{
    std::vector<DetectedStopSign> detectedSigns;

    // Convert to HSV color space
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Threshold for red color in HSV
    // Note: Red can appear in two HSV ranges; this is simplified.
    cv::Mat mask1, mask2;
    cv::inRange(hsv, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(170, 70, 50), cv::Scalar(180, 255, 255), mask2);

    // Combine the two masks
    cv::Mat mask = mask1 | mask2;

    // Optional: Morphological operations to clean up noise
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &cnt : contours) {
        // Approximate polygon
        std::vector<cv::Point> approx;
        double perimeter = cv::arcLength(cnt, true);
        cv::approxPolyDP(cnt, approx, 0.02 * perimeter, true);

        // Check if polygon is (approximately) octagonal
        if (approx.size() == 8 && cv::contourArea(approx) > 1000) {
            // Create bounding box
            cv::Rect bbox = cv::boundingRect(approx);

            // You can perform additional checks here, such as
            // checking the ratio of the bounding box to ensure
            // it roughly matches a typical stop sign shape.

            DetectedStopSign sign;
            sign.boundingBox = bbox;
            sign.contour = approx;
            detectedSigns.push_back(sign);
        }
    }

    return detectedSigns;
}

/**
 * @struct MapNode
 * @brief Represents a node (e.g., intersection) in a map.
 */
struct MapNode {
    double x;     /**< X coordinate (e.g., longitude or local coordinate). */
    double y;     /**< Y coordinate (e.g., latitude or local coordinate). */
    bool hasStop; /**< Flag indicating if this node has a stop sign. */
};

/**
 * @fn void updateRoutingWithStopSigns(std::vector<MapNode> &route, const std::vector<DetectedStopSign> &signs)
 * @brief Updates the route with information about detected stop signs.
 *
 * @param route The route as a vector of map nodes.
 * @param signs A vector of detected stop signs from the camera or sensor.
 *
 * This function simulates associating detected stop signs with a
 * node in your route (e.g., if the bounding box corresponds to a
 * location near one of the nodes, mark it as having a stop sign).
 */
void updateRoutingWithStopSigns(std::vector<MapNode> &route, const std::vector<DetectedStopSign> &signs)
{
    // For demonstration, let's assume boundingBox.x, boundingBox.y can be mapped
    // to the route's coordinate system. In practice, you'd have more advanced
    // sensor fusion / GPS alignment logic here.
    for (auto &node : route) {
        // Check if any sign bounding box is near the node's (x, y) position
        for (const auto &sign : signs) {
            // In a real scenario, you'd transform the bounding box coordinates
            // to a global or local map reference. Here, we'll do a simplistic check.
            double dx = node.x - static_cast<double>(sign.boundingBox.x);
            double dy = node.y - static_cast<double>(sign.boundingBox.y);
            double distance = std::sqrt(dx * dx + dy * dy);

            // If the sign is "close," mark it
            if (distance < 50.0) {  // Arbitrary threshold for demonstration
                node.hasStop = true;
            }
        }
    }
}

/**
 * @fn int main()
 * @brief Main usage example of stop sign detection integrated with map routing.
 *
 * @return Exit code.
 */
int main()
{
    // Example: Load an image that might contain a stop sign
    cv::Mat frame = cv::imread("stop_sign_sample.jpg");
    if (frame.empty()) {
        std::cerr << "Error loading image. Make sure the image path is correct.\n";
        return -1;
    }

    // Detect stop signs in the image
    std::vector<DetectedStopSign> signs = detectStopSigns(frame);

    // Example route with some nodes (x, y)
    std::vector<MapNode> route = {
        {100.0, 150.0, false},
        {200.0, 250.0, false},
        {300.0, 350.0, false},
        // ...
    };

    // Update our route based on detected stop signs
    updateRoutingWithStopSigns(route, signs);

    // Print out the route info
    std::cout << "Route:\n";
    for (size_t i = 0; i < route.size(); i++) {
        std::cout << " Node " << i << ": (x=" << route[i].x << ", y=" << route[i].y 
                  << "), hasStop=" << (route[i].hasStop ? "true" : "false") << "\n";
    }

    // Optional: visualize the detection results
    for (const auto &sign : signs) {
        cv::rectangle(frame, sign.boundingBox, cv::Scalar(0, 255, 0), 3);
        cv::polylines(frame, sign.contour, true, cv::Scalar(255, 0, 0), 2);
    }
    cv::imshow("Stop Sign Detection", frame);
    cv::waitKey(0);

    return 0;
}
