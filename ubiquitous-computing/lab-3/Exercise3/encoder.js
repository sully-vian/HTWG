function Encoder(device, measurements) {
    // Get Device Information
    var device = device.id;
    var serial = device.serial_number;
    var name = device.name;

    // Create JSON Payload for Publish
    var payload = {
        message: "Hello World!",
        info: "Sent from Datacake IoT Platform",
        device: device,
        name: name,
        serial: serial,
        temperature: 23.45,
        status: true,
    };

    // Return Topic and Payload
    return {
        topic: "Temperature", // define topic
        payload: JSON.stringify(payload), // encode to string
    };
}
