#include <ESP8266FtpServer.h>

extern FtpServer ftpSrv; 

void FTP_Init(void) {
  ftpSrv.begin("SeaIA","opossum");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
}


