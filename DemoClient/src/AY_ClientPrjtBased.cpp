//#include "pch.h"
#undef UNICODE

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_Socket.h>
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>

CFG_FILES	CngFile;
Ui08 SIGNING_PUB_KEY[2048];
Ui08 SIGNING_PR_KEY[2048];
Ui32 CLIENT_CERT_NO;

const Ui08 DefaultInterface[45] = "Wi-Fi";// "Local Area Connection";
const Ui08 DefaultServerDns[] = "datakomip.dyndns.org";
const Ui16 DefaultServerPort = 1982;
const Ui08 DefaultDNS_Ip[4] = { 8,8,8,8 };
const Ui16 DefaultDNS_Port = 53;
const Ui16 DefaultServerID = 1;


const Ui16 MyTCP_StartPort = 1985;
const Ui08 MyMac[6] = { 0x60,0xF6,0x77,0xBE,0x5F,0x40 }; //{0x60,0xF6,0x77,0xBE,0x5F,0x30};
const Ui08 DefaultMac[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, };


const Ui08 * const Default_Config_File = (unsigned char *)
"< ? xml version = \"1.0\" encoding = \"UTF-8\" ? >\n"
"<!--\n"
"By modifying this file,\n"
"-->\n"
"<Gateway>\n"
"<Item id = \"1\" Name = \"Local Network Interface Name\" 		Value = \"Wi-Fi\" />	<!--exp: \"Local Area Network\", \"Wi-Fi\", \"Local Area Network 2\", \"\"(means Local Area Connection)-->\n"
"<Item id = \"2\" Name = \"Local Network Ip Address\" 			Value = \"\" />	<!--exp: \"192.168.2.101\", \"\"(means automatic)-->\n"
"<Item id = \"3\" Name = \"Local Network Subnet Mask\" 			Value = \"\" />	<!--exp: \"255.255.255.0\", \"\"(means automatic)-->\n"
"<Item id = \"4\" Name = \"Local Network Gateway Ip\" 			Value = \"\" />	<!--exp: \"192.168.2.1\", \"\"(means automatic)-->\n"
"<Item id = \"5\" Name = \"Local Network Subnet Ip\" 			Value = \"\" />	<!--exp: \"192.168.2.0\", \"\"(means automatic)-->\n"
"<Item id = \"6\" Name = \"Gateway Name\" 						Value = \"\" />	<!--exp: \"Gateway-1\", \"Office\", \"Home\", \"\"(means empty)-->\n"
"<Item id = \"7\" Name = \"Gateway Password\" 					Value = \"\" />	<!--exp: \"1234\", \"A1b4c32\", \"\"(means empty)-->\n"
"<Item id = \"8\" Name = \"Allow To Add/Remove Ghost Ips\" 		Value = \"YES\" />	<!--exp: \"YES\", \"NO\", \"\"(means YES)-->\n"
"<Item id = \"9\" Name = \"Unique ID\" 							Value = \"\" />	<!--exp: \"112233445566778899AABBCC\", \"\"(means generate new)-->\n"
"<Item id = \"10\" Name = \"Server Dns\" 						Value = \"datakomip.dyndns.org\" />	<!--exp: \"LocalAndSafe.com\" -->\n"
"<Item id = \"11\" Name = \"Server Port\" 						Value = \"1982\" />	<!--exp: \"1982\" -->\n"
"<Item id = \"12\" Name = \"DNS Ip\" 							Value = \"8.8.8.8\" />	<!--exp: \"8.8.4.4\" -->\n"
"<Item id = \"13\" Name = \"DNS Port\" 							Value = \"53\" />	<!--exp: \"53\" -->\n"
"<Item id = \"14\" Name = \"Server ID\" 							Value = \"1\" />	<!--exp: \"1\", \"121\", \"\"(means 1 LocalAndSafe.com default server)-->\n"
"<Item id = \"15\" Name = \"Server Public Key\" 					Value = \"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnJC0Zax2WvioDwC8IDfc\n"
"Bk3Iw9CaVU0t6QgVA3Mgn0v3x/m0uPk/CzkCR4l0IDbG4mpn2N9E1727Gpn4sZo5\n"
"lv9xy35K4kt+lUhd6Oj4JUj2HhnvUDeG81FdAQ/E1BBmLyWAYYNTVrG2tQ1+O7K9\n"
"7lyE3avTPnfAeGOfvQe/lQ07TCyealtdYFYYFW3n5JW1okS2AIrIO7yt8bJ1fU1h\n"
"3uIqO9IsVN11dO/Yz9PX4heYclBQzDnzzd/wXq7bQPXToZJE5MqQszAF+mliiCMG\n"
"fp8+9OlJQwpwNxNKxEAVsBnK9u+CvSq/PQlwqdan1DUb/jnTCXQCDEEoLM4QLKbP\n"
"HwIDAQAB\n"
"-----END PUBLIC KEY-----\n"
"\"/>	<!-- Server's public key for Server ID  Server ID=1 means LocalAndSafe.com default server -->\n"
"\n"
"</Gateway>\n";

const Ui08 * const Default_Cert_File = (unsigned char *)
"1\n"
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxD4ypIxIvaC0y+FAOZu5\n"
"eWRscRCiDnYbVJ6SCn/QTxlbYz/KwH460L1E+waexOvv42QTxe8dqSWlQT1ESkwt\n"
"yEdIkzzJ41vrwUVoMyclErxeLaSNR2HjQwqiZS8hPSIaGukpmatQGcPPSUO1W/Zl\n"
"wKVWNcWjZxBjGyIQyBbLHjOX+BNZLQTcVoTniUuZy+p89aZ5WXc1qO8le6DrmZFb\n"
"x1G3GES5Nuv9wNXKIso+JY9vz0TEd+EtGvKHTIqPrY/pjzzeWKsmwNpGPKokNC/T\n"
"Amme+A5q8mIwUoRfd0Y6fnJqw42UaoU7dhZLIMB20FgaQcq8Dj/JNgUkkHwXzzg8\n"
"LQIDAQAB\n"
"-----END PUBLIC KEY-----\n"
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpAIBAAKCAQEAxD4ypIxIvaC0y+FAOZu5eWRscRCiDnYbVJ6SCn/QTxlbYz/K\n"
"wH460L1E+waexOvv42QTxe8dqSWlQT1ESkwtyEdIkzzJ41vrwUVoMyclErxeLaSN\n"
"R2HjQwqiZS8hPSIaGukpmatQGcPPSUO1W/ZlwKVWNcWjZxBjGyIQyBbLHjOX+BNZ\n"
"LQTcVoTniUuZy+p89aZ5WXc1qO8le6DrmZFbx1G3GES5Nuv9wNXKIso+JY9vz0TE\n"
"d+EtGvKHTIqPrY/pjzzeWKsmwNpGPKokNC/TAmme+A5q8mIwUoRfd0Y6fnJqw42U\n"
"aoU7dhZLIMB20FgaQcq8Dj/JNgUkkHwXzzg8LQIDAQABAoIBAQC+xOv8M/deiARZ\n"
"kd4aYny6OlqEyzOOtVe6OfAkTr3UtwS1BcV2vP81/lg5YuUi720i8x2Zs+p0jQkJ\n"
"06I4S7fR0yIFJ/nmkGwtVOiHmY0bmow++JDyjIEdog/eJyO7xxPzhygOn4/djBFM\n"
"2opKHMwrq06UNU20miSNst73c9j+wFn7IV9eBIQSPrcO9wtuFHP0uHN3N/O5xfAM\n"
"Zihw42//St4m3yFtkwh30JW0y/WGL/QFNzaVdDsDBphp94QBVJnbPHxrqHF9t7te\n"
"PB4pS4lbZ6jAA26754vlsjIYQjrYFdI7KtBmbRjSBD2/B2HIBntTCWUDil682Z6a\n"
"In9rWy4BAoGBAPG/bqd5WoV8poIUMqvTnm+jToyEHSQedTF70X2Fivmyzt8y7Id+\n"
"Dx/4YvhkfQqpbMrLEaGZNw+q7q2AykC7I2Sixm/d9XxXPIkIt9Pv9O097QlfcnCX\n"
"dOrkdP6BQIXGPy84wWLsF5X5H0q1eyA3XczMNZtnJqjY0mDz2UkG0qSBAoGBAM/P\n"
"/G5bqWYMkndCd31s+d/OVm35UluRDTclsI7qU+yVPKiPy11cmfZDTpKJ2CnmluZ+\n"
"UnSWMA3bkdSgSzx2qrF0bnXvyVi4cRJWkOWi00zEoZmcmDzqnntUfMfxhkg6nlOF\n"
"9Jc7Qstrj7yvm+uzr6ZVVUC/NdLkEvpPPmGCspGtAoGAVNRc6u2dgxvjfd/CjCL9\n"
"BKWkiZBOfX8mlo+hSk+THd3N0+qVr1Hf0tMhP5JbgmNDZsHtVOxjst7DVCTOU/qX\n"
"B9hKk7SqUNB30Io7n2YjZyCEzfYtPS8Yethh6XDHiGmxgEuFKMY53ImHnnqiUwK6\n"
"qoO6B+chXLyAzj6jL4nyXoECgYEAztrWB7ff5/+tr16O+F+IXnnSWIerrg+1muY+\n"
"WBPyU/W9OoBfyuAJNTOMO5tQe2zXWjn1s61nyfN8tqyUF2x7vkYKSgTUZCa8Dqfy\n"
"dVJdh/XWccSJWWGlPVg9GP1WUAFhZfxaGktX0oqPuh8HUlg7CJO6V8e9ON3j8hr9\n"
"GizxuMUCgYA8Zq0nBvApHeCHg+KM6WqfhNHhriVinJooo2Lkbg4YtEwtmxsyO6BM\n"
"uI1u16sYmTHsEBTwgwac5C838XtIfRpteAISN2CM9MuzCtQeRSW9v71UiJV/bR6k\n"
"M8CKF+/FqW5HXuDPk/5pMQj72KVA2A0kehl3rBbXzLiM5cCg6S+lGA==\n"
"-----END RSA PRIVATE KEY-----\n";




//--------------------------------------------------------------------------------------//
const Ui08 DEMO_CLNT_MAC[3][6] = { { 0x60,0xF6,0x77,0xBE,0x5F,0x30 },{ 0x60,0xF6,0x77,0xBE,0x5F,0x31 },{ 0x60,0xF6,0x77,0xBE,0x5F,0x32 } };

const Ui08 * const SERVER_PUB_KEY = (Ui08 *)
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnJC0Zax2WvioDwC8IDfc\n"
"Bk3Iw9CaVU0t6QgVA3Mgn0v3x/m0uPk/CzkCR4l0IDbG4mpn2N9E1727Gpn4sZo5\n"
"lv9xy35K4kt+lUhd6Oj4JUj2HhnvUDeG81FdAQ/E1BBmLyWAYYNTVrG2tQ1+O7K9\n"
"7lyE3avTPnfAeGOfvQe/lQ07TCyealtdYFYYFW3n5JW1okS2AIrIO7yt8bJ1fU1h\n"
"3uIqO9IsVN11dO/Yz9PX4heYclBQzDnzzd/wXq7bQPXToZJE5MqQszAF+mliiCMG\n"
"fp8+9OlJQwpwNxNKxEAVsBnK9u+CvSq/PQlwqdan1DUb/jnTCXQCDEEoLM4QLKbP\n"
"HwIDAQAB\n"
"-----END PUBLIC KEY-----\n";


const Ui08 * const SERVER_PR_KEY = (unsigned char *)
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEogIBAAKCAQEAnJC0Zax2WvioDwC8IDfcBk3Iw9CaVU0t6QgVA3Mgn0v3x/m0\n"
"uPk/CzkCR4l0IDbG4mpn2N9E1727Gpn4sZo5lv9xy35K4kt+lUhd6Oj4JUj2Hhnv\n"
"UDeG81FdAQ/E1BBmLyWAYYNTVrG2tQ1+O7K97lyE3avTPnfAeGOfvQe/lQ07TCye\n"
"altdYFYYFW3n5JW1okS2AIrIO7yt8bJ1fU1h3uIqO9IsVN11dO/Yz9PX4heYclBQ\n"
"zDnzzd/wXq7bQPXToZJE5MqQszAF+mliiCMGfp8+9OlJQwpwNxNKxEAVsBnK9u+C\n"
"vSq/PQlwqdan1DUb/jnTCXQCDEEoLM4QLKbPHwIDAQABAoIBABTdfA0oxMUu6kbF\n"
"lEy+5QW6DA18mXTQylOKDjLdfvV7zFPMRXd5UBuQx2r9OIqqgiref2R5rNULpgnP\n"
"h0M0CITFGkdCV3x9DyuZ71oA4kElzhcI4cnQ/J0ktL5VeafBX7zcxJU6STr+iBjs\n"
"6WvPUFyZ2+Mm6QokL3Rb4ofwpmmYmndeQe4rBtwguZRdlVjTQ4+uG49Na1ODkbq7\n"
"BoAIoLu75XDRy8iCxt7vFEEvV+Qy9twB33UU5JzN0Rskknm15SiBv+1EiimDS0+0\n"
"+RtpQV2RyqA10vsQxpxHVqx5g87u89GYQmwKZvBO1rLNJVPPuemdZ15V07xrbd0m\n"
"HJdgmvECgYEAydmKJIsg9UvHdcJwDWl0IJSRhQhl9vFLKtKZYcAXNqKt3JaOUROt\n"
"rEhkvgTUEB8FUh1iLMAZEdOLISsLKzSU89K/AMiUUx3oh1Cuj28Ujv0MmziIjAiS\n"
"cSsKgUi1u+lK92f/Ncn49ammkLtbrrG8QSmjvZ+grOK6pEq6tudndnMCgYEAxpEn\n"
"vkVRly2Zx95cAgnbSKRd61gjGV4sL2a2FoWa+bB9qbgL2vPZOFQsR2Vrfruo9aPf\n"
"NUENqfHRjvuqUrb0wDL7lb13RLvAOxoKSp2lzgHcfzbxNMpGPHOPtK1r+fnYTRCM\n"
"aLpOt7vmc1EHEyJw8vFytMET7nM8s9GLh8W17aUCgYAWLoPR1EBA9JzswftSXf5W\n"
"/jyno+6gno+mGxKj/VoPtPzqOtlU65awUhxQDXPIecA57+S0CkA3B0pI0NQfYV3l\n"
"FYnL6j7yi1zcUe9877YQqJpUDHhk+yQsr5At+PGPvulwGsue+50SUhxt10kR/5Iy\n"
"jwLhecNMfHYKS/QM2Y0zJQKBgB78edo8UP4pdIz3Cx6aUSjl1W4rY9bKllSTT6j2\n"
"EngSbZm1icwSL3yVzg8aKYsuV7B1cRiLXUl+3ffVGAAuYsU3b4EvftQwBFB0wNRi\n"
"A4TBT5pehYd6ZTl7wxor0eTBc1qbUX3qTOxpGw41v8fhoQSYXxEL7ENSok9hTMfU\n"
"N46tAoGAZl9KucuLL6Yo5BpwdEuDrhKHw12oqNS0KNMMwmXRcWiTRQ7y6yXeZ/iw\n"
"HGuoula+y2CpNKK71W75LPPNP2tGj14y88f11j/+ZhpwJ1Z21tjA8ADSuf7eK5NC\n"
"//R7eVRKQy2FueJY7w/gzUWbu9lDu6CKcDhAbBTobaOq0lBu3eA=\n"
"-----END RSA PRIVATE KEY-----\n";

const char TestData[65] = "Merhaba Dunya. Test data test. Aykut Yakisir tarafindan yapilan ";
const Ui08 DemoAESkey[16] = { 0xDE,0x37,0x09,0x9B,0x9C,0x58,0xD0,0xF3,0xDD,0x4D,0x6E,0x8E,0xAD,0x47,0xC2,0x91 };
