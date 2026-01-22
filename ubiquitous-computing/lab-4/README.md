# Lab 4: Smart Home

## Platform installation

To start the platform:

```bash
docker run -d \
  --name homeassistant \
  --privileged \
  --restart=unless-stopped \
  -e TZ=Europe/Paris \
  -v "$(pwd)/config":/config \
  -v /run/dbus:/run/dbus:ro \
  --network=host \
  ghcr.io/home-assistant/home-assistant:stable
```

Access it at http://localhost:8123.

## Presentation Ideas

- Monitor the washing machine
    - Notification when finishes
    - Very useful with shared household appliances (student dorm)
- Monitoring the Mailbox
    - Notified when new mail has been delivered
    - Prevents theft
- Swimming pool monitoring
    - Long-term level tracking to avoid wasting water
    - Water composition monitoring to either alert when harmful or fix the composition
- All house lamps monitoring
    - All out check (before holiday leave for instance)
- Electronical devices monitoring
    - Electricity usage detailed tracking
    - Prevents wasting power
- Water consumption monitoring
    - Water usage detailed tracking
    - Prevents wasting water
- Smart garage door and shutters ("Bunker Mode")
    - Close them or check they're closed in a second (in evenings or before long absences)

## Questions

- [x] Which types of devices could be implemented in the building ? Try to use as many different devices as possible, but all of them should provide additional value for the user.

- [x] How are you going to connect them to each other ?

- [x] How are you going to ensure the cooperative work of the devices ?

- [x] Which protocols/interfaces may be used ?

- [] How are you going to provide the power supply ?

- [] Which devices will have which aims (e.g. increasing comfort/providing security/safety, etc.) ?
