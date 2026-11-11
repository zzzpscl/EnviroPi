#include <WiFi.h>
#include <HTTPClient.h>
#include <lvgl.h>
#include "Arduino_GFX_Library.h"
#include "lv_conf.h"
#include <NetworkClient.h>
#include <ArduinoJson.h>

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2
#define LCD_DC     15
#define LCD_CS     14
#define LCD_SCK     7
#define LCD_MOSI    6
#define LCD_RST    21
#define LCD_BL     22
#define COL_OFFSET1  30
#define ROW_OFFSET1  0
#define COL_OFFSET2 0
#define ROW_OFFSET2  0
#define LCD_WIDTH  172
#define LCD_HEIGHT 320

const char* fake_data =
    R"({"timestamp":"2026-10-12T09:15:00","server_time":"2026-10-12T09:15:42","temperature":22.4,"humidity":48.2,"pressure":1013.6,"co2":845,"gas_raw":null})";

const char* ssid = "KBMRaspberry"; // Replace with your Wi-Fi name
const char* password = "BonbonbuntesBananeneis4711."; // Replace with your Wi-Fi password

static const uint16_t screenWidth = LCD_WIDTH;
static const uint16_t screenHeight = LCD_HEIGHT;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];
static lv_style_t style_large_white_font;

/* LCD - Display Hardware Interface Definitions */
Arduino_DataBus* bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);
/*maybe wrong order on the offsets the IDE IS SO BAAADDD*/
Arduino_GFX* gfx = new Arduino_ST7789(bus, LCD_RST /* RST */, 0 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT,
                                      COL_OFFSET1, ROW_OFFSET1, COL_OFFSET2, ROW_OFFSET2);


//Your Domain name with URL path or IP address with path
String serverName = "http://172.16.9.173:5000/api/current";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 5 minutes (300000)
//unsigned long timerDelay = 300000;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 30000;
// Set timer to 5 seconds (5000)
//unsigned long timerDelay = 5000;

void setup()
{
    Serial.begin(115200);
    // 1. Hardware Initialization
    // It is recommended to group all initialization code directly related to physical hardware together.
    // Initialize the screen display
    gfx->begin();
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    // 2. LVGL Core and Tick Initialization
    // This is the foundation for LVGL to run and should be completed as early as possible.
    lv_init();

    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };

    esp_timer_handle_t lvgl_tick_timer = NULL;
    esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

    // 3. LVGL Display Driver Initialization
    // Connect LVGL with the hardware driver

    // --- Initialize Display Driver ---
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // 4. UI Creation and Style Application
    // This is the application layer, executed after all underlying layers are ready.

    // --- Style Definition Optimization ---
    // Defining all related style properties (font, color, etc.) within the same style object is more modular.
    lv_style_init(&style_large_white_font);
    lv_style_set_text_font(&style_large_white_font, &lv_font_montserrat_14);
    lv_style_set_text_color(&style_large_white_font, lv_color_hex(0xffffff));

    // Set screen background color
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

    connect_WiFi();
}


void loop()
{
    lv_timer_handler();

    if ((millis() - lastTime) > timerDelay)
    {
        lastTime = millis();
        // Allocate the JSON document


        JsonDocument doc;
        // Deserialize the JSON document
        // DeserializationError error = deserializeJson(doc, body.c_str());
        const char* body = make_call().c_str();
        DeserializationError error = deserializeJson(doc, body);
        // Test if parsing succeeds.
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            Serial.println(body);
            return;
        }

        // Fetch values.
        // Most of the time, you can rely on the implicit casts.
        // In other case, you can do doc["time"].as<long>();
        const char* timestamp = doc["timestamp"];
        float humidity = doc["humidity"];
        float temperature = doc["temperature"];
        float pressure = doc["pressure"];
        float co2 = doc["co2"];


        //https://www.geeksforgeeks.org/c/snprintf-c-library/
        char humidity_buf[32];
        snprintf(humidity_buf, sizeof(humidity_buf), "%.2f", humidity);

        char temperature_buf[32];
        snprintf(temperature_buf, sizeof(temperature_buf), "%.2f", temperature);

        char pressure_buf[32];
        snprintf(pressure_buf, sizeof(pressure_buf), "%.2f", pressure);

        char co2_buf[32];
        snprintf(co2_buf, sizeof(co2_buf), "%.2f", co2);

        create_layout(humidity_buf, temperature_buf, pressure_buf, co2_buf);
    }
}

void create_layout(const char* humidity, const char* temperature, const char* pressure, const char* co2)
{
    create_label("humidity:", -30, -100);
    create_label(humidity, 50, -100);
    create_label("temperature:", -30, -80);
    create_label(temperature, 50, -80);
    create_label("co2:", -30, -60);
    create_label(co2, 50, -60);
    create_label("pressure:", -30, -40);
    create_label(pressure, 50, -40);
}


//https://randomnerdtutorials.com/esp32-http-get-post-arduino/
String make_call()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        String serverPath = serverName;

        // Your Domain name with URL path or IP address with path
        http.begin(serverPath.c_str());

        // Send HTTP GET request
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            http.end();
            return payload;
        }
        else
        {
            Serial.println(httpResponseCode);
            Serial.print("Error code: ");
        }
        // Free resources
        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
        connect_WiFi();
    }
}

lv_obj_t* create_label(const char* text, int x, int y)
{
    // Create Label
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);

    // Apply the style containing all properties to the label at once
    lv_obj_add_style(label, &style_large_white_font, 0);
    return label;
}

void connect_WiFi()
{
    // Connect to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // Wait for the connection to succeed
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("not yet connected");
    }

    Serial.println();
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

/* LVGL  Display Refresh Callback */
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t*)&color_p->full, w, h);

    lv_disp_flush_ready(disp);
}

/* LVGL Tick Increment Callback */
void example_increase_lvgl_tick(void* arg)
{
    /* Inform LVGL of the elapsed milliseconds */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
