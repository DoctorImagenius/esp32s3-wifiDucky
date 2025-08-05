#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

#define KEY_INSERT        0x49
#define KEY_HOME          0x4A
#define KEY_PAGE_UP       0x4B
#define KEY_DELETE        0x4C
#define KEY_END           0x4D
#define KEY_PAGE_DOWN     0x4E
#define KEY_RIGHT_ARROW   0x4F
#define KEY_LEFT_ARROW    0x50
#define KEY_DOWN_ARROW    0x51
#define KEY_UP_ARROW      0x52
#define KEY_NUM_LOCK      0x53
#define KEY_PRINT_SCREEN  0x46
#define KEY_SCROLL_LOCK   0x47
#define KEY_PAUSE         0x48
#define KEY_CAPS_LOCK     0x39

const char* ssid = "iMagenius Ducky";
const char* password = "12345678";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 WiFi Rubber Ducky</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      background-color: #121212;
      color: #f1f1f1;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      margin: 0;
      padding: 20px;
    }
    h2, h3 {
      text-align: center;
      color: #00ffc8;
    }
    .container {
      max-width: 900px;
      margin: auto;
      padding: 20px;
      background: #1e1e1e;
      border-radius: 10px;
    }
    input, textarea, select, button {
      width: 100%;
      padding: 10px;
      margin: 8px 0;
      border-radius: 5px;
      font-size: 16px;
      border: none;
      box-sizing: border-box;
    }
    input, textarea, select {
      background: #292929;
      color: #fff;
    }
    button {
      background-color: #00ffc8;
      color: #000;
      font-weight: bold;
      cursor: pointer;
      transition: background 0.3s;
    }
    button:hover {
      background-color: #00cc9e;
    }
    .keyboard {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 5px;
    }
    .keyboard button {
      width: auto;
      min-width: 60px;
      padding: 8px;
      font-size: 14px;
      white-space: nowrap;
    }
    .row {
      display: flex;
      flex-wrap: wrap;
      gap: 10px;
      justify-content: center;
      margin-top: 10px;
    }
    .snippets button {
      width: auto;
      padding: 6px 12px;
    }
    .section {
      margin-top: 30px;
    }
    .label-row {
      margin-top: 10px;
      text-align: left;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>iMagenius WiFi Ducky</h2>

    <div class="section">
      <h3>Send Key Command</h3>
      <input type="text" id="command" placeholder="Type a key, string or combo (e.g., CTRL ALT DEL)" />
      <button onclick="sendCommand()">Send</button>
    </div>

    <div class="section">
      <h3>Virtual Keyboard</h3>
      <div class="keyboard" id="keyboard"></div>
    </div>

    <div class="section">
      <h3>Ducky Script Editor</h3>
      <textarea id="scriptArea" placeholder="Type or paste your Ducky Script here..."></textarea>
      <div class="snippets row">
        <button onclick="insert('STRING ')">STRING</button>
        <button onclick="insert('DELAY 1000')">DELAY</button>
        <button onclick="insert('ENTER')">ENTER</button>
        <button onclick="insert('TAB')">TAB</button>
        <button onclick="insert('SPACE')">SPACE</button>
        <button onclick="insert('ESCAPE')">ESCAPE</button>
        <button onclick="insert('BACKSPACE')">BACKSPACE</button>
        <button onclick="insert('INSERT')">INSERT</button>
        <button onclick="insert('DELETE')">DELETE</button>
        <button onclick="insert('HOME')">HOME</button>
        <button onclick="insert('END')">END</button>
        <button onclick="insert('PAGEUP')">PAGEUP</button>
        <button onclick="insert('PAGEDOWN')">PAGEDOWN</button>
        <button onclick="insert('UPARROW')">UPARROW</button>
        <button onclick="insert('DOWNARROW')">DOWNARROW</button>
        <button onclick="insert('LEFTARROW')">LEFTARROW</button>
        <button onclick="insert('RIGHTARROW')">RIGHTARROW</button>
        <button onclick="insert('CAPSLOCK')">CAPSLOCK</button>
        <button onclick="insert('NUMLOCK')">NUMLOCK</button>
        <button onclick="insert('SCROLLLOCK')">SCROLLLOCK</button>
        <button onclick="insert('PRINTSCREEN')">PRINTSCREEN</button>
        <button onclick="insert('PAUSE')">PAUSE</button>
        <button onclick="insert('BREAK')">BREAK</button>
        <button onclick="insert('CTRL')">CTRL</button>
        <button onclick="insert('ALT')">ALT</button>
        <button onclick="insert('SHIFT')">SHIFT</button>
        <button onclick="insert('GUI')">GUI</button>
        <button onclick="insert('WINDOWS')">WINDOWS</button>
        <button onclick="insert('REM ')">REM</button>
      </div>
    </div>

    <div class="section">
      <h3>Script Management</h3>
      <input type="text" id="scriptName" placeholder="Enter script name to save..." />
      <button onclick="saveScript()">Save Script</button>

      <select id="scriptList"></select>
      <div class="row">
        <button onclick="loadScript()">Load</button>
        <button onclick="editScript()">Edit</button>
        <button onclick="deleteScript()">Delete</button>
        <button onclick="document.getElementById('scriptArea').value = '';">Clear</button>
        <button onclick="runScript()">Run</button>
      </div>
    </div>

    <div class="section">
      <h3>Auto Run Settings</h3>
      <select id="autoSelect"></select>
      <label><input type="checkbox" id="autoEnable" onchange="setAuto()"> Enable Auto Run</label>
    </div>
  </div>

  <script>
    const keyboardKeys = [
      "ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
      "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE",
      "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\",
      "CAPSLOCK", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "ENTER",
      "SHIFT", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "SHIFT",
      "CTRL", "ALT", "SPACE", "GUI"
    ];
    function loadKeyboard() {
      const kb = document.getElementById("keyboard");
      keyboardKeys.forEach(k => {
        const btn = document.createElement("button");
        btn.innerText = k;
        btn.onclick = () => sendKey(k);
        kb.appendChild(btn);
      });
    }

    function insert(txt) {
      let area = document.getElementById("scriptArea");
      area.value += (area.value ? "\n" : "") + txt;
    }

    function sendKey(k) {
      fetch("/send?cmd=" + encodeURIComponent(k));
    }

    function sendCommand() {
      const cmd = document.getElementById("command").value;
      fetch("/send?cmd=" + encodeURIComponent(cmd));
      document.getElementById("command").value = "";
    }

    function saveScript() {
      const name = document.getElementById("scriptName").value.trim();
      if (!name) return;
      const content = document.getElementById("scriptArea").value;
      fetch("/save", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: `name=${encodeURIComponent(name)}&script=${encodeURIComponent(content)}`
      }).then(() => {
        document.getElementById("scriptName").value = "";
        refreshScripts();
      });
    }

    function loadScript() {
      const name = document.getElementById("scriptList").value;
      fetch("/load?name=" + encodeURIComponent(name))
        .then(res => res.text())
        .then(text => document.getElementById("scriptArea").value = text);
    }

    function deleteScript() {
      const name = document.getElementById("scriptList").value;
      fetch("/delete?name=" + encodeURIComponent(name), { method: "POST" })
        .then(refreshScripts);
    }

    function editScript() {
      const name = document.getElementById("scriptList").value;
      document.getElementById("scriptName").value = name;
      loadScript();
    }

    function runScript() {
      const content = document.getElementById("scriptArea").value;
      fetch("/run", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: `script=${encodeURIComponent(content)}`
      });
    }

    function setAuto() {
      const name = document.getElementById("autoSelect").value;
      const enabled = document.getElementById("autoEnable").checked ? "1" : "0";
      fetch("/auto", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: `name=${encodeURIComponent(name)}&enable=${enabled}`
      });
    }

    function refreshScripts() {
      fetch("/list").then(res => res.json()).then(list => {
        const sList = document.getElementById("scriptList");
        const aList = document.getElementById("autoSelect");
        sList.innerHTML = ""; aList.innerHTML = "";
        list.forEach(name => {
          let o1 = new Option(name, name);
          let o2 = new Option(name, name);
          sList.appendChild(o1);
          aList.appendChild(o2);
        });
        fetch("/autoStatus").then(res => res.json()).then(data => {
          document.getElementById("autoSelect").value = data.name || "";
          document.getElementById("autoEnable").checked = data.enabled || false;
        });
      });
    }

    loadKeyboard();
    refreshScripts();
  </script>
</body>
</html>
)rawliteral";


WebServer server(80);
USBHIDKeyboard Keyboard;
Preferences prefs;

bool hasRun = false;
String autoScriptName = "";
bool autoEnabled = false;

String getScriptList() {
  prefs.begin("wifi-duck", true);
  String all = prefs.getString("script_list", "");
  prefs.end();
  return all;
}

void addToScriptList(String name) {
  prefs.begin("wifi-duck", false);
  String all = prefs.getString("script_list", "");

  // Normalize string to search in a safe way
  String formatted = "," + all + ",";
  if (formatted.indexOf("," + name + ",") == -1) {
    if (all.length() > 0) all += ",";
    all += name;
    prefs.putString("script_list", all);
  }
  prefs.end();
}


void removeFromScriptList(String name) {
  prefs.begin("wifi-duck", false);
  String all = prefs.getString("script_list", "");
  all.replace("," + name, "");
  if (all.startsWith(name + ",")) all = all.substring(name.length() + 1);
  if (all == name) all = "";
  prefs.putString("script_list", all);
  prefs.end();
}

void parseDuckyScript(String script) {
  script.replace("\r", "");
  while (script.length()) {
    int idx = script.indexOf('\n');
    String line = (idx != -1) ? script.substring(0, idx) : script;
    script = (idx != -1) ? script.substring(idx + 1) : "";

    line.trim();
    if (line.length() == 0 || line.startsWith("REM")) continue;
    if (line.startsWith("STRING ")) Keyboard.print(line.substring(7));
    else if (line.startsWith("DELAY ")) delay(line.substring(6).toInt());
    else if (line == "ESCAPE") { Keyboard.press(KEY_ESC); delay(5); Keyboard.release(KEY_ESC); }
    else if (line == "INSERT") { Keyboard.press(KEY_INSERT); delay(5); Keyboard.release(KEY_INSERT); }
    else if (line == "DELETE") { Keyboard.press(KEY_DELETE); delay(5); Keyboard.release(KEY_DELETE); }
    else if (line == "HOME") { Keyboard.press(KEY_HOME); delay(5); Keyboard.release(KEY_HOME); }
    else if (line == "END") { Keyboard.press(KEY_END); delay(5); Keyboard.release(KEY_END); }
    else if (line == "PAGEUP") { Keyboard.press(KEY_PAGE_UP); delay(5); Keyboard.release(KEY_PAGE_UP); }
    else if (line == "PAGEDOWN") { Keyboard.press(KEY_PAGE_DOWN); delay(5); Keyboard.release(KEY_PAGE_DOWN); }
    else if (line == "UPARROW") { Keyboard.press(KEY_UP_ARROW); delay(5); Keyboard.release(KEY_UP_ARROW); }
    else if (line == "DOWNARROW") { Keyboard.press(KEY_DOWN_ARROW); delay(5); Keyboard.release(KEY_DOWN_ARROW); }
    else if (line == "LEFTARROW") { Keyboard.press(KEY_LEFT_ARROW); delay(5); Keyboard.release(KEY_LEFT_ARROW); }
    else if (line == "RIGHTARROW") { Keyboard.press(KEY_RIGHT_ARROW); delay(5); Keyboard.release(KEY_RIGHT_ARROW); }
    else if (line == "CAPSLOCK") { Keyboard.press(KEY_CAPS_LOCK); delay(5); Keyboard.release(KEY_CAPS_LOCK); }
    else if (line == "NUMLOCK") { Keyboard.press(KEY_NUM_LOCK); delay(5); Keyboard.release(KEY_NUM_LOCK); }
    else if (line == "SCROLLLOCK") { Keyboard.press(KEY_SCROLL_LOCK); delay(5); Keyboard.release(KEY_SCROLL_LOCK); }
    else if (line == "PRINTSCREEN") { Keyboard.press(KEY_PRINT_SCREEN); delay(5); Keyboard.release(KEY_PRINT_SCREEN); }
    else if (line == "PAUSE" || line == "BREAK") { Keyboard.press(KEY_PAUSE); delay(5); Keyboard.release(KEY_PAUSE); }

    else if (line == "ENTER") { Keyboard.press(KEY_RETURN); delay(5); Keyboard.release(KEY_RETURN); }
    else if (line == "TAB") { Keyboard.press(KEY_TAB); delay(5); Keyboard.release(KEY_TAB); }
    else if (line == "SPACE") Keyboard.write(' ');
    else if (line == "BACKSPACE") { Keyboard.press(KEY_BACKSPACE); delay(5); Keyboard.release(KEY_BACKSPACE); }
    else if (line.startsWith("CTRL")) { Keyboard.press(KEY_LEFT_CTRL); delay(5); Keyboard.releaseAll(); }
    else if (line.startsWith("ALT")) { Keyboard.press(KEY_LEFT_ALT); delay(5); Keyboard.releaseAll(); }
    else if (line.startsWith("GUI ")) {
      String key = line.substring(4);
      Keyboard.press(KEY_LEFT_GUI);
      delay(5);
      Keyboard.press(key.charAt(0));
      delay(5);
      Keyboard.releaseAll();
    }
    else if (line == "GUI") {
      Keyboard.press(KEY_LEFT_GUI);
      delay(5);
      Keyboard.releaseAll();
    }
    else for (char c : line) { Keyboard.write(c); delay(5); }
  }
}

void setup() {
  Serial.begin(115200);
  USB.begin();
  Keyboard.begin();
  WiFi.softAP(ssid, password);

  prefs.begin("wifi-duck", true);
  autoEnabled = prefs.getBool("auto_en", false);
  autoScriptName = prefs.getString("auto_name", "");
  String key = "script_" + autoScriptName;
  String autoScript = prefs.getString(key.c_str(), "");
  prefs.end();

  if (autoEnabled && !hasRun && autoScript.length() > 0) {
    delay(100);
    parseDuckyScript(autoScript);
    hasRun = true;
  }

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  
server.on("/send", HTTP_GET, []() {
 String cmd = server.arg("cmd");
  cmd.trim();

  // Prepare uppercase version for comparison
  String cmdUpper = cmd;
  cmdUpper.toUpperCase();

  // If it's a plain string (no spaces and lowercase), or starts with STRING
  if (cmd.startsWith("STRING ") || (cmd.indexOf(" ") == -1 && cmd != cmdUpper)) {
    if (cmd.startsWith("STRING ")) {
      Keyboard.print(cmd.substring(7));
      server.send(200, "text/plain", "Sent string: " + cmd.substring(7));
    } else {
      Keyboard.print(cmd);
      server.send(200, "text/plain", "Sent string: " + cmd);
    }
    return;
  }

  // Otherwise, treat as key combo
  std::vector<String> parts;
  int spaceIndex;
  while ((spaceIndex = cmd.indexOf(' ')) != -1) {
    parts.push_back(cmd.substring(0, spaceIndex));
    cmd = cmd.substring(spaceIndex + 1);
  }
  if (cmd.length()) parts.push_back(cmd);

  for (String &p : parts) p.toUpperCase();

  for (String &p : parts) {
    if (p == "CTRL") Keyboard.press(KEY_LEFT_CTRL);
    else if (p == "ALT") Keyboard.press(KEY_LEFT_ALT);
    else if (p == "SHIFT") Keyboard.press(KEY_LEFT_SHIFT);
    else if (p == "GUI" || p == "WINDOWS") Keyboard.press(KEY_LEFT_GUI);
  }

  String last = parts.back();
  if (last == "ENTER") Keyboard.press(KEY_RETURN);
  else if (last == "TAB") Keyboard.press(KEY_TAB);
  else if (last == "SPACE") Keyboard.write(' ');
  else if (last == "BACKSPACE") Keyboard.press(KEY_BACKSPACE);
  else if (last == "ESCAPE") Keyboard.press(KEY_ESC);
  else if (last == "INSERT") Keyboard.press(KEY_INSERT);
  else if (last == "DELETE") Keyboard.press(KEY_DELETE);
  else if (last == "HOME") Keyboard.press(KEY_HOME);
  else if (last == "END") Keyboard.press(KEY_END);
  else if (last == "PAGEUP") Keyboard.press(KEY_PAGE_UP);
  else if (last == "PAGEDOWN") Keyboard.press(KEY_PAGE_DOWN);
  else if (last == "UPARROW") Keyboard.press(KEY_UP_ARROW);
  else if (last == "DOWNARROW") Keyboard.press(KEY_DOWN_ARROW);
  else if (last == "LEFTARROW") Keyboard.press(KEY_LEFT_ARROW);
  else if (last == "RIGHTARROW") Keyboard.press(KEY_RIGHT_ARROW);
  else if (last == "CAPSLOCK") Keyboard.press(KEY_CAPS_LOCK);
  else if (last == "NUMLOCK") Keyboard.press(KEY_NUM_LOCK);
  else if (last == "SCROLLLOCK") Keyboard.press(KEY_SCROLL_LOCK);
  else if (last == "PRINTSCREEN") Keyboard.press(KEY_PRINT_SCREEN);
  else if (last == "PAUSE" || last == "BREAK") Keyboard.press(KEY_PAUSE);
  else if (last.length() == 1) Keyboard.press(last.charAt(0));

  delay(10);
  Keyboard.releaseAll();
  server.send(200, "text/plain", "Key combo sent: " + last);
});




  server.on("/save", HTTP_POST, []() {
    String name = server.arg("name");
    String script = server.arg("script");
    String key = "script_" + name;
    prefs.begin("wifi-duck", false);
    prefs.putString(key.c_str(), script);
    prefs.end();
    addToScriptList(name);
    server.send(200, "text/plain", "Saved");
  });
  
  server.on("/run", HTTP_POST, []() {
    String script = server.arg("script");
    parseDuckyScript(script);
    server.send(200, "text/plain", "Script executed");
  });

  server.on("/load", HTTP_GET, []() {
    String name = server.arg("name");
    String key = "script_" + name;
    prefs.begin("wifi-duck", true);
    String script = prefs.getString(key.c_str(), "");
    prefs.end();
    server.send(200, "text/plain", script);
  });

  server.on("/delete", HTTP_POST, []() {
    String name = server.arg("name");
    String key = "script_" + name;
    prefs.begin("wifi-duck", false);
    prefs.remove(key.c_str());
    prefs.end();
    removeFromScriptList(name);
    server.send(200, "text/plain", "Deleted");
  });

  server.on("/list", HTTP_GET, []() {
    String all = getScriptList();
    String json = "[";
    int idx = 0;
    while ((idx = all.indexOf(',')) != -1) {
      json += "\"" + all.substring(0, idx) + "\",";
      all = all.substring(idx + 1);
    }
    if (all.length()) json += "\"" + all + "\"";
    if (json.endsWith(",")) json.remove(json.length() - 1);
    json += "]";
    server.send(200, "application/json", json);
  });

  server.on("/auto", HTTP_POST, []() {
    prefs.begin("wifi-duck", false);
    prefs.putBool("auto_en", server.arg("enable") == "1");
    prefs.putString("auto_name", server.arg("name"));
    prefs.end();
    server.send(200, "text/plain", "Auto updated");
  });

  server.on("/autoStatus", HTTP_GET, []() {
    prefs.begin("wifi-duck", true);
    bool en = prefs.getBool("auto_en", false);
    String name = prefs.getString("auto_name", "");
    prefs.end();
    server.send(200, "application/json", "{\"enabled\":" + String(en ? "true" : "false") + ",\"name\":\"" + name + "\"}");
  });

  server.begin();
  Serial.println("WiFi Duck Ready on IP: " + WiFi.softAPIP().toString());
}

void loop() {
  server.handleClient();
}
