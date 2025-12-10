# Lab 3: End-to-end project with Node-RED

## Exercice 1

- Plug Arduino board
- Upload sketch to Arnuino board:
    ```bash
    arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload ./Exercice1/Exercice1.ino
    ```
- Start Node-RED and open browser
    ```bash
    node-red Exercise1/exercise-1.json
    ```
- Open dashboard
- Play with dashboard switch and Arduino button
- Click on inject nodes
- Show the output from the board:
    ```bash
    arduino-cli monitor -p /dev/ttyACM0
    ```

If the GND pin is not working, use your finger :/.

## Exercise 2

- PLug Arduino board
- Upload sketch to Arduini board:
    ```bash
    arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload ./Exercise2/Exercise2.ino
    ```
- Start Node-RED an open browser
    ```bash
    node-red Exercise2/exercise-2.json
    ```
- Open dashboard
- Place (heated) finger on board or board on radiator to see the temperature change
- Show the output from the board:
    ```bash
    arduino-cli monitor -p /dev/ttyACM0
    ```

## Exercise 3

Username: node-red \
Password: UBI-lab-3

- PLug Arduino board
- Upload sketch to Arduini board:
    ```bash
    arduino-cli compile --fqbn arduino:mbed:nanorp2040connect --port /dev/ttyACM0 --upload ./Exercise3/Exercise3.ino
    ```
- Start Node-RED an open browser
    ```bash
    node-red Exercise3/exercise-3.json
    ```
- Maybe need to re-enter credentials in the "Security" tab of the mqtt-broker configuration node
- Place (heated) finger on board or board on radiator to see the temperature change
- Show the output from the board:
    ```bash
    arduino-cli monitor -p /dev/ttyACM0
    ```
- Show the [configuration of the Datacake device](https://app.datacake.de/htwg-10/devices/d/315db159-62c9-4263-9c1b-75c66a7ec3bb/t/configuration) and the fact that the MQTT integration is now a paid feature.

