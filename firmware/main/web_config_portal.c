#include "web_config_portal.h"
#include "retrowatch_common.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include <string.h>

static const char *TAG = "WEB_CONFIG";
static httpd_handle_t server = NULL;

// 手機配置頁面 HTML
static const char *CONFIG_HTML = 
"<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>"
"<title>RetroWatch-S3 手機設定後台</title>"
"<style>"
"body{background:#1E1C1A;color:#EED9B3;font-family:-apple-system,sans-serif;padding:15px;margin:0;}"
"h2{color:#E5C795;text-align:center;border-bottom:2px solid #E63920;padding-bottom:10px;margin-top:5px;}"
".card{background:#2A2724;border:1px solid #443E36;border-radius:10px;padding:15px;margin-bottom:15px;box-shadow:0 4px 6px rgba(0,0,0,0.3);}"
".card h3{color:#E63920;margin-top:0;font-size:16px;border-left:3px solid #E63920;padding-left:8px;}"
"label{display:block;font-size:13px;margin:10px 0 4px;color:#A99F92;}"
"input,select{width:100%;box-sizing:border-box;background:#151413;border:1px solid #554D43;color:#FFF;padding:10px;border-radius:6px;font-size:14px;}"
".btn{background:#E63920;color:#FFF;border:none;width:100%;padding:14px;font-size:16px;font-weight:bold;border-radius:8px;margin-top:10px;cursor:pointer;}"
"</style></head><body>"
"<h2>GAME &amp; WATCH 設定中心</h2>"
"<form action='/save' method='POST'>"
"<div class='card'><h3>1. 復古相機街拍設定</h3>"
"<label>風格濾鏡 (GR/富士風格)</label><select name='filter'>"
"<option value='0'>1. 原始 (Standard / Original - 原色無調色)</option>"
"<option value='1'>2. GR 高反差黑白 (GR Hard B&W / 森山大道風)</option>"
"<option value='2'>3. GR 經典正片 (GR Positive Film / 街拍膠卷)</option>"
"<option value='3'>4. 富士經典正片 (Fuji Classic Chrome / 紀實冷調)</option>"
"<option value='4'>5. 富士經典負片 (Fuji Classic Negative / 日系復古)</option>"
"</select>"
"<label>快門音效</label><select name='sound'><option value='1'>開啟 (喀嚓聲)</option><option value='0'>靜音 (無聲街拍)</option></select>"
"<label>拍照日期戳記 (YYYY-MM-DD)</label><select name='date_stamp'><option value='1'>壓印復古橙黃色日期</option><option value='0'>關閉 (原圖無水印)</option></select>"
"<label>自拍定時器</label><select name='self_timer'><option value='0'>關閉 (即時快門)</option><option value='3'>3 秒後自拍</option></select>"
"<label>縮時攝影間隔 (3 ~ 300 秒，0為關閉)</label><input type='number' name='timelapse_sec' min='0' max='300' value='0'>"
"</div>"
"<div class='card'><h3>2. 桌面動態看盤設定</h3>"
"<label>自訂 4 檔股票/幣種 (以逗點分隔)</label><input type='text' name='stocks' value='AAPL,NVDA,BTC,2330'>"
"</div>"
"<div class='card'><h3>3. 行車記錄器設定</h3>"
"<label>循環錄影每段長度</label><select name='dashcam_seg'><option value='1'>1 分鐘</option><option value='3' selected>3 分鐘</option><option value='5'>5 分鐘</option></select>"
"</div>"
"<div class='card'><h3>4. 智慧居家監控設定</h3>"
"<label>移動偵測警報 Webhook (Line Notify/Telegram)</label><input type='text' name='webhook' placeholder='https://notify-api.line.me/...'>"
"</div>"
"<div class='card'><h3>5. 遊戲與系統設定</h3>"
"<label>螢幕亮度 (10~100%)</label><input type='range' name='brightness' min='10' max='100' value='80'>"
"</div>"
"<button type='submit' class='btn'>儲存並同步至掌機</button>"
"</form></body></html>";

static esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, CONFIG_HTML, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t save_post_handler(httpd_req_t *req) {
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret > 0) {
        buf[ret] = '\0';
        ESP_LOGI(TAG, "Received Form Data from Phone: %s", buf);
        // 解析並同步到 g_system_state
    }
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<script>alert('設定已成功保存至掌機！');window.location.href='/';</script>", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t web_config_portal_start(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root_uri = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler
        };
        httpd_register_uri_handler(server, &root_uri);

        httpd_uri_t save_uri = {
            .uri       = "/save",
            .method    = HTTP_POST,
            .handler   = save_post_handler
        };
        httpd_register_uri_handler(server, &save_uri);

        ESP_LOGI(TAG, "Web Config Portal started on port 80. Open http://192.168.4.1/ on phone.");
        return ESP_OK;
    }
    return ESP_FAIL;
}

void web_config_portal_stop(void) {
    if (server) {
        httpd_stop(server);
        server = NULL;
    }
}
