/**
 * @file soulcage.h
 * @brief Soul Cage connector — uploads wardriving CSV to soulcage.win
 * @version 0.1
 */

#ifndef __SOULCAGE_H__
#define __SOULCAGE_H__

#include <WiFiClientSecure.h>
#include <globals.h>

class SoulCage {
public:
    SoulCage();
    ~SoulCage();

    bool upload(FS *fs, String filepath, bool auto_delete = true);
    bool upload_all(FS *fs, String folder, bool auto_delete = true);

private:
    const char *host = "soulcage.win";
    const char *upload_path = "/api/ingest/wigle-csv";

    bool _check_api_key(void);
    bool _upload_file(File file, String upload_message);
    void _send_upload_headers(WiFiClientSecure &client, String filename, int filesize, String boundary);
    void _display_banner(void);
};

#endif
