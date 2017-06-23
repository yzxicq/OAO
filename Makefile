
include makefile.common


all:
	@echo "没事做"

#git同步到本地
down:
	git pull
	#git submodule add git@github.com:angelllls/d.aider.git PRO/d.aider

update:
	git add .
	git commit -m "$(your.name)"
	git push
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
