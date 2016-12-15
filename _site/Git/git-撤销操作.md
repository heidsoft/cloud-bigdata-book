# 恢复撤销篇

1、取消本地修改，恢复到上一次的commit
如果你的本地文件修改得一团乱，但是还没有提交，可以通过下面的命令恢复到上次提交时的状态( 其实就是恢复到上次的 commit，因此这次的本地修改自然没了）（注意是修改，不包括添加文件）

git reset --hard
这条命令将会取消掉本地的所有修改，恢复到上一次我们的commit信息
2、取消 git add 的文件
有时我们会不小心git add . 所有文件，想反悔，取消某些add的文件。那么可以(还原暂存区)

git reset HEAD xxxx
3、取消对文件的修改
如果觉得刚才对XX文件的修改没有必要，想要恢复，那么:

git checkout -- xxx
4、取消已经提交的修改
如果已经做了一个提交(commit),并且马上后悔了。那么可以创建一个新的提交，在新提交里撤销老的提交所做的修改
创建一个新的，撤销(revert)了前期修改的提交(commit)是很容易的。

git revert HEAD
这样就创建了一个撤销了上次提交(HEAD)的新提交

gir revert HEAD^
撤销上上次
5、Tips
回滚到某一次 commit
git reset --hard aa89cd6bcc
这样会回滚到某个commit状态，只要知道commit id 就可以
回滚后反悔怎么办？
git reflog 记录了我们的每一次命令（ commit、merge 等信息）
根据这命令来查出我们的历史 commit id，然后 git reset即可
HEAD指向的版本就是当前版本，因此，Git允许我们在版本的历史之间穿梭，使用命令git reset –hard commit_id。
穿梭前，用git log可以查看提交历史，以便确定要回退到哪个版本。
要重返未来，用git reflog查看命令历史，以便确定要回到未来的哪个版本。
场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令git checkout – file。
场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令git reset HEAD file，就回到了场景1，第二步按场景1操作。
6. 取消已经 push 的行为（重建分支）
# 备份到另一分支，以防万一
git branch old_master

# 推送备份分支
git push origin old_master:old_master

# 本地仓库回退到某一版本
git reset -hard xxxx

# 删除远程分支
git push origin :master

# 推送本地还原后的分支
git push origin master
6. 取消已经 push 的行为（强制 PUSH）
# 本地仓库回退到某一版本
git reset -hard xxxx

# 强制 PUSH，此时远程分支已经恢复成指定的 commit 了
git push origin master --force
