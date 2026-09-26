Smart Height-Based Child Safety Door System

An IoT-enabled smart door system designed to distinguish between adults and children using multi-level IR sensors, automated door control, local alerts, and event-triggered image notifications.

📌 Overview

The Smart Height-Based Child Safety Door System is a prototype designed to improve indoor child safety by automatically classifying a person based on height.

The system uses four IR break-beam sensors arranged at two different heights on the entry and exit sides of the door. An Arduino UNO processes the sensor inputs and determines whether the detected person is an adult or a child.

Two SG90 servo motors control the door, while a buzzer provides a local alert for child detection.

An ESP32-CAM captures an image when a detection event occurs and sends the image through Wi-Fi to a predefined Telegram account.

The prototype achieved approximately 98% overall classification accuracy across 100 tested scenarios.

🎯 Problem Statement

Conventional automatic doors generally detect the presence of a person but do not distinguish between adults and children.

This project addresses this limitation using height-based multi-level IR sensing to classify the detected person and provide an appropriate response.

💡 Key Features

- Height-based adult and child classification
- Four IR break-beam sensors
- Detection from entry and exit sides
- Automatic door control using two servo motors
- Buzzer alert for child detection
- Event-triggered ESP32-CAM image capture
- Telegram notification through Wi-Fi
- Adjustable sensor verification timing
- Privacy-oriented event-based image capture

🏗️ System Architecture

The main system consists of:

IR Sensors → Arduino UNO → Classification & Control → Servo Motors / Buzzer

For image notification:

Arduino UNO → ESP32-CAM → Wi-Fi → Telegram

System Architecture

!["System Architecture"](system_architecture.jpg)

🔧 Hardware Components

Component| Quantity| Purpose
Arduino UNO| 1| Sensor processing and system control
ESP32-CAM| 1| Image capture and IoT notification
IR Break-Beam Sensors| 4| Height-based detection
SG90 Servo Motors| 2| Door movement
Buzzer| 1| Child-detection alert
5V 3A Power Adapter| 1| Power supply
Prototype Door/Frame| 1| Demonstration setup

💻 Software & Technologies

- Arduino IDE
- Embedded C/C++
- Arduino UNO
- ESP32-CAM
- Servo library
- ESP32 camera library
- Wi-Fi communication
- Telegram Bot API

📐 ![Height-Based Detection](flowchart.jpg)

The prototype uses two sensing levels:

- Lower sensor level: approximately 6 cm
- Upper sensor level: approximately 16 cm

These prototype dimensions represent scaled versions of approximately 1 ft and 3.5 ft respectively for a full-scale implementation.

Four sensors are arranged as:

- Front lower sensor
- Front upper sensor
- Back lower sensor
- Back upper sensor

Sensor Arrangement

![Sensor Arrangement](sensor_arrangement.jpeg)

⚙️ Working Principle

The system determines the detected person's category based on which IR sensors are interrupted.

![Adult Detection](adult_detected.png)

When the upper sensor is triggered, the system classifies the person as an adult.

The Arduino:

1. Classifies the person as an adult.
2. Opens the door using the servo motors.
3. Triggers the ESP32-CAM.
4. Captures an image.
5. Sends the image through Telegram.

![Child Detection](child_detected.jpg)

When only the lower sensor is triggered, the system waits for approximately 350 ms for an upper-sensor response.

If the upper sensor is not triggered:

1. The person is classified as a child.
2. The door remains closed.
3. The buzzer is activated.
4. The ESP32-CAM is triggered.
5. An image is sent through Telegram.

🚪 Prototype

Two SG90 servo motors are used to provide balanced door movement.

📡 IoT & Telegram Notification

The ESP32-CAM operates as an event-triggered camera.

When a relevant detection event occurs, the Arduino triggers the ESP32-CAM. The camera captures a JPEG image and sends it through Wi-Fi to Telegram.

!["Telegram Notification"](Telegram-notification.jpeg)

📊 Experimental Results

The prototype was evaluated using 100 test scenarios.

Category| Test Cases| Correctly Classified| Accuracy
Adult| 40| 39| 97.5%
Child| 40| 40| 100%
Pet| 20| 19| 95%
Overall| 100| 98| 98%

Detection Accuracy

!["Detection Accuracy"](accuracy_chart.png.jpeg)

Serial Monitor

!["Serial Monitor"](Serial_monitor.png)

⚡ Performance

According to the experimental evaluation:

- Overall classification accuracy: 98%
- Adult classification accuracy: 97.5%
- Child classification accuracy: 100%
- Pet/object test accuracy: 95%
- Reported average system response: approximately 350 ms
- Camera transmission success: approximately 95%
- Average image transmission time: approximately 2.8–3 seconds

⚠️ Limitations

- Multiple-person simultaneous detection is not handled.
- Strong sunlight or halogen lighting may affect IR sensing.
- Height ambiguity may occur between a tall child and a short adult.
- The prototype uses small-scale sensor spacing and servo motors.
- Full-scale implementation would require longer-range sensors and stronger actuators.
- Wi-Fi connectivity affects remote image transmission.

🚀 Future Improvements

- Ultrasonic or thermal sensing as a secondary classification method
- TinyML-based classification using ESP32-S3
- Industrial-grade motors and actuators
- Improved lighting immunity
- Multi-person detection
- Web/mobile monitoring dashboard
- Voice-assistant integration

📄 Documentation

The complete technical details, methodology, experimental setup, results, and references are available in the project paper.

!["📄 Read the Project Paper"](documentation/smart_door_conference_paper.pdf)

👩‍💻 Team

Aisha Rushan
Deeksha Manjunath Naik
Sahana Anand Naik
Navin Kasim

Department of Electronics and Communication Engineering
Anjuman Institute of Technology & Management (AITM), Bhatkal


💻 Source Code

The source code used for the project is available in the "code" folder.

- [Arduino_code.ino](code/arduino_code.ino) — Arduino code for the main door control and sensor system.
- [ESP_code.ino](code/esp.ino) — ESP32 code used in the project.

📚 Reference

📚 References

1. F. Aman and A. C., “Motion sensing and image capturing base smart door system on android platform,” in Proc. Int. Conf. Energy, Communication, Data Analytics and Soft Computing (ICECDS), 2017, pp. 2346–2350.

2. P. B. V. Raja Rao, P. Lahari Manojna, V. S. Sonaleo, T. Hari Chandana, P. Sri Lakshmi, and V. Hemalatha, “Home security with IoT and ESP32-CAM – AI thinker module,” in Proc. Int. Conf. Cognitive Robotics and Intelligent Systems (ICC-ROBINS), 2024, pp. 710–714.

3. R. Kakade, K. Wadetwar, K. Nimje, S. S. Badhiye, P. Borkar, and S. Shinde, “IoT-integrated door sensor solution for enhancing smart home security,” in Proc. 4th Int. Conf. Technological Advancements in Computational Sciences (ICTACS), Nov. 2024, pp. 913–917, doi: 10.1109/ICTACS62700.2024.10840511.

4. M. Vijarania, V. Jaglan, and A. Sanjay, “Security surveillance and home automation system using IoT,” EAI Endorsed Transactions on Smart Cities, Aug. 2020, doi: 10.4108/eai.21-7-2020.165963.

5. R. S. Nakandhra Kumar, S. Aravinth, and R. Venkatasamy, “Design and development of IoT-based smart door lock system,” in Proc. 3rd Int. Conf. Intelligent Computing, Instrumentation and Control Technologies (ICCICT), 2022, pp. 1525–1528.

6. A. K. Singh, Laxmi, Shamith, H. Nagarathna, M. Keshav, and M. Krishna, “Design and implementation of smart door lock system using IoT,” in Proc. 8th Int. Conf. Computational System and Information Technology for Sustainable Solutions (CSITSS), 2024.

7. P. Kumar, N. Subramanian, and K. Zhang, “SaViT: Technique for visualization of digital home safety,” in Proc. 8th IEEE/ACIS Int. Conf. Computer and Information Science (ICIS), Shanghai, China, 2009, pp. 1120–1125.

8. S. Begum, P. M. R. Reddy, and R. K. Kodali, “Advancing safety: IoT-based multi-sensor system for real-time multiple hazards detection and alarming,” in Proc. 5th Int. Conf. Smart Electronics and Communication (ICOSEC), 2024.

9. S. Sahu, R. Singh, P. Arya, and R. Nirala, “Smart home automation lighting system and smart door lock using Internet of Things,” in Proc. 4th Int. Conf. Advances in Computing, Communication Control and Networking (ICAC3N), 2022, pp. 1320–1325.

10. S. R. K., G. S. Hegde, B. S. Sannakashappanavar, M. M., and M. Kumar, “Intelligent surveillance and protection system for farmlands from animals,” in Proc. IEEE Int. Conf. Contemporary Computing and Communications (InC4), 2024, pp. 1–8.
