@REM Clean workspace and reset the status to head

@REM git clean
@REM
@REM   -d
@REM   Normally, when no <path> is specified, 
@REM   git clean will not recurse into untracked directories to avoid @REMoving too much.
@REM   Specify -d to have it recurse into such directories as well.
@REM   If any paths are specified, -d is irrelevant; 
@REM   all untracked files matching the specified paths (with exceptions for nested git directories mentioned under --force) will be @REMoved.
@REM
@REM   -f , --force
@REM   If the Git configuration variable clean.requireForce is not set to false, 
@REM   git clean will refuse to delete files or directories unless given -f or -i.
@REM   Git will refuse to modify untracked nested git repositories (directories with a .git subdirectory) unless a second -f is given.
@REM
@REM   -x
@REM   Don’t use the standard ignore rules (see gitignore(5)),
@REM   but still use the ignore rules given with -e options from the command line.
@REM   This allows @REMoving all untracked files, including build products.
@REM   This can be used (possibly in conjunction with git restore or git reset) to create a pristine working directory to test a clean build.
@REM

@REM git reset
@REM
@REM    --hard
@REM    Resets the index and working tree.
@REM    Any changes to tracked files in the working tree since <commit> are discarde

@git clean -fdx && git reset --hard

@call CreateShortcut.bat