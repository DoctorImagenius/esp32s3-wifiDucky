# 🧠 Imagenius WiFi Ducky — ESP32-S3 Rubber Ducky Over WiFi

Part of the [**Imagenius Cube**], this project turns an **ESP32-S3 board** into a **WiFi Rubber Ducky** — a remotely controlled HID (Human Interface Device) tool for automation, pen-testing, and ethical hacking purposes.

> ⚠️ For educational and ethical hacking **only**. Do not use this on systems you do not own or have explicit permission to test.

---

## 📦 Features

✅ Host a full **WiFi AP + Web Interface** on the ESP32-S3  
✅ Send **real-time keystrokes or strings** to the connected PC  
✅ Write and run **Ducky Scripts** from a web-based editor  
✅ Save, load, delete, and auto-run scripts on boot  
✅ Built-in **Virtual Keyboard** with one-click key injection  
✅ Styled with a responsive and dark-themed UI  
✅ No internet or software needed — just power and a browser

---

## 🌐 How It Works

1. The ESP32-S3 creates a WiFi Access Point (SSID: `iMagenius Ducky`).
2. You connect to it using your phone or laptop.
3. Open the browser and visit: `192.168.4.1`
4. Use the interface to:
   - Send key commands
   - Write/run Ducky Script
   - Save/load scripts
   - Auto-execute payloads on boot

---

## 🛠️ Requirements

- **ESP32-S3** Dev Board (must support USB HID)
- **Arduino IDE**
- Required libraries:
  - `WiFi.h`
  - `WebServer.h`
  - `Preferences.h`
  - `USB.h`
  - `USBHIDKeyboard.h`

---

## 🚀 Getting Started

1. Clone this repo or copy the code into Arduino IDE.
2. Select your **ESP32-S3** board under:
   `Tools > Board > ESP32 Arduino > ESP32S3 Dev Module`
3. Connect your ESP32-S3 via USB.
4. Upload the code.
5. After upload:
   - Connect to WiFi `iMagenius Ducky` (Password: `12345678`)
   - Open browser and visit: `http://192.168.4.1`

---

## 💡 What is Imagenius Cube?

**Imagenius Cube** is a multi-functional ethical hacking toolkit developed by [Haroon Babar](https://github.com/DoctorImagenius).  
It includes:

- Persistent Live Kali Linux setup
- USB data storage with payloads and tools
- A custom-built **WiFi Rubber Ducky** using ESP32-S3
- HID attack support via **ATTiny85-based Rubber Ducky**
- Built-in WiFi Extender for extended network access during tests
- Compact and portable hardware built into a cube design

This `WiFi Ducky` project is just **one powerful feature** of the full Imagenius Cube system.

---

## 🔐 Ethical Use Notice

This project is intended for **educational purposes** and **authorized penetration testing only**.  
Misuse can be illegal.  
Use it **only** on systems you have permission to test.

---

## 🤝 Contributing

Pull requests, issues, and ideas are always welcome!  
If you improve this project or build something cool on top of it — feel free to contribute or share.

---

## 📃 License

This project is released under the MIT License.  
See [LICENSE](LICENSE) for details.

---

## 📫 Author

**Haroon Babar**  
📧 imagenius1001@gmail.com  
🌐 [imagenius1001.netlify.app](https://imagenius1001.netlify.app)  
🔗 [GitHub](https://github.com/DoctorImagenius) | [LinkedIn](https://www.linkedin.com/in/haroon-babar-imagenius1001)

---

