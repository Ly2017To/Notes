# FPGA Voltage Standards

### single-ended I/O standards: LVCOMS; LVTTL; HSTL; SSTL; PCI;
### differential I/O standards: LVDS; RSDS; TMDS; differential HSTL and SSTL;

### TTL (Transistor-Transistor Logic)
- TTL is an older logic standard, typically used in low-speed and low-power applications.
- The signal quality is relatively poor and is commonly found in early FPGA and integrated circuits.

### LVCMOS (Low Voltage CMOS)
- Low voltage, low power, suitable for high-density, high-speed logic circuits.
- Offers good noise immunity.
- LVCMOS is widely used in FPGA input and output ports.

### LVTTL (Low Voltage TTL)
- A low-voltage version of TTL that reduces power consumption.
- Provides faster signal switching speeds, commonly used in high-speed applications.

### Summary
| Standard| Vcc| Input                        | Output                 |
|:-------:|:--:|:---------------------------: | :---------------------:|
|TTL      |5V  | 1>=2.4V; 0<=0.5V;            | 1>=2.0V; 0<=0.8V;      |
|LVTTL3V3 |3.3V| 1>=2.4V; 0<=0.4V;            | 1>=2.0V; 0<=0.8V;      | 
|LVTTL2V5 |2.5V| 1>=2.4V; 0<=0.2V;            | 1>=1.7V; 0<=0.7V;      | 
|CMOS     |5V  | 1>=4.45V; 0<=0.5V;           | 1>=3.5V; 0<=1.5V;      | 
|LVCMOS3V3|3.3V| 1>=3.2V; 0<=0.4V;            | 1>=2.0V; 0<=0.7V;      | 
|LVCMOS2V5|2.5V| 1>=2.0V; 0<=0.4V;            | 1>=1.7V; 0<=0.7V;      | 
|LVCMOS1V8|1.8V| 1>=Vcc-0.45V; 0<=0.45V;      | 1>=0.65Vcc; 0<=0.35Vcc;| 
|LVCMOS1V2|1.2V| 1:close to Vcc; 0:close to 0;| 1>=0.65Vcc; 0<=0.35Vcc;|

### HSTL (High-Speed Transistor Logic)
- Voltage Standard:
    - High-level: 1.7V ~ Vcc (Vcc is typically 2.5V or 1.8V)
    - Low-level: 0V ~ 0.4V
- Characteristics:
    - Primarily used for high-frequency and high-speed communication and interfaces, widely used in DDR (Double Data Rate) memory interfaces.
    - The voltage range is narrow while transmitting high-speed signals.

### LVDS (Low Voltage Differential Signaling)
- Voltage Standard:
    - High-level: ±350mV ~ ±600mV
    - Low-level: ±150mV ~ ±350mV
- Characteristics:
    - Differential signal method, suitable for high-speed transmission.
    - Provides strong noise immunity, commonly used for high-bandwidth data transmission (such as high-speed serial interfaces).
    - Low power consumption.

### PECL (Positive Emitter Coupled Logic)
- Voltage Standard:
    - High-level: 3.0V ~ 3.5V
    - Low-level: 0V ~ 0.8V
- Characteristics:
    - Uses differential signal transmission, suitable for high-frequency applications.
    - Higher power consumption, but provides very fast signal switching.
    - Commonly used in high-speed clock signals, counters, etc.

### CML (Current Mode Logic)
- Voltage Standard:
    - High-level: +0.8V
    - Low-level: 0V
- Characteristics:
    - Signal transmission is based on current rather than voltage, offering higher speeds and lower latency.
    - Commonly used in high-speed serial communication and signal links.

### SSTL (Stub Series Terminated Logic)
- Voltage Standard:
    - High-level: 2.5V ~ 2.8V (for 2.5V systems)
    - Low-level: 0V ~ 0.6V
- Characteristics:
    - Suitable for Double Data Rate (DDR) memory interfaces.
    - Supports high-speed signal transmission and low power consumption.

### I2C (Inter-Integrated Circuit)
- Voltage Standard:
    - High-level: 3.3V, 2.5V, or 1.8V
    - Low-level: 0V
- Characteristics:
    - Used for low-speed, short-distance communication, widely used for internal control and communication within an FPGA.
    - Suitable for simple serial communication between multiple devices.


