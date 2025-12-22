# Event-Driven Architecture (EDA) in IoT & Embedded Software: 
---
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
---
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
---
## Direct Event Handler Registration vs. Pub/Sub with a Broker (Event-Driven Architecture)

- **Direct Event Handler Registration**: Producers (event emitters) maintain a direct list of handler functions. When an event fires, the producer calls registered handlers directly.    
- **Pub/Sub with a Broker**: A central broker mediates between publishers (producers) and subscribers (consumers). Publishers send events to the broker. Subscribers register with the broker for events—no direct producer-consumer communication.
- **Coupling is the biggest difference**: direct registration trades flexibility for simplicity (tight coupling), while pub/sub with a broker trades minimal complexity for loose coupling, scalability, and distribution. Choose based on your application’s size, architecture (single-process vs. distributed), and need for advanced features like persistence or filtering.                             

### Key Differences (Detailed Comparison)

| **Dimension**                 | **Direct Event Handler Registration**                                                                                       | **Pub/Sub with a Broker**                                                                                                                 |
| ----------------------------- | --------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| **Coupling**                  | Tight coupling (producer ↔ consumer): Exactly which handlers are registered. Consumers must know the producer to register. | Loose coupling (producer ↔ broker ↔ consumer): Publishers have no knowledge of subscribers. Subscribers have no knowledge of publishers. |
| **Event Routing**             | Producer handles routing (iterates over and calls handlers directly).                                                       | Broker handles routing (forwards events to matching subscribers).                                                                         |
| **Scalability**               | Poor: Adding new consumers requires modifying/accessing the producer. Producers become bottlenecks as handlers grow.     | Excellent: Publishers/Subscribers without touching existing components. Can scale independently (e.g., distributed brokers like Kafka).   |
| **Location Transparency**     | No (Producers/Consumers must share the same process/memory space — e.g., a single C program).                                 | Yes (Supports distributed systems): Publishers/Subscribers can run on different machines, processes, or languages.                       |
| **Event Filtering**           | Limited: Filtering done by the producer or in the handler (e.g., a handler checks if the event is relevant).                | Rich: Filtering (subscribers can target specific event types, topics, or content).                                                        |
| **Multiple Producers**        | Works for 1 producer (each producer has its own handler list; shared handlers across producers are cumbersome).             | Works for multiple producers (multiple publishers can send to the same broker topic; subscribers receive all relevant events).            |
| **Complexity**                | Simple (minimal code: function pointer lists + loop to call handlers).                                                      | More complex (requires broker implementation or third-party integration — e.g., Redis, MQTT, Kafka).                                        |
| **Reliability & Persistence** | No built-in persistence: In-flight events lost if the producer crashes. Handlers must be re-registered on restart.      | Built-in persistence: Brokers can store events (e.g., Kafka logs) so offline subscribers don’t miss data.                                 |
| **Overhead**                  | Low (no broker latency; direct function calls).                                                                             | Moderate (broker adds minimal latency, but enables advanced features).                                                                    |

### Use Case Guidelines
- Choose Direct Event Handler Registration When:
    - Building small, single-process applications (e.g., embedded C programs for microcontrollers).
    - Minimal overhead is critical (e.g., real-time systems with strict latency requirements).
    - Tight coupling is acceptable (e.g., a UI button triggering local callbacks).
- Choose Pub/Sub with a Broker When:
    - Building distributed systems (e.g., IoT sensors → cloud backend, microservices).
    - Loose coupling is required (modular, maintainable architecture).
    - Scalability is a priority (adding components without breaking existing code).
    - Event persistence, filtering, or cross-language communication is needed.
---

## EDA code example in C language
```c
//define the event types
typedef enum{
    EVENT_SENSOR_DATA_READY,
    EVENT_BUTTON_PRESSED,
    EVENT_TIMER_EXPIRED,
    EVENT_UNKNOWN    
}eventType_t;

//define event data structure
typedef struct {
    float sensorValue;
}sensorDataEvent_t;

typedef struct {
    int buttonId;
}buttonPressedEvent_t;

typedef struct {
    int timerId;
}timerExpiredEvent_t;

//define event structure
typedef struct {
    eventType_t type;
    time_t timestamp;
    union{
        sensorDataEvent_t sensorData,
        buttonPressedEvent_t buttonPressed,
        timerExpiredEvent_t timerExpired,
    }data;
    unsigned int priority;
}event;

//define the event handler call back function
typedef void (* handler_t)(event);

//define eventHandlerRegistry
typedef struct{
    eventType type[HANDLER_SIZE];
    eventHandler handler[HANDLER_SIZE];
}eventHandlerRegistry_t;

//define circular buffer for event
typedef struct{
    event events[EVENT_QUEUE_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
}eventBuff_t;

//initialize eventHandlerRegistry
void iniEventHandlerRegistry(eventHandlerRegistry_t * pHandlerRegistry)
{
    int i=0; 
    for(int i=0; i<HANDLER_SIZE>; i++){
        pHandlerRegistry->handler[i]=NULL;
    }
}

//initialize eventBuff
void iniEventBuff(eventBuff_t * pEventBuff)
{
    pEventBuff->head=0;
    pEventBuff->tail=0;
    pEventBuff->count=0;
}

//event register
bool eventRegister(event_t event, handler_t handler, eventHandlerRegistry_t * pHandlerRegistry)
{
    int i=0; 
    for(int i=0; i<HANDLER_SIZE; i++){
        if(pHandlerRegistry->handler[i]==NULL){
            pHandlerRegistry->handler[i]=handler;
            pHandlerRegistry->type[i]=event.type;
            return true;
        }
    }
    return false;
}

//event unregister
void eventUnregister(event_t event, handler_t handler, eventHandlerRegistry_t * pHandlerRegistry)
{
    int i=0; 
    for(int i=0; i<HANDLER_SIZE; i++){
        if(pHandlerRegistry->event[i].type==event.type&&pHandlerRegistry->handler[i]==handler){
            pHandlerRegistry->handler[i]=NULL;
            return true;
        }
    }
    return false;
}

//event enqueue, when an event generates
bool eventEnqueue(event_t event, eventBuff_t * pEventBuff)
{
    if(pEventBuff->count>=EVENT_QUEUE_SIZE) return false;

    int position = pEventBuff->tail;

    if(pEventBuff->count>0){
        while(pEventBuff->event[(position-1+EVENT_QUEUE_SIZE)%EVENT_QUEUE_SIZE].priority<event.priority){
            position = (position-1+EVENT_QUEUE_SIZE)%EVENT_QUEUE_SIZE;
        }
    }

    pEventBuff->event[position]=event;
    pEventBuff->tail=(pEventBuff->tail+1)%EVENT_QUEUE_SIZE;
    pEventBuff->count++;

    return true;

}

//process events
void processEvents(eventBuff_t * pEventBuff, eventHandlerRegistry_t * pHandlerRegistry)
{   
    event_t event;
    int i=0;

    while(pEventBuff->count>0){
        event = pEventBuff[pEventBuff->head];
        pEventBuff->head=(pEventBuff->head+1)%EVENT_QUEUE_SIZE;
        pEventBuff->count--;
        for(int i=0; i<HANDLER_SIZE; i++){
            if(pHandlerRegistry[i].handle!=NULL&&pHandlerRegistry->event[i].type==event.type){
                pHandlerRegistry->handler[i](event);
            }
        }
    }
}

// Functions to produce events
void ensor_produce_event(float sensorData, int priority, eventBuff_t * pEventBuff) {
    event_t event = { .type = EVENT_SENSOR_DATA_READY, .data.sensorData = sensorData, .timestamp = time(NULL), .priority = priority };
    eventEnqueue(event,pEventBuff);
}

```
---



