#define _PREFIX "sensor"       // the prefix for the sensor Access Points
#define _SENSOR_CHANNEL 1      // WiFi channel used by sensors.  0 if unknown or multiple channels
#define _SSID ""               // the ssid of your wifi router
#define _PASSWORD ""           // the password for above
#define _HOST "train01"        // the name of this scanner
#define _TRAIN_NUM 1           // the train number of this scanner
#define _SCAN_INTERVAL  500    // minimum time between scans
#define _MAX_SERVER_NAMES 10   // maximum number of server scanner names
#define _SERVER_NAMES "DCCpp32", "sensor01", "sensor02" // servers where status updates are delivered
#define _RECONNECT_INTERVAL 10000 // time to wait, in milliseconds, before trying to reconnect to an unresponsive server
#define _MAX_RETRIES  10       // maximum times that the scanner will try to reach a given server
#define _SPEED_CHANGE_RATE  10  // maximum speed change in % per second
#define _ETHERNET_PORT 2560

#define _IR_PIN       2
#define _BRAKE_PIN    0
#define _LEFT_PIN     12
#define _RIGHT_PIN    13
#define _HEAD_PIN     14
#define _TAIL_PIN     16
#define _IR_SENS_PIN  3
#define _SENS2_PIN    4
#define _SENS3_PIN    5

