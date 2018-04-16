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
