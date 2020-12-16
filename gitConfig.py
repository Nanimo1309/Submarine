#!/usr/bin/python3

from subprocess import call
import sys

config =\
{
    "y": lambda: True,
    "Y": lambda: True,
    "d": lambda: False,
    "D": lambda: False
}.get(input("To config project [Y], to delete config [D]?: "), sys.exit)()

if config:
    def run(option, val):
        call(["git", "config", "--local", option, val])

else:
    def run(option, val):
        call(["git", "config", "--local", "--unset", option])

### User data
name = "";email = ""

if config:
    name = input("Input your name: ")
    email = input("Input your github email: ")

run("user.name", name)
run("user.email", email)
run("credential.https://github.com/.username", email)
run("credential.helper", "cache")

### Aliases
# Log
prettyFormat = "--pretty=\"format:%C(yellow bold)%h %C(green)%s %C(blue)%cn %C(magenta)<%cr> %C(yellow)%d\""
run("alias.l", "log " + prettyFormat)
run("alias.la", "log --all " + prettyFormat)
run("alias.lg", "log --graph " + prettyFormat)
run("alias.lga", "log --graph --all " + prettyFormat)

# Files
run("alias.s", "status -s")
run("alias.sl", "status")
run("alias.a", "add --all")
run("alias.c", "commit")
run("alias.cm", "commit -m")
run("alias.p", "push origin")

### Colors
run("color.status.added", "green bold")
run("color.status.changed", "yellow bold")
run("color.status.untracked", "red bold")