

all:
	@echo "没事做"

clean:
	@echo "清理垃圾"
	cd /home/angelllls/DATE/工程/s.嵌入式/开发板.nanoPC.T3/other/source/android
	make clean
	#
	cd /home/angelllls/DATE/工程/s.嵌入式/开发板.nanoPC.T3/other/source/uboot
	make clean
	cd /home/angelllls/DATE/工程/s.嵌入式/开发板.nanoPC.T3/other/source/linux-3.4.y
	make clean

install:
	@echo "为了方便"
