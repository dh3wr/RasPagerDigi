package rasPager;

import java.util.ArrayList;

public class Pocsag {
	// char table
	public static char [] isotab =
		{
			 0x00, 0x40, 0x20, 0x60, 0x10, 0x50, 0x30, 0x70,
			 0x08, 0x48, 0x28, 0x68, 0x18, 0x58, 0x38, 0x78,
			 0x04, 0x44, 0x24, 0x64, 0x14, 0x54, 0x34, 0x74,
			 0x0c, 0x4c, 0x2c, 0x6c, 0x1c, 0x5c, 0x3c, 0x7c,
			 0x02, 0x42, 0x22, 0x62, 0x12, 0x52, 0x32, 0x72,
			 0x0a, 0x4a, 0x2a, 0x6a, 0x1a, 0x5a, 0x3a, 0x7a,
			 0x06, 0x46, 0x26, 0x66, 0x16, 0x56, 0x36, 0x76,
			 0x0e, 0x4e, 0x2e, 0x6e, 0x1e, 0x5e, 0x3e, 0x7e,
			 0x01, 0x41, 0x21, 0x61, 0x11, 0x51, 0x31, 0x71,
			 0x09, 0x49, 0x29, 0x69, 0x19, 0x59, 0x39, 0x79,
			 0x05, 0x45, 0x25, 0x65, 0x15, 0x55, 0x35, 0x75,
			 0x0d, 0x4d, 0x2d, 0x6d, 0x1d, 0x5d, 0x3d, 0x7d,
			 0x03, 0x43, 0x23, 0x63, 0x13, 0x53, 0x33, 0x73,
			 0x0b, 0x4b, 0x2b, 0x6b, 0x1b, 0x5b, 0x3b, 0x7b,
			 0x07, 0x47, 0x27, 0x67, 0x17, 0x57, 0x37, 0x77,
			 0x0f, 0x4f, 0x2f, 0x6f, 0x1f, 0x5f, 0x3f, 0x7f,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x01,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x6d, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x1d, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x5d, 0x3a, 0x3a, 0x3f,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x6f, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x1f, 0x3a,
			 0x3a, 0x3a, 0x3a, 0x3a, 0x5f, 0x3a, 0x3a, 0x3a
		};
	
	// settings
	public static final int POC_BITS_PER_CW	= 20;
	public static final int POC_BITS_PER_CHAR = 7;
	public static final int POC_BITS_PER_DIGIT = 4;
	
	// special code words
	public static final int PRAEEMBEL = 0xaaaaaaaa; // send 18 times
	public static final int SYNC = 0x7CD215D8; // sync-codeword
	public static final int IDLE = 0x7A89C197; // idle-codeword
	
	
	public static int crc(int cw) {
		int crc;
		int d;
		int m;
		char p;
		
		// crc
		crc = cw;
		d = 0xed200000;

		
		for (m = 0x80000000; (m & 0x400) == 0; m >>>= 1) {
			// m ist Bitmaske mit nur einer 1, die vom MSB bis vor den Anfang des (CRC+Praität) bereichs läuft, d.h. bis Bit 11 einschl.
			if ((crc & m) != 0)
				crc ^= d;

			d >>>= 1;
		}

		cw |= crc;

		// parity
		
		p = (char)(((cw >>> 24) & 0xff) ^
			((cw >>> 16) & 0xff) ^
		    ((cw >>>  8) & 0xff) ^
		            (cw & 0xff));

		p ^= (p >>> 4);
		p ^= (p >>> 2);
		p ^= (p >>> 1);
		p &= 0x01;
		
		
		return cw | p;
	}
	
	public static int encodeACW(int addr, int func) {
		
		return (((addr & 0x001ffff8) << 10) | ((func & 0x00000003) << 11));
		
	}
	
	public static int encodeMCW(int msg) {
		
		return (0x80000000 | ((msg & 0x000fffff) << 11));
				
	}
	
	public static char encodeChar(char ch) {
		return isotab[ch & 0xff];
	}
	
	public static char encodeDigit(char ch) {
		char [] mirrorTab = {
			0x00, 0x08, 0x04, 0x0c, 0x02, 
			0x0a, 0x06, 0x0e, 0x01, 0x09
		};

		if (ch >= '0' && ch <= '9')
			return mirrorTab[ch - '0'];

		switch (ch) {
		case ' ':
			return 0x03;

		case 'U':
			return 0x0d;

		case '_':
			return 0x0b;

		case '[':
			return 0x0f;

		case ']':
			return 0x07;
		}

		return 0x05;
	}
	
	public static ArrayList<Integer> encodeNum (int addr, int func, String text) {
//		int cwCnt;
		ArrayList<Integer> cwBuf = new ArrayList<Integer>();
		int msg = 0;
		int msgBitsLeft;
		
		// Anzahl der benoetigten Codewoerter berechnen
//		cwCnt = 1 + (POC_BITS_PER_DIGIT * text.length() + POC_BITS_PER_CW - 1) / POC_BITS_PER_CW;
		
		int framePos = addr & 7;
		cwBuf.add(framePos); // position 0
		
		// Adress-Codewort erzeugen und im Puffer speichern.
		cwBuf.add(crc(encodeACW(addr, func)));
		
		// Komplette Nachricht codieren und speichern.
		msgBitsLeft = POC_BITS_PER_CW;
		
		for (int i = 0; i < text.length(); i++) {
			char ch = encodeDigit(text.charAt(i));
			
			msg <<= POC_BITS_PER_DIGIT;
			msg |= ch;
			msgBitsLeft -= POC_BITS_PER_DIGIT;
			
			if (msgBitsLeft == 0) {
				cwBuf.add(crc(encodeMCW(msg)));
				msgBitsLeft = POC_BITS_PER_CW;
			}
		}
		
		// Wenn das letzte Codewort nicht komplett ist, wird es mit Spaces aufgefuellt.
		if(msgBitsLeft != POC_BITS_PER_CW) {
			while(msgBitsLeft > 0) {
				msg <<= POC_BITS_PER_DIGIT;
				msg |= 0x03; /* Space */
				msgBitsLeft -= POC_BITS_PER_DIGIT;
			}
			
			cwBuf.add(crc(encodeMCW(msg)));
		}
		
		return cwBuf;
	}
	
	public static ArrayList<Integer> encodeStr(int addr, int func, String text) {
//		int cwCnt;
		ArrayList<Integer> cwBuf = new ArrayList<Integer>();
		int msg = 0;
		int msgBitsLeft;
		
		// Anzahl der benoetigten Codewoerter berechnen. und ein gross genuges Page-Objekt erzeugen.
//		cwCnt = 1 + (POC_BITS_PER_CHAR * text.length() + POC_BITS_PER_CW - 1) / POC_BITS_PER_CW;
		
		int framePos = addr & 7;
		cwBuf.add(framePos); // position 0
		
		// Adress-Codewort erzeugen und im Puffer speichern.
		cwBuf.add(crc(encodeACW(addr, func)));

		// Komplette Nachricht codieren und speichern.
		msgBitsLeft = POC_BITS_PER_CW;
		 
		for(int i = 0; i < text.length(); i++) {
			char ch = encodeChar(text.charAt(i));
		
			if (msgBitsLeft >= POC_BITS_PER_CHAR) 
			{
				msg <<= POC_BITS_PER_CHAR;
				msg |= ch;
				msgBitsLeft -= POC_BITS_PER_CHAR;

				if (msgBitsLeft == 0) {
					cwBuf.add(crc(encodeMCW(msg)));
					msgBitsLeft = POC_BITS_PER_CW;
				}
			} 
			else 
			{
				msg <<= msgBitsLeft;
				msg |= ch >> (POC_BITS_PER_CHAR - msgBitsLeft);
				
				cwBuf.add(crc(encodeMCW(msg)));

				msg = ch;
				msgBitsLeft = POC_BITS_PER_CW - POC_BITS_PER_CHAR + msgBitsLeft;
			} 
		}

		if (msgBitsLeft != POC_BITS_PER_CW) {
			msg <<= msgBitsLeft;
			cwBuf.add(crc(encodeMCW(msg)));
		}

		return cwBuf;
	}
		
}
