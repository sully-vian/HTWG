# Lab 1: Arduino

## Exercise 1: Internal RGB

```bash
arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload Exercise1_RGB/Exercise1_RGB.ino
```

Verify that the colors switch correctly.

## Exercise 2: Temperature sensore

```bash
arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload Exercise2_Temperature/Exercise2_Temperature.ino

# read from serial port
arduino-cli monitor -p /dev/ttyACM0 -c baudrate=9600
```

Open room window and look at temperature dropping.

## Exercise 3: Microphone

```bash
arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload Exercise3_Microphone/Exercise3_Microphone.ino

# plot the values from the serial port
arduino-cli monitor -p /dev/ttyACM0 -c baudrate=9600 | ttyplot
```

Make some noise to show the evolution. Snap fingers to show the light turning on/off.

## Exercise 4: Posture

```bash
arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload Exercise4_Posture/Exercise4_Posture.ino

# read from the serial port
arduino-cli monitor -p /dev/ttyACM0
```

Tilt the board to 90° to see the red LED turn off.

