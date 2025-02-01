# Stop Sign Detection and Map Routing

This project provides a **basic demonstration** of detecting stop signs in images using color segmentation and shape approximation with OpenCV, and then integrating those detections into a **simple map routing** scenario.

> **Disclaimer:**  
> This approach is primarily educational. Real-world applications require more robust methods (e.g., machine learning, advanced feature detection, sensor fusion).

---

## Features

- **Stop Sign Detection**:
  
  - Converts images from BGR to HSV for easier color thresholding.
  - Uses red color masks to isolate stop signs.
  - Finds contours and approximates polygons to identify octagonal shapes.
- **Map Routing Integration**:
  
  - Associates detected stop signs with map nodes via proximity checks.
  - Example route data structure sets a flag (`hasStop`) for any node near a detected stop sign.

---

## Requirements

1. **OpenCV** (tested with OpenCV 4.x, but other versions may work):
  - Core, Imgproc, Highgui modules.
2. A **C++17** (or later) compatible compiler (GCC, Clang, MSVC, etc.).

---

## Building the Project

Below is an example of how to build using **CMake**. If you are new to CMake, install it from your package manager (Linux) or from [the official website](https://cmake.org/).

1. **Clone or download** this repository:
  
  `git clone https://github.com/example/stop_sign_detection.git
  cd stop_sign_detection`
  
2. **Configure** the project with CMake:
  
  `mkdir build
  cd build
  cmake ..`
  
  Make sure that CMake finds your OpenCV installation. You may need to set `OpenCV_DIR` if it is installed in a non-standard location.
  
3. **Build** the project:
  
  `cmake --build .`
  
  This generates an executable (e.g., `stop_sign_detection` or `stop_sign_detection.exe` on Windows).
  

---

## Usage

1. **Prepare an image** (`stop_sign_sample.jpg`) that has a visible stop sign.
  
2. **Run the built executable**:
  
  `./stop_sign_detection`
  
  or on Windows:
  
  `stop_sign_detection.exe`
  
3. The program:
  
  - Loads the image.
  - Detects stop signs by color and shape (octagon).
  - Creates a sample route and marks any route node that is close to a detected sign (`hasStop = true`).
  - Prints route information to the console and opens a window displaying bounding boxes around any detected stop sign.
4. **Press any key** when the image window is in focus to exit the program.
  

---

## File Structure

```
stop_sign_detection/
  ├── CMakeLists.txt
  ├── stop_sign_detection.cpp      # Main source code with detection and routing logic
  ├── stop_sign_sample.jpg         # Example image (add your own if needed)
  └── README.md                    # This file
```

---

## Customization

- **Color Thresholding**:  
  The code uses two HSV ranges to detect red:
  
  `cv::inRange(hsv, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), mask1);
  cv::inRange(hsv, cv::Scalar(170, 70, 50), cv::Scalar(180, 255, 255), mask2);`
  
  Adjust these ranges for improved detection under different lighting conditions.
  
- **Contour Size**:  
  The code discards very small contours. Update this line in `detectStopSigns` as needed:
  
  `if (approx.size() == 8 && cv::contourArea(approx) > 1000) { ... }`
  
- **Proximity Threshold**:  
  In `updateRoutingWithStopSigns`, the distance threshold (`50.0` units) decides when to mark a route node as having a stop sign. Adjust to match real-world scaling.
  

---

## Contributing

Feel free to open issues or submit pull requests for improvements. This project is maintained as a **demo** and is not meant for production use.

---

## License

This project is open-source. You may freely modify and distribute it. If you use it as part of a larger project, **attribution** is appreciated but not required.
