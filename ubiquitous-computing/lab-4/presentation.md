# Introduction

General Aims:
- maximize comfort
- maximize security
- maximize safety

---

# The House

- 4 levels
- Basement (lighting)
- Garage
- Hot Tub on roof level
- Decks

---

# The Devices


- **Utility & Safety**
    - Smart washing machine
        - Notification when done
        - Useful when sharing household appliances (student dorm)
    - Smart Hot Tub
        - Water composition monitoring to either alert when harmful or fix the composition
- **Security**
    - Smart Mailbox
        - Notification when new mail
        - Prevents theft (in case of valuable package delivery)
    - Smart garage door and shutters
        - Close all accesses
        - Monitor all accesses states (closed/open)
- **Comfort**
    - Smart oven
        - Schedule baking (auto-turn on 1h before arrival)
        - Schedule pre-heating
    - Smart sunshade
        - Deploy when luminosity
        - Retract when dark or wind
    - Lights following the Circadian rhythm
        - Underground lights are low at night

---

# The Smart Ecosystem

- Connections via
    - Wireless protocols like Wi-Fi & Bluetooth
    - Ethernet
- Power via
    - outlets (maybe solar for the sunshades)
    - PoE (Power over Ethernet) for wired devices


---

# The Technical Infrastructure

- Central Brain: Home Assitant as primary integration platform for all devices
    - Hosted on a Raspberry Pi _i.e._
- Logic & Automation: Node-RED
    - Handle complex cooperative work between devices (retract sunshades when wind)
- Hardware Interaction: Arduino for custom sensor input
- Human Interface: Telegram bots
    - Easy remote interaction
    - Low-friction notifications
    - Multi-dispatch notifications (on groups _i.e._)

---

# Conclusion

