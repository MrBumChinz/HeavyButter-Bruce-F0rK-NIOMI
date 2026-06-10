/**
 * @file soulcage.cpp
 * @brief Soul Cage connector — uploads wardriving CSV to soulcage.win
 * @version 0.1
 *
 * Endpoint: POST https://soulcage.win/api/ingest/wigle-csv
 * Auth:     X-API-Key header (64-char hex, kind=EXTERNAL, no HMAC signing)
 * Response: 202 Accepted  {"job_id":"...","state":"queued","networks_in":N,...}
 *
 * Mint your key at soulcage.win — choose "External wardriver" in the key
 * type dropdown and paste the 64-char key into scApiKey in bruce.conf.
 */

#include "soulcage.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/wifi/wifi_common.h"

#define CBUFLEN 1024

SoulCage::SoulCage() {}

SoulCage::~SoulCage() {}

bool SoulCage::_check_api_key() {
    if (bruceConfig.scApiKey.length() != 64) {
        displayError("Set API key in bruce.conf\nGet it at soulcage.win", true);
        return false;
    }

    if (!wifiConnected) wifiConnectMenu();

    return wifiConnected;
}

void SoulCage::_display_banner() {
    drawMainBorderWithTitle("SC Upload");
    padprintln("\n");
}

void SoulCage::_send_upload_headers(WiFiClientSecure &client, String filename, int filesize, String boundary) {
    int cd_header_len = 147 - 45 - 8 + filename.length() + 2 * (boundary.length() + 2);

    client.print("POST ");
    client.print(upload_path);
    client.println(" HTTP/1.0");
    client.print("Host: ");
    client.println(host);
    client.println("Connection: close");
    client.println("User-Agent: bruce.wardriving");
    client.print("X-API-Key: ");
    client.println(bruceConfig.scApiKey);
    client.print("Content-Type: multipart/form-data; boundary=");
    client.println(boundary);
    client.print("Content-Length: ");
    client.println(filesize + cd_header_len);
    client.println();

    client.println("--" + boundary);
    client.print("Content-Disposition: form-data; name=\"file\"; filename=\"");
    client.print(filename);
    client.println("\"");
    client.println("Content-Type: text/csv");
    client.println();
}

bool SoulCage::upload(FS *fs, String filepath, bool auto_delete) {
    _display_banner();

    if (!fs || !_check_api_key()) return false;

    padprintln("Uploading to Soul Cage...");

    File file = fs->open(filepath);
    if (!file) {
        displayError("Failed to open file", true);
        return false;
    }

    if (!_upload_file(file, "Uploading...")) {
        file.close();
        displayError("File upload error", true);
        return false;
    }

    file.close();
    if (auto_delete) fs->remove(filepath);

    displaySuccess("File upload queued", true);
    return true;
}

bool SoulCage::upload_all(FS *fs, String folder, bool auto_delete) {
    _display_banner();

    if (!fs || !_check_api_key()) return false;

    File root = fs->open(folder);
    if (!root || !root.isDirectory()) return false;

    padprintln("Uploading all to Soul Cage...");
    int i = 1;
    bool success;

    while (true) {
        bool isDir;
        success = false;

        String fullPath = root.getNextFileName(&isDir);
        String nameOnly = fullPath.substring(fullPath.lastIndexOf("/") + 1);
        if (fullPath == "") break;

        if (!isDir) {
            int dotIndex = nameOnly.lastIndexOf(".");
            String ext = dotIndex >= 0 ? nameOnly.substring(dotIndex + 1) : "";
            ext.toUpperCase();
            if (ext.equals("CSV")) {
                File file = fs->open(fullPath);
                if (file) {
                    if (!_upload_file(file, "Uploading " + String(i) + "...")) {
                        file.close();
                        displayError("File upload error", true);
                        return false;
                    }
                    i++;
                    success = true;
                    file.close();
                    if (success && auto_delete) fs->remove(fullPath);
                }
            }
        }
    }
    root.close();

    String plural = i > 2 ? "s" : "";
    displaySuccess(String(i - 1) + " file" + plural + " queued", true);
    return true;
}

bool SoulCage::_upload_file(File file, String upload_message) {
    WiFiClientSecure client;
    client.setInsecure();
    if (!client.connect(host, 443)) {
        displayError("Soul Cage connection failed", true);
        return false;
    }

    String filename = file.name();
    int filesize = file.size();
    String boundary = "BRUCE";
    boundary.concat(esp_random());

    _send_upload_headers(client, filename, filesize, boundary);

    byte cbuf[CBUFLEN];
    float percent = 0;
    while (file.available()) {
        long bytes_available = file.available();
        int toread = CBUFLEN;
        if (bytes_available < CBUFLEN) toread = bytes_available;
        file.read(cbuf, toread);
        client.write(cbuf, toread);
        percent = ((float)file.position() / (float)file.size()) * 100;
        progressHandler(percent, 100, upload_message);
    }

    client.println();
    client.print("--");
    client.print(boundary);
    client.println("--");
    client.println();
    client.flush();

    // Read response — Soul Cage responds 202 Accepted with {"job_id":"...","state":"queued",...}
    String serverres = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            serverres.concat(c);
        }
        if (serverres.length() > 1024) break;
    }

    client.stop();

    // Accept 202 queued response or direct success
    return (serverres.indexOf("\"job_id\"") > -1 || serverres.indexOf("\"state\":\"queued\"") > -1);
}
