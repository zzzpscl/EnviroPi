#include <WiFi.h>
#include <HTTPClient.h>
#include <lvgl.h>
#include "Arduino_GFX_Library.h"
#include "lv_conf.h"
#include <NetworkClient.h>

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2
#define LCD_DC     15
#define LCD_CS     14
#define LCD_SCK     7
#define LCD_MOSI    6
#define LCD_RST    21
#define LCD_BL     22
#define COL_OFFSET1  40
#define ROW_OFFSET1  0
#define COL_OFFSET2 0
#define ROW_OFFSET2  0
#define LCD_WIDTH  172
#define LCD_HEIGHT 320
const char *ssid = "KBMRaspberry";         // Replace with your Wi-Fi name
const char *password = "BonbonbuntesBananeneis4711.";  // Replace with your Wi-Fi password

static const uint16_t screenWidth = LCD_WIDTH;
static const uint16_t screenHeight = LCD_HEIGHT;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];
static lv_style_t style_large_white_font;

/* LCD - Display Hardware Interface Definitions */
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);
/*maybe wrong order on the offsets the IDE IS SO BAAADDD*/
Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */, 0 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, COL_OFFSET1, ROW_OFFSET1, COL_OFFSET2, ROW_OFFSET2);

/* LVGL  Display Refresh Callback */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

  lv_disp_flush_ready(disp);
}

/* LVGL Tick Increment Callback */
void example_increase_lvgl_tick(void *arg) {
  /* Inform LVGL of the elapsed milliseconds */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void setup() {
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
//https://randomnerdtutorials.com/esp32-http-get-post-arduino/
void loop() {
  lv_timer_handler();
  create_label("Hello World!");
  
  delay(500);
}

void create_label(const char *text){
  // Create Label
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, text);
  lv_obj_align(label, LV_ALIGN_CENTER, 20, 20);

  // Apply the style containing all properties to the label at once
  lv_obj_add_style(label, &style_large_white_font, 0);
}

void connect_WiFi(){
  // Connect to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Wait for the connection to succeed
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("not yet connected");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}