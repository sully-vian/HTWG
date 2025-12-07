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

## Exercese 2

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

TODO

