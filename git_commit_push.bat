@echo off
echo Staging untracked, modified, and deleted files
CALL git_commit.bat
echo Checking status
git status
echo Pushing committed files with message: "%msg%"
CALL git_push.bat
echo Process finished
pause
