<p>This project demonstrates how to create a <strong>Remote Temperature Monitoring System</strong> using the <strong>ESP8266</strong> Wi-Fi module, paired with the <strong>DHT11 sensor</strong> for temperature measurement, and integrated with <strong>Telegram</strong> for real-time updates and remote control. The system enables users to track temperature remotely, set custom alerts for temperature ranges, and control connected devices like an LED from anywhere in the world using simple Telegram commands.</p>
<h3><strong>Overview:</strong></h3>
<p>The system allows continuous monitoring of environmental temperature and can trigger notifications if the temperature exceeds or drops below a user-defined range. This is achieved via a Telegram bot that interacts with the system, enabling remote control and management. It also tracks the battery level, ensuring you're always informed of your device&rsquo;s power status. Additionally, the device is designed with <strong>deep sleep mode</strong> for power-saving between readings and supports <strong>Over-the-Air (OTA) updates</strong>, so firmware can be easily updated without needing to connect the device physically.</p>
<h3><strong>Key Features:</strong></h3>
<ul>
<li><strong>Remote Temperature Monitoring</strong>: With a simple Telegram interface, you can access real-time temperature data from anywhere in the world.</li>
<li><strong>Custom Temperature Alerts</strong>: Set a minimum and maximum temperature threshold. The system will notify you if the temperature moves beyond these limits, ideal for environments that require stable conditions (e.g., server rooms, greenhouses).</li>
<li><strong>Telegram Integration</strong>: The system uses a Telegram bot to send messages, and accept commands for controlling the device (e.g., turning the LED on/off or checking battery status).</li>
<li><strong>Battery Monitoring</strong>: Get notifications when the battery drops below 35%, ensuring you don&rsquo;t get caught off guard.</li>
<li><strong>Power Efficiency</strong>: The ESP8266 enters deep sleep mode after performing its tasks, extending battery life for long-term use.</li>
<li><strong>OTA Firmware Updates</strong>: With <strong>ArduinoOTA</strong>, firmware can be updated wirelessly, saving you the hassle of connecting the device to a computer every time you need to upgrade.</li>
</ul>
<h3><strong>How It Works:</strong></h3>
<ol>
<li><strong>Sensor Data Collection</strong>: The <strong>DHT11 sensor</strong> measures the temperature, sending data to the <strong>ESP8266</strong>, which processes the information.</li>
<li><strong>Telegram Bot Communication</strong>: The ESP8266 communicates with a <strong>Telegram bot</strong>, sending temperature updates and receiving commands from the user. You can access and control the system directly from your smartphone or another Telegram-enabled device.</li>
<li><strong>Battery Level Monitoring</strong>: The system keeps track of the battery voltage. When the battery is low (below 35%), it will notify the user via Telegram.</li>
<li><strong>Sleep Mode</strong>: After each task cycle, the ESP8266 enters a <strong>deep sleep mode</strong> to save power. This feature is especially useful for battery-powered applications where charging or replacing batteries is not practical.</li>
<li><strong>Over-the-Air (OTA) Updates</strong>: This system includes OTA update functionality. When the firmware is updated, you can send the new code to the ESP8266 over Wi-Fi, removing the need for physical connections.</li>
</ol>
<h3><strong>Step-by-Step Operation:</strong></h3>
<ol>
<li><strong>Initial Setup</strong>: The system connects to your <strong>Wi-Fi network</strong>. Once online, the ESP8266 can communicate with Telegram, send temperature updates, and receive commands.</li>
<li><strong>Monitoring Temperature</strong>: The <strong>DHT11 sensor</strong> continuously monitors the temperature, sending the data to the ESP8266.</li>
<li><strong>Telegram Bot Updates</strong>: Every time the temperature changes, the ESP8266 updates the Telegram bot, which sends you the current temperature via your phone.</li>
<li><strong>User Commands</strong>: You can send specific commands to the bot, such as:
<ul>
<li><code>/ledon</code> to turn the LED on</li>
<li><code>/ledoff</code> to turn the LED off</li>
<li><code>/Tempratue</code> to get the current temperature reading</li>
<li><code>/BatteryLevel</code> to check the battery percentage</li>
</ul>
</li>
<li><strong>Set Alerts</strong>: You can define temperature thresholds, such as setting a maximum or minimum temperature. If the system detects that the temperature exceeds or falls below these values, it will notify you immediately.</li>
<li><strong>Power Saving Mode</strong>: Once the system has completed its task cycle, it enters deep sleep mode, waking up after a predefined interval to conserve energy. This allows for a long battery life, perfect for remote or outdoor monitoring where power outlets are not available.</li>
<li><strong>Updating Firmware</strong>: When updates are required, simply use the OTA feature to send new firmware updates to the ESP8266 without connecting it to a computer.</li>
</ol>
<h3><strong>Materials Needed:</strong></h3>
<ul>
<li><strong>ESP8266 Microcontroller</strong>: The core of the system, it connects to Wi-Fi and runs the entire monitoring and control process. (e.g., NodeMCU, Wemos D1 Mini).</li>
<li><strong>DHT11 Temperature and Humidity Sensor</strong>: A simple sensor for measuring temperature and humidity, essential for the monitoring system.</li>
<li><strong>LED</strong>: An optional visual indicator to show system status or to control devices (such as fans, lights, or alarms).</li>
<li><strong>Battery (Li-ion or LiPo)</strong>: A rechargeable battery that powers the ESP8266 and sensor. A typical choice is a <strong>3.7V Li-ion battery</strong>.</li>
<li><strong>Battery Charging Module (TP4056)</strong>: A charging module that allows you to safely charge the Li-ion battery using a micro-USB cable.</li>
<li><strong>Telegram Bot Token</strong>: A Telegram bot token to allow communication with the ESP8266. This bot is created using <strong>BotFather</strong> on Telegram.</li>
<li><strong>Resistors &amp; Wires</strong>: Basic components like resistors (10k&Omega; for the sensor) and jumper wires for connecting the hardware components.</li>
</ul>
<h3><strong>Telegram Bot Commands:</strong></h3>
<ul>
<li><code>/ledon</code>: Turns the connected LED on.</li>
<li><code>/ledoff</code>: Turns the connected LED off.</li>
<li><code>/Temperature</code>: Retrieves the current temperature from the DHT11 sensor.</li>
<li><code>/TemperatureRange Min Max</code>: Sets the minimum and maximum allowable temperature ranges. Alerts will be sent if the temperature goes outside these bounds.</li>
<li><code>/BatteryLevel</code>: Displays the current battery percentage.</li>
<li><code>/OTA</code>: Initiates the <strong>Over-the-Air</strong> update process for firmware.</li>
<li><code>/sleep Time</code>: Puts the ESP8266 into deep sleep for a specified duration.</li>
</ul>
<h3><strong>Applications:</strong></h3>
<ul>
<li><strong>Home Automation</strong>: Integrate this system into your smart home to control climate-sensitive devices like thermostats, heaters, or fans.</li>
<li><strong>Remote Environmental Monitoring</strong>: Ideal for monitoring the temperature in greenhouses, server rooms, or remote locations where direct access is difficult.</li>
<li><strong>Battery-Powered Devices</strong>: The deep sleep feature makes this system ideal for battery-powered applications that need long operational periods without frequent recharging.</li>
<li><strong>Temperature Alert System</strong>: Set up alerts for temperature-sensitive environments (e.g., refrigerated storage, agricultural operations) to be notified instantly if conditions change.</li>
</ul>
<h3><strong>Conclusion:</strong></h3>
<p>This <strong>Remote Temperature Monitoring System</strong> offers a simple yet powerful solution for monitoring environmental conditions remotely via Telegram. With <strong>customizable alerts</strong>, <strong>battery saving features</strong>, and <strong>OTA updates</strong>, this project is perfect for anyone looking to keep track of temperature in real-time, while maintaining efficiency and flexibility. Whether you&rsquo;re managing a smart home, greenhouse, or remote sensors, this system provides an easy-to-use, reliable way to monitor and control temperature with minimal effort and cost.</p>
<p>&nbsp;</p>
