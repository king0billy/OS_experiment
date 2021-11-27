
https://my.oschina.net/u/2007323/blog/1860305/
**cmd命令!**

1. git init
2. 
git remote add exercise1 ../第一题
git remote add exercise2 ../第二题
git remote add exercise3 ../第三题
git remote add exercise4 ../第四题

3. !!!!本地先commit 一个master分支!!!eg:git add xxx,git commit -m "提交实验报告.创建trunk以合并四个实验仓库分支""

4. 
git fetch exercise1
git fetch exercise2
git fetch exercise3
git fetch exercise4

5. 
git checkout -b exercise1-branch exercise1/master
git checkout -b exercise2-branch exercise2/master
git checkout -b exercise3-branch exercise3/master
git checkout -b exercise4-branch exercise4/master

6. 
git checkout master

7. 
git merge exercise1-branch --allow-unrelated-histories
git merge exercise2-branch --allow-unrelated-histories
git merge exercise3-branch --allow-unrelated-histories
git merge exercise4-branch --allow-unrelated-histories


### 4.a 危险操作:
The following untracked working tree files would be overwritten by checkout:
        .idea/.gitignore
        .idea/modules.xml
        .idea/vcs.xml
Please move or remove them before you switch branches.
1、

git rm –cached
2、

git clean -d -fx “”

-x means ignored files are also removed as well as files unknown to git.


-d means remove untracked directories in addition to untracked files.


-f is required to force it to run.



### 5.a Switched to branch 'master'
PS E:\UniversityCollege\大二下考前\操作系统\操作系统实验> git merge exercise1-branch --allow-unrelated-histories
Merge made by the 'recursive' strategy.
 ...26\207\346\234\254\346\226\207\346\241\243.cpp" | 192 +++++++++++++++++++++
 ...26\207\346\234\254\346\226\207\346\241\243.exe" | Bin 0 -> 165179 bytes
 "\350\267\257\345\276\204.txt"                     |   1 +
 3 files changed, 193 insertions(+)
 create mode 100644 "\346\226\260\345\273\272\346\226\207\346\234\254\346\226\207\346\241\243.cpp"
 create mode 100644 "\346\226\260\345\273\272\346\226\207\346\234\254\346\226\207\346\241\243.exe"
 create mode 100644 "\350\267\257\345\276\204.txt"
PS E:\UniversityCollege\大二下考前\操作系统\操作系统实验> git merge exercise2-branch --allow-unrelated-histories
Merge made by the 'recursive' strategy.
 "\345\234\260\345\235\200.txt"                     |   1 +
 ...\226\207\346\234\254\346\226\207\346\241\243.c" | 235 +++++++++++++++++++++
 2 files changed, 236 insertions(+)
 create mode 100644 "\345\234\260\345\235\200.txt"
 create mode 100644 "\346\226\260\345\273\272\346\226\207\346\234\254\346\226\207\346\241\243.c"
PS E:\UniversityCollege\大二下考前\操作系统\操作系统实验> git merge exercise3-branch --allow-unrelated-histories
CONFLICT (add/add): Merge conflict in 路径.txt
Auto-merging 路径.txt
CONFLICT (add/add): Merge conflict in 新建文本文档.c
Auto-merging 新建文本文档.c
Automatic merge failed; fix conflicts and then commit the result.
PS E:\UniversityCollege\大二下考前\操作系统\操作系统实验> git merge exercise4-branch --allow-unrelated-histories
error: Merging is not possible because you have unmerged files.
hint: Fix them up in the work tree, and then use 'git add/rm <file>'
hint: as appropriate to mark resolution and make a commit.
fatal: Exiting because of an unresolved conflict.

## 7.a merge 路径.txt!!