function Decoder(topic, payload) {
    return [
        {
            // Datacake device serial number
            device: "31663110-f18f-4ac8-8af3-804a97e33637",
            // field name in db
            field: "TEMPERATURE",
            value: JSON.parse(payload),
            timestamp: Date.now(),
        },
    ];
}
