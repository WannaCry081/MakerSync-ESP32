# MakerSync-ESP32

MakerSync-ESP32 is a project aimed at converting plastic materials (PET) into 3D printing filament. It utilizes an ESP32 microcontroller integrated with various sensors and components for interactive control, along with a backend Django REST API for data exchange.

## Project Overview

The project involves the integration of an ESP32 microcontroller with sensors and actuators to control the filament conversion process. It communicates with a backend Django REST API via HTTP requests, enabling data exchange and control from a remote location via a Flutter app.

## Components

The project comprises the following components:

- **ESP32 Microcontroller**: Manages the control and communication between sensors, actuators, and the backend API.
- **IR Sensor**: Detects the presence of PET plastic materials.
- **Relay Module (5V Single Channel)**: Controls the heating element or other high-power components.
- **MAX6675**: Monitors temperature during the filament conversion process.
- **LEDs (Red, Yellow, Green)**: Provides visual feedback for system status.
- **LCD 16x2 Display**: Displays relevant information about the process.
- **Push Buttons**: Used for user interaction and control.

## Integration with Backend

The ESP32 utilizes Wi-Fi connectivity to communicate with the backend Django REST API. It leverages HttpClient for making HTTP requests to send data and receive instructions from the server. Additionally, ArduinoJson library is used for parsing JSON responses from the API.

## Development Environment

The project code is developed using PlatformIO as the IDE for ESP32 development. PlatformIO offers a unified environment for embedded development with support for multiple platforms and frameworks, making it suitable for managing complex projects like MakerSync-ESP32.

## Installation

Follow these steps to get started with MakerSync-ESP32:

1. **Clone the Repository:**  
    Clone the MakerSync-ESP32 repository to your local machine using Git:

    ```bash
    git clone https://github.com/WannaCry081/MakerSync-ESP32.git
    ```

2. **Set Up Hardware Components:**  
    Set up the necessary hardware components for MakerSync-ESP32, including:
    - IR Sensor
    - Relay Module (5V Single Channel)
    - MAX6675
    - LEDs (Red, Yellow, Green)
    - LCD 16x2 Display
    - Push Buttons

3. **Install PlatformIO IDE:**  
    Install the PlatformIO IDE for ESP32 development. PlatformIO offers a unified environment for embedded development with support for multiple platforms and frameworks.

4. **Configure Wi-Fi and Backend API:**  
    In the ESP32 code, configure the Wi-Fi credentials to connect to your network and the backend API endpoint to communicate with the Django REST API.

5. **Upload Code to ESP32:**  
    Upload the MakerSync-ESP32 code to your ESP32 microcontroller using the PlatformIO IDE.

6. **Run Backend Django REST API:**
    Before proceeding, ensure that the backend Django REST API is up and running. This API facilitates communication with the ESP32 and controls the filament conversion process.

    You can find instructions for setting up and running the backend Django REST API in the project repository: [Backend Django REST API](https://github.com/WannaCry081/MakerSync-Django.git)

7. **Monitor and Control:**
    Monitor the system behavior and control the filament conversion process using the Flutter app or by directly interacting with the ESP32.

    Additionally, you can explore the MakerSync-Flutter codebase on [GitHub](https://github.com/shielamae02/MakerSync-Flutter.git) to understand the implementation details and contribute to the project.
