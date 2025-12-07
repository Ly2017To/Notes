# Communications Summary

Wireless communication refers to the transfer of information between two or more devices without the use of physical wires or cables. It involves the transmission of data via electromagnetic waves, such as radio waves, microwaves, or infrared. Here is a table summarizing the IEEE classification of frequency bands.

---

## IEEE Classification of Frequency Bands

| **Frequency Band**                | **Frequency Range**  |                     **Common Applications**                     |
|-----------------------------------|----------------------|-----------------------------------------------------------------|
| **Extremely Low Frequency (ELF)** | 3 Hz – 30 Hz         | Submarine communication, geophysical research                   |
| **Super Low Frequency (SLF)**     | 30 Hz – 300 Hz       | Submarine communication                                         |
| **Ultra Low Frequency (ULF)**     | 300 Hz – 3 kHz       | Underground communication                                       |
| **Very Low Frequency (VLF)**      | 3 kHz – 30 kHz       | Long-range radio communication, navigation systems              |
| **Low Frequency (LF)**            | 30 kHz – 300 kHz     | AM radio broadcasting, maritime communication                   |
| **Medium Frequency (MF)**         | 300 kHz – 3 MHz      | AM radio broadcasting, aeronautical communication               |
| **High Frequency (HF)**           | 3 MHz – 30 MHz       | Shortwave radio, long-range communication (military, aviation)  |
| **Very High Frequency (VHF)**     | 30 MHz – 300 MHz     | FM radio, TV broadcasting, two-way radio communication          |
| **Ultra High Frequency (UHF)**    | 300 MHz – 3 GHz      | Cellular phones, GPS, Bluetooth, Wi-Fi, TV broadcasting         |
| **Super High Frequency (SHF)**    | 3 GHz – 30 GHz       | Satellite communication, radar, Wi-Fi (5 GHz), microwave links  |
| **Extremely High Frequency (EHF)**| 30 GHz – 300 GHz     | Advanced radar, satellite communication, scientific research    |
| **Terahertz (THz)**               | 300 GHz – 3 THz      | Imaging, spectroscopy, wireless communications (under research) |

## Notes
- **VLF, LF, MF**: Often used for long-distance communication and navigation systems.
- **HF**: Popular for shortwave radio, including international broadcasting and amateur radio.
- **VHF, UHF**: Widely used for FM radio, TV broadcasting, cellular communication, and wireless networking (Wi-Fi, Bluetooth).
- **SHF, EHF**: Used for high-capacity point-to-point communication systems (e.g., satellite links, microwave links), and military radar systems.
- **THz**: A developing area for high-frequency wireless communication and imaging technologies.

## Communication Models
   - **Point-to-Point**: Communication between two devices (e.g., microwave links, fiber optic cables).
   - **Point-to-Multipoint**: Communication from one device to multiple devices (e.g., broadcasting, Wi-Fi).
   - **Multipoint-to-Multipoint**: Communication between multiple devices in a network (e.g., mobile networks, ad hoc networks).

---

## Wireless Communication Principles and Metrics

### Modulation
   - The process of encoding information onto a carrier wave by varying its properties (amplitude, frequency, or phase).
   - **Types**:
     - **AM (Amplitude Modulation)**: Amplitude Modulation (AM) is a modulation technique used in electronic communication, most commonly for transmitting information via a radio carrier wave. In AM, the amplitude (strength) of the carrier wave is varied in proportion to the waveform (signal) that is being sent. The frequency and phase of the carrier wave remain constant. AM radio is an application example. AM radio stations use this technique to broadcast audio signals over long distances.
     - **FM (Frequency Modulation)**: Frequency Modulation (FM) is a method of encoding data in a carrier wave by varying its frequency. The amplitude remains constant, but the frequency is altered in proportion to the modulating signal, which could be an audio or digital signal. FM radio is an application example. FM is commonly used for radio broadcasting because it provides better sound quality and less susceptibility to noise compared to AM.
     - **PM (Phase Modulation)**: Phase Modulation (PM) is a modulation technique where the phase of the carrier wave is altered in response to the amplitude of the modulating signal. The frequency and amplitude of the carrier remain constant, but the phase shifts according to the information being transmitted. PM is used in some digital modulation schemes, such as Phase Shift Keying (PSK), which is used in communication systems like satellite communication and cellular networks.

### Multiplexing
   - Allows multiple signals to share the same communication channel.
   - Types:
     - **Time Division Multiplexing (TDM)**: Time Division Multiplexing (TDM) is a method of transmitting and receiving multiple signals over a single communication channel by dividing the channel into time slots. Each signal is assigned a specific time slot during which it can transmit its data. TDM ensures that each signal uses the channel in turns without interfering with others. Integrated Services Digital Network (ISDN) is an application example, where multiple voice calls are carried over the same transmission line. The system divides the available bandwidth into time slots. Each call gets its own time slot during which it transmits voice data.
     - **Frequency Division Multiplexing (FDM)**: Frequency Division Multiplexing (FDM) is a technique that divides the total bandwidth of a communication channel into multiple smaller frequency bands, each of which is used to transmit a separate signal. Each signal is assigned a distinct frequency band, ensuring that the signals do not interfere with each other. Cable television is an application example. In a cable TV system, multiple television channels are transmitted over a single coaxial or fiber-optic cable using FDM. Each channel is assigned a specific frequency band within the cable's overall bandwidth.
     - **Code Division Multiplexing (CDM)**: Code Division Multiplexing (CDM) is a technique that allows multiple signals to be transmitted simultaneously over the same frequency channel by assigning a unique code to each signal. The signals are spread over the entire frequency spectrum and distinguished by their unique codes. CDMA is an application example. In a Code Division Multiple Access (CDMA) cellular network, multiple users can make phone calls or send data over the same frequency band. Each call or data transmission is assigned a unique code, which distinguishes it from other transmissions.

### channel Capacity
- The maximum data rate a communication channel can support, defined by **Shannon-Hartley theorem**, given by the formula:

$$
C = B \log_2 (1 + \text{SNR})
$$

Where:
- \( C \) = **Channel capacity** (maximum data rate) in **bits per second (bps)**
- \( B \) = **Bandwidth** of the channel in **Hertz (Hz)**
- **SNR** = **Signal-to-Noise Ratio** (unitless, ratio of signal power to noise power)

### **Bandwidth**
   - The amount of data that can be transmitted over a network in a given time period.
   - Higher bandwidth allows faster data transfer.

### **Signal-to-Noise Ratio (SNR)**
   - The ratio of the signal strength to the background noise level.
   - Higher SNR leads to better quality communication.

### **Bit Error Rate (BER)**
   - The percentage of bits received incorrectly in a transmission.
   - A lower BER indicates better reliability of the communication system.

### **Latency**
   - The time it takes for data to travel from the source to the destination.
   - Low latency is essential for real-time communication.
   
---

## Signal Propagation
- Refers to the behavior of electromagnetic waves as they travel through space.
- Affected by factors like **distance**, **obstacles**, **weather**, and **frequency**.
- Types:
     - **Free Space Propagation**: **free space path loss (FSPL)** formula calculates the loss of signal strength as it travels through free space without any obstacles. It is given by:

$$
\text{FSPL} = \frac{(4 \pi d f)}{c}
$$

Where:
- \( \text{FSPL} \) = **Free Space Path Loss** (in decibels, dB)
- \( d \) = **Distance** between the transmitter and receiver (in meters)
- \( f \) = **Frequency** of the signal (in Hertz, Hz)
- \( c \) = **Speed of light** in a vacuum (\( 3 \times 10^8 \) m/s)

Alternatively, a more common form in dB is:

$$
\text{FSPL(dB)} = 20 \log_{10}(d) + 20 \log_{10}(f) - 147.55
$$

Where:
- \( d \) = **Distance** between transmitter and receiver (in meters)
- \( f \) = **Frequency** of the signal (in Hz)
     
- **Multipath Propagation**: multipath propagation occurs when a signal reaches the receiver via multiple paths due to reflection, diffraction, and scattering. The **received signal power** in a multipath environment can be represented using the **Rician fading model** or **Rayleigh fading model**.

1. **Rician Fading Model**: This model describes a scenario where there is a dominant line-of-sight (LOS) path along with reflected paths:

$$
\text{Received Power} = P_{\text{LOS}} \cdot (1 + K)
$$

   Where:
   - \( P_{\text{LOS}} \) = **Line-of-sight power** (dominant signal)
   - \( K \) = **Rician factor** (ratio of the power in the line-of-sight path to the scattered power)

3. **Rayleigh Fading Model**: In the this model, there is no line-of-sight (LOS) path, and the signal is scattered in all directions. The **received signal power** in Rayleigh fading follows:
   
$$
P_{\text{received}} = P_0 \cdot \left(\frac{d_0}{d}\right)^n
$$

   Where:
   - \( P_0 \) = **Reference power** at a reference distance \( d_0 \)
   - \( d_0 \) = **Reference distance**
   - \( d \) = **Actual distance** between transmitter and receiver
   - \( n \) = **Path loss exponent** (usually between 2 and 4)

- **Friis' Transmission Equation**: This equation calculates the power received by an antenna when the power is transmitted from another antenna over a free-space path. It is used in the analysis of wireless communication systems, especially for line-of-sight propagation. The equation is given by:

$$
P_r = P_t \cdot \left( \frac{G_t G_r \lambda^2}{(4 \pi d)^2} \right)
$$

Where:
- \( P_r \) = **Received power** at the receiver (in watts, W)
- \( P_t \) = **Transmitted power** from the transmitter (in watts, W)
- \( G_t \) = **Gain of the transmitting antenna** (dimensionless, usually in linear scale)
- \( G_r \) = **Gain of the receiving antenna** (dimensionless, usually in linear scale)
- \( \lambda \) = **Wavelength** of the signal (in meters, m)
- \( d \) = **Distance** between the transmitting and receiving antennas (in meters, m)

---

## IoT Wireless Communication Protocols Comparison

| **Protocol**  | **Frequency Band**    | **Range**             | **Data Rate**       | **Power Consumption** | **Application**                        | **Example Use Cases**                                 |
|---------------|-----------------------|-----------------------|---------------------|-----------------------|----------------------------------------|-------------------------------------------------------|
| **Wi-Fi**     | 2.4 GHz, 5 GHz        | 100m (indoors)        | Up to 1 Gbps        | High                  | High-bandwidth applications            | Smart homes, smart appliances, high-speed IoT devices |
| **Bluetooth** | 2.4 GHz               | 100m (Bluetooth 5.0)  | 1 Mbps - 2 Mbps     | Low                   | Short-range, low-power communication   | Wearables, headsets, IoT sensors, devices with low data rate |
| **Zigbee**    | 2.4 GHz, 868 MHz      | 10-100m               | 250 Kbps            | Very Low              | Low-power, low-data rate, mesh network | Smart lighting, home automation, industrial control |
| **Z-Wave**    | 868 MHz (Europe), 900 MHz (US) | 30-100m      | 100 Kbps            | Very Low              | Home automation, security systems      | Smart thermostats, door/window sensors, locks       |
| **LoRaWAN**   | Sub-GHz (433 MHz, 868 MHz, 915 MHz) | 2-5 km (urban), up to 15 km (line-of-sight) | 0.3 Kbps - 50 Kbps | Very Low | Long-range, low-power, low-data rate   | Agriculture, smart cities, remote monitoring |
| **NB-IoT**    | Licensed LTE bands    | 10-20 km (urban)      | 20 Kbps - 250 Kbps  | Low                   | Cellular IoT, wide-area connectivity   | Smart metering, asset tracking, smart agriculture   |
| **Sigfox**    | Sub-GHz (868 MHz, 915 MHz) | 10-50 km (line-of-sight) | 100 bps - 600 bps   | Very Low      | Long-range, low-data rate, low-power   | Asset tracking, environmental monitoring, smart agriculture |
| **Thread**    | 2.4 GHz               | 10-100m               | 250 Kbps            | Low                   | Mesh network for home automation       | Smart home devices, lighting, thermostats            |

## Key Takeaways:
- **Wi-Fi** is suitable for high-speed, high-bandwidth IoT applications but consumes more power and has a shorter range.
- **Bluetooth** is commonly used for short-range, low-power applications with relatively low data transfer needs.
- **Zigbee** and **Z-Wave** are ideal for low-power, low-data rate applications like smart home automation and sensors.
- **LoRaWAN** and **Sigfox** are great for long-range, low-power applications where a small amount of data needs to be sent over a large area.
- **NB-IoT** offers a cellular solution with reliable coverage for applications needing low to medium data rates over wide areas.
- **Thread** is designed for secure, low-power, and scalable mesh networks, perfect for smart homes and connected devices.

---
