#include <Arduino.h>
#include <RadioLib.h>

// Module(uint8_t cs, uint8_t irq, uint8_t rst, uint8_t gpio = (uint8_t)255U)
//                 Module(cs     , irq,      rst,      gpio)
//                        18       26        23        33 
SX1276 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_D1);
PagerClient pager(&radio);					// Create Pager client using FSK module

uint32_t address = 0xC383C;
uint8_t command[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x46, 0x46, 0x30, 0x30, 0x02, 0x41, 0x30, 0x43, 0x35, 0x30, 0x37, 0x46, 0x30, 0x30, 0x30, 0x30, 0x32, 0x33, 0x35, 0x39, 0xff, 0xff, 0xff, 0x33, 0xfe, 0x46, 0xfd, 0x42, 0xfa, 0x44, 0x03, 0x30, 0x61, 0x31, 0x66, 0x04};

void setup()
{
	Serial.begin(9600);
	Serial.println();
	Serial.println();	
  	// Initialize SX1276
  	Serial.print(F("[SX1276] Initializing ... "));
  	// carrier frequency:           462.45 MHz
  	// bit rate:                    1.2 kbps
  	// frequency deviation:         4.5 kHz
  	// Rx bandwidth:                12.5 kHz
  	// output power:                17 dBm
  	// preambleLength				0 bits done in POCSAG encoding
	// enableOOK					false = use FSK instead of OOK
//*
  	int state = radio.beginFSK(462.45, 1.2, 4.5, 12.5, 17, 0, false);
/*/
	int state = radio.beginFSK();
//*/
  	if(state == RADIOLIB_ERR_NONE)
  	{
	    Serial.println(F("success!"));
  	}
	else
	{
	    Serial.print(F("failed, code "));
    	Serial.println(state);
  	}

	// Initalize Pager client
  	Serial.print(F("[Pager] Initializing ... "));
  	//state = pager.begin(462.45, 512);		// 462.45 MHz, 512 bps
  	state = pager.begin(462.45, 1200);		// 462.45 MHz, 1200 bps
  	if(state == RADIOLIB_ERR_NONE)
  	{
	    Serial.println(F("success!"));
  	}
	else
	{
    	Serial.print(F("failed, code "));
    	Serial.println(state);
	}
}


void loop() 
{
		char msgChar[sizeof(command)];
		String msgString = "";
		for(int i=0; i<(sizeof(command)); i++)
		{
			msgChar[i] = char(command[i]);
			msgString = msgString + msgChar[i];
		}
		Serial.println();

		uint32_t address = 0xC383C;
		Serial.print("Address: ");
		Serial.println(address);
		Serial.print("Message: ");
		Serial.println(msgString);
		Serial.println();

		Serial.print(F("[Pager] Test#1: Send Tone ... "));
		int state = pager.sendTone(address);
		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

		delay(1000);

		Serial.print(F("[Pager] Test#2: uint8 data ... "));
		//PagerClient::transmit(uint8_t* data, size_t len, uint32_t addr, uint8_t encoding)
		state |= pager.transmit(command, sizeof(command), address, RADIOLIB_PAGER_ASCII);
		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

		delay(1000);

		Serial.print(F("[Pager] Test#3a: const char* str ... "));
		const char* str1 = "This is a test";
		//PagerClient::transmit(const char* str, uint32_t addr, uint8_t encoding)
		state |= pager.transmit(str1, address, RADIOLIB_PAGER_ASCII);
		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

		delay(1000);

		Serial.print(F("[Pager] Test#3b: const char* str ... "));
		//PagerClient::transmit(const char* str, uint32_t addr, uint8_t encoding)
		state |= pager.transmit(msgChar, address, RADIOLIB_PAGER_ASCII);
  		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

		delay(1000);

		Serial.print(F("[Pager] Test#4a: String str ... "));
		String str2 = "This is a test";
		//PagerClient::transmit(String& str, uint32_t addr, uint8_t encoding) 
		state |= pager.transmit(str2, address, RADIOLIB_PAGER_ASCII);
  		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

		delay(1000);

		Serial.print(F("[Pager] Test#4b: String str ... "));
		state |= pager.transmit(msgString, address, RADIOLIB_PAGER_ASCII);
  		if(state == RADIOLIB_ERR_NONE) 
		{
    		Serial.println(F("success!"));
		} 
		else 
		{
		    Serial.print(F("failed, code "));
	    	Serial.println(state);
		}
		Serial.println();

}
