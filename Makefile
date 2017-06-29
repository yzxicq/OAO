
#包含了变量好函数
include makefile.common


all:
	@echo "没事做"

#git同步到本地
down:
	git pull
	# git submodule add git@github.com:angelllls/d.aider.git PRO/d.aider
	# git fetch PRO/d.aider
	# git subtree add -P PRO/d.aider 'PRO/d.aider/master'

#上传代码到git
update:
	# eval "$(ssh-agent -s)"
	# ssh-add
	git add .
	git commit -m "${your.name}"
	git push
	cd ${ROOT}

