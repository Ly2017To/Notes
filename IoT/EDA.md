# Event-Driven Architecture (EDA) in IoT & Embedded Software: 

## EDA Overview in IoT/Embedded

Event-driven architecture (EDA) is a **software design paradigm** centered on the production, detection, consumption, and reaction to *events*—discrete occurrences or state changes that carry meaningful information (e.g., a sensor detecting a temperature spike, a button press, or a device disconnecting from a network).

In IoT and embedded systems, EDA is particularly valuable because these systems are often **distributed, resource-constrained, and reactive by nature** (they need to respond to real-world stimuli quickly). Unlike traditional request-response models (where a component actively polls for data), EDA uses a *push-based* model where events are broadcast, and relevant components act only when they receive events they care about.

In embedded software development, a **call-back based event-driven architecture** is commonly used to handle asynchronous events efficiently. This approach delegates event handling to specific callback functions, which are triggered when certain events occur (e.g., hardware interrupts, user inputs, or timers).

### Core Components of EDA in IoT/Embedded Systems

| Component                | Description                                                                                 | IoT/Embedded Examples                                                          |
| ------------------------ | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| **Event Producers**      | Generate events when state changes occur.                                                   | Sensors (temperature, motion), actuators, GPIO interrupts, timer ticks.        |
| **Event Broker/Manager** | Routes events from producers to consumers; decouples producers and consumers.               | Lightweight message queues (embedded), MQTT brokers (IoT), custom dispatchers. |
| **Event Consumers**      | Listen for specific events and execute actions.                                             | Embedded firmware handlers, cloud dashboards, mobile apps, actuators.          |
| **Events**               | Structured data packets encapsulating state changes (lightweight for resource constraints). | `{type: "temp_alert", value: 35°C, device_id: "sensor_01"}`                    |


### Key Benefits of EDA for IoT/Embedded

* **Loose Coupling**: Producers and consumers are independent—modify one without impacting the other.
* **Resource Efficiency**: Eliminates polling (saves CPU/power in battery-powered devices).
* **Real-Time Responsiveness**: Immediate reactions to critical events (e.g., smoke detector alarms).
* **Scalability**: Add new consumers without modifying producers (e.g., new analytics tools for sensor data).
* **Fault Tolerance**: Buffer or route events to backups if a consumer fails.

### Challenges in IoT/Embedded EDA

* **Resource Constraints**: Limited memory/CPU—requires lightweight brokers (e.g., MQTT) and event formats.
* **Latency**: Time-sensitive apps (industrial control) need edge computing to reduce cloud delays.
* **Event Consistency**: Ensure reliable delivery in unstable networks (ACKs, retries).

## Register vs. Subscribe: Key Differences

While sometimes used interchangeably, these terms have distinct meanings in EDA—especially for embedded/IoT systems.

| Aspect                     | Register                                                                            | Subscribe                                                                                           |
| -------------------------- | ----------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| **Core Definition**        | Associate a handler function with an event type in a **local, in-process context**. | Request to receive events from a **broker/external publisher** in a **distributed context**.        |
| **Scope**                  | Intra-process (single embedded firmware/app).                                       | Inter-process, device-to-device, or device-to-cloud.                                                |
| **Mechanism**              | Local dispatcher maps event types to handler functions (lookup table).              | Consumer sends a request to a broker to receive specific events.                                    |
| **Broker Dependency**      | No broker required.                                                                 | Requires a broker (e.g., MQTT, LoRaWAN gateway).                                                    |
| **IoT/Embedded Use Cases** | - Attach an ISR to a GPIO interrupt.- Link a callback to a timer event.             | - Gateway subscribes to sensor events via MQTT.>- Cloud server subscribes to device status updates. |
| **Lifetime**               | Tied to the process/component (destroyed if component is removed).                  | Persists until unsubscribed or broker times out.                                                    |
