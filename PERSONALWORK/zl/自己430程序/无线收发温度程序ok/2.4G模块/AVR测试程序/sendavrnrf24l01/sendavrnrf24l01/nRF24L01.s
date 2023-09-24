	.module nRF24L01.c
	.area data(ram, con, rel)
_TX_ADDRESS::
	.blkb 2
	.area idata
	.byte 52,'C
	.area data(ram, con, rel)
	.blkb 2
	.area idata
	.byte 16,16
	.area data(ram, con, rel)
	.blkb 1
	.area idata
	.byte 1
	.area data(ram, con, rel)
	.dbfile E:\avrnrf24l01\nRF24L01.c
	.dbsym e TX_ADDRESS _TX_ADDRESS A[5:5]c
	.area text(rom, con, rel)
	.dbfile E:\avrnrf24l01\nRF24L01.c
	.dbfunc e nRF24L01_Initial _nRF24L01_Initial fV
	.even
_nRF24L01_Initial::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e nRF24L01ioConfig _nRF24L01ioConfig fV
	.even
_nRF24L01ioConfig::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e SPI_RW _SPI_RW fc
;           byte -> R0
	.even
_SPI_RW::
	.dbline 0 ; func end
	ret
	.dbsym l byte 0 c
	.dbend
	.dbfunc e SPI_RW_Reg _SPI_RW_Reg fc
;          value -> R1
;            reg -> R0
	.even
_SPI_RW_Reg::
	.dbline 0 ; func end
	ret
	.dbsym l value 2 c
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Read _SPI_Read fc
;            reg -> R0
	.even
_SPI_Read::
	.dbline 0 ; func end
	ret
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Read_Buf _SPI_Read_Buf fc
;          bytes -> R2
;           pBuf -> R1,R2
;            reg -> R0
	.even
_SPI_Read_Buf::
	.dbline 0 ; func end
	ret
	.dbsym l bytes 4 c
	.dbsym l pBuf 2 pc
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Write_Buf _SPI_Write_Buf fc
;          bytes -> R2
;           pBuf -> R1,R2
;            reg -> R0
	.even
_SPI_Write_Buf::
	.dbline 0 ; func end
	ret
	.dbsym l bytes 4 c
	.dbsym l pBuf 2 pc
	.dbsym l reg 0 c
	.dbend
	.dbfunc e RX_Mode _RX_Mode fV
	.even
_RX_Mode::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e TX_Mode _TX_Mode fV
	.even
_TX_Mode::
	.dbline 0 ; func end
	ret
	.dbend
