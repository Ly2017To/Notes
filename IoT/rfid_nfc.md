# RFID Technology Overview

RFID (Radio Frequency Identification) technology is widely applied in various fields, including logistics, transportation, library management, and retail. The application system consists of electronic tags, readers, and application software. The electronic tags and readers communicate via radio frequency in a non-contact, bidirectional manner. The reader reads the information stored in the electronic tag, and the application software analyzes and processes the data read by the reader.

## RFID Electronic Tags Types

According to their working mode, RFID electronic tags can be divided into three categories:

1. **Active RFID Tags**: These tags are powered by batteries and actively transmit signals.
2. **Passive RFID Tags**: These tags do not have batteries and are activated by the signal received from the reader. They are powered by the energy from the reader’s signal.
3. **Semi-active RFID Tags**: These tags generally remain in a low-power sleep state and only activate when a signal is received, entering an operational state.

## RFID Electronic Tags Frequency Types

Based on their operating frequency, RFID electronic tags can be categorized into the following types:

1. **Low Frequency (LF) Tags**: Operating frequency range from 125KHz to 133KHz.
2. **High Frequency (HF) Tags**: Operating frequency of 13.56MHz.
3. **Ultra High Frequency (UHF) Tags**: Operating frequency range from 860MHz to 960MHz.
4. **Active RFID Tags**: Operating frequency range from 2.45GHz to 5.8GHz.

## Standards for RFID Frequency Ranges

# RFID Frequency Range and Standards

| **Frequency** | **Operating Frequency** | **Tag Type**   | **Reading Distance** | **Typical Applications**                                         | **Protocols**                                                     |
|---------------|-------------------------|----------------|----------------------|------------------------------------------------------------------|-------------------------------------------------------------------|
| **LF**        | 125kHz; 133kHz          | Passive        | < 1m                 | Animal identification; Container identification; Tool identification; Electronic locking & anti-theft systems | ISO 11784, ISO 11785, ISO 18000-2 |
| **HF**        | 13.56MHz                | Passive        | < 1m                 | Electronic tickets; Electronic ID cards; Access control systems; Electronic locking & anti-theft systems | ISO 14443, ISO 15693, ISO 18000-3, EPC HF Class 1, EPC HF Version 2, GB/T 33848.3-2017 |
| **UHF**       | 433MHz; 860MHz ~ 960MHz | Passive/Active | 4m ~ 6m; > 10m       | Asset management; Item tracking; Highway vehicle identification & automatic tolling | ISO 18000-7, ISO 18000-6C/6B, EPC UHF Class 0&1, EPC Class I Generation 2, GB/T 29768-2013 |
| **Microwave** | 2.45GHz; 5.8GHz         | Passive/Active | 1m ~ 15m             | Mobile vehicle identification; Electronic locking & anti-theft systems; Medical research | ISO 18000-4 Mode 1&2        |

# NFC (Near Field Communication) Overview

NFC (Near Field Communication) is a short-range wireless communication technology operating at a frequency of 13.56 MHz. It is based on a combination of RFID technology and wireless interconnection technology. NFC integrates functions of a contactless card reader, contactless card, and peer-to-peer communication into a single chip. It is backward compatible with RFID technology, and digital devices supporting this technology can perform contactless point-to-point data transfer at short distances (less than or equal to 10 cm). 

The data exchange mode of NFC includes two types: active mode and passive mode. 

- In **passive mode**, the device uses the energy from the radio frequency field provided by the NFC communication initiator and sends data to the initiator using load modulation. The data transmission rate can be one of the following three options: 106 kbps, 212 kbps, and 424 kbps.
  
- In **active mode**, the device generates its own radio frequency field and sends data to the device with which it is communicating. The data transmission rate options include 856 kbps, 1695 kbps, and others.

Common NFC technology standard protocols include **ISO 18092** and **ISO 14443**.

## NFC Work Modes and Applications

| **Mode**                | **Explanation**                                                      | **Applications**                                           |
|-------------------------|----------------------------------------------------------------------|------------------------------------------------------------|
| **Card Mode**           | Uses RFID technology in an IC card, similar to an RFID tag           | Access control; Tickets; Event tickets                     |
| **Reader Mode**         | Functions as a contactless card reader, similar to an RFID tag reader| Information browsing                                       |
| **Point-to-Point Mode** | Two NFC-enabled devices in active mode, performing data transfer     | Data exchange                                              |
