
unsigned int crc_table[256];
void		 crc32_init( void );
unsigned int crc32_calc(char * pData, int nLen);

// -------------------------------------------------------------

void crc32_init( void ){
      const unsigned int CRC_POLY = 0xEDB88320;
      unsigned int i, j, r;
      for (i = 0; i < 256; i++){
             for (r = i, j = 8; j; j--)
                   r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
             crc_table[i] = r;
      }
};
 
unsigned int crc32_calc( char * pData, int nLen){
      const unsigned int CRC_MASK = 0xD202EF8D;
	  unsigned int crc = 0;
      while (nLen--){
             crc = crc_table[(crc & 0xFF) ^ *pData++] ^ crc >> 8;
             crc ^= CRC_MASK;
      }
	  return( crc );
}

unsigned int calc4summ( unsigned int * ptr, int cnt ) {
	unsigned int res = 0;
	while( cnt > 0 ) {
		res = res + ptr[cnt-1];
		cnt = cnt - 1;
	};
	return( res );
}
